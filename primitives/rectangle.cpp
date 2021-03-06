#include "rectangle.h"
#include "bounding_sphere.h"
#include "ray.h"

namespace raytracer {

REGISTER_GENERATOR(Rectangle)

Rectangle::Rectangle() :
    m_width(1.f),
    m_height(1.f)
{
}

Rectangle::Rectangle(float width, float height) :
    m_width(width),
    m_height(height)
{
}

bool Rectangle::collisionTest(float& rayParam, SurfacePoint& p, const Ray& ray) const
{
    auto& T = transform();
    auto center = translation(T);
    auto A = affine(T);
    float sf = scalingFactor(A);
    // v3f n = normalMatrix(T) * mkv3f(0.f, 0.f, 1.f);
    // Since there's no shear, normalMatrix(T) is the same as affine(T) up to scaling.
    auto n = A.constCol(2);
    float en = dot(ray.dir, n);
    if (en == 0.f)
        return false;
    auto d = ray.origin - center;
    rayParam = -dot(d, n) / en;
    if (rayParam <= 0.f)
        return false;
    auto dr = d + rayParam*ray.dir;
    float tex1 = dot(A.constCol(0), dr) / (0.5f*m_width*sf);
    if (fabs(tex1) > 1.f)
        return false;
    float tex2 = dot(A.constCol(1), dr) / (0.5f*m_height*sf);
    if (fabs(tex2) > 1.f)
        return false;
    sppos(p) = center + dr;
    spnormal(p) = n / n.norm2();
    sptex(p) = mkv2f(tex1, tex2);
    return true;
}

BoundingSphere Rectangle::boundingSphere() const
{
    return transformBoundingSphere(
                BoundingSphere(mkv2f(0.5*m_width, 0.5*m_height).norm2()));

}

void Rectangle::read(const QVariant& v)
{
    Primitive::read(v);

    auto m = safeVariantMap(v);
    readOptionalProperty(m_width, m, QString("width"));
    readOptionalProperty(m_height, m, QString("height"));
}

} // end namespace raytracer
