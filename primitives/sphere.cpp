#include "sphere.h"
#include "bounding_sphere.h"
#include "ray.h"

namespace raytracer {

REGISTER_GENERATOR(Sphere)

Sphere::Sphere() :
    m_radius(1.f)
{
}

Sphere::Sphere(float radius) :
    m_radius(radius)
{
}

bool Sphere::collisionTest(float& rayParam, SurfacePoint& p, const Ray& ray) const
{
    auto& T = transform();
    auto center = translation(T);
    float radius = m_radius * scalingFactor(affine(T));

    auto d = center - ray.origin;
    float b = dot(d, ray.dir);
    float D = b*b + radius*radius - dot(d, d);
    if (D < 0)
        return false;
    D = sqrt(D);
    if (b+D < 0)
        return false;
    rayParam = b-D;
    if (rayParam < 0)
        rayParam = b+D;
    auto pos = ray.origin + rayParam*ray.dir;
    sppos(p) = pos;
    auto n = pos - center;
    spnormal(p) = n / n.norm2();
    sptex(p) = mkv2f(0.f, 0.f);  // TODO
    return true;
}

BoundingSphere Sphere::boundingSphere() const
{
    return transformBoundingSphere(
            BoundingSphere(m_radius));
}

void Sphere::read(const QVariant& v)
{
    Primitive::read(v);

    auto m = safeVariantMap(v);
    readOptionalProperty(m_radius, m, QString("radius"));
}

} // end namespace raytracer
