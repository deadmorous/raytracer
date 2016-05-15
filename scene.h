#ifndef SCENE_H
#define SCENE_H

/// \file
/// \brief Declaration of the Scene class.

#include <vector>
#include "primitive.h"

namespace raytracer {

struct Primitive;
struct LightSource;

/// \brief Class holding a scene to be visualized.
class Scene
{
public:
    Scene();

    /// \brief Returns all primitives of the scene.
    ///
    /// \note The scene is responsible for the lifetime of all
    /// its primitives.
    std::vector<const Primitive*> primitives() const;

    /// \brief Returns all light sources of the scene.
    ///
    /// \note The scene is responsible for the lifetime of all
    /// its light sources.
    std::vector<const LightSource*> lightSources() const;
};

} // end namespace raytracer

#endif // SCENE_H
