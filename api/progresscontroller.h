#ifndef PROGRESSCONTROLLER_H_INCLUDED
#define PROGRESSCONTROLLER_H_INCLUDED

#include "paraverkerneltypes.h"

class KernelConnection;

class ProgressController
{
  public:
    typedef void(*ProgressHandler)( ProgressController* );

    static ProgressController *create( KernelConnection *whichKernel );

    ProgressController() {};
    virtual ~ProgressController() {};

    virtual void setHandler( ProgressHandler whichHandler ) = 0;
    virtual void callHandler( ProgressController *not_used ) = 0;
    virtual double getEndLimit() const = 0;
    virtual void setEndLimit( double limit ) = 0;
    virtual double getCurrentProgress() const = 0;
    virtual void setCurrentProgress( double progress ) = 0;
    virtual void setPartner( ProgressController* partner ) = 0;
    virtual void setStop( bool value ) = 0;
    virtual bool getStop() const = 0;

    virtual ProgressController *getConcrete() const
    {
      return NULL;
    }
};


class ProgressControllerProxy:public ProgressController
{
  public:
    ~ProgressControllerProxy();

    void setHandler( ProgressHandler whichHandler );
    void callHandler( ProgressController *not_used );
    double getEndLimit() const;
    void setEndLimit( double limit );
    double getCurrentProgress() const;
    void setCurrentProgress( double progress );
    void setPartner( ProgressController* partner );
    virtual void setStop( bool value );
    virtual bool getStop() const;

    ProgressController *getConcrete() const;

  private:
    ProgressControllerProxy( KernelConnection *whichKernel );

    KernelConnection *myKernel;
    ProgressController *myPartner;

    ProgressHandler handler;
    double endLimit;
    double currentProgress;

    friend ProgressController *ProgressController::create( KernelConnection * );
};

#endif // PROGRESSCONTROLLER_H_INCLUDED
