#include <limits>
#include <time.h>
#include <stdlib.h>
#include "drawmode.h"

// LAST is the default method
template <int method>
inline TSemanticValue selectMethod( vector<TSemanticValue>& v )
{
  return v[ v.size() -1 ];
}

template <>
inline TSemanticValue selectMethod<DRAW_MAXIMUM>( vector<TSemanticValue>& v )
{
  TSemanticValue max = 0;

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
    if( *it > 0 && *it < min ) min = *it;
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

  UINT32 i = 0;
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
  TSemanticValue avg = 0;

  for( vector<TSemanticValue>::iterator it = v.begin(); it != v.end(); ++it )
  {
    avg += *it;
  }

  return avg / v.size();
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

    default:
      break;
  }

  return selectMethod<DRAW_LAST>( v );
}
