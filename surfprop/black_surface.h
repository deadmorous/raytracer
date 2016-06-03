/// \file
/// \brief Declaration of the BlackSurface class.

#ifndef BLACK_SURFACE_H
#define BLACK_SURFACE_H

#include "surface_properties.h"

namespace raytracer {

class BlackSurface : public SurfaceProperties
{
    DECL_GENERATOR(BlackSurface)
public:
    void processCollision(
        const Ray& ray,
        const SurfacePoint& surfacePoint,
        RayTracer& rayTracer) const;
    void read(const QVariant &v);
};

} // end namespace raytracer

#endif // BLACK_SURFACE_H
