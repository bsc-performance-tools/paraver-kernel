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

#include <iostream>
#include <sstream>
#include <fstream>
#include <type_traits>

#include "prvgetline.h"

#define PARAM_TYPENAME class    TraceStreamT, \
                       class    RecordContainerT, \
                       class    ProcessModelT, \
                       class    ResourceModelT, \
                       typename StateT, \
                       typename EventTypeT, \
                       class    MetadataManagerT, \
                       typename RecordTimeT, \
                       class    RecordT

#define PARAM_LIST TraceStreamT, \
                   RecordContainerT, \
                   ProcessModelT, \
                   ResourceModelT, \
                   StateT, \
                   EventTypeT, \
                   MetadataManagerT, \
                   RecordTimeT, \
                   RecordT

template< PARAM_TYPENAME >
std::string TraceBodyIO_v1< PARAM_LIST >::multiEventLine;

template< PARAM_TYPENAME >
typename TraceBodyIO_v1< PARAM_LIST >::TMultiEventCommonInfo TraceBodyIO_v1< PARAM_LIST >::multiEventCommonInfo =
        { nullptr, (TThreadOrder)0, (TCPUOrder)0, (RecordTimeT)0 };

template< PARAM_TYPENAME >
std::string TraceBodyIO_v1< PARAM_LIST >::line;

template< PARAM_TYPENAME >
std::ostringstream TraceBodyIO_v1< PARAM_LIST >::ostr;

// Optimization on conversion string to numbers, but with no error control
//#define USE_ATOLL
// Even more optimization using custom function instead of atoll with error checking
#define USE_PRV_ATOLL

template< PARAM_TYPENAME >
bool TraceBodyIO_v1< PARAM_LIST >::ordered() const
{
  return false;
}


template< PARAM_TYPENAME >
void TraceBodyIO_v1< PARAM_LIST >::read( TraceStreamT& file,
                                         RecordContainerT& records,
                                         const ProcessModelT& whichProcessModel,
                                         const ResourceModelT& whichResourceModel,
                                         std::unordered_set<StateT>& states,
                                         std::unordered_set<EventTypeT>& events,
                                         MetadataManagerT& traceInfo,
                                         RecordTimeT& endTime  ) const
{
  prvGetLine( file, line );

  if ( line.size() == 0 )
    return;

  switch ( line[0] )
  {
    case CommentRecord:
      readTraceInfo( line, traceInfo );
      break;

    case StateRecord:
      readState( line, whichProcessModel, whichResourceModel, records, states );
      break;

    case EventRecord:
      readEvent( line, whichProcessModel, whichResourceModel, records, events );
      break;

    case CommRecord:
      readComm( line, whichProcessModel, whichResourceModel, records );
      break;

    case GlobalCommRecord:
      //readGlobalComm( line, records );
      break;

    default:
      std::cerr << "Unkwnown record type." << std::endl;
      std::cerr << line << std::endl;
      break;
  };
}

template< PARAM_TYPENAME >
void TraceBodyIO_v1< PARAM_LIST >::bufferWrite( std::fstream& whichStream, bool writeReady, bool lineClear ) const
{
  if ( writeReady )
  {
    whichStream << line << '\n';
    if ( lineClear )
      line.clear();
  }
}

template< PARAM_TYPENAME >
void TraceBodyIO_v1< PARAM_LIST >::write( std::fstream& whichStream,
                            const ProcessModelT& whichProcessModel,
                            const ResourceModelT& whichResourceModel,
                            RecordT *record ) const
{
  bool writeReady = false;
  TRecordType type = record->getType();
  line.clear();

  if ( type == EMPTYREC )
  {
    writeReady = writePendingMultiEvent( whichProcessModel );
    bufferWrite( whichStream, writeReady );
  }
  else
  {
    if ( type & STATE )
    {
      writeReady = writePendingMultiEvent( whichProcessModel );
      bufferWrite( whichStream, writeReady );
      writeReady = writeState( whichProcessModel, whichResourceModel, record );
    }
    else if ( type & EVENT )
    {
      if ( !sameMultiEvent( record ) )
      {
        writeReady = writePendingMultiEvent( whichProcessModel );

        multiEventCommonInfo.myStream = &whichStream;
        multiEventCommonInfo.cpu = record->getCPU();
        multiEventCommonInfo.thread = record->getThread();
        multiEventCommonInfo.time = record->getTime();

        multiEventLine.clear();
      }

      appendEvent( record );
    }
    else if ( type & COMM )
    {
      writeReady = writePendingMultiEvent( whichProcessModel );
      bufferWrite( whichStream, writeReady );

      writeReady = writeComm( whichProcessModel, whichResourceModel, record );
    }
    else if ( type & GLOBCOMM )
    {
      writeReady = writePendingMultiEvent( whichProcessModel );
      bufferWrite( whichStream, writeReady );

      writeReady = writeGlobalComm( whichProcessModel, record );
    }
    else if ( type & RSEND || type & RRECV )
      writeReady = false;
    else
    {
      writeReady = false;
      std::cerr << "TraceBodyIO_v1::write()" << std::endl;
      std::cerr << "Unkwnown record type in memory." << std::endl;
    }

    bufferWrite( whichStream, writeReady, false );
  }
}


/**********************
  Read line functions
***********************/
template< PARAM_TYPENAME >
inline void TraceBodyIO_v1< PARAM_LIST >::readTraceInfo( const std::string& line, MetadataManagerT& traceInfo ) const
{
  traceInfo.NewMetadata( line );
  // dummy set also to false if it is a comment
}

template< PARAM_TYPENAME >
inline void TraceBodyIO_v1< PARAM_LIST >::readState( const std::string& line,
                                       const ProcessModelT& whichProcessModel,
                                       const ResourceModelT& whichResourceModel,
                                       RecordContainerT& records,
                                       std::unordered_set<StateT>& states ) const
{
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  RecordTimeT time;
  RecordTimeT endtime;
  StateT state;

  auto it = line.begin() + 2;

  // Read the common info
  if ( !readCommon( whichProcessModel, whichResourceModel, it, line.cend(), CPU, appl, task, thread, time ) )
  {
    std::cerr << "Error reading state record." << std::endl;
    std::cerr << line << std::endl;
    return;
  }

  if( !prv_atoll_v( it, line.cend(), endtime, state ) )
  {
    std::cerr << "Error reading state record." << std::endl;
    std::cerr << line << std::endl;
    return;
  }

  thread = whichProcessModel.getGlobalThread( appl - 1, task - 1, thread - 1 );
  records.newRecord( thread );
  records.setType( STATE + BEGIN );
  records.setTime( time );
  records.setCPU( CPU );
  records.setThread( thread );
  records.setState( state );
  records.setStateEndTime( endtime );

  if ( endtime != -1 )
  {
    records.newRecord( thread );
    records.setType( STATE + END );
    records.setTime( endtime );
    records.setCPU( CPU );
    records.setThread( thread );
    records.setState( state );
    records.setStateEndTime( time );
  }

  states.insert( state );
}

template< PARAM_TYPENAME >
inline void TraceBodyIO_v1< PARAM_LIST >::readEvent( const std::string& line,
                                       const ProcessModelT& whichProcessModel,
                                       const ResourceModelT& whichResourceModel,
                                       RecordContainerT& records,
                                       std::unordered_set<EventTypeT>& events ) const
{
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  RecordTimeT time;
  EventTypeT eventtype;
  TEventValue eventvalue;

  auto it = line.begin() + 2;

  // Read the common info
  if ( !readCommon( whichProcessModel, whichResourceModel, it, line.cend(), CPU, appl, task, thread, time ) )
  {
    std::cerr << "Error reading event record." << std::endl;
    std::cerr << line << std::endl;
    return;
  }

  thread = whichProcessModel.getGlobalThread( appl - 1, task - 1, thread - 1 );
  while ( it != line.end() )
  {
    if( !prv_atoll_v( it, line.cend(), eventtype, eventvalue ) )
    {
      std::cerr << "Error reading event record." << std::endl;
      std::cerr << line << std::endl;
      return;
    }

    records.newRecord( thread );
    records.setType( EVENT );
    records.setTime( time );
    records.setCPU( CPU );
    records.setThread( thread );
    records.setEventType( eventtype );
    records.setEventValue( eventvalue );

    events.insert( eventtype );
  }
}

template< PARAM_TYPENAME >
inline void TraceBodyIO_v1< PARAM_LIST >::readComm( const std::string& line,
                                      const ProcessModelT& whichProcessModel,
                                      const ResourceModelT& whichResourceModel,
                                      RecordContainerT& records ) const
{
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  RecordTimeT logSend;
  RecordTimeT phySend;
  RecordTimeT logReceive;
  RecordTimeT phyReceive;
  TCPUOrder remoteCPU;
  TApplOrder remoteAppl;
  TTaskOrder remoteTask;
  TThreadOrder remoteThread;
  TCommSize commSize;
  TCommTag commTag;

  auto it = line.begin() + 2;

  // Read the common info
  if ( !readCommon( whichProcessModel, whichResourceModel, it, line.cend(), CPU, appl, task, thread, logSend ) )
  {
    std::cerr << "Error reading communication record." << std::endl;
    std::cerr << line << std::endl;
    return;
  }

  if( !prv_atoll_v( it, line.cend(), phySend, remoteCPU, remoteAppl, remoteTask, remoteThread, logReceive, phyReceive, commSize, commTag ) )
  {
    std::cerr << "Error reading communication record." << std::endl;
    std::cerr << line << std::endl;
    return;
  }

  if( !whichProcessModel.isValidThread( remoteAppl - 1, remoteTask - 1, remoteThread - 1 ) || 
      !whichResourceModel.isValidGlobalCPU( remoteCPU ) )
  {
    std::cerr << "Error reading communication record." << std::endl;
    std::cerr << line << std::endl;
    return;
  }

  thread = whichProcessModel.getGlobalThread( appl - 1, task - 1, thread - 1 );
  remoteThread = whichProcessModel.getGlobalThread( remoteAppl - 1, remoteTask - 1, remoteThread - 1 );
  records.newComm( thread, remoteThread );
  records.setSenderCPU( CPU );
  records.setSenderThread( thread );
  records.setReceiverCPU( remoteCPU );
  records.setReceiverThread( remoteThread );
  records.setLogicalSend( logSend );
  records.setPhysicalSend( phySend );
  records.setLogicalReceive( logReceive );
  records.setPhysicalReceive( phyReceive );
  records.setCommSize( commSize );
  records.setCommTag( commTag );
}


template< PARAM_TYPENAME >
inline void TraceBodyIO_v1< PARAM_LIST >::readGlobalComm( const std::string& line, RecordContainerT& records ) const
{}


template< PARAM_TYPENAME >
inline bool TraceBodyIO_v1< PARAM_LIST >::readCommon( const ProcessModelT& whichProcessModel,
                                        const ResourceModelT& whichResourceModel,
                                        std::string::const_iterator& it,
                                        const std::string::const_iterator& end,
                                        TCPUOrder& CPU,
                                        TApplOrder& appl,
                                        TTaskOrder& task,
                                        TThreadOrder& thread,
                                        RecordTimeT& time ) const
{
  return prv_atoll_v( it, end, CPU, appl, task, thread, time ) &&
         whichResourceModel.isValidGlobalCPU( CPU ) &&
         whichProcessModel.isValidThread( appl - 1, task - 1, thread - 1 );
}


/**************************
  Write records functions
***************************/
template< PARAM_TYPENAME >
bool TraceBodyIO_v1< PARAM_LIST >::writeState( const ProcessModelT& whichProcessModel,
                                 const ResourceModelT& whichResourceModel,
                                 const RecordT *record ) const
{
  if ( record->getType() & END )
    return false;

  ostr.clear();
  ostr.str( "" );
  ostr << std::fixed;
  ostr << std::dec;
  ostr.precision( 0 );

  ostr << StateRecord << ':';
  writeCommon( ostr, whichProcessModel, whichResourceModel, record );
  ostr << record->getStateEndTime() << ':' << record->getState();

  line += ostr.str();
  return true;
}


template< PARAM_TYPENAME >
bool TraceBodyIO_v1< PARAM_LIST >::writePendingMultiEvent( const ProcessModelT& whichProcessModel ) const
{
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;

  bool writeLine = !multiEventLine.empty();

  if ( writeLine )
  {
    ostr.clear();
    ostr.str( "" );
    ostr << std::fixed;
    ostr << std::dec;
    ostr.precision( 0 );

    ostr << EventRecord << ':';
    ostr << multiEventCommonInfo.cpu << ':';

    whichProcessModel.getThreadLocation( multiEventCommonInfo.thread, appl, task, thread );
    ostr << appl + 1 << ':' << task + 1 << ':' << thread + 1 << ':';

    ostr << multiEventCommonInfo.time << ':';
    ostr << multiEventLine;

    line += ostr.str();

    bufferWrite( *multiEventCommonInfo.myStream, true );

    multiEventCommonInfo.myStream = nullptr;
    multiEventCommonInfo.cpu = 0;
    multiEventCommonInfo.thread = 0;
    multiEventCommonInfo.time = 0;

    multiEventLine.clear();
  }

  return false;
}


template< PARAM_TYPENAME >
void TraceBodyIO_v1< PARAM_LIST >::appendEvent( const RecordT *record ) const
{
  ostr.clear();
  ostr.str( "" );
  ostr << std::fixed;
  ostr << std::dec;
  ostr.precision( 0 );

  if ( !multiEventLine.empty() )
    ostr << ':';

  ostr << record->getEventType() << ':' << record->getEventValueAsIs();

  multiEventLine += ostr.str();
}


template< PARAM_TYPENAME >
bool TraceBodyIO_v1< PARAM_LIST >::writeComm( const ProcessModelT& whichProcessModel,
                                const ResourceModelT& whichResourceModel,
                                const RecordT *record ) const
{
  TApplOrder recvAppl;
  TTaskOrder recvTask;
  TThreadOrder recvThread;

  ostr.clear();
  ostr.str( "" );
  ostr << std::fixed;
  ostr << std::dec;
  ostr.precision( 0 );

  if ( !( record->getType() == ( COMM + LOG + SEND ) ) )
    return false;

  ostr << CommRecord << ':';
  writeCommon( ostr, whichProcessModel, whichResourceModel, record );
  ostr << record->getPhysicalSend() << ':';
  if ( whichResourceModel.isReady() )
    ostr << record->getReceiverCPU() << ':';
  else
    ostr << '0' << ':';
  whichProcessModel.getThreadLocation( record->getReceiverThread(),
                                recvAppl, recvTask, recvThread );
  ostr << recvAppl + 1 << ':' << recvTask + 1 << ':' << recvThread + 1 << ':';
  ostr << record->getLogicalReceive() << ':';
  ostr << record->getPhysicalReceive() << ':';

  ostr << record->getCommSize() << ':';
  ostr << record->getCommTag();

  line += ostr.str();
  return true;
}

template< PARAM_TYPENAME >
bool TraceBodyIO_v1< PARAM_LIST >::writeGlobalComm( const ProcessModelT& whichProcessModel,
                                      const RecordT *record ) const
{
  return true;
}

template< PARAM_TYPENAME >
void TraceBodyIO_v1< PARAM_LIST >::writeCommon( std::ostringstream& line,
                                  const ProcessModelT& whichProcessModel,
                                  const ResourceModelT& whichResourceModel,
                                  const RecordT *record ) const
{
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;

  if ( whichResourceModel.isReady() )
    line << record->getCPU() << ':';
  else
    line << '0' << ':';

  whichProcessModel.getThreadLocation( record->getThread(), appl, task, thread );
  line << appl + 1 << ':' << task + 1 << ':' << thread + 1 << ':';
  line << record->getTime() << ':';
}


template< PARAM_TYPENAME >
bool TraceBodyIO_v1< PARAM_LIST >::sameMultiEvent( const RecordT *record ) const
{
  return ( multiEventCommonInfo.myStream != nullptr &&
           multiEventCommonInfo.cpu == record->getCPU() &&
           multiEventCommonInfo.thread == record->getThread() &&
           multiEventCommonInfo.time == record->getTime() );
}
