#ifndef KPROGRESSCONTROLLER_H_INCLUDED
#define KPROGRESSCONTROLLER_H_INCLUDED

#include "progresscontroller.h"

class KProgressController: public ProgressController
{
  public:
    KProgressController();
    ~KProgressController();

    void setHandler( ProgressHandler whichHandler );
    void callHandler( ProgressController *not_used );
    double getEndLimit() const;
    void setEndLimit( double limit );
    double getCurrentProgress() const;
    void setCurrentProgress( double progress );
    void setPartner( ProgressController* partner );

  private:
    ProgressController *myPartner;

    ProgressHandler handler;
    double endLimit;
    double currentProgress;

};


#endif // KPROGRESSCONTROLLER_H_INCLUDED
