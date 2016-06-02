#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "primitive.h"

namespace raytracer {

class Rectangle : public Primitive
{
    DECL_GENERATOR(Rectangle)
public:
    Rectangle();
    Rectangle(float width, float height);

    bool collisionTest(SurfacePoint& p, const Ray& ray) const;
    BoundingSphere boundingSphere() const;

    void read(const QVariant& v);

private:
    float m_width;
    float m_height;
};

} // end namespace raytracer

#endif // RECTANGLE_H
