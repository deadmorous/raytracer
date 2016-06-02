#include "point_light.h"

namespace raytracer {

REGISTER_GENERATOR(PointLight)

PointLight::PointLight() :
    m_color(mkv3f(1.f, 1.f, 1.f))
{
}

void PointLight::emitRays(int count, RayTracer& rayTracer) const
{
    // TODO
}

void PointLight::read(const QVariant &v)
{
    LightSource::read(v);

    m_color = mkv3f(1.f, 1.f, 1.f);
    readOptionalProperty(m_color, v, "color");
}

} // end namespace raytracer
