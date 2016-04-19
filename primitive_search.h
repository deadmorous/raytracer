#ifndef PRIMITIVE_SEARCH_H
#define PRIMITIVE_SEARCH_H

#include "common.h"

struct Ray;
struct Primitive;

class PrimitiveSearch
{
public:
    PrimitiveSearch();
    void add(const Primitive *primitive);
    Primitive *find(const Ray& ray) const;
};

#endif // PRIMITIVE_SEARCH_H
