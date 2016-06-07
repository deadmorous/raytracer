/// \file
/// \brief Implements methods of the base class for scene primitive.

#include "primitive.h"
#include "transform.h"
#include "bounding_sphere.h"
#include "surfprop/black_surface.h"

namespace raytracer {

Primitive::Primitive() :
    m_transform(fsmx::identity<m4f>())
{
}

BoundingSphere Primitive::transformBoundingSphere(const BoundingSphere& bs) const
{
    Q_ASSERT(!hasShear(affine(m_transform)));
    return BoundingSphere(
                bs.center + translation(m_transform),
                bs.radius * scalingFactor(affine(m_transform)));
}

QString Primitive::name() const
{
    return m_name;
}

void Primitive::setName(const QString& name)
{
    m_name = name;
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

    m_name = QString();
    m_transform = fsmx::identity<m4f>();
    m_surfaceProperties.reset();

    QVariantMap m = safeVariantMap(v);

    readOptionalProperty(m_name, m, "name");

    if (!readOptionalTypedProperty(m_surfaceProperties, m, "surf_prop"))
        m_surfaceProperties = std::make_shared<BlackSurface>();

    Transform::Ptr t;
    if (readOptionalTypedProperty(t, m, "transform"))
        (*t)(m_transform);
}

} // end namespace raytracer
