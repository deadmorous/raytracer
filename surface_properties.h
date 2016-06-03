/// \file
/// \brief Declaration of the SurfaceProperties interface.

#ifndef SURFACE_PROPERTIES_H
#define SURFACE_PROPERTIES_H

#include "surface_point.h"
#include "factory.h"
#include "serial.h"

namespace raytracer {

struct Ray;
class RayTracer;

struct SurfaceProperties :
        public Readable,
        public FactoryMixin<SurfaceProperties>
{
    virtual void processCollision(
            const Ray& ray,
            const SurfacePoint& surfacePoint,
            RayTracer& rayTracer) const = 0;
};

} // end namespace raytracer

#endif // SURFACE_PROPERTIES_H
