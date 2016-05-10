#ifndef COMMON_H
#define COMMON_H

/// \file
/// \brief Common declarations used in the ray tracer.

#include "fsmx.h"

/// \brief Functionality specific to the ray tracer.
namespace raytracer {

/// \brief 3D vector.
typedef fsmx::MX< fsmx::Data<3,1,float> > v3f;

/// \brief Returns 3D vector with specified coordinates.
/// \param x1 First coordinate.
/// \param x2 Second coordinate.
/// \param x3 Third coordinate.
/// \return Vector with coordinates \a x1, \a x2, \a x3.
inline v3f mkv3f(float x1, float x2, float x3)
{
    return fsmx::mxMake<v3f>(x1, x2, x3);
}

/// \brief 2D vector.
typedef fsmx::MX< fsmx::Data<2,1,float> > v2f;

/// \brief Returns 2D vector with specified coordinates.
/// \param x1 First coordinate.
/// \param x2 Second coordinate.
/// \return Vector with coordinates \a x1, \a x2.
inline v2f mkv2f(float x1, float x2)
{
    return fsmx::mxMake<v2f>(x1, x2);
}

/// \brief A range of a sequence.
///
/// The \a Iterator template parameter is the type of iterator in the original sequence.
template< class Iterator >
struct Range
{
    typedef Iterator iterator;  ///< \brief Iterator type.
    Iterator begin;             ///< \brief Beginning of range.
    Iterator end;               ///< \brief End of range.

    /// \brief Constructor.
    /// \param begin Initializer for #begin.
    /// \param end Initializer for #end.
    Range(Iterator begin, Iterator end) : begin(begin), end(end) {}

    /// \brief Returns true if this range is empty, false otherwise.
    bool empty() const {
        return begin == end;
    }
};

} // end namespace raytracer

#endif // COMMON_H
