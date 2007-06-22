#include <string>
#include <sstream>
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
