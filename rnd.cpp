#include "rnd.h"

namespace raytracer {

std::mt19937& rnd::gen()
{
    static std::mt19937 g { std::random_device()() };
    return g;
}

} // end namespace raytracer
