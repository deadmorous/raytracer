#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

/// \file
/// \brief Defines the BoundingSphere data structure.

#include "common.h"
#include "ray.h"

namespace raytracer {

/// \brief Bounding sphere.
///
/// This data structure defines the geometry of a sphere in 3D space.
/// It is for use by a search data structure, e.g., PrimitiveSearch.
struct BoundingSphere
{
    v3f center;         ///< \brief Sphere center.
    float radius;       ///< \brief Sphere radius.

    /// \brief Default constructor (does nothing).
    BoundingSphere() {}


    /// \brief Constructor.
    /// \param center Initializer for #center
    /// \param radius Initializer for #radius
    BoundingSphere(const v3f& center, float radius) :
        center(center),
        radius(radius)
    {}

    /// \brief Checks for collision between a ray and this bounding sphere.
    /// \param ray Ray to test collision with.
    /// \return true if this bounding sphere collides with ray \a ray,
    /// false otherwise.
    bool collidesWith(const Ray& ray) const {
        auto d = ray.origin - center;
        float b = d.T()*ray.dir;
        float c = d.T()*d - radius*radius;
        float D = b*b - c;
        return D >= 0   &&   b + sqrt(D) >= 0;
    }
};

} // end namespace raytracer

#endif // BOUNDING_SPHERE_H
