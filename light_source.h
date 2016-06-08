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
class LightSource :
        public Readable,
        public FactoryMixin<LightSource>
{
public:
    LightSource();

    virtual void emitRays(quint64 count, RayTracer& rayTracer) const = 0;

    /// \brief Returns primitive transformation matrix.
    const m4f& transform() const;

    /// \brief Sets primitive transformation matrix.
    void setTransform(const m4f &transform);

    /// \brief Reads light source transformation.
    void read(const QVariant& v);

private:
    m4f m_transform;
};

} // end namespace raytracer

#endif // LIGHT_SOURCE_H

