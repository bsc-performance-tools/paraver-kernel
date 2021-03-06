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

#include <fstream>
#include <sstream>
#ifdef _MSC_VER
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
#include "traceeditblocks.h"

using namespace std;
#ifdef _MSC_VER
using namespace stdext;
#else
using namespace __gnu_cxx;
#endif
using boost::posix_time::time_input_facet;

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


TThreadOrder KTrace::getLastThread( TApplOrder inAppl, TTaskOrder inTask ) const
{
  return traceProcessModel.getLastThread( inAppl, inTask );
}

void KTrace::getThreadsPerNode( TNodeOrder inNode, std::vector<TThreadOrder>& onVector ) const
{
  traceProcessModel.getThreadsPerNode( inNode, onVector );
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

bool KTrace::isSameObjectStruct( Trace *compareTo, bool compareProcessModel ) const
{
  KTrace *tmpTrace = (KTrace *)compareTo;
  if ( compareProcessModel )
    return traceProcessModel == tmpTrace->traceProcessModel;
  
  return traceResourceModel == tmpTrace->traceResourceModel;
}

bool KTrace::isSubsetObjectStruct( Trace *compareTo, bool compareProcessModel ) const
{
  KTrace *tmpTrace = (KTrace *)compareTo;
  if( compareProcessModel )
    return traceProcessModel < tmpTrace->traceProcessModel;

  return traceResourceModel < tmpTrace->traceResourceModel;
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


TRecordTime KTrace::customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const
{
  TRecordTime tmpTime;
  TRecordTime factor = 1;

  if( getTimeUnit() == whichUnits )
    tmpTime = whichTime;
  else
  {
    PRV_UINT16 from = getTimeUnit() > whichUnits ? whichUnits : getTimeUnit();
    PRV_UINT16 to = getTimeUnit() > whichUnits ? getTimeUnit() : whichUnits;

    for( PRV_UINT16 i = from + 1; i <= to; i++ )
      factor *= factorTable[ i ];

    if( getTimeUnit() > whichUnits )
      tmpTime = whichTime / factor;
    else
      tmpTime = whichTime * factor;
  }

  return tmpTime;
}


TRecordTime KTrace::traceUnitsToCustomUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const
{
  TRecordTime tmpTime;
  TRecordTime factor = 1;

  if( getTimeUnit() == whichUnits )
    tmpTime = whichTime;
  else
  {
    PRV_UINT16 from = getTimeUnit() > whichUnits ? whichUnits : getTimeUnit();
    PRV_UINT16 to = getTimeUnit() > whichUnits ? getTimeUnit() : whichUnits;

    for( PRV_UINT16 i = from + 1; i <= to; i++ )
      factor *= factorTable[ i ];

    if( getTimeUnit() > whichUnits )
      tmpTime = whichTime * factor;
    else
      tmpTime = whichTime / factor;
  }

  return tmpTime;
}


void KTrace::dumpFileHeader( std::fstream& file, bool newFormat , PRV_INT32 numIter) const
{
  ostringstream ostr;

  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  file << fixed;
  file << dec;
  file.precision( 0 );

  if( newFormat )
    file << "new format" << endl;

  file << "#Paraver (";
  file << myTraceTime << "):";
  ostr << traceEndTime * numIter;
  file << ostr.str();
  if ( traceTimeUnit != US )
    file << "_ns";
  file << ':';
  traceResourceModel.dumpToFile( file );
  file << ':';
  traceProcessModel.dumpToFile( file, existResourceInfo() );
  if ( communicators.begin() != communicators.end() )
  {
    file << ',' << communicators.size() << endl;
    for ( vector<string>::const_iterator it = communicators.begin();
          it != communicators.end(); ++it )
      file << ( *it ) << endl;
  }
  else
    file << endl;
}


void KTrace::dumpFile( const string& whichFile, PRV_INT32 numIter ) const
{
  ostringstream ostr;

  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  std::fstream file( whichFile.c_str(), fstream::out | fstream::trunc );
  dumpFileHeader( file, true, numIter );

#ifdef BYTHREAD
  TraceBodyIO *body = TraceBodyIO::createTraceBody();
  body->writeCommInfo( file, *this, numIter );

  for ( TThreadOrder iThread = 0; iThread < totalThreads(); ++iThread )
  {
    for ( PRV_INT32 iter = 0; iter < numIter; ++iter )
    {
      MemoryTrace::iterator *it = memTrace->threadBegin( iThread );

      while ( !it->isNull() )
      {
        body->write( file, *this, it, iter );
        if ( it->getTime() == traceEndTime ) break;
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
MemoryTrace::iterator* KTrace::empty() const
{
  return memTrace->empty();
}

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


void KTrace::parseDateTime( string &whichDateTime )
{
  // Create locales for every time format that whichDateTime may take
  vector<string> strFormatDate;
  strFormatDate.push_back("%d/%m/%Y at %H:%M:%S%F");
  strFormatDate.push_back("%d/%m/%Y at %H:%M:%S");
  strFormatDate.push_back("%d/%m/%Y at %H:%M");
  strFormatDate.push_back("%d/%m/%Y");
  /*
  // ptime will ignore these ones. ¿?
  strFormatDate.push_back("%d/%m/%y at %H:%M:%S%F");
  strFormatDate.push_back("%d/%m/%y at %H:%M:%S");
  strFormatDate.push_back("%d/%m/%y at %H:%M");
  strFormatDate.push_back("%d/%m/%y");
  */

  vector<std::locale> formatDate;
  for( vector<string>::const_iterator it = strFormatDate.begin(); it != strFormatDate.end(); ++it )
  {
    formatDate.push_back(std::locale( std::locale::classic(), new time_input_facet(*it)));
  }

  // Guarantee year field in whichDateTime has 4 digits (years with 2 digits not covered by ptime)
  string tmpDate;
  string tmpYear = whichDateTime.substr( 0, whichDateTime.find_first_of(' ') );
  tmpYear = tmpYear.substr( tmpYear.find_last_of('/') + 1, tmpYear.find_first_of(' '));
  if ( tmpYear.length() == 4 )
  {
    tmpDate = whichDateTime;
  }
  else
  {
    int newYear;
    std::istringstream ss( tmpYear );
    ss >> newYear;

    if ( newYear < 80 )
      newYear += 2000;
    else
      newYear += 1900;

    string tmpTail = whichDateTime.substr( whichDateTime.find_first_of(' ') ); // " at hh:mm:ss" or empty

    // c++ 11
    // tmpDate = whichDateTime.substr( 0, whichDateTime.find_last_of( '/' ) ) + "/" + std::to_string( newYear ) + tmpTail;
    stringstream tmpSSYear;
    tmpSSYear << newYear;
    tmpDate = whichDateTime.substr( 0, whichDateTime.find_last_of( '/' ) ) + "/" + tmpSSYear.str() + tmpTail;
  }

  // Now, try to match one ptime from {date * format}
  std::stringstream tmpSSDate( tmpDate );
  for( vector<std::locale>::const_iterator it = formatDate.begin(); it != formatDate.end(); ++it )
  {
    tmpSSDate.imbue( *it );
    tmpSSDate >> myTraceTime;

    if ( !myTraceTime.is_not_a_date_time() )
      break;
  }
}


KTrace::KTrace( const string& whichFile, ProgressController *progress, bool noLoad )
    : fileName( whichFile )
{
  string tmpstr;

  ready = false;
  TraceStream *file = TraceStream::openFile( fileName );

  if ( !file->good() )
  {
    delete file;
    throw ParaverKernelException( ParaverKernelException::cannotOpenTrace, fileName.c_str() );
  }

  if ( file->canseekend() )
  {
    file->seekend();
    if ( progress != NULL )
      progress->setEndLimit( file->tellg() );
    file->seekbegin();
  }
  body = TraceBodyIO::createTraceBody( file, this );

// Reading the header
  string fileType = whichFile.substr( whichFile.find_last_of( '.' ) + 1 );
  if ( fileType == "csv" )
  {
    // Saved CSV traces are in nanoseconds.
    traceTimeUnit = NS;
    traceEndTime = 0;
    traceProcessModel = ProcessModel( this, whichFile, traceEndTime );
    traceResourceModel = ResourceModel( );

    body->setProcessModel( &traceProcessModel );
    body->setResourceModel( &traceResourceModel );

    if ( !file->canseekend() && progress != NULL )
    {
      progress->setEndLimit( traceEndTime );
    }
    //setEventTypePrecision( _, 2 );  //algo asi
  }
  else
  {
    file->getline( tmpstr );
    istringstream header( tmpstr );

    string tmpDate;
    std::getline( header, tmpDate, ')' );
    tmpDate = tmpDate.substr( tmpDate.find_first_of( '(' ) + 1 );
    parseDateTime( tmpDate );

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
        delete file;
        throw TraceHeaderException( TraceHeaderException::invalidTime,
                                    tmpstr.c_str() );
      }
    }
    else
    {
      string strTimeUnit( tmpstr.substr( pos, tmpstr.length() ) );
      if ( strTimeUnit == "_ns" )
        traceTimeUnit = NS;
      else if ( strTimeUnit == "_ms" )
        traceTimeUnit = MS;
      else //if ( strTimeUnit == "_us" )
        traceTimeUnit = US;

      istringstream stringEndTime( tmpstr.substr( 0, pos ) );
      if ( !( stringEndTime >> traceEndTime ) )
      {
        delete file;
        throw TraceHeaderException( TraceHeaderException::invalidTime,
                                    tmpstr.c_str() );
      }
   //   else cout << traceEndTime << endl;
    }

    if ( !file->canseekend() && progress != NULL )
    {
      progress->setEndLimit( traceEndTime );
    }

    traceResourceModel = ResourceModel( header );
    traceProcessModel = ProcessModel( header, existResourceInfo() );

    body->setProcessModel( &traceProcessModel );
    body->setResourceModel( &traceResourceModel );

    // Communicators
    PRV_UINT32 numberComm = 0;
    if ( !header.eof() )
    {
      std::getline( header, tmpstr );
      if ( tmpstr != "" )
      {
        istringstream streamComm( tmpstr );

        if ( !( streamComm >> numberComm ) )
        {
          delete file;
          throw TraceHeaderException( TraceHeaderException::invalidCommNumber,
                                      tmpstr.c_str() );
        }
      }
    }

    for ( PRV_UINT32 count = 0; count < numberComm; count++ )
    {
      file->getline( tmpstr );
      if ( tmpstr[0] != 'C' && tmpstr[0] != 'c' && tmpstr[0] != 'I' && tmpstr[0] != 'i' )
      {
        delete file;
        throw TraceHeaderException( TraceHeaderException::unknownCommLine,
                                    tmpstr.c_str() );
      }
      communicators.push_back( tmpstr );
    }
    // End communicators
  }
// End reading the header

// Reading the body

  if ( noLoad && body->ordered() )
  {
    blocks = new NoLoadBlocks( traceResourceModel, traceProcessModel, body, file, traceEndTime );
    memTrace = new NoLoadTrace( blocks, traceProcessModel, traceResourceModel );
    ( ( NoLoadBlocks * )blocks )->setFirstOffset( file->tellg() );
  }
  else if( noLoad && !body->ordered() )
  {
    blocks = new TraceEditBlocks( traceResourceModel, traceProcessModel, body, file, traceEndTime );
    memTrace = new NoLoadTrace( blocks, traceProcessModel, traceResourceModel );
    ( ( TraceEditBlocks * )blocks )->setFirstOffset( file->tellg() );
  }
  else if ( body->ordered() )
  {
    blocks = new PlainBlocks( traceResourceModel, traceProcessModel, traceEndTime );
    memTrace  = new PlainTrace( this,
                                traceProcessModel,
                                traceResourceModel );
  }
  else
  {
    blocks = new BPlusTreeBlocks( traceProcessModel );
    memTrace  = new BPlusTree( this,
                               traceProcessModel.totalThreads(),
                               traceResourceModel.totalCPUs() );
  }

  hash_set<TEventType> hashevents;
  hash_set<TState> hashstates;

  unsigned long long count = 0;
  if( !( noLoad && !body->ordered() ) )
  {
    while ( !file->eof() )
    {
      body->read( file, *blocks, hashstates, hashevents, myTraceInfo );
      if( blocks->getCountInserted() > 0 )
        ++count;

      if ( blocks->getCountInserted() >= 10000 )
      {
        memTrace->insert( blocks );
        if ( progress != NULL )
        {
          if ( file->canseekend() )
            progress->setCurrentProgress( file->tellg() );
          else
            progress->setCurrentProgress( blocks->getLastRecordTime() );
        }

        if ( progress != NULL && progress->getStop() )
          break;
      }
    }

    if ( blocks->getCountInserted() > 0 )
    {
      memTrace->insert( blocks );
      ++count;
    }

    if (count == 0)
    {
      delete file;
      throw TraceHeaderException( TraceHeaderException::emptyBody,
                                  whichFile.c_str() );
    }

    for ( hash_set<TEventType>::iterator it = hashevents.begin(); it != hashevents.end(); ++it )
      events.insert( *it );

    for ( hash_set<TState>::iterator it = hashstates.begin(); it != hashstates.end(); ++it )
      states.insert( *it );
  }

// End reading the body
  traceEndTime = memTrace->finish( traceEndTime, this );

//  cout << traceEndTime << endl;

  if ( !noLoad )
  {
    file->close();
    delete body;
    body = NULL;
  }
  else
  {
    file->clear();
  }

  blocks->setFileLoaded();

  delete file;

  ready = true;
}

KTrace::~KTrace()
{
  delete blocks;
  delete memTrace;
  if ( body != NULL )
    delete body;
}

const set<TState>& KTrace::getLoadedStates() const
{
  return states;
}

const set<TEventType>& KTrace::getLoadedEvents() const
{
  return events;
}

bool KTrace::eventLoaded( TEventType whichType ) const
{
  return events.find( whichType ) != events.end();
}

bool KTrace::anyEventLoaded( TEventType firstType, TEventType lastType ) const
{
  set<TEventType>::const_iterator it;
  it = events.lower_bound( firstType );
  if( it != events.end() && *it <= lastType )
    return true;

  return false;
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

bool KTrace::findNextEvent( TThreadOrder whichThread,
                            TRecordTime whichTime,
                            TEventType whichEvent,
                            TRecordTime& foundTime ) const
{
  bool found = false;
  vector<MemoryTrace::iterator *> listIter;
  MemoryTrace::iterator *it;

  listIter.insert( listIter.begin(), totalThreads(), NULL );
  getRecordByTimeThread( listIter, whichTime );
  it = listIter[ whichThread ];
  while( it->getTime() < whichTime )
    ++( *it );

  while ( !it->isNull() && !found )
  {
    if ( ( it->getType() & EVENT ) && ( it->getEventType() == whichEvent ) && ( it->getTime() != whichTime ) )
    {
      foundTime = it->getTime();
      found = true;
      break;
    }
    ++( *it );
  }

  for ( vector<MemoryTrace::iterator *>::iterator itErase = listIter.begin();
        itErase != listIter.end(); ++itErase )
    delete *itErase;

  return found;
}


bool KTrace::getFillStateGaps() const
{
  return fillStateGaps;
}

void KTrace::setFillStateGaps( bool fill )
{
  fillStateGaps = fill;
}


PRV_UINT64 KTrace::getCutterLastOffset()
{
  return myTraceInfo.GetCutterLastOffset();
}


PRV_UINT64 KTrace::getCutterLastBeginTime()
{
  return myTraceInfo.GetCutterLastBeginTime();
}


PRV_UINT64 KTrace::getCutterLastEndTime()
{
  return myTraceInfo.GetCutterLastEndTime();
}


PRV_UINT64 KTrace::getCutterOffset()
{
  //return myTraceInfo.cutterOffset;
  PRV_UINT64 offset = 0;

  vector<CutterMetadata*> cutterData = myTraceInfo.GetCutterMetadata();
  if( !cutterData.empty() )
    offset = cutterData[0]->GetOffset();

  return offset;
}


PRV_UINT64 KTrace::getCutterBeginTime()
{
  //return myTraceInfo.cutterBeginTime;
  PRV_UINT64 beginTime = 0;

  if ( !myTraceInfo.GetError() )
  {
    vector<CutterMetadata*> cutterData = myTraceInfo.GetCutterMetadata();
    if( !cutterData.empty() )
      beginTime = cutterData[0]->GetBeginTime();
  }

  return beginTime;
}


PRV_UINT64 KTrace::getCutterEndTime()
{
  //return myTraceInfo.cutterEndTime;
  PRV_UINT64 endTime = 0;

  if ( !myTraceInfo.GetError() )
  {
    vector<CutterMetadata*> cutterData = myTraceInfo.GetCutterMetadata();
    if( !cutterData.empty() )
     endTime = cutterData[0]->GetEndTime();
  }

  return endTime;
}


void KTrace::setLogicalSend( TCommID whichComm, TRecordTime whichTime )
{
  blocks->setLogicalSend( whichComm, whichTime );
}


void KTrace::setLogicalReceive( TCommID whichComm, TRecordTime whichTime )
{
  blocks->setLogicalReceive( whichComm, whichTime );
}


void KTrace::setPhysicalSend( TCommID whichComm, TRecordTime whichTime )
{
  blocks->setPhysicalSend( whichComm, whichTime );
}


void KTrace::setPhysicalReceive( TCommID whichComm, TRecordTime whichTime )
{
  blocks->setPhysicalReceive( whichComm, whichTime );
}

void KTrace::setEventTypePrecision( TEventType whichType, double whichPrecision )
{
  eventsPrecision.insert( std::pair<TEventType, double>( whichType, whichPrecision ) ) ;
}

double KTrace::getEventTypePrecision( TEventType whichType ) const
{
  map<TEventType, double>::const_iterator it = eventsPrecision.find( whichType );
  if( it != eventsPrecision.end() )
    return it->second;

  return 0.0;
}
