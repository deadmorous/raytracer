#ifndef RAY_H
#define RAY_H

/// \file
/// \brief Definition of the Ray data structure.

#include "common.h"

namespace raytracer {

/// \brief Structure representing an instance of ray for use in the ray tracer.
struct Ray
{
    v3f origin;         ///< \brief Ray origin.
    v3f dir;            ///< \brief Ray direction (unit length vector).
    int generation;     ///< \brief Ray generation number.
    v3f color;          ///< \brief Ray color.

    /// \brief Default constructor (does nothing).
    Ray() {}

    /// \brief Constructor.
    /// \param origin Initializer for #origin.
    /// \param dir Initializer for #dir.
    /// \param color Initializer for #color.
    /// \param generation Initializer for #generation.
    Ray(const v3f& origin,
        const v3f& dir,
        const v3f& color,
        int generation) :
        origin(origin),
        dir(dir),
        generation(generation),
        color(color)
    {}
};

} // end namespace raytracer

#endif // RAY_H
