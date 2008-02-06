#include "histogram.h"
#include "window.h"

Histogram *Histogram::create()
{
  return new HistogramProxy();
}


HistogramProxy::HistogramProxy()
{}

HistogramProxy::~HistogramProxy()
{}

bool HistogramProxy::getThreeDimensions() const
{
  return false;
}

TRecordTime HistogramProxy::getBeginTime() const
{
  return 0;
}

TRecordTime HistogramProxy::getEndTime() const
{
  return 0;
}

Window *HistogramProxy::getControlWindow() const
{
  return 0;
}

Window *HistogramProxy::getDataWindow() const
{
  return 0;
}

Window *HistogramProxy::getExtraControlWindow() const
{
  return 0;
}

void HistogramProxy::setControlWindow( Window *whichWindow )
{}

void HistogramProxy::setDataWindow( Window *whichWindow )
{}

void HistogramProxy::setExtraControlWindow( Window *whichWindow )
{}

void HistogramProxy::clearControlWindow()
{}

void HistogramProxy::clearDataWindow()
{}

void HistogramProxy::clearExtraControlWindow()
{}

void HistogramProxy::setControlMin( THistogramLimit whichMin )
{}

void HistogramProxy::setControlMax( THistogramLimit whichMax )
{}

void HistogramProxy::setControlDelta( THistogramLimit whichDelta )
{}

void HistogramProxy::setExtraControlMin( THistogramLimit whichMin )
{}

void HistogramProxy::setExtraControlMax( THistogramLimit whichMax )
{}

void HistogramProxy::setExtraControlDelta( THistogramLimit whichDelta )
{}

void HistogramProxy::setDataMin( THistogramLimit whichMin )
{}

void HistogramProxy::setDataMax( THistogramLimit whichMax )
{}

THistogramLimit HistogramProxy::getControlMin() const
{
  return 0;
}

THistogramLimit HistogramProxy::getControlMax() const
{
  return 0;
}

THistogramLimit HistogramProxy::getControlDelta() const
{
  return 0;
}

THistogramLimit HistogramProxy::getExtraControlMin() const
{
  return 0;
}

THistogramLimit HistogramProxy::getExtraControlMax() const
{
  return 0;
}

THistogramLimit HistogramProxy::getExtraControlDelta() const
{
  return 0;
}

THistogramLimit HistogramProxy::getDataMin() const
{
  return 0;
}

THistogramLimit HistogramProxy::getDataMax() const
{
  return 0;
}

void HistogramProxy::setInclusive( bool newValue )
{}

THistogramColumn HistogramProxy::getNumPlanes() const
{
  return 0;
}

THistogramColumn HistogramProxy::getNumColumns() const
{
  return 0;
}

TObjectOrder HistogramProxy::getNumRows() const
{
  return 0;
}

TSemanticValue HistogramProxy::getCurrentValue( UINT32 col,
    UINT16 idStat,
    UINT32 plane ) const
{
  return 0.0;
}

UINT32 HistogramProxy::getCurrentRow( UINT32 col, UINT32 plane ) const
{
  return 0;
}

void HistogramProxy::setNextCell( UINT32 col, UINT32 plane )
{}

void HistogramProxy::setFirstCell( UINT32 col, UINT32 plane )
{}

bool HistogramProxy::endCell( UINT32 col, UINT32 plane )
{
  return false;
}

bool HistogramProxy::planeWithValues( UINT32 plane ) const
{
  return false;
}

TSemanticValue HistogramProxy::getCommCurrentValue( UINT32 col,
    UINT16 idStat,
    UINT32 plane ) const
{
  return 0.0;
}

UINT32 HistogramProxy::getCommCurrentRow( UINT32 col, UINT32 plane ) const
{
  return 0;
}

void HistogramProxy::setCommNextCell( UINT32 col, UINT32 plane )
{}

void HistogramProxy::setCommFirstCell( UINT32 col, UINT32 plane )
{}

bool HistogramProxy::endCommCell( UINT32 col, UINT32 plane )
{
  return false;
}

bool HistogramProxy::planeCommWithValues( UINT32 plane ) const
{
  return false;
}

KHistogramTotals *HistogramProxy::getColumnTotals() const
{
  return 0;
}

KHistogramTotals *HistogramProxy::getCommColumnTotals() const
{
  return 0;
}

KHistogramTotals *HistogramProxy::getRowTotals() const
{
  return 0;
}

KHistogramTotals *HistogramProxy::getCommRowTotals() const
{
  return 0;
}

void HistogramProxy::clearStatistics()
{}

void HistogramProxy::pushbackStatistic( HistogramStatistic *whichStatistic )
{}

void HistogramProxy::execute( TRecordTime whichBeginTime, TRecordTime whichEndTime )
{}

