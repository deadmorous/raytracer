/// \file
/// \brief Declaration of the SimpleCamera class.

#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include "camera.h"

namespace raytracer {

/// \brief Class representing the simple camera.
class SimpleCamera : public Camera
{
    DECL_GENERATOR(SimpleCamera)
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

        float focusingDistance;

        Geometry() :
            fovy(90.f),
            aspect(16.f/9.f),
            dist(1.f),
            resx(1600),
            resy(900),
            focusingDistance(0.f)
        {}
        Geometry(
                float fovy,
                float aspect,
                float dist,
                int resx,
                int resy,
                float focusingDistance) :
            fovy(fovy),
            aspect(aspect),
            dist(dist),
            resx(resx),
            resy(resy),
            focusingDistance(focusingDistance)
        {}

        float screenWidth() const {
            return screenHeight() * aspect;
        }

        float screenHeight() const {
            return dist * 2.f*tan(0.5f*deg2rad(fovy));
        }
    };

    void clear();

    Primitive::Ptr cameraPrimitive() const;

    const Canvas& canvas() const;

    void read(const QVariant &v);

    const Geometry& geometry() const;
    void setGeometry(const Geometry& geometry);

private:
    Primitive::Ptr m_primitive;
    Geometry m_geometry;
    QString m_raysOutputFileName;
    QString m_raysInputFileName;
    Canvas m_canvas;
};

} // end namespace raytracer

#endif // SIMPLE_CAMERA_H
