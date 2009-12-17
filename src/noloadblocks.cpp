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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "noloadblocks.h"

using namespace NoLoad;

NoLoadBlocks::NoLoadBlocks()
{}

NoLoadBlocks::~NoLoadBlocks()
{}

TData *NoLoadBlocks::getLastRecord( UINT16 position ) const
{
  return NULL;
}

void NoLoadBlocks::newRecord()
{}

void NoLoadBlocks::setType( TRecordType whichType )
{}

void NoLoadBlocks::setTime( TRecordTime whichTime )
{}

void NoLoadBlocks::setThread( TThreadOrder whichThread )
{}

void NoLoadBlocks::setThread( TApplOrder whichAppl,
                              TTaskOrder whichTask,
                              TThreadOrder whichThread )
{}

void NoLoadBlocks::setCPU( TCPUOrder whichCPU )
{}

void NoLoadBlocks::setEventType( TEventType whichType )
{}

void NoLoadBlocks::setEventValue( TEventValue whichValue )
{}

void NoLoadBlocks::setState( TState whichState )
{}

void NoLoadBlocks::setStateEndTime( TRecordTime whichTime )
{}

void NoLoadBlocks::setCommIndex( TCommID whichID )
{}


void NoLoadBlocks::newComm( bool createRecords )
{}

void NoLoadBlocks::setSenderThread( TThreadOrder whichThread )
{}

void NoLoadBlocks::setSenderThread( TApplOrder whichAppl,
                                    TTaskOrder whichTask,
                                    TThreadOrder whichThread )
{}

void NoLoadBlocks::setSenderCPU( TCPUOrder whichCPU )
{}

void NoLoadBlocks::setReceiverThread( TThreadOrder whichThread )
{}

void NoLoadBlocks::setReceiverThread( TApplOrder whichAppl,
                                      TTaskOrder whichTask,
                                      TThreadOrder whichThread )
{}

void NoLoadBlocks::setReceiverCPU( TCPUOrder whichCPU )
{}

void NoLoadBlocks::setCommTag( TCommTag whichTag )
{}

void NoLoadBlocks::setCommSize( TCommSize whichSize )
{}

void NoLoadBlocks::setLogicalSend( TRecordTime whichTime )
{}

void NoLoadBlocks::setLogicalReceive( TRecordTime whichTime )
{}

void NoLoadBlocks::setPhysicalSend( TRecordTime whichTime )
{}

void NoLoadBlocks::setPhysicalReceive( TRecordTime whichTime )
{}

TCommID NoLoadBlocks::getTotalComms() const
{
  return 0;
}

TThreadOrder NoLoadBlocks::getSenderThread( TCommID whichComm ) const
{
  return 0;
}

TCPUOrder NoLoadBlocks::getSenderCPU( TCommID whichComm ) const
{
  return 0;
}

TThreadOrder NoLoadBlocks::getReceiverThread( TCommID whichComm ) const
{
  return 0;
}

TCPUOrder NoLoadBlocks::getReceiverCPU( TCommID whichComm ) const
{
  return 0;
}

TCommTag NoLoadBlocks::getCommTag( TCommID whichComm ) const
{
  return 0;
}

TCommSize NoLoadBlocks::getCommSize( TCommID whichComm ) const
{
  return 0;
}

TRecordTime NoLoadBlocks::getLogicalSend( TCommID whichComm ) const
{
  return 0;
}

TRecordTime NoLoadBlocks::getLogicalReceive( TCommID whichComm ) const
{
  return 0;
}

TRecordTime NoLoadBlocks::getPhysicalSend( TCommID whichComm ) const
{
  return 0;
}

TRecordTime NoLoadBlocks::getPhysicalReceive( TCommID whichComm ) const
{
  return 0;
}

TRecordTime NoLoadBlocks::getLastRecordTime() const
{
  return 0;
}
