#ifndef PRIMITIVE_H
#define PRIMITIVE_H

/// \file
/// \brief Defines the base class for scene primitive.

#include "common.h"
#include "surface_properties.h"
#include "factory.h"
#include "serial.h"

namespace raytracer {

struct Ray;
struct BoundingSphere;

/// \brief Interface for scene primitive.
class Primitive :
    public Readable,
    public FactoryMixin<Primitive>
{
public:
    /// \brief Virtual destructor.
    virtual ~Primitive() {}

    /// \brief Checks for a collision with the specified ray.
    /// \param p Surface point at the intersection of this primitive
    /// and the ray \a ray (value is undefined if there is no collision).
    /// \param ray Ray to test collision with.
    /// \return True if this primitive intersects with \a ray, false otherwise.
    virtual bool collisionTest(SurfacePoint& p, const Ray& ray) const = 0;

    /// \brief Returns bounding sphere for this primitive.
    virtual BoundingSphere boundingSphere() const = 0;

    /// \brief Sets primitive surface properties
    void setSurfaceProperties(const SurfaceProperties::Ptr& surfaceProperties);

    /// \brief Returns primitive surface properties
    SurfaceProperties::Ptr surfaceProperties() const;

    /// @brief Reads surface properties, if any
    void read(const QVariant& v);

private:
    SurfaceProperties::Ptr m_surfaceProperties;
};

} // end namespace raytracer

#endif // PRIMITIVE_H
