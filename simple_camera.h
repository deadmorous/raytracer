#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

/// \file
/// \brief Declaration of the SimpleCamera class.

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
        /// \brief Specifies the aspect ratio that determines the field of view in the x direction.
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
        Geometry() : fovy(45.f), aspect(16.f/9.f), dist(1.f), resx(1600), resy(900) {}
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
    };

    /// \brief Default constructor
    SimpleCamera();

    /// \brief Sets camera geometry returns *this.
    SimpleCamera& setGeometry(const Geometry& geometry);

    /// \brief Returns camera geometry.
    Geometry geometry() const;

    Primitive::Ptr cameraPrimitive() const;

    QPixmap pixmap() const;

    void read(const QVariant &v);

private:
    Primitive::Ptr m_primitive;
    Geometry m_geometry;
    QPixmap m_pixmap;
};

} // end namespace raytracer

#endif // SIMPLE_CAMERA_H
