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

#include <string.h>
#include "plainblocks.h"
#include "paraverkernelexception.h"

using namespace Plain;

const TRecordType PlainBlocks::commTypes[] =
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

PlainBlocks::PlainBlocks( const ResourceModel& resource, const ProcessModel& process )
    : resourceModel( resource ), processModel( process )
{
  blocks.reserve( processModel.totalThreads() );
  currentBlock.reserve( processModel.totalThreads() );
  currentRecord.reserve( processModel.totalThreads() );
  for ( TThreadOrder iThread = 0; iThread < processModel.totalThreads(); ++iThread )
  {
    blocks.push_back( vector<TRecord *>() );
    blocks[ iThread ].push_back( NULL );
    currentBlock.push_back( NULL );
    currentRecord.push_back( 0 );
  }
}

PlainBlocks::~PlainBlocks()
{
  for ( TThreadOrder iThread = 0; iThread < processModel.totalThreads(); ++iThread )
  {
    if ( ( blocks[ iThread ] )[ 0 ] != NULL )
    {
      for ( UINT32 i = 0; i < blocks[ iThread ].size(); i++ )
        delete[] ( blocks[ iThread ] )[ i ];
    }
    blocks[ iThread ].clear();
  }

  for ( UINT32 i = 0; i < communications.size(); i++ )
    delete communications[i];

  blocks.clear();
  communications.clear();
}

TData *PlainBlocks::getLastRecord( UINT16 position ) const
{
  return (TData *)&lastRecords[ position ];
}

void PlainBlocks::resetCountInserted()
{
  MemoryBlocks::resetCountInserted();
  lastRecords.clear();
}

void PlainBlocks::newRecord()
{
  inserted = false;
}

void PlainBlocks::setType( TRecordType whichType )
{
  if ( inserted )
    currentBlock[ insertedOnThread ][ currentRecord[ insertedOnThread ] ].type = whichType;
  else
    tmpRecord.type = whichType;
}

void PlainBlocks::setTime( TRecordTime whichTime )
{
  if ( inserted )
    currentBlock[ insertedOnThread ][ currentRecord[ insertedOnThread ] ].time = whichTime;
  else
    tmpRecord.time = whichTime;
}

void PlainBlocks::setThread( TThreadOrder whichThread )
{
  if ( currentBlock[ whichThread ] == NULL )
  {
    blocks[ whichThread ][ 0 ] = new TRecord[ blockSize ];
    currentBlock[ whichThread ] = blocks[ whichThread ][ 0 ];
    TRecord empty;
    empty.time = 0;
    empty.type = EMPTYREC;
    empty.CPU = 0;
    currentBlock[ whichThread ][ 0 ] = empty;
    currentRecord[ whichThread ] = 1;
  }
  else
  {
    ++currentRecord[ whichThread ];
    if ( currentRecord[ whichThread ] == blockSize )
    {
      blocks[ whichThread ].push_back( new TRecord[ blockSize ] );
      currentBlock[ whichThread ] = blocks[ whichThread ][ blocks[ whichThread ].size() - 1 ];
      currentRecord[ whichThread ] = 0;
    }
  }
  currentBlock[ whichThread ][ currentRecord[ whichThread ] ] = tmpRecord;
  inserted = true;
  insertedOnThread = whichThread;

  TLastRecord tmpLast;
  tmpLast.time = tmpRecord.time;
  tmpLast.thread = whichThread;
  tmpLast.block = blocks[ whichThread ].size() - 1;
  tmpLast.pos = currentRecord[ whichThread ];
  lastRecords.push_back( tmpLast );
  ++countInserted;
}

void PlainBlocks::setThread( TApplOrder whichAppl,
                             TTaskOrder whichTask,
                             TThreadOrder whichThread )
{
  whichThread = processModel.getGlobalThread( whichAppl,
                whichTask,
                whichThread );

  if ( currentBlock[ whichThread ] == NULL )
  {
    blocks[ whichThread ][ 0 ] = new TRecord[ blockSize ];
    memset( blocks[ whichThread ][ 0 ], 0, blockSize * sizeof( TRecord ) );
    currentBlock[ whichThread ] = blocks[ whichThread ][ 0 ];
    currentRecord[ whichThread ] = 0;
  }
  else
  {
    ++currentRecord[ whichThread ];
    if ( currentRecord[ whichThread ] == blockSize )
    {
      blocks[ whichThread ].push_back( new TRecord[ blockSize ] );
      memset( blocks[ whichThread ][ blocks[ whichThread ].size() - 1 ], 0, blockSize * sizeof( TRecord ) );
      currentBlock[ whichThread ] = blocks[ whichThread ][ blocks.size() - 1 ];
      currentRecord[ whichThread ] = 0;
    }
  }
  currentBlock[ whichThread ][ currentRecord[ whichThread ] ] = tmpRecord;
  inserted = true;
  insertedOnThread = whichThread;

  TLastRecord tmpLast;
  tmpLast.thread = whichThread;
  tmpLast.block = blocks[ whichThread ].size() - 1;
  tmpLast.pos = currentRecord[ whichThread ];
  lastRecords.push_back( tmpLast );
  ++countInserted;
}

void PlainBlocks::setCPU( TCPUOrder whichCPU )
{
  if ( inserted )
    currentBlock[ insertedOnThread ][ currentRecord[ insertedOnThread ] ].CPU = whichCPU;
  else
    tmpRecord.CPU = whichCPU;
}

void PlainBlocks::setEventType( TEventType whichType )
{
  if ( inserted )
    currentBlock[ insertedOnThread ][ currentRecord[ insertedOnThread ] ].URecordInfo.eventRecord.type = whichType;
  else
    tmpRecord.URecordInfo.eventRecord.type = whichType;
}

void PlainBlocks::setEventValue( TEventValue whichValue )
{
  if ( inserted )
    currentBlock[ insertedOnThread ][ currentRecord[ insertedOnThread ] ].URecordInfo.eventRecord.value = whichValue;
  else
    tmpRecord.URecordInfo.eventRecord.value = whichValue;
}

void PlainBlocks::setState( TState whichState )
{
  if ( inserted )
    currentBlock[ insertedOnThread ][ currentRecord[ insertedOnThread ] ].URecordInfo.stateRecord.state = whichState;
  else
    tmpRecord.URecordInfo.stateRecord.state = whichState;
}

void PlainBlocks::setStateEndTime( TRecordTime whichTime )
{
  if ( inserted )
    currentBlock[ insertedOnThread ][ currentRecord[ insertedOnThread ] ].URecordInfo.stateRecord.endTime = whichTime;
  else
    tmpRecord.URecordInfo.stateRecord.endTime = whichTime;
}

void PlainBlocks::setCommIndex( TCommID whichID )
{
  if ( inserted )
    currentBlock[ insertedOnThread ][ currentRecord[ insertedOnThread ] ].URecordInfo.commRecord.index = whichID;
  else
    tmpRecord.URecordInfo.commRecord.index = whichID;
}

void PlainBlocks::newComm( bool createRecords )
{
  if ( createRecords )
    throw ParaverKernelException();

  communications.push_back( new TCommInfo() );
  currentComm = communications.size() - 1;
}

void PlainBlocks::setSenderThread( TThreadOrder whichThread )
{
  communications[currentComm]->senderThread = whichThread;
}

void PlainBlocks::setSenderThread( TApplOrder whichAppl,
                                   TTaskOrder whichTask,
                                   TThreadOrder whichThread )
{
  TThreadOrder globalThread = processModel.getGlobalThread( whichAppl,
                              whichTask,
                              whichThread );
  communications[currentComm]->senderThread = globalThread;
}

void PlainBlocks::setSenderCPU( TCPUOrder whichCPU )
{
  communications[currentComm]->senderCPU = whichCPU;
}

void PlainBlocks::setReceiverThread( TThreadOrder whichThread )
{
  communications[currentComm]->receiverThread = whichThread;
}

void PlainBlocks::setReceiverThread( TApplOrder whichAppl,
                                     TTaskOrder whichTask,
                                     TThreadOrder whichThread )
{
  TThreadOrder globalThread = processModel.getGlobalThread( whichAppl,
                              whichTask,
                              whichThread );
  communications[currentComm]->receiverThread = globalThread;
}

void PlainBlocks::setReceiverCPU( TCPUOrder whichCPU )
{
  communications[currentComm]->receiverCPU = whichCPU;
}

void PlainBlocks::setCommTag( TCommTag whichTag )
{
  communications[currentComm]->tag = whichTag;
}

void PlainBlocks::setCommSize( TCommSize whichSize )
{
  communications[currentComm]->size = whichSize;
}

void PlainBlocks::setLogicalSend( TRecordTime whichTime )
{
  communications[currentComm]->logicalSendTime = whichTime;
}

void PlainBlocks::setLogicalReceive( TRecordTime whichTime )
{
  communications[currentComm]->logicalReceiveTime = whichTime;
}

void PlainBlocks::setPhysicalSend( TRecordTime whichTime )
{
  communications[currentComm]->physicalSendTime = whichTime;
}

void PlainBlocks::setPhysicalReceive( TRecordTime whichTime )
{
  communications[currentComm]->physicalReceiveTime = whichTime;
}

TCommID PlainBlocks::getTotalComms() const
{
  return communications.size();
}

TThreadOrder PlainBlocks::getSenderThread( TCommID whichComm ) const
{
  return communications[whichComm]->senderThread;
}

TCPUOrder PlainBlocks::getSenderCPU( TCommID whichComm ) const
{
  return communications[whichComm]->senderCPU;
}

TThreadOrder PlainBlocks::getReceiverThread( TCommID whichComm ) const
{
  return communications[whichComm]->receiverThread;
}

TCPUOrder PlainBlocks::getReceiverCPU( TCommID whichComm ) const
{
  return communications[whichComm]->receiverCPU;
}

TCommTag PlainBlocks::getCommTag( TCommID whichComm ) const
{
  return communications[whichComm]->tag;
}

TCommSize PlainBlocks::getCommSize( TCommID whichComm ) const
{
  return communications[whichComm]->size;
}

TRecordTime PlainBlocks::getLogicalSend( TCommID whichComm ) const
{
  return communications[whichComm]->logicalSendTime;
}

TRecordTime PlainBlocks::getLogicalReceive( TCommID whichComm ) const
{
  return communications[whichComm]->logicalReceiveTime;
}

TRecordTime PlainBlocks::getPhysicalSend( TCommID whichComm ) const
{
  return communications[whichComm]->physicalSendTime;
}

TRecordTime PlainBlocks::getPhysicalReceive( TCommID whichComm ) const
{
  return communications[whichComm]->physicalReceiveTime;
}

TRecordTime PlainBlocks::getLastRecordTime() const
{
  TThreadOrder thread = lastRecords[ lastRecords.size() - 1 ].thread;
  UINT32 block = lastRecords[ lastRecords.size() - 1 ].block;
  UINT32 pos = lastRecords[ lastRecords.size() - 1 ].pos;
  TRecordTime time = blocks[ thread ][ block ][ pos ].time;

  return time;
}
