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

bool Sphere::collisionTest(SurfacePoint& p, const Ray& ray) const
{
    m4f T = transform();
    v3f center = translation(T);
    float radius = m_radius * scalingFactor(T);

    auto d = ray.origin - center;
    float b = dot(d, ray.dir);
    float c = dot(d, d) - radius*radius;
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
