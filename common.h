#ifndef COMMON_H
#define COMMON_H

/// \file
/// \brief Common declarations used in the ray tracer.

#include "fsmx.h"

/// \brief Functionality specific to the ray tracer.
namespace raytracer {

/// \brief 4D vector.
typedef fsmx::MX< fsmx::Data<4,1,float> > v4f;

/// \brief Returns 3D vector with specified coordinates.
/// \param x1 First coordinate.
/// \param x2 Second coordinate.
/// \param x3 Third coordinate.
/// \param x4 Fourth coordinate, defaults to one.
/// \return Vector with coordinates \a x1, \a x2, \a x3, \a x4.
inline v4f mkv4f(float x1, float x2, float x3, float x4 = 1.0f)
{
    return fsmx::mxMake<v4f>(x1, x2, x3, x4);
}

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

/// \brief Conversion of vectors
template< class To, class From >
inline To conv(const From& src);

template<>
inline v3f conv(const v4f& src) {
    return src.block< 3, 1 >(0, 0) * (1.f / src[3]);
}

template<>
inline v4f conv(const v3f& src) {
    return mkv4f(src[0], src[1], src[2], 1.0f);
}

/// \brief 3x3 matrix.
typedef fsmx::MX< fsmx::Data<3,3,float> > m3f;

/// \brief 4x4 matrix.
typedef fsmx::MX< fsmx::Data<4,4,float> > m4f;

inline m3f dyad(const v3f& left, const v3f& right) {
    return left * right.T();
}

/// \brief Returns the upper-left 3x3 block of 4x4 matrix (the affine transformation).
inline auto affine(m4f& t) -> decltype(t.block<3,3>(0,0)) {
    return t.block< 3, 3 >(0, 0);
}

/// \brief Returns the upper-left 3x3 block of 4x4 matrix (the affine transformation), overload.
inline auto affine(const m4f& t) -> decltype(t.block<3,3>(0,0)) {
    return t.block< 3, 3 >(0, 0);
}

/// \brief Returns the upper-right 3x1 block of 4x4 matrix (the translation).
inline auto translation(m4f& t) -> decltype(t.block<3,1>(0,3)) {
    return t.block< 3, 1 >(0, 3);
}

/// \brief Returns the upper-right 3x1 block of 4x4 matrix (the translation), overload.
inline auto translation(const m4f& t) -> decltype(t.block<3,1>(0,3)) {
    return t.block< 3, 1 >(0, 3);
}

/// \brief Returns true if \a t has shear, false otherwise.
inline bool hasShear(const m3f& t)
{
    const float relTol = 1e-7f;
    m3f tt = t*t.T();
    float x = tt.trace() / 3.f;
    const float *d = tt.data().data();
    float tol = x*relTol;
    for (int i=0, index=0; i<3; ++i)
        for (int j=0; j<3; ++j, ++index) {
            float v = i == j ?   x :   0.f;
            if (fabs(v-d[index]) > tol)
                return true;
        }
    return false;
}

/// \brief Returns true if the affine part of \a t has shear, false otherwise.
inline bool hasShear(const m4f& t) {
    return hasShear(static_cast<m3f>(affine(t)));
}

/// \brief Returns scaling factor of the affine transformation matrix without shear.
inline float scalingFactor(const m3f& t) {
    Q_ASSERT(!hasShear(t));
    return sqrt((t.T()*t).trace() / 3.f);
}

/// \brief Returns scaling factor of transformation matrix without shear.
inline float scalingFactor(const m4f& t) {
    return scalingFactor(static_cast<m3f>(affine(t)));
}

/// \brief Returns the inverse of transposed \a t.
inline m3f normalMatrix(const m3f& t) {
    return t.T().inv();
}

/// \brief Returns the inverse of transposed \a t.
inline m3f normalMatrix(const m4f& t) {
    return normalMatrix(static_cast<m3f>(affine(t)));
}

inline m3f spin(const v3f& x) {
    m3f result;
    float *d = result.data().data();
    const float *xd = x.data().data();
    // Note: In m3f, data is stored row by row,
    d[0] = d[4] = d[8] = 0.f;
    d[1] = -xd[2];
    d[2] = xd[1];
    d[3] = xd[2];
    d[5] = -xd[0];
    d[6] = -xd[1];
    d[7] = xd[0];
    return result;
}

inline m3f rotation(const v3f& axis, float angle) {
    float axisLength = axis.norm2();
    if (axisLength == 0.f   ||   angle == 0.f)
        return fsmx::identity<m3f>();
    v3f n = axis * (1.f/axisLength);
    m3f nn = dyad(n, n);
    float c = static_cast<float>(cos(angle));
    float s = static_cast<float>(sin(angle));
    return nn + c*(fsmx::identity<m3f>() - nn) + spin(s*n);
}

inline float deg2rad(float deg) {
    static const float pi = 3.14159265358979323846f;
    return deg*pi/180;
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
