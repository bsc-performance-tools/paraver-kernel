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
#include "noloadexception.h"
#include "paraverkernelexception.h"

using namespace NoLoad;

class TraceBodyIO;
class TraceStream;

NoLoadBlocks::NoLoadBlocks( const ResourceModel& resource, const ProcessModel& process,
                            TraceBodyIO *whichBody, TraceStream *whichFile, TRecordTime endTime )
    : resourceModel( resource ), processModel( process ),
    body( whichBody ), file( whichFile )
{
  fileLoaded = false;

  globalBeginRec.time = 0;
  globalBeginRec.type = EMPTYREC;
  globalEndRec.time = endTime;
  globalEndRec.type = EMPTYREC;

  if ( body->ordered() )
  {
    for ( TThreadOrder i = 0; i < processModel.totalThreads(); ++i )
    {
      traceIndex.push_back( Index<PRV_INT64>( 1000 ) );
      emptyBeginRecords.push_back( globalBeginRec );
      emptyEndRecords.push_back( globalEndRec );
    }
  }

  PRV_INT64 currentOffset = file->tellg();
  file->seekend();
  endFileOffset = file->tellg();
  file->seekg( currentOffset );

#warning Verify if initiaOffset == firstRecordOffset
  initialOffset = currentOffset;
}

NoLoadBlocks::~NoLoadBlocks()
{
  for ( PRV_UINT32 i = 0; i < communications.size(); ++i )
    delete communications[i];

  communications.clear();

  file->close();
}

TData *NoLoadBlocks::getLastRecord( PRV_UINT16 position ) const
{
  return NULL;
}

void NoLoadBlocks::newRecord()
{
  if ( fileLoaded )
  {
    if ( lastData == NULL )
    {
      lastData = new fileLineData();
      lastData->numUseds = 0;
      lastData->endOffset = file->tellg();
      lastData->records.push_back( TRecord() );
      lastRecord = 0;
      blocks[ lastPos ] = lastData;
    }
    else
    {
      lastData->records.push_back( TRecord() );
      ++lastRecord;
    }
  }
}

void NoLoadBlocks::setType( TRecordType whichType )
{
  if ( fileLoaded )
    lastData->records[ lastRecord ].type = whichType;
}

void NoLoadBlocks::setTime( TRecordTime whichTime )
{
  if ( fileLoaded )
    lastData->records[ lastRecord ].time = whichTime;
  else
    loadingRec.time = whichTime;
}

void NoLoadBlocks::setThread( TThreadOrder whichThread )
{
  if ( !fileLoaded )
  {
    if ( body->ordered() )
    {
      loadingThread = whichThread;
      traceIndex[ loadingThread ].indexRecord( loadingRec.time, lastPos );
      if ( loadingThread == beginThread.size() )
        beginThread.push_back( lastPos );
    }
    else
    {
#warning NoLoadBlocks::setThread
    }

    lastPos = file->tellg();
  }
  else
  {
    lastData->thread = whichThread;
  }
}

void NoLoadBlocks::setThread( TApplOrder whichAppl,
                              TTaskOrder whichTask,
                              TThreadOrder whichThread )
{
  if ( !fileLoaded )
  {
    if ( body->ordered() )
    {
      whichThread = processModel.getGlobalThread( whichAppl,
                    whichTask,
                    whichThread );
      loadingThread = whichThread;
      traceIndex[ loadingThread ].indexRecord( loadingRec.time, lastPos );
    }
    else
    {
#warning NoLoadBlocks::setThread
    }

    lastPos = file->tellg();

    if ( body->ordered() )
    {
      if ( loadingThread == beginThread.size() )
        beginThread.push_back( lastPos );
    }
    else
    {
#warning NoLoadBlocks::setThread
    }
  }
  else
  {
    lastData->thread = whichThread;
  }
}

void NoLoadBlocks::setCPU( TCPUOrder whichCPU )
{
  if ( fileLoaded )
    lastData->records[ lastRecord ].CPU = whichCPU;
}

void NoLoadBlocks::setEventType( TEventType whichType )
{
  if ( fileLoaded )
    lastData->records[ lastRecord ].URecordInfo.eventRecord.type = whichType;
}

void NoLoadBlocks::setEventValue( TEventValue whichValue )
{
  if ( fileLoaded )
    lastData->records[ lastRecord ].URecordInfo.eventRecord.value = whichValue;
}

void NoLoadBlocks::setState( TState whichState )
{
  if ( fileLoaded )
    lastData->records[ lastRecord ].URecordInfo.stateRecord.state = whichState;
}

void NoLoadBlocks::setStateEndTime( TRecordTime whichTime )
{
  if ( fileLoaded )
    lastData->records[ lastRecord ].URecordInfo.stateRecord.endTime = whichTime;
}

void NoLoadBlocks::setCommIndex( TCommID whichID )
{
  if ( fileLoaded )
    lastData->records[ lastRecord ].URecordInfo.commRecord.index = whichID;
}


void NoLoadBlocks::newComm( bool createRecords )
{
  if ( createRecords )
    throw ParaverKernelException();

  lastPos = file->tellg();

  communications.push_back( new TCommInfo() );
  currentComm = communications.size() - 1;
}

void NoLoadBlocks::setSenderThread( TThreadOrder whichThread )
{
  communications[currentComm]->senderThread = whichThread;
}

void NoLoadBlocks::setSenderThread( TApplOrder whichAppl,
                                    TTaskOrder whichTask,
                                    TThreadOrder whichThread )
{
  TThreadOrder globalThread = processModel.getGlobalThread( whichAppl,
                              whichTask,
                              whichThread );
  communications[currentComm]->senderThread = globalThread;
}

void NoLoadBlocks::setSenderCPU( TCPUOrder whichCPU )
{
  communications[currentComm]->senderCPU = whichCPU;
}

void NoLoadBlocks::setReceiverThread( TThreadOrder whichThread )
{
  communications[currentComm]->receiverThread = whichThread;
}

void NoLoadBlocks::setReceiverThread( TApplOrder whichAppl,
                                      TTaskOrder whichTask,
                                      TThreadOrder whichThread )
{
  TThreadOrder globalThread = processModel.getGlobalThread( whichAppl,
                              whichTask,
                              whichThread );
  communications[currentComm]->receiverThread = globalThread;
}

void NoLoadBlocks::setReceiverCPU( TCPUOrder whichCPU )
{
  communications[currentComm]->receiverCPU = whichCPU;
}

void NoLoadBlocks::setCommTag( TCommTag whichTag )
{
  communications[currentComm]->tag = whichTag;
}

void NoLoadBlocks::setCommSize( TCommSize whichSize )
{
  communications[currentComm]->size = whichSize;
}

void NoLoadBlocks::setLogicalSend( TRecordTime whichTime )
{
  communications[currentComm]->logicalSendTime = whichTime;
}

void NoLoadBlocks::setLogicalReceive( TRecordTime whichTime )
{
  communications[currentComm]->logicalReceiveTime = whichTime;
}

void NoLoadBlocks::setPhysicalSend( TRecordTime whichTime )
{
  communications[currentComm]->physicalSendTime = whichTime;
}

void NoLoadBlocks::setPhysicalReceive( TRecordTime whichTime )
{
  communications[currentComm]->physicalReceiveTime = whichTime;
}

TCommID NoLoadBlocks::getTotalComms() const
{
  return communications.size();
}

TThreadOrder NoLoadBlocks::getSenderThread( TCommID whichComm ) const
{
  return communications[whichComm]->senderThread;
}

TCPUOrder NoLoadBlocks::getSenderCPU( TCommID whichComm ) const
{
  return communications[whichComm]->senderCPU;
}

TThreadOrder NoLoadBlocks::getReceiverThread( TCommID whichComm ) const
{
  return communications[whichComm]->receiverThread;
}

TCPUOrder NoLoadBlocks::getReceiverCPU( TCommID whichComm ) const
{
  return communications[whichComm]->receiverCPU;
}

TCommTag NoLoadBlocks::getCommTag( TCommID whichComm ) const
{
  return communications[whichComm]->tag;
}

TCommSize NoLoadBlocks::getCommSize( TCommID whichComm ) const
{
  return communications[whichComm]->size;
}

TRecordTime NoLoadBlocks::getLogicalSend( TCommID whichComm ) const
{
  return communications[whichComm]->logicalSendTime;
}

TRecordTime NoLoadBlocks::getLogicalReceive( TCommID whichComm ) const
{
  return communications[whichComm]->logicalReceiveTime;
}

TRecordTime NoLoadBlocks::getPhysicalSend( TCommID whichComm ) const
{
  return communications[whichComm]->physicalSendTime;
}

TRecordTime NoLoadBlocks::getPhysicalReceive( TCommID whichComm ) const
{
  return communications[whichComm]->physicalReceiveTime;
}

TRecordTime NoLoadBlocks::getLastRecordTime() const
{
  return 0;
}

void NoLoadBlocks::getBeginRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  *record = &globalBeginRec;
  offset = -1;
  recPos = 0;
}


void NoLoadBlocks::getEndRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  *record = &globalEndRec;
  offset = endFileOffset;
  recPos = 0;
}

void NoLoadBlocks::getBeginThreadRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  if ( !body->ordered() )
    throw NoLoad::NoLoadException( NoLoad::NoLoadException::wrongTraceBodyVersion, "" , __FILE__, __LINE__ );

  *record = &emptyBeginRecords[ whichThread ];
  offset = -1;
  recPos = 0;
}

void NoLoadBlocks::getEndThreadRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  if ( !body->ordered() )
    throw NoLoad::NoLoadException( NoLoad::NoLoadException::wrongTraceBodyVersion, "" , __FILE__, __LINE__ );

  *record = &emptyEndRecords[ whichThread ];
  if ( whichThread == processModel.totalThreads() - 1 )
    offset = endFileOffset;
  else
    offset = beginThread[ whichThread + 1 ];
  recPos = 0;
}


// Must be used with TraceBodyIO_v1
void NoLoadBlocks::getNextRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  if( *record == &globalEndRec )
  {
    *record = NULL;
    return;
  }
  else if ( offset != -1 )
  {
    fileLineData *tmpData = blocks[ offset ];
    if ( recPos < tmpData->records.size() - 1 )
    {
      ++recPos;
      *record = &tmpData->records[ recPos ];
      return;
    }
    else if ( tmpData->endOffset == endFileOffset )
    {
      decNumUseds( offset );
      offset = endFileOffset;
      *record = NULL;
      recPos = 0;
      return;
    }

    decNumUseds( offset );
    offset = tmpData->endOffset;
  }
  else
    offset = initialOffset;

  if ( blocks.count( offset ) == 0 )
  {
    file->clear();
    file->seekg( offset );
    lastData = NULL;
    lastPos = offset;
    body->read( file, *this, notUsedEvents, dummyTraceInfo );
  }

  fileLineData *currentData = blocks[ offset ];
  *record = &currentData->records[ 0 ];
  recPos = 0;
  ++currentData->numUseds;
}


// Must be used with TraceBodyIO_v1
void NoLoadBlocks::getPrevRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  if ( offset == -1 )
  {
    *record = NULL;
    return;
  }
  else if( *record != &globalBeginRec )
  {
    if ( recPos > 0 )
    {
      fileLineData *tmpData = blocks[ offset ];
      --recPos;
      *record = &tmpData->records[ recPos ];
      return;
    }
    else if ( offset == initialOffset )
    {
      decNumUseds( offset );
      offset = -1;
      *record = NULL;
      return;
    }
  }
  file->clear();
  file->seekg( offset );
  goToPrevLine();

  if( *record != &globalBeginRec )
    decNumUseds( offset );

  offset = file->tellg();
  if ( blocks.count( offset ) == 0 )
  {
    lastData = NULL;
    lastPos = offset;
    body->read( file, *this, notUsedEvents, dummyTraceInfo );
  }

  fileLineData *currentData = blocks[ offset ];
  *record = &currentData->records[ 0 ];
  recPos = 0;
  ++currentData->numUseds;
}


// Must be used with TraceBodyIO_v2
void NoLoadBlocks::getNextRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  if( *record == &emptyEndRecords[ whichThread ] )
  {
    *record = NULL;
    return;
  }
  else if ( offset != -1 )
  {
    fileLineData *tmpData = blocks[ offset ];
    if ( recPos < tmpData->records.size() - 1 )
    {
      ++recPos;
      *record = &tmpData->records[ recPos ];
      return;
    }
    else if ( ( whichThread < processModel.totalThreads() - 1 && tmpData->endOffset == beginThread[ whichThread + 1 ] )
              ||
              ( whichThread == processModel.totalThreads() - 1 && tmpData->endOffset == endFileOffset ) )
    {
      decNumUseds( offset );
      offset = endFileOffset;
      *record = NULL;
      recPos = 0;
      return;
    }

    decNumUseds( offset );
    offset = tmpData->endOffset;
  }
  else
    offset = beginThread[ whichThread ];

  if ( blocks.count( offset ) == 0 )
  {
    file->clear();
    file->seekg( offset );
    lastData = NULL;
    lastPos = offset;
    body->read( file, *this, notUsedEvents, dummyTraceInfo );
  }

  fileLineData *currentData = blocks[ offset ];
  *record = &currentData->records[ 0 ];
  recPos = 0;
  ++currentData->numUseds;
}

// Must be used with TraceBodyIO_v2
void NoLoadBlocks::getPrevRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  if ( offset == -1 )
  {
    *record = NULL;
    return;
  }
  else if( *record != &emptyEndRecords[ whichThread ] ) // TODO  maybe begin?
  {
    if ( recPos > 0 )
    {
      fileLineData *tmpData = blocks[ offset ];
      --recPos;
      *record = &tmpData->records[ recPos ];
      return;
    }
    else if ( offset == beginThread[ whichThread ] )
    {
      decNumUseds( offset );
      offset = -1;
      *record = NULL;
      return;
    }
  }
  file->clear();
  file->seekg( offset );
  goToPrevLine();

  if( *record != &emptyEndRecords[ whichThread ] )  // TODO  maybe begin?
    decNumUseds( offset );

  offset = file->tellg();
  if ( blocks.count( offset ) == 0 )
  {
    lastData = NULL;
    lastPos = offset;
    body->read( file, *this, notUsedEvents, dummyTraceInfo );
  }

  fileLineData *currentData = blocks[ offset ];
  *record = &currentData->records[ 0 ];
  recPos = 0;
  ++currentData->numUseds;
}


void NoLoadBlocks::getThreadRecordByTime( TThreadOrder whichThread, TRecordTime whichTime,
    TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos )
{
  if ( !body->ordered() )
    throw NoLoad::NoLoadException( NoLoad::NoLoadException::wrongTraceBodyVersion, "" , __FILE__, __LINE__ );

  if ( !traceIndex[ whichThread ].findRecord( whichTime, offset ) )
  {
    offset = -1;
    record = NULL;
    return;
  }

  if ( blocks.count( offset ) == 0 )
  {
    file->clear();
    file->seekg( offset );
    lastData = NULL;
    lastPos = offset;
    body->read( file, *this, notUsedEvents, dummyTraceInfo );
  }

  fileLineData *currentData = blocks[ offset ];
  *record = &currentData->records[ 0 ];
  recPos = 0;
  ++currentData->numUseds;
}

void NoLoadBlocks::incNumUseds( PRV_INT64 offset )
{
  if( blocks.count( offset ) == 0 )
    return;
  fileLineData *tmpData = blocks[ offset ];
  if( tmpData != NULL )
    ++tmpData->numUseds;
}

void NoLoadBlocks::decNumUseds( PRV_INT64 offset )
{
  if( blocks.count( offset ) == 0 )
    return;
  fileLineData *tmpData = blocks[ offset ];
  --tmpData->numUseds;
  if ( tmpData->numUseds <= 0 )
  {
    delete tmpData;
    blocks.erase( offset );
  }
}

void NoLoadBlocks::goToPrevLine()
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

void NoLoadBlocks::setFileLoaded()
{
  fileLoaded = true;
}

void NoLoadBlocks::setFirstOffset( PRV_INT64 whichOffset )
{
  lastPos = whichOffset;
}
