/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                            ptools_common_files                            *
 *                   Performance tools common files library                  *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/


#pragma once


#include <ptools_config.h>


   #include <stdint.h>
#endif

   #include <inttypes.h>
#endif

#if defined(HAVE_INT64_T) && defined(HAVE_UINT64_T)
  /* If system has uint64_t/int64_t use them first */
  typedef int64_t  PTOOLS_INT64;
  typedef uint64_t PTOOLS_UINT64;
#else
  /* If system does not have PTOOLS_UINT64/PTOOLS_INT64, check which type can be used instead */
  #if defined(HAVE_LONG_LONG) && (SIZEOF_LONG_LONG == 8)
    /* long long occupies 8 bytes!, use it as PTOOLS_UINT64/PTOOLS_INT64 */
    typedef long long PTOOLS_INT64;
    typedef unsigned long long PTOOLS_UINT64;
  #elif defined(HAVE_LONG) && (SIZEOF_LONG == 8)
    /* long occupies 8 bytes!, use it as PTOOLS_UINT64/PTOOLS_INT64 */
    typedef long PTOOLS_INT64;
    typedef unsigned long PTOOLS_UINT64;
  #else
    #error "No 64-bit data type found"
  #endif
#endif

#if defined(HAVE_INT8_T) && defined(HAVE_UINT8_T)
  /* If system has uint8/int8 use them first */
  typedef int8_t  PTOOLS_INT8;
  typedef uint8_t PTOOLS_UINT8;
#else
  /* If system does not have PTOOLS_UINT8/PTOOLS_INT8, check which type can be used instead */
  #if defined(HAVE_CHAR) && (SIZEOF_CHAR == 1)
    /* char occupies 1 byte!, use it as PTOOLS_UINT8/PTOOLS_INT8 */
    typedef char PTOOLS_INT8;
    typedef unsigned char PTOOLS_UINT8;
  #else
    #error "No 8-bit data type found"
  #endif
#endif

#if defined(HAVE_INT16_T) && defined(HAVE_UINT16_T)
  /* If system has uint16/int16 use them first */
  typedef int16_t  PTOOLS_INT16;
  typedef uint16_t PTOOLS_UINT16;
#else
  /* If system does not have PTOOLS_UINT16/PTOOLS_INT16, check which type can be used instead */
  #if defined(HAVE_SHORT_INT) && (SIZEOF_SHORT_INT == 2)
    /* short int occupies 2 bytes!, use it as PTOOLS_UINT16/PTOOLS_INT16 */
    typedef short int PTOOLS_INT16;
    typedef unsigned short int PTOOLS_UINT16;
  #else
    #error "No 16-bit data type found"
  #endif
#endif

#if defined(HAVE_INT32_T) && defined(HAVE_UINT32_T)
  /* If system has uint32/int32 use them first */
  typedef int32_t  PTOOLS_INT32;
  typedef uint32_t PTOOLS_UINT32;
#else
  /* If system does not have PTOOLS_UINT32/PTOOLS_INT32, check which type can be used instead */
  #if defined(HAVE_INT) && (SIZEOF_INT == 4)
    /* int occupies 4 bytes!, use it as PTOOLS_UINT32/PTOOLS_INT32 */
    typedef int PTOOLS_INT32;
    typedef unsigned int PTOOLS_UINT32;
  #else
    #error "No 32-bit data type found"
  #endif
#endif

typedef PTOOLS_UINT64 STACK_ADDRESS;


