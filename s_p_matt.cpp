#include "s_p_matt.h"
#include "ray_tracer.h"
#include "ray.h"

namespace raytracer {

s_p_matt::s_p_matt():
    m_mattsurf(mkv3f(0.9f, 0.9f, 0.9f))
{
}

void s_p_matt::processCollision(
        const Ray& ray,
        const SurfacePoint& surfacePoint,
        RayTracer& rayTracer) const

{
    auto n = spnormal(surfacePoint);
//    v3f color = fsmx::memberwise(ray.color, m_reflectivity,
//                                 [](float a, float b)->float {return a*b;});
    auto n1=n;

    // TODO: Refactor
    v3f color = mkv3f(
            ray.color[0]*m_mattsurf[0],
            ray.color[1]*m_mattsurf[1],
            ray.color[2]*m_mattsurf[2]);

    rayTracer.processRay(Ray(
        sppos(surfacePoint),
        ray.dir - n1*(2.f*(n1.T()*ray.dir)),
        color,
        ray.generation+1));
//    origin=v.block< 3, 1 >
//        dir(dir),
//        generation(generation),
//        color(color)

}


v3f s_p_matt::mattsurf() const
{
    return m_mattsurf;
}

void s_p_matt::setMattsurf(const v3f&mattsurf)
{
     m_mattsurf=mattsurf;
}

} // end namespace raytracer
