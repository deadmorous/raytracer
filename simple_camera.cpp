/// \file
/// \brief Implementation of the SimpleCamera class.

#include "simple_camera.h"
#include "primitives/rectangle.h"
#include "transform.h"
#include "ray_tracer.h"
#include "ray.h"

namespace raytracer {

namespace {

class CameraSurfProp : public SurfaceProperties
{
public:
    CameraSurfProp(const SimpleCamera::Geometry& geom, std::vector< v3f >& canvas, const m4f& transform) :
        m_geom(geom),
        m_canvas(canvas),
        m_transform(transform)
    {
        // Compute transformation from eye coordinates to screen coordinates
        v3x4f S = fsmx::zero<v3x4f>();
        float *Sd = S.data().data();
        Sd[0] = geom.dist * geom.resx / geom.screenWidth();
        Sd[2] = 0.5f * geom.resx;
        Sd[5] = geom.dist * geom.resy / geom.screenHeight();
        Sd[6] = 0.5f * geom.resy;
        Sd[10] = 1.f;
        m_ST = S * transform;
    }

    void processCollision(
            const Ray& ray,
            const SurfacePoint& surfacePoint,
            RayTracer& rayTracer) const
    {
        Q_UNUSED(rayTracer);
        Q_UNUSED(surfacePoint)

        v2f re = conv<v2f>(m_ST * conv<v4f>(ray.origin));   // Screen coordinates of ray origin
        int x = static_cast<int>(re[0]);
        int y = static_cast<int>(re[1]);
        if (x < 0   ||   x >= m_geom.resx   ||
            y < 0   ||   y >= m_geom.resy)
            return;
        int index = x + y*m_geom.resx;
        Q_ASSERT(index >= 0   &&   index < static_cast<int>(m_canvas.size()));
        m_canvas[index] += ray.color;
    }
    void read(const QVariant&) {}

private:
    const SimpleCamera::Geometry& m_geom;
    std::vector< v3f >& m_canvas;
    const m4f& m_transform;

    typedef fsmx::MX< fsmx::Data< 3, 4, float > > v3x4f;
    v3x4f m_ST; // Transformation from world coordinates to screen coordinates
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

QImage SimpleCamera::image() const
{
    QImage image(m_geometry.resx, m_geometry.resy, QImage::Format_RGB32);
    if (m_canvas.empty())
        return image;

    // Determine canvas color range
    auto minIntensity = m_canvas[0][0];
    auto maxIntensity = minIntensity;
    std::for_each(m_canvas.begin(), m_canvas.end(), [&minIntensity, &maxIntensity](const v3f& color) {
        for (int i=0; i< 3; ++i) {
            auto intensity = color[i];
            if (minIntensity > intensity)
                minIntensity = intensity;
            else if (maxIntensity < intensity)
                maxIntensity = intensity;
        }
    });

    // In case of monotonous color, return all-black or all-gray image
    if (minIntensity == maxIntensity) {
        image.fill(minIntensity == 0.f ?   0xff000000u :   0xff888888u);
        return image;
    }

    // Copy canvas pixels to the image
    uchar *bits = image.bits();
    std::for_each(m_canvas.begin(), m_canvas.end(), [&minIntensity, &maxIntensity, &bits](const v3f& color) {
        bits[3] = 0xff;
        for (int i=0; i<3; ++i) {
            auto normalizedIntensity = (color[i] - minIntensity) / (maxIntensity - minIntensity);
            bits[2-i] = static_cast<uchar>(normalizedIntensity * 255.999);
        }
        bits += 4;
    });
    return image;
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

    m_primitive->setSurfaceProperties(std::make_shared<CameraSurfProp>(m_geometry, m_canvas, transform()));
}

} // end namespace raytracer
