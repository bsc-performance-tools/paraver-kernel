#ifndef _PRVTYPES_H_
#define _PRVTYPES_H_

/**
 * To include this file add these lines in configure.in:

AC_CHECK_TYPES([long, long long, char, int, short int])
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(short int)
AC_CHECK_TYPES([int64_t, uint64_t, int8_t, uint8_t, int32_t, uint32_t, int16_t, uint16_t])

 */

#include <config_types.h>

#ifdef HAVE_STDINT_H
   #include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
   #include <inttypes.h>
#endif

#if defined(HAVE_INT64_T) && defined(HAVE_UINT64_T)
  /* If system has uint64_t/int64_t use them first */
  typedef int64_t  PRV_INT64;
  typedef uint64_t PRV_UINT64;
#else
  /* If system does not have PRV_UINT64/PRV_INT64, check which type can be used instead */
  #if defined(HAVE_LONG_LONG) && (SIZEOF_LONG_LONG == 8)
    /* long long occupies 8 bytes!, use it as PRV_UINT64/PRV_INT64 */
    typedef long long PRV_INT64;
    typedef unsigned long long PRV_UINT64;
  #elif defined(HAVE_LONG) && (SIZEOF_LONG == 8)
    /* long occupies 8 bytes!, use it as PRV_UINT64/PRV_INT64 */
    typedef long PRV_INT64;
    typedef unsigned long PRV_UINT64;
  #else
    #error "No 64-bit data type found"
  #endif
#endif

#if defined(HAVE_INT8_T) && defined(HAVE_UINT8_T)
  /* If system has uint8/int8 use them first */
  typedef int8_t  PRV_INT8;
  typedef uint8_t PRV_UINT8;
#else
  /* If system does not have PRV_UINT8/PRV_INT8, check which type can be used instead */
  #if defined(HAVE_CHAR) && (SIZEOF_CHAR == 1)
    /* char occupies 1 byte!, use it as PRV_UINT8/PRV_INT8 */
    typedef char PRV_INT8;
    typedef unsigned char PRV_UINT8;
  #else
    #error "No 8-bit data type found"
  #endif
#endif

#if defined(HAVE_INT16_T) && defined(HAVE_UINT16_T)
  /* If system has uint16/int16 use them first */
  typedef int16_t  PRV_INT16;
  typedef uint16_t PRV_UINT16;
#else
  /* If system does not have PRV_UINT16/PRV_INT16, check which type can be used instead */
  #if defined(HAVE_SHORT_INT) && (SIZEOF_SHORT_INT == 2)
    /* short int occupies 2 bytes!, use it as PRV_UINT16/PRV_INT16 */
    typedef short int PRV_INT16;
    typedef unsigned short int PRV_UINT16;
  #else
    #error "No 16-bit data type found"
  #endif
#endif

#if defined(HAVE_INT32_T) && defined(HAVE_UINT32_T)
  /* If system has uint32/int32 use them first */
  typedef int32_t  PRV_INT32;
  typedef uint32_t PRV_UINT32;
#else
  /* If system does not have PRV_UINT32/PRV_INT32, check which type can be used instead */
  #if defined(HAVE_INT) && (SIZEOF_INT == 4)
    /* int occupies 4 bytes!, use it as PRV_UINT32/PRV_INT32 */
    typedef int PRV_INT32;
    typedef unsigned int PRV_UINT32;
  #else
    #error "No 32-bit data type found"
  #endif
#endif

typedef PRV_UINT64 STACK_ADDRESS;

#endif /* _PRVTYPES_H_ */
