/// \file
/// \brief Declaration of the RayTracer class.

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"
#include "camera.h"
#include "primitive_search.h"
#include "serial.h"
#include "ray.h"
#include "image_processor.h"

#include <QTime>

// deBUG, TODO: Comment out
// #define DEBUG_RAY_BOUNCES

namespace raytracer {

/// @brief Class responsible for the ray tracing algorithm in general.
class RayTracer :
        public Readable
{
public:
    struct Options
    {
        /// \brief Maximum total number of rays allowed.
        quint64 totalRayLimit;

        /// \brief Ray reflection limit.
        int reflectionLimit;

        /// \brief Minimum ray component intensity required to process ray.
        float intensityThreshold;

        /// \brief Minimum ray parameter threshold for accepted collision.
        float rayParamThreshold;

        Options() :
            totalRayLimit(100000),
            reflectionLimit(10),
            intensityThreshold(0.1f),
            rayParamThreshold(1e-5f)
        {
        }

        Options& setTotalRayLimit(quint64 x) {
            totalRayLimit = x;
            return *this;
        }
        Options& setReflectionLimit(int x) {
            reflectionLimit = x;
            return *this;
        }
        Options& setIntensityThreshold(float x) {
            intensityThreshold = x;
            return *this;
        }
        Options& setRayParamThreshold(float x) {
            rayParamThreshold = x;
            return *this;
        }
    };
    typedef std::function<void(float, bool, quint64)> ProgressCallback;

    RayTracer();

    /// \brief Sets scene to be visualized and returns *this.
    RayTracer& setScene(const Scene& scene);

    /// \brief Returns scene being visualized.
    Scene scene() const;

    /// \brief Sets camera and returns *this.
    RayTracer& setCamera(const Camera::Ptr& camera);

    /// \brief Returns camera.
    Camera::Ptr camera() const;

    /// \brief Sets options and returns *this.
    RayTracer& setOptions(const Options& options);

    /// \brief Returns options.
    Options options() const;

    /// \brief Processes the ray specified.
    void processRay(const Ray& ray);

    /// \brief Reads scene and camera from variant
    void read(const QVariant& v);

    /// \brief Starts ray tracing.
    void run();

    /// \brief Requests the ray tracing to stop.
    void requestTermination();

    /// \brief Sets progress callback
    /// \param cb Callback to be called during the ray tracing process.
    /// The callback is given three arguments:
    ///   - progress in the range [0, 1];
    ///   - flag indicating the final call of the callback when the ray tracing finishes;
    ///   - total number of rays emitted.
    ///   .
    /// \param msecInterval Interval, in milliseconds, between successive callback call;
    /// \param raysGranularity Number of rays processed between successive time measurements.
    /// Low values can have an impact on the performance!
    void setProgressCallback(ProgressCallback cb, int msecInterval = 1000, quint64 raysGranularity = 100000);

    /// \brief Sets image processor
    void setImageProcessor(const ImageProcessor::Ptr& imageProcessor);

    /// \brief Returns image processor
    ImageProcessor::Ptr imageProcessor() const;

private:
    Scene m_scene;
    Camera::Ptr m_camera;
    ImageProcessor::Ptr m_imageProcessor;
    Options m_options;

    PrimitiveSearch m_psearch;
    struct CollisionData
    {
        const Primitive *primitive;
        SurfacePoint surfacePoint;
        float rayParam;
        bool operator<(const CollisionData& that) {
            return rayParam < that.rayParam;
        }
    };
    std::vector<CollisionData> m_collisionDataBuffer;
    typedef std::vector<CollisionData>::size_type SizeType;
    SizeType m_collisionDataBufferSize;
    template< class T >
    class ScopedBuf {
    public:
        explicit ScopedBuf(std::vector<T>& buf, SizeType& initialSize) : m_buf(buf), m_initialSize(initialSize), m_actualSize(initialSize) {}
        ~ScopedBuf() {
            Q_ASSERT(m_actualSize >= m_initialSize);
            m_actualSize = m_initialSize;
        }
        typename std::vector<T>::iterator begin() {
            return m_buf.begin() + m_initialSize;
        }
        typename std::vector<T>::iterator end() {
            return m_buf.begin() + m_actualSize;
        }
        bool empty() const {
            return m_actualSize == m_initialSize;
        }
        T& push() {
            SizeType s = m_buf.size();
            Q_ASSERT(s >= m_actualSize);
            if (s == m_actualSize)
                m_buf.resize(1 + (s << 1));
            ++m_actualSize;
            return m_buf[m_actualSize-1];
        }
        void push(const T& x) {
            push() = x;
        }
        T& pop() {
            Q_ASSERT(m_actualSize > m_initialSize);
            --m_actualSize;
            return m_buf[m_actualSize];
        }

    private:
        std::vector<T>& m_buf;
        SizeType m_initialSize;
        SizeType& m_actualSize;
    };
    ScopedBuf< CollisionData > collisionData() {
        return ScopedBuf< CollisionData >(m_collisionDataBuffer, m_collisionDataBufferSize);
    }

    quint64 m_lastRayNumber;
    ProgressCallback m_cb;
    int m_cbMsecInterval;
    quint64 m_cbRaysGranularity;
    QTime m_cbLastTime;
    class ScopedCallbackCaller;
    friend class ScopedCallbackCaller;
    bool m_terminationRequested;

#ifdef DEBUG_RAY_BOUNCES
    enum { DebugMaxRayBounceChains = 1000 };
    enum RayBounceChainDeathReason {
        RayBounceChainNoReason,
        RayBounceChainGoneAway,
        RayBounceChainReflectionLimitReached,
        RayBounceChainColorThresholdReached
    };
    struct RayBounceInfo {
        Ray ray;
        CollisionData cd;
        RayBounceChainDeathReason reason;
        RayBounceInfo() {}
        RayBounceInfo(const Ray& ray, const CollisionData& cd) : ray(ray), cd(cd), reason(RayBounceChainNoReason) {}
        RayBounceInfo(const Ray& ray, RayBounceChainDeathReason reason) : ray(ray), reason(reason) {
            cd.primitive = 0;
            cd.rayParam = -1;
            cd.surfacePoint.fill(0.f);
        }
    };
    typedef std::vector< RayBounceInfo > RayBounceChain;
    std::vector< RayBounceChain > m_rayBounceChains;
    void addRayBounceInfo(const RayBounceInfo& rbi);
#define ADD_RAY_BOUNCE_INFO(ray, cd) addRayBounceInfo(RayBounceInfo(ray, cd));
#define FINISH_RAY_BOUNCE_CHAIN(ray, reason) addRayBounceInfo(RayBounceInfo(ray, reason));
#else // DEBUG_RAY_BOUNCES
#define ADD_RAY_BOUNCE_INFO(ray, cd)
#define FINISH_RAY_BOUNCE_CHAIN(ray, reason)
#endif // DEBUG_RAY_BOUNCES
};

} // end namespace raytracer

#endif // RAYTRACER_H
