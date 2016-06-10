/// \file
/// \brief Implementation of the SimpleCamera class.

#include "simple_camera.h"
#include "primitives/single_sided_rectangle.h"
#include "transform.h"
#include "ray_tracer.h"
#include "ray.h"

namespace raytracer {

namespace {

class CameraSurfProp : public SurfaceProperties
{
public:
    CameraSurfProp(const SimpleCamera::Geometry& geom, Camera::Canvas& canvas, const m4f& transform) :
        m_geom(geom),
        m_canvas(canvas),
        m_transform(transform),
        m_ST(projectionMatrix() * transform.inv())
    {
    }

    void processCollision(
            const Ray& ray,
            const SurfacePoint& surfacePoint,
            RayTracer& rayTracer) const
    {
        Q_UNUSED(rayTracer);
        Q_UNUSED(surfacePoint)

//        if (ray.generation == 0)
//            // Ignore direct rays from light source
//            return;

        //*
        v2f re = conv<v2f>(m_ST * conv<v4f>(ray.origin));   // Screen coordinates of ray origin
        /*/
        // No lens
        v2f re = sptex(surfacePoint);
        re[0] = 0.5*(re[0]+1) * m_geom.resx;
        re[1] = 0.5*(re[1]+1) * m_geom.resy;
        //*/

        auto xy = mkv2i(static_cast<int>(re[0]), static_cast<int>(re[1]));  // TODO better
        if (!m_canvas.contains(xy))
            return;

        //*
        m_canvas[xy] += ray.color;
        /*/
        auto& pixel = m_canvas[index];
        for (int i=0; i<3; ++i)
            pixel[i] = std::max(pixel[i], ray.color[i]);
        //*/
    }
    void read(const QVariant&) {}

private:
    const SimpleCamera::Geometry& m_geom;
    Camera::Canvas& m_canvas;
    const m4f& m_transform;

    typedef fsmx::MX< fsmx::Data< 3, 4, float > > m3x4f;
    m3x4f m_ST; // Transformation from world coordinates to screen coordinates
    m3x4f projectionMatrix() const
    {
        // Compute transformation from eye coordinates to screen coordinates
        m3x4f S = fsmx::zero<m3x4f>();
        float *Sd = S.data().data();
        Sd[0] = -m_geom.dist * m_geom.resx / m_geom.screenWidth();
        Sd[2] = 0.5f * m_geom.resx;
        Sd[5] = -m_geom.dist * m_geom.resy / m_geom.screenHeight();
        Sd[6] = 0.5f * m_geom.resy;
        Sd[10] = 1.f;
        return S;
    }
};

} // anonymous namespace

REGISTER_GENERATOR(SimpleCamera)

SimpleCamera::SimpleCamera() :
    m_filterImage(true)
{
}

Primitive::Ptr SimpleCamera::cameraPrimitive() const
{
    return m_primitive;
}

void SimpleCamera::clear()
{
    m_canvas = Canvas(mkv2i(m_geometry.resx, m_geometry.resy));

    m_primitive = std::make_shared<SingleSidedRectangle>(
                m_geometry.screenWidth(),
                m_geometry.screenHeight()
                );

    m4f T = transform();
    Rotate(mkv3f(0.f, 1.f, 0.f), 180.f)(T);
    v3f axis = affine(T).col(2);
    axis /= axis.norm2();
    Translate(-axis*m_geometry.dist)(T);
    m_primitive->setTransform(T);

    m_primitive->setName("camera screen");

    m_primitive->setSurfaceProperties(std::make_shared<CameraSurfProp>(m_geometry, m_canvas, transform()));
}

const Camera::Canvas& SimpleCamera::canvas() const {
    return m_canvas;
}

void SimpleCamera::read(const QVariant &v)
{
    Camera::read(v);

    m_geometry = Geometry();
    m_filterImage = true;

    QVariantMap m = safeVariantMap(v);
    readOptionalProperty(m, "geometry", [this](const QVariant& v) {
        Geometry g;
        QVariantMap m = safeVariantMap(v);
        readOptionalProperty(g.fovy, m, "fovy");
        readOptionalProperty(g.aspect, m, "aspect");
        readOptionalProperty(g.dist, m, "dist");
        readOptionalProperty(g.resx, m, "resx");
        readOptionalProperty(g.resy, m, "resy");
        m_geometry = g;
    });
    readOptionalProperty(m_filterImage, m, "filter_image");

    clear();
}

} // end namespace raytracer
