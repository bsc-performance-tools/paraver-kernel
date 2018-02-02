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

#include <string>
#include <sstream>
#include <iostream>
#include "tracebodyio_v1.h"

using namespace std;

string TraceBodyIO_v1::multiEventLine;
TraceBodyIO_v1::TMultiEventCommonInfo TraceBodyIO_v1::multiEventCommonInfo =
        { (TThreadOrder)0, (TCPUOrder)0, (TRecordTime)0 };

istringstream TraceBodyIO_v1::fieldStream;
istringstream TraceBodyIO_v1::strLine;
string TraceBodyIO_v1::tmpstring;
string TraceBodyIO_v1::line;
ostringstream TraceBodyIO_v1::ostr;

// Optimization on conversion string to numbers, but with no error control
//#define USE_ATOLL
// Even more optimization using custom function instead of atoll with error checking
#define USE_PRV_ATOLL
template <typename T>
bool prv_atoll( const char *p, T *result )
{
  register long long int tmp = 0;
  bool neg = false;

  if( *p == '-' )
  {
    neg = true;
    ++p;
  }
  while( *p >= '0' && *p <= '9' )
  {
    tmp = ( tmp * 10 ) + ( *p - '0' );
    ++p;
  }

  if( *p != '\n' && *p != '\r' && *p != '\0' )
    return false;

  if( neg )
  {
    tmp = -tmp;
  }
  *result = tmp;
  return true;
}

bool TraceBodyIO_v1::ordered() const
{
  return false;
}

void TraceBodyIO_v1::read( TraceStream *file, MemoryBlocks& records,
                           hash_set<TEventType>& events, MetadataManager& traceInfo ) const
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
      readState( line, records );
      break;

    case EventRecord:
      readEvent( line, records, events );
      break;

    case CommRecord:
      readComm( line, records );
      break;

    case GlobalCommRecord:
      //readGlobalComm( line, records );
      break;

    default:
      cerr << "No logging system yet. TraceBodyIO_v1::read()" << endl;
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
                            const KTrace& whichTrace,
                            MemoryTrace::iterator *record,
                            PRV_INT32 numIter ) const
{
  bool writeReady;
  TRecordType type = record->getType();
  line.clear();

  if ( type == EMPTYREC )
  {
    writeReady = writePendingMultiEvent( whichTrace );
    bufferWrite( whichStream, writeReady );
  }
  else
  {
    if ( type & STATE )
    {
      writeReady = writePendingMultiEvent( whichTrace );
      bufferWrite( whichStream, writeReady );
      writeReady = writeState( whichTrace, record );
    }
    else if ( type & EVENT )
    {
      if ( !sameMultiEvent( record ) )
      {
        writeReady = writePendingMultiEvent( whichTrace );

        multiEventCommonInfo.cpu = record->getCPU();
        multiEventCommonInfo.thread = record->getThread();
        multiEventCommonInfo.time = record->getTime();

        multiEventLine.clear();
      }

      appendEvent( record );
    }
    else if ( type & COMM )
    {
      writeReady = writePendingMultiEvent( whichTrace );
      bufferWrite( whichStream, writeReady );

      writeReady = writeComm( whichTrace, record );
    }
    else if ( type & GLOBCOMM )
    {
      writeReady = writePendingMultiEvent( whichTrace );
      bufferWrite( whichStream, writeReady );

      writeReady = writeGlobalComm( whichTrace, record );
    }
    else if ( type & RSEND || type & RRECV )
      writeReady = false;
    else
    {
      writeReady = false;
      cerr << "No logging system yet. TraceBodyIO_v1::write()" << endl;
      cerr << "Unkwnown record type in memory." << endl;
    }

    bufferWrite( whichStream, writeReady, false );
  }
}


void TraceBodyIO_v1::writeCommInfo( fstream& whichStream,
                                    const KTrace& whichTrace,
                                    PRV_INT32 numIter ) const
{}

/**********************
  Read line functions
***********************/
inline void TraceBodyIO_v1::readTraceInfo(  const std::string& line, MetadataManager& traceInfo ) const
{
  traceInfo.NewMetadata( line );
  // dummy set also to false if it is a comment
}

inline void TraceBodyIO_v1::readState( const string& line, MemoryBlocks& records ) const
{
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  TRecordTime time;
  TRecordTime endtime;
  TState state;

  strLine.clear();
  strLine.str( line );

  // Discarding record type
  std::getline( strLine, tmpstring, ':' );
  // Read the common info
  if ( !readCommon( strLine, CPU, appl, task, thread, time ) )
  {
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );

#ifdef USE_ATOLL
  endtime = atoll( tmpstring.c_str() );
#else
#ifdef USE_PRV_ATOLL
  if( !prv_atoll<TRecordTime>( tmpstring.c_str(), &endtime ) )
#else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> endtime ) )
#endif
  {
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }
#endif

  std::getline( strLine, tmpstring );

#ifdef USE_ATOLL
  state = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TState>( tmpstring.c_str(), &state ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> state ) )
  #endif
  {
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }
#endif

  //if ( time == endtime ) return;

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
}


inline void TraceBodyIO_v1::readEvent( const string& line, MemoryBlocks& records,
                                       hash_set<TEventType>& events ) const
{
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  TRecordTime time;
  TEventType eventtype;
  TEventValue eventvalue;

  strLine.clear();
  strLine.str( line );

  // Discarding record type
  std::getline( strLine, tmpstring, ':' );

  // Read the common info
  if ( !readCommon( strLine, CPU, appl, task, thread, time ) )
  {
    cerr << "Error reading event record." << endl;
    cerr << line << endl;
    return;
  }

  while ( !strLine.eof() )
  {
    std::getline( strLine, tmpstring, ':' );

#ifdef USE_ATOLL
    eventtype = atoll( tmpstring.c_str() );
#else
    #ifdef USE_PRV_ATOLL
    if( !prv_atoll<TEventType>( tmpstring.c_str(), &eventtype ) )
    #else
    fieldStream.clear();
    fieldStream.str( tmpstring );
    if ( !( fieldStream >> eventtype ) )
    #endif
    {
      cerr << "Error reading event record." << endl;
      cerr << line << endl;
      return;
    }
#endif

    std::getline( strLine, tmpstring, ':' );

#ifdef USE_ATOLL
    eventvalue = atoll( tmpstring.c_str() );
#else
    #ifdef USE_PRV_ATOLL
    if( !prv_atoll<TEventValue>( tmpstring.c_str(), &eventvalue ) )
    #else
    fieldStream.clear();
    fieldStream.str( tmpstring );
    if ( !( fieldStream >> eventvalue ) )
    #endif
    {
      cerr << "Error reading event record." << endl;
      cerr << line << endl;
      return;
    }
#endif

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


inline void TraceBodyIO_v1::readComm( const string& line, MemoryBlocks& records ) const
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

  strLine.clear();
  strLine.str( line );

  // Discarding record type
  std::getline( strLine, tmpstring, ':' );

  // Read the common info
  if ( !readCommon( strLine, CPU, appl, task, thread, logSend ) )
  {
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );

#ifdef USE_ATOLL
  phySend = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TRecordTime>( tmpstring.c_str(), &phySend ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> phySend ) )
  #endif
  {
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }
#endif

  if ( !readCommon( strLine, remoteCPU, remoteAppl, remoteTask, remoteThread,
                    logReceive ) )
  {
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );

#ifdef USE_ATOLL
  phyReceive = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TRecordTime>( tmpstring.c_str(), &phyReceive ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> phyReceive ) )
  #endif
  {
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }
#endif

  std::getline( strLine, tmpstring, ':' );

#ifdef USE_ATOLL
  commSize = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TCommSize>( tmpstring.c_str(), &commSize ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> commSize ) )
  #endif
  {
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }
#endif

  std::getline( strLine, tmpstring, ':' );

#ifdef USE_ATOLL
  commTag = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TCommTag>( tmpstring.c_str(), &commTag ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> commTag ) )
  #endif
  {
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }
#endif

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


inline bool TraceBodyIO_v1::readCommon( istringstream& line,
                                        TCPUOrder& CPU,
                                        TApplOrder& appl,
                                        TTaskOrder& task,
                                        TThreadOrder& thread,
                                        TRecordTime& time ) const
{
  std::getline( line, tmpstring, ':' );
#ifdef USE_ATOLL
  CPU = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TCPUOrder>( tmpstring.c_str(), &CPU ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> CPU ) )
  #endif
  {
    return false;
  }
#endif

  if ( !resourceModel->isValidCPU( CPU ) )
    return false;


  std::getline( line, tmpstring, ':' );
#ifdef USE_ATOLL
  appl = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TApplOrder>( tmpstring.c_str(), &appl ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> appl ) )
  #endif
  {
    return false;
  }
#endif


  std::getline( line, tmpstring, ':' );
#ifdef USE_ATOLL
  task = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TTaskOrder>( tmpstring.c_str(), &task ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> task ) )
  #endif
  {
    return false;
  }
#endif

  std::getline( line, tmpstring, ':' );
#ifdef USE_ATOLL
  thread = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TThreadOrder>( tmpstring.c_str(), &thread ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> thread ) )
  #endif
  {
    return false;
  }
#endif

  if ( !processModel->isValidThread( appl - 1, task - 1, thread - 1 ) )
    return false;

  std::getline( line, tmpstring, ':' );
#ifdef USE_ATOLL
  time = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TRecordTime>( tmpstring.c_str(), &time ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> time ) )
  #endif
  {
    return false;
  }
#endif

  return true;
}


/**************************
  Write records functions
***************************/
bool TraceBodyIO_v1::writeState( const KTrace& whichTrace,
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
  writeCommon( ostr, whichTrace, record );
  ostr << record->getStateEndTime() << ':' << record->getState();

  line += ostr.str();
  return true;
}


bool TraceBodyIO_v1::writePendingMultiEvent( const KTrace& whichTrace ) const
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

    whichTrace.getThreadLocation( multiEventCommonInfo.thread, appl, task, thread );
    ostr << appl + 1 << ':' << task + 1 << ':' << thread + 1 << ':';

    ostr << multiEventCommonInfo.time << ':';
    ostr << multiEventLine;

    line += ostr.str();

    multiEventCommonInfo.cpu = 0;
    multiEventCommonInfo.thread = 0;
    multiEventCommonInfo.time = 0;

    multiEventLine.clear();
  }

  return writeLine;
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

  ostr << record->getEventType() << ':' << record->getEventValue();

  multiEventLine += ostr.str();
}


bool TraceBodyIO_v1::writeComm( const KTrace& whichTrace,
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

  commID = record->getCommIndex();

  ostr << CommRecord << ':';
  writeCommon( ostr, whichTrace, record );
  ostr << whichTrace.getPhysicalSend( commID ) << ':';
  if ( whichTrace.existResourceInfo() )
    ostr << whichTrace.getReceiverCPU( commID ) << ':';
  else
    ostr << '0' << ':';
  whichTrace.getThreadLocation( whichTrace.getReceiverThread( commID ),
                                recvAppl, recvTask, recvThread );
  ostr << recvAppl + 1 << ':' << recvTask + 1 << ':' << recvThread + 1 << ':';
  ostr << whichTrace.getLogicalReceive( commID ) << ':';
  ostr << whichTrace.getPhysicalReceive( commID ) << ':';

  ostr << whichTrace.getCommSize( commID ) << ':';
  ostr << whichTrace.getCommTag( commID );

  line += ostr.str();
  return true;
}


bool TraceBodyIO_v1::writeGlobalComm( const KTrace& whichTrace,
                                      const MemoryTrace::iterator *record ) const
{
  return true;
}


void TraceBodyIO_v1::writeCommon( ostringstream& line,
                                  const KTrace& whichTrace,
                                  const MemoryTrace::iterator *record ) const
{
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;

  if ( whichTrace.existResourceInfo() )
    line << record->getCPU() << ':';
  else
    line << '0' << ':';

  whichTrace.getThreadLocation( record->getThread(), appl, task, thread );
  line << appl + 1 << ':' << task + 1 << ':' << thread + 1 << ':';
  line << record->getTime() << ':';
}


bool TraceBodyIO_v1::sameMultiEvent( const MemoryTrace::iterator *record ) const
{
  return ( multiEventCommonInfo.cpu == record->getCPU() &&
           multiEventCommonInfo.thread == record->getThread() &&
           multiEventCommonInfo.time == record->getTime() );
}
