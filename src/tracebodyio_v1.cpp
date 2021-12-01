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
#include <string>
#include <sstream>
#include <type_traits>

#include "tracebodyio_v1.h"

using namespace std;

string TraceBodyIO_v1::multiEventLine;
TraceBodyIO_v1::TMultiEventCommonInfo TraceBodyIO_v1::multiEventCommonInfo =
        { nullptr, (TThreadOrder)0, (TCPUOrder)0, (TRecordTime)0 };

string TraceBodyIO_v1::line;
ostringstream TraceBodyIO_v1::ostr;

// Optimization on conversion string to numbers, but with no error control
//#define USE_ATOLL
// Even more optimization using custom function instead of atoll with error checking
#define USE_PRV_ATOLL
constexpr bool prv_atoll_v( string::const_iterator& it, const string::const_iterator& end )
{
  return true;
}

template <typename T, typename... Targs>
constexpr bool prv_atoll_v( string::const_iterator& it, const string::const_iterator& end, T& result, Targs&... Fargs )
{
  result = 0;
  int negative = 1;

  if( it == end )
    return false;

  if( *it == '-' )
  {
    if( is_unsigned<T>::value )
      return false;
    negative = -1;
    ++it;
  }

  if( *it >= '0' && *it <= '9' )
  {
    result = ( *it++ - '0' );
    while( *it >= '0' && *it <= '9' )
      result = ( result * 10 ) + ( *it++ - '0' );

    result *= negative;
  }

  if( it == end )
    return sizeof...( Targs ) == 0;

  return prv_atoll_v( ++it, end, Fargs... );
}

bool TraceBodyIO_v1::ordered() const
{
  return false;
}

void TraceBodyIO_v1::read( TraceStream *file,
                           MemoryBlocks& records,
                           const ProcessModel& whichProcessModel,
                           const ResourceModel& whichResourceModel,
                           std::unordered_set<TState>& states,
                           std::unordered_set<TEventType>& events,
                           MetadataManager& traceInfo,
                           TRecordTime& endTime  ) const
{
  file->getline( line );

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
      cerr << "TraceBodyIO_v1::read()" << endl;
      cerr << "Unkwnown record type." << endl;
      break;
  };
}

void TraceBodyIO_v1::bufferWrite( fstream& whichStream, bool writeReady, bool lineClear ) const
{
  if ( writeReady )
  {
    whichStream << line << '\n';
    if ( lineClear )
      line.clear();
  }
}


void TraceBodyIO_v1::write( fstream& whichStream,
                            const ProcessModel& whichProcessModel,
                            const ResourceModel& whichResourceModel,
                            MemoryTrace::iterator *record ) const
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
      cerr << "TraceBodyIO_v1::write()" << endl;
      cerr << "Unkwnown record type in memory." << endl;
    }

    bufferWrite( whichStream, writeReady, false );
  }
}


/**********************
  Read line functions
***********************/
inline void TraceBodyIO_v1::readTraceInfo( const std::string& line, MetadataManager& traceInfo ) const
{
  traceInfo.NewMetadata( line );
  // dummy set also to false if it is a comment
}

inline void TraceBodyIO_v1::readState( const string& line,
                                       const ProcessModel& whichProcessModel,
                                       const ResourceModel& whichResourceModel,
                                       MemoryBlocks& records,
                                       unordered_set<TState>& states ) const
{
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  TRecordTime time;
  TRecordTime endtime;
  TState state;

  auto it = line.begin() + 2;

  // Read the common info
  if ( !readCommon( whichProcessModel, whichResourceModel, it, line.cend(), CPU, appl, task, thread, time ) )
  {
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }

  if( !prv_atoll_v( it, line.cend(), endtime, state ) )
  {
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }

  records.newRecord();
  records.setType( STATE + BEGIN );
  records.setTime( time );
  records.setCPU( CPU );
  records.setThread( appl - 1, task - 1, thread - 1 );
  records.setState( state );
  records.setStateEndTime( endtime );

  if ( endtime != -1 )
  {
    records.newRecord();
    records.setType( STATE + END );
    records.setTime( endtime );
    records.setCPU( CPU );
    records.setThread( appl - 1, task - 1, thread - 1 );
    records.setState( state );
    records.setStateEndTime( time );
  }

  states.insert( state );
}


inline void TraceBodyIO_v1::readEvent( const string& line,
                                       const ProcessModel& whichProcessModel,
                                       const ResourceModel& whichResourceModel,
                                       MemoryBlocks& records,
                                       unordered_set<TEventType>& events ) const
{
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  TRecordTime time;
  TEventType eventtype;
  TEventValue eventvalue;

  auto it = line.begin() + 2;

  // Read the common info
  if ( !readCommon( whichProcessModel, whichResourceModel, it, line.cend(), CPU, appl, task, thread, time ) )
  {
    cerr << "Error reading event record." << endl;
    cerr << line << endl;
    return;
  }

  while ( it != line.end() )
  {
    if( !prv_atoll_v( it, line.cend(), eventtype, eventvalue ) )
    {
      cerr << "Error reading event record." << endl;
      cerr << line << endl;
      return;
    }

    records.newRecord();
    records.setType( EVENT );
    records.setTime( time );
    records.setCPU( CPU );
    records.setThread( appl - 1, task - 1, thread - 1 );
    records.setEventType( eventtype );
    records.setEventValue( eventvalue );

    events.insert( eventtype );
  }
}


inline void TraceBodyIO_v1::readComm( const string& line,
                                      const ProcessModel& whichProcessModel,
                                      const ResourceModel& whichResourceModel,
                                      MemoryBlocks& records ) const
{
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  TRecordTime logSend;
  TRecordTime phySend;
  TRecordTime logReceive;
  TRecordTime phyReceive;
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
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }

  if( !prv_atoll_v( it, line.cend(), phySend, remoteCPU, remoteAppl, remoteTask, remoteThread, logReceive, phyReceive, commSize, commTag ) )
  {
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }

  records.newComm();
  records.setSenderCPU( CPU );
  records.setSenderThread( appl - 1, task - 1, thread - 1 );
  records.setReceiverCPU( remoteCPU );
  records.setReceiverThread( remoteAppl - 1, remoteTask - 1, remoteThread - 1 );
  records.setLogicalSend( logSend );
  records.setPhysicalSend( phySend );
  records.setLogicalReceive( logReceive );
  records.setPhysicalReceive( phyReceive );
  records.setCommSize( commSize );
  records.setCommTag( commTag );
}


inline void TraceBodyIO_v1::readGlobalComm( const string& line, MemoryBlocks& records ) const
{}


inline bool TraceBodyIO_v1::readCommon( const ProcessModel& whichProcessModel,
                                        const ResourceModel& whichResourceModel,
                                        string::const_iterator& it,
                                        const string::const_iterator& end,
                                        TCPUOrder& CPU,
                                        TApplOrder& appl,
                                        TTaskOrder& task,
                                        TThreadOrder& thread,
                                        TRecordTime& time ) const
{
  return prv_atoll_v( it, end, CPU, appl, task, thread, time ) &&
         whichResourceModel.isValidGlobalCPU( CPU ) &&
         whichProcessModel.isValidThread( appl - 1, task - 1, thread - 1 );
}


/**************************
  Write records functions
***************************/
bool TraceBodyIO_v1::writeState( const ProcessModel& whichProcessModel,
                                 const ResourceModel& whichResourceModel,
                                 const MemoryTrace::iterator *record ) const
{
  if ( record->getType() & END )
    return false;

  ostr.clear();
  ostr.str( "" );
  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  ostr << StateRecord << ':';
  writeCommon( ostr, whichProcessModel, whichResourceModel, record );
  ostr << record->getStateEndTime() << ':' << record->getState();

  line += ostr.str();
  return true;
}


bool TraceBodyIO_v1::writePendingMultiEvent( const ProcessModel& whichProcessModel ) const
{
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;

  bool writeLine = !multiEventLine.empty();

  if ( writeLine )
  {
    ostr.clear();
    ostr.str( "" );
    ostr << fixed;
    ostr << dec;
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


void TraceBodyIO_v1::appendEvent( const MemoryTrace::iterator *record ) const
{
  ostr.clear();
  ostr.str( "" );
  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  if ( !multiEventLine.empty() )
    ostr << ':';

  ostr << record->getEventType() << ':' << record->getEventValueAsIs();

  multiEventLine += ostr.str();
}


bool TraceBodyIO_v1::writeComm( const ProcessModel& whichProcessModel,
                                const ResourceModel& whichResourceModel,
                                const MemoryTrace::iterator *record ) const
{
  TCommID commID;
  TApplOrder recvAppl;
  TTaskOrder recvTask;
  TThreadOrder recvThread;

  ostr.clear();
  ostr.str( "" );
  ostr << fixed;
  ostr << dec;
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


bool TraceBodyIO_v1::writeGlobalComm( const ProcessModel& whichProcessModel,
                                      const MemoryTrace::iterator *record ) const
{
  return true;
}


void TraceBodyIO_v1::writeCommon( ostringstream& line,
                                  const ProcessModel& whichProcessModel,
                                  const ResourceModel& whichResourceModel,
                                  const MemoryTrace::iterator *record ) const
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


bool TraceBodyIO_v1::sameMultiEvent( const MemoryTrace::iterator *record ) const
{
  return ( multiEventCommonInfo.cpu == record->getCPU() &&
           multiEventCommonInfo.thread == record->getThread() &&
           multiEventCommonInfo.time == record->getTime() );
}
