// compile_assert.h

#ifndef _COMPILE_ASSERT_H_8A20E4E5_42D0_4d8a_B50D_4326E9683BB8_
#define _COMPILE_ASSERT_H_8A20E4E5_42D0_4d8a_B50D_4326E9683BB8_

namespace ctm { namespace cxx {
template< bool X > struct CompileAssert {};
    } };

/// \brief Compile-time assertion.
///
/// In if the argument is zero, the compiler generates an error. Otherwise, nothing happens.
/// \note The argument must be a constant expression, i.e., it has to be possible
/// to calculate its value at compile time.
/// \note The implementation is based on code found here: http://src.chromium.org/svn/trunk/src/third_party/cld/base/macros.h
#define COMPILE_ASSERT(expr) \
    typedef ::ctm::cxx::CompileAssert<(bool(expr))> CompileAssertTypedef[bool(expr) ? 1 : -1]

#endif // _COMPILE_ASSERT_H_8A20E4E5_42D0_4d8a_B50D_4326E9683BB8_
