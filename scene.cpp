#include "scene.h"

/// \file
/// \brief Implementation of the Scene class.

namespace raytracer {

Scene::Scene()
{

}

std::vector<const Primitive*> Scene::primitives() const
{
    return std::vector<const Primitive*>(); // TODO
}

std::vector<const LightSource*> Scene::lightSources() const
{
    return std::vector<const LightSource*>(); // TODO
}

} // end namespace raytracer
