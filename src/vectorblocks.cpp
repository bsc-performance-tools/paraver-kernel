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

#include "kprogresscontroller.h"
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

VectorBlocks::VectorBlocks( const ResourceModel<>& resource,
                            const ProcessModel<>& process,
                            TRecordTime endTime,
                            ProgressController *whichProgress )
    : resourceModel( resource ), processModel( process ), progress( whichProgress )
{
  TRecord empty;
  empty.time = 0;
  empty.type = EMPTYREC;
  empty.CPU = 0;

  TThreadRecordContainer tmpThreadVector;
  tmpThreadVector.emplace_back( empty );
  threadRecords.reserve( processModel.totalThreads() );
  threadRecords.insert( threadRecords.begin(), processModel.totalThreads(), tmpThreadVector );

  cpuRecords.reserve( resourceModel.totalCPUs() );
  cpuRecords.insert( cpuRecords.begin(), resourceModel.totalCPUs(), TCPURecordContainer() );

  cpuBeginEmptyRecords.reserve( resourceModel.totalCPUs() );
  cpuBeginEmptyRecords.insert( cpuBeginEmptyRecords.begin(), resourceModel.totalCPUs(), Plain::TRecord() );

  cpuEndEmptyRecords.reserve( resourceModel.totalCPUs() );
  cpuEndEmptyRecords.insert( cpuEndEmptyRecords.begin(), resourceModel.totalCPUs(), Plain::TRecord() );

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

void VectorBlocks::setRecordType( TRecordType whichType )
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
    TThreadRecordContainer::iterator itSender;
    TThreadRecordContainer::iterator itReceiver;

    if( whichSenderThread != whichReceiverThread )
    {
      itSender   = threadRecords[ whichSenderThread ].insert( threadRecords[ whichSenderThread ].end(), 4, Plain::TRecord() );
      itReceiver = threadRecords[ whichReceiverThread ].insert( threadRecords[ whichReceiverThread ].end(), 4, Plain::TRecord() );
    }
    else
    {
      itSender = threadRecords[ whichSenderThread ].insert( threadRecords[ whichSenderThread ].end(), 8, Plain::TRecord() );
      itReceiver = itSender + 4;
    }

    commRecords[ logicalSend ] = &( *itSender );
    commRecords[ logicalReceive ] = &( *itReceiver );
    commRecords[ physicalSend ] = &( *++itSender );
    commRecords[ physicalReceive ] = &( *++itReceiver );

    commRecords[ remoteLogicalSend ] = &( *++itReceiver );
    commRecords[ remoteLogicalReceive ] = &( *++itSender );
    commRecords[ remotePhysicalSend ] = &( *++itReceiver );
    commRecords[ remotePhysicalReceive ] = &( *++itSender );

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
  TRecord beginEmptyRecord;
  beginEmptyRecord.type = EMPTYREC;
  beginEmptyRecord.time = 0.0;

  TRecord endEmptyRecord;
  endEmptyRecord.type = EMPTYREC;
  endEmptyRecord.time = traceEndTime;

  auto compareRecords = []( const TRecord& r1, const TRecord& r2 )
      {
        if ( r1.time < r2.time )
          return true;
        else if ( r1.time > r2.time )
          return false;
        return ( getTypeOrdered( &r1 ) < getTypeOrdered( &r2 ) );
      };

  auto sortThread = [this, compareRecords]( size_t iThread, TRecord endEmptyRecord )
      {
        auto &vectorThread = threadRecords[ iThread ];
        std::stable_sort( ++vectorThread.begin(), vectorThread.end(), compareRecords );

        endEmptyRecord.thread = iThread;
        endEmptyRecord.CPU = vectorThread.back().CPU;
        vectorThread.emplace_back( endEmptyRecord );
        vectorThread.shrink_to_fit();
      };

  size_t progressCounter = 0;
#ifndef _WIN32
  if ( progress != nullptr )
  {
    progress->setMessage( "Sorting trace..." );
    progress->setCurrentProgress( progressCounter );
  }
#endif
  if( !resourceModel.isReady() )
  {
#ifndef _WIN32
    if ( progress != nullptr )
      progress->setEndLimit( processModel.totalThreads() );
#endif

    size_t iThread;
    #pragma omp parallel for firstprivate( endEmptyRecord ) \
                             private( iThread ) \
                             default( shared )
    for( iThread = 0; iThread < processModel.totalThreads(); ++iThread )
    {
      sortThread( iThread, endEmptyRecord );
#ifndef _WIN32
      if ( progress != nullptr )
      {
        #pragma omp atomic
        ++progressCounter;
        #pragma omp critical
        {
          if( processModel.totalThreads() < 128 ||
              progressCounter % 16 == 0 )
            progress->setCurrentProgress( progressCounter );
        }
      }
#endif
    }
  }
  else
  {
#ifndef _WIN32
    if ( progress != nullptr )
      progress->setEndLimit( resourceModel.totalCPUs() );
#endif

    size_t iNode;
    #pragma omp parallel for firstprivate( beginEmptyRecord, endEmptyRecord ) \
                             private( iNode ) \
                             default( shared )
    for( iNode = 0; iNode < resourceModel.size(); ++iNode )
    {
      std::vector<TThreadOrder> threadsInNode;
      processModel.getThreadsPerNode( iNode + 1, threadsInNode );

      for( TCPUOrder iCPU = resourceModel.getFirstCPU( iNode ); iCPU <= resourceModel.getLastCPU( iNode ); ++iCPU )
      {
        cpuBeginEmptyRecords[ iCPU - 1 ] = beginEmptyRecord;
        cpuBeginEmptyRecords[ iCPU - 1 ].CPU = iCPU;
        if ( !threadsInNode.empty() )
          cpuBeginEmptyRecords[ iCPU - 1 ].thread = threadsInNode[ 0 ];
        else
          cpuBeginEmptyRecords[ iCPU - 1 ].thread = 0;
        cpuRecords[ iCPU - 1 ].emplace_back( &cpuBeginEmptyRecords[ iCPU - 1 ] );
      }

      for( auto iThread : threadsInNode )
      {
        sortThread( iThread, endEmptyRecord );

        auto &vectorThread = threadRecords[ iThread ];
        auto itRecord = vectorThread.begin();

        auto itEmptyRecord = --vectorThread.end();

        // skip empty record
        if( itRecord != itEmptyRecord )
          ++itRecord;

        for( ; itRecord != itEmptyRecord; ++itRecord )
        {
          if( itRecord->CPU != 0 )
            cpuRecords[ itRecord->CPU - 1 ].emplace_back( &( *itRecord ) );
        }
      }

      for( TCPUOrder iCPU = resourceModel.getFirstCPU( iNode ); iCPU <= resourceModel.getLastCPU( iNode ); ++iCPU )
      {
        std::stable_sort( ++cpuRecords[ iCPU - 1 ].begin(), cpuRecords[ iCPU - 1 ].end(),
                          []( const auto& lhs, const auto& rhs ){ return lhs->time < rhs->time; } );

        cpuEndEmptyRecords[ iCPU - 1 ] = endEmptyRecord;
        cpuEndEmptyRecords[ iCPU - 1 ].CPU = iCPU;
        cpuEndEmptyRecords[ iCPU - 1 ].thread = cpuRecords[ iCPU - 1 ].back()->thread;
        cpuRecords[ iCPU - 1 ].emplace_back( &cpuEndEmptyRecords[ iCPU - 1 ] );

        cpuBeginEmptyRecords[ iCPU - 1 ].thread = cpuRecords[ iCPU - 1 ][ 1 ]->thread;

        cpuRecords[ iCPU - 1 ].shrink_to_fit();

#ifndef _WIN32
        if ( progress != nullptr )
        {
          #pragma omp atomic
          ++progressCounter;
          #pragma omp critical
          {
            if( resourceModel.totalCPUs() < 128 ||
                progressCounter % 16 == 0 )
              progress->setCurrentProgress( progressCounter );
          }
        }
#endif
      }
    }
  }
}
