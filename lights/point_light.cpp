/// \file
/// \brief Implementation of the PointLight class.

#include "point_light.h"
#include "ray_tracer.h"
#include "ray.h"
#include "rnd.h"

namespace raytracer {

REGISTER_GENERATOR(PointLight)

PointLight::PointLight() :
    m_color(mkv3f(1.f, 1.f, 1.f))
{
}

void PointLight::emitRays(int count, RayTracer& rayTracer) const
{
    // Initialize random generation facilities
    auto& gen = rnd::gen();
    std::uniform_real_distribution<float> dis(-1.f, 1.f);

    // Obtain light source origin
    v3f origin = translation(transform());

    // Emit count rays in random directions
    for (int i=0; i<count; ++i)
    {
        // Generate random direction
        v3f dir;
        forever {
            dir = mkv3f(dis(gen), dis(gen), dis(gen));
            float dirNorm = dir.norm2();
            if (dirNorm == 0.f)
                continue;
            dir /= dirNorm;
            break;
        }

        // Emit ray
        rayTracer.processRay(Ray(origin, dir, m_color, 0));
    }
}

void PointLight::read(const QVariant &v)
{
    LightSource::read(v);

    m_color = mkv3f(1.f, 1.f, 1.f);
    readOptionalProperty(m_color, v, "color");
}

} // end namespace raytracer
