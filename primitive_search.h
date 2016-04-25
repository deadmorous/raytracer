#ifndef PRIMITIVE_SEARCH_H
#define PRIMITIVE_SEARCH_H

/// \file
/// \brief Declares the PrimitiveSearch class.

#include "common.h"
#include <vector>

namespace raytracer {

struct Ray;
struct Primitive;

/// \brief Class that provides the functionality for finding primitives that collide with the specified ray.
class PrimitiveSearch
{
public:
    /// \brief Default constructor.
    PrimitiveSearch();

    /// \brief Takes the specified primitive into account in this instance of the search structure.
    /// \param primitive Pointer to primitive to add.
    void add(const Primitive *primitive);

    /// \brief Iterator for a sequence of primitives.
    ///
    /// \todo Redefine if necessary.
    typedef std::vector<const Primitive*>::const_iterator PrimitiveSequenceIterator;

    /// \brief A range of a sequence of primitives.
    typedef Range<PrimitiveSequenceIterator> PrimitiveSequenceRange;

    /// \brief Finds primitives that may collide with the specified ray.
    /// \param ray Ray to test collision with.
    /// \return Range of primitives that may collide with the ray.
    PrimitiveSequenceRange find(const Ray& ray) const;
};

} // end namespace raytracer

#endif // PRIMITIVE_SEARCH_H
