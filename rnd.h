#ifndef RND_H
#define RND_H

#include <random>

namespace raytracer {

class rnd
{
public:
    static std::mt19937& gen();
};

} // end namespace raytracer

#endif // RND_H
