#include "ray_tracer.h"
#include "surface_properties.h"

/// \file
/// \brief Implementation of the RayTracer class.

namespace raytracer {

RayTracer::RayTracer()
{

}

/// \brief Sets scene to be visualized and returns *this.
RayTracer& RayTracer::setScene(const Scene& scene)
{
    m_scene = scene;
    m_psearch = PrimitiveSearch();
    for (Primitive::Ptr p : m_scene.primitives())
        m_psearch.add(p.get());
    return *this;
}

/// \brief Returns scene being visualized.
Scene RayTracer::scene() const
{
    return m_scene;
}

/// \brief Sets camera and returns *this.
RayTracer& RayTracer::setCamera(const Camera& camera)
{
    m_camera = camera;
    return *this;
}

/// \brief Returns camera.
Camera RayTracer::camera() const
{
    return m_camera;
}

/// \brief Processes the ray specified.
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

} // end namespace raytracer

