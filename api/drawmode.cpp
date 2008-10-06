#include <limits>
#include "drawmode.h"

// LAST is the default method
template <int method>
inline TSemanticValue selectMethod( vector<TSemanticValue> v )
{
  return v[ v.size() -1 ];
}

template <>
inline TSemanticValue selectMethod<DRAW_MAXIMUM>( vector<TSemanticValue> v )
{
  TSemanticValue max = 0;

  return max;
}

template <>
inline TSemanticValue selectMethod<DRAW_MINNOTZERO>( vector<TSemanticValue> v )
{
  TSemanticValue min = std::numeric_limits<TSemanticValue>::max();

  return min;
}

template <>
inline TSemanticValue selectMethod<DRAW_RANDOM>( vector<TSemanticValue> v )
{
  TSemanticValue tmp = 0;

  return tmp;
}

template <>
inline TSemanticValue selectMethod<DRAW_RANDNOTZERO>( vector<TSemanticValue> v )
{
  TSemanticValue tmp = 0;

  return tmp;
}

template <>
inline TSemanticValue selectMethod<DRAW_AVERAGE>( vector<TSemanticValue> v )
{
  TSemanticValue avg = 0;

  return avg;
}

TSemanticValue DrawMode::selectValue( vector<TSemanticValue> v,
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
