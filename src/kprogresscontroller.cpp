/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/


#include "kprogresscontroller.h"

KProgressController::KProgressController()
{
  endLimit = 0;
  currentProgress = 0;
  stop = false;
}

KProgressController::~KProgressController()
{}

void KProgressController::setHandler( void* whichProgressDialog, ProgressHandler whichHandler )
{
  handler = whichHandler;
}

void KProgressController::callHandler( ProgressController *not_used )
{}

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
  myPartner->callHandler( nullptr );
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

void KProgressController::setMessage( std::string whichMessage )
{
  myPartner->setMessage( whichMessage );
}

std::string KProgressController::getMessage() const
{
  return myPartner->getMessage();
}

void KProgressController::clearMessageChanged()
{
  myPartner->clearMessageChanged();
}

bool KProgressController::getMessageChanged() const
{
  return myPartner->getMessageChanged();
}

void KProgressController::setLastUpdate( double whichLastUpdate )
{
  myPartner->setLastUpdate( whichLastUpdate );
}

double KProgressController::getLastUpdate() const
{
  return myPartner->getLastUpdate();
}
