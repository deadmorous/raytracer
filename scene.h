#ifndef SCENE_H
#define SCENE_H

/// \file
/// \brief Declaration of the Scene class.

#include <vector>
#include "primitive.h"
#include "light_source.h"

namespace raytracer {

class LightSource;
class Camera;

/// \brief Class holding a scene to be visualized.
class Scene : public Readable
{
public:
    Scene();

    /// \brief Returns all primitives of the scene.
    ///
    /// \note The scene is responsible for the lifetime of all
    /// its primitives.
    const std::vector<Primitive::Ptr>& primitives() const;

    /// \brief Returns all light sources of the scene.
    ///
    /// \note The scene is responsible for the lifetime of all
    /// its light sources.
    const std::vector<LightSource::Ptr>& lightSources() const;

    // Camera

    void read(const QVariant& v);

private:
    std::vector<Primitive::Ptr> m_primitives;
    std::vector<LightSource::Ptr> m_lightSources;
};

} // end namespace raytracer

#endif // SCENE_H
