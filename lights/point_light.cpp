/// \file
/// \brief Implementation of the PointLight class.

#include "point_light.h"
#include "ray_tracer.h"
#include "ray.h"
#include "math_util.h"

namespace raytracer {

REGISTER_GENERATOR(PointLight)

PointLight::PointLight() :
    m_color(mkv3f(1.f, 1.f, 1.f))
{
}

void PointLight::emitRays(quint64 count, RayTracer& rayTracer) const
{

    // Obtain light source origin
    v3f origin = translation(transform());

    // Emit count rays in random directions
    for (quint64 i=0; i<count; ++i)
    {
        // Emit ray in random direction
        rayTracer.processRay(Ray(origin, randomPointOnUnitSphere(), m_color, 0));
    }
}

void PointLight::read(const QVariant &v)
{
    LightSource::read(v);

    m_color = mkv3f(1.f, 1.f, 1.f);
    readOptionalProperty(m_color, v, "color");
}

} // end namespace raytracer
