#ifndef CAMERA_H
#define CAMERA_H

/// \file
/// \brief Declaration of the Scene class.

#include "primitive.h"
#include <QImage>

namespace raytracer {

/// \brief Interface for a camera.
class Camera :
        public Readable,
        public FactoryMixin<Camera>
{
public:
    /// \brief Default constructor.
    Camera();

    /// \brief Clears canvas and re-creates primitive.
    ///
    /// \note Must be called after setTransform() and probably
    /// other methods of a derived class.
    virtual void clear() = 0;

    /// \brief Returns primitive that represents
    /// camera screen.
    virtual Primitive::Ptr cameraPrimitive() const = 0;

    /// \brief Returns generated image.
    virtual QImage image() const = 0;

    /// \brief Returns primitive transformation matrix.
    const m4f& transform() const;

    /// \brief Sets primitive transformation matrix.
    void setTransform(const m4f &transform);

    /// \brief Reads camera transformation.
    void read(const QVariant& v);

private:
    m4f m_transform;
};

} // end namespace raytracer

#endif // CAMERA_H
