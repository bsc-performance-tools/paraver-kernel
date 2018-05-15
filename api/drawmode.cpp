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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include <limits>
#include <time.h>
#include <stdlib.h>
#include <map>
#include "drawmode.h"

using std::vector;
using std::map;

// LAST is the default method
template <int method>
inline TSemanticValue selectMethod( vector<TSemanticValue>& v )
{
  return v[ v.size() -1 ];
}

template <>
inline TSemanticValue selectMethod<DRAW_MAXIMUM>( vector<TSemanticValue>& v )
{
#if __cplusplus >= 201103L
  TSemanticValue max = std::numeric_limits<TSemanticValue>::lowest();
#else
  TSemanticValue max = -std::numeric_limits<TSemanticValue>::max();
#endif

  for( vector<TSemanticValue>::iterator it = v.begin(); it != v.end(); ++it )
  {
    if( *it > max ) max = *it;
  }

  return max;
}

template <>
inline TSemanticValue selectMethod<DRAW_MINNOTZERO>( vector<TSemanticValue>& v )
{
  TSemanticValue min = std::numeric_limits<TSemanticValue>::max();

  for( vector<TSemanticValue>::iterator it = v.begin(); it != v.end(); ++it )
  {
    if( *it < min ) min = *it;
  }

  if( min == std::numeric_limits<TSemanticValue>::max() )
    return 0;

  return min;
}

template <>
inline TSemanticValue selectMethod<DRAW_RANDOM>( vector<TSemanticValue>& v )
{
  int pos;

  pos = v.size() * rand() / RAND_MAX;
  if( pos >= (int) v.size() )
    pos = v.size() - 1;

  return v[ pos ];
}

template <>
inline TSemanticValue selectMethod<DRAW_RANDNOTZERO>( vector<TSemanticValue>& v )
{
  int pos;

  pos = v.size() * rand() / RAND_MAX;
  if( pos >= (int) v.size() )
    pos = v.size() - 1;

  PRV_UINT32 i = 0;
  while( v[ pos ] == 0 )
  {
    ++pos;
    pos = pos % v.size();
    i++;
    if( i == v.size() ) return 0;
  }

  return v[ pos ];
}

template <>
inline TSemanticValue selectMethod<DRAW_AVERAGE>( vector<TSemanticValue>& v )
{
  TSemanticValue avg = 0.0;

  for( vector<TSemanticValue>::iterator it = v.begin(); it != v.end(); ++it )
  {
    avg += *it;
  }

  return avg / v.size();
}

template <>
inline TSemanticValue selectMethod<DRAW_AVERAGENOTZERO>( vector<TSemanticValue>& v )
{
  TSemanticValue avg = 0.0;
  TSemanticValue times = 0.0;

  for( vector<TSemanticValue>::iterator it = v.begin(); it != v.end(); ++it )
  {
    if( *it != 0.0 )
    {
      avg += *it;
      ++times;
    }
  }

  if( times == 0.0 )
    return 0.0;
  return avg / times;
}

template<>
inline TSemanticValue selectMethod<DRAW_MODE>( vector<TSemanticValue>& v )
{
  map<TSemanticValue, int> modes;
  TSemanticValue currentMode = 0;
  int currentModeCount = 0;

  for( vector<TSemanticValue>::iterator it = v.begin(); it != v.end(); ++it )
  {
    modes[ *it ]++;
    if( modes[ *it ] > currentModeCount )
    {
      currentMode = *it;
      currentModeCount = modes[ *it ];
    }
  }

  return currentMode;
}

TSemanticValue DrawMode::selectValue( vector<TSemanticValue>& v,
                                      DrawModeMethod method )
{
  switch ( method )
  {
    case DRAW_MAXIMUM:
      return selectMethod<DRAW_MAXIMUM>( v );
      break;

    case DRAW_MINNOTZERO:
      return selectMethod<DRAW_MINNOTZERO>( v );
      break;

    case DRAW_RANDOM:
      return selectMethod<DRAW_RANDOM>( v );
      break;

    case DRAW_RANDNOTZERO:
      return selectMethod<DRAW_RANDNOTZERO>( v );
      break;

    case DRAW_AVERAGE:
      return selectMethod<DRAW_AVERAGE>( v );
      break;

    case DRAW_AVERAGENOTZERO:
      return selectMethod<DRAW_AVERAGENOTZERO>( v );
      break;

    case DRAW_MODE:
      return selectMethod<DRAW_MODE>( v );
      break;

    default:
      break;
  }

  return selectMethod<DRAW_LAST>( v );
}
