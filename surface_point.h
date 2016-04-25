#ifndef SURFACE_POINT_H
#define SURFACE_POINT_H

/// \file
/// \brief Functions for working with a point on a surface.

#include "fsmx.h"

namespace raytracer {

/// \brief A point on a surface.
///
/// - Components 0, 1, 2 contain point position.
/// - Components 3, 4, 5 contain surface normal vector.
/// - Components 6, 7 contain surface texture coordinates.
/// .
typedef fsmx::MX< fsmx::Data<8,1,float> > SurfacePoint;

/// \brief Returns surface point with specified components.
inline SurfacePoint surfacePoint( float x1=0, float x2=0, float x3=0, float n1=0, float n2=0, float n3=0, float t1=0, float t2=0 ) {
    return fsmx::mxMake< SurfacePoint >( x1, x2, x3, n1, n2, n3, t1, t2 );
    }

/// \brief Returns proxy to surface point that represents its position as a 3D vector.
inline fsmx::MX< fsmx::SliceProxy< 3, 1, 8, 1, float, float > > sppos( SurfacePoint& v ) {
    return v.block< 3, 1 >( 0, 0 );
    }

/// \brief Returns proxy to surface point that represents its position as a 3D vector (overload for const argument).
inline fsmx::MX< fsmx::SliceProxy< 3, 1, 8, 1, float, const float > > sppos( const SurfacePoint& v ) {
    return v.block< 3, 1 >( 0, 0 );
    }

/// \brief Returns proxy to surface point that represents its surface normal vector as a 3D vector.
inline fsmx::MX< fsmx::SliceProxy< 3, 1, 8, 1, float, float > > spnormal( SurfacePoint& v ) {
    return v.block< 3, 1 >( 3, 0 );
    }

/// \brief Returns proxy to surface point that represents its surface normal vector as a 3D vector (overload for const argument).
inline fsmx::MX< fsmx::SliceProxy< 3, 1, 8, 1, float, const float > > spnormal( const SurfacePoint& v ) {
    return v.block< 3, 1 >( 3, 0 );
    }

/// \brief Returns proxy to surface point that represents its texture coordinates as a 2D vector.
inline fsmx::MX< fsmx::SliceProxy< 2, 1, 8, 1, float, float > > sptex( SurfacePoint& v ) {
    return v.block< 2, 1 >( 6, 0 );
    }

/// \brief Returns proxy to surface point that represents its texture coordinates as a 2D vector (overload for const argument).
inline fsmx::MX< fsmx::SliceProxy< 2, 1, 8, 1, float, const float > > sptex( const SurfacePoint& v ) {
    return v.block< 2, 1 >( 6, 0 );
    }

} // end namespace raytracer

#endif // SURFACE_POINT_H
