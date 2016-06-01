#include "sphere.h"
#include "bounding_sphere.h"
#include "ray.h"

namespace raytracer {

REGISTER_GENERATOR(Sphere)

Sphere::Sphere() :
    m_center(mkv3f(0.f, 0.f, 0.f)),
    m_radius(1.f)
{
}

Sphere::Sphere(const v3f& center, float radius) :
    m_center(center),
    m_radius(radius)
{
}

bool Sphere::collisionTest(SurfacePoint& p, const Ray& ray) const
{
    auto d = ray.origin - m_center;
    float b = d.T()*ray.dir;
    float c = d.T()*d - m_radius*m_radius;
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
    auto n = pos - m_center;
    spnormal(p) = n / n.norm2();
    sptex(p) = mkv2f(0.f, 0.f);  // TODO
    return true;
}

BoundingSphere Sphere::boundingSphere() const
{
    return BoundingSphere(m_center, m_radius);
}

void Sphere::read(const QVariant& v)
{
    Primitive::read(v);

    auto m = safeVariantMap(v);
    readOptionalProperty(m_center, m, QString("center"));
    readOptionalProperty(m_radius, m, QString("radius"));
}

} // end namespace raytracer
