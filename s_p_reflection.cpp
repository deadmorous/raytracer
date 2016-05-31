#include "s_p_reflection.h"
#include "ray_tracer.h"
#include "ray.h"

namespace raytracer {

s_p_reflection::s_p_reflection() :
    m_reflectivity(mkv3f(0.9f, 0.9f, 0.9f))
{
}

void s_p_reflection::processCollision(
        const Ray& ray,
        const SurfacePoint& surfacePoint,
        RayTracer& rayTracer) const

{
    auto n = spnormal(surfacePoint);
//    v3f color = fsmx::memberwise(ray.color, m_reflectivity,
//                                 [](float a, float b)->float {return a*b;});

    // TODO: Refactor
    v3f color = mkv3f(
            ray.color[0]*m_reflectivity[0],
            ray.color[1]*m_reflectivity[1],
            ray.color[2]*m_reflectivity[2]);

    rayTracer.processRay(Ray(
        sppos(surfacePoint),
        ray.dir - n*(2.f*(n.T()*ray.dir)),
        color,
        ray.generation+1));
//    origin=v.block< 3, 1 >
//        dir(dir),
//        generation(generation),
//        color(color)

}


v3f s_p_reflection::reflectivity() const
{
    return m_reflectivity;
}

void s_p_reflection::setReflectivity(const v3f&reflectivity)
{
     m_reflectivity=reflectivity;
}

} // end namespace raytracer
