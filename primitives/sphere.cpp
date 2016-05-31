#include "sphere.h"
#include "bounding_sphere.h"
#include "ray.h"

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
    auto d = ray.origin - center;
    float b = d.T()*ray.dir;
    float c = d.T()*d - radius*radius;
    float D = b*b - c;
    if (D < 0)
        return false;
    D = sqrt(D);
    if (b+D < 0)
        return false;
    float x = b-D;
    if (x < 0)
        x = b+D;
    auto pos = ray.origin + x*ray.dir;
    sppos(p) = pos;
    auto n = pos - center;
    spnormal(p) = n / n.norm2();
    sptex(p) = mkv2f(0.f, 0.f);  // TODO
    return true;
}

BoundingSphere Sphere::boundingSphere() const
{
    return BoundingSphere(center, radius);
}

} // end namespace raytracer
