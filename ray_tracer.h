#ifndef RAYTRACER_H
#define RAYTRACER_H

/// \file
/// \brief Declaration of the RayTracer class.

#include "scene.h"
#include "camera.h"
#include "primitive_search.h"

namespace raytracer {

/// @brief Class responsible for the ray tracing algorithm in general.
class RayTracer
{
public:
    RayTracer();

    /// \brief Sets scene to be visualized and returns *this.
    RayTracer& setScene(const Scene& scene);

    /// \brief Returns scene being visualized.
    Scene scene() const;

    /// \brief Sets camera and returns *this.
    RayTracer& setCamera(const Camera& camera);

    /// \brief Returns camera.
    Camera camera() const;

    /// \brief Processes the ray specified.
    void processRay(const Ray& ray);

private:
    Scene m_scene;
    Camera m_camera;

    PrimitiveSearch m_psearch;
};

} // end namespace raytracer

#endif // RAYTRACER_H
