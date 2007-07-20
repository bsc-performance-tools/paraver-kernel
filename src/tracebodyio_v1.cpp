#include <string>
#include <sstream>
#include <iostream>
#include "tracebodyio_v1.h"

using namespace std;

void TraceBodyIO_v1::read( fstream& file, MemoryBlocks& records )
{
  string line;

  std::getline( file, line );

  if ( line[0] == '#' || line.size() == 0 )
    return;

  switch ( line[0] )
  {
    case StateRecord:
      readState( line, records );
      break;

    case EventRecord:
      readEvent( line, records );
      break;

    case CommRecord:
      readComm( line, records );
      break;

    case GlobalCommRecord:
      //readGlobalComm( line, records );
      break;

    default:
      cerr << "Falta sistema de logging TraceBodyIO_v1::read()" << endl;
      cerr << "Tipo de record desconocido" << endl;
      break;
  };
}


void TraceBodyIO_v1::write( fstream& whichStream,
                            const Trace& whichTrace,
                            const MemoryTrace::iterator *record )
{
  string line;
  bool writeReady;

  switch ( record->getType() )
  {
    case STATE:
      writeReady = writeState( line, whichTrace, record );
      break;
    case EVENT:
      writeReady = writeEvent( line, whichTrace, record );
      break;
    case COMM:
      writeReady = writeComm( line, whichTrace, record );
      break;
    case GLOBCOMM:
      writeReady = writeGlobalComm( line, whichTrace, record );
      break;
    default:
      writeReady = false;
      cerr << "Falta sistema de logging TraceBodyIO_v1::write()" << endl;
      cerr << "Tipo de record desconocido en memoria" << endl;
      break;
  };

  if ( !writeReady )
    return;

  whichStream << line << endl;
}


void TraceBodyIO_v1::writeEvents( fstream& whichStream,
                                  const Trace& whichTrace,
                                  const vector<MemoryTrace::iterator *>& recordList )
{
  string line;

  for ( UINT16 i = 0; i < recordList.size(); i++ )
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


/**********************
  Read line functions
***********************/
void TraceBodyIO_v1::readState( const string& line, MemoryBlocks& records )
{
  string tmpstring;
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  TRecordTime time;
  TRecordTime endtime;
  TState state;

  istringstream strLine( line );

  // Discarding record type
  std::getline( strLine, tmpstring, ':' );

  // Read the common info
  if ( !readCommon( strLine, CPU, appl, task, thread, time ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readState()" << endl;
    cerr << "Error leyendo record de estado" << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  istringstream endtimeStream( tmpstring );
  if ( !( endtimeStream >> endtime ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readState()" << endl;
    cerr << "Error leyendo record de estado" << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring );
  istringstream stateStream( tmpstring );
  if ( !( stateStream >> state ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readState()" << endl;
    cerr << "Error leyendo record de estado" << endl;
    cerr << line << endl;
    return;
  }

  if ( time == endtime ) return;

  records.newRecord();
  records.setType( STATE + BEGIN );
  records.setTime( time );
  if ( CPU == 0 )
    records.setCPU( CPU );
  else
    records.setCPU( CPU - 1 );
  records.setThread( appl - 1, task - 1, thread - 1 );
  records.setState( state );
  records.setStateEndTime( endtime );

  if ( endtime != -1 )
  {
    records.newRecord();
    records.setType( STATE + END );
    records.setTime( endtime );
    if ( CPU == 0 )
      records.setCPU( CPU );
    else
      records.setCPU( CPU - 1 );
    records.setThread( appl - 1, task - 1, thread - 1 );
    records.setState( state );
    records.setStateEndTime( time );
  }
}


void TraceBodyIO_v1::readEvent( const string& line, MemoryBlocks& records )
{
  string tmpstring;
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  TRecordTime time;
  TEventType eventtype;
  TEventValue eventvalue;

  istringstream strLine( line );

  // Discarding record type
  std::getline( strLine, tmpstring, ':' );

  // Read the common info
  if ( !readCommon( strLine, CPU, appl, task, thread, time ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readEvent()" << endl;
    cerr << "Error leyendo record de evento" << endl;
    cerr << line << endl;
    return;
  }

  while ( !strLine.eof() )
  {
    std::getline( strLine, tmpstring, ':' );
    istringstream eventtypeStream( tmpstring );
    if ( !( eventtypeStream >> eventtype ) )
    {
      cerr << "Falta sistema de logging TraceBodyIO_v1::readEvent()" << endl;
      cerr << "Error leyendo record de evento" << endl;
      cerr << line << endl;
      return;
    }

    std::getline( strLine, tmpstring, ':' );
    istringstream eventvalueStream( tmpstring );
    if ( !( eventvalueStream >> eventvalue ) )
    {
      cerr << "Falta sistema de logging TraceBodyIO_v1::readEvent()" << endl;
      cerr << "Error leyendo record de evento" << endl;
      cerr << line << endl;
      return;
    }

    records.newRecord();
    records.setType( EVENT );
    records.setTime( time );
    if ( CPU == 0 )
      records.setCPU( CPU );
    else
      records.setCPU( CPU - 1 );
    records.setThread( appl - 1, task - 1, thread - 1 );
    records.setEventType( eventtype );
    records.setEventValue( eventvalue );
  }
}


void TraceBodyIO_v1::readComm( const string& line, MemoryBlocks& records )
{
  string tmpstring;
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

  istringstream strLine( line );

  // Discarding record type
  std::getline( strLine, tmpstring, ':' );

  // Read the common info
  if ( !readCommon( strLine, CPU, appl, task, thread, logSend ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error leyendo record de communicacion" << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  istringstream phySendStream( tmpstring );
  if ( !( phySendStream >> phySend ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error leyendo record de comunicacion" << endl;
    cerr << line << endl;
    return;
  }

  if ( !readCommon( strLine, remoteCPU, remoteAppl, remoteTask, remoteThread,
                    logReceive ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error leyendo record de comunicacion" << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  istringstream phyReceiveStream( tmpstring );
  if ( !( phyReceiveStream >> phyReceive ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error leyendo record de comunicacion" << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  istringstream sizeStream( tmpstring );
  if ( !( sizeStream >> size ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error leyendo record de comunicacion" << endl;
    cerr << line << endl;
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  istringstream tagStream( tmpstring );
  if ( !( tagStream >> tag ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error leyendo record de comunicacion" << endl;
    cerr << line << endl;
    return;
  }

  records.newComm();
  if ( CPU == 0 )
    records.setSenderCPU( CPU );
  else
    records.setSenderCPU( CPU - 1 );
  records.setSenderThread( appl - 1, task - 1, thread - 1 );
  if ( remoteCPU == 0 )
    records.setReceiverCPU( remoteCPU );
  else
    records.setReceiverCPU( remoteCPU - 1 );
  records.setReceiverThread( remoteAppl - 1, remoteTask - 1, remoteThread - 1 );
  records.setLogicalSend( logSend );
  records.setPhysicalSend( phySend );
  records.setLogicalReceive( logReceive );
  records.setPhysicalReceive( phyReceive );
  records.setCommSize( size );
  records.setCommTag( tag );
}


void TraceBodyIO_v1::readGlobalComm( const string& line, MemoryBlocks& records )
{}


bool TraceBodyIO_v1::readCommon( istringstream& line,
                                 TCPUOrder& CPU,
                                 TApplOrder& appl,
                                 TTaskOrder& task,
                                 TThreadOrder& thread,
                                 TRecordTime& time )
{
  string tmpstring;

  std::getline( line, tmpstring, ':' );
  istringstream CPUStream( tmpstring );
  if ( !( CPUStream >> CPU ) )
  {
    return false;
  }

  std::getline( line, tmpstring, ':' );
  istringstream applStream( tmpstring );
  if ( !( applStream >> appl ) )
  {
    return false;
  }

  std::getline( line, tmpstring, ':' );
  istringstream taskStream( tmpstring );
  if ( !( taskStream >> task ) )
  {
    return false;
  }

  std::getline( line, tmpstring, ':' );
  istringstream threadStream( tmpstring );
  if ( !( threadStream >> thread ) )
  {
    return false;
  }

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
bool TraceBodyIO_v1::writeState( string& line,
                                 const Trace& whichTrace,
                                 const MemoryTrace::iterator *record )
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
                                 const Trace& whichTrace,
                                 const MemoryTrace::iterator *record,
                                 bool needCommons )
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
                                const Trace& whichTrace,
                                const MemoryTrace::iterator *record )
{
  ostringstream ostr;
  TCommID commID;
  TApplOrder recvAppl;
  TTaskOrder recvTask;
  TThreadOrder recvThread;

  if ( !( record->getType() & LOG & SEND ) )
    return false;

  commID = record->getCommIndex();

  ostr << CommRecord << ':';
  writeCommon( ostr, whichTrace, record );
  ostr << whichTrace.getPhysicalSend( commID ) << ':';
  if ( whichTrace.existResourceInfo() )
    ostr << whichTrace.getReceiverCPU( commID ) + 1 << ':';
  else
    ostr << '0' << ':';
  whichTrace.getThreadLocation( whichTrace.getReceiverThread( commID ),
                                recvAppl, recvTask, recvThread );
  ostr << recvAppl << ':' << recvTask << ':' << recvThread << ':';
  ostr << whichTrace.getLogicalReceive( commID ) << ':';
  ostr << whichTrace.getPhysicalReceive( commID );

  line += ostr.str();
  return true;
}


bool TraceBodyIO_v1::writeGlobalComm( string& line,
                                      const Trace& whichTrace,
                                      const MemoryTrace::iterator *record )
{
  return true;
}


void TraceBodyIO_v1::writeCommon( ostringstream& line,
                                  const Trace& whichTrace,
                                  const MemoryTrace::iterator *record )
{
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;

  if ( whichTrace.existResourceInfo() )
    line << record->getCPU() + 1 << ':';
  else
    line << '0' << ':';

  whichTrace.getThreadLocation( record->getThread(), appl, task, thread );
  line << appl + 1 << ':' << task + 1 << ':' << thread + 1 << ':';
  line << record->getTime() << ':';
}


