/// \file
/// \brief Implementation of the RayTracer class.

#include "ray_tracer.h"
#include "surface_properties.h"
#include "cxx_exception.h"

#ifdef DEBUG_RAY_BOUNCES
#include <QDebug>
#endif // DEBUG_RAY_BOUNCES

namespace raytracer {

class RayTracer::ScopedCallbackCaller
{
public:
    ScopedCallbackCaller(RayTracer& rt) : m_rt(rt) {}
    ~ScopedCallbackCaller() {
        if (m_rt.m_cbMsecInterval > 0   &&
                m_rt.m_lastRayNumber % m_rt.m_cbRaysGranularity == 0) {
            if (m_rt.m_cbLastTime.elapsed() >= m_rt.m_cbMsecInterval)  {
                float progress = static_cast<float>(m_rt.m_lastRayNumber) / m_rt.m_options.totalRayLimit;
                m_rt.m_cb(progress, false, m_rt.m_lastRayNumber);
                m_rt.m_cbLastTime.restart();
            }
        }
    }
private:
    RayTracer& m_rt;
};



CTM_DECL_EXCEPTION(RayTracerTerminationException, cxx::exception)



RayTracer::RayTracer() :
    m_imageProcessor(IdentityImageProcessor::newInstance()),
    m_collisionDataBufferSize(0),
    m_lastRayNumber(0),
    m_cbMsecInterval(0),
    m_cbRaysGranularity(100000),
    m_terminationRequested(false)
{
}

RayTracer& RayTracer::setScene(const Scene& scene)
{
    m_scene = scene;
    return *this;
}

Scene RayTracer::scene() const
{
    return m_scene;
}

RayTracer& RayTracer::setCamera(const Camera::Ptr &camera)
{
    m_camera = camera;
    return *this;
}

Camera::Ptr RayTracer::camera() const
{
    return m_camera;
}

RayTracer& RayTracer::setOptions(const Options& options) {
    m_options = options;
    return *this;
}

RayTracer::Options RayTracer::options() const {
    return m_options;
}

void RayTracer::processRay(const Ray& ray)
{
    if (m_terminationRequested)
        throw RayTracerTerminationException();

    if (m_lastRayNumber >= m_options.totalRayLimit)
        return;

    ++m_lastRayNumber;
    ScopedCallbackCaller scc(*this);

    if (ray.generation > m_options.reflectionLimit) {
        // No collisions occurred
        FINISH_RAY_BOUNCE_CHAIN(ray, RayBounceChainReflectionLimitReached);
        return;
    }
    if (ray.color[0] + ray.color[1] + ray.color[2] < m_options.intensityThreshold) {
        FINISH_RAY_BOUNCE_CHAIN(ray, RayBounceChainColorThresholdReached);
        return;
    }

    // Find candidates for collisions
    auto r = m_psearch.find(ray);

    // Check for collision; collect positive results in cdbuf
    auto cdbuf = collisionData();
    for (auto it=r.begin; it!=r.end; ++it) {
        // Allocate buffer element if there's no collision
        CollisionData& cd = cdbuf.push();

        // Check for collision
        cd.primitive = *it;
        bool hasCollision = cd.primitive->collisionTest(cd.rayParam, cd.surfacePoint, ray);

        // Discard collision if it occurs too close to ray origin
        // (that could mean collision with object just emitted the ray)
        if (hasCollision && cd.rayParam < m_options.rayParamThreshold)
            hasCollision = false;

        // Deallocate buffer element if there's no collision
        if (!hasCollision)
            cdbuf.pop();
    }

    if (cdbuf.empty()) {
        // No collisions occurred
        FINISH_RAY_BOUNCE_CHAIN(ray, RayBounceChainGoneAway);
        return;
    }

    // Process nearest collision
    const CollisionData& cd0 = *std::min_element(cdbuf.begin(), cdbuf.end());
    ADD_RAY_BOUNCE_INFO(ray, cd0)
    cd0.primitive->surfaceProperties()->processCollision(ray, cd0.surfacePoint, *this);
}

#ifdef DEBUG_RAY_BOUNCES
void RayTracer::addRayBounceInfo(const RayBounceInfo& rbi)
{
    auto formatv3f = [](const v3f& v) -> QString {
        return QString("[%1, %2, %3]").arg(v[0]).arg(v[1]).arg(v[2]);
    };
    auto formatHexByte = [](float v) -> QString {
        int x = static_cast<int>(v * 255.999);
        if (x < 0   ||   x > 255)
            return "??";
        QString result;
        if (x < 0x10)
            result += "0";
        result += QString::number(x, 16);
        return result;
    };
    auto formatColor = [formatHexByte](const v3f& v) -> QString {
        return QString("#%1%2%3")
                .arg(formatHexByte(v[0]))
                .arg(formatHexByte(v[1]))
                .arg(formatHexByte(v[2]));
    };
    auto formatRayBounceChainDeathReason = [](const RayBounceChainDeathReason& reason) -> QString {
        switch (reason) {
        case RayBounceChainNoReason: return "no reason";
        case RayBounceChainGoneAway: return "gone away";
        case RayBounceChainReflectionLimitReached: return "reflection limit reached";
        case RayBounceChainColorThresholdReached: return "color threshold reached";
        }
        return "no reason";
    };

    if (m_rayBounceChains.size() > DebugMaxRayBounceChains)
        return;
    if (rbi.ray.generation == 0) {
        if (m_rayBounceChains.size() == DebugMaxRayBounceChains) {
            foreach (const RayBounceChain& chain, m_rayBounceChains) {
                QStringList items;
                Q_ASSERT(!chain.empty());
                foreach (const RayBounceInfo& item, chain) {
                    if (item.cd.primitive)
                        items << QString("%1: @%2 >%3 %4 => %5 @%6 >%7").arg(
                                     QString::number(item.ray.generation),
                                     formatv3f(item.ray.origin),
                                     formatv3f(item.ray.dir),
                                     formatColor(item.ray.color),
                                     item.cd.primitive->name(),
                                     formatv3f(sppos(item.cd.surfacePoint)),
                                     formatv3f(spnormal(item.cd.surfacePoint)));
                    else
                        items << QString("%1: @%2 >%3 %4 =| (%5)").arg(
                                     QString::number(item.ray.generation),
                                     formatv3f(item.ray.origin),
                                     formatv3f(item.ray.dir),
                                     formatColor(item.ray.color),
                                     formatRayBounceChainDeathReason(item.reason));
                }
                qDebug().noquote() << items.join("\n    ");
            }
        }
        m_rayBounceChains.push_back(RayBounceChain());
    }
    Q_ASSERT(!m_rayBounceChains.empty());
    m_rayBounceChains.rbegin()->push_back(rbi);
}
#endif // DEBUG_RAY_BOUNCES

void RayTracer::read(const QVariant& v)
{
    m_options = Options();

    QVariantMap m = safeVariantMap(v);
    m_scene.read(readProperty(m, "scene"));
    readTypedProperty(m_camera, m, "camera");
    readOptionalTypedProperty(m_imageProcessor, m, "imgproc");
    readOptionalProperty(m, "options", [this](const QVariant& v) {
        QVariantMap m = safeVariantMap(v);
        readOptionalProperty(m_options.totalRayLimit, m, "max_rays");
        readOptionalProperty(m_options.reflectionLimit, m, "max_reflections");
        readOptionalProperty(m_options.intensityThreshold, m, "intensity_threshold");
        readOptionalProperty(m_options.rayParamThreshold, m, "ray_param_threshold");
    });
}

void RayTracer::run()
{
    // Reset ray counter
    m_lastRayNumber = 0;

    // Prepare the search structure
    m_psearch = PrimitiveSearch();
    for (Primitive::Ptr p : m_scene.primitives())
        m_psearch.add(p.get());
    if (m_camera)
    {
        m_camera->clear();
        m_psearch.add(m_camera->cameraPrimitive().get());
    }

    auto lights = m_scene.lightSources();

    if (lights.empty())
        // No light sources, nothing to do
        return;

    // Compute rays per light
    quint64 typicalRayGenerationCount = std::max(1, m_options.reflectionLimit / 3); // TODO better
    quint64 raysPerLight = m_options.totalRayLimit / (lights.size() * typicalRayGenerationCount);
    if (raysPerLight < 1)
        // Zero rays per light, nothing to do
        return;

    // Start counting the time
    m_cbLastTime.start();

    // Clear termination request flag
    m_terminationRequested = false;

    try {
        // Emit rays from light sources
        foreach (const LightSource::Ptr& light, lights) {
            light->emitRays(raysPerLight, *this);
        }
    }
    catch (const RayTracerTerminationException&)
    {
    }

    if (m_cbMsecInterval > 0)
        // Invoke the progress callback last time
        m_cb(1.0f, true, m_lastRayNumber);
}

void RayTracer::requestTermination()
{
    m_terminationRequested = true;
}

void RayTracer::setProgressCallback(ProgressCallback cb, int msecInterval, quint64 raysGranularity)
{
    Q_ASSERT(cb);
    m_cb = cb;
    m_cbMsecInterval = msecInterval;
    m_cbRaysGranularity = raysGranularity;
}

void RayTracer::setImageProcessor(const ImageProcessor::Ptr& imageProcessor)
{
    m_imageProcessor = imageProcessor;
}

ImageProcessor::Ptr RayTracer::imageProcessor() const
{
    return m_imageProcessor;
}

} // end namespace raytracer
