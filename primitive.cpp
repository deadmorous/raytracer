/// \file
/// \brief Implements methods of the base class for scene primitive.

#include "primitive.h"
#include "transform.h"
#include "bounding_sphere.h"

namespace raytracer {

Primitive::Primitive() :
    m_transform(fsmx::identity<m4f>())
{
}

BoundingSphere Primitive::transformBoundingSphere(const BoundingSphere& bs) const
{
    Q_ASSERT(!hasShear(m_transform));
    return BoundingSphere(
                bs.center + translation(m_transform),
                bs.radius * scalingFactor(m_transform));
}

const m4f &Primitive::transform() const
{
    return m_transform;
}

void Primitive::setTransform(const m4f& transform)
{
    m_transform = transform;
}

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
    using namespace std::placeholders;

    m_transform = fsmx::identity<m4f>();
    m_surfaceProperties.reset();

    QVariantMap m = safeVariantMap(v);
    readOptionalTypedProperty(m_surfaceProperties, m, "surf_prop");

    Transform::Ptr t;
    if (readOptionalTypedProperty(t, m, "transform"))
        (*t)(m_transform);
}

} // end namespace raytracer
