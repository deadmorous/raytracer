#include "surfprop/s_p_matt.h"
#include "ray_tracer.h"
#include "ray.h"
#include "rnd.h"

namespace raytracer {

REGISTER_GENERATOR(MattSurface)

MattSurface::MattSurface():
    m_color(mkv3f(1.f, 1.f, 1.f))
{
}

void MattSurface::processCollision(
        const Ray& ray,
        const SurfacePoint& surfacePoint,
        RayTracer& rayTracer) const

{
    // TODO: Refactor
    auto color = mkv3f(
            ray.color[0]*m_color[0],
            ray.color[1]*m_color[1],
            ray.color[2]*m_color[2]);

    auto& gen = rnd::gen();
    std::uniform_real_distribution<float> d1(-1.f, 1.f), d2(0.f, 1.f);
    v3f dir;
    forever {
        dir = mkv3f(d1(gen), d1(gen), d2(gen));
        float dirNorm = dir.norm2();
        if (dirNorm == 0.f)
            continue;
        dir /= dirNorm;
        break;
    }

    v3f n = spnormal(surfacePoint);
    if (dot(n, ray.dir) > 0)
        n = -n;
    if (!(n[0] == 0.f && n[1] == 0.f)) {
        v3f p = n;
        p[2] -= 1;
        p /= p.norm2();
        dir -= p*(2*dot(p, dir));
    }

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


void MattSurface::read(const QVariant &v)
{
    m_color = mkv3f(1.f, 1.f, 1.f);
    readOptionalProperty(m_color, v, "color");
}

} // end namespace raytracer
