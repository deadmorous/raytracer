#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"

namespace raytracer {

class Sphere : public Primitive
{
    DECL_GENERATOR(Sphere)
public:
    Sphere();
    Sphere(float radius);

    bool collisionTest(float &rayParam, SurfacePoint& p, const Ray& ray) const;
    BoundingSphere boundingSphere() const;

    void read(const QVariant& v);

private:
    float m_radius;
};

} // end namespace raytracer

#endif // SPHERE_H
