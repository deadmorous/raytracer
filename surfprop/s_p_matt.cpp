#include "surfprop/s_p_matt.h"
#include "ray_tracer.h"
#include "ray.h"

namespace raytracer {

MattSurface::MattSurface():
    m_mattsurf(mkv3f(0.9f, 0.9f, 0.9f))
{
}

void MattSurface::processCollision(
        const Ray& ray,
        const SurfacePoint& surfacePoint,
        RayTracer& rayTracer) const

{
    auto n = spnormal(surfacePoint);
//    v3f color = fsmx::memberwise(ray.color, m_reflectivity,
//                                 [](float a, float b)->float {return a*b;});

    float maxrd = 1.0;
    float minrd = -1.0;


   // for (int i=1; i<=3; ++i)
    //{
      // auto rdo(i) = minrd+(float)rand()*(maxrd-minrd)
   // }

   // auto s = sqrt(rdo(1)*rdo(1)+rdo(2)*rdo(2)+rdo(3)*rdo(3));
   // v3f nmod = mkv3f(rdo(1)/s,
                    // rdo(2)/s,
                    // rdo(3)/s);


   auto rd1 = minrd+(float)rand()/RAND_MAX*(maxrd-minrd);
   auto rd2 = minrd+(float)rand()/RAND_MAX*(maxrd-minrd);
   auto rd3 = minrd+(float)rand()/RAND_MAX*(maxrd-minrd);

   auto s = sqrt(rd1*rd1+rd2*rd2+rd3*rd3);

   v3f nmod = mkv3f(rd1/s,
                      rd2/s,
                      rd3/s);

    float nres = n*nmod;

    if (nres <= 0)
       nmod = mkv3f(0,0,0);

    // TODO: Refactor
    v3f color = mkv3f(
            ray.color[0]*m_mattsurf[0],
            ray.color[1]*m_mattsurf[1],
            ray.color[2]*m_mattsurf[2]);


    rayTracer.processRay(Ray(
        sppos(surfacePoint),
        ray.dir - nmod*(2.f*(nmod.T()*ray.dir)),
        color,
        ray.generation+1));
//    origin=v.block< 3, 1 >
//        dir(dir),
//        generation(generation),
//        color(color)

}


v3f MattSurface::mattsurf() const
{
    return m_mattsurf;
}

void MattSurface::setMattsurf(const v3f&mattsurf)
{
     m_mattsurf=mattsurf;
}

} // end namespace raytracer
