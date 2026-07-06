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

#include "tracebodyio_csv.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

istringstream TraceBodyIO_csv::fieldStream;
istringstream TraceBodyIO_csv::strLine;
string TraceBodyIO_csv::tmpstring;
string TraceBodyIO_csv::line;
ostringstream TraceBodyIO_csv::ostr;

constexpr bool prv_atoll_v( std::string::const_iterator& it, const std::string::const_iterator& end )
{
  return true;
}

template< typename T, typename std::enable_if< std::is_integral< T >::value, T >::type* = nullptr >
constexpr bool prv_atoll_impl( std::string::const_iterator& it, const std::string::const_iterator& end, T& result )
{
  result       = 0;
  int negative = 1;

  if( *it == '-' )
  {
    if( std::is_unsigned< T >::value )
      return false;
    negative = -1;
    ++it;
  }

  if( *it >= '0' && *it <= '9' )
  {
    result = ( *it++ - '0' );
    while( *it >= '0' && *it <= '9' )
    {
      result = ( result * 10 ) + ( *it - '0' );
      ++it;
    }

    result *= negative;
  }

  return true;
}


template< typename T, typename std::enable_if< std::is_floating_point< T >::value, T >::type* = nullptr >
constexpr bool prv_atoll_impl( std::string::const_iterator& it, const std::string::const_iterator& end, T& result )
{
  result                 = 0.0;
  int negative           = 1;
  double numDecimals     = 1.0;
  bool decimalPointFound = false;

  if( *it == '-' )
  {
    negative = -1;
    ++it;
  }

  if( *it >= '0' && *it <= '9' )
  {
    result = ( *it++ - '0' );
    while( ( *it >= '0' && *it <= '9' ) || *it == '.' )
    {
      if( *it == '.' )
        decimalPointFound = true;
      else if( decimalPointFound )
        numDecimals *= 0.1;

      if( *it != '.' )
        result = ( result * 10 ) + ( *it - '0' );
      ++it;
    }

    result *= numDecimals;
    result *= negative;
  }

  return true;
}

template< typename T, typename... Targs >
constexpr bool prv_atoll_v( std::string::const_iterator& it, const std::string::const_iterator& end, T& result, Targs&... Fargs )
{
  if( it == end )
    return false;

  while( *it == ' ' )
    ++it;

  if( !prv_atoll_impl( it, end, result ) )
    return false;

  if( it == end )
    return sizeof...( Targs ) == 0;

  return prv_atoll_v( ++it, end, Fargs... );
}

template< typename T >
bool prv_atoll_precision( std::string::const_iterator& it, const std::string::const_iterator& end, T& result, double& decimals )
{
  long long int tmp  = 0;
  double decs        = 1.0;
  long long int sign = 1;

  if( *it == '-' )
  {
    sign = -1;
    ++it;
  }

  while( ( '0' <= *it && *it <= '9' ) || *it == '.' )
  {
    if( *it == '.' )
    {
      ++it;
      break;
    }
    else
    {
      tmp = ( tmp * 10 ) + ( *it - '0' );
      ++it;
    }
  }
  while( '0' <= *it && *it <= '9' )
  {
    tmp  = ( tmp * 10 ) + ( *it - '0' );
    decs = decs / 10.0f;
    ++it;
  }

  result   = tmp * sign;
  decimals = decs;
  return true;
}

TraceBodyIO_csv::TraceBodyIO_csv( Trace* trace, ProcessModel<>& whichProcessModel, bool appendCSV )
  : myTrace( trace ), myProcessModel( &whichProcessModel ), doAppendCSV( appendCSV )
{
}


bool TraceBodyIO_csv::ordered() const
{
  return true;
}

void TraceBodyIO_csv::read( TraceStream& file,
                            MemoryBlocks& records,
                            const ProcessModel<>& whichProcessModel,
                            const ResourceModel<>& whichResourceModel,
                            std::unordered_set< TState >& states,
                            std::unordered_set< TEventType >& events,
                            MetadataManager& traceInfo,
                            TRecordTime& endTime ) const
{
  file.getline( line );
  if( line.size() == 0 )
    return;

  if( line[ 0 ] == '#' )
  {
    readTraceInfo( line, traceInfo );
    return;
  }

  readEvents( whichProcessModel, whichResourceModel, line, records, events, endTime );
}

void TraceBodyIO_csv::bufferWrite( fstream& whichStream, bool writeReady, bool lineClear ) const
{
  if( writeReady )
  {
    whichStream << line << '\n';
    if( lineClear )
      line.clear();
  }
}


void TraceBodyIO_csv::write( std::fstream& whichStream,
                             const ProcessModel<>& whichProcessModel,
                             const ResourceModel<>& whichResourceModel,
                             MemoryTrace::iterator* record ) const
{
}


/**********************
  Read line functions
***********************/
inline void TraceBodyIO_csv::readTraceInfo( const std::string& line, MetadataManager& traceInfo ) const
{
  traceInfo.newMetadata( line );
}


inline void TraceBodyIO_csv::readEvents( const ProcessModel<>& whichProcessModel,
                                         const ResourceModel<>& whichResourceModel,
                                         const string& line,
                                         MemoryBlocks& records,
                                         unordered_set< TEventType >& events,
                                         TRecordTime& endTime ) const
{
  bool ignoreRecord = false;
  TCPUOrder CPU     = 0;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  TRecordTime time;
  TRecordTime begintime;
  TEventValue eventvalue;
  double decimals;

  strLine.clear();
  strLine.str( line );
  // Read the common info
  if( !readCommon( whichResourceModel, strLine, CPU, appl, task, thread, begintime, time, eventvalue, decimals ) )
  {
    cerr << "Error reading record (CSV file)." << endl;
    cerr << line << endl;
    return;
  }
  if( endTime < begintime + time )
    endTime = begintime + time;
  TEventType eventtype = 1;

  if( doAppendCSV )
  {
    try
    {
      thread = whichProcessModel.getGlobalThread( appl - 1, task - 1, thread - 1 );
    }
    catch( TraceHeaderException& e )
    {
      ignoreRecord = true;
      cerr << "Error reading record (CSV file):\n" << line << endl;
    }

    if( !ignoreRecord )
    {
      records.newRecord( thread );
      records.setThread( thread );
    }
  }
  else
  {
    records.newRecord();
    records.setThread( appl - 1, task - 1, thread - 1 );
  }
  records.setRecordType( EVENT );
  records.setTime( begintime );
  records.setCPU( CPU );
  records.setEventType( eventtype );
  records.setEventValue( eventvalue );
  events.insert( eventtype );

  myTrace->setEventTypePrecision( eventtype, decimals );
}


inline bool TraceBodyIO_csv::readCommon( const ResourceModel<>& whichResourceModel,
                                         istringstream& line,
                                         TCPUOrder& CPU,
                                         TApplOrder& appl,
                                         TTaskOrder& task,
                                         TThreadOrder& thread,
                                         TRecordTime& begintime,
                                         TRecordTime& time,
                                         TEventValue& eventvalue,
                                         double& decimals ) const
{
  static ProcessModel<>::ThreadLocation lastATT = { 0, 0, 0 };

  std::getline( line, tmpstring, '.' );
  if( line.eof() )
    return false;
  auto it = tmpstring.cbegin();
  if( !prv_atoll_v( it, tmpstring.cend(), appl ) || appl == 0 )
    throw ParaverKernelException( TErrorCode::csvWrongFormat );

  std::getline( line, tmpstring, '.' );
  if( line.eof() )
    return false;
  it = tmpstring.cbegin();
  if( !prv_atoll_v( it, tmpstring.end(), task ) || task == 0 )
    throw ParaverKernelException( TErrorCode::csvWrongFormat );

  std::getline( line, tmpstring, '\t' );
  if( line.eof() )
    return false;
  it = tmpstring.cbegin();
  if( !prv_atoll_v( it, tmpstring.end(), thread ) || thread == 0 )
    throw ParaverKernelException( TErrorCode::csvWrongFormat );

  if( !doAppendCSV )
  {
    ProcessModel<>::ThreadLocation currATT = { appl, task, thread };
    if( !( currATT == lastATT ) )
    {
      lastATT = currATT;
      --currATT.appl;
      --currATT.task;
      --currATT.thread;
      myProcessModel->addApplTaskThread( currATT );
    }
  }

  std::getline( line, tmpstring, '\t' );
  if( line.eof() )
    return false;
  it = tmpstring.cbegin();
  if( !prv_atoll_v( it, tmpstring.end(), begintime ) )
    throw ParaverKernelException( TErrorCode::csvWrongFormat );

  std::getline( line, tmpstring, '\t' );
  if( line.eof() )
    return false;
  it = tmpstring.cbegin();
  if( !prv_atoll_v( it, tmpstring.end(), time ) )
    throw ParaverKernelException( TErrorCode::csvWrongFormat );

  std::getline( line, tmpstring );
  it = tmpstring.cbegin();
  if( !prv_atoll_precision( it, tmpstring.end(), eventvalue, decimals ) )
    throw ParaverKernelException( TErrorCode::csvWrongFormat );

  return true;
}
