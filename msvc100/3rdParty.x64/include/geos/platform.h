/**********************************************************************
 *
 * platform.h - generated by CMake from platform.h.cmake
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.osgeo.org
 *
 * Copyright (C) 2009 Mateusz Loskot
 * Copyright (C) 2005-2009 Refractions Research Inc.
 * Copyright (C) 2001-2009 Vivid Solutions Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation. 
 * See the COPYING file for more information.
 *
 *********************************************************************/

#ifndef GEOS_PLATFORM_H_INCLUDED
#define GEOS_PLATFORM_H_INCLUDED

/* Disable inclusion of platform.h not generated by CMake */
#define GEOS_PLATFORM_H 1

/* Set to 1 if you have stdint.h */
#define HAVE_STDINT_H 1

/* Set to 1 if you have inttypes.h */
/* #undef HAVE_INTTYPES_H */

/* Set to 1 if you have ieeefp.h */
/* #undef HAVE_IEEEFP_H */

/* Set to 1 if you have `int64_t' type */
#define HAVE_INT64_T_64 1

/* Set to 1 if `long int' is 64 bits */
/* #undef HAVE_LONG_INT_64 */

/* Set to 1 if `long long int' is 64 bits */
/* #undef HAVE_LONG_LONG_INT_64 */

/* Set to 1 if C++/C99 std::isnan is defined */
/* #undef HAVE_STD_ISNAN */

/* Set to 1 if C99 isnan is defined */
#define HAVE_ISNAN 1

/* Set to 1 if XCode __isnand is defined */
/* #undef HAVE_ISNAND_XCODE */

/* Set to 1 if XCode __inline_isnand is defined */
/* #undef HAVE_INLINE_ISNAND_XCODE */

/* Set to 1 if C++/C99 std::isfinite is defined */
/* #undef HAVE_STD_ISFINITE */

/* Set to 1 if C99 isfinite is defined */
/* #undef HAVE_ISFINITE */

/* Set to 1 if Visual C++ finite is defined */
#define HAVE_FINITE 1


#ifdef HAVE_IEEEFP_H
extern "C"
{
#include <ieeefp.h>
}
#endif

#ifdef HAVE_INTTYPES_H
extern "C"
{
#include <inttypes.h>
}
# ifndef HAVE_INT64_T_64
#  error Warning: inttypes.h included but HAVE_INT64_T_64 not defined
# endif
#endif

/* We need M_PI, but on MSVC you need to define _USE_MATH_DEFINES before
 * including math.h to get it.  If we are too late (math.h already included)
 * we will define it manually.
 */
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <cmath>
#include <float.h>
#else
#include <cmath>
#endif
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* For MingW the appropriate definitions are included in math.h
 * and float.h but the definitions in math.h are only included
 * if __STRICT_ANSI__ is not defined. Since GEOS is compiled
 * with -ansi that means those definitions are not available.
 */
#if defined(__GNUC__) && defined(_WIN32)
#include <float.h>
#endif

#ifdef HAVE_INT64_T_64
# ifdef _MSC_VER
   typedef __int64 int64;
# else
   typedef int64_t int64;
# endif
#else
# ifdef HAVE_LONG_LONG_INT_64
   typedef long long int int64;
# else
   typedef long int int64;
#  ifndef HAVE_LONG_INT_64
#   define INT64_IS_REALLY32 1
#   warning "Could not find 64bit integer definition!"
#  endif
# endif
#endif

#if defined(HAVE_STD_ISNAN)
# define ISNAN(x) (std::isnan)(x)
#elif defined(HAVE_INLINE_ISNAND_XCODE)
# define ISNAN(x) __inline_isnand(static_cast<double>(x))
#elif defined(HAVE_ISNAND_XCODE)
# define ISNAN(x) __isnand(static_cast<double>(x))
#elif defined(HAVE_ISNAN)
# if defined(_MSC_VER)
#  define ISNAN(x) _isnan(static_cast<double>(x))
# else
#  define ISNAN(x) isnan(x)
# endif
#else
# error "Could not find isnan function or macro!"
#endif

#if defined(HAVE_STD_ISFINITE)
# define FINITE(x) (std::isfinite)(x)
#elif defined(HAVE_ISFINITE)
# define FINITE(x) isfinite(x)
#elif defined(HAVE_FINITE)
# define FINITE(x) _finite(static_cast<double>(x))
#else
# error "Could not find finite or isfinite function or macro!"
#endif

#define DoubleNegInfinity (-(std::numeric_limits<double>::infinity)())
#define DoubleMax (std::numeric_limits<double>::max)()
// Defines NaN for Intel platforms
#define DoubleNotANumber std::numeric_limits<double>::quiet_NaN()
// Don't forget to define infinities
#define DoubleInfinity (std::numeric_limits<double>::infinity)()

#endif // GEOS_PLATFORM_H_INCLUDED
