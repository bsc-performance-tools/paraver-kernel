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

    void clearStatistics();
    void pushbackStatistic( HistogramStatistic *whichStatistic );

  protected:

  private:
    KWindow *controlWindow;
    KWindow *dataWindow;
    KWindow *xtraControlWindow;

    vector<HistogramStatistic *> statisticFunctions;
    UINT16 numStatistics;
};


#endif // HISTOGRAM_H_INCLUDED
