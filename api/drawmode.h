#ifndef DRAWMODE_H_INCLUDED
#define DRAWMODE_H_INCLUDED

#include <vector>
#include "paraverkerneltypes.h"

using namespace std;

typedef enum DrawModeMethod
{
  DRAW_LAST = 0, DRAW_MAXIMUM, DRAW_MINNOTZERO, DRAW_RANDOM,
  DRAW_RANDNOTZERO, DRAW_AVERAGE,
  DRAW_NUMMETHODS
};

class DrawMode
{
    static TSemanticValue selectValue( vector<TSemanticValue> v,
                                       DrawModeMethod method );
};

#endif // DRAWMODE_H_INCLUDED
