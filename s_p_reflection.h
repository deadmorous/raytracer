#ifndef S_P_REFLECTION
#define S_P_REFLECTION


#include "surface_point.h"
#include "surface_properties.h"
#include "common.h"

namespace raytracer {

struct Ray;
class RayTracer;

class s_p_reflection : public SurfaceProperties
{
    s_p_reflection();

    void processCollision(
            const Ray& ray,
            const SurfacePoint& surfacePoint,
            RayTracer& rayTracer) const;

    v3f reflectivity()const;
    void setReflectivity(const v3f&reflectivity);

private:
    v3f m_reflectivity;
};


} // end namespace raytracer
#endif // S_P_REFLECTION

