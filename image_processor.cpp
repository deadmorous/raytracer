#include "image_processor.h"

namespace raytracer {

REGISTER_GENERATOR(IdentityImageProcessor)

REGISTER_GENERATOR(AverageImage)

REGISTER_GENERATOR(ClampImage)

REGISTER_GENERATOR(CombinedImageProcessor)

} // end namespace raytracer
