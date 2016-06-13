/// \file
/// \brief Declaration of the FlatLensCamera class.

#ifndef FLATLENSCAMERA_H
#define FLATLENSCAMERA_H

#include "camera.h"

namespace raytracer {

class FlatLensCamera : public Camera
{
    DECL_GENERATOR(FlatLensCamera)
public:
    struct Geometry
    {
        /// \brief Specifies the field of view angle, in degrees, in the y direction.
        ///
        /// \sa gluPerspective().
        float fovy;
        /// \brief Specifies the aspect ratio that determines the field of view in the y direction.
        ///
        /// The aspect ratio is the ratio of x (width) to y (height).
        /// \sa gluPerspective().
        float aspect;

        /// \brief Distance from the origin of the camera to the camera screen.
        float dist;

        /// \brief Camera screen resolution in the x direction.
        int resx;

        /// \brief Camera screen resolution in the y direction.
        int resy;
        Geometry() : fovy(90.f), aspect(16.f/9.f), dist(1.f), resx(1600), resy(900) {}
        Geometry(
                float fovy,
                float aspect,
                float dist,
                int resx,
                int resy) :
            fovy(fovy),
            aspect(aspect),
            dist(dist),
            resx(resx),
            resy(resy)
        {}

        float screenWidth() const {
            return screenHeight() * aspect;
        }

        float screenHeight() const {
            return dist * 2.f*sin(0.5f*deg2rad(fovy));
        }
    };

    FlatLensCamera();

    void clear();

    Primitive::Ptr cameraPrimitive() const;

    const Canvas& canvas() const;

    void read(const QVariant &v);

    const Geometry& geometry() const;
    void setGeometry(const Geometry& geometry);

private:
    Primitive::Ptr m_primitive;
    Geometry m_geometry;
    float m_focusingDistance;
    bool m_filterImage;
    QString m_raysInputFileName;
    Canvas m_canvas;
};

} // end namespace raytracer

#endif // FLATLENSCAMERA_H
