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

#include <typeinfo>
#include "kwindow.h"
#include "semanticcomposefunctions.h"
#include "semanticderived.h"
#include "semanticcpufunctions.h"
#include "semanticnotthreadfunctions.h"
#include "semanticthreadfunctions.h"
#include "functionmanagement.h"
#include "semanticmanagement.h"

using namespace std;

KTimeline::~KTimeline()
{
  for( map< TWindowLevel, vector< vector< IntervalCompose * > > >::iterator itMap = extraCompose.begin();
       itMap != extraCompose.end(); ++itMap )
  {
    for( vector< vector< IntervalCompose * > >::iterator itVec = itMap->second.begin();
         itVec != itMap->second.end(); ++itVec )
    {
      for( vector<IntervalCompose *>::iterator it = itVec->begin(); it != itVec->end(); ++it )
        delete *it;
    }
  }

  for( map< TWindowLevel, vector< SemanticFunction * > >::iterator itMap = extraComposeFunctions.begin();
       itMap != extraComposeFunctions.end(); ++itMap )
  {
    for( vector< SemanticFunction * >::iterator itVec = itMap->second.begin();
         itVec != itMap->second.end(); ++itVec )
      delete *itVec;
  }
}

TObjectOrder KTimeline::cpuObjectToWindowObject( TCPUOrder whichCPU )
{
  TObjectOrder tmpObject = 0;

  if( level == TTraceLevel::CPU )
    tmpObject = whichCPU;
  else if( level == TTraceLevel::NODE )
  {
    TCPUOrder myCPU;
    TNodeOrder myNode;

    myTrace->getCPULocation( whichCPU, myNode, myCPU );
    tmpObject = myNode;
  }
  else if( level == TTraceLevel::SYSTEM )
  {
    tmpObject = 0;
  }

  return tmpObject;
}


TObjectOrder KTimeline::threadObjectToWindowObject( TThreadOrder whichThread )
{
  TObjectOrder tmpObject = 0;

  if( level == TTraceLevel::THREAD )
    tmpObject = whichThread;
  else if( level == TTraceLevel::TASK )
  {
    TThreadOrder myThread;
    TTaskOrder myTask;
    TApplOrder myAppl;

    myTrace->getThreadLocation( whichThread, myAppl, myTask, myThread );
    tmpObject = myTrace->getGlobalTask( myAppl, myTask );
  }
  else if( level == TTraceLevel::APPLICATION )
  {
    TThreadOrder myThread;
    TTaskOrder myTask;
    TApplOrder myAppl;

    myTrace->getThreadLocation( whichThread, myAppl, myTask, myThread );
    tmpObject = myAppl;
  }
  else if( level == TTraceLevel::WORKLOAD )
  {
    tmpObject = 0;
  }

  return tmpObject;
}


TObjectOrder KTimeline::getWindowLevelObjects() const
{
  TObjectOrder objectSize = 0;
  TTraceLevel whichLevel = getLevel();

  if( whichLevel == TTraceLevel::WORKLOAD )
    objectSize = 1;
  else if( whichLevel == TTraceLevel::APPLICATION )
    objectSize = myTrace->totalApplications();
  else if( whichLevel == TTraceLevel::TASK )
    objectSize = myTrace->totalTasks();
  else if( whichLevel == TTraceLevel::THREAD )
    objectSize = myTrace->totalThreads();
  else if( whichLevel == TTraceLevel::SYSTEM )
    objectSize = 1;
  else if( whichLevel == TTraceLevel::NODE )
    objectSize = myTrace->totalNodes();
  else if( whichLevel == TTraceLevel::CPU )
    objectSize = myTrace->totalCPUs();

  return objectSize;
}


TRecordTime KTimeline::customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const
{
  return myTrace->customUnitsToTraceUnits( whichTime, whichUnits );
}


TRecordTime KTimeline::traceUnitsToCustomUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const
{
  return myTrace->traceUnitsToCustomUnits( whichTime, whichUnits );
}


TRecordTime KTimeline::traceUnitsToWindowUnits( TRecordTime whichTime ) const
{
  TRecordTime tmpTime;
  TRecordTime factor = 1;

  if( myTrace->getTimeUnit() == timeUnit )
    tmpTime = whichTime;
  else
  {
    PRV_UINT16 from = myTrace->getTimeUnit() > timeUnit ? timeUnit : myTrace->getTimeUnit();
    PRV_UINT16 to = myTrace->getTimeUnit() > timeUnit ? myTrace->getTimeUnit() : timeUnit;

    for( PRV_UINT16 i = from + 1; i <= to; i++ )
      factor *= factorTable[ i ];

    if( myTrace->getTimeUnit() > timeUnit )
      tmpTime = whichTime * factor;
    else
      tmpTime = whichTime / factor;
  }

  return tmpTime;
}

TRecordTime KTimeline::windowUnitsToTraceUnits( TRecordTime whichTime ) const
{
  TRecordTime tmpTime;
  TRecordTime factor = 1;

  if( myTrace->getTimeUnit() == timeUnit )
    tmpTime = whichTime;
  else
  {
    PRV_UINT16 from = myTrace->getTimeUnit() > timeUnit ? timeUnit : myTrace->getTimeUnit();
    PRV_UINT16 to = myTrace->getTimeUnit() > timeUnit ? myTrace->getTimeUnit() : timeUnit;

    for( PRV_UINT16 i = from + 1; i <= to; i++ )
      factor *= factorTable[ i ];

    if( myTrace->getTimeUnit() > timeUnit )
      tmpTime = whichTime / factor;
    else
      tmpTime = whichTime * factor;
  }

  return tmpTime;
}

RecordList *KTimeline::getRecordList( TObjectOrder whichObject )
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->getRecordList();
  }

  return intervalTopCompose1[ whichObject ].getRecordList();
}

KTimeline *KTimeline::clone( bool recursiveClone )
{
  return nullptr;
}

void KTimeline::getGroupLabels( PRV_UINT32 whichGroup, vector<string>& onVector ) const
{
  FunctionManagement<SemanticFunction>::getInstance()->getAll( onVector, whichGroup );
}


bool KTimeline::getParametersOfFunction( string whichFunction,
                                       PRV_UINT32 &numParameters,
                                       vector< string > &nameParameters,
                                       vector< vector < double > > &defaultValues ) const
{
  bool done = false;

  SemanticFunction *tmp = FunctionManagement<SemanticFunction>::getInstance()->getFunction( whichFunction );

  if( tmp != nullptr )
  {
    numParameters = tmp->getMaxParam();

    nameParameters.clear();
    defaultValues.clear();
    for( PRV_UINT32 i = 0; i < numParameters; ++i )
    {
      nameParameters.push_back( tmp->getParamName( TParamIndex( i ) ) );
      defaultValues.push_back( tmp->getParam( TParamIndex( i ) ) );
    }

    delete tmp; // getFunctions returns a clone

    done = true;
  }

  return done;
}

// Extra composes
size_t KTimeline::getExtraNumPositions( TWindowLevel whichLevel ) const
{
  map< TWindowLevel, vector< vector< IntervalCompose * > > >::const_iterator it = extraCompose.find( whichLevel );

  if( it == extraCompose.end() )
    return 0;

  return it->second.size();
}


/**********************************************************************
 *  KSingleWindow implementation
 **********************************************************************/

KSingleWindow::KSingleWindow( Trace *whichTrace ): KTimeline( whichTrace )
{
  initSemanticFunctions();

  recordsByTimeThread.reserve( myTrace->totalThreads() );
  for( TThreadOrder i = 0; i < myTrace->totalThreads(); ++i )
    recordsByTimeThread.push_back( nullptr );
  recordsByTimeCPU.reserve( myTrace->totalCPUs() );
  for( TCPUOrder i = 0; i < myTrace->totalCPUs(); ++i )
    recordsByTimeCPU.push_back( nullptr );
  if( myTrace->totalThreads() > myTrace->totalCPUs() )
  {
    intervalTopCompose1.reserve( myTrace->totalThreads() );
    intervalTopCompose2.reserve( myTrace->totalThreads() );
    for( TThreadOrder i = 0; i < myTrace->totalThreads(); ++i )
    {
      intervalTopCompose1.push_back( IntervalCompose( this, TOPCOMPOSE1, i ) );
      intervalTopCompose2.push_back( IntervalCompose( this, TOPCOMPOSE2, i ) );
    }
  }
  else
  {
    intervalTopCompose1.reserve( myTrace->totalCPUs() );
    intervalTopCompose2.reserve( myTrace->totalCPUs() );
    for( TCPUOrder i = 0; i < myTrace->totalCPUs(); ++i )
    {
      intervalTopCompose1.push_back( IntervalCompose( this, TOPCOMPOSE1, i ) );
      intervalTopCompose2.push_back( IntervalCompose( this, TOPCOMPOSE2, i ) );
    }
  }

  intervalWorkload.push_back( IntervalNotThread( this, WORKLOAD, 0 ) );
  intervalComposeWorkload.push_back( IntervalCompose( this, COMPOSEWORKLOAD, 0 ) );

  intervalApplication.reserve( myTrace->totalApplications() );
  intervalComposeApplication.reserve( myTrace->totalApplications() );
  for( TApplOrder i = 0; i < myTrace->totalApplications(); ++i )
  {
    intervalApplication.push_back( IntervalNotThread( this, APPLICATION, i ) );
    intervalComposeApplication.push_back( IntervalCompose( this, COMPOSEAPPLICATION, i ) );
  }

  intervalTask.reserve( myTrace->totalTasks() );
  intervalComposeTask.reserve( myTrace->totalTasks() );
  for( TTaskOrder i = 0; i < myTrace->totalTasks(); ++i )
  {
    intervalTask.push_back( IntervalNotThread( this, TASK, i ) );
    intervalComposeTask.push_back( IntervalCompose( this, COMPOSETASK, i ) );
  }

  intervalThread.reserve( myTrace->totalThreads() );
  intervalComposeThread.reserve( myTrace->totalThreads() );
  for( TThreadOrder i = 0; i < myTrace->totalThreads(); ++i )
  {
    intervalThread.push_back( IntervalThread( this, THREAD, i ) );
    intervalComposeThread.push_back( IntervalCompose( this, COMPOSETHREAD, i ) );
  }

  intervalSystem.push_back( IntervalNotThread( this, SYSTEM, 0 ) );
  intervalComposeSystem.push_back( IntervalCompose( this, COMPOSESYSTEM, 0 ) );

  intervalNode.reserve( myTrace->totalNodes() );
  intervalComposeNode.reserve( myTrace->totalNodes() );
  for( TNodeOrder i = 0; i < myTrace->totalNodes(); ++i )
  {
    intervalNode.push_back( IntervalNotThread( this, NODE, i ) );
    intervalComposeNode.push_back( IntervalCompose( this, COMPOSENODE, i ) );
  }

  intervalCPU.reserve( myTrace->totalCPUs() );
  intervalComposeCPU.reserve( myTrace->totalCPUs() );
  for( TCPUOrder i = 0; i < myTrace->totalCPUs(); ++i )
  {
    intervalCPU.push_back( IntervalCPU( this, CPU, i + 1 ) );
    intervalComposeCPU.push_back( IntervalCompose( this, COMPOSECPU, i ) );
  }

  myFilter = new KFilter( this );
}


KSingleWindow::~KSingleWindow()
{
  for( PRV_UINT32 twlIdx = ( PRV_UINT32 ) WORKLOAD; twlIdx <= ( PRV_UINT32 ) COMPOSECPU; ++twlIdx )
  {
    if( functions[ ( TWindowLevel ) twlIdx ] != nullptr )
      delete functions[ ( TWindowLevel ) twlIdx ];
  }
  
  for( TObjectOrder i = 0; i < recordsByTimeThread.size(); ++i )
  {
    if( recordsByTimeThread[ i ] != nullptr )
      delete recordsByTimeThread[ i ];
  }
  recordsByTimeThread.clear();

  for( TObjectOrder i = 0; i < recordsByTimeCPU.size(); ++i )
  {
    if( recordsByTimeCPU[ i ] != nullptr )
      delete recordsByTimeCPU[ i ];
  }
  recordsByTimeCPU.clear();

  delete myFilter;
}


void KSingleWindow::init( TRecordTime initialTime, TCreateList create, bool updateLimits )
{
  for( PRV_UINT8 i = WORKLOAD; i <= COMPOSECPU; i++ )
  {
    if( functions[ i ] != nullptr )
      functions[ i ]->init( this );
  }

  for( map< TWindowLevel, vector< SemanticFunction * > >::iterator itMap = extraComposeFunctions.begin();
       itMap != extraComposeFunctions.end(); ++itMap )
  {
    for (vector<SemanticFunction *>::iterator it = itMap->second.begin();
         it != itMap->second.end(); ++it)
      (*it)->init( this );
  }

  if( extraCompose[ TOPCOMPOSE1 ].size() > 0 )
  {
    for( size_t iPos = 0; iPos < extraCompose[ TOPCOMPOSE1 ].size(); ++iPos )
    {
      for( size_t iRow = 0; iRow < ( extraCompose[ TOPCOMPOSE1 ] )[ iPos ].size(); ++iRow )
        ( ( extraCompose[ TOPCOMPOSE1 ] )[ iPos ] )[ iRow ]->setSemanticFunction( (SemanticCompose *)( extraComposeFunctions[ TOPCOMPOSE1 ] )[ iPos ] );
    }
  }

  if( level >= TTraceLevel::SYSTEM )
  {
    if( initialTime > 0 && !initFromBegin() )
      myTrace->getRecordByTimeCPU( recordsByTimeCPU, initialTime );
    else
    {
      for( TCPUOrder i = 0; i < myTrace->totalCPUs(); ++i )
      {
        if( recordsByTimeCPU[ i ] != nullptr )
          delete recordsByTimeCPU[ i ];
        recordsByTimeCPU[ i ] = myTrace->CPUBegin( i );
      }
    }
  }
  if( initialTime > 0 && !initFromBegin() )
    myTrace->getRecordByTimeThread( recordsByTimeThread, initialTime );
  else
  {
    for( TThreadOrder i = 0; i < myTrace->totalThreads(); ++i )
    {
      if( recordsByTimeThread[ i ] != nullptr )
        delete recordsByTimeThread[ i ];
      recordsByTimeThread[ i ] = myTrace->threadBegin( i );
    }
  }
}

void KSingleWindow::initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits )
{
  if( extraCompose[ TOPCOMPOSE1 ].size() > 0 )
  {
    for( size_t iPos = 0; iPos < extraCompose[ TOPCOMPOSE1 ].size(); ++iPos )
    {
      ( ( extraCompose[ TOPCOMPOSE1 ] )[ iPos ] )[ whichRow ]->setSemanticFunction( (SemanticCompose *)( extraComposeFunctions[ TOPCOMPOSE1 ] )[ iPos ] );
    }

    ( extraCompose[ TOPCOMPOSE1 ].back() )[ whichRow ]->init( initialTime, create );
  }
  else
    intervalTopCompose1[ whichRow ].init( initialTime, create );
}

bool KSingleWindow::setLevelFunction( TWindowLevel whichLevel,
                                      const string& whichFunction )
{
  if( whichLevel == DERIVED )
    return false;

  if( functions[ whichLevel ] != nullptr )
    delete functions[ whichLevel ];

  functions[ whichLevel ] =
    ( FunctionManagement<SemanticFunction>::getInstance() )->
    getFunction( whichFunction );

  if( functions[ whichLevel ] == nullptr )
    return false;

  return true;
}


string KSingleWindow::getLevelFunction( TWindowLevel whichLevel ) const
{
  return functions[ whichLevel ]->getName();
}


SemanticFunction *KSingleWindow::getSemanticFunction( TWindowLevel whichLevel )
{
  return functions[ whichLevel ];
}


string KSingleWindow::getFirstUsefulFunction()
{
  if( typeid( *functions[ TOPCOMPOSE1 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE1 ]->getName();
  if( typeid( *functions[ TOPCOMPOSE2 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE2 ]->getName();
  if( typeid( *functions[ getComposeLevel( getLevel() ) ] ) != typeid( ComposeAsIs ) )
    return functions[ getComposeLevel( getLevel() )]->getName();

  return functions[ static_cast<TWindowLevel>( getLevel() ) ]->getName();
}

TWindowLevel KSingleWindow::getFirstFreeCompose() const
{
  if( typeid( *functions[ getComposeLevel( getLevel() )] ) == typeid( ComposeAsIs ) )
    return getComposeLevel( getLevel() );
  if( typeid( *functions[ TOPCOMPOSE2 ] ) == typeid( ComposeAsIs ) )
    return TOPCOMPOSE2;
  return TOPCOMPOSE1;
}

SemanticFunction *KSingleWindow::getFirstSemUsefulFunction()
{
  if( typeid( *functions[ TOPCOMPOSE1 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE1 ];
  if( typeid( *functions[ TOPCOMPOSE2 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE2 ];
  if( typeid( *functions[ getComposeLevel( getLevel() ) ] ) != typeid( ComposeAsIs ) )
    return functions[ getComposeLevel( getLevel() ) ];

  return functions[ static_cast<TWindowLevel>( getLevel() ) ];
}


void KSingleWindow::setFunctionParam( TWindowLevel whichLevel,
                                      TParamIndex whichParam,
                                      const TParamValue& newValue )
{
  functions[ whichLevel ]->setParam( whichParam, newValue );
}

TParamIndex KSingleWindow::getFunctionNumParam( TWindowLevel whichLevel ) const
{
  return functions[ whichLevel ]->getMaxParam();
}

TParamValue KSingleWindow::getFunctionParam( TWindowLevel whichLevel,
    TParamIndex whichParam ) const
{
  return functions[ whichLevel ]->getParam( whichParam );
}

string KSingleWindow::getFunctionParamName( TWindowLevel whichLevel,
    TParamIndex whichParam ) const
{
  return functions[ whichLevel ]->getParamName( whichParam );
}

// Extra composes
void KSingleWindow::addExtraCompose( TWindowLevel whichLevel )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= COMPOSECPU ) )
    return; // Should throw an Exception

  vector< vector< IntervalCompose * > >& vCompose = extraCompose[ whichLevel ];
  vector<IntervalCompose *> tmpV;
  if( myTrace->totalThreads() > myTrace->totalCPUs() )
  {
    tmpV.reserve( myTrace->totalThreads() );
    for( TThreadOrder i = 0; i < myTrace->totalThreads(); ++i )
    {
      IntervalCompose *tmpInterval = new IntervalCompose( this, TOPCOMPOSE1, i );
      tmpV.push_back( tmpInterval );
      tmpInterval->setNotWindowInits( true );
      if( extraCompose[ whichLevel ].size() > 0 )
        tmpInterval->setCustomChild( ( extraCompose[ whichLevel ].back() )[ i ] );
      else
        tmpInterval->setCustomChild( getLevelInterval( TOPCOMPOSE1, i ) );
    }
  }
  else
  {
    tmpV.reserve( myTrace->totalCPUs() );
    for( TCPUOrder i = 0; i < myTrace->totalCPUs(); ++i )
    {
      IntervalCompose *tmpInterval = new IntervalCompose( this, TOPCOMPOSE1, i );
      tmpV.push_back( tmpInterval );
      tmpInterval->setNotWindowInits( true );
      if( extraCompose[ whichLevel ].size() > 0 )
        tmpInterval->setCustomChild( ( extraCompose[ whichLevel ].back() )[ i ] );
      else
        tmpInterval->setCustomChild( getLevelInterval( TOPCOMPOSE1, i ) );
    }
  }
  vCompose.push_back( tmpV );

  vector< SemanticFunction * >& vComposeFunction = extraComposeFunctions[ whichLevel ];
  vComposeFunction.push_back( new ComposeAsIs() );
}

void KSingleWindow::removeExtraCompose( TWindowLevel whichLevel )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= COMPOSECPU ) )
    return; // Should throw an Exception

  if( extraCompose[ whichLevel ].size() > 0 )
  {
    for( vector< IntervalCompose * >::iterator it = extraCompose[ whichLevel ].back().begin();
         it != extraCompose[ whichLevel ].back().end(); ++it )
      delete *it;
    extraCompose[ whichLevel ].pop_back();
  }

  if( extraComposeFunctions[ whichLevel ].size() > 0 )
  {
    delete extraComposeFunctions[ whichLevel ].back();
    extraComposeFunctions[ whichLevel ].pop_back();
  }
}

bool KSingleWindow::setExtraLevelFunction( TWindowLevel whichLevel,
                                           size_t whichPosition,
                                           const string& whichFunction )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= COMPOSECPU ) )
    return false; // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return false;

  if( it->second.size() <= whichPosition )
    return false;

  delete it->second[ whichPosition ];

  it->second[ whichPosition ] = ( FunctionManagement<SemanticFunction>::getInstance() )->getFunction( whichFunction );

  if( it->second[ whichPosition ] == nullptr )
    return false;

  return true;
}

string KSingleWindow::getExtraLevelFunction( TWindowLevel whichLevel,
                                             size_t whichPosition )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= COMPOSECPU ) )
    return ""; // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return "";

  if( it->second.size() <= whichPosition )
    return "";

  return it->second[ whichPosition ]->getName();
}

void KSingleWindow::setExtraFunctionParam( TWindowLevel whichLevel,
                                           size_t whichPosition,
                                           TParamIndex whichParam,
                                           const TParamValue& newValue )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= COMPOSECPU ) )
    return; // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return;

  if( it->second.size() <= whichPosition )
    return;

  it->second[ whichPosition ]->setParam( whichParam, newValue );
}

TParamIndex KSingleWindow::getExtraFunctionNumParam( TWindowLevel whichLevel, size_t whichPosition ) const
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= COMPOSECPU ) )
    return 0; // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::const_iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return 0;

  if( it->second.size() <= whichPosition )
    return 0;

  return it->second[ whichPosition ]->getMaxParam();
}

TParamValue KSingleWindow::getExtraFunctionParam( TWindowLevel whichLevel,
                                                  size_t whichPosition,
                                                  TParamIndex whichParam ) const
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= COMPOSECPU ) )
    return std::vector<double>(); // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::const_iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return std::vector<double>();

  if( it->second.size() <= whichPosition )
    return std::vector<double>();

  return it->second[ whichPosition ]->getParam( whichParam );
}

string KSingleWindow::getExtraFunctionParamName( TWindowLevel whichLevel,
                                                 size_t whichPosition,
                                                 TParamIndex whichParam ) const
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= COMPOSECPU ) )
    return ""; // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::const_iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return "";

  if( it->second.size() <= whichPosition )
    return "";

  return it->second[ whichPosition ]->getParamName( whichParam );
}


bool KSingleWindow::initFromBegin() const
{
  bool tmp = false;

  map< TWindowLevel, vector<SemanticFunction *> >::const_iterator itExtra = extraComposeFunctions.find( TOPCOMPOSE1 );
  if( itExtra != extraComposeFunctions.end() )
  {
    for( vector<SemanticFunction *>::const_iterator it = itExtra->second.begin();
         it != itExtra->second.end(); ++it )
      tmp = tmp || (*it)->getInitFromBegin();
  }

  tmp = tmp || functions[ TOPCOMPOSE1 ]->getInitFromBegin();
  tmp = tmp || functions[ TOPCOMPOSE2 ]->getInitFromBegin();

  if( level == TTraceLevel::WORKLOAD )
  {
    tmp = tmp || functions[ COMPOSEWORKLOAD ]->getInitFromBegin();
    tmp = tmp || functions[ WORKLOAD ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSEAPPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ APPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::APPLICATION )
  {
    tmp = tmp || functions[ COMPOSEAPPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ APPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::TASK )
  {
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::SYSTEM )
  {
    tmp = tmp || functions[ COMPOSESYSTEM ]->getInitFromBegin();
    tmp = tmp || functions[ SYSTEM ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSENODE ]->getInitFromBegin();
    tmp = tmp || functions[ NODE ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::NODE )
  {
    tmp = tmp || functions[ COMPOSENODE ]->getInitFromBegin();
    tmp = tmp || functions[ NODE ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::CPU )
  {
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }

  tmp = tmp || functions[ COMPOSETHREAD ]->getInitFromBegin();
  tmp = tmp || functions[ THREAD ]->getInitFromBegin();

  return tmp;
}


RecordList *KSingleWindow::calcNext( TObjectOrder whichObject, bool updateLimits )
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->calcNext();
  }

  return intervalTopCompose1[ whichObject ].calcNext();
}


RecordList *KSingleWindow::calcPrev( TObjectOrder whichObject, bool updateLimits )
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->calcPrev();
  }

  return intervalTopCompose1[ whichObject ].calcPrev();
}


TRecordTime KSingleWindow::getBeginTime( TObjectOrder whichObject ) const
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->getBeginTime();
  }

  return intervalTopCompose1[ whichObject ].getBeginTime();
}


TRecordTime KSingleWindow::getEndTime( TObjectOrder whichObject ) const
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->getEndTime();
  }

  return intervalTopCompose1[ whichObject ].getEndTime();
}


TSemanticValue KSingleWindow::getValue( TObjectOrder whichObject ) const
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->getValue();
  }

  return intervalTopCompose1[ whichObject ].getValue();
}


Interval *KSingleWindow::getLevelInterval( TWindowLevel whichLevel,
                                           TObjectOrder whichOrder,
                                           bool includeExtraCompose )
{
  if( whichLevel == TOPCOMPOSE1 )
  {
    if( includeExtraCompose && extraCompose[ whichLevel ].size() > 0 )
      return ( extraCompose[ whichLevel ].back() )[ whichOrder ];
    return &intervalTopCompose1[ whichOrder ];
  }
  else if( whichLevel == TOPCOMPOSE2 )
    return &intervalTopCompose2[ whichOrder ];
  else if( whichLevel == COMPOSEWORKLOAD )
    return &intervalComposeWorkload[ whichOrder ];
  else if( whichLevel == WORKLOAD )
    return &intervalWorkload[ whichOrder ];
  else if( whichLevel == COMPOSEAPPLICATION )
    return &intervalComposeApplication[ whichOrder ];
  else if( whichLevel == APPLICATION )
    return &intervalApplication[ whichOrder ];
  else if( whichLevel == COMPOSETASK )
    return &intervalComposeTask[ whichOrder ];
  else if( whichLevel == TASK )
    return &intervalTask[ whichOrder ];
  else if( whichLevel == COMPOSETHREAD )
    return &intervalComposeThread[ whichOrder ];
  else if( whichLevel == THREAD )
    return &intervalThread[ whichOrder ];
  else if( whichLevel == COMPOSESYSTEM )
    return &intervalComposeSystem[ whichOrder ];
  else if( whichLevel == SYSTEM )
    return &intervalSystem[ whichOrder ];
  else if( whichLevel == COMPOSENODE )
    return &intervalComposeNode[ whichOrder ];
  else if( whichLevel == NODE )
    return &intervalNode[ whichOrder ];
  else if( whichLevel == COMPOSECPU )
    return &intervalComposeCPU[ whichOrder ];
  else if( whichLevel == CPU )
    return &intervalCPU[ whichOrder ];
  return nullptr;
}

SemanticInfoType KSingleWindow::getSemanticInfoType() const
{
  map< TWindowLevel, vector<SemanticFunction *> >::const_iterator itMap = extraComposeFunctions.find( TOPCOMPOSE1 );
  if( itMap != extraComposeFunctions.end() )
  {
    for( vector<SemanticFunction *>::const_reverse_iterator it = itMap->second.rbegin();
         it != itMap->second.rend(); ++it )
    {
      if( (*it)->getSemanticInfoType() != SAME_TYPE )
        return (*it)->getSemanticInfoType();
    }
  }

  if( functions[ TOPCOMPOSE1 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ TOPCOMPOSE1 ]->getSemanticInfoType();
  if( functions[ TOPCOMPOSE2 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ TOPCOMPOSE2 ]->getSemanticInfoType();

  if( level >= TTraceLevel::SYSTEM && level <= TTraceLevel::CPU )
  {
    switch( level )
    {
      case TTraceLevel::SYSTEM:
        if( functions[ COMPOSESYSTEM ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSESYSTEM ]->getSemanticInfoType();
        if( functions[ SYSTEM ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ SYSTEM ]->getSemanticInfoType();
      case TTraceLevel::NODE:
        if( functions[ COMPOSENODE ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSENODE ]->getSemanticInfoType();
        if( functions[ NODE ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ NODE ]->getSemanticInfoType();
      case TTraceLevel::CPU:
        if( functions[ COMPOSECPU ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSECPU ]->getSemanticInfoType();
        if( functions[ CPU ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ CPU ]->getSemanticInfoType();
      default:
        break;
    }
  }
  else if( level >= TTraceLevel::WORKLOAD && level <= TTraceLevel::THREAD )
  {
    switch( level )
    {
      case TTraceLevel::WORKLOAD:
        if( functions[ COMPOSEWORKLOAD ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSEWORKLOAD ]->getSemanticInfoType();
        if( functions[ WORKLOAD ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ WORKLOAD ]->getSemanticInfoType();
      case TTraceLevel::APPLICATION:
        if( functions[ COMPOSEAPPLICATION ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSEAPPLICATION ]->getSemanticInfoType();
        if( functions[ APPLICATION ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ APPLICATION ]->getSemanticInfoType();
      case TTraceLevel::TASK:
        if( functions[ COMPOSETASK ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSETASK ]->getSemanticInfoType();
        if( functions[ TASK ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ TASK ]->getSemanticInfoType();
      default:
        break;
    }
  }
  if( functions[ COMPOSETHREAD ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ COMPOSETHREAD ]->getSemanticInfoType();
  return functions[ THREAD ]->getSemanticInfoType();
}


KTimeline *KSingleWindow::clone( bool recursiveClone )
{
  KSingleWindow *clonedKSWindow = new KSingleWindow( myTrace );

  clonedKSWindow->level = level;
  clonedKSWindow->timeUnit = timeUnit;

  for( map< TWindowLevel, vector< vector< IntervalCompose * > > >::iterator itMap = extraCompose.begin();
       itMap != extraCompose.end(); ++itMap )
  {
    for( size_t i = 0; i < itMap->second.size(); ++i )
      clonedKSWindow->addExtraCompose( itMap->first );
  }

  for( map< TWindowLevel, vector< SemanticFunction * > >::iterator itMap = extraComposeFunctions.begin();
       itMap != extraComposeFunctions.end(); ++itMap )
  {
    for( size_t i = 0; i < itMap->second.size(); ++i )
    {
      delete ( clonedKSWindow->extraComposeFunctions[ itMap->first ] )[ i ];
      ( clonedKSWindow->extraComposeFunctions[ itMap->first ] )[ i ] = ( extraComposeFunctions[ itMap->first ] )[ i ]->clone();
    }
  }

  for( size_t iPos = 0; iPos < clonedKSWindow->extraCompose[ TOPCOMPOSE1 ].size(); ++iPos )
  {
    for( size_t iRow = 0; iRow < ( clonedKSWindow->extraCompose[ TOPCOMPOSE1 ] )[ iPos ].size(); ++iRow )
      ( ( clonedKSWindow->extraCompose[ TOPCOMPOSE1 ] )[ iPos ] )[ iRow ]->setSemanticFunction( (SemanticCompose *)( clonedKSWindow->extraComposeFunctions[ TOPCOMPOSE1 ] )[ iPos ] );
  }

  for( int i = 0; i < COMPOSECPU + 1; ++i )
  {
    if( functions[ i ] != nullptr )
    { 
      delete clonedKSWindow->functions[ i ];
      clonedKSWindow->functions[ i ] = functions[ i ]->clone();
    }
    else
      clonedKSWindow->functions[ i ] = nullptr;
  }

  delete clonedKSWindow->myFilter;
  clonedKSWindow->myFilter = myFilter->clone( clonedKSWindow );

  clonedKSWindow->recordsByTimeCPU.clear();
  for( vector<MemoryTrace::iterator *>::const_iterator it = recordsByTimeCPU.begin();
       it != recordsByTimeCPU.end(); ++it )
  {
    if( *it != nullptr )
      clonedKSWindow->recordsByTimeCPU.push_back( (*it)->clone() );
    else
      clonedKSWindow->recordsByTimeCPU.push_back( nullptr );
  }

  clonedKSWindow->recordsByTimeThread.clear();
  for( vector<MemoryTrace::iterator *>::const_iterator it = recordsByTimeThread.begin();
       it != recordsByTimeThread.end(); ++it )
  {
    if( *it != nullptr )
      clonedKSWindow->recordsByTimeThread.push_back( (*it)->clone() );
    else
      clonedKSWindow->recordsByTimeThread.push_back( nullptr );
  }

  return clonedKSWindow;
}


void KSingleWindow::initSemanticFunctions()
{
  functions[ 0 ] = nullptr;

  functions[ TOPCOMPOSE1 ] = new ComposeAsIs();
  functions[ TOPCOMPOSE2 ] = new ComposeAsIs();

  functions[ COMPOSEWORKLOAD ] = new ComposeAsIs();
  functions[ WORKLOAD ] = new Adding();

  functions[ COMPOSEAPPLICATION ] = new ComposeAsIs();
  functions[ APPLICATION ] = new Adding();

  functions[ COMPOSETASK ] = new ComposeAsIs();
  functions[ TASK ] = new Adding();

  functions[ COMPOSETHREAD ] = new ComposeAsIs();
  functions[ THREAD ] = new StateAsIs();

  functions[ COMPOSESYSTEM ] = new ComposeAsIs();
  functions[ SYSTEM ] = new Adding();

  functions[ COMPOSENODE ] = new ComposeAsIs();
  functions[ NODE ] = new Adding();

  functions[ COMPOSECPU ] = new ComposeAsIs();
  functions[ CPU ] = new ActiveThread();
}

/**********************************************************************
 *  KDerivedWindow implementation
 **********************************************************************/

KDerivedWindow::~KDerivedWindow()
{
  for( PRV_UINT32 twlIdx = (PRV_UINT32)WORKLOAD; twlIdx <= (PRV_UINT32)DERIVED; ++twlIdx )
  {
    if( functions[ (TWindowLevel)twlIdx ] != nullptr )
      delete functions[ (TWindowLevel)twlIdx ];
  }
}

void KDerivedWindow::setup( KTrace* whichTrace )
{
  if( whichTrace == nullptr )
  {
    if( ( ( KTrace* )parents[ 0 ]->getTrace() )->getEndTime() >= ( ( KTrace* )parents[ 1 ]->getTrace() )->getEndTime() )
      myTrace = ( KTrace* )parents[ 0 ]->getTrace();
    else
      myTrace = ( KTrace* )parents[ 1 ]->getTrace();
  }
  else
    myTrace = whichTrace;

  level = getMinAcceptableLevel();

  intervalTopCompose1.clear();
  intervalTopCompose2.clear();
  intervalDerived.clear();
  intervalControlDerived.clear();
  if( myTrace->totalThreads() > myTrace->totalCPUs() )
  {
    intervalTopCompose1.reserve( myTrace->totalThreads() );
    intervalTopCompose2.reserve( myTrace->totalThreads() );
    intervalDerived.reserve( myTrace->totalThreads() );
    intervalControlDerived.reserve( myTrace->totalThreads() );
    for( TThreadOrder i = 0; i < myTrace->totalThreads(); ++i )
    {
      intervalTopCompose1.push_back( IntervalCompose( this, TOPCOMPOSE1, i ) );
      intervalTopCompose2.push_back( IntervalCompose( this, TOPCOMPOSE2, i ) );
      intervalDerived.push_back( IntervalDerived( this, DERIVED, i ) );
      intervalControlDerived.push_back( IntervalControlDerived( this, DERIVED, i ) );
    }
  }
  else
  {
    intervalTopCompose1.reserve( myTrace->totalCPUs() );
    intervalTopCompose2.reserve( myTrace->totalCPUs() );
    intervalDerived.reserve( myTrace->totalCPUs() );
    intervalControlDerived.reserve( myTrace->totalCPUs() );
    for( TCPUOrder i = 0; i < myTrace->totalCPUs(); ++i )
    {
      intervalTopCompose1.push_back( IntervalCompose( this, TOPCOMPOSE1, i ) );
      intervalTopCompose2.push_back( IntervalCompose( this, TOPCOMPOSE2, i ) );
      intervalDerived.push_back( IntervalDerived( this, DERIVED, i ) );
      intervalControlDerived.push_back( IntervalControlDerived( this, DERIVED, i ) );
    }
  }

  intervalWorkload.clear();
  intervalComposeWorkload.clear();
  intervalWorkload.push_back( IntervalNotThread( this, WORKLOAD, 0 ) );
  intervalComposeWorkload.push_back( IntervalCompose( this, COMPOSEWORKLOAD, 0 ) );

  intervalApplication.clear();
  intervalComposeApplication.clear();
  intervalApplication.reserve( myTrace->totalApplications() );
  intervalComposeApplication.reserve( myTrace->totalApplications() );
  for( TApplOrder i = 0; i < myTrace->totalApplications(); i++ )
  {
    intervalApplication.push_back( IntervalNotThread( this, APPLICATION, i ) );
    intervalComposeApplication.push_back( IntervalCompose( this, COMPOSEAPPLICATION, i ) );
  }

  intervalTask.clear();
  intervalComposeTask.clear();
  intervalTask.reserve( myTrace->totalTasks() );
  intervalComposeTask.reserve( myTrace->totalTasks() );
  for( TTaskOrder i = 0; i < myTrace->totalTasks(); ++i )
  {
    intervalTask.push_back( IntervalNotThread( this, TASK, i ) );
    intervalComposeTask.push_back( IntervalCompose( this, COMPOSETASK, i ) );
  }

  intervalComposeThread.clear();
  intervalComposeThread.reserve( myTrace->totalThreads() );
  for( TThreadOrder i = 0; i < myTrace->totalThreads(); ++i )
    intervalComposeThread.push_back( IntervalCompose( this, COMPOSETHREAD, i ) );

  intervalSystem.clear();
  intervalComposeSystem.clear();
  intervalSystem.push_back( IntervalNotThread( this, SYSTEM, 0 ) );
  intervalComposeSystem.push_back( IntervalCompose( this, COMPOSESYSTEM, 0 ) );

  intervalNode.clear();
  intervalComposeNode.clear();
  intervalNode.reserve( myTrace->totalNodes() );
  intervalComposeNode.reserve( myTrace->totalNodes() );
  for( TNodeOrder i = 0; i < myTrace->totalNodes(); ++i )
  {
    intervalNode.push_back( IntervalNotThread( this, NODE, i ) );
    intervalComposeNode.push_back( IntervalCompose( this, COMPOSENODE, i ) );
  }

  intervalComposeCPU.clear();
  intervalComposeCPU.reserve( myTrace->totalCPUs() );
  for( TCPUOrder i = 0; i < myTrace->totalCPUs(); ++i )
    intervalComposeCPU.push_back( IntervalCompose( this, COMPOSECPU, i ) );
}


bool KDerivedWindow::setLevelFunction( TWindowLevel whichLevel,
                                       const string& whichFunction )
{
  if( functions[ whichLevel ] != nullptr )
    delete functions[ whichLevel ];

  functions[ whichLevel ] =
    ( FunctionManagement<SemanticFunction>::getInstance() )->
    getFunction( whichFunction );

  if( functions[ whichLevel ] == nullptr )
    return false;

  return true;
}


string KDerivedWindow::getLevelFunction( TWindowLevel whichLevel ) const
{
  return functions[ whichLevel ]->getName();
}

SemanticFunction *KDerivedWindow::getSemanticFunction( TWindowLevel whichLevel )
{
  return functions[ whichLevel ];
}

string KDerivedWindow::getFirstUsefulFunction()
{
  if( typeid( *functions[ TOPCOMPOSE1 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE1 ]->getName();
  if( typeid( *functions[ TOPCOMPOSE2 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE2 ]->getName();
  if( typeid( *functions[ getComposeLevel( getLevel() )] ) != typeid( ComposeAsIs ) )
    return functions[ getComposeLevel( getLevel()  )]->getName();

  if( getLevel() == getMinAcceptableLevel() )
    return functions[ DERIVED ]->getName();

  return functions[ static_cast<TWindowLevel>( getLevel() ) ]->getName();
}

TWindowLevel KDerivedWindow::getFirstFreeCompose() const
{
  if( typeid( *functions[ getComposeLevel( getLevel() )] ) == typeid( ComposeAsIs ) )
    return getComposeLevel( getLevel()  );
  if( typeid( *functions[ TOPCOMPOSE2 ] ) == typeid( ComposeAsIs ) )
    return TOPCOMPOSE2;
  return TOPCOMPOSE1;
}

SemanticFunction *KDerivedWindow::getFirstSemUsefulFunction()
{
  if( typeid( *functions[ TOPCOMPOSE1 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE1 ];
  if( typeid( *functions[ TOPCOMPOSE2 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE2 ];
  if( typeid( *functions[ getComposeLevel( getLevel()  ) ] ) != typeid( ComposeAsIs ) )
    return functions[ getComposeLevel( getLevel()  ) ];

  if( getLevel() == getMinAcceptableLevel() )
    return functions[ DERIVED ];

  return functions[ static_cast<TWindowLevel>( getLevel() ) ];
}

void KDerivedWindow::setFunctionParam( TWindowLevel whichLevel,
                                       TParamIndex whichParam,
                                       const TParamValue& newValue )
{
  functions[ whichLevel ]->setParam( whichParam, newValue );
}

TParamIndex KDerivedWindow::getFunctionNumParam( TWindowLevel whichLevel ) const
{
  return functions[ whichLevel ]->getMaxParam();
}

TParamValue KDerivedWindow::getFunctionParam( TWindowLevel whichLevel,
    TParamIndex whichParam ) const
{
  return functions[ whichLevel ]->getParam( whichParam );
}

string KDerivedWindow::getFunctionParamName( TWindowLevel whichLevel,
    TParamIndex whichParam ) const
{
  return functions[ whichLevel ]->getParamName( whichParam );
}

// Extra composes
void KDerivedWindow::addExtraCompose( TWindowLevel whichLevel )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= DERIVED ) )
    return; // Should throw an Exception

  vector< vector< IntervalCompose * > >& vCompose = extraCompose[ whichLevel ];
  vector<IntervalCompose *> tmpV;
  if( myTrace->totalThreads() > myTrace->totalCPUs() )
  {
    tmpV.reserve( myTrace->totalThreads() );
    for( TThreadOrder i = 0; i < myTrace->totalThreads(); ++i )
    {
      IntervalCompose *tmpInterval = new IntervalCompose( this, TOPCOMPOSE1, i );
      tmpV.push_back( tmpInterval );
      tmpInterval->setNotWindowInits( true );
      if( extraCompose[ whichLevel ].size() > 0 )
        tmpInterval->setCustomChild( ( extraCompose[ whichLevel ].back() )[ i ] );
      else
        tmpInterval->setCustomChild( getLevelInterval( TOPCOMPOSE1, i ) );
    }
  }
  else
  {
    tmpV.reserve( myTrace->totalCPUs() );
    for( TCPUOrder i = 0; i < myTrace->totalCPUs(); ++i )
    {
      IntervalCompose *tmpInterval = new IntervalCompose( this, TOPCOMPOSE1, i );
      tmpV.push_back( tmpInterval );
      tmpInterval->setNotWindowInits( true );
      if( extraCompose[ whichLevel ].size() > 0 )
        tmpInterval->setCustomChild( ( extraCompose[ whichLevel ].back() )[ i ] );
      else
        tmpInterval->setCustomChild( getLevelInterval( TOPCOMPOSE1, i ) );
    }
  }
  vCompose.push_back( tmpV );

  vector< SemanticFunction * >& vComposeFunction = extraComposeFunctions[ whichLevel ];
  vComposeFunction.push_back( new ComposeAsIs() );
}

void KDerivedWindow::removeExtraCompose( TWindowLevel whichLevel )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= DERIVED ) )
    return; // Should throw an Exception

  if( extraCompose[ whichLevel ].size() > 0 )
  {
    for( vector< IntervalCompose * >::iterator it = extraCompose[ whichLevel ].back().begin();
         it != extraCompose[ whichLevel ].back().end(); ++it )
      delete *it;
    extraCompose[ whichLevel ].pop_back();
  }

  if( extraComposeFunctions[ whichLevel ].size() > 0 )
  {
    delete extraComposeFunctions[ whichLevel ].back();
    extraComposeFunctions[ whichLevel ].pop_back();
  }
}

bool KDerivedWindow::setExtraLevelFunction( TWindowLevel whichLevel,
                                            size_t whichPosition,
                                            const string& whichFunction )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= DERIVED ) )
    return false; // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return false;

  if( it->second.size() <= whichPosition )
    return false;

  delete it->second[ whichPosition ];

  it->second[ whichPosition ] = ( FunctionManagement<SemanticFunction>::getInstance() )->getFunction( whichFunction );

  if( it->second[ whichPosition ] == nullptr )
    return false;

  return true;
}

string KDerivedWindow::getExtraLevelFunction( TWindowLevel whichLevel,
                                              size_t whichPosition )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= DERIVED ) )
    return ""; // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return "";

  if( it->second.size() <= whichPosition )
    return "";

  return it->second[ whichPosition ]->getName();
}

void KDerivedWindow::setExtraFunctionParam( TWindowLevel whichLevel,
                                            size_t whichPosition,
                                            TParamIndex whichParam,
                                            const TParamValue& newValue )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= DERIVED ) )
    return; // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return;

  if( it->second.size() <= whichPosition )
    return;

  it->second[ whichPosition ]->setParam( whichParam, newValue );
}

TParamIndex KDerivedWindow::getExtraFunctionNumParam( TWindowLevel whichLevel, size_t whichPosition ) const
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= DERIVED ) )
    return 0; // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::const_iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return 0;

  if( it->second.size() <= whichPosition )
    return 0;

  return it->second[ whichPosition ]->getMaxParam();
}

TParamValue KDerivedWindow::getExtraFunctionParam( TWindowLevel whichLevel,
                                                  size_t whichPosition,
                                                  TParamIndex whichParam ) const
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= DERIVED ) )
    return std::vector<double>(); // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::const_iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return std::vector<double>();

  if( it->second.size() <= whichPosition )
    return std::vector<double>();

  return it->second[ whichPosition ]->getParam( whichParam );
}

string KDerivedWindow::getExtraFunctionParamName( TWindowLevel whichLevel,
                                                 size_t whichPosition,
                                                 TParamIndex whichParam ) const
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= DERIVED ) )
    return ""; // Should throw an Exception

  map< TWindowLevel, vector< SemanticFunction * > >::const_iterator it = extraComposeFunctions.find( whichLevel );
  if( it == extraComposeFunctions.end() )
    return "";

  if( it->second.size() <= whichPosition )
    return "";

  return it->second[ whichPosition ]->getParamName( whichParam );
}


bool KDerivedWindow::initFromBegin() const
{
  bool tmp = false;

  if( getShift( 0 ) != 0 || getShift( 1 ) != 0 )
    return true;

  map< TWindowLevel, vector<SemanticFunction *> >::const_iterator itExtra = extraComposeFunctions.find( TOPCOMPOSE1 );
  if( itExtra != extraComposeFunctions.end() )
  {
    for( vector<SemanticFunction *>::const_iterator it = itExtra->second.begin();
         it != itExtra->second.end(); ++it )
      tmp = tmp || (*it)->getInitFromBegin();
  }

  tmp = tmp || functions[ TOPCOMPOSE1 ]->getInitFromBegin();
  tmp = tmp || functions[ TOPCOMPOSE2 ]->getInitFromBegin();

  if( level == TTraceLevel::WORKLOAD )
  {
    tmp = tmp || functions[ COMPOSEWORKLOAD ]->getInitFromBegin();
    tmp = tmp || functions[ WORKLOAD ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSEAPPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ APPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::APPLICATION )
  {
    tmp = tmp || functions[ COMPOSEAPPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ APPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::TASK )
  {
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::SYSTEM )
  {
    tmp = tmp || functions[ COMPOSESYSTEM ]->getInitFromBegin();
    tmp = tmp || functions[ SYSTEM ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSENODE ]->getInitFromBegin();
    tmp = tmp || functions[ NODE ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::NODE )
  {
    tmp = tmp || functions[ COMPOSENODE ]->getInitFromBegin();
    tmp = tmp || functions[ NODE ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::CPU )
  {
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
  }
  else if( level == TTraceLevel::THREAD )
    tmp = tmp || functions[ COMPOSETHREAD ]->getInitFromBegin();

  tmp = tmp || functions[ DERIVED ]->getInitFromBegin();

  return tmp;
}


void KDerivedWindow::init( TRecordTime initialTime, TCreateList create, bool updateLimits )
{
/*  TObjectOrder objectSize = 0;
  TWindowLevel tmpLevel = getLevel();
*/
  for( PRV_UINT8 i = WORKLOAD; i <= DERIVED; ++i )
  {
    if( functions[ i ] != nullptr )
      functions[ i ]->init( this );
  }

  for( map< TWindowLevel, vector< SemanticFunction * > >::iterator itMap = extraComposeFunctions.begin();
       itMap != extraComposeFunctions.end(); ++itMap )
  {
    for( vector<SemanticFunction *>::iterator it = itMap->second.begin();
         it != itMap->second.end(); ++it )
      (*it)->init( this );
  }

  if( extraCompose[ TOPCOMPOSE1 ].size() > 0 )
  {
    for( size_t iPos = 0; iPos < extraCompose[ TOPCOMPOSE1 ].size(); ++iPos )
    {
      for( size_t iRow = 0; iRow < ( extraCompose[ TOPCOMPOSE1 ] )[ iPos ].size(); ++iRow )
        ( ( extraCompose[ TOPCOMPOSE1 ] )[ iPos ] )[ iRow ]->setSemanticFunction( (SemanticCompose *)( extraComposeFunctions[ TOPCOMPOSE1 ] )[ iPos ] );
    }
  }

/*  if( tmpLevel == WORKLOAD )
    objectSize = 1;
  else if( tmpLevel == APPLICATION )
    objectSize = myTrace->totalApplications();
  else if( tmpLevel == TASK )
    objectSize = myTrace->totalTasks();
  else if( tmpLevel == THREAD )
    objectSize = myTrace->totalThreads();
  else if( tmpLevel == SYSTEM )
    objectSize = 1;
  else if( tmpLevel == NODE )
    objectSize = myTrace->totalNodes();
  else if( tmpLevel == CPU )
    objectSize = myTrace->totalCPUs();
*/
  if( initFromBegin() )
    initialTime = 0;

  for( PRV_UINT16 i = 0; i < parents.size(); i++ )
    parents[ i ]->init( initialTime, create );
}

void KDerivedWindow::initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits )
{
  if( extraCompose[ TOPCOMPOSE1 ].size() > 0 )
  {
    for( size_t iPos = 0; iPos < extraCompose[ TOPCOMPOSE1 ].size(); ++iPos )
    {
      ( ( extraCompose[ TOPCOMPOSE1 ] )[ iPos ] )[ whichRow ]->setSemanticFunction( (SemanticCompose *)( extraComposeFunctions[ TOPCOMPOSE1 ] )[ iPos ] );
    }

    ( extraCompose[ TOPCOMPOSE1 ].back() )[ whichRow ]->init( initialTime, create );
  }
  else
    intervalTopCompose1[ whichRow ].init( initialTime, create );
}

RecordList *KDerivedWindow::calcNext( TObjectOrder whichObject, bool updateLimits )
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->calcNext();
  }

  return intervalTopCompose1[ whichObject ].calcNext();
}


RecordList *KDerivedWindow::calcPrev( TObjectOrder whichObject, bool updateLimits )
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->calcPrev();
  }

  return intervalTopCompose1[ whichObject ].calcPrev();
}


TRecordTime KDerivedWindow::getBeginTime( TObjectOrder whichObject ) const
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->getBeginTime();
  }

  return intervalTopCompose1[ whichObject ].getBeginTime();
}


TRecordTime KDerivedWindow::getEndTime( TObjectOrder whichObject ) const
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->getEndTime();
  }

  return intervalTopCompose1[ whichObject ].getEndTime();
}


TSemanticValue KDerivedWindow::getValue( TObjectOrder whichObject ) const
{
  map< TWindowLevel, vector< vector<IntervalCompose *> > >::const_iterator itExtra = extraCompose.find( TOPCOMPOSE1 );
  if( itExtra != extraCompose.end() )
  {
    if( itExtra->second.size() > 0 )
      return ( itExtra->second.back() )[ whichObject ]->getValue();
  }

  return intervalTopCompose1[ whichObject ].getValue();
}


Interval *KDerivedWindow::getLevelInterval( TWindowLevel whichLevel,
                                            TObjectOrder whichOrder,
                                            bool includeExtraCompose )
{
  if( whichLevel == static_cast<TWindowLevel>( getMinAcceptableLevel() ) )
    whichLevel = DERIVED;

  if( whichLevel == TOPCOMPOSE1 )
  {
    if( includeExtraCompose && extraCompose[ whichLevel ].size() > 0 )
      return ( extraCompose[ whichLevel ].back() )[ whichOrder ];
    return &intervalTopCompose1[ whichOrder ];
  }
  else if( whichLevel == TOPCOMPOSE2 )
    return &intervalTopCompose2[ whichOrder ];
  else if( whichLevel == COMPOSEWORKLOAD )
    return &intervalComposeWorkload[ whichOrder ];
  else if( whichLevel == WORKLOAD )
    return &intervalWorkload[ whichOrder ];
  else if( whichLevel == COMPOSEAPPLICATION )
    return &intervalComposeApplication[ whichOrder ];
  else if( whichLevel == APPLICATION )
    return &intervalApplication[ whichOrder ];
  else if( whichLevel == COMPOSETASK )
    return &intervalComposeTask[ whichOrder ];
  else if( whichLevel == TASK )
    return &intervalTask[ whichOrder ];
  else if( whichLevel == COMPOSETHREAD )
    return &intervalComposeThread[ whichOrder ];
  else if( whichLevel == COMPOSESYSTEM )
    return &intervalComposeSystem[ whichOrder ];
  else if( whichLevel == SYSTEM )
    return &intervalSystem[ whichOrder ];
  else if( whichLevel == COMPOSENODE )
    return &intervalComposeNode[ whichOrder ];
  else if( whichLevel == NODE )
    return &intervalNode[ whichOrder ];
  else if( whichLevel == COMPOSECPU )
    return &intervalComposeCPU[ whichOrder ];
  else if( whichLevel == DERIVED )
  {
    if( ( ( SemanticDerived * )functions[ DERIVED ] )->isControlDerived() )
    {
      return &intervalControlDerived[ whichOrder ];
    }
    else
    {
      return &intervalDerived[ whichOrder ];
    }
  }
  return nullptr;
}

void KDerivedWindow::setParent( PRV_UINT16 whichParent, Timeline *whichWindow )
{
  parents[ whichParent ] = ( KTimeline * )whichWindow;
  if( parents[ 0 ] != nullptr && parents[ 1 ] != nullptr )
    setup( nullptr );
}

Timeline *KDerivedWindow::getParent( PRV_UINT16 whichParent ) const
{
  return parents[whichParent];
}

void KDerivedWindow::setLevel( TTraceLevel whichLevel )
{
  if( whichLevel <= getMinAcceptableLevel() )
    level = whichLevel;
}

TTraceLevel KDerivedWindow::getMinAcceptableLevel() const
{
  TTraceLevel tmp = TTraceLevel::NONE;

  for( PRV_UINT16 i = 0; i < parents.size(); i++ )
  {
    if( parents[ i ] != nullptr && parents[ i ]->getLevel() > tmp )
      tmp = parents[ i ]->getLevel();
  }

  if( tmp == TTraceLevel::NONE )
    return TTraceLevel::THREAD;
  return tmp;
}

KTimeline *KDerivedWindow::clone( bool recursiveClone )
{
  KDerivedWindow *clonedKDerivedWindow = new KDerivedWindow();

  for( size_t i = 0; i < parents.size(); ++i )
  {
    if( recursiveClone )
      clonedKDerivedWindow->parents[ i ] = parents[ i ]->clone( recursiveClone );
    else
      clonedKDerivedWindow->parents[ i ] = nullptr;
    clonedKDerivedWindow->factor[ i ] = factor[ i ];
    clonedKDerivedWindow->shift[ i ] = shift[ i ];
  }

  for ( PRV_UINT16 i = 0; i <= DERIVED; ++i )
  {
    if( functions[ i ] != nullptr )
    {
      clonedKDerivedWindow->functions[ i ] = functions[ i ]->clone();
    }
    else
      clonedKDerivedWindow->functions[ i ] = nullptr;
  }

  clonedKDerivedWindow->setup( myTrace );
  clonedKDerivedWindow->level = getLevel();
  clonedKDerivedWindow->timeUnit = timeUnit;

  for( map< TWindowLevel, vector< vector< IntervalCompose * > > >::iterator itMap = extraCompose.begin();
       itMap != extraCompose.end(); ++itMap )
  {
    for( size_t i = 0; i < itMap->second.size(); ++i )
      clonedKDerivedWindow->addExtraCompose( itMap->first );
  }

  for( map< TWindowLevel, vector< SemanticFunction * > >::iterator itMap = extraComposeFunctions.begin();
       itMap != extraComposeFunctions.end(); ++itMap )
  {
    for( size_t i = 0; i < itMap->second.size(); ++i )
    {
      delete ( clonedKDerivedWindow->extraComposeFunctions[ itMap->first ] )[ i ];
      ( clonedKDerivedWindow->extraComposeFunctions[ itMap->first ] )[ i ] = ( extraComposeFunctions[ itMap->first ] )[ i ]->clone();
    }
  }

  for( size_t iPos = 0; iPos < clonedKDerivedWindow->extraCompose[ TOPCOMPOSE1 ].size(); ++iPos )
  {
    for( size_t iRow = 0; iRow < ( clonedKDerivedWindow->extraCompose[ TOPCOMPOSE1 ] )[ iPos ].size(); ++iRow )
      ( ( clonedKDerivedWindow->extraCompose[ TOPCOMPOSE1 ] )[ iPos ] )[ iRow ]->setSemanticFunction( (SemanticCompose *)( clonedKDerivedWindow->extraComposeFunctions[ TOPCOMPOSE1 ] )[ iPos ] );
  }

  return clonedKDerivedWindow;
}


SemanticInfoType KDerivedWindow::getSemanticInfoType() const
{
  map< TWindowLevel, vector<SemanticFunction *> >::const_iterator itMap = extraComposeFunctions.find( TOPCOMPOSE1 );
  if( itMap != extraComposeFunctions.end() )
  {
    for( vector<SemanticFunction *>::const_reverse_iterator it = itMap->second.rbegin();
         it != itMap->second.rend(); ++it )
    {
      if( (*it)->getSemanticInfoType() != SAME_TYPE )
        return (*it)->getSemanticInfoType();
    }
  }

  if( functions[ TOPCOMPOSE1 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ TOPCOMPOSE1 ]->getSemanticInfoType();
  if( functions[ TOPCOMPOSE2 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ TOPCOMPOSE2 ]->getSemanticInfoType();

  if( level >= TTraceLevel::SYSTEM && level <= TTraceLevel::CPU )
  {
    switch( level )
    {
      case TTraceLevel::SYSTEM:
        if( functions[ COMPOSESYSTEM ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSESYSTEM ]->getSemanticInfoType();
        if( getMinAcceptableLevel() > TTraceLevel::SYSTEM && functions[ SYSTEM ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ SYSTEM ]->getSemanticInfoType();
      case TTraceLevel::NODE:
        if( functions[ COMPOSENODE ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSENODE ]->getSemanticInfoType();
        if( getMinAcceptableLevel() > TTraceLevel::NODE && functions[ NODE ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ NODE ]->getSemanticInfoType();
      case TTraceLevel::CPU:
        if( functions[ COMPOSECPU ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSECPU ]->getSemanticInfoType();
      default:
        break;
    }
  }
  else if( level >= TTraceLevel::WORKLOAD && level <= TTraceLevel::THREAD )
  {
    switch( level )
    {
      case TTraceLevel::WORKLOAD:
        if( getMinAcceptableLevel() > TTraceLevel::WORKLOAD && functions[ COMPOSEWORKLOAD ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSEWORKLOAD ]->getSemanticInfoType();
        if( functions[ WORKLOAD ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ WORKLOAD ]->getSemanticInfoType();
      case TTraceLevel::APPLICATION:
        if( functions[ COMPOSEAPPLICATION ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSEAPPLICATION ]->getSemanticInfoType();
        if( getMinAcceptableLevel() > TTraceLevel::APPLICATION && functions[ APPLICATION ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ APPLICATION ]->getSemanticInfoType();
      case TTraceLevel::TASK:
        if( functions[ COMPOSETASK ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSETASK ]->getSemanticInfoType();
        if( getMinAcceptableLevel() > TTraceLevel::TASK && functions[ TASK ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ TASK ]->getSemanticInfoType();
      default:
        break;
    }
  }
  if( functions[ COMPOSETHREAD ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ COMPOSETHREAD ]->getSemanticInfoType();

  if( functions[ DERIVED ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ DERIVED ]->getSemanticInfoType();

  return parents[ 0 ]->getSemanticInfoType();
}


void KDerivedWindow::initSemanticFunctions()
{
  functions[ 0 ] = nullptr;

  functions[ TOPCOMPOSE1 ] = new ComposeAsIs();
  functions[ TOPCOMPOSE2 ] = new ComposeAsIs();

  functions[ COMPOSEWORKLOAD ] = new ComposeAsIs();
  functions[ WORKLOAD ] = new Adding();

  functions[ COMPOSEAPPLICATION ] = new ComposeAsIs();
  functions[ APPLICATION ] = new Adding();

  functions[ COMPOSETASK ] = new ComposeAsIs();
  functions[ TASK ] = new Adding();

  functions[ COMPOSETHREAD ] = new ComposeAsIs();
  functions[ THREAD ] = nullptr;

  functions[ COMPOSESYSTEM ] = new ComposeAsIs();
  functions[ SYSTEM ] = new Adding();

  functions[ COMPOSENODE ] = new ComposeAsIs();
  functions[ NODE ] = new Adding();

  functions[ COMPOSECPU ] = new ComposeAsIs();
  functions[ CPU ] = new ActiveThread();

  functions[ DERIVED ] = nullptr;//new DerivedProduct();
}


