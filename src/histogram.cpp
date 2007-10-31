#include "histogram.h"
#include "histogramstatistic.h"


Histogram::Histogram()
{
  controlWindow = NULL;
  dataWindow = NULL;
  xtraControlWindow = NULL;

  numStatistics = 0;
}


Histogram::~Histogram()
{
  clearStatistics();
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


void Histogram::clearStatistics()
{
  vector<HistogramStatistic *>::iterator it = statisticFunctions.begin();

  while ( it != statisticFunctions.end() )
    delete *it;

  statisticFunctions.clear();
  numStatistics = 0;
}


void Histogram::pushbackStatistic( HistogramStatistic *whichStatistic )
{
  statisticFunctions.push_back( whichStatistic );
  numStatistics++;
}

