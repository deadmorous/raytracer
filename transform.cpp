/// \file
/// \brief Implementation of transformation matrix utility classes.

#include "transform.h"

namespace raytracer {

REGISTER_GENERATOR(Translate)

Translate::Translate() :
    m_translation(fsmx::zero<v3f>())
{
}

Translate::Translate(const v3f& translation) :
    m_translation(translation)
{
}

void Translate::read(const QVariant& v)
{
    m_translation = fromVariant<v3f>(v);
}

void Translate::operator()(m4f& transform) const
{
    translation(transform) += affine(transform)*m_translation;
}



REGISTER_GENERATOR(Rotate)

Rotate::Rotate() : m_axis(mkv3f(0.f, 0.f, 1.f)), m_angle(0.f) {}

Rotate::Rotate(const v3f &axis, float degrees) : m_axis(axis), m_angle(degrees) {}

void Rotate::read(const QVariant& v)
{
    QVariantMap m = safeVariantMap(v);
    readProperty(m_axis, m, "axis");
    readProperty(m_angle, m, "angle");
}

void Rotate::operator()(m4f& transform) const
{
    affine(transform) *= rotation(m_axis, deg2rad(m_angle));
}



REGISTER_GENERATOR(Scale)

void Scale::read(const QVariant& v)
{
    if (v.type() == QVariant::List)
        m_scale = fromVariant<v3f>(v);
    else if (v.canConvert<float>())
        m_scale.fill(v.value<float>());
    else {
        Q_ASSERT(false);
    }
}

void Scale::operator()(m4f& transform) const
{
    auto s = fsmx::zero<m4f>();
    s.diag() = conv<v4f>(m_scale);
    transform *= s;
}



REGISTER_GENERATOR(CombinedTransform)

void CombinedTransform::read(const QVariant& v)
{
    readTypedInstances(m_transformations, v);
}

void CombinedTransform::operator()(m4f& transform) const
{
    std::for_each(m_transformations.begin(), m_transformations.end(), [&transform](const Transform::Ptr& item) {
        (*item)(transform);
    });
}

} // end namespace raytracer
