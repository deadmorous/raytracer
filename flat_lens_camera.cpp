/// \file
/// \brief Implementation of the FlatLensCamera class.

#include "flat_lens_camera.h"
#include "primitives/single_sided_rectangle.h"
#include "transform.h"
#include "ray_tracer.h"
#include "ray.h"

namespace raytracer {

namespace {

class CameraSurfProp : public SurfaceProperties
{
public:
    CameraSurfProp(
            const FlatLensCamera::Geometry& geom,
            float focusingDistance,
            Camera::Canvas& canvas, const m4f& transform) :
        m_geom(geom),
        m_focusingDistance(focusingDistance),
        m_canvas(canvas),
        m_transform(transform),
        m_invTransform(transform.inv()),
        m_ST(projectionMatrix() * m_invTransform)
    {
    }

    void processCollision(
            const Ray& ray,
            const SurfacePoint& surfacePoint,
            RayTracer& rayTracer) const
    {
        Q_UNUSED(rayTracer);

//        if (ray.generation == 0)
//            // Ignore direct rays from light source
//            return;

        auto rEye = m_invTransform*conv<v4f>(v3f(sppos(surfacePoint)));
        v2f rEyeScreen = rEye.block<2,1>(0,0) / (-rEye[2]);
        float x = rEyeScreen.norm2();
        v3f e;
        if (x == 0.f)
            e = -ray.dir;
        else {
            float gamma = atan(x/m_focusingDistance);
            v2f tau = rEyeScreen / x;
            v3f p = affine(m_transform) * mkv3f(-tau[1], tau[0], 0.f);
            e = rotation(p, -gamma)*(-ray.dir);
        }

        //*
        v2f re = conv<v2f>(m_ST * conv<v4f>(e));   // Screen coordinates of ray origin
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
    const FlatLensCamera::Geometry& m_geom;
    float m_focusingDistance;
    Camera::Canvas& m_canvas;
    const m4f& m_transform;
    m4f m_invTransform;

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

REGISTER_GENERATOR(FlatLensCamera)

FlatLensCamera::FlatLensCamera() : m_focusingDistance(1.f)
{
}

Primitive::Ptr FlatLensCamera::cameraPrimitive() const
{
    return m_primitive;
}

void FlatLensCamera::clear()
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
    m_primitive->setSurfaceProperties(
                std::make_shared<CameraSurfProp>(
                    m_geometry,
                    m_focusingDistance,
                    m_canvas,
                    transform()));
    if (!m_raysInputFileName.isEmpty())
        readRays(m_raysInputFileName);
}

const Camera::Canvas& FlatLensCamera::canvas() const {
    return m_canvas;
}

void FlatLensCamera::read(const QVariant &v)
{
    Camera::read(v);

    m_geometry = Geometry();
    m_filterImage = true;
    m_raysInputFileName.clear();
    m_focusingDistance = 1.f;

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
    readOptionalProperty(m_raysInputFileName, m, "read_rays");
    readOptionalProperty(m_focusingDistance, v, "focus_dist");
}

void FlatLensCamera::setGeometry(const Geometry& geometry)
{
    m_geometry = geometry;
    clear();
}

} // end namespace raytracer
