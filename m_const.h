// m_const.h

/// \file
/// \brief Declarations for mathematical constants.

#ifndef _EX_PI_H_8A20E4E5_42D0_4d8a_B50D_4326E9683BB8_
#define _EX_PI_H_8A20E4E5_42D0_4d8a_B50D_4326E9683BB8_

#ifdef __gnu_linux__
#include <math.h>
#else // __gnu_linux__

#define M_PI            3.14159265358979323846  ///< The 'Pi' constant
#define M_PI_2          (M_PI/2)                ///< A half of the 'Pi' constant

#endif // __gnu_linux__

#endif // _EX_PI_H_8A20E4E5_42D0_4d8a_B50D_4326E9683BB8_
