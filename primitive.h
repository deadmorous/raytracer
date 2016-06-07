#ifndef PRIMITIVE_H
#define PRIMITIVE_H

/// \file
/// \brief Defines the base class for scene primitive.

#include "common.h"
#include "surface_properties.h"
#include "factory.h"
#include "serial.h"

namespace raytracer {

struct Ray;
struct BoundingSphere;

/// \brief Interface for scene primitive.
class Primitive :
    public Readable,
    public FactoryMixin<Primitive>
{
public:
    /// \brief Default constructor.
    ///
    /// Sets transformation to identity.
    Primitive();

    /// \brief Checks for a collision with the specified ray.
    /// \param p Surface point at the intersection of this primitive
    /// and the ray \a ray (value is undefined if there is no collision).
    /// \param ray Ray to test collision with.
    /// \return True if this primitive intersects with \a ray, false otherwise.
    virtual bool collisionTest(float& rayParam, SurfacePoint& p, const Ray& ray) const = 0;

    /// \brief Returns bounding sphere for this primitive.
    ///
    /// \note Current transformation must be taken into account.
    /// This can be done by first computing the bounding sphere
    /// for the untransformed geometry, and then calling
    /// transformBoundingSphere().
    virtual BoundingSphere boundingSphere() const = 0;

    /// \brief Helper method to compute bounding sphere for transformed geometry.
    /// \param bs Bounding sphere for untransformed geometry.
    BoundingSphere transformBoundingSphere(const BoundingSphere &bs) const;

    /// \brief Returns primitive name.
    QString name() const;

    /// \brief Sets primitive name.
    void setName(const QString& name);

    /// \brief Returns primitive transformation matrix.
    const m4f& transform() const;

    /// \brief Sets primitive transformation matrix.
    void setTransform(const m4f &transform);

    /// \brief Sets primitive surface properties
    void setSurfaceProperties(const SurfaceProperties::Ptr& surfaceProperties);

    /// \brief Returns primitive surface properties
    SurfaceProperties::Ptr surfaceProperties() const;

    /// \brief Reads transformation and surface properties, if any
    void read(const QVariant& v);

private:
    QString m_name;
    m4f m_transform;
    SurfaceProperties::Ptr m_surfaceProperties;
};

} // end namespace raytracer

#endif // PRIMITIVE_H
