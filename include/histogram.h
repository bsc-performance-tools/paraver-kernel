#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include "kwindow.h"

class HistogramStatistic;


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

    void clearStatistics();
    void pushbackStatistic( HistogramStatistic *whichStatistic );

    void execute( TRecordTime beginTime, TRecordTime endTime );

  protected:

  private:
    KWindow *controlWindow;
    KWindow *dataWindow;
    KWindow *xtraControlWindow;

    vector<HistogramStatistic *> statisticFunctions;
    UINT16 numStatistics;

    vector<KWindow *> orderedWindows;

    void orderWindows();

};


#endif // HISTOGRAM_H_INCLUDED
