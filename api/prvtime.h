/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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

#include <array>

#include "utils/traceparser/tracetypes.h"

constexpr TTimeUnit TIME_SCALE_SIZE = DAY + 1;

static const TRecordTime factorTable[ TIME_SCALE_SIZE ] =
{
  1,   // ns
  1E3, // us
  1E3, // ms
  1E3, // seconds
  60,  // minutes
  60,  // hours
  24   // days
};

static const TTimeUnit precisionTable[ TIME_SCALE_SIZE ] =
{
  0,   // ns
  3,   // us
  3,   // ms
  3,   // seconds
  2,   // minutes
  2,   // hours
  2    // days
};


using TTimeFactorTable    = std::array< std::array< TTime,     TIME_SCALE_SIZE >, TIME_SCALE_SIZE >;
using TTimePrecisionTable = std::array< std::array< TTimeUnit, TIME_SCALE_SIZE >, TIME_SCALE_SIZE >;


template< TTimeUnit Length, typename Generator >
constexpr auto fillTimeConversionTable( Generator&& f )
{
  using content_type = decltype( f( TTimeUnit{0} ) );
  std::array<content_type, Length> result {};

  for( TTimeUnit i = NS; i < Length; ++i )
  {
    result[ i ] = f( i );
  }

  return result;
}


template< TTimeUnit Length >
inline constexpr auto timeFactorsTable = fillTimeConversionTable<Length>
  ( []( TTimeUnit unit )
    {
      std::array< TTime, Length> result {};
      
      result[ unit ] = 1.0;

      if ( unit > NS )
      {
        for( TTimeUnit i = unit - 1; i > NS; --i )
        {
          result[ i ] = result[ i + 1 ] * factorTable[ i + 1 ];
        }
        result[ NS ] = result[ US ] * factorTable[ US ];
      }

      if ( unit < DAY )
      {
        for( TTimeUnit i = unit + 1; i <= DAY; ++i )
        {
          result[ i ] = result[ i - 1 ] / factorTable[ i ];
        }
      }

      return result;
    }
  );


template< TTimeUnit Length >
inline constexpr auto timePrecisionsTable = fillTimeConversionTable<Length>
  ( []( TTimeUnit unit )
    {
      std::array< TTimeUnit, Length> result {};
      
      result[ unit ] = 0;

      if ( unit > NS )
      {
        for( TTimeUnit i = unit - 1; i > NS; --i )
        {
          result[ i ] = result[ i + 1 ] + precisionTable[ i + 1 ];
        }
        result[ NS ] = result[ US ] + precisionTable[ US ];
      }

      return result;
    }
  );


