#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"

namespace raytracer {

class Sphere : public Primitive
{
public:
    Sphere();
    Sphere(const v3f& center, float radius);

    bool collisionTest(SurfacePoint& p, const Ray& ray) const;
    BoundingSphere boundingSphere() const;
    const SurfaceProperties *surfaceProperties() const;

private:
    v3f center;
    float radius;
};

} // end namespace raytracer

#endif // SPHERE_H
