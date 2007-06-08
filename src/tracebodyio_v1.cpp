#include <string>
#include <sstream>
#include "tracebodyio_v1.h"
#include "tracebodyexception.h"

using namespace std;

void TraceBodyIO_v1::read( fstream& file, MemoryBlocks& records )
{
  string line;

  std::getline( file, line );

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
  istringstream tmpstream;
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
    return;
  }

  std::getline( strLine, tmpstring, ':' );
  tmpstream.str( tmpstring );
  if ( !( tmpstream >> endtime ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readState()" << endl;
    cerr << "Error leyendo record de estado" << endl;
    return;
  }

  std::getline( strLine, tmpstring );
  tmpstream.str( tmpstring );
  if ( !( tmpstream >> state ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readState()" << endl;
    cerr << "Error leyendo record de estado" << endl;
    return;
  }

  if( time == endtime ) return;

  records.newRecord();
  records.setType( STATE + BEGIN );
  records.setTime( time );
  records.setCPU( CPU );
  records.setThread( appl, task, thread );
  records.setState( state );
  records.setStateEndTime( endtime );

  if( endtime != -1 )
  {
    records.newRecord();
    records.setType( STATE + END );
    records.setTime( endtime );
    records.setCPU( CPU );
    records.setThread( appl, task, thread );
    records.setState( state );
    records.setStateEndTime( time );
  }
}


void TraceBodyIO_v1::readEvent( const string& line, MemoryBlocks& records )
{
  string tmpstring;
  istringstream tmpstream;
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
    return;
  }

  while( !strLine.eof() )
  {
    std::getline( strLine, tmpstring, ':' );
    tmpstream.str( tmpstring );
    if ( !( tmpstream >> eventtype ) )
    {
      cerr << "Falta sistema de logging TraceBodyIO_v1::readState()" << endl;
      cerr << "Error leyendo record de estado" << endl;
      return;
    }

    std::getline( strLine, tmpstring, ':' );
    tmpstream.str( tmpstring );
    if ( !( tmpstream >> eventvalue ) )
    {
      cerr << "Falta sistema de logging TraceBodyIO_v1::readState()" << endl;
      cerr << "Error leyendo record de estado" << endl;
      return;
    }

    records.newRecord();
    records.setType( EVENT );
    records.setTime( time );
    records.setCPU( CPU );
    records.setThread( appl, task, thread );
    records.setEventType( eventtype );
    records.setEventValue( eventvalue );
  }
}


void TraceBodyIO_v1::readComm( const string& line, MemoryBlocks& records )
{
  string tmpstr;
  TCPUOrder CPU;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  TRecordTime time;
  TRecordTime logSend;
  TRecordTime phySend;
  TRecordTime logReceive;
  TRecordTime phyReceive;
  TCPUOrder remoteCPU;
  TApplOrder remoteAppl;
  TTaskOrder remoteTask;
  TThreadOrder remoteThread;

  istringstream strLine( line );

  // Discarding record type
  std::getline( strLine, tmpstr, ':' );

  // Read the common info
  if ( !readCommon( strLine, CPU, appl, task, thread, time ) )
  {
    cerr << "Falta sistema de logging TraceBodyIO_v1::readComm()" << endl;
    cerr << "Error leyendo record de communicacion" << endl;
    return;
  }
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
  istringstream tmpstream;

  std::getline( line, tmpstring, ':' );
  tmpstream.str( tmpstring );
  if ( !( tmpstream >> CPU ) )
  {
    return false;
  }

  std::getline( line, tmpstring, ':' );
  tmpstream.str( tmpstring );
  if ( !( tmpstream >> appl ) )
  {
    return false;
  }

  std::getline( line, tmpstring, ':' );
  tmpstream.str( tmpstring );
  if ( !( tmpstream >> task ) )
  {
    return false;
  }

  std::getline( line, tmpstring, ':' );
  tmpstream.str( tmpstring );
  if ( !( tmpstream >> thread ) )
  {
    return false;
  }

  std::getline( line, tmpstring, ':' );
  tmpstream.str( tmpstring );
  if ( !( tmpstream >> time ) )
  {
    return false;
  }

  return true;
}
