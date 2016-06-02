#include "simple_camera.h"

/// \file
/// \brief Implementation of the SimpleCamera class.

namespace raytracer {

REGISTER_GENERATOR(SimpleCamera)

SimpleCamera::SimpleCamera()
{

}

Primitive::Ptr SimpleCamera::cameraPrimitive() const
{
    return m_primitive;
}

QPixmap SimpleCamera::pixmap() const
{
    return m_pixmap;
}

void SimpleCamera::read(const QVariant &v)
{
    Camera::read(v);

    readOptionalProperty(v, "geometry", [this](const QVariant& v) {
        Geometry g;
        QVariantMap m = safeVariantMap(v);
        readOptionalProperty(g.fovy, m, "fovy");
        readOptionalProperty(g.aspect, m, "aspect");
        readOptionalProperty(g.dist, m, "dist");
        readOptionalProperty(g.resx, m, "resx");
        readOptionalProperty(g.resy, m, "resy");
        m_geometry = g;
    });
}

} // end namespace raytracer
