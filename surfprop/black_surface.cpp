/// \file
/// \brief Implementation of the BlackSurface class.

#include "black_surface.h"

namespace raytracer {

REGISTER_GENERATOR(BlackSurface)

void BlackSurface::processCollision(
    const Ray& ray,
    const SurfacePoint& surfacePoint,
    RayTracer& rayTracer) const
{
    // Just consume the ray and do not emit any more rays
    Q_UNUSED(ray);
    Q_UNUSED(surfacePoint);
    Q_UNUSED(rayTracer);
}

void BlackSurface::read(const QVariant &v)
{
    Q_UNUSED(v);
    // No parameters
}

} // end namespace raytracer
