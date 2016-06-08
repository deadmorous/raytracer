#ifndef MATT_SURFACE_H
#define MATT_SURFACE_H

#include "surface_point.h"
#include "surface_properties.h"
#include "common.h"



namespace raytracer {

struct Ray;
class RayTracer;

class MattSurface : public SurfaceProperties
{
    MattSurface();

    void processCollision(
            const Ray& ray,
            const SurfacePoint& surfacePoint,
            RayTracer& rayTracer) const;

    v3f mattsurf()const;
    void setMattsurf(const v3f&mattsurf);

private:
    v3f m_mattsurf;
};


} // end namespace raytracer
#endif // MATT_SURFACE_H

