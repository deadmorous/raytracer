#include "surfprop/simple_diffuse_surface.h"
#include "ray_tracer.h"
#include "ray.h"
#include "math_util.h"

namespace raytracer {

REGISTER_GENERATOR(SimpleDiffuseSurface)

SimpleDiffuseSurface::SimpleDiffuseSurface():
    m_color(mkv3f(1.f, 1.f, 1.f)),
    m_translucency(0.f)
{
}

void SimpleDiffuseSurface::processCollision(
        const Ray& ray,
        const SurfacePoint& surfacePoint,
        RayTracer& rayTracer) const

{
    // TODO: Refactor
    auto color = mkv3f(
            ray.color[0]*m_color[0],
            ray.color[1]*m_color[1],
            ray.color[2]*m_color[2]);

    v3f n = spnormal(surfacePoint);
    bool reflect;
    if (m_translucency == 0.f)
        reflect = true;
    else if (m_translucency == 1.f)
        reflect = false;
    else
        reflect = std::uniform_real_distribution<float>(0.f, 1.f)(rnd::gen()) > m_translucency;
    if ((dot(n, ray.dir) > 0) == reflect)
        n = -n;
    v3f dir = randomPointOnUnitSemiSphere(n);

    rayTracer.processRay(Ray(
        sppos(surfacePoint),
        dir,
        color,
        ray.generation+1));
//    origin=v.block< 3, 1 >
//        dir(dir),
//        generation(generation),
//        color(color)

}


void SimpleDiffuseSurface::read(const QVariant &v)
{
    m_color = mkv3f(1.f, 1.f, 1.f);
    m_translucency = 0.f;

    readOptionalProperty(m_color, v, "color");
    readOptionalProperty(m_translucency, v, "translucency");
}

} // end namespace raytracer
