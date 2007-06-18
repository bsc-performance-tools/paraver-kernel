#include <fstream>
#include <sstream>
#include "trace.h"
#include "traceheaderexception.h"
#include "tracebodyio_v1.h"

using namespace std;

TApplOrder Trace::totalApplications() const
{
  return traceProcessModel.totalApplications();
}


TTaskOrder Trace::totalTasks() const
{
  return traceProcessModel.totalTasks();
}


TTaskOrder Trace::getGlobalTask( const TApplOrder& inAppl,
                                 const TTaskOrder& inTask ) const
{
  return traceProcessModel.getGlobalTask( inAppl, inTask );
}

TThreadOrder Trace::totalThreads() const
{
  return traceProcessModel.totalThreads();
}


TThreadOrder Trace::getGlobalThread( const TApplOrder& inAppl,
                                     const TTaskOrder& inTask,
                                     const TThreadOrder& inThread ) const
{
  return traceProcessModel.getGlobalThread( inAppl, inTask, inThread );
}


TNodeOrder Trace::totalNodes() const
{
  return traceResourceModel.totalNodes();
}


TCPUOrder Trace::totalCPUs() const
{
  return traceResourceModel.totalCPUs();
}


TCPUOrder Trace::getGlobalCPU( const TNodeOrder& inNode,
                               const TCPUOrder& inCPU ) const
{
  return traceResourceModel.getGlobalCPU( inNode, inCPU );
}


TTime Trace::getEndTime() const
{
  return traceEndTime;
}


TTimeUnit Trace::getTimeUnit() const
{
  return traceTimeUnit;
}


Trace::Trace( const string& whichFile ) : fileName( whichFile )
{
  string tmpstr;

  ready = false;
  std::fstream file( fileName.c_str(), fstream::in );

// Reading the header
  std::getline( file, tmpstr );
  istringstream header( tmpstr );

  std::getline( header, date, ')' );
  date += ')';
  header.get();

  std::getline( header, tmpstr, ':' );
  size_t pos = tmpstr.find( '_' );
  if ( pos == string::npos )
  {
    // No '_' char found. The trace is in us.
    traceTimeUnit = US;
    istringstream stringEndTime( tmpstr );
    if ( !( stringEndTime >> traceEndTime ) )
    {
      throw TraceHeaderException( TraceHeaderException::invalidTime,
                                  tmpstr.c_str() );
    }
  }
  else
  {
    // '_' char found. The trace is in ns.
    traceTimeUnit = NS;
    istringstream stringEndTime( tmpstr.substr( 0, pos ) );
    if ( !( stringEndTime >> traceEndTime ) )
    {
      throw TraceHeaderException( TraceHeaderException::invalidTime,
                                  tmpstr.c_str() );
    }
  }

  traceResourceModel = ResourceModel( header );
  traceProcessModel = ProcessModel( header );

  // Communicators
  UINT32 numberComm;
  std::getline( header, tmpstr );
  istringstream streamComm( tmpstr );

  if ( !( streamComm >> numberComm ) )
  {
    throw TraceHeaderException( TraceHeaderException::invalidCommNumber,
                                tmpstr.c_str() );
  }

  for ( UINT32 count = 0; count < numberComm; count++ )
  {
    std::getline( file, tmpstr );
    if ( tmpstr[0] != 'C' && tmpstr[0] != 'c' )
    {
      throw TraceHeaderException( TraceHeaderException::unknownCommLine,
                                  tmpstr.c_str() );
    }
    communicators.push_back( tmpstr );
  }
  // End communicators

// End reading the header

// Reading the body
  blocks = new BPlusTreeBlocks( traceProcessModel );
  while( !file.eof() )
  {
    TraceBodyIO_v1::read( file, *blocks );
  }
// End reading the body
  file.close();
  ready = true;
}
