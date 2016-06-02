/// \file
/// \brief Declaration of transformation matrix utility classes.

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "common.h"
#include "factory.h"
#include "serial.h"

namespace raytracer {

class Transform :
    public Readable,
    public FactoryMixin<Transform>
{
public:
    virtual void operator()(m4f& transform) const = 0;
};

class Translate : public Transform
{
    DECL_GENERATOR(Translate)
public:
    Translate();
    explicit Translate(const v3f& translation);
    void read(const QVariant& v);
    void operator()(m4f& transform) const;

private:
    v3f m_translation;  // Translation vector
};

class Rotate : public Transform
{
    DECL_GENERATOR(Rotate)
public:
    void read(const QVariant& v);
    void operator()(m4f& transform) const;

private:
    v3f m_axis;         // Rotation axis, a nonzero vector
    float m_angle;      // Rotation angle, in degrees
};

class Scale : public Transform
{
    DECL_GENERATOR(Scale)
public:
    void read(const QVariant& v);
    void operator()(m4f& transform) const;

private:
    v3f m_scale;        // Scaling coefficients in three directions
};

class CombinedTransform : public Transform
{
    DECL_GENERATOR(CombinedTransform)
public:
    void read(const QVariant& v);
    void operator()(m4f& transform) const;

private:
    // List of transformations
    std::vector< Transform::Ptr > m_transformations;
};

} // end namespace raytracer

#endif // TRANSFORM_H
