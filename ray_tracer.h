/// \file
/// \brief Declaration of the RayTracer class.

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"
#include "camera.h"
#include "primitive_search.h"
#include "serial.h"

namespace raytracer {

/// @brief Class responsible for the ray tracing algorithm in general.
class RayTracer :
        public Readable
{
public:
    struct Options
    {
        /// \brief Maximum total number of rays allowed.
        int totalRayLimit;

        /// \brief Ray reflection limit.
        int reflectionLimit;

        /// \brief Minimum ray component intensity required to process ray.
        float intensityThreshold;

        Options() :
            totalRayLimit(100000),
            reflectionLimit(10),
            intensityThreshold(0.1)
        {
        }

        Options& setTotalRayLimit(int x) {
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
    };

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



    /// \brief Starts ray tracing
    void run();

private:
    Scene m_scene;
    Camera::Ptr m_camera;
    Options m_options;

    PrimitiveSearch m_psearch;
};

} // end namespace raytracer

#endif // RAYTRACER_H
