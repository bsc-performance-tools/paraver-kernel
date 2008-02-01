#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include "paraverkerneltypes.h"

class KHistogramTotals;
class HistogramStatistic;
class KWindow;

class Histogram
{
  public:
    //Histogram() {}
    virtual ~Histogram() {}

    static Histogram *create();

    virtual bool getThreeDimensions() const = 0;

    virtual TRecordTime getBeginTime() const = 0;
    virtual TRecordTime getEndTime() const = 0;

    virtual KWindow *getControlWindow() const = 0;
    virtual KWindow *getDataWindow() const = 0;
    virtual KWindow *getExtraControlWindow() const = 0;
    virtual void setControlWindow( KWindow *whichWindow ) = 0;
    virtual void setDataWindow( KWindow *whichWindow ) = 0;
    virtual void setExtraControlWindow( KWindow *whichWindow ) = 0;
    virtual void clearControlWindow() = 0;
    virtual void clearDataWindow() = 0;
    virtual void clearExtraControlWindow() = 0;

    virtual void setControlMin( THistogramLimit whichMin ) = 0;
    virtual void setControlMax( THistogramLimit whichMax ) = 0;
    virtual void setControlDelta( THistogramLimit whichDelta ) = 0;
    virtual void setExtraControlMin( THistogramLimit whichMin ) = 0;
    virtual void setExtraControlMax( THistogramLimit whichMax ) = 0;
    virtual void setExtraControlDelta( THistogramLimit whichDelta ) = 0;
    virtual void setDataMin( THistogramLimit whichMin ) = 0;
    virtual void setDataMax( THistogramLimit whichMax ) = 0;

    virtual THistogramLimit getControlMin() const = 0;
    virtual THistogramLimit getControlMax() const = 0;
    virtual THistogramLimit getControlDelta() const = 0;
    virtual THistogramLimit getExtraControlMin() const = 0;
    virtual THistogramLimit getExtraControlMax() const = 0;
    virtual THistogramLimit getExtraControlDelta() const = 0;
    virtual THistogramLimit getDataMin() const = 0;
    virtual THistogramLimit getDataMax() const = 0;

    virtual void setInclusive( bool newValue ) = 0;

    virtual THistogramColumn getNumPlanes() const = 0;
    virtual THistogramColumn getNumColumns() const = 0;
    virtual TObjectOrder getNumRows() const = 0;

    virtual TSemanticValue getCurrentValue( UINT32 col,
                                    UINT16 idStat,
                                    UINT32 plane = 0 ) const = 0;
    virtual UINT32 getCurrentRow( UINT32 col, UINT32 plane = 0 ) const = 0;
    virtual void setNextCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual void setFirstCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual bool endCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual bool planeWithValues( UINT32 plane = 0 ) const = 0;

    virtual TSemanticValue getCommCurrentValue( UINT32 col,
                                        UINT16 idStat,
                                        UINT32 plane = 0 ) const = 0;
    virtual UINT32 getCommCurrentRow( UINT32 col, UINT32 plane = 0 ) const = 0;
    virtual void setCommNextCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual void setCommFirstCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual bool endCommCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual bool planeCommWithValues( UINT32 plane = 0 ) const = 0;

    virtual KHistogramTotals *getColumnTotals() const = 0;
    virtual KHistogramTotals *getCommColumnTotals() const = 0;
    virtual KHistogramTotals *getRowTotals() const = 0;
    virtual KHistogramTotals *getCommRowTotals() const = 0;

    virtual void clearStatistics() = 0;
    virtual void pushbackStatistic( HistogramStatistic *whichStatistic ) = 0;

    virtual void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime ) = 0;

};


class HistogramProxy :public Histogram
{
  public:
    HistogramProxy();
    virtual ~HistogramProxy();
};


#endif // HISTOGRAM_H_INCLUDED
