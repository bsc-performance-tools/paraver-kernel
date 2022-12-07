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

#include <algorithm>

#include "plaintrace.h"
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

  for( auto& c : commRecords )
    c = nullptr;
}

TData *VectorBlocks::getLastRecord( PRV_UINT16 position ) const
{
  return nullptr;
}

void VectorBlocks::newRecord()
{
}

void VectorBlocks::newRecord( TThreadOrder whichThread )
{
  threadRecords[ whichThread ].emplace_back();
  insertedOnThread = whichThread;
  ++countInserted;
}

void VectorBlocks::setType( TRecordType whichType )
{
  threadRecords[ insertedOnThread ].back().type = whichType;
}

void VectorBlocks::setTime( TRecordTime whichTime )
{
  threadRecords[ insertedOnThread ].back().time = whichTime;

  if( whichTime > lastRecordTime )
    lastRecordTime = whichTime;
}

void VectorBlocks::setThread( TThreadOrder whichThread )
{
  threadRecords[ insertedOnThread ].back().thread = whichThread;
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
 threadRecords[ insertedOnThread ].back().CPU = whichCPU;
 }

void VectorBlocks::setEventType( TEventType whichType )
{
  threadRecords[ insertedOnThread ].back().URecordInfo.eventRecord.type = whichType;
}

void VectorBlocks::setEventValue( TEventValue whichValue )
{
  threadRecords[ insertedOnThread ].back().URecordInfo.eventRecord.value = whichValue;
}

void VectorBlocks::setState( TState whichState )
{
  threadRecords[ insertedOnThread ].back().URecordInfo.stateRecord.state = whichState;
}

void VectorBlocks::setStateEndTime( TRecordTime whichTime )
{
  threadRecords[ insertedOnThread ].back().URecordInfo.stateRecord.endTime = whichTime;
}

void VectorBlocks::setCommIndex( TCommID whichID )
{
  threadRecords[ insertedOnThread ].back().URecordInfo.commRecord.index = whichID;
}

void VectorBlocks::newComm( bool createRecords )
{
}

void VectorBlocks::newComm( TThreadOrder whichSenderThread, TThreadOrder whichReceiverThread, bool createRecords )
{
  communications.emplace_back( TCommInfo() );
  if( createRecords )
  {
    threadRecords[ whichSenderThread ].insert( threadRecords[ whichSenderThread ].end(), 4, Plain::TRecord() );
    threadRecords[ whichReceiverThread ].insert( threadRecords[ whichReceiverThread ].end(), 4, Plain::TRecord() );

    commRecords[ logicalSend ] = &threadRecords[ whichSenderThread ][ threadRecords[ whichSenderThread ].size() - 4 ];
    commRecords[ logicalReceive ] = &threadRecords[ whichReceiverThread ][ threadRecords[ whichReceiverThread ].size() - 4 ];
    commRecords[ physicalSend ] = &threadRecords[ whichSenderThread ][ threadRecords[ whichSenderThread ].size() - 3 ];
    commRecords[ physicalReceive ] = &threadRecords[ whichReceiverThread ][ threadRecords[ whichReceiverThread ].size() - 3 ];
    commRecords[ remoteLogicalSend ] = &threadRecords[ whichReceiverThread ][ threadRecords[ whichReceiverThread ].size() - 2 ];
    commRecords[ remoteLogicalReceive ] = &threadRecords[ whichSenderThread ][ threadRecords[ whichSenderThread ].size() - 2 ];
    commRecords[ remotePhysicalSend ] = &threadRecords[ whichReceiverThread ][ threadRecords[ whichReceiverThread ].size() - 1 ];
    commRecords[ remotePhysicalReceive ] = &threadRecords[ whichSenderThread ][ threadRecords[ whichSenderThread ].size() - 1 ];

    for( size_t i = 0; i < commTypeSize; ++i )
    {
      commRecords[ i ]->type = commTypes[ i ];
      commRecords[ i ]->URecordInfo.commRecord.index = communications.size() - 1;
    }

    countInserted += commTypeSize;
  }
}

void VectorBlocks::setSenderThread( TThreadOrder whichThread )
{
  communications.back().senderThread = whichThread;
  if( commRecords[ logicalSend ] != nullptr )
  {
    commRecords[ logicalSend ]->thread = whichThread;
    commRecords[ physicalSend ]->thread = whichThread;
    commRecords[ remoteLogicalReceive ]->thread = whichThread;
    commRecords[ remotePhysicalReceive ]->thread = whichThread;
  }
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
  if( commRecords[ logicalSend ] != nullptr )
  {
    commRecords[ logicalSend ]->CPU = whichCPU;
    commRecords[ physicalSend ]->CPU = whichCPU;
    commRecords[ remoteLogicalReceive ]->CPU = whichCPU;
    commRecords[ remotePhysicalReceive ]->CPU = whichCPU;
  }
}

void VectorBlocks::setReceiverThread( TThreadOrder whichThread )
{
  communications.back().receiverThread = whichThread;
  if( commRecords[ logicalSend ] != nullptr )
  {
    commRecords[ logicalReceive ]->thread = whichThread;
    commRecords[ physicalReceive ]->thread = whichThread;
    commRecords[ remoteLogicalSend ]->thread = whichThread;
    commRecords[ remotePhysicalSend ]->thread = whichThread;
  }
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
  if( commRecords[ logicalSend ] != nullptr )
  {
    commRecords[ logicalReceive ]->CPU = whichCPU;
    commRecords[ physicalReceive ]->CPU = whichCPU;
    commRecords[ remoteLogicalSend ]->CPU = whichCPU;
    commRecords[ remotePhysicalSend ]->CPU = whichCPU;
  }
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
  commRecords[ logicalSend ]->time = whichTime;
  commRecords[ remoteLogicalSend ]->time = whichTime;
}

void VectorBlocks::setLogicalReceive( TRecordTime whichTime )
{
  communications.back().logicalReceiveTime = whichTime;
  if( commRecords[ logicalSend ] != nullptr )
  {
    commRecords[ logicalReceive ]->time = whichTime;
    commRecords[ remoteLogicalReceive ]->time = whichTime;
  }
}

void VectorBlocks::setPhysicalSend( TRecordTime whichTime )
{
  communications.back().physicalSendTime = whichTime;
  if( commRecords[ logicalSend ] != nullptr )
  {
    commRecords[ physicalSend ]->time = whichTime;
    commRecords[ remotePhysicalSend ]->time = whichTime;
  }
}

void VectorBlocks::setPhysicalReceive( TRecordTime whichTime )
{
  communications.back().physicalReceiveTime = whichTime;
  if( commRecords[ logicalSend ] != nullptr )
  {
    commRecords[ physicalReceive ]->time = whichTime;
    commRecords[ remotePhysicalReceive ]->time = whichTime;
  }
}

TCommID VectorBlocks::getTotalComms() const
{
  return communications.size();
}

TThreadOrder VectorBlocks::getSenderThread( TCommID whichComm ) const
{
  return communications[whichComm].senderThread;
}

TCPUOrder VectorBlocks::getSenderCPU( TCommID whichComm ) const
{
  return communications[whichComm].senderCPU;
}

TThreadOrder VectorBlocks::getReceiverThread( TCommID whichComm ) const
{
  return communications[whichComm].receiverThread;
}

TCPUOrder VectorBlocks::getReceiverCPU( TCommID whichComm ) const
{
  return communications[whichComm].receiverCPU;
}

TCommTag VectorBlocks::getCommTag( TCommID whichComm ) const
{
  return communications[whichComm].tag;
}

TCommSize VectorBlocks::getCommSize( TCommID whichComm ) const
{
  return communications[whichComm].size;
}

TRecordTime VectorBlocks::getLogicalSend( TCommID whichComm ) const
{
  return communications[whichComm].logicalSendTime;
}

TRecordTime VectorBlocks::getLogicalReceive( TCommID whichComm ) const
{
  return communications[whichComm].logicalReceiveTime;
}

TRecordTime VectorBlocks::getPhysicalSend( TCommID whichComm ) const
{
  return communications[whichComm].physicalSendTime;
}

TRecordTime VectorBlocks::getPhysicalReceive( TCommID whichComm ) const
{
  return communications[whichComm].physicalReceiveTime;
}

TRecordTime VectorBlocks::getLastRecordTime() const
{
  return lastRecordTime;
}

void VectorBlocks::setFileLoaded( TRecordTime traceEndTime )
{

  TRecord tmpRecord;
  tmpRecord.type = EMPTYREC;
  tmpRecord.time = traceEndTime;

  #pragma omp parallel for firstprivate( tmpRecord ) shared( threadRecords ) default( none )
  for( auto& v : threadRecords )
  {
    v.shrink_to_fit();
    std::stable_sort( v.begin(), v.end(), 
      []( const TRecord& r1, const TRecord& r2 )
      {
        if ( r1.time < r2.time )
          return true;
        else if ( r1.time > r2.time )
          return false;
        return ( getTypeOrdered( &r1 ) < getTypeOrdered( &r2 ) );
      } );
    tmpRecord.thread = v.back().thread;
    v.emplace_back( tmpRecord );
  }
}
