// cxx_exception.h

/** \ingroup CTMSTD_CXX*/
/** \defgroup CTMSTD_CXX_EXCEPTION Exceptions support
This module introduces the ctm::cxx::exception class and the #CTM_DECL_EXCEPTION macro.
While the \b std::exception class exists, it is in some cases inconvenient
to use it directly or as a base class for another exception class.
The reason is that \b std::exception does not provide (at least for STL available with gcc 3.2)
a constructor that accepts a string describing the exceptin. In many cases, however,
this featire is desirable. The ctm::cxx::exception class inherits the \b std::exception class and
provides the constructor that accepts a text string.

To exploit the concept of structural exception handling, you can derive your own
exceptions from ctm::cxx::exception by using the #CTM_DECL_EXCEPTION macro.

<b> General usage </b>
  - Derive appropriate exception(s) from ctm::cxx::exception by using #CTM_DECL_EXCEPTION (optional).
  - Whenever a textual description is required for an exception,
    just throw your exception, providing the description text as the constructor argument.
  - Catch your exception somewhere in your code, or catch a reference to \b std::exception or ctm::cxx::exception.

<b> Supported platforms:</b> platform independent.

<b> Module type:</b> header file.
*/

/// \ingroup CTMSTD_CXX_EXCEPTION
/// \file cxx_exception.h
/// \brief \ref CTMSTD_CXX_EXCEPTION
///
/// \b Contents:
/// - the ctm::cxx::exception exception that supports construction by a string and returns that string in what();
/// - The #CTM_DECL_EXCEPTION macro for declaring derived exceptions.

#ifndef _CXX_EXCEPTION_H_8A20E4E5_42D0_4d8a_B50D_4326E9683BB8_
#define _CXX_EXCEPTION_H_8A20E4E5_42D0_4d8a_B50D_4326E9683BB8_

#include <string>

/// \brief Collection of features related to the C++ language
///
/// The ctm::cxx namespace contains general-purpose features
/// that are often required in C++ programs. The features include
/// pointer wrappers, iterators, sequences, and so on.
/// Many modules add members to this namespace
namespace cxx {

/** \addtogroup CTMSTD_CXX_EXCEPTION */
//@{
/// \brief An exception derived from \b std::exception and having a constructor that accepts a string.
///
/// The \b std::exception class has the what() virtual function but, in general, provides no way
/// to initialize the string member in which a description of the exception is stored.
/// In MSVC 6.0, \b std::exception has a constructor that accepts a text string; however,
/// in STL version that comes with gcc 3.2 such a constructor is absent.
/// So, this class just provides a constructor that accepts a text string.
class exception : public std::exception
    {
    public:
        /// Default constructor
        exception() throw() {}
        
        /// \brief The destructor
        ///
        /// The destructor seems to be required just in order to declare that any
        /// exceptions are not thrown. gcc 3.2 requires this, MSVC 6.0 does not.
        ~exception() throw() {}

        /// Constructor that accepts a text string that contains a description of the exception
        exception( const std::string& reason_ ) throw() : reason( reason_ ) {}

        /// Returns a text string that contains a description of the exception
        const char *what() const throw() { return reason.c_str(); }
    protected:
        std::string reason;     ///< Contains a description of the exception. Returned by what().
                                ///< Derived exceptions can access and modify this member.
    };

/// \brief This macro is to be used to declare derived exceptions that behave just like ctm::cxx::exception
///
/// The macro declares the \a nameEx class that inherits the \a nameBaseEx class.
/// \a nameBaseEx must directly or indirectly inherit the ctm::cxx::exception class.
///
/// Exceptions derived using this macro differ from ctm::cxx::exception only by their type.
/// The purpose to derive exceptions from ctm::cxx::exception is the structured exception handling.
/// \note All exceptions derived from ctm::cxx::exception also inherit \b std::exception, since it is the base class.
#define CTM_DECL_EXCEPTION( nameEx, nameBaseEx )\
class nameEx : public nameBaseEx\
    {\
    public:\
        nameEx() throw() {}\
        ~nameEx() throw() {}\
        nameEx( const ::std::string& reason_ ) throw() : nameBaseEx( reason_ ) {}\
    };
//@}

} // end namespace cxx

#endif // _CXX_EXCEPTION_H_8A20E4E5_42D0_4d8a_B50D_4326E9683BB8_
