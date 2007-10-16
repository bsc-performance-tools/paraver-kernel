#include <fstream>
#include <sstream>
#include "trace.h"
#include "traceheaderexception.h"
#include "tracebodyio_v1.h"
#include "tracestream.h"

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


void Trace::getTaskLocation( TTaskOrder globalTask,
                             TApplOrder& inAppl,
                             TTaskOrder& inTask ) const
{
  traceProcessModel.getTaskLocation( globalTask, inAppl, inTask );
}


TTaskOrder Trace::getFirstTask( TApplOrder inAppl ) const
{
  return traceProcessModel.getFirstTask( inAppl );
}


TTaskOrder Trace::getLastTask( TApplOrder inAppl ) const
{
  return traceProcessModel.getLastTask( inAppl );
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


void Trace::getThreadLocation( TThreadOrder globalThread,
                               TApplOrder& inAppl,
                               TTaskOrder& inTask,
                               TThreadOrder& inThread ) const
{
  traceProcessModel.getThreadLocation( globalThread, inAppl, inTask, inThread );
}


TThreadOrder Trace::getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const
{
  return traceProcessModel.getFirstThread( inAppl, inTask );
}


TThreadOrder Trace::getLastThread( TApplOrder inAppl, TTaskOrder inTask )const
{
  return traceProcessModel.getLastThread( inAppl, inTask );
}


bool Trace::existResourceInfo() const
{
  return traceResourceModel.isReady();
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


void Trace::getCPULocation( TCPUOrder globalCPU,
                         TNodeOrder& inNode,
                         TCPUOrder& inCPU ) const
{
  traceResourceModel.getCPULocation( globalCPU, inNode, inCPU );
}


TCPUOrder Trace::getFirstCPU( TNodeOrder inNode ) const
{
  return traceResourceModel.getFirstCPU( inNode );
}


TCPUOrder Trace::getLastCPU( TNodeOrder inNode ) const
{
  return getLastCPU( inNode );
}


TThreadOrder Trace::getSenderThread( TCommID whichComm ) const
{
  return blocks->getSenderThread( whichComm );
}

TCPUOrder Trace::getSenderCPU( TCommID whichComm ) const
{
  return blocks->getSenderCPU( whichComm );
}

TThreadOrder Trace::getReceiverThread( TCommID whichComm ) const
{
  return blocks->getReceiverThread( whichComm );
}

TCPUOrder Trace::getReceiverCPU( TCommID whichComm ) const
{
  return blocks->getReceiverCPU( whichComm );
}

TCommTag Trace::getCommTag( TCommID whichComm ) const
{
  return blocks->getCommTag( whichComm );
}

TCommSize Trace::getCommSize( TCommID whichComm ) const
{
  return blocks->getCommSize( whichComm );
}

TRecordTime Trace::getLogicalSend( TCommID whichComm ) const
{
  return blocks->getLogicalSend( whichComm );
}

TRecordTime Trace::getLogicalReceive( TCommID whichComm ) const
{
  return blocks->getLogicalReceive( whichComm );
}

TRecordTime Trace::getPhysicalSend( TCommID whichComm ) const
{
  return blocks->getPhysicalSend( whichComm );
}

TRecordTime Trace::getPhysicalReceive( TCommID whichComm ) const
{
  return blocks->getPhysicalReceive( whichComm );
}


void Trace::dumpFile( const string& whichFile ) const
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
MemoryTrace::iterator* Trace::begin() const
{
  return btree->begin();
}

MemoryTrace::iterator* Trace::end() const
{
  return btree->end();
}

MemoryTrace::iterator* Trace::threadBegin( TThreadOrder whichThread ) const
{
  return btree->threadBegin( whichThread );
}

MemoryTrace::iterator* Trace::threadEnd( TThreadOrder whichThread ) const
{
  return btree->threadEnd( whichThread );
}

MemoryTrace::iterator* Trace::CPUBegin( TCPUOrder whichCPU ) const
{
  return btree->CPUBegin( whichCPU );
}

MemoryTrace::iterator* Trace::CPUEnd( TCPUOrder whichCPU ) const
{
  return btree->CPUEnd( whichCPU );
}

void Trace::getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                   TRecordTime whichTime ) const
{
  btree->getRecordByTimeThread( listIter, whichTime );
}

void Trace::getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                TRecordTime whichTime ) const
{
  btree->getRecordByTimeCPU( listIter, whichTime );
}


Trace::Trace( const string& whichFile ) : fileName( whichFile )
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
  if( !header.eof() )
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
