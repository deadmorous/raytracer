#include "primitive_search.h"
#include "ray.h"

namespace raytracer {

PrimitiveSearch::PrimitiveSearch()
{
}

void PrimitiveSearch::add(const Primitive *primitive)
{
    // TODO
    // Note: This code is a stub
    m_primitives.push_back(primitive);
}

PrimitiveSearch::PrimitiveSequenceRange PrimitiveSearch::find(const Ray& ray) const
{
    // TODO
    // Note: This code is a stub
    Q_UNUSED(ray);
    return PrimitiveSequenceRange(m_primitives.begin(), m_primitives.end());
}

} // end namespace raytracer
