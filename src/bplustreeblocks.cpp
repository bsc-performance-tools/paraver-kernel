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
#include "bplustreeblocks.h"

using namespace bplustree;

const TRecordType BPlusTreeBlocks::commTypes[] =
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

/****************************************
 * Record blocks management methods
 ****************************************/
void BPlusTreeBlocks::newRecord()
{
  if ( currentBlock == NULL )
  {
    blocks[0] = new TRecord[blockSize];
    memset( blocks[0], 0, blockSize * sizeof( TRecord ) );
    currentBlock = blocks[0];
    currentRecord = 0;
  }
  else
  {
    ++currentRecord;
    if ( currentRecord == blockSize )
    {
      blocks.push_back( new TRecord[blockSize] );
      memset( blocks[blocks.size() - 1], 0, blockSize * sizeof( TRecord ) );
      currentBlock = blocks[blocks.size() - 1];
      currentRecord = 0;
    }
  }
  currentBlock[currentRecord].next = NULL;
  currentBlock[currentRecord].prev = NULL;
  currentBlock[currentRecord].threadNext = NULL;
  currentBlock[currentRecord].threadPrev = NULL;
  lastRecords.push_back( &currentBlock[currentRecord] );
  ++countInserted;
}

void BPlusTreeBlocks::setType( TRecordType whichType )
{
  currentBlock[currentRecord].type = whichType;
}

void BPlusTreeBlocks::setTime( TRecordTime whichTime )
{
  currentBlock[currentRecord].time = whichTime;
}

void BPlusTreeBlocks::setThread( TThreadOrder whichThread )
{
  currentBlock[currentRecord].thread = whichThread;
}

void BPlusTreeBlocks::setThread( TApplOrder whichAppl,
                                 TTaskOrder whichTask,
                                 TThreadOrder whichThread )
{
  currentBlock[currentRecord].thread = traceModel.getGlobalThread( whichAppl,
                                       whichTask,
                                       whichThread );
}

void BPlusTreeBlocks::setCPU( TCPUOrder whichCPU )
{
  currentBlock[currentRecord].CPU = whichCPU;
}

void BPlusTreeBlocks::setEventType( TEventType whichType )
{
  currentBlock[currentRecord].URecordInfo.eventRecord.type = whichType;
}

void BPlusTreeBlocks::setEventValue( TEventValue whichValue )
{
  currentBlock[currentRecord].URecordInfo.eventRecord.value = whichValue;
}

void BPlusTreeBlocks::setState( TState whichState )
{
  currentBlock[currentRecord].URecordInfo.stateRecord.state = whichState;
}

void BPlusTreeBlocks::setStateEndTime( TRecordTime whichTime )
{
  currentBlock[currentRecord].URecordInfo.stateRecord.endTime = whichTime;
}

void BPlusTreeBlocks::setCommIndex( TCommID whichID )
{
  currentBlock[currentRecord].URecordInfo.commRecord.index = whichID;
}


/****************************************
 * Communications management methods
 ****************************************/
void BPlusTreeBlocks::newComm( bool createRecords )
{
  communications.push_back( new TCommInfo() );
  currentComm = communications.size() - 1;
  if ( !createRecords )
  {
    for ( PRV_UINT8 i = 0; i < commTypeSize; ++i )
      commRecords[ i ] = NULL;
  }
  else
  {
    for ( PRV_UINT8 i = 0; i < commTypeSize; ++i )
    {
      newRecord();
      commRecords[i] = &currentBlock[currentRecord];
      setType( commTypes[i] );
      setCommIndex( currentComm );
    }
  }
}

void BPlusTreeBlocks::setSenderThread( TThreadOrder whichThread )
{
  communications[currentComm]->senderThread = whichThread;
  if ( commRecords[logicalSend] != NULL )
  {
    commRecords[logicalSend]->thread = whichThread;
    commRecords[physicalSend]->thread = whichThread;
    commRecords[remoteLogicalReceive]->thread = whichThread;
    commRecords[remotePhysicalReceive]->thread = whichThread;
  }
}

void BPlusTreeBlocks::setSenderThread( TApplOrder whichAppl,
                                       TTaskOrder whichTask,
                                       TThreadOrder whichThread )
{
  TThreadOrder globalThread = traceModel.getGlobalThread( whichAppl,
                              whichTask,
                              whichThread );
  communications[currentComm]->senderThread = globalThread;
  if ( commRecords[logicalSend] != NULL )
  {
    commRecords[logicalSend]->thread = globalThread;
    commRecords[physicalSend]->thread = globalThread;
    commRecords[remoteLogicalReceive]->thread = globalThread;
    commRecords[remotePhysicalReceive]->thread = globalThread;
  }
}

void BPlusTreeBlocks::setSenderCPU( TCPUOrder whichCPU )
{
  communications[currentComm]->senderCPU = whichCPU;
  if ( commRecords[logicalSend] != NULL )
  {
    commRecords[logicalSend]->CPU = whichCPU;
    commRecords[physicalSend]->CPU = whichCPU;
    commRecords[remoteLogicalReceive]->CPU = whichCPU;
    commRecords[remotePhysicalReceive]->CPU = whichCPU;
  }
}

void BPlusTreeBlocks::setReceiverThread( TThreadOrder whichThread )
{
  communications[currentComm]->receiverThread = whichThread;
  if ( commRecords[logicalReceive] != NULL )
  {
    commRecords[logicalReceive]->thread = whichThread;
    commRecords[physicalReceive]->thread = whichThread;
    commRecords[remoteLogicalSend]->thread = whichThread;
    commRecords[remotePhysicalSend]->thread = whichThread;
  }
}

void BPlusTreeBlocks::setReceiverThread( TApplOrder whichAppl,
    TTaskOrder whichTask,
    TThreadOrder whichThread )
{
  TThreadOrder globalThread = traceModel.getGlobalThread( whichAppl,
                              whichTask,
                              whichThread );
  communications[currentComm]->receiverThread = globalThread;
  if ( commRecords[logicalReceive] != NULL )
  {
    commRecords[logicalReceive]->thread = globalThread;
    commRecords[physicalReceive]->thread = globalThread;
    commRecords[remoteLogicalSend]->thread = globalThread;
    commRecords[remotePhysicalSend]->thread = globalThread;
  }
}

void BPlusTreeBlocks::setReceiverCPU( TCPUOrder whichCPU )
{
  communications[currentComm]->receiverCPU = whichCPU;
  if ( commRecords[logicalReceive] != NULL )
  {
    commRecords[logicalReceive]->CPU = whichCPU;
    commRecords[physicalReceive]->CPU = whichCPU;
    commRecords[remoteLogicalSend]->CPU = whichCPU;
    commRecords[remotePhysicalSend]->CPU = whichCPU;
  }
}

void BPlusTreeBlocks::setCommTag( TCommTag whichTag )
{
  communications[currentComm]->tag = whichTag;
}

void BPlusTreeBlocks::setCommSize( TCommSize whichSize )
{
  communications[currentComm]->size = whichSize;
}

void BPlusTreeBlocks::setLogicalSend( TRecordTime whichTime )
{
  communications[currentComm]->logicalSendTime = whichTime;
  if ( commRecords[logicalSend] != NULL )
  {
    commRecords[logicalSend]->time = whichTime;
    commRecords[remoteLogicalSend]->time = whichTime;
  }
}

void BPlusTreeBlocks::setLogicalReceive( TRecordTime whichTime )
{
  communications[currentComm]->logicalReceiveTime = whichTime;
  if ( commRecords[logicalReceive] != NULL )
  {
    commRecords[logicalReceive]->time = whichTime;
    commRecords[remoteLogicalReceive]->time = whichTime;
  }
}

void BPlusTreeBlocks::setPhysicalSend( TRecordTime whichTime )
{
  communications[currentComm]->physicalSendTime = whichTime;
  if ( commRecords[physicalSend] != NULL )
  {
    commRecords[physicalSend]->time = whichTime;
    commRecords[remotePhysicalSend]->time = whichTime;
  }
}

void BPlusTreeBlocks::setPhysicalReceive( TRecordTime whichTime )
{
  communications[currentComm]->physicalReceiveTime = whichTime;
  if ( commRecords[physicalReceive] != NULL )
  {
    commRecords[physicalReceive]->time = whichTime;
    commRecords[remotePhysicalReceive]->time = whichTime;
  }
}

TCommID BPlusTreeBlocks::getTotalComms() const
{
  return communications.size();
}

TThreadOrder BPlusTreeBlocks::getSenderThread( TCommID whichComm ) const
{
  return communications[whichComm]->senderThread;
}

TCPUOrder BPlusTreeBlocks::getSenderCPU( TCommID whichComm ) const
{
  return communications[whichComm]->senderCPU;
}

TThreadOrder BPlusTreeBlocks::getReceiverThread( TCommID whichComm ) const
{
  return communications[whichComm]->receiverThread;
}

TCPUOrder BPlusTreeBlocks::getReceiverCPU( TCommID whichComm ) const
{
  return communications[whichComm]->receiverCPU;
}

TCommTag BPlusTreeBlocks::getCommTag( TCommID whichComm ) const
{
  return communications[whichComm]->tag;
}

TCommSize BPlusTreeBlocks::getCommSize( TCommID whichComm ) const
{
  return communications[whichComm]->size;
}

TRecordTime BPlusTreeBlocks::getLogicalSend( TCommID whichComm ) const
{
  return communications[whichComm]->logicalSendTime;
}

TRecordTime BPlusTreeBlocks::getLogicalReceive( TCommID whichComm ) const
{
  return communications[whichComm]->logicalReceiveTime;
}

TRecordTime BPlusTreeBlocks::getPhysicalSend( TCommID whichComm ) const
{
  return communications[whichComm]->physicalSendTime;
}

TRecordTime BPlusTreeBlocks::getPhysicalReceive( TCommID whichComm ) const
{
  return communications[whichComm]->physicalReceiveTime;
}

TRecordTime BPlusTreeBlocks::getLastRecordTime() const
{
  return currentBlock[currentRecord].time;
}
