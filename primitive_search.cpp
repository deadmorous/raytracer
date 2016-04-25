#include "primitive_search.h"

namespace raytracer {

PrimitiveSearch::PrimitiveSearch()
{
}

void PrimitiveSearch::add(const Primitive *primitive)
{
    // TODO
    Q_UNUSED(primitive);
}

PrimitiveSearch::PrimitiveSequenceRange PrimitiveSearch::find(const Ray& ray) const
{
    // TODO
    Q_UNUSED(ray);
    return PrimitiveSequenceRange(PrimitiveSequenceIterator(), PrimitiveSequenceIterator());
}

} // end namespace raytracer
