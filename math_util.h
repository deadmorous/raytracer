#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "common.h"
#include "rnd.h"

#ifndef M_PI
#define M_PI            3.14159265358979323846  ///< The 'Pi' constant
#endif // M_PI

#ifndef M_PI_2
#define M_PI_2          (M_PI/2)                ///< A half of the 'Pi' constant
#endif // M_PI_2

namespace raytracer {

inline v3f randomPointOnUnitSphere()
{
    std::uniform_real_distribution<float> dis(-1.f, 1.f);
    auto& gen = rnd::gen();
    float z = dis(gen);
    float phi = dis(gen)*M_PI;
    float r = sqrt(1.f - z*z);
    return mkv3f(r*cos(phi), r*sin(phi), z);
}

inline v3f randomPointOnUnitSemiSphere()
{
    std::uniform_real_distribution<float> d1(-1.f, 1.f),   d2(0.f, 1.f);
    auto& gen = rnd::gen();
    float z = d2(gen);
    float phi = d1(gen)*M_PI;
    float r = sqrt(1.f - z*z);
    return mkv3f(r*cos(phi), r*sin(phi), z);
}

inline v3f randomPointOnUnitSemiSphere(const v3f& n)
{
    v3f result = randomPointOnUnitSemiSphere();
    if (!(n[0] == 0.f && n[1] == 0.f && n[2] > 0)) {
        v3f p = n;
        p[2] -= 1;
        p /= p.norm2();
        result -= p*(2*dot(p, result));
    }
    return result;
}

} // end namespace raytracer

#endif // MATH_UTIL_H
