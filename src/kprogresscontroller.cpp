#include "kprogresscontroller.h"

KProgressController::KProgressController()
{}

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

INT64 KProgressController::getEndLimit() const
{
  return endLimit;
}

void KProgressController::setEndLimit( INT64 limit )
{
  endLimit = limit;
}

INT64 KProgressController::getCurrentProgress() const
{
  return currentProgress;
}

void KProgressController::setCurrentProgress( INT64 progress )
{
  currentProgress = progress;
  myPartner->setCurrentProgress( progress );
  myPartner->callHandler( NULL );
}

void KProgressController::setPartner( ProgressController* partner )
{
  myPartner = partner;
}

