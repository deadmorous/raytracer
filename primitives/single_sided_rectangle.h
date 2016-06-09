#ifndef SINGLE_SIDED_RECTANGLE_H
#define SINGLE_SIDED_RECTANGLE_H

#include "primitive.h"

namespace raytracer {

class SingleSidedRectangle : public Primitive
{
    DECL_GENERATOR(SingleSidedRectangle)
public:
    SingleSidedRectangle();
    SingleSidedRectangle(float width, float height);

    bool collisionTest(float &rayParam, SurfacePoint& p, const Ray& ray) const;
    BoundingSphere boundingSphere() const;

    void read(const QVariant& v);

private:
    float m_width;
    float m_height;
};

} // end namespace raytracer

#endif // SINGLE_SIDED_RECTANGLE_H
