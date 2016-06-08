#include "rnd.h"

// #define FIXED_RANDOM_SEED

namespace raytracer {

std::mt19937& rnd::gen()
{
    static std::mt19937 g {
#ifdef FIXED_RANDOM_SEED
        12345
#else // FIXED_RANDOM_SEED
        std::random_device()()
#endif // FIXED_RANDOM_SEED
    };
    return g;
}

} // end namespace raytracer
