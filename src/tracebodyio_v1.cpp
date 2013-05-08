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

istringstream TraceBodyIO_v1::fieldStream;
istringstream TraceBodyIO_v1::strLine;
string TraceBodyIO_v1::tmpstring;
string TraceBodyIO_v1::line;


bool TraceBodyIO_v1::ordered() const
{
  return false;
}

void TraceBodyIO_v1::read( TraceStream *file, MemoryBlocks& records,
                           hash_set<TEventType>& events ) const
{
  file->getline( line );

  if ( line[0] == '#' || line.size() == 0 )
    return;

  switch ( line[0] )
  {
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


void TraceBodyIO_v1::write( fstream& whichStream,
                            const KTrace& whichTrace,
                            MemoryTrace::iterator *record,
                            PRV_INT32 numIter ) const
{
  bool writeReady;
  TRecordType type = record->getType();

  if ( type == EMPTYREC )
    return;
  else if ( type & STATE )
    writeReady = writeState( line, whichTrace, record );
  else if ( type & EVENT )
    writeReady = writeEvent( line, whichTrace, record );
  else if ( type & COMM )
    writeReady = writeComm( line, whichTrace, record );
  else if ( type & GLOBCOMM )
    writeReady = writeGlobalComm( line, whichTrace, record );
  else if ( type & RSEND || type & RRECV )
    writeReady = false;
  else
  {
    writeReady = false;
    cerr << "No logging system yet. TraceBodyIO_v1::write()" << endl;
    cerr << "Unkwnown record type in memory." << endl;
  }

  if ( !writeReady )
    return;

  whichStream << line << endl;
}


void TraceBodyIO_v1::writeEvents( fstream& whichStream,
                                  const KTrace& whichTrace,
                                  vector<MemoryTrace::iterator *>& recordList ) const
{
  for ( PRV_UINT16 i = 0; i < recordList.size(); i++ )
  {
    if ( i > 0 )
    {
      line += ':';
      writeEvent( line, whichTrace, recordList[i], false );
    }
    else // i == 0
      writeEvent( line, whichTrace, recordList[i], true );
  }

  whichStream << line << endl;
}

void TraceBodyIO_v1::writeCommInfo( fstream& whichStream,
                                    const KTrace& whichTrace,
                                    PRV_INT32 numIter ) const
{}

/**********************
  Read line functions
***********************/
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
    cerr << "No logging system yet. TraceBodyIO_v1::readState()" << endl;
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> endtime ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v1::readState()" << endl;
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring );
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> state ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v1::readState()" << endl;
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }

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
    cerr << "No logging system yet. TraceBodyIO_v1::readEvent()" << endl;
    cerr << "Error reading event record." << endl;
    cerr << line << endl;
    return;
  }

  while ( !strLine.eof() )
  {
    std::getline( strLine, tmpstring, ':' );
    fieldStream.clear();
    fieldStream.str( tmpstring );
    if ( !( fieldStream >> eventtype ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v1::readEvent()" << endl;
      cerr << "Error reading event record." << endl;
      cerr << line << endl;
      return;
    }

    std::getline( strLine, tmpstring, ':' );
    fieldStream.clear();
    fieldStream.str( tmpstring );
    if ( !( fieldStream >> eventvalue ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v1::readEvent()" << endl;
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
  TCommSize size;
  TCommTag tag;

  strLine.clear();
  strLine.str( line );

  // Discarding record type
  std::getline( strLine, tmpstring, ':' );

  // Read the common info
  if ( !readCommon( strLine, CPU, appl, task, thread, logSend ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> phySend ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }

  if ( !readCommon( strLine, remoteCPU, remoteAppl, remoteTask, remoteThread,
                    logReceive ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> phyReceive ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> size ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error reading communication record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> tag ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v1::readComm()" << endl;
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
  records.setCommSize( size );
  records.setCommTag( tag );
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
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> CPU ) )
  {
    return false;
  }

  std::getline( line, tmpstring, ':' );
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> appl ) )
  {
    return false;
  }

  std::getline( line, tmpstring, ':' );
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> task ) )
  {
    return false;
  }

  std::getline( line, tmpstring, ':' );
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> thread ) )
  {
    return false;
  }

  if ( !processModel->isValidThread( appl - 1, task - 1, thread - 1 ) )
    return false;

  std::getline( line, tmpstring, ':' );
  fieldStream.clear();
  fieldStream.str( tmpstring );
  if ( !( fieldStream >> time ) )
  {
    return false;
  }

  return true;
}


/**************************
  Write records functions
***************************/
bool TraceBodyIO_v1::writeState( string& line,
                                 const KTrace& whichTrace,
                                 const MemoryTrace::iterator *record ) const
{
  if ( record->getType() & END )
    return false;

  ostringstream ostr;

  ostr << StateRecord << ':';
  writeCommon( ostr, whichTrace, record );
  ostr << record->getStateEndTime() << ':' << record->getState();

  line += ostr.str();
  return true;
}


bool TraceBodyIO_v1::writeEvent( string& line,
                                 const KTrace& whichTrace,
                                 const MemoryTrace::iterator *record,
                                 bool needCommons ) const
{
  ostringstream ostr;

  if ( needCommons )
  {
    ostr << EventRecord << ':';
    writeCommon( ostr, whichTrace, record );
  }
  ostr << record->getEventType() << ':' << record->getEventValue();

  line += ostr.str();
  return true;
}


bool TraceBodyIO_v1::writeComm( string& line,
                                const KTrace& whichTrace,
                                const MemoryTrace::iterator *record ) const
{
  ostringstream ostr;
  TCommID commID;
  TApplOrder recvAppl;
  TTaskOrder recvTask;
  TThreadOrder recvThread;

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


bool TraceBodyIO_v1::writeGlobalComm( string& line,
                                      const KTrace& whichTrace,
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


