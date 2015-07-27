/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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

#include "kernelconnection.h"
#include "trace.h"
#include "pcfparser/libtools/ParaverTraceConfig.h"
//#include "pcfparser/ParaverStatesColor.h"
//#include "pcfparser/ParaverGradientColor.h"
#include "progresscontroller.h"
#include <sstream>
#include "paraverconfig.h"
//#include "extrae_user_events.h"

using namespace libparaver;
using namespace std;

Trace *Trace::create( KernelConnection *whichKernel, const string& whichFile,
                      bool noLoad, ProgressController *progress )
{
  return new TraceProxy( whichKernel, whichFile, noLoad, progress );
}


// Smarter detections welcome!
bool Trace::isTraceFile( const string& filename )
{
  string auxName( filename );
  string suffixCompressed( "" );
  string suffixNotCompressed( "" );

  if ( auxName.length() > GZIPPED_PRV_SUFFIX.length() )
    suffixCompressed = auxName.substr( auxName.length() - GZIPPED_PRV_SUFFIX.length() );

  if ( auxName.length() > PRV_SUFFIX.length() )
    suffixNotCompressed = auxName.substr( auxName.length() - PRV_SUFFIX.length() );

  return (( suffixCompressed.compare( GZIPPED_PRV_SUFFIX ) == 0 ) ||
          ( suffixNotCompressed.compare( PRV_SUFFIX ) == 0 ));
}


bool Trace::isOTF2TraceFile( const string& filename )
{
  string auxName( filename );
  string suffix( "" );

  if ( auxName.length() > OTF2_SUFFIX.length() )
    suffix = auxName.substr( auxName.length() - OTF2_SUFFIX.length() );

  return ( ( suffix.compare( OTF2_SUFFIX ) == 0 ));
}


Trace::Trace( KernelConnection *whichKernel ):
    myKernel( whichKernel )
{}

TObjectOrder Trace::getLevelObjects( TWindowLevel onLevel ) const
{
  switch ( onLevel )
  {
    case WORKLOAD:
    case SYSTEM:
      return 1;
      break;
    case APPLICATION:
      return totalApplications();
      break;
    case TASK:
      return totalTasks();
      break;
    case THREAD:
      return totalThreads();
      break;
    case NODE:
      return totalNodes();
      break;
    case CPU:
      return totalCPUs();
      break;
    default:
      return 0;
  }
  return 0;
}

TraceProxy::TraceProxy( KernelConnection *whichKernel, const string& whichFile,
                        bool noLoad, ProgressController *progress ):
    Trace( whichKernel )
{
  unload = false;
  instanceNumber = 0;

  myTrace = myKernel->newTrace( whichFile, noLoad, progress );
  string pcfFile = myKernel->getPCFFileLocation( whichFile );
  parsePCF( pcfFile );
  string rowFile = myKernel->getROWFileLocation( whichFile );
  parseROW( rowFile );

#ifdef FIXED_LABELS
  setFixedLabels();
#endif

  myTrace->setFillStateGaps( ParaverConfig::getInstance()->getGlobalFillStateGaps() );
}

#ifdef FIXED_LABELS
TObjectOrder getNumLevelObjects( TWindowLevel whichLevel, Trace *myTrace )
{
  TObjectOrder objectSize = 0;

  if( whichLevel == WORKLOAD )
    objectSize = 1;
  else if( whichLevel == APPLICATION )
    objectSize = myTrace->totalApplications();
  else if( whichLevel == TASK )
    objectSize = myTrace->totalTasks();
  else if( whichLevel == THREAD )
    objectSize = myTrace->totalThreads();
  else if( whichLevel == SYSTEM )
    objectSize = 1;
  else if( whichLevel == NODE )
    objectSize = myTrace->totalNodes();
  else if( whichLevel == CPU )
    objectSize = myTrace->totalCPUs();

  return objectSize;
}

#include "labelconstructor.h"

void TraceProxy::setFixedLabels()
{
  RowLabels tmpRowLabels;
  int maxLabelLength = 0;
  std::string tmpObjectLabel;
  std::string newObjectLabel;

  for( int lvl = WORKLOAD; lvl <= CPU; ++lvl )
  {
    for( TObjectOrder order = 0; order < getNumLevelObjects( (TWindowLevel)lvl, this ); ++order )
    {
      tmpObjectLabel = LabelConstructor::objectLabel( order, (TWindowLevel)lvl, this );
      if( tmpObjectLabel.length() > maxLabelLength )
        maxLabelLength = tmpObjectLabel.length();
    }
  }

  for( int lvl = WORKLOAD; lvl <= CPU; ++lvl )
  {
    for( TObjectOrder order = 0; order < getNumLevelObjects( (TWindowLevel)lvl, this ); ++order )
    {
      tmpObjectLabel = LabelConstructor::objectLabel( order, (TWindowLevel)lvl, this );
      int numSpaces = maxLabelLength - tmpObjectLabel.length();
      newObjectLabel.clear();
      newObjectLabel.append( numSpaces, ' ' );
      newObjectLabel.append( tmpObjectLabel );
      tmpRowLabels.pushBack( (TWindowLevel)lvl, newObjectLabel );
    }
  }

  myRowLabels = tmpRowLabels;
}
#endif

TraceProxy::~TraceProxy()
{
  delete myTrace;
}

string TraceProxy::getFileName() const
{
  return myTrace->getFileName();
}

string TraceProxy::getTraceName() const
{
  return myTrace->getTraceName();
}

string TraceProxy::getFileNameNumbered() const
{
  string name = myTrace->getFileName();

  if ( instanceNumber > 0 )
  {
    stringstream suffix;
    suffix << instanceNumber;
    name = name + " #" + suffix.str();
  }

  return name;
}

string TraceProxy::getTraceNameNumbered() const
{
  string name = myTrace->getTraceName();

  if ( instanceNumber > 0 )
  {
    stringstream suffix;
    suffix << instanceNumber;
    name = name + " #" + suffix.str();
  }

  return name;
}

void TraceProxy::setInstanceNumber( PRV_UINT32 whichInstanceNumber )
{
  instanceNumber = whichInstanceNumber;
}

void TraceProxy::dumpFileHeader( std::fstream& file, bool newFormat, PRV_INT32 numIter ) const
{
  myTrace->dumpFileHeader( file, newFormat, numIter );
}

void TraceProxy::dumpFile( const string& whichFile, PRV_INT32 numIter ) const
{
  myTrace->dumpFile( whichFile, numIter );
}

TApplOrder TraceProxy::totalApplications() const
{
  return myTrace->totalApplications();
}

TTaskOrder TraceProxy::totalTasks() const
{
  return myTrace->totalTasks();
}

TTaskOrder TraceProxy::getGlobalTask( const TApplOrder& inAppl,
                                      const TTaskOrder& inTask ) const
{
  return myTrace->getGlobalTask( inAppl, inTask );
}

void TraceProxy::getTaskLocation( TTaskOrder globalTask,
                                  TApplOrder& inAppl,
                                  TTaskOrder& inTask ) const
{
  myTrace->getTaskLocation( globalTask, inAppl, inTask );
}

TTaskOrder TraceProxy::getFirstTask( TApplOrder inAppl ) const
{
  return myTrace->getFirstTask( inAppl );
}

TTaskOrder TraceProxy::getLastTask( TApplOrder inAppl ) const
{
  return myTrace->getLastTask( inAppl );
}

TThreadOrder TraceProxy::totalThreads() const
{
  return myTrace->totalThreads();
}

TThreadOrder TraceProxy::getGlobalThread( const TApplOrder& inAppl,
    const TTaskOrder& inTask,
    const TThreadOrder& inThread ) const
{
  return myTrace->getGlobalThread( inAppl, inTask, inThread );
}

void TraceProxy::getThreadLocation( TThreadOrder globalThread,
                                    TApplOrder& inAppl,
                                    TTaskOrder& inTask,
                                    TThreadOrder& inThread ) const
{
  myTrace->getThreadLocation( globalThread, inAppl, inTask, inThread );
}

TThreadOrder TraceProxy::getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const
{
  return myTrace->getFirstThread( inAppl, inTask );
}

TThreadOrder TraceProxy::getLastThread( TApplOrder inAppl, TTaskOrder inTask ) const
{
  return myTrace->getLastThread( inAppl, inTask );
}

void TraceProxy::getThreadsPerNode( TNodeOrder inNode, std::vector<TThreadOrder>& onVector ) const
{
  myTrace->getThreadsPerNode( inNode, onVector );
}

bool TraceProxy::existResourceInfo() const
{
  return myTrace->existResourceInfo();
}

TNodeOrder TraceProxy::totalNodes() const
{
  return myTrace->totalNodes();
}

TCPUOrder TraceProxy::totalCPUs() const
{
  return myTrace->totalCPUs();
}

TCPUOrder TraceProxy::getGlobalCPU( const TNodeOrder& inNode,
                                    const TCPUOrder& inCPU ) const
{
  return myTrace->getGlobalCPU( inNode, inCPU );
}

void TraceProxy::getCPULocation( TCPUOrder globalCPU,
                                 TNodeOrder& inNode,
                                 TCPUOrder& inCPU ) const
{
  myTrace->getCPULocation( globalCPU, inNode, inCPU );
}

TCPUOrder TraceProxy::getFirstCPU( TNodeOrder inNode ) const
{
  return myTrace->getFirstCPU( inNode );
}

TCPUOrder TraceProxy::getLastCPU( TNodeOrder inNode ) const
{
  return myTrace->getLastCPU( inNode );
}

TObjectOrder TraceProxy::getFirst( TObjectOrder globalOrder,
                                   TWindowLevel fromLevel,
                                   TWindowLevel toLevel ) const
{
  return myTrace->getFirst( globalOrder, fromLevel, toLevel );
}

TObjectOrder TraceProxy::getLast( TObjectOrder globalOrder,
                                  TWindowLevel fromLevel,
                                  TWindowLevel toLevel ) const
{
  return myTrace->getLast( globalOrder, fromLevel, toLevel );
}

bool TraceProxy::isSameObjectStruct( Trace *compareTo ) const
{
  return myTrace->isSameObjectStruct( compareTo->getConcrete() );
}

TThreadOrder TraceProxy::getSenderThread( TCommID whichComm ) const
{
  return myTrace->getSenderThread( whichComm );
}

TCPUOrder TraceProxy::getSenderCPU( TCommID whichComm ) const
{
  return myTrace->getSenderCPU( whichComm );
}

TThreadOrder TraceProxy::getReceiverThread( TCommID whichComm ) const
{
  return myTrace->getReceiverThread( whichComm );
}

TCPUOrder TraceProxy::getReceiverCPU( TCommID whichComm ) const
{
  return myTrace->getReceiverCPU( whichComm );
}

TCommTag TraceProxy::getCommTag( TCommID whichComm ) const
{
  return myTrace->getCommTag( whichComm );
}

TCommSize TraceProxy::getCommSize( TCommID whichComm ) const
{
  return myTrace->getCommSize( whichComm );
}

TRecordTime TraceProxy::getLogicalSend( TCommID whichComm ) const
{
  return myTrace->getLogicalSend( whichComm );
}

TRecordTime TraceProxy::getLogicalReceive( TCommID whichComm ) const
{
  return myTrace->getLogicalReceive( whichComm );
}

TRecordTime TraceProxy::getPhysicalSend( TCommID whichComm ) const
{
  return myTrace->getPhysicalSend( whichComm );
}

TRecordTime TraceProxy::getPhysicalReceive( TCommID whichComm ) const
{
  return myTrace->getPhysicalReceive( whichComm );
}

TTime TraceProxy::getEndTime() const
{
  return myTrace->getEndTime();
}

void TraceProxy::setEndTime( TTime whichTraceEndTime )
{
  myTrace->setEndTime( whichTraceEndTime );
}

TTimeUnit TraceProxy::getTimeUnit() const
{
  return myTrace->getTimeUnit();
}

bool TraceProxy::getUnload() const
{
  return unload;
}

void TraceProxy::setUnload( bool newValue )
{
  unload = newValue;
}

Trace *TraceProxy::getConcrete() const
{
  return myTrace;
}

void TraceProxy::parsePCF( const string& whichFile )
{
  ParaverTraceConfig *config;

  try
  {
    config = new ParaverTraceConfig();
    config->parse( whichFile /* true */ );
  }
  catch ( ... )
  {
    myEventLabels = EventLabels( myTrace->getLoadedEvents() );
    return;
  }

  rgb tmpColor;

  const vector< int >& stateColors = config->getStateColors();
  for ( vector< int >::const_iterator it = stateColors.begin(); it != stateColors.end(); ++it )
  {
    tmpColor.red = config->getStateColor( *it ).getRed();
    tmpColor.green = config->getStateColor( *it ).getGreen();
    tmpColor.blue = config->getStateColor( *it ).getBlue();
    myCodeColor.setColor( ( PRV_UINT32 )( *it ), tmpColor );
  }



  const vector< int >& gradientColors = config->getGradientColors();
  if ( gradientColors.begin() != gradientColors.end() )
  {
    tmpColor.red = config->getGradientColor( gradientColors[0] ).getRed();
    tmpColor.green = config->getGradientColor( gradientColors[0] ).getGreen();
    tmpColor.blue = config->getGradientColor( gradientColors[0] ).getBlue();
    myGradientColor.setBeginGradientColor( tmpColor );

    size_t last = gradientColors.size() - 1;
    tmpColor.red = config->getGradientColor( gradientColors[ last ] ).getRed();
    tmpColor.green = config->getGradientColor( gradientColors[ last ] ).getGreen();
    tmpColor.blue = config->getGradientColor( gradientColors[ last ] ).getBlue();
    myGradientColor.setEndGradientColor( tmpColor );
  }
  myEventLabels = EventLabels( *config, myTrace->getLoadedEvents() );
  myStateLabels = StateLabels( *config );

  // myDefaultTaskSemanticFunc = config->get_default_task_semantic_func();
  // myDefaultThreadSemanticFunc = config->get_default_thread_semantic_func();

  delete config;
}

void TraceProxy::parseROW( const string& whichFile )
{
  myRowLabels = RowLabels( whichFile );
}

const CodeColor& TraceProxy::getCodeColor() const
{
  return myCodeColor;
}

const GradientColor& TraceProxy::getGradientColor() const
{
  return myGradientColor;
}

const EventLabels& TraceProxy::getEventLabels() const
{
  return myEventLabels;
}

const StateLabels& TraceProxy::getStateLabels() const
{
  return myStateLabels;
}

string TraceProxy::getRowLabel( TWindowLevel whichLevel, TObjectOrder whichRow ) const
{
  return myRowLabels.getRowLabel( whichLevel, whichRow );
}


/*
 string TraceProxy::getDefaultSemanticFunc( TWindowLevel whichLevel ) const
{
  switch ( whichLevel )
  {
    case ( TASK ):
            return myDefaultTaskSemanticFunc;
      break;
    case( THREAD ):
            return myDefaultThreadSemanticFunc;
      break;
    default:
      break;
  }

  return myDefaultThreadSemanticFunc;
}

*/

bool TraceProxy::eventLoaded( TEventType whichType ) const
{
  return myTrace->eventLoaded( whichType );
}

bool TraceProxy::anyEventLoaded( TEventType firstType, TEventType lastType ) const
{
  return myTrace->anyEventLoaded( firstType, lastType );
}

const set<TEventType>& TraceProxy::getLoadedEvents() const
{
  return myTrace->getLoadedEvents();
}

bool TraceProxy::findLastEventValue( TThreadOrder whichThread,
                                     TRecordTime whichTime,
                                     const vector<TEventType>& whichEvent,
                                     TEventType& returnType,
                                     TEventValue& returnValue ) const
{
  return myTrace->findLastEventValue( whichThread, whichTime, whichEvent, returnType, returnValue );
}

bool TraceProxy::findNextEvent( TThreadOrder whichThread,
                                TRecordTime whichTime,
                                TEventType whichEvent,
                                TRecordTime& foundTime ) const
{
  return myTrace->findNextEvent( whichThread, whichTime, whichEvent, foundTime );
}

bool TraceProxy::getFillStateGaps() const
{
  return myTrace->getFillStateGaps();
}


void TraceProxy::setFillStateGaps( bool fill )
{
  myTrace->setFillStateGaps( fill );
}


PRV_UINT64 TraceProxy::getCutterOffset()
{
  return myTrace->getCutterOffset();
}


PRV_UINT64 TraceProxy::getCutterBeginTime()
{
  return myTrace->getCutterBeginTime();
}


PRV_UINT64 TraceProxy::getCutterEndTime()
{
  return myTrace->getCutterEndTime();
}


void TraceProxy::setLogicalSend( TCommID whichComm, TRecordTime whichTime )
{
  myTrace->setLogicalSend( whichComm, whichTime );
}


void TraceProxy::setLogicalReceive( TCommID whichComm, TRecordTime whichTime )
{
  myTrace->setLogicalReceive( whichComm, whichTime );
}


void TraceProxy::setPhysicalSend( TCommID whichComm, TRecordTime whichTime )
{
  myTrace->setPhysicalSend( whichComm, whichTime );
}


void TraceProxy::setPhysicalReceive( TCommID whichComm, TRecordTime whichTime )
{
  myTrace->setPhysicalReceive( whichComm, whichTime );
}

