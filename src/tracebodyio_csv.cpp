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

#include <string>
#include <sstream>
#include <iostream>
#include "tracebodyio_csv.h"

using namespace std;

string TraceBodyIO_csv::multiEventLine;
TraceBodyIO_csv::TMultiEventCommonInfo TraceBodyIO_csv::multiEventCommonInfo =
        { (TThreadOrder)0, (TCPUOrder)0, (TRecordTime)0 };

istringstream TraceBodyIO_csv::fieldStream;
istringstream TraceBodyIO_csv::strLine;
string TraceBodyIO_csv::tmpstring;
string TraceBodyIO_csv::line;
ostringstream TraceBodyIO_csv::ostr;

// Optimization on conversion string to numbers, but with no error control
//#define USE_ATOLL
// Even more optimization using custom function instead of atoll with error checking
#define USE_PRV_ATOLL
template <typename T>
bool prv_atoll( const char *p, T *result )
{
  register long long int tmp = 0;
  long long int sign = 1;

  if( *p == '-' )
  {
    sign = -1;
    ++p;
  }
  while( *p >= '0' && *p <= '9' )
  {
    tmp = ( tmp * 10 ) + ( *p - '0' );
    ++p;
  }

  if( *p != '\n' && *p != '\r' && *p != '\0' )
    return false;

  *result = tmp * sign;
  return true;
}

template <typename T>
bool prv_atoll( const char *p, T *result, double *decimals )
{
  register long long int tmp = 0;
  double decs = 1.0;
  long long int sign = 1;

  if( *p == '-' )
  {
    sign = -1;
    ++p;
  }
  bool dotFound = false;
  while( ( '0' <= *p && *p <= '9' ) || *p == '.' )
  {
    if ( *p == '.' )
    {
      ++p;
      break;
    }
    else
    {
      tmp = ( tmp * 10 ) + ( *p - '0' );
      ++p;
    }
  }
  while( '0' <= *p && *p <= '9' )
  {
    tmp = ( tmp * 10 ) + ( *p - '0' );
    decs = decs / 10.0f;
    ++p;
  }

  if( *p != '\n' && *p != '\r' && *p != '\0' )
    return false;

  *result = tmp * sign;
  *decimals = decs;
  return true;
}

TraceBodyIO_csv::TraceBodyIO_csv( Trace* trace )
: whichTrace( trace )
{}


bool TraceBodyIO_csv::ordered() const
{
  return true;
}

void TraceBodyIO_csv::read( TraceStream *file, MemoryBlocks& records,
                           unordered_set<TState>& states, unordered_set<TEventType>& events,
                           MetadataManager& traceInfo ) const
{
  file->getline( line );
  if ( line.size() == 0 )
    return;

  readEvents( line, records, states );  
}

void TraceBodyIO_csv::bufferWrite( fstream& whichStream, bool writeReady, bool lineClear ) const
{
  if ( writeReady )
  {
    whichStream << line << '\n';
    if ( lineClear )
      line.clear();
  }
}


void TraceBodyIO_csv::write( fstream& whichStream,
                            const KTrace& whichTrace,
                            MemoryTrace::iterator *record,
                            PRV_INT32 numIter ) const
{
  bool writeReady = false;
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
      cerr << "No logging system yet. TraceBodyIO_csv::write()" << endl;
      cerr << "Unkwnown record type in memory." << endl;
    }

    bufferWrite( whichStream, writeReady, false );
  }
}


void TraceBodyIO_csv::writeCommInfo( fstream& whichStream,
                                    const KTrace& whichTrace,
                                    PRV_INT32 numIter ) const
{}

/**********************
  Read line functions
***********************/
inline void TraceBodyIO_csv::readTraceInfo(  const std::string& line, MetadataManager& traceInfo ) const
{
  traceInfo.NewMetadata( line );
  // dummy set also to false if it is a comment
}


inline void TraceBodyIO_csv::readEvents( const string& line, MemoryBlocks& records,
                                       unordered_set<TEventType>& events ) const
{ 
  TCPUOrder CPU = 0;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  TRecordTime time;
  TRecordTime begintime;
  TRecordTime endtime;
  TEventValue eventvalue;
  double decimals = 1.05;

  strLine.clear();
  strLine.str( line );
  // Read the common info
  if ( !readCommon( strLine, CPU, appl, task, thread, 
                    begintime, time, eventvalue, decimals ) )
  {
    cerr << "Error reading state record (error in Common)." << endl;
    cerr << line << endl;
    return;
  }
  endtime = begintime + time;
  TEventType eventtype = 1;

  //Event
  records.newRecord();
  records.setType( EVENT );
  records.setTime( begintime );
  records.setCPU( CPU );
  records.setThread( appl - 1, task - 1, thread - 1 );
  records.setEventType( eventtype );
  records.setEventValue( eventvalue );
  events.insert( eventtype );

  whichTrace->setEventTypePrecision( eventtype, decimals );
}


inline bool TraceBodyIO_csv::readCommon( istringstream& line,
                                        TCPUOrder& CPU,
                                        TApplOrder& appl,
                                        TTaskOrder& task,
                                        TThreadOrder& thread,
                                        TRecordTime& begintime,
                                        TRecordTime& time,
                                        TEventValue& eventvalue,
                                        double& decimals ) const
{ 
  std::getline( line, tmpstring, '.' ); 
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

  std::getline( line, tmpstring, '.' ); 
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

  std::getline( line, tmpstring, '\t' );  
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

  std::getline( line, tmpstring, '.' ); 
#ifdef USE_ATOLL
  begintime = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TRecordTime>( tmpstring.c_str(), &begintime ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> begintime ) )
  #endif
  {
    return false;
  }
#endif

  std::getline( line, tmpstring, '\t' );
  std::getline( line, tmpstring, '.' );
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

  std::getline( line, tmpstring, '\t' );
  std::getline( line, tmpstring, '\n' );
#ifdef USE_ATOLL
  eventvalue = atoll( tmpstring.c_str() );
#else
  #ifdef USE_PRV_ATOLL
  if( !prv_atoll<TEventValue>( tmpstring.c_str(), &eventvalue, &decimals ) )
  #else
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> eventvalue ) )
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
bool TraceBodyIO_csv::writeState( const KTrace& whichTrace,
                                 const MemoryTrace::iterator *record ) const
{
  if ( record->getType() & END )
    return false;

  ostr.clear();
  ostr.str( "" );
  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  ostr << StateRecord << '\t';
  writeCommon( ostr, whichTrace, record );
  ostr << record->getStateEndTime() << '\t' << record->getState();

  line += ostr.str();
  return true;
}


bool TraceBodyIO_csv::writePendingMultiEvent( const KTrace& whichTrace ) const
{ 
  return true;
}


void TraceBodyIO_csv::appendEvent( const MemoryTrace::iterator *record ) const
{ }


bool TraceBodyIO_csv::writeComm( const KTrace& whichTrace,
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

  ostr << CommRecord << '\t';
  writeCommon( ostr, whichTrace, record );
  ostr << whichTrace.getPhysicalSend( commID ) << '\t';
  if ( whichTrace.existResourceInfo() )
    ostr << whichTrace.getReceiverCPU( commID ) << '\t';
  else
    ostr << '0' << '\t';
  whichTrace.getThreadLocation( whichTrace.getReceiverThread( commID ),
                                recvAppl, recvTask, recvThread );
  ostr << recvAppl + 1 << '\t' << recvTask + 1 << '\t' << recvThread + 1 << '\t';
  ostr << whichTrace.getLogicalReceive( commID ) << '\t';
  ostr << whichTrace.getPhysicalReceive( commID ) << '\t';

  ostr << whichTrace.getCommSize( commID ) << '\t';
  ostr << whichTrace.getCommTag( commID );

  line += ostr.str();
  return true;
}


bool TraceBodyIO_csv::writeGlobalComm( const KTrace& whichTrace,
                                      const MemoryTrace::iterator *record ) const
{
  return true;
}


void TraceBodyIO_csv::writeCommon( ostringstream& line,
                                  const KTrace& whichTrace,
                                  const MemoryTrace::iterator *record ) const
{
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;

  if ( whichTrace.existResourceInfo() )
    line << record->getCPU() << '\t';
  else
    line << '0' << '\t';

  whichTrace.getThreadLocation( record->getThread(), appl, task, thread );
  line << appl + 1 << '\t' << task + 1 << '\t' << thread + 1 << '\t';
  line << record->getTime() << '\t';
}


bool TraceBodyIO_csv::sameMultiEvent( const MemoryTrace::iterator *record ) const
{
  return ( multiEventCommonInfo.cpu == record->getCPU() &&
           multiEventCommonInfo.thread == record->getThread() &&
           multiEventCommonInfo.time == record->getTime() );
}
