#include <fstream>
#include <sstream>
#include "ktrace.h"
#include "traceheaderexception.h"
#include "tracebodyio_v1.h"
#include "tracestream.h"

using namespace std;

string KTrace::getFileName() const
{
  return fileName;
}

TApplOrder KTrace::totalApplications() const
{
  return traceProcessModel.totalApplications();
}


TTaskOrder KTrace::totalTasks() const
{
  return traceProcessModel.totalTasks();
}


TTaskOrder KTrace::getGlobalTask( const TApplOrder& inAppl,
                                 const TTaskOrder& inTask ) const
{
  return traceProcessModel.getGlobalTask( inAppl, inTask );
}


void KTrace::getTaskLocation( TTaskOrder globalTask,
                             TApplOrder& inAppl,
                             TTaskOrder& inTask ) const
{
  traceProcessModel.getTaskLocation( globalTask, inAppl, inTask );
}


TTaskOrder KTrace::getFirstTask( TApplOrder inAppl ) const
{
  return traceProcessModel.getFirstTask( inAppl );
}


TTaskOrder KTrace::getLastTask( TApplOrder inAppl ) const
{
  return traceProcessModel.getLastTask( inAppl );
}


TThreadOrder KTrace::totalThreads() const
{
  return traceProcessModel.totalThreads();
}


TThreadOrder KTrace::getGlobalThread( const TApplOrder& inAppl,
                                     const TTaskOrder& inTask,
                                     const TThreadOrder& inThread ) const
{
  return traceProcessModel.getGlobalThread( inAppl, inTask, inThread );
}


void KTrace::getThreadLocation( TThreadOrder globalThread,
                               TApplOrder& inAppl,
                               TTaskOrder& inTask,
                               TThreadOrder& inThread ) const
{
  traceProcessModel.getThreadLocation( globalThread, inAppl, inTask, inThread );
}


TThreadOrder KTrace::getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const
{
  return traceProcessModel.getFirstThread( inAppl, inTask );
}


TThreadOrder KTrace::getLastThread( TApplOrder inAppl, TTaskOrder inTask )const
{
  return traceProcessModel.getLastThread( inAppl, inTask );
}


bool KTrace::existResourceInfo() const
{
  return traceResourceModel.isReady();
}


TNodeOrder KTrace::totalNodes() const
{
  return traceResourceModel.totalNodes();
}


TCPUOrder KTrace::totalCPUs() const
{
  return traceResourceModel.totalCPUs();
}


TCPUOrder KTrace::getGlobalCPU( const TNodeOrder& inNode,
                               const TCPUOrder& inCPU ) const
{
  return traceResourceModel.getGlobalCPU( inNode, inCPU );
}


void KTrace::getCPULocation( TCPUOrder globalCPU,
                            TNodeOrder& inNode,
                            TCPUOrder& inCPU ) const
{
  traceResourceModel.getCPULocation( globalCPU, inNode, inCPU );
}


TCPUOrder KTrace::getFirstCPU( TNodeOrder inNode ) const
{
  return traceResourceModel.getFirstCPU( inNode );
}


TCPUOrder KTrace::getLastCPU( TNodeOrder inNode ) const
{
  return getLastCPU( inNode );
}


// PRECOND: fromLevel > toLevel
TObjectOrder KTrace::getFirst( TObjectOrder globalOrder,
                              TWindowLevel fromLevel,
                              TWindowLevel toLevel  ) const
{
  if ( fromLevel == WORKLOAD || fromLevel == SYSTEM )
  {
    return 0;
  }
  else if ( fromLevel == APPLICATION )
  {
    if ( toLevel == TASK )
      return getFirstTask( globalOrder );
    else
      return getFirstThread( globalOrder, 0 );
  }
  else if ( fromLevel == TASK )
  {
    TApplOrder application;
    TTaskOrder task;
    getTaskLocation( globalOrder, application, task );
    return getFirstThread( application, task );
  }
  else if ( fromLevel == NODE )
  {
    return getFirstCPU( globalOrder );
  }

  return 0;
}


TObjectOrder KTrace::getLast( TObjectOrder globalOrder,
                             TWindowLevel fromLevel,
                             TWindowLevel toLevel ) const
{
  if ( fromLevel == WORKLOAD )
  {
    if ( toLevel == APPLICATION )
      return totalApplications() - 1;
    else if ( toLevel == TASK )
      return totalTasks() - 1;
    else
      return totalThreads() - 1;
  }
  else if ( fromLevel == APPLICATION )
  {
    if ( toLevel == TASK )
      return getLastTask( globalOrder );
    else
      return getLastThread( globalOrder, getLastTask( globalOrder ) );
  }
  else if ( fromLevel == TASK )
  {
    TApplOrder application;
    TTaskOrder task;
    getTaskLocation( globalOrder, application, task );
    return getLastThread( application, task );
  }
  else if ( fromLevel == SYSTEM )
  {
    if ( toLevel == NODE )
      return totalNodes() - 1;
    else
      return totalCPUs() - 1;
  }
  else if ( fromLevel == NODE )
  {
    return getLastCPU( globalOrder );
  }

  return 0;
}



TThreadOrder KTrace::getSenderThread( TCommID whichComm ) const
{
  return blocks->getSenderThread( whichComm );
}

TCPUOrder KTrace::getSenderCPU( TCommID whichComm ) const
{
  return blocks->getSenderCPU( whichComm );
}

TThreadOrder KTrace::getReceiverThread( TCommID whichComm ) const
{
  return blocks->getReceiverThread( whichComm );
}

TCPUOrder KTrace::getReceiverCPU( TCommID whichComm ) const
{
  return blocks->getReceiverCPU( whichComm );
}

TCommTag KTrace::getCommTag( TCommID whichComm ) const
{
  return blocks->getCommTag( whichComm );
}

TCommSize KTrace::getCommSize( TCommID whichComm ) const
{
  return blocks->getCommSize( whichComm );
}

TRecordTime KTrace::getLogicalSend( TCommID whichComm ) const
{
  return blocks->getLogicalSend( whichComm );
}

TRecordTime KTrace::getLogicalReceive( TCommID whichComm ) const
{
  return blocks->getLogicalReceive( whichComm );
}

TRecordTime KTrace::getPhysicalSend( TCommID whichComm ) const
{
  return blocks->getPhysicalSend( whichComm );
}

TRecordTime KTrace::getPhysicalReceive( TCommID whichComm ) const
{
  return blocks->getPhysicalReceive( whichComm );
}


void KTrace::dumpFile( const string& whichFile ) const
{
  std::fstream file( whichFile.c_str(), fstream::out | fstream::trunc );
  // pendiente volcar cabecera

  // volcado cuerpo
  MemoryTrace::iterator *it = btree->begin();
  while ( !it->isNull() )
  {
    TraceBodyIO_v1::write( file, *this, it );
    ++( *it );
  }

  file.close();
  delete it;
}


// Forward MemoryTrace iterator functions
MemoryTrace::iterator* KTrace::begin() const
{
  return btree->begin();
}

MemoryTrace::iterator* KTrace::end() const
{
  return btree->end();
}

MemoryTrace::iterator* KTrace::threadBegin( TThreadOrder whichThread ) const
{
  return btree->threadBegin( whichThread );
}

MemoryTrace::iterator* KTrace::threadEnd( TThreadOrder whichThread ) const
{
  return btree->threadEnd( whichThread );
}

MemoryTrace::iterator* KTrace::CPUBegin( TCPUOrder whichCPU ) const
{
  return btree->CPUBegin( whichCPU );
}

MemoryTrace::iterator* KTrace::CPUEnd( TCPUOrder whichCPU ) const
{
  return btree->CPUEnd( whichCPU );
}

void KTrace::getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                   TRecordTime whichTime ) const
{
  btree->getRecordByTimeThread( listIter, whichTime );
}

void KTrace::getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                TRecordTime whichTime ) const
{
  btree->getRecordByTimeCPU( listIter, whichTime );
}


KTrace::KTrace( const string& whichFile ) : fileName( whichFile )
{
  string tmpstr;

  ready = false;
  TraceStream *file = TraceStream::openFile( fileName );

// Reading the header
  file->getline( tmpstr );
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
  UINT32 numberComm = 0;
  if ( !header.eof() )
  {
    std::getline( header, tmpstr );
    istringstream streamComm( tmpstr );

    if ( !( streamComm >> numberComm ) )
    {
      throw TraceHeaderException( TraceHeaderException::invalidCommNumber,
                                  tmpstr.c_str() );
    }
  }

  for ( UINT32 count = 0; count < numberComm; count++ )
  {
    file->getline( tmpstr );
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

  btree  = new BPlusTree( traceProcessModel.totalThreads(),
                          traceResourceModel.totalCPUs() );

  while ( !file->eof() )
  {
    TraceBodyIO_v1::read( file, *blocks );
    btree->insert( blocks );
  }

// End reading the body
  traceEndTime = btree->finish( traceEndTime );
  file->close();
  ready = true;

}
