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

        float refractionCoefficient;
        float focusingDistance;

        // Computed values
        float screenWidth;
        float screenHeight;
        float R;
        float fx;
        float matrixWidth;
        float matrixHeight;

        Geometry() :
            fovy(90.f),
            aspect(16.f/9.f),
            dist(1.f),
            resx(1600),
            resy(900),
            refractionCoefficient(2.f),
            focusingDistance(10.f)
        {
            computeValues();
        }
        Geometry(
                float fovy,
                float aspect,
                float dist,
                int resx,
                int resy,
                float refractionCoefficient,
                float focusingDistance) :
            fovy(fovy),
            aspect(aspect),
            dist(dist),
            resx(resx),
            resy(resy),
            refractionCoefficient(refractionCoefficient),
            focusingDistance(focusingDistance)
        {
            computeValues();
        }

        void computeValues() {
            screenHeight = dist * 2.f*tan(0.5f*deg2rad(fovy));
            screenWidth = screenHeight * aspect;
            R = dist;
            fx = 1.f / ((1.f-1.f/refractionCoefficient)/R - 1.f/(refractionCoefficient*focusingDistance));
            matrixHeight = screenHeight * (fx-R) / R;
            matrixWidth = screenWidth * (fx-R) / R;
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
    QString m_raysInputFileName;
    Canvas m_canvas;
};

} // end namespace raytracer

#endif // FLATLENSCAMERA_H
