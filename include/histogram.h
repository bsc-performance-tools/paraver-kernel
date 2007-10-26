#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include "kwindow.h"

class Histogram
{
  public:
    Histogram();
    ~Histogram();

    void setControlWindow( KWindow *whichWindow );
    void setDataWindow( KWindow *whichWindow );
    void setExtraControlWindow( KWindow *whichWindow );

  protected:

  private:
    KWindow *controlWindow;
    KWindow *dataWindow;
    KWindow *xtraControlWindow;

};


#endif // HISTOGRAM_H_INCLUDED
