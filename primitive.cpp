#include "primitive.h"

/// \file
/// \brief Implements methods of the base class for scene primitive.

namespace raytracer {

void Primitive::setSurfaceProperties(const SurfaceProperties::Ptr& surfaceProperties)
{
    m_surfaceProperties = surfaceProperties;
}

SurfaceProperties::Ptr Primitive::surfaceProperties() const
{
    return m_surfaceProperties;
}

void Primitive::read(const QVariant& v)
{
    readOptionalTypedProperty(m_surfaceProperties, v, "surf_prop");
}

} // end namespace raytracer
