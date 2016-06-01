#include "scene.h"

/// \file
/// \brief Implementation of the Scene class.

namespace raytracer {

Scene::Scene()
{

}

const std::vector<Primitive::Ptr>& Scene::primitives() const
{
    return m_primitives;
}

const std::vector<LightSource::Ptr>& Scene::lightSources() const
{
    return m_lightSources;
}

void Scene::read(const QVariant& v)
{
    m_primitives.clear();
    m_lightSources.clear();

    auto m = safeVariantMap(v);
    using namespace std::placeholders;
    readProperty(m, "primitives", std::bind(&readTypedInstances<Primitive>, std::ref(m_primitives), _1));
    readProperty(m, "lights", std::bind(&readTypedInstances<LightSource>, std::ref(m_lightSources), _1));
}

} // end namespace raytracer
