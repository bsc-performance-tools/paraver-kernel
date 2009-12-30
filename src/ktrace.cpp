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

#include <fstream>
#include <sstream>
#ifdef WIN32
#include <hash_set>
#else
#include <ext/hash_set>
#endif
#include "ktrace.h"
#include "traceheaderexception.h"
#include "tracebodyio.h"
#include "tracestream.h"
#include "kprogresscontroller.h"
#include "bplustree.h"
#include "bplustreeblocks.h"
#include "plaintrace.h"
#include "plainblocks.h"
#include "noloadtrace.h"
#include "noloadblocks.h"

using namespace std;
#ifdef WIN32
using namespace stdext;
#else
using namespace __gnu_cxx;
#endif

using namespace bplustree;
using namespace Plain;
using namespace NoLoad;

string KTrace::getFileName() const
{
  return fileName;
}

string KTrace::getTraceName() const
{
  string traceName = fileName;

#ifdef WIN32
  string::size_type pos = traceName.find_last_of( '\\' );
#else
  string::size_type pos = traceName.find_last_of( '/' );
#endif
  if ( pos != string::npos )
    traceName = traceName.substr( pos + 1, traceName.size() );

  return traceName;
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
  return traceResourceModel.getLastCPU( inNode );
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



TCommID KTrace::getTotalComms() const
{
  return blocks->getTotalComms();
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


void KTrace::dumpFile( const string& whichFile, INT32 numIter ) const
{
  ostringstream ostr;

  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  std::fstream file( whichFile.c_str(), fstream::out | fstream::trunc );
  file << fixed;
  file << dec;
  file.precision( 0 );

  file << "new format" << endl;
  file << date << ':';
  ostr << traceEndTime * numIter;
  file << ostr.str();
  if ( traceTimeUnit != US )
    file << "_ns";
  file << ':';
  traceResourceModel.dumpToFile( file );
  file << ':';
  traceProcessModel.dumpToFile( file );
  if ( communicators.begin() != communicators.end() )
  {
    file << ',' << communicators.size() << endl;
    for ( vector<string>::const_iterator it = communicators.begin();
          it != communicators.end(); ++it )
      file << ( *it ) << endl;
  }
  else
    file << endl;

#ifdef BYTHREAD
  TraceBodyIO *body = TraceBodyIO::createTraceBody();
  body->writeCommInfo( file, *this, numIter );

  for ( TThreadOrder iThread = 0; iThread < totalThreads(); ++iThread )
  {
    for ( INT32 iter = 0; iter < numIter; ++iter )
    {
      MemoryTrace::iterator *it = memTrace->threadBegin( iThread );

      while ( !it->isNull() )
      {
        body->write( file, *this, it, iter );
        ++( *it );
      }

      delete it;
    }
  }
#else
  MemoryTrace::iterator *it = memTrace->begin();
  TraceBodyIO *body = TraceBodyIO::createTraceBody();
  body->writeCommInfo( file, *this );

  while ( !it->isNull() )
  {
    body->write( file, *this, it );
    ++( *it );
  }

  delete it;
#endif
  file.close();
}


// Forward MemoryTrace iterator functions
MemoryTrace::iterator* KTrace::begin() const
{
  return memTrace->begin();
}

MemoryTrace::iterator* KTrace::end() const
{
  return memTrace->end();
}

MemoryTrace::iterator* KTrace::threadBegin( TThreadOrder whichThread ) const
{
  return memTrace->threadBegin( whichThread );
}

MemoryTrace::iterator* KTrace::threadEnd( TThreadOrder whichThread ) const
{
  return memTrace->threadEnd( whichThread );
}

MemoryTrace::iterator* KTrace::CPUBegin( TCPUOrder whichCPU ) const
{
  return memTrace->CPUBegin( whichCPU );
}

MemoryTrace::iterator* KTrace::CPUEnd( TCPUOrder whichCPU ) const
{
  return memTrace->CPUEnd( whichCPU );
}

void KTrace::getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                    TRecordTime whichTime ) const
{
  memTrace->getRecordByTimeThread( listIter, whichTime );
}

void KTrace::getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                 TRecordTime whichTime ) const
{
  memTrace->getRecordByTimeCPU( listIter, whichTime );
}


KTrace::KTrace( const string& whichFile, ProgressController *progress, bool noLoad )
    : fileName( whichFile )
{
  string tmpstr;

  ready = false;
  TraceStream *file = TraceStream::openFile( fileName );
  if ( !file->good() )
    throw ParaverKernelException( ParaverKernelException::cannotOpenTrace,
                                  fileName.c_str() );

  if ( file->canseekend() )
  {
    file->seekend();
    if ( progress != NULL )
      progress->setEndLimit( file->tellg() );
    file->seekbegin();
  }
  body = TraceBodyIO::createTraceBody( file );

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

  if ( !file->canseekend() )
  {
    if ( progress != NULL )
      progress->setEndLimit( traceEndTime );
  }

  traceResourceModel = ResourceModel( header );
  traceProcessModel = ProcessModel( header );

  // Communicators
  UINT32 numberComm = 0;
  if ( !header.eof() )
  {
    std::getline( header, tmpstr );
    if ( tmpstr != "" )
    {
      istringstream streamComm( tmpstr );

      if ( !( streamComm >> numberComm ) )
      {
        throw TraceHeaderException( TraceHeaderException::invalidCommNumber,
                                    tmpstr.c_str() );
      }
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

  if ( noLoad && body->ordered() )
  {
    blocks = new NoLoadBlocks( traceResourceModel, traceProcessModel, body, file, traceEndTime );
    memTrace = new NoLoadTrace( blocks, traceProcessModel, traceResourceModel );
    ( ( NoLoadBlocks * )blocks )->setFirstOffset( file->tellg() );
  }
  else if ( body->ordered() )
  {
    blocks = new PlainBlocks( traceResourceModel, traceProcessModel );
    memTrace  = new PlainTrace( traceProcessModel,
                                traceResourceModel );
  }
  else
  {
    blocks = new BPlusTreeBlocks( traceProcessModel );
    memTrace  = new BPlusTree( traceProcessModel.totalThreads(),
                               traceResourceModel.totalCPUs() );
  }

  hash_set<TEventType> hashevents;
  int count = 0;
  while ( !file->eof() )
  {
    body->read( file, *blocks, hashevents );
    if ( count == 5000 )
    {
      memTrace->insert( blocks );
      if ( progress != NULL )
      {
        if ( file->canseekend() )
          progress->setCurrentProgress( file->tellg() );
        else
          progress->setCurrentProgress( blocks->getLastRecordTime() );
      }
      count = 0;
      if ( progress != NULL && progress->getStop() )
        break;
    }
    else
      ++count;
  }
  if ( blocks->getCountInserted() > 0 )
    memTrace->insert( blocks );

  for ( hash_set<TEventType>::iterator it = hashevents.begin(); it != hashevents.end(); ++it )
    events.insert( *it );

// End reading the body
  traceEndTime = memTrace->finish( traceEndTime );
  if ( !( noLoad && body->ordered() ) )
  {
    file->close();
    delete body;
    body = NULL;
  }
  else
  {
    ( ( NoLoadBlocks * ) blocks )->setFileLoaded();
    file->clear();
  }

  ready = true;
}

KTrace::~KTrace()
{
  delete blocks;
  delete memTrace;
  if ( body != NULL )
    delete body;
}

const set<TEventType>& KTrace::getLoadedEvents() const
{
  return events;
}

bool KTrace::eventLoaded( TEventType whichType ) const
{
  return events.find( whichType ) != events.end();
}

bool KTrace::findLastEventValue( TThreadOrder whichThread,
                                 TRecordTime whichTime,
                                 const vector<TEventType>& whichEvent,
                                 TEventType& returnType,
                                 TEventValue& returnValue ) const
{
  bool result = false;
  vector<MemoryTrace::iterator *> listIter;
  MemoryTrace::iterator *it;

  for ( vector<TEventType>::const_iterator itEvt = whichEvent.begin();
        itEvt != whichEvent.end(); ++itEvt )
  {
    if ( !eventLoaded( *itEvt ) )
      return false;
  }

  listIter.insert( listIter.begin(), totalThreads(), NULL );
  getRecordByTimeThread( listIter, whichTime );
  it = listIter[ whichThread ];

  while ( it->getTime() > whichTime )
    --( *it );

  while ( !it->isNull() && !result )
  {
    for ( vector<TEventType>::const_iterator itEvt = whichEvent.begin();
          itEvt != whichEvent.end(); ++itEvt )
    {
      if ( ( it->getType() & EVENT ) && ( it->getEventType() == *itEvt )
           && ( it->getEventValue() != 0 ) )
      {
        returnType = it->getEventType();
        returnValue = it->getEventValue();
        result = true;
        break;
      }
    }
    --( *it );
  }

  for ( vector<MemoryTrace::iterator *>::iterator itErase = listIter.begin();
        itErase != listIter.end(); ++itErase )
    delete *itErase;

  return result;
}

bool KTrace::getFillStateGaps() const
{
  return fillStateGaps;
}

void KTrace::setFillStateGaps( bool fill )
{
  fillStateGaps = fill;
}
