#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "common.h"

struct Ray;

struct BoundingSphere
{
    BoundingSphere() {}
    BoundingSphere(const v3f& center, float radius) :
        center(center),
        radius(radius)
    {}
    bool collidesWith(const Ray& ray) const {
        return false;   // TODO
    }
    v3f center;
    float radius;
};

#endif // BOUNDING_SPHERE_H
