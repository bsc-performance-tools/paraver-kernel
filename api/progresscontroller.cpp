#include "progresscontroller.h"
#include "kernelconnection.h"

ProgressController *ProgressController::create( KernelConnection *whichKernel )
{
  return new ProgressControllerProxy( whichKernel );
}

ProgressController::ProgressController()
{}

ProgressController::~ProgressController()
{}

ProgressControllerProxy::ProgressControllerProxy( KernelConnection * whichKernel )
: myKernel( whichKernel )
{
  setPartner( myKernel->newProgressController() );
}

ProgressControllerProxy::~ProgressControllerProxy()
{}

void ProgressControllerProxy::setHandler( ProgressHandler whichHandler )
{
  handler = whichHandler;
}

INT64 ProgressControllerProxy::getEndLimit() const
{
  return endLimit;
}

void ProgressControllerProxy::setEndLimit( INT64 limit )
{
  endLimit = limit;
}

INT64 ProgressControllerProxy::getCurrentProgress() const
{
  return currentProgress;
}

void ProgressControllerProxy::setCurrentProgress( INT64 progress )
{
  currentProgress = progress;
}

void ProgressControllerProxy::setPartner( ProgressController* partner )
{
  myPartner = partner;
  myPartner->setPartner( this );
}

void ProgressControllerProxy::callHandler( ProgressController *not_used )
{
  handler( this );
}
