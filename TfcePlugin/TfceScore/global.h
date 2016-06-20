#ifndef GLOBAL_H
#define GLOBAL_H


// Platform identifiers
//
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
# define OS_WIN32
#elif defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__))
#define OS_MACX
#elif defined(__MACOSX__)
#define OS_MACX
#elif defined(__sun) || defined(sun)
#define OS_SOLARIS
#elif defined(__linux__) || defined(__linux)
#define OS_LINUX
#endif

// Utility macros and inline functions
//

#define QX_MAX(a, b)	((b) < (a) ? (a) : (b))
#define QX_MIN(a, b)	((a) < (b) ? (a) : (b))
#define QX_ABS(a)		((a) >= 0  ? (a) : -(a))

inline int qxRound( double d )
{
    return d >= 0.0 ? int(d + 0.5) : int( d - ((int)d-1) + 0.5 ) + ((int)d-1);
}



/*
 Uncomment the following to suppress relatively useless Microsoft compiler warnings
*/

/*#if defined(_MSC_VER)
#    pragma warning(disable: 4244) // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#    pragma warning(disable: 4275) // non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#    pragma warning(disable: 4514) // unreferenced inline/local function has been removed
#    pragma warning(disable: 4800) // 'type' : forcing value to bool 'true' or 'false' (performance warning)
#    pragma warning(disable: 4097) // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#    pragma warning(disable: 4706) // assignment within conditional expression
#    pragma warning(disable: 4786) // truncating debug info after 255 characters
#    pragma warning(disable: 4660) // template-class specialization 'identifier' is already instantiated
#    pragma warning(disable: 4355) // 'this' : used in base member initializer list
#    pragma warning(disable: 4231) // nonstandard extension used : 'extern' before template explicit instantiation
#    pragma warning(disable: 4710) // function not inlined
#endif */

#endif  // GLOBAL_H