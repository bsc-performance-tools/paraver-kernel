#include "progresscontroller.h"
#include "kernelconnection.h"

ProgressController *ProgressController::create( KernelConnection *whichKernel )
{
  return new ProgressControllerProxy( whichKernel );
}

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

void ProgressControllerProxy::setStop( bool value )
{
  myPartner->setStop( value );
}

bool ProgressControllerProxy::getStop() const
{
  return myPartner->getStop();
}

ProgressController *ProgressControllerProxy::getConcrete() const
{
  return myPartner;
}
