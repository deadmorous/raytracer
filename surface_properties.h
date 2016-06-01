#ifndef SURFACE_PROPERTIES_H
#define SURFACE_PROPERTIES_H

/// \file
/// \brief Declaration of the SurfaceProperties interface.

#include "surface_point.h"
#include "factory.h"
#include "serial.h"

#include <memory>

namespace raytracer {

struct Ray;
class RayTracer;

struct SurfaceProperties :
        public Readable,
        public FactoryMixin<SurfaceProperties>
{
    virtual ~SurfaceProperties() {}
    virtual void processCollision(
            const Ray& ray,
            const SurfacePoint& surfacePoint,
            RayTracer& rayTracer) const = 0;
};

} // end namespace raytracer

#endif // SURFACE_PROPERTIES_H
