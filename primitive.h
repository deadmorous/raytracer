#ifndef PRIMITIVE_H
#define PRIMITIVE_H

/// \file
/// \brief Defines the interface for scene primitive.

#include "common.h"
#include "surface_point.h"

namespace raytracer {

struct Ray;
struct BoundingSphere;
struct SurfaceProperties;

/// \brief Interface for scene primitive.
struct Primitive
{
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

    /// \brief Returns primitive surface properties
    virtual const SurfaceProperties *surfaceProperties() const = 0;
};

} // end namespace raytracer

#endif // PRIMITIVE_H
