#include "kernelconnection.h"
#include "trace.h"
#include "pcfparser/ParaverTraceConfig.h"
#include "pcfparser/ParaverStatesColor.h"
#include "pcfparser/ParaverGradientColor.h"
#include "progresscontroller.h"
#include <sstream>
#include "paraverconfig.h"

using namespace libparaver;

Trace *Trace::create( KernelConnection *whichKernel, const string& whichFile,
                      ProgressController *progress )
{
  return new TraceProxy( whichKernel, whichFile, progress );
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
                        ProgressController *progress ):
    Trace( whichKernel )
{
  unload = false;
  instanceNumber = 0;

  myTrace = myKernel->newTrace( whichFile, progress );
  string pcfFile = myKernel->getPCFFileLocation( whichFile );
  parsePCF( pcfFile );
  string rowFile = myKernel->getROWFileLocation( whichFile );
  parseROW( rowFile );

  myTrace->setFillStateGaps( ParaverConfig::getInstance()->getGlobalFillStateGaps() );
}

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

void TraceProxy::setInstanceNumber( UINT32 whichInstanceNumber )
{
  instanceNumber = whichInstanceNumber;
}

void TraceProxy::dumpFile( const string& whichFile ) const
{
  myTrace->dumpFile( whichFile );
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
    config = new ParaverTraceConfig( whichFile );
  }
  catch ( ... )
  {
    myEventLabels = EventLabels( myTrace->getLoadedEvents() );
    return;
  }

  rgb tmpColor;

  if ( config->get_statesColor().begin() != config->get_statesColor().end() )
  {
    for ( vector<ParaverStatesColor *>::const_iterator it = config->get_statesColor().begin();
          it != config->get_statesColor().end(); ++it )
    {
      tmpColor.red = ( *it )->get_color1();
      tmpColor.green = ( *it )->get_color2();
      tmpColor.blue = ( *it )->get_color3();
      if ( ( UINT32 )( *it )->get_key() < myCodeColor.getNumColors() )
        myCodeColor.setColor( ( UINT32 )( *it )->get_key(), tmpColor );
      else
        myCodeColor.addColor( tmpColor );
    }
  }

  if ( config->get_gradientColors().begin() != config->get_gradientColors().end() )
  {
    ParaverGradientColor *grad = ( config->get_gradientColors() )[ 0 ];
    tmpColor.red = grad->get_color1();
    tmpColor.green = grad->get_color2();
    tmpColor.blue = grad->get_color3();
    myGradientColor.setBeginGradientColor( tmpColor );

    grad = ( config->get_gradientColors() )[ config->get_gradientColors().size() - 1 ];
    tmpColor.red = grad->get_color1();
    tmpColor.green = grad->get_color2();
    tmpColor.blue = grad->get_color3();
    myGradientColor.setEndGradientColor( tmpColor );
  }
  myEventLabels = EventLabels( *config, myTrace->getLoadedEvents() );
  myStateLabels = StateLabels( *config );

  myDefaultTaskSemanticFunc = config->get_default_task_semantic_func();
  myDefaultThreadSemanticFunc = config->get_default_thread_semantic_func();

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

bool TraceProxy::eventLoaded( TEventType whichType ) const
{
  return myTrace->eventLoaded( whichType );
}

const set<TEventType>& TraceProxy::getLoadedEvents() const
{
  return myTrace->getLoadedEvents();
}

bool TraceProxy::findLastEventValue( TThreadOrder whichThread,
                                     TRecordTime whichTime,
                                     TEventType whichEvent,
                                     TEventValue& returnValue ) const
{
  return myTrace->findLastEventValue( whichThread, whichTime, whichEvent, returnValue );
}

bool TraceProxy::getFillStateGaps() const
{
  return myTrace->getFillStateGaps();
}


void TraceProxy::setFillStateGaps( bool fill )
{
  myTrace->setFillStateGaps( fill );
}

