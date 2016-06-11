/// \file
/// \brief Declaration of the FlatLensCamera class.

#ifndef FLATLENSCAMERA_H
#define FLATLENSCAMERA_H

#include "simple_camera.h"

namespace raytracer {

class FlatLensCamera : public SimpleCamera
{
    DECL_GENERATOR(FlatLensCamera)
public:
    void clear();
};

} // end namespace raytracer

#endif // FLATLENSCAMERA_H
