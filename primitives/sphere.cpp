#include "sphere.h"

namespace raytracer {

Sphere::Sphere() :
    center(mkv3f(0.f, 0.f, 0.f)),
    radius(1.f)
{
}

Sphere::Sphere(const v3f& center, float radius) :
    center(center),
    radius(radius)
{
}

bool Sphere::collisionTest(SurfacePoint& p, const Ray& ray) const
{

}

BoundingSphere Sphere::boundingSphere() const
{

}

const SurfaceProperties *Sphere::surfaceProperties() const
{

}

} // end namespace raytracer
