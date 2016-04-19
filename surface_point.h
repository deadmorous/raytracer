#ifndef SURFACE_POINT_H
#define SURFACE_POINT_H

#include "common.h"

struct SurfacePoint
{
    SurfacePoint() {}
    SurfacePoint(const v3f& r, const v3f& n) :
        r(r),
        n(n)
    {}
    v3f r;
    v3f n;
};


#endif // SURFACE_POINT_H
