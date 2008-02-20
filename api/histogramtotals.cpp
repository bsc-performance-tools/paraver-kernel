#include "histogramtotals.h"

HistogramTotals *HistogramTotals::create( HistogramTotals *whichTotals )
{
  return new HistogramTotalsProxy( whichTotals );
}

HistogramTotalsProxy::HistogramTotalsProxy( HistogramTotals *whichTotals )
    : myTotals( whichTotals )
{}

TSemanticValue HistogramTotalsProxy::getTotal( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getTotal( idStat, whichColumn, whichPlane );
}

TSemanticValue HistogramTotalsProxy::getAverage( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getAverage( idStat, whichColumn, whichPlane );
}

TSemanticValue HistogramTotalsProxy::getMaximum( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getMaximum( idStat, whichColumn, whichPlane );
}

TSemanticValue HistogramTotalsProxy::getMinimum( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getMinimum( idStat, whichColumn, whichPlane );
}

TSemanticValue HistogramTotalsProxy::getStdev( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getStdev( idStat, whichColumn, whichPlane );
}

TSemanticValue HistogramTotalsProxy::getAvgDivMax( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getAvgDivMax( idStat, whichColumn, whichPlane );
}

void HistogramTotalsProxy::getAll( vector<TSemanticValue>& where,
                                   UINT16 idStat,
                                   THistogramColumn whichColumn,
                                   THistogramColumn whichPlane ) const
{
  myTotals->getAll( where, idStat, whichColumn, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByTotal( UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByTotal( idStat, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByAverage( UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByAverage( idStat, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByMaximum( UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByMaximum( idStat, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByMinimum( UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByMinimum( idStat, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByStdev( UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByStdev( idStat, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByAvgDivMax( UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByAvgDivMax( idStat, whichPlane );
}
