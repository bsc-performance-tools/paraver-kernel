#ifndef PROGRESSCONTROLLER_H_INCLUDED
#define PROGRESSCONTROLLER_H_INCLUDED

#include "paraverkerneltypes.h"

class KernelConnection;

class ProgressController
{
  public:
    typedef void (*ProgressHandler)( ProgressController* );

    static ProgressController *create( KernelConnection *whichKernel );

    ProgressController();
    virtual ~ProgressController();

    virtual void setHandler( ProgressHandler whichHandler ) = 0;
    virtual void callHandler( ProgressController *not_used ) = 0;
    virtual INT64 getEndLimit() const = 0;
    virtual void setEndLimit( INT64 limit ) = 0;
    virtual INT64 getCurrentProgress() const = 0;
    virtual void setCurrentProgress( INT64 progress ) = 0;
    virtual void setPartner( ProgressController* partner ) = 0;

};


class ProgressControllerProxy:public ProgressController
{
  public:
    ~ProgressControllerProxy();

    void setHandler( ProgressHandler whichHandler );
    void callHandler( ProgressController *not_used );
    INT64 getEndLimit() const;
    void setEndLimit( INT64 limit );
    INT64 getCurrentProgress() const;
    void setCurrentProgress( INT64 progress );
    void setPartner( ProgressController* partner );

  private:
    ProgressControllerProxy( KernelConnection *whichKernel );

    KernelConnection *myKernel;
    ProgressController *myPartner;

    ProgressHandler handler;
    INT64 endLimit;
    INT64 currentProgress;

    friend ProgressController *ProgressController::create( KernelConnection * );
};

#endif // PROGRESSCONTROLLER_H_INCLUDED
