#include "output.h"
#include "textoutput.h"
#include "gnuplotoutput.h"

Output *Output::createOutput( TOutput whichOutput )
{
  switch( whichOutput )
  {
    case TEXT:
      return new TextOutput();
      break;

    case GNUPLOT:
      return new GNUPlotOutput();
      break;

    default:
      return NULL;
  }
}
