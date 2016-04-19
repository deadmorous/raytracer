#ifndef RAY_H
#define RAY_H

#include "common.h"

class Color;

struct Ray
{
    v3f origin;
    v3f dir;
    int generation;
    v3f color;

    Ray() {}
    Ray(const v3f& origin,
        const v3f& dir,
        const v3f& color,
        int generation) :
        origin(origin),
        dir(dir),
        color(color),
        generation(generation)
    {}

};

#endif // RAY_H
