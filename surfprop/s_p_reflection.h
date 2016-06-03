#ifndef REFLECTION_SURFACE_H
#define REFLECTION_SURFACE_H


#include "surface_point.h"
#include "surface_properties.h"
#include "common.h"

namespace raytracer {

struct Ray;
class RayTracer;

class ReflectionSurface : public SurfaceProperties
{
    DECL_GENERATOR(ReflectionSurface)
public:
    ReflectionSurface();

    void processCollision(
            const Ray& ray,
            const SurfacePoint& surfacePoint,
            RayTracer& rayTracer) const;
    void read(const QVariant &v);

    v3f reflectivity()const;
    void setReflectivity(const v3f&reflectivity);

private:
    v3f m_reflectivity;
};


} // end namespace raytracer
#endif // REFLECTION_SURFACE_H

