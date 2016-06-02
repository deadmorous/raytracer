#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

/// \file
/// \brief Declaration of the LightSource interface.

#include "common.h"
#include "factory.h"
#include "serial.h"

namespace raytracer {

class RayTracer;

/// \brief The light source interface.
struct LightSource :
        public Readable,
        public FactoryMixin<LightSource>
{
public:
    typedef std::shared_ptr<LightSource> Ptr;
    virtual void emitRays(int count, RayTracer& rayTracer) const;
};

} // end namespace raytracer

#endif // LIGHT_SOURCE_H

