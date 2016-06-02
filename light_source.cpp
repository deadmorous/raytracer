/// \file
/// \brief Declaration of LightSource methods.

#include "light_source.h"
#include "transform.h"

namespace raytracer {

LightSource::LightSource() :
    m_transform(fsmx::identity<m4f>())
{
}

const m4f &LightSource::transform() const
{
    return m_transform;
}

void LightSource::setTransform(const m4f& transform)
{
    m_transform = transform;
}

void LightSource::read(const QVariant& v)
{
    m_transform = fsmx::identity<m4f>();
    Transform::Ptr t;
    if (readOptionalTypedProperty(t, v, "transform"))
        (*t)(m_transform);
}

} // end namespace raytracer
