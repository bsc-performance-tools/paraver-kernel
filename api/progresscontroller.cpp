/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

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
  message = "";
  messageChanged = false;
}

ProgressControllerProxy::~ProgressControllerProxy()
{}

void ProgressControllerProxy::setHandler( ProgressHandler whichHandler, void *callerWindow )
{
  handler = whichHandler;
  window = callerWindow;
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
  myPartner->setCurrentProgress( currentProgress );
  callHandler( NULL );
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
    handler( this, window );
}

void ProgressControllerProxy::setStop( bool value )
{
  myPartner->setStop( value );
}

bool ProgressControllerProxy::getStop() const
{
  return myPartner->getStop();
}

void ProgressControllerProxy::setMessage( std::string whichMessage )
{
  message = whichMessage;
  messageChanged = true;
}

std::string ProgressControllerProxy::getMessage() const
{
  return message;
}

void ProgressControllerProxy::clearMessageChanged()
{
  messageChanged = false;
}

bool ProgressControllerProxy::getMessageChanged() const
{
  return messageChanged;
}

ProgressController *ProgressControllerProxy::getConcrete() const
{
  return myPartner;
}
