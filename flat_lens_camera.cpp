/// \file
/// \brief Implementation of the FlatLensCamera class.

#include "flat_lens_camera.h"
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

REGISTER_GENERATOR(FlatLensCamera)

void FlatLensCamera::clear()
{
    SimpleCamera::clear();
    cameraPrimitive()->setSurfaceProperties(std::make_shared<CameraSurfProp>(geometry(), canvasRef(), transform()));
}

} // end namespace raytracer
