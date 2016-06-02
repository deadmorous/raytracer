/// \file
/// \brief Declaration of the PointLight class.

#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light_source.h"

namespace raytracer {

class PointLight : public LightSource
{
    DECL_GENERATOR(PointLight)
public:
    PointLight();
    PointLight(const v3f& color);

    void emitRays(int count, RayTracer& rayTracer) const;
    void read(const QVariant &v);

private:
    v3f m_color;
};

} // end namespace raytracer

#endif // POINTLIGHT_H
