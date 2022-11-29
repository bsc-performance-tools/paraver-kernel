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

const TRecordType VectorBlocks::commTypes[] =
{
  COMM + LOG + SEND,
  COMM + LOG + RECV,
  COMM + PHY + SEND,
  COMM + PHY + RECV,
  RSEND + LOG,
  RRECV + LOG,
  RSEND + PHY,
  RRECV + PHY
};

VectorBlocks::VectorBlocks( const ResourceModel<>& resource, const ProcessModel<>& process,
                            TRecordTime endTime )
    : resourceModel( resource ), processModel( process )
{
  TRecord empty;
  empty.time = 0;
  empty.type = EMPTYREC;
  empty.CPU = 0;

  std::vector<TRecord> tmpThreadVector;
  tmpThreadVector.emplace_back( empty );
  threadRecords.reserve( processModel.totalThreads() );
  threadRecords.insert( threadRecords.begin(), processModel.totalThreads(), tmpThreadVector );

  cpuRecords.reserve( resourceModel.totalCPUs() );
  cpuRecords.insert( cpuRecords.begin(), resourceModel.totalCPUs(), std::vector<TRecord *>() );

  for ( auto& commRecord : commRecords )
    commRecord.type = EMPTYREC;
}

TData *VectorBlocks::getLastRecord( PRV_UINT16 position ) const
{
  return nullptr;
}

void VectorBlocks::newRecord()
{
  inserted = false;
}

void VectorBlocks::setType( TRecordType whichType )
{
  if( inserted )
    threadRecords[ insertedOnThread ].back().type = whichType;
  else
    tmpRecord.type = whichType;
}

void VectorBlocks::setTime( TRecordTime whichTime )
{
  if( inserted )
    threadRecords[ insertedOnThread ].back().time = whichTime;
  else
    tmpRecord.time = whichTime;
}

void VectorBlocks::setThread( TThreadOrder whichThread )
{
  if( inserted )
    threadRecords[ insertedOnThread ].back().thread = whichThread;
  else
  {
    tmpRecord.thread = whichThread;
    insertedOnThread = whichThread;
    inserted = true;
  }
}

void VectorBlocks::setThread( TApplOrder whichAppl,
                              TTaskOrder whichTask,
                              TThreadOrder whichThread )
{
  whichThread = processModel.getGlobalThread( whichAppl,
                whichTask,
                whichThread );
  setThread( whichThread );
}

void VectorBlocks::setCPU( TCPUOrder whichCPU )
{
  if( inserted )
    threadRecords[ insertedOnThread ].back().CPU = whichCPU;
  else
    tmpRecord.CPU = whichCPU;

}

void VectorBlocks::setEventType( TEventType whichType )
{
  if( inserted )
    threadRecords[ insertedOnThread ].back().URecordInfo.eventRecord.type = whichType;
  else
    tmpRecord.URecordInfo.eventRecord.type = whichType;

}

void VectorBlocks::setEventValue( TEventValue whichValue )
{
  if( inserted )
    threadRecords[ insertedOnThread ].back().URecordInfo.eventRecord.value = whichValue;
  else
    tmpRecord.URecordInfo.eventRecord.value = whichValue;
}

void VectorBlocks::setState( TState whichState )
{
  if( inserted )
    threadRecords[ insertedOnThread ].back().URecordInfo.stateRecord.state = whichState;
  else
    tmpRecord.URecordInfo.stateRecord.state = whichState;
}

void VectorBlocks::setStateEndTime( TRecordTime whichTime )
{
  if( inserted )
    threadRecords[ insertedOnThread ].back().URecordInfo.stateRecord.endTime = whichTime;
  else
    tmpRecord.URecordInfo.stateRecord.endTime = whichTime;
}

void VectorBlocks::setCommIndex( TCommID whichID )
{
  if( inserted )
    threadRecords[ insertedOnThread ].back().URecordInfo.commRecord.index = whichID;
  else
    tmpRecord.URecordInfo.commRecord.index = whichID;
}

void VectorBlocks::newComm( bool createRecords )
{
  communications.emplace_back( TCommInfo() );
  if ( createRecords )
  {
    commRecordsInserted = false;
    for ( PRV_UINT8 i = 0; i < commTypeSize; ++i )
    {
      commRecords[ i ].type = commTypes[ i ];
      commRecords[ i ].URecordInfo.commRecord.index = communications.size() - 1;
    }
  }
}

void VectorBlocks::setSenderThread( TThreadOrder whichThread )
{
  communications.back().senderThread = whichThread;
}

void VectorBlocks::setSenderThread( TApplOrder whichAppl,
                                    TTaskOrder whichTask,
                                    TThreadOrder whichThread )
{
  TThreadOrder globalThread = processModel.getGlobalThread( whichAppl, whichTask, whichThread );
  setSenderThread( globalThread );
}

void VectorBlocks::setSenderCPU( TCPUOrder whichCPU )
{
  communications.back().senderCPU = whichCPU;

}

void VectorBlocks::setReceiverThread( TThreadOrder whichThread )
{
  communications.back().receiverThread = whichThread;

}

void VectorBlocks::setReceiverThread( TApplOrder whichAppl,
                                      TTaskOrder whichTask,
                                      TThreadOrder whichThread )
{
  TThreadOrder globalThread = processModel.getGlobalThread( whichAppl, whichTask, whichThread );
  setReceiverThread( globalThread );
}

void VectorBlocks::setReceiverCPU( TCPUOrder whichCPU )
{
  communications.back().receiverCPU = whichCPU;

}

void VectorBlocks::setCommTag( TCommTag whichTag )
{
  communications.back().tag = whichTag;

}

void VectorBlocks::setCommSize( TCommSize whichSize )
{
  communications.back().size = whichSize;

}

void VectorBlocks::setLogicalSend( TRecordTime whichTime )
{
  communications.back().logicalSendTime = whichTime;

}

void VectorBlocks::setLogicalReceive( TRecordTime whichTime )
{
  communications.back().logicalReceiveTime = whichTime;

}

void VectorBlocks::setPhysicalSend( TRecordTime whichTime )
{
  communications.back().physicalSendTime = whichTime;

}

void VectorBlocks::setPhysicalReceive( TRecordTime whichTime )
{
  communications.back().physicalReceiveTime = whichTime;

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
