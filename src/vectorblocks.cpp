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

#include "vectorblocks.h"

using namespace Plain;

TData *VectorBlocks::getLastRecord( PRV_UINT16 position ) const
{
  return nullptr;
}

void VectorBlocks::newRecord()
{

}

void VectorBlocks::setType( TRecordType whichType )
{

}

void VectorBlocks::setTime( TRecordTime whichTime )
{

}

void VectorBlocks::setThread( TThreadOrder whichThread )
{

}

void VectorBlocks::setThread( TApplOrder whichAppl,
                              TTaskOrder whichTask,
                              TThreadOrder whichThread )
{

}

void VectorBlocks::setCPU( TCPUOrder whichCPU )
{

}

void VectorBlocks::setEventType( TEventType whichType )
{

}

void VectorBlocks::setEventValue( TEventValue whichValue )
{

}

void VectorBlocks::setState( TState whichState )
{

}

void VectorBlocks::setStateEndTime( TRecordTime whichTime )
{

}

void VectorBlocks::setCommIndex( TCommID whichID )
{

}

void VectorBlocks::newComm( bool createRecords )
{

}

void VectorBlocks::setSenderThread( TThreadOrder whichThread )
{

}

void VectorBlocks::setSenderThread( TApplOrder whichAppl,
                                    TTaskOrder whichTask,
                                    TThreadOrder whichThread )
{

}

void VectorBlocks::setSenderCPU( TCPUOrder whichCPU )
{

}

void VectorBlocks::setReceiverThread( TThreadOrder whichThread )
{

}

void VectorBlocks::setReceiverThread( TApplOrder whichAppl,
                                      TTaskOrder whichTask,
                                      TThreadOrder whichThread )
{

}

void VectorBlocks::setReceiverCPU( TCPUOrder whichCPU )
{

}

void VectorBlocks::setCommTag( TCommTag whichTag )
{

}

void VectorBlocks::setCommSize( TCommSize whichSize )
{

}

void VectorBlocks::setLogicalSend( TRecordTime whichTime )
{

}

void VectorBlocks::setLogicalReceive( TRecordTime whichTime )
{

}

void VectorBlocks::setPhysicalSend( TRecordTime whichTime )
{

}

void VectorBlocks::setPhysicalReceive( TRecordTime whichTime )
{

}

TCommID VectorBlocks::getTotalComms() const
{
  return 0;
}

TThreadOrder VectorBlocks::getSenderThread( TCommID whichComm ) const
{
  return 0;
}

TCPUOrder VectorBlocks::getSenderCPU( TCommID whichComm ) const
{
  return 0;
}

TThreadOrder VectorBlocks::getReceiverThread( TCommID whichComm ) const
{
  return 0;
}

TCPUOrder VectorBlocks::getReceiverCPU( TCommID whichComm ) const
{
  return 0;
}

TCommTag VectorBlocks::getCommTag( TCommID whichComm ) const
{
  return 0;
}

TCommSize VectorBlocks::getCommSize( TCommID whichComm ) const
{
  return 0;
}

TRecordTime VectorBlocks::getLogicalSend( TCommID whichComm ) const
{
  return 0;
}

TRecordTime VectorBlocks::getLogicalReceive( TCommID whichComm ) const
{
  return 0;
}

TRecordTime VectorBlocks::getPhysicalSend( TCommID whichComm ) const
{
  return 0;
}

TRecordTime VectorBlocks::getPhysicalReceive( TCommID whichComm ) const
{
  return 0;
}

TRecordTime VectorBlocks::getLastRecordTime() const
{
  return 0;
}
