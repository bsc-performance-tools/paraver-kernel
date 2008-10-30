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
    INT64 getEndLimit() const;
    void setEndLimit( INT64 limit );
    INT64 getCurrentProgress() const;
    void setCurrentProgress( INT64 progress );
    void setPartner( ProgressController* partner );

  private:
    ProgressController *myPartner;

    ProgressHandler handler;
    INT64 endLimit;
    INT64 currentProgress;

};


#endif // KPROGRESSCONTROLLER_H_INCLUDED
