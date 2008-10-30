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
  handler = NULL;
}

ProgressControllerProxy::~ProgressControllerProxy()
{}

void ProgressControllerProxy::setHandler( ProgressHandler whichHandler )
{
  handler = whichHandler;
}

double ProgressControllerProxy::getEndLimit() const
{
  return endLimit;
}

void ProgressControllerProxy::setEndLimit( double limit )
{
  endLimit = limit;
}

double ProgressControllerProxy::getCurrentProgress() const
{
  return currentProgress;
}

void ProgressControllerProxy::setCurrentProgress( double progress )
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
  currentProgress = myPartner->getCurrentProgress();
  if( handler != NULL )
    handler( this );
}

ProgressController *ProgressControllerProxy::getConcrete() const
{
  return myPartner;
}
