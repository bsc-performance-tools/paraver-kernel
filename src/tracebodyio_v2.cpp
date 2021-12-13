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
#include "tracebodyio_v2.h"

using namespace std;


bool TraceBodyIO_v2::ordered() const
{
  return true;
}

void TraceBodyIO_v2::read( TraceStream *file,
                           MemoryBlocks& records,
                           const ProcessModel& whichProcessModel,
                           const ResourceModel<>& whichResourceModel,
                           unordered_set<TState>& states,
                           unordered_set<TEventType>& events,
                           MetadataManager& traceInfo,
                           TRecordTime& endTime  ) const
{
  string line;

  file->getline( line );

  if ( line[0] == '#' || line.size() == 0 )
    return;

  switch ( line[0] )
  {
    case StateBeginRecord:
    case StateEndRecord:
      readState( line, whichProcessModel, whichResourceModel, records, states );
      break;

    case EventRecord:
      readEvent( line, whichProcessModel, whichResourceModel, records, events );
      break;

    case CommRecord:
    case LogicalSendRecord:
    case LogicalRecvRecord:
    case PhysicalSendRecord:
    case PhysicalRecvRecord:
      readComm( line, whichProcessModel, whichResourceModel, records );
      break;

    case GlobalCommRecord:
      //readGlobalComm( line, records );
      break;

    default:
      cerr << "No logging system yet. TraceBodyIO_v2::read()" << endl;
      cerr << "Unkwnown record type." << endl;
      break;
  };
}


void TraceBodyIO_v2::write( std::fstream& whichStream,
                            const ProcessModel& whichProcessModel,
                            const ResourceModel<>& whichResourceModel,
                            MemoryTrace::iterator *record ) const
{
  string line;
  bool writeReady;
  TRecordType type = record->getType();

  if ( type == EMPTYREC )
    return;
  else if ( type & STATE )
    writeReady = writeState( line, whichProcessModel, whichResourceModel, record );
  else if ( type & EVENT )
    writeReady = writeEvent( line, whichProcessModel, whichResourceModel, record, true );
  else if ( type & COMM )
    writeReady = writeCommRecord( line, record );
  else if ( type & GLOBCOMM )
    writeReady = writeGlobalComm( line, whichProcessModel, record );
  else if ( type & RSEND || type & RRECV )
    writeReady = false;
  else
  {
    writeReady = false;
    cerr << "No logging system yet. TraceBodyIO_v2::write()" << endl;
    cerr << "Unkwnown record type in memory." << endl;
  }

  if ( !writeReady )
    return;

  whichStream << line << endl;
}


void TraceBodyIO_v2::writeEvents( fstream& whichStream,
                                  const ProcessModel& whichProcessModel,
                                  const ResourceModel<>& whichResourceModel,
                                  vector<MemoryTrace::iterator *>& recordList ) const
{
  string line;

  for ( PRV_UINT16 i = 0; i < recordList.size(); i++ )
  {
    if ( i > 0 )
    {
      line += ':';
      writeEvent( line, whichProcessModel, whichResourceModel, recordList[i], false );
    }
    else // i == 0
      writeEvent( line, whichProcessModel, whichResourceModel, recordList[i], true );
  }

  whichStream << line << endl;
}


void TraceBodyIO_v2::writeCommInfo( fstream& whichStream,
                                    const KTrace& whichTrace,
                                    PRV_INT32 numIter ) const
{
  TRecordTime endTraceTime = whichTrace.getEndTime();

  for ( PRV_INT32 i = 0; i < numIter; ++i )
  {
    TRecordTime baseTime = endTraceTime * i;

    for ( TCommID id = 0; id < whichTrace.getTotalComms(); ++id )
    {
      ostringstream ostr;
      ostr << fixed;
      ostr << dec;
      ostr.precision( 0 );

      ostr << CommRecord << ':';
      if ( whichTrace.existResourceInfo() )
        ostr << whichTrace.getSenderCPU( id ) + 1 << ':';
      else
        ostr << '0' << ':';
      ostr << whichTrace.getSenderThread( id ) + 1 << ':';
      ostr << whichTrace.getLogicalSend( id ) + baseTime << ':';
      ostr << whichTrace.getPhysicalSend( id ) + baseTime << ':';
      if ( whichTrace.existResourceInfo() )
        ostr << whichTrace.getReceiverCPU( id ) + 1 << ':';
      else
        ostr << '0' << ':';
      ostr << whichTrace.getReceiverThread( id ) + 1 << ':';
      ostr << whichTrace.getLogicalReceive( id ) + baseTime << ':';
      ostr << whichTrace.getPhysicalReceive( id ) + baseTime << ':';
      ostr << whichTrace.getCommSize( id ) << ':';
      ostr << whichTrace.getCommTag( id );

      whichStream << ostr.str();
      whichStream << endl;
    }
  }
}


/**********************
  Read line functions
***********************/
void TraceBodyIO_v2::readState( const string& line, 
                                const ProcessModel& whichProcessModel,
                                const ResourceModel<>& whichResourceModel,
                                MemoryBlocks& records,
                                unordered_set<TState>& states ) const
{
  string tmpstring;
  TCPUOrder CPU;
  TThreadOrder thread;
  TRecordTime time;
  TRecordTime endtime;
  TState state;

  istringstream strLine( line );

  // Discarding record type
  std::getline( strLine, tmpstring, ':' );

  // Read the common info
  if ( !readCommon( strLine, whichProcessModel, whichResourceModel, CPU, thread, time ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v2::readState()" << endl;
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  istringstream endtimeStream( tmpstring );
  if ( !( endtimeStream >> endtime ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v2::readState()" << endl;
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring );
  istringstream stateStream( tmpstring );
  if ( !( stateStream >> state ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v2::readState()" << endl;
    cerr << "Error reading state record." << endl;
    cerr << line << endl;
    return;
  }

  if ( time == endtime ) return;

  records.newRecord();
  if ( line[0] == StateBeginRecord )
    records.setType( STATE + BEGIN );
  else
    records.setType( STATE + END );
  records.setTime( time );
  records.setCPU( CPU - 1 );
  records.setThread( thread - 1 );
  records.setState( state );
  records.setStateEndTime( endtime );

  states.insert( state );
}


void TraceBodyIO_v2::readEvent( const string& line,
                                const ProcessModel& whichProcessModel,
                                const ResourceModel<>& whichResourceModel,
                                MemoryBlocks& records,
                                unordered_set<TEventType>& events ) const
{
  string tmpstring;
  TCPUOrder CPU;
  TThreadOrder thread;
  TRecordTime time;
  TEventType eventtype;
  TEventValue eventvalue;

  istringstream strLine( line );

  // Discarding record type
  std::getline( strLine, tmpstring, ':' );

  // Read the common info
  if ( !readCommon( strLine, whichProcessModel, whichResourceModel, CPU, thread, time ) )
  {
    cerr << "No logging system yet. TraceBodyIO_v2::readEvent()" << endl;
    cerr << "Error reading event record." << endl;
    cerr << line << endl;
    return;
  }

  while ( !strLine.eof() )
  {
    std::getline( strLine, tmpstring, ':' );
    istringstream eventtypeStream( tmpstring );
    if ( !( eventtypeStream >> eventtype ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v2::readEvent()" << endl;
      cerr << "Error reading event record." << endl;
      cerr << line << endl;
      return;
    }

    std::getline( strLine, tmpstring, ':' );
    istringstream eventvalueStream( tmpstring );
    if ( !( eventvalueStream >> eventvalue ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v2::readEvent()" << endl;
      cerr << "Error reading event record." << endl;
      cerr << line << endl;
      return;
    }

    records.newRecord();
    records.setType( EVENT );
    records.setTime( time );
    records.setCPU( CPU - 1 );
    records.setThread( thread - 1 );
    records.setEventType( eventtype );
    records.setEventValue( eventvalue );

    events.insert( eventtype );
  }
}


void TraceBodyIO_v2::readComm( const string& line,
                               const ProcessModel& whichProcessModel,
                               const ResourceModel<>& whichResourceModel,
                               MemoryBlocks& records ) const
{
  string tmpstring;
  TCPUOrder CPU;
  TThreadOrder thread;
  TRecordTime logSend;
  TRecordTime phySend;
  TRecordTime logReceive;
  TRecordTime phyReceive;
  TCPUOrder remoteCPU;
  TThreadOrder remoteThread;
  TCommSize size;
  TCommTag tag;
  TCommID commid;

  istringstream strLine( line );

  // Discarding record type
  std::getline( strLine, tmpstring, ':' );

  if ( line[0] == CommRecord )
  {
    // Read the common info
    if ( !readCommon( strLine, whichProcessModel, whichResourceModel, CPU, thread, logSend ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v2::readComm()" << endl;
      cerr << "Error reading communication record." << endl;
      cerr << line << endl;
      return;
    }

    std::getline( strLine, tmpstring, ':' );
    istringstream phySendStream( tmpstring );
    if ( !( phySendStream >> phySend ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v2::readComm()" << endl;
      cerr << "Error reading communication record." << endl;
      cerr << line << endl;
      return;
    }

    if ( !readCommon( strLine, whichProcessModel, whichResourceModel, remoteCPU, remoteThread, logReceive ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v2::readComm()" << endl;
      cerr << "Error reading communication record." << endl;
      cerr << line << endl;
      return;
    }

    std::getline( strLine, tmpstring, ':' );
    istringstream phyReceiveStream( tmpstring );
    if ( !( phyReceiveStream >> phyReceive ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v2::readComm()" << endl;
      cerr << "Error reading communication record." << endl;
      cerr << line << endl;
      return;
    }

    std::getline( strLine, tmpstring, ':' );
    istringstream sizeStream( tmpstring );
    if ( !( sizeStream >> size ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v2::readComm()" << endl;
      cerr << "Error reading communication record." << endl;
      cerr << line << endl;
      return;
    }

    std::getline( strLine, tmpstring, ':' );
    istringstream tagStream( tmpstring );
    if ( !( tagStream >> tag ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v2::readComm()" << endl;
      cerr << "Error reading communication record." << endl;
      cerr << line << endl;
      return;
    }

    records.newComm( false );
    records.setSenderCPU( CPU - 1 );
    records.setSenderThread( thread - 1 );
    records.setReceiverCPU( remoteCPU - 1 );
    records.setReceiverThread( remoteThread - 1 );
    records.setLogicalSend( logSend );
    records.setPhysicalSend( phySend );
    records.setLogicalReceive( logReceive );
    records.setPhysicalReceive( phyReceive );
    records.setCommSize( size );
    records.setCommTag( tag );
  }
  else
  {
    std::getline( strLine, tmpstring );
    istringstream commidStream( tmpstring );
    if ( !( commidStream >> commid ) )
    {
      cerr << "No logging system yet. TraceBodyIO_v2::readComm()" << endl;
      cerr << "Error reading communication record." << endl;
      cerr << line << endl;
      return;
    }

    records.newRecord();
    switch ( line[0] )
    {
      case LogicalSendRecord:
        records.setType( COMM + LOG + SEND );
        records.setTime( records.getLogicalSend( commid ) );
        records.setCPU( records.getSenderCPU( commid ) );
        records.setThread( records.getSenderThread( commid ) );
        break;
      case LogicalRecvRecord:
        records.setType( COMM + LOG + RECV );
        records.setTime( records.getLogicalReceive( commid ) );
        records.setCPU( records.getReceiverCPU( commid ) );
        records.setThread( records.getReceiverThread( commid ) );
        break;
      case PhysicalSendRecord:
        records.setType( COMM + PHY + SEND );
        records.setTime( records.getPhysicalSend( commid ) );
        records.setCPU( records.getSenderCPU( commid ) );
        records.setThread( records.getSenderThread( commid ) );
        break;
      case PhysicalRecvRecord:
        records.setType( COMM + PHY + RECV );
        records.setTime( records.getPhysicalReceive( commid ) );
        records.setCPU( records.getReceiverCPU( commid ) );
        records.setThread( records.getReceiverThread( commid ) );
        break;

    }
    records.setCommIndex( commid );
  }
}


void TraceBodyIO_v2::readGlobalComm( const string& line, MemoryBlocks& records ) const
{}


bool TraceBodyIO_v2::readCommon( istringstream& line,
                                 const ProcessModel& whichProcessModel,
                                 const ResourceModel<>& whichResourceModel,
                                 TCPUOrder& CPU,
                                 TThreadOrder& thread,
                                 TRecordTime& time ) const
{
  string tmpstring;

  std::getline( line, tmpstring, ':' );
  istringstream CPUStream( tmpstring );
  if ( !( CPUStream >> CPU ) )
  {
    return false;
  }

  if ( !whichResourceModel.isValidGlobalCPU( CPU ) )
    return false;

  std::getline( line, tmpstring, ':' );
  istringstream threadStream( tmpstring );
  if ( !( threadStream >> thread ) )
  {
    return false;
  }

  if ( !whichProcessModel.isValidThread( thread - 1 ) )
    return false;

  std::getline( line, tmpstring, ':' );
  istringstream timeStream( tmpstring );
  if ( !( timeStream >> time ) )
  {
    return false;
  }

  return true;
}


/**************************
  Write records functions
***************************/
bool TraceBodyIO_v2::writeState( string& line,
                                 const ProcessModel& whichProcessModel,
                                 const ResourceModel<>& whichResourceModel,
                                 MemoryTrace::iterator *record ) const
{
  ostringstream ostr;
  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  if ( record->getType() == ( STATE + BEGIN ) )
    ostr << StateBeginRecord << ':';
  else if ( record->getType() == ( STATE + END ) )
    ostr << StateEndRecord << ':';
  writeCommon( ostr, whichProcessModel, whichResourceModel, record );
  ostr << record->getStateEndTime();
  ostr << ':' << record->getState();

  line += ostr.str();
  return true;
}


bool TraceBodyIO_v2::writeEvent( string& line,
                                 const ProcessModel& whichProcessModel,
                                 const ResourceModel<>& whichResourceModel,
                                 MemoryTrace::iterator *record,
                                 bool needCommons ) const
{
  TRecordType firstType;
  TRecordTime firstTime;
  TThreadOrder firstThread;
  ostringstream ostr;
  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  if ( needCommons )
  {
    ostr << EventRecord << ':';
    writeCommon( ostr, whichProcessModel, whichResourceModel, record );
  }
  ostr << record->getEventType() << ':' << record->getEventValueAsIs();
  firstType = record->getType();
  firstTime = record->getTime();
  firstThread = record->getThread();
  ++( *record );
  while ( !record->isNull() && record->getType() == firstType &&
          record->getTime() == firstTime && record->getThread() == firstThread )
  {
    ostr << ':' << record->getEventType() << ':' << record->getEventValueAsIs();
    ++( *record );
  }
  if ( !record->isNull() )
    --( *record );

  line += ostr.str();
  return true;
}


bool TraceBodyIO_v2::writeCommRecord( string& line,
                                      MemoryTrace::iterator *record ) const
{
  ostringstream ostr;
  TRecordType type;
  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  type = record->getType();

  if ( type == ( COMM + LOG + SEND ) )
    ostr << LogicalSendRecord << ':';
  else if ( type == ( COMM + LOG + RECV ) )
    ostr << LogicalRecvRecord << ':';
  else if ( type == ( COMM + PHY + SEND ) )
    ostr << PhysicalSendRecord << ':';
  else if ( type == ( COMM + PHY + RECV ) )
    ostr << PhysicalRecvRecord << ':';
  ostr << record->getCommIndex();

  line += ostr.str();
  return true;
}


bool TraceBodyIO_v2::writeGlobalComm( string& line,
                                      const ProcessModel& whichProcessModel,
                                      MemoryTrace::iterator *record ) const
{
  return true;
}


void TraceBodyIO_v2::writeCommon( ostringstream& line,
                                  const ProcessModel& whichProcessModel,
                                  const ResourceModel<>& whichResourceModel,
                                  MemoryTrace::iterator *record ) const
{
  if ( whichResourceModel.isReady() )
    line << record->getCPU() + 1 << ':';
  else
    line << '0' << ':';

  line << record->getThread() + 1 << ':';
  line << ( record->getTime() ) << ':';
}


