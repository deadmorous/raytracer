/// \file
/// \brief Implementation of the RayTracer class.

#include "ray_tracer.h"
#include "surface_properties.h"

namespace raytracer {

RayTracer::RayTracer()
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
    auto r = m_psearch.find(ray);
    for (auto it=r.begin; it!=r.end; ++it) {
        const Primitive* p = *it;
        SurfacePoint sp;
        if (p->collisionTest(sp, ray))
        {
            p->surfaceProperties()->processCollision(ray, sp, *this);
            break;
        }
    }
}

void RayTracer::read(const QVariant& v)
{
    m_options = Options();

    QVariantMap m = safeVariantMap(v);
    m_scene.read(readProperty(m, "scene"));
    readTypedProperty(m_camera, m, "camera");
    readOptionalProperty(m, "options", [this](const QVariant& v) {
        QVariantMap m = safeVariantMap(v);
        readOptionalProperty(m_options.totalRayLimit, m, "max_rays");
        readOptionalProperty(m_options.reflectionLimit, m, "max_reflactions");
        readOptionalProperty(m_options.intensityThreshold, m, "intensity_threshold");
    });
}

void RayTracer::run()
{
    // Prepare the search structure
    m_psearch = PrimitiveSearch();
    for (Primitive::Ptr p : m_scene.primitives())
        m_psearch.add(p.get());
    if (m_camera)
        m_psearch.add(m_camera->cameraPrimitive().get());


    auto lights = m_scene.lightSources();

    if (lights.empty())
        // No light sources, nothing to do
        return;

    // Compute rays per light
    int typicalRayGenerationCount = std::max(1, m_options.reflectionLimit / 3); // TODO better
    int raysPerLight = m_options.totalRayLimit / (lights.size() * typicalRayGenerationCount);
    if (raysPerLight < 1)
        // Zero rays per light, nothing to do
        return;

    // Emit rays from light sources
    foreach (const LightSource::Ptr& light, lights) {
        light->emitRays(raysPerLight, *this);
    }
}

} // end namespace raytracer
