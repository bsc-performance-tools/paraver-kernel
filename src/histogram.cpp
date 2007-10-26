#include "histogram.h"


Histogram::Histogram()
{
  controlWindow = NULL;
  dataWindow = NULL;
  xtraControlWindow = NULL;
}


void Histogram::setControlWindow( KWindow *whichWindow )
{
  controlWindow = whichWindow;
}


void Histogram::setDataWindow( KWindow *whichWindow )
{
  dataWindow = whichWindow;
}


void Histogram::setExtraControlWindow( KWindow *whichWindow )
{
  xtraControlWindow = whichWindow;
}
