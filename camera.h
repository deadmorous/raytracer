#ifndef CAMERA_H
#define CAMERA_H

/// \file
/// \brief Declaration of the Scene class.

#include "common.h"

namespace raytracer {

struct Primitive;

/// \brief Class representing the camera.
class Camera
{
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
    Camera();

    /// \brief Sets camera origin and returns *this.
    Camera& setPos(const v3f& pos);

    /// \brief Returns camera origin.
    v3f pos() const;

    /// \brief Sets camera orientation and returns *this.
    ///
    /// \note When \a orientation is the identity matrix, the camera
    /// axes are collinear with the global axes.
    Camera& setOrientation(const m3f& orientation);

    /// \brief Returns camera orientation.
    m3f orientation() const;

    /// \brief Sets camera geometry returns *this.
    Camera& setGeometry(const Geometry& geometry);

    /// \brief Returns camera geometry.
    Geometry geometry() const;

    /// \brief Returns primitive that represents
    /// camera screen.
    ///
    /// \note The camera is responsible for the lifetime
    /// of the primitive.
    Primitive *cameraPrimitive();

    /// Accepts ray colliding with camera screen.
    /// \param x Camera screen x coordinate.
    /// \param y Camera screen y coordinate.
    /// \param color Ray color.
    void acceptRay(int x, int y, const v3f& color);
};

} // end namespace raytracer

#endif // CAMERA_H
