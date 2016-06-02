#include "camera.h"
#include "transform.h"

/// \file
/// \brief Implementation of the Camera class.

namespace raytracer {

Camera::Camera() :
    m_transform(fsmx::identity<m4f>())
{
}

const m4f &Camera::transform() const
{
    return m_transform;
}

void Camera::setTransform(const m4f& transform)
{
    m_transform = transform;
}

void Camera::read(const QVariant& v)
{
    m_transform = fsmx::identity<m4f>();
    Transform::Ptr t;
    if (readOptionalTypedProperty(t, v, "transform"))
        (*t)(m_transform);
}

} // end namespace raytracer
