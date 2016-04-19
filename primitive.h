#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "common.h"

struct Ray;
struct SurfacePoint;
struct BoundingSphere;

struct Primitive
{
    virtual ~Primitive() {}
    bool collisionTest(SurfacePoint& p, const Ray& ray) const = 0;
    BoundingSphere boundingSphere() const = 0;
};

#endif // PRIMITIVE_H
