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

#include "traceeditblocks.h"
#include "paraverkernelexception.h"

using namespace NoLoad;

class TraceBodyIO;
class TraceStream;

TraceEditBlocks::TraceEditBlocks( const ResourceModel& resource, const ProcessModel& process,
                            TraceBodyIO *whichBody, TraceStream *whichFile, TRecordTime endTime )
    : NoLoadBlocks( resource, process ),
      resourceModel( resource ), processModel( process ),
      body( whichBody ), file( whichFile )
{
  fileLoaded = false;

  globalBeginRec.time = 0;
  globalBeginRec.type = EMPTYREC;
  globalEndRec.time = endTime;
  globalEndRec.type = EMPTYREC;

  PRV_INT64 currentOffset = file->tellg();
  file->seekend();
  endFileOffset = file->tellg();
  file->seekg( currentOffset );

  initialOffset = currentOffset;
}

TraceEditBlocks::~TraceEditBlocks()
{
  file->close();
}

TData *TraceEditBlocks::getLastRecord( PRV_UINT16 position ) const
{
  return NULL;
}

void TraceEditBlocks::newRecord()
{
  lastData.endOffset = file->tellg();
  lastData.records.push_back( TRecord() );
  ++lastRecord;
}

void TraceEditBlocks::setType( TRecordType whichType )
{
  lastData.records[ lastRecord ].type = whichType;
}

void TraceEditBlocks::setTime( TRecordTime whichTime )
{
  lastData.records[ lastRecord ].time = whichTime;
}

void TraceEditBlocks::setThread( TThreadOrder whichThread )
{
  lastData.thread = whichThread;
  lastData.records[ lastRecord ].thread = whichThread;
}

void TraceEditBlocks::setThread( TApplOrder whichAppl,
                              TTaskOrder whichTask,
                              TThreadOrder whichThread )
{
  whichThread = processModel.getGlobalThread( whichAppl,
                                              whichTask,
                                              whichThread );
  lastData.thread = whichThread;
  lastData.records[ lastRecord ].thread = whichThread;
}

void TraceEditBlocks::setCPU( TCPUOrder whichCPU )
{
  lastData.records[ lastRecord ].CPU = whichCPU;
}

void TraceEditBlocks::setEventType( TEventType whichType )
{
  lastData.records[ lastRecord ].URecordInfo.eventRecord.type = whichType;
}

void TraceEditBlocks::setEventValue( TEventValue whichValue )
{
  lastData.records[ lastRecord ].URecordInfo.eventRecord.value = whichValue;
}

void TraceEditBlocks::setState( TState whichState )
{
  lastData.records[ lastRecord ].URecordInfo.stateRecord.state = whichState;
}

void TraceEditBlocks::setStateEndTime( TRecordTime whichTime )
{
  lastData.records[ lastRecord ].URecordInfo.stateRecord.endTime = whichTime;
}

void TraceEditBlocks::setCommIndex( TCommID whichID )
{
  lastData.records[ lastRecord ].URecordInfo.commRecord.index = whichID;
}


void TraceEditBlocks::newComm( bool createRecords )
{
  if ( createRecords )
  {
    newRecord();
    setType( COMM + LOG + SEND );

    newRecord();
    setType( COMM + LOG + RECV );

    newRecord();
    setType( COMM + PHY + SEND );

    newRecord();
    setType( COMM + PHY + RECV );

    logSend = &lastData.records[ lastRecord - 3 ];
    logRecv = &lastData.records[ lastRecord - 2 ];
    phySend = &lastData.records[ lastRecord - 1 ];
    phyRecv = &lastData.records[ lastRecord ];
  }

  lastPos = file->tellg();

  communications.push_back( TCommInfo() );
  currentComm = communications.size() - 1;
  logSend->URecordInfo.commRecord.index = currentComm;
  logRecv->URecordInfo.commRecord.index = currentComm;
  phySend->URecordInfo.commRecord.index = currentComm;
  phyRecv->URecordInfo.commRecord.index = currentComm;
}

void TraceEditBlocks::setSenderThread( TThreadOrder whichThread )
{
  communications[currentComm].senderThread = whichThread;
  logSend->thread = whichThread;
  phySend->thread = whichThread;
}

void TraceEditBlocks::setSenderThread( TApplOrder whichAppl,
                                       TTaskOrder whichTask,
                                       TThreadOrder whichThread )
{
  TThreadOrder globalThread = processModel.getGlobalThread( whichAppl,
                                                            whichTask,
                                                            whichThread );
  communications[currentComm].senderThread = globalThread;
  logSend->thread = globalThread;
  phySend->thread = globalThread;
}

void TraceEditBlocks::setSenderCPU( TCPUOrder whichCPU )
{
  communications[currentComm].senderCPU = whichCPU;
  logSend->CPU = whichCPU;
  phySend->CPU = whichCPU;
}

void TraceEditBlocks::setReceiverThread( TThreadOrder whichThread )
{
  communications[currentComm].receiverThread = whichThread;
  logRecv->thread = whichThread;
  phyRecv->thread = whichThread;
}

void TraceEditBlocks::setReceiverThread( TApplOrder whichAppl,
                                      TTaskOrder whichTask,
                                      TThreadOrder whichThread )
{
  TThreadOrder globalThread = processModel.getGlobalThread( whichAppl,
                              whichTask,
                              whichThread );
  communications[currentComm].receiverThread = globalThread;
  logRecv->thread = globalThread;
  phyRecv->thread = globalThread;
}

void TraceEditBlocks::setReceiverCPU( TCPUOrder whichCPU )
{
  communications[currentComm].receiverCPU = whichCPU;
  logRecv->CPU = whichCPU;
  phyRecv->CPU = whichCPU;
}

void TraceEditBlocks::setCommTag( TCommTag whichTag )
{
  communications[currentComm].tag = whichTag;
}

void TraceEditBlocks::setCommSize( TCommSize whichSize )
{
  communications[currentComm].size = whichSize;
}

void TraceEditBlocks::setLogicalSend( TRecordTime whichTime )
{
  communications[currentComm].logicalSendTime = whichTime;
  logSend->time = whichTime;
}

void TraceEditBlocks::setLogicalReceive( TRecordTime whichTime )
{
  communications[currentComm].logicalReceiveTime = whichTime;
  logRecv->time = whichTime;
}

void TraceEditBlocks::setPhysicalSend( TRecordTime whichTime )
{
  communications[currentComm].physicalSendTime = whichTime;
  phySend->time = whichTime;
}

void TraceEditBlocks::setPhysicalReceive( TRecordTime whichTime )
{
  communications[currentComm].physicalReceiveTime = whichTime;
  phyRecv->time = whichTime;
}

void TraceEditBlocks::setLogicalSend( TCommID whichComm, TRecordTime whichTime )
{
  communications[whichComm].logicalSendTime = whichTime;
}

void TraceEditBlocks::setLogicalReceive( TCommID whichComm, TRecordTime whichTime )
{
  communications[whichComm].logicalReceiveTime = whichTime;
}

void TraceEditBlocks::setPhysicalSend( TCommID whichComm, TRecordTime whichTime )
{
  communications[whichComm].physicalSendTime = whichTime;
}

void TraceEditBlocks::setPhysicalReceive( TCommID whichComm, TRecordTime whichTime )
{
  communications[whichComm].physicalReceiveTime = whichTime;
}

TCommID TraceEditBlocks::getTotalComms() const
{
  return communications.size();
}

TThreadOrder TraceEditBlocks::getSenderThread( TCommID whichComm ) const
{
  return communications[whichComm].senderThread;
}

TCPUOrder TraceEditBlocks::getSenderCPU( TCommID whichComm ) const
{
  return communications[whichComm].senderCPU;
}

TThreadOrder TraceEditBlocks::getReceiverThread( TCommID whichComm ) const
{
  return communications[whichComm].receiverThread;
}

TCPUOrder TraceEditBlocks::getReceiverCPU( TCommID whichComm ) const
{
  return communications[whichComm].receiverCPU;
}

TCommTag TraceEditBlocks::getCommTag( TCommID whichComm ) const
{
  return communications[whichComm].tag;
}

TCommSize TraceEditBlocks::getCommSize( TCommID whichComm ) const
{
  return communications[whichComm].size;
}

TRecordTime TraceEditBlocks::getLogicalSend( TCommID whichComm ) const
{
  return communications[whichComm].logicalSendTime;
}

TRecordTime TraceEditBlocks::getLogicalReceive( TCommID whichComm ) const
{
  return communications[whichComm].logicalReceiveTime;
}

TRecordTime TraceEditBlocks::getPhysicalSend( TCommID whichComm ) const
{
  return communications[whichComm].physicalSendTime;
}

TRecordTime TraceEditBlocks::getPhysicalReceive( TCommID whichComm ) const
{
  return communications[whichComm].physicalReceiveTime;
}

TRecordTime TraceEditBlocks::getLastRecordTime() const
{
  return 0;
}

void TraceEditBlocks::getBeginRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  *record = &globalBeginRec;
  offset = -1;
  recPos = 0;
}


void TraceEditBlocks::getEndRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  *record = &globalEndRec;
  offset = endFileOffset;
  recPos = 0;
}

void TraceEditBlocks::getBeginThreadRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
}

void TraceEditBlocks::getEndThreadRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
}


// Must be used with TraceBodyIO_v1
void TraceEditBlocks::getNextRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  if( *record == &globalEndRec )
  {
    *record = NULL;
    return;
  }
  else if ( offset != -1 )
  {
    if ( recPos < lastData.records.size() - 1 )
    {
      ++recPos;
      *record = &lastData.records[ recPos ];
      return;
    }
    else if ( lastData.endOffset == endFileOffset )
    {
      offset = endFileOffset;
      *record = NULL;
      recPos = 0;
      return;
    }

    offset = lastData.endOffset;
  }
  else
    offset = initialOffset;

  if( file->tellg() == (std::streampos)endFileOffset )
  {
    offset = endFileOffset;
    *record = NULL;
    recPos = 0;
    return;
  }

  lastData.records.clear();
  lastRecord = -1;
  communications.clear();
  PRV_UINT16 numLines = 0;
//  while( lastData.records.size() == 0 )
  while( numLines < 128 )
  {
    PRV_UINT16 previousSize = lastData.records.size();

    body->read( file, *this, notUsedStates, notUsedEvents, dummyTraceInfo );

    if ( previousSize < lastData.records.size() )
      ++numLines;

    if( lastData.records.size() == 0 && file->tellg() == (std::streampos)endFileOffset )
    {
      offset = endFileOffset;
      *record = NULL;
      recPos = 0;
      return;
    }
    else if ( lastData.records.size() > 0 && file->tellg() == (std::streampos)endFileOffset )
    {
      offset = endFileOffset;
      break;
    }
  }

  *record = &lastData.records[ 0 ];
  recPos = 0;
}


// Must be used with TraceBodyIO_v1
void TraceEditBlocks::getPrevRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
}


// Must be used with TraceBodyIO_v2
void TraceEditBlocks::getNextRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
}

// Must be used with TraceBodyIO_v2
void TraceEditBlocks::getPrevRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
}


void TraceEditBlocks::getThreadRecordByTime( TThreadOrder whichThread, TRecordTime whichTime,
    TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
}

void TraceEditBlocks::incNumUseds( PRV_INT64 offset )
{
}

void TraceEditBlocks::decNumUseds( PRV_INT64 offset )
{
}

void TraceEditBlocks::goToPrevLine()
{
  file->clear();
  PRV_INT64 curOffset = file->tellg();
  curOffset -= 2;

  file->seekg( curOffset );
  while ( (char)file->peek() != '\n' )
  {
    --curOffset;
    file->seekg( curOffset );
  }

  ++curOffset;
  file->seekg( curOffset );
}

void TraceEditBlocks::setFileLoaded()
{
  fileLoaded = true;
}

void TraceEditBlocks::setFirstOffset( PRV_INT64 whichOffset )
{
  lastPos = whichOffset;
}
