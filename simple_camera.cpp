/// \file
/// \brief Implementation of the SimpleCamera class.

#include "simple_camera.h"
#include "primitives/rectangle.h"
#include "transform.h"

namespace raytracer {

namespace {

class CameraSurfProp : public SurfaceProperties
{
public:
    CameraSurfProp(const SimpleCamera::Geometry& geom, std::vector< v3f >& canvas) :
        m_geom(geom),
        m_canvas(canvas)
    {
    }

    void processCollision(
            const Ray& ray,
            const SurfacePoint& surfacePoint,
            RayTracer& rayTracer) const
    {
        Q_UNUSED(rayTracer);
        Q_UNUSED(rayTracer)
        auto tex = sptex(surfacePoint);
    }
    void read(const QVariant &v) {}

private:
    const SimpleCamera::Geometry& m_geom;
    std::vector< v3f >& m_canvas;
};

} // anonymous namespace

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

    m_geometry = Geometry();
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

    m_canvas.resize(m_geometry.resx * m_geometry.resy);
    std::fill(m_canvas.begin(), m_canvas.end(), fsmx::zero<v3f>());

    m_primitive = std::make_shared<Rectangle>(
                m_geometry.screenWidth(),
                m_geometry.screenHeight()
                );

    m4f T = transform();
    v3f axis = affine(T).col(2);
    axis /= axis.norm2();
    Translate(-axis*m_geometry.dist)(T);
    m_primitive->setTransform(T);

    m_primitive->setSurfaceProperties(std::make_shared<CameraSurfProp>(m_geometry, m_canvas));
}

} // end namespace raytracer
