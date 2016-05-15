#ifndef SURFACE_PROPERTIES_H
#define SURFACE_PROPERTIES_H

/// \file
/// \brief Declaration of the SurfaceProperties interface.

#include "surface_point.h"

namespace raytracer {

struct Ray;
class RayTracer;

struct SurfaceProperties
{
    virtual ~SurfaceProperties() {}
    virtual void processCollision(
            const Ray& ray,
            const SurfacePoint& surfacePoint,
            RayTracer& rayTracer) const = 0;
};

} // end namespace raytracer

#endif // SURFACE_PROPERTIES_H
