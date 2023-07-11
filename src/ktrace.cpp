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

#include <array>
#include <fstream>
#include <sstream>
#ifdef _MSC_VER
#include <hash_set>
#else
#include  <unordered_set>
#endif

#include "ktrace.h"
#include "traceheaderexception.h"
#include "tracebodyiofactory.h"
#include "tracebodyio_v2.h"
#include "tracestream.h"
#include "kprogresscontroller.h"
#include "plaintrace.h"
#include "plainblocks.h"
#include "noloadtrace.h"
#include "noloadblocks.h"
#include "traceeditblocks.h"
#include "customalgorithms.h"
#include "utils/traceparser/traceheader.h"
#include "vectorblocks.h"
#include "vectortrace.h"

using namespace std;
#ifdef _MSC_VER
using namespace stdext;
#endif
using boost::posix_time::time_input_facet;

using namespace Plain;
using namespace NoLoad;

string KTrace::getFileName() const
{
  return fileName;
}

string KTrace::getTraceName() const
{
  string traceName = fileName;

#ifdef _WIN32
  string::size_type pos = traceName.find_last_of( '\\' );
#else
  string::size_type pos = traceName.find_last_of( '/' );
#endif
  if ( pos != string::npos )
    traceName = traceName.substr( pos + 1, traceName.size() );

  return traceName;
}

TTraceSize KTrace::getTraceSize() const
{
  return traceSize;
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

TNodeOrder KTrace::getNodeFromThread( TThreadOrder &whichThread ) const
{
  return traceProcessModel.getNode(whichThread);
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
                               TTraceLevel fromLevel,
                               TTraceLevel toLevel  ) const
{
  if ( fromLevel == TTraceLevel::WORKLOAD || fromLevel == TTraceLevel::SYSTEM )
  {
    return 0;
  }
  else if ( fromLevel == TTraceLevel::APPLICATION )
  {
    if ( toLevel == TTraceLevel::TASK )
      return getFirstTask( globalOrder );
    else
      return getFirstThread( globalOrder, 0 );
  }
  else if ( fromLevel == TTraceLevel::TASK )
  {
    TApplOrder application;
    TTaskOrder task;
    getTaskLocation( globalOrder, application, task );
    return getFirstThread( application, task );
  }
  else if ( fromLevel == TTraceLevel::NODE )
  {
    return getFirstCPU( globalOrder );
  }

  return 0;
}


TObjectOrder KTrace::getLast( TObjectOrder globalOrder,
                              TTraceLevel fromLevel,
                              TTraceLevel toLevel ) const
{
  if ( fromLevel == TTraceLevel::WORKLOAD )
  {
    if ( toLevel == TTraceLevel::APPLICATION )
      return totalApplications() - 1;
    else if ( toLevel == TTraceLevel::TASK )
      return totalTasks() - 1;
    else
      return totalThreads() - 1;
  }
  else if ( fromLevel == TTraceLevel::APPLICATION )
  {
    if ( toLevel == TTraceLevel::TASK )
      return getLastTask( globalOrder );
    else
      return getLastThread( globalOrder, getLastTask( globalOrder ) );
  }
  else if ( fromLevel == TTraceLevel::TASK )
  {
    TApplOrder application;
    TTaskOrder task;
    getTaskLocation( globalOrder, application, task );
    return getLastThread( application, task );
  }
  else if ( fromLevel == TTraceLevel::SYSTEM )
  {
    if ( toLevel == TTraceLevel::NODE )
      return totalNodes() - 1;
    else
      return totalCPUs() - 1;
  }
  else if ( fromLevel == TTraceLevel::NODE )
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


void KTrace::dumpFileHeader( std::fstream& file, bool newFormat ) const
{
  string tmpTraceTime;
  if ( !myTraceTime.is_not_a_date_time() )
  {
    ostringstream tmpostr;
    tmpostr << myTraceTime;
    tmpTraceTime = tmpostr.str();
  }
  else
    tmpTraceTime = rawTraceTime;

  dumpTraceHeader( file, tmpTraceTime, traceEndTime, traceTimeUnit, traceResourceModel, traceProcessModel, communicators );
}


void KTrace::dumpFile( const string& whichFile ) const
{
  ostringstream ostr;

  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  std::fstream file( whichFile.c_str(), fstream::out | fstream::trunc );
  dumpFileHeader( file, true );

  MemoryTrace::iterator *it = memTrace->begin();
  TraceBodyIO< PARAM_TRACEBODY_CLASS > *body = TraceBodyIOFactory::createTraceBody();
  if( TraceBodyIO_v2 *tmpBody = dynamic_cast<TraceBodyIO_v2 *>( body ) )
    tmpBody->writeCommInfo( file, *this );

  while ( !it->isNull() )
  {
    body->write( file, traceProcessModel, traceResourceModel, it );
    ++( *it );

  }

  delete it;

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
  rawTraceTime = whichDateTime;

  // Create locales for every time format that whichDateTime may take
  static array<string, 6> strYearFormat
  {
    "%d/%m/%Y",
    "%d/%b/%Y",
    "%Y/%m/%d",
    "%Y/%b/%d",
    "%m/%d/%Y",
    "%b/%d/%Y"
  };
  static array<string, 2> strMidFormat
  {
    " ",
    ""
  };
  static array<string, 4> strHourFormat
  {
    "%H:%M:%S%F",
    "%H:%M:%S",
    "%H:%M",
    ""
  };

  static vector<array<string, 3>> strFormatDate;
  static vector<std::locale> formatDate;

  if( strFormatDate.size() == 0 )
  {
    cartesian_product( std::back_inserter( strFormatDate ), strYearFormat.begin(), strYearFormat.end(),
                                                            strMidFormat.begin(),  strMidFormat.end(),
                                                            strHourFormat.begin(), strHourFormat.end() );

    for( auto el: strFormatDate )
    {
      string tmp =  el[ 0 ] + el[ 1 ] + el[ 2 ];
      formatDate.push_back(std::locale( std::locale::classic(), new time_input_facet( tmp ) ) );
    }
  }

  try
  {
    whichDateTime.erase( whichDateTime.find( "at" ), 3 );
  }
  catch(const std::out_of_range& e)
  {
  }

  // Now, try to match one ptime from {date * format}
  for( vector<std::locale>::const_iterator it = formatDate.begin(); it != formatDate.end(); ++it )
  {
    std::stringstream tmpSSDate( whichDateTime );
    tmpSSDate.imbue( *it );
    boost::posix_time::ptime tmpTraceTime;
    if( tmpSSDate >> tmpTraceTime )
    {
      if( !tmpTraceTime.is_not_a_date_time() )
      {
        myTraceTime = tmpTraceTime;
        break;
      }
    }
  }
}


KTrace::KTrace( const string& whichFile, ProgressController *progress, bool noLoad, TTraceSize whichTraceSize )
    : fileName( whichFile ), traceSize( whichTraceSize )
{
  string tmpstr;

  ready = false;
  TraceStream *file = TraceStream::openFile( fileName );

  if ( !file->good() )
  {
    delete file;
    throw ParaverKernelException( TErrorCode::cannotOpenTrace, fileName.c_str() );
  }

  if ( file->canseekend() )
  {
    file->seekend();
    if ( progress != nullptr )
      progress->setEndLimit( file->tellg() );
    file->seekbegin();
  }

// Reading the header
  string fileType = whichFile.substr( whichFile.find_last_of( '.' ) + 1 );
  if ( fileType == "csv" )
  {
    // Saved CSV traces are in nanoseconds.
    traceTimeUnit = NS;
    traceEndTime = 0;
    traceProcessModel = ProcessModel<>( );
    traceResourceModel = ResourceModel<>( );

    traceProcessModel.setReady( true );

    if ( !file->canseekend() && progress != nullptr )
    {
      progress->setEndLimit( traceEndTime );
    }
  }
  else
  {
    string tmpDate;
    try
    {
      parseTraceHeader( *file, tmpDate, traceTimeUnit, traceEndTime, traceResourceModel, traceProcessModel, communicators );
    }
    catch( TraceHeaderException& e )
    {
      delete file;
      throw e;
    }

    parseDateTime( tmpDate );

    if ( !file->canseekend() && progress != nullptr )
    {
      progress->setEndLimit( traceEndTime );
    }
  }

// Reading the body
  body = TraceBodyIOFactory::createTraceBody( file, this, traceProcessModel );

  if ( noLoad && body->ordered() )
  {
    blocks = new NoLoadBlocks( traceResourceModel, traceProcessModel, body, file, traceEndTime );
    memTrace = new NoLoadTrace( this, blocks, traceProcessModel, traceResourceModel );
    ( ( NoLoadBlocks * )blocks )->setFirstOffset( file->tellg() );
  }
  else if( noLoad && !body->ordered() )
  {
    blocks = new TraceEditBlocks( traceResourceModel, traceProcessModel, body, file, traceEndTime );
    memTrace = new NoLoadTrace( this, blocks, traceProcessModel, traceResourceModel );
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
    blocks = new VectorBlocks( traceResourceModel, traceProcessModel, traceEndTime, progress );
    memTrace = new VectorTrace();
  }

  unordered_set<TEventType> hashevents;
  unordered_set<TState> hashstates;

  unsigned long long count = 0;
  if( !( noLoad && !body->ordered() ) )
  {
    int insertCounter = 0;
    while ( !file->eof() )
    {
      body->read( *file, *blocks, traceProcessModel, traceResourceModel, hashstates, hashevents, myTraceInfo, traceEndTime );
      if( blocks->getCountInserted() > 0 )
        ++count;

      if ( blocks->getCountInserted() >= 10000 )
      {
        memTrace->insert( blocks );
        if ( progress != nullptr && insertCounter == 100 )
        {
          insertCounter = 0;
          if ( file->canseekend() )
            progress->setCurrentProgress( file->tellg() );
          else
            progress->setCurrentProgress( blocks->getLastRecordTime() );
        }

        if ( progress != nullptr && progress->getStop() )
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
      throw TraceHeaderException( TTraceHeaderErrorCode::emptyBody,
                                  whichFile.c_str() );
    }

    for ( unordered_set<TEventType>::iterator it = hashevents.begin(); it != hashevents.end(); ++it )
      events.insert( *it );

    for ( unordered_set<TState>::iterator it = hashstates.begin(); it != hashstates.end(); ++it )
      states.insert( *it );
  }

// End reading the body
  traceEndTime = memTrace->finish( traceEndTime, this );

  if ( !noLoad )
  {
    file->close();
    delete body;
    body = nullptr;
    delete file;
  }
  else
  {
    file->clear();
  }

  blocks->setFileLoaded( traceEndTime );

  ready = true;
}

KTrace::~KTrace()
{
  delete blocks;
  delete memTrace;
  if ( body != nullptr )
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

  listIter.insert( listIter.begin(), totalThreads(), nullptr );
  getRecordByTimeThread( listIter, whichTime );
  it = listIter[ whichThread ];

  while ( it->getTime() > whichTime )
    --( *it );

  while ( !it->isNull() && !result )
  {
    for ( vector<TEventType>::const_iterator itEvt = whichEvent.begin();
          itEvt != whichEvent.end(); ++itEvt )
    {
      if ( ( it->getRecordType() & EVENT ) && ( it->getEventType() == *itEvt )
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

  listIter.insert( listIter.begin(), totalThreads(), nullptr );
  getRecordByTimeThread( listIter, whichTime );
  it = listIter[ whichThread ];
  while( it->getTime() < whichTime )
    ++( *it );

  while ( !it->isNull() && !found )
  {
    if ( ( it->getRecordType() & EVENT ) && ( it->getEventType() == whichEvent ) && ( it->getTime() != whichTime ) )
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
  const auto it = eventsPrecision.find( whichType );
  if( it != eventsPrecision.end() )
    return it->second;

  return 0.0;
}
