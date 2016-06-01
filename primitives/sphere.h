#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"

namespace raytracer {

class Sphere : public Primitive
{
    DECL_GENERATOR(Sphere)
public:
    Sphere();
    Sphere(const v3f& m_center, float m_radius);

    bool collisionTest(SurfacePoint& p, const Ray& ray) const;
    BoundingSphere boundingSphere() const;

    void read(const QVariant& v);

private:
    v3f m_center;
    float m_radius;
};

} // end namespace raytracer

#endif // SPHERE_H
