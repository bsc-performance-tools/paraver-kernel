#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include "kwindow.h"

class HistogramStatistic;

class RowsTranslator
{
  public:
    RowsTranslator( vector<KWindow *>& windows );
    ~RowsTranslator();

    TObjectOrder globalTranslate( UINT16 winIndex, TObjectOrder rowIndex );
    void getRowChilds( UINT16 winIndex, TObjectOrder rowIndex,
                       TObjectOrder& iniRow, TObjectOrder& endRow );
  protected:

  private:

};


class Histogram
{
  public:
    Histogram();
    ~Histogram();

    void setControlWindow( KWindow *whichWindow );
    void setDataWindow( KWindow *whichWindow );
    void setExtraControlWindow( KWindow *whichWindow );
    void clearControlWindow();
    void clearDataWindow();
    void clearExtraControlWindow();

    void setControlMin( THistogramLimit whichMin );
    void setControlMax( THistogramLimit whichMax );
    void setControlDelta( THistogramLimit whichDelta );
    void setExtraControlMin( THistogramLimit whichMin );
    void setExtraControlMax( THistogramLimit whichMax );
    void setExtraControlDelta( THistogramLimit whichDelta );
    void setDataMin( THistogramLimit whichMin );
    void setDataMax( THistogramLimit whichMax );

    THistogramLimit getControlMin();
    THistogramLimit getControlMax();
    THistogramLimit getControlDelta();
    THistogramLimit getExtraControlMin();
    THistogramLimit getExtraControlMax();
    THistogramLimit getExtraControlDelta();
    THistogramLimit getDataMin();
    THistogramLimit getDataMax();

    void clearStatistics();
    void pushbackStatistic( HistogramStatistic *whichStatistic );

    void execute( TRecordTime beginTime, TRecordTime endTime );

  protected:

  private:
    KWindow *controlWindow;
    KWindow *dataWindow;
    KWindow *xtraControlWindow;

    THistogramLimit controlMin;
    THistogramLimit controlMax;
    THistogramLimit controlDelta;
    THistogramLimit xtraControlMin;
    THistogramLimit xtraControlMax;
    THistogramLimit xtraControlDelta;
    THistogramLimit dataMin;
    THistogramLimit dataMax;

    vector<HistogramStatistic *> statisticFunctions;
    UINT16 numStatistics;

    vector<KWindow *> orderedWindows;

    void orderWindows();

};


#endif // HISTOGRAM_H_INCLUDED
