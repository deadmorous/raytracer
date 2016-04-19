#ifndef COMMON_H
#define COMMON_H

#include "fsmx.h"

// 3D vector
typedef fsmx::MX< fsmx::Data<3,1,float> > v3f;

inline v3f mkv3f(float x1, float x2, float x3)
{
    return fsmx::mxMake<v3f>(x1, x2, x3);
}

#endif // COMMON_H
