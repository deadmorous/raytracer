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
            Camera::Canvas& canvas,
            const m4f& transform) :
        m_geom(geom),
        m_canvas(canvas),
        m_transform(transform),
        m_invTransform(transform.inv())
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
        v2f rScreen = rEye.block<2,1>(0,0);
        float x = rScreen.norm2();
        v3f e = affine(m_invTransform)*ray.dir;
        // The direction of the refracted ray
        if (x != 0.f) {
            float alpha = atan(x/m_geom.R);
            v3f tau = mkv3f(rScreen[0]/x, rScreen[1]/x, 0.f);
            v3f n = tau*static_cast<float>(sin(alpha));
            n[2] = static_cast<float>(-cos(alpha));

            v3f q = cross(n, e);
            float sinIncomingTheta = q.norm2();
            if (sinIncomingTheta != 0.f) {
                q /= sinIncomingTheta;
                float sinOutcomingTheta = sinIncomingTheta/m_geom.refractionCoefficient;
                e = rotation(q, asin(sinIncomingTheta) - asin(sinOutcomingTheta))*e;
            }
        }

        float rayParamOnMatrix = m_geom.fx / e[2];
        v2f rMatrix = rScreen + rayParamOnMatrix*e.block<2,1>(0,0);
        auto xy = mkv2i(
            static_cast<int>((0.5f - rMatrix[0]/m_geom.matrixWidth) * m_geom.resx),
            static_cast<int>((0.5f - rMatrix[1]/m_geom.matrixHeight) * m_geom.resy));
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
    Camera::Canvas& m_canvas;
    const m4f& m_transform;
    m4f m_invTransform;
};

} // anonymous namespace

REGISTER_GENERATOR(FlatLensCamera)

Primitive::Ptr FlatLensCamera::cameraPrimitive() const
{
    return m_primitive;
}

void FlatLensCamera::clear()
{
    m_canvas = Canvas(mkv2i(m_geometry.resx, m_geometry.resy));

    m_primitive = std::make_shared<SingleSidedRectangle>(
                m_geometry.screenWidth,
                m_geometry.screenHeight
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
    m_raysInputFileName.clear();

    QVariantMap m = safeVariantMap(v);
    readOptionalProperty(m, "geometry", [this](const QVariant& v) {
        Geometry g;
        QVariantMap m = safeVariantMap(v);
        readOptionalProperty(g.fovy, m, "fovy");
        readOptionalProperty(g.aspect, m, "aspect");
        readOptionalProperty(g.dist, m, "dist");
        readOptionalProperty(g.resx, m, "resx");
        readOptionalProperty(g.resy, m, "resy");
        readOptionalProperty(g.refractionCoefficient, m, "refraction_coeff");
        readOptionalProperty(g.focusingDistance, m, "focus_dist");
        m_geometry = g;
        m_geometry.computeValues();
    });
    readOptionalProperty(m_raysInputFileName, m, "read_rays");

}

void FlatLensCamera::setGeometry(const Geometry& geometry)
{
    m_geometry = geometry;
    m_geometry.computeValues();
    clear();
}

} // end namespace raytracer
