#include "kprogresscontroller.h"

KProgressController::KProgressController()
{
  endLimit = 0;
  currentProgress = 0;
  stop = false;
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
  currentProgress = progress;
  myPartner->callHandler( NULL );
}

void KProgressController::setPartner( ProgressController* partner )
{
  myPartner = partner;
}

void KProgressController::setStop( bool value )
{
  stop = value;
}

bool KProgressController::getStop() const
{
  return stop;
}
