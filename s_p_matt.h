#ifndef S_P_MATT_H
#define S_P_MATT_H

#include "surface_point.h"
#include "surface_properties.h"
#include "common.h"



namespace raytracer {

struct Ray;
class RayTracer;

class s_p_matt : public SurfaceProperties
{
    s_p_matt();

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
#endif // S_P_MATT_H

