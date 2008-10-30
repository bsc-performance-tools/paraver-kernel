#include "kprogresscontroller.h"

KProgressController::KProgressController()
{
  endLimit = 0;
  currentProgress = 0;
}

KProgressController::~KProgressController()
{}

void KProgressController::setHandler( ProgressHandler whichHandler )
{
  handler = whichHandler;
}

void KProgressController::callHandler( ProgressController *not_used )
{
  handler( NULL );
}

double KProgressController::getEndLimit() const
{
  return endLimit;
}

void KProgressController::setEndLimit( double limit )
{
  endLimit = limit;
  myPartner->setEndLimit( limit );
}

double KProgressController::getCurrentProgress() const
{
  return currentProgress;
}

void KProgressController::setCurrentProgress( double progress )
{
  if( progress < currentProgress )
    return;
  currentProgress = progress;
  myPartner->callHandler( NULL );
}

void KProgressController::setPartner( ProgressController* partner )
{
  myPartner = partner;
}

