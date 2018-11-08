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

TObjectOrder KWindow::cpuObjectToWindowObject( TCPUOrder whichCPU )
{
  TObjectOrder tmpObject = 0;

  if( level == CPU )
    tmpObject = whichCPU;
  else if( level == NODE )
  {
    TCPUOrder myCPU;
    TNodeOrder myNode;

    myTrace->getCPULocation( whichCPU, myNode, myCPU );
    tmpObject = myNode;
  }
  else if( level == SYSTEM )
  {
    tmpObject = 0;
  }

  return tmpObject;
}


TObjectOrder KWindow::threadObjectToWindowObject( TThreadOrder whichThread )
{
  TObjectOrder tmpObject = 0;

  if( level == THREAD )
    tmpObject = whichThread;
  else if( level == TASK )
  {
    TThreadOrder myThread;
    TTaskOrder myTask;
    TApplOrder myAppl;

    myTrace->getThreadLocation( whichThread, myAppl, myTask, myThread );
    tmpObject = myTrace->getGlobalTask( myAppl, myTask );
  }
  else if( level == APPLICATION )
  {
    TThreadOrder myThread;
    TTaskOrder myTask;
    TApplOrder myAppl;

    myTrace->getThreadLocation( whichThread, myAppl, myTask, myThread );
    tmpObject = myAppl;
  }
  else if( level == WORKLOAD )
  {
    tmpObject = 0;
  }

  return tmpObject;
}


TObjectOrder KWindow::getWindowLevelObjects() const
{
  TObjectOrder objectSize = 0;
  TWindowLevel whichLevel = getLevel();

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


TRecordTime KWindow::customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const
{
  TRecordTime tmpTime;
  TRecordTime factor = 1;

  if( myTrace->getTimeUnit() == whichUnits )
    tmpTime = whichTime;
  else
  {
    PRV_UINT16 from = myTrace->getTimeUnit() > whichUnits ? whichUnits : myTrace->getTimeUnit();
    PRV_UINT16 to = myTrace->getTimeUnit() > whichUnits ? myTrace->getTimeUnit() : whichUnits;

    for( PRV_UINT16 i = from + 1; i <= to; i++ )
      factor *= factorTable[ i ];

    if( myTrace->getTimeUnit() > whichUnits )
      tmpTime = whichTime / factor;
    else
      tmpTime = whichTime * factor;
  }

  return tmpTime;
}


TRecordTime KWindow::traceUnitsToCustomUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const
{
  TRecordTime tmpTime;
  TRecordTime factor = 1;

  if( myTrace->getTimeUnit() == whichUnits )
    tmpTime = whichTime;
  else
  {
    PRV_UINT16 from = myTrace->getTimeUnit() > whichUnits ? whichUnits : myTrace->getTimeUnit();
    PRV_UINT16 to = myTrace->getTimeUnit() > whichUnits ? myTrace->getTimeUnit() : whichUnits;

    for( PRV_UINT16 i = from + 1; i <= to; i++ )
      factor *= factorTable[ i ];

    if( myTrace->getTimeUnit() > whichUnits )
      tmpTime = whichTime * factor;
    else
      tmpTime = whichTime / factor;
  }

  return tmpTime;
}


TRecordTime KWindow::traceUnitsToWindowUnits( TRecordTime whichTime ) const
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

TRecordTime KWindow::windowUnitsToTraceUnits( TRecordTime whichTime ) const
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

RecordList *KWindow::getRecordList( TObjectOrder whichObject )
{
  return intervalTopCompose1[ whichObject ].getRecordList();
}

KWindow *KWindow::clone( bool recursiveClone )
{
  return NULL;
}

void KWindow::getGroupLabels( PRV_UINT32 whichGroup, vector<string>& onVector ) const
{
  FunctionManagement<SemanticFunction>::getInstance()->getAll( onVector, whichGroup );
}


bool KWindow::getParametersOfFunction( string whichFunction,
                                       PRV_UINT32 &numParameters,
                                       vector< string > &nameParameters,
                                       vector< vector < double > > &defaultValues ) const
{
  bool done = false;

  SemanticFunction *tmp = FunctionManagement<SemanticFunction>::getInstance()->getFunction( whichFunction );

  if( tmp != NULL )
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
size_t KWindow::getExtraNumPositions( TWindowLevel whichLevel ) const
{
  map< TWindowLevel, vector< IntervalCompose * > >::const_iterator it = extraCompose.find( whichLevel );

  if( it == extraCompose.end() )
    return 0;

  return it->second.size();
}


/**********************************************************************
 *  KSingleWindow implementation
 **********************************************************************/

KSingleWindow::KSingleWindow( Trace *whichTrace ): KWindow( whichTrace )
{
  initSemanticFunctions();

  recordsByTimeThread.reserve( myTrace->totalThreads() );
  for( TThreadOrder i = 0; i < myTrace->totalThreads(); ++i )
    recordsByTimeThread.push_back( NULL );
  recordsByTimeCPU.reserve( myTrace->totalCPUs() );
  for( TCPUOrder i = 0; i < myTrace->totalCPUs(); ++i )
    recordsByTimeCPU.push_back( NULL );
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
  if( functions[ TOPCOMPOSE1 ] != NULL )
    delete functions[ TOPCOMPOSE1 ];
  if( functions[ TOPCOMPOSE2 ] != NULL )
    delete functions[ TOPCOMPOSE2 ];

  if( functions[ COMPOSEWORKLOAD ] != NULL )
    delete functions[ COMPOSEWORKLOAD ];
  if( functions[ WORKLOAD ] != NULL )
    delete functions[ WORKLOAD ];
  if( functions[ COMPOSEAPPLICATION ] != NULL )
    delete functions[ COMPOSEAPPLICATION ];
  if( functions[ APPLICATION ] != NULL )
    delete functions[ APPLICATION ];
  if( functions[ COMPOSETASK ] != NULL )
    delete functions[ COMPOSETASK ];
  if( functions[ TASK ] != NULL )
    delete functions[ TASK ];
  if( functions[ COMPOSETHREAD ] != NULL )
    delete functions[ COMPOSETHREAD ];
  if( functions[ THREAD ] != NULL )
    delete functions[ THREAD ];

  if( functions[ COMPOSESYSTEM ] != NULL )
    delete functions[ COMPOSESYSTEM ];
  if( functions[ SYSTEM ] != NULL )
    delete functions[ SYSTEM ];
  if( functions[ COMPOSENODE ] != NULL )
    delete functions[ COMPOSENODE ];
  if( functions[ NODE ] != NULL )
    delete functions[ NODE ];
  if( functions[ COMPOSECPU ] != NULL )
    delete functions[ COMPOSECPU ];
  if( functions[ CPU ] != NULL )
    delete functions[ CPU ];

  for( TObjectOrder i = 0; i < recordsByTimeThread.size(); ++i )
  {
    if( recordsByTimeThread[ i ] != NULL )
      delete recordsByTimeThread[ i ];
  }
  recordsByTimeThread.clear();

  for( TObjectOrder i = 0; i < recordsByTimeCPU.size(); ++i )
  {
    if( recordsByTimeCPU[ i ] != NULL )
      delete recordsByTimeCPU[ i ];
  }
  recordsByTimeCPU.clear();

  delete myFilter;
}

void KSingleWindow::init( TRecordTime initialTime, TCreateList create, bool updateLimits )
{
  for( PRV_UINT8 i = WORKLOAD; i <= COMPOSECPU; i++ )
  {
    if( functions[ i ] != NULL )
      functions[ i ]->init( this );
  }

  if( level >= SYSTEM )
  {
    if( initialTime > 0 && !initFromBegin() )
      myTrace->getRecordByTimeCPU( recordsByTimeCPU, initialTime );
    else
    {
      for( TCPUOrder i = 0; i < myTrace->totalCPUs(); ++i )
      {
        if( recordsByTimeCPU[ i ] != NULL )
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
      if( recordsByTimeThread[ i ] != NULL )
        delete recordsByTimeThread[ i ];
      recordsByTimeThread[ i ] = myTrace->threadBegin( i );
    }
  }
}

void KSingleWindow::initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits )
{
  intervalTopCompose1[ whichRow ].init( initialTime, create );
}

bool KSingleWindow::setLevelFunction( TWindowLevel whichLevel,
                                      const string& whichFunction )
{
  if( whichLevel == DERIVED )
    return false;

  if( functions[ whichLevel ] != NULL )
    delete functions[ whichLevel ];

  functions[ whichLevel ] =
    ( FunctionManagement<SemanticFunction>::getInstance() )->
    getFunction( whichFunction );

  if( functions[ whichLevel ] == NULL )
    return false;

  return true;
}


string KSingleWindow::getLevelFunction( TWindowLevel whichLevel )
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
  if( typeid( *functions[ getComposeLevel( getLevel() )] ) != typeid( ComposeAsIs ) )
    return functions[ getComposeLevel( getLevel() )]->getName();

  return functions[ getLevel()]->getName();
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
  if( typeid( *functions[ getComposeLevel( getLevel() )] ) != typeid( ComposeAsIs ) )
    return functions[ getComposeLevel( getLevel() )];

  return functions[ getLevel()];
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

  vector< IntervalCompose * > vCompose = extraCompose[ whichLevel ];
  vCompose.push_back( new IntervalCompose() );

  vector< SemanticFunction * > vComposeFunction = extraComposeFunctions[ whichLevel ];
  vComposeFunction.push_back( new ComposeAsIs() );
}

void KSingleWindow::removeExtraCompose( TWindowLevel whichLevel )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= COMPOSECPU ) )
    return; // Should throw an Exception

  delete extraCompose[ whichLevel ].back();
  extraCompose[ whichLevel ].pop_back();

  delete extraComposeFunctions[ whichLevel ].back();
  extraComposeFunctions[ whichLevel ].pop_back();
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

  if( it->second[ whichPosition ] == NULL )
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

bool KSingleWindow::initFromBegin() const
{
  bool tmp = false;

  tmp = tmp || functions[ TOPCOMPOSE1 ]->getInitFromBegin();
  tmp = tmp || functions[ TOPCOMPOSE2 ]->getInitFromBegin();

  if( level == WORKLOAD )
  {
    tmp = tmp || functions[ COMPOSEWORKLOAD ]->getInitFromBegin();
    tmp = tmp || functions[ WORKLOAD ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSEAPPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ APPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == APPLICATION )
  {
    tmp = tmp || functions[ COMPOSEAPPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ APPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == TASK )
  {
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == SYSTEM )
  {
    tmp = tmp || functions[ COMPOSESYSTEM ]->getInitFromBegin();
    tmp = tmp || functions[ SYSTEM ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSENODE ]->getInitFromBegin();
    tmp = tmp || functions[ NODE ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }
  else if( level == NODE )
  {
    tmp = tmp || functions[ COMPOSENODE ]->getInitFromBegin();
    tmp = tmp || functions[ NODE ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }
  else if( level == CPU )
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
  return intervalTopCompose1[ whichObject ].calcNext();
}


RecordList *KSingleWindow::calcPrev( TObjectOrder whichObject, bool updateLimits )
{
  return intervalTopCompose1[ whichObject ].calcPrev();
}


TRecordTime KSingleWindow::getBeginTime( TObjectOrder whichObject ) const
{
  return intervalTopCompose1[ whichObject ].getBeginTime();
}


TRecordTime KSingleWindow::getEndTime( TObjectOrder whichObject ) const
{
  return intervalTopCompose1[ whichObject ].getEndTime();
}


TSemanticValue KSingleWindow::getValue( TObjectOrder whichObject ) const
{
  return intervalTopCompose1[ whichObject ].getValue();
}


Interval *KSingleWindow::getLevelInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  if( whichLevel == TOPCOMPOSE1 )
    return &intervalTopCompose1[ whichOrder ];
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
  return NULL;
}

SemanticInfoType KSingleWindow::getSemanticInfoType() const
{
  if( functions[ TOPCOMPOSE1 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ TOPCOMPOSE1 ]->getSemanticInfoType();
  if( functions[ TOPCOMPOSE2 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ TOPCOMPOSE2 ]->getSemanticInfoType();

  if( level >= SYSTEM && level <= CPU )
  {
    switch( level )
    {
      case SYSTEM:
        if( functions[ COMPOSESYSTEM ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSESYSTEM ]->getSemanticInfoType();
        if( functions[ SYSTEM ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ SYSTEM ]->getSemanticInfoType();
      case NODE:
        if( functions[ COMPOSENODE ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSENODE ]->getSemanticInfoType();
        if( functions[ NODE ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ NODE ]->getSemanticInfoType();
      case CPU:
        if( functions[ COMPOSECPU ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSECPU ]->getSemanticInfoType();
        if( functions[ CPU ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ CPU ]->getSemanticInfoType();
      default:
        break;
    }
  }
  else if( level >= WORKLOAD && level <= THREAD )
  {
    switch( level )
    {
      case WORKLOAD:
        if( functions[ COMPOSEWORKLOAD ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSEWORKLOAD ]->getSemanticInfoType();
        if( functions[ WORKLOAD ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ WORKLOAD ]->getSemanticInfoType();
      case APPLICATION:
        if( functions[ COMPOSEAPPLICATION ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSEAPPLICATION ]->getSemanticInfoType();
        if( functions[ APPLICATION ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ APPLICATION ]->getSemanticInfoType();
      case TASK:
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


KWindow *KSingleWindow::clone( bool recursiveClone )
{
  KSingleWindow *clonedKSWindow = new KSingleWindow( myTrace );

  clonedKSWindow->level = level;
  clonedKSWindow->timeUnit = timeUnit;

  for( int i = 0; i < COMPOSECPU + 1; ++i )
  {
    if( functions[ i ] != NULL )
      clonedKSWindow->functions[ i ] = functions[ i ]->clone();
    else
      clonedKSWindow->functions[ i ] = NULL;
  }

  delete clonedKSWindow->myFilter;
  clonedKSWindow->myFilter = myFilter->clone( clonedKSWindow );

  clonedKSWindow->recordsByTimeCPU.clear();
  for( vector<MemoryTrace::iterator *>::const_iterator it = recordsByTimeCPU.begin();
       it != recordsByTimeCPU.end(); ++it )
  {
    if( *it != NULL )
      clonedKSWindow->recordsByTimeCPU.push_back( (*it)->clone() );
    else
      clonedKSWindow->recordsByTimeCPU.push_back( NULL );
  }

  clonedKSWindow->recordsByTimeThread.clear();
  for( vector<MemoryTrace::iterator *>::const_iterator it = recordsByTimeThread.begin();
       it != recordsByTimeThread.end(); ++it )
  {
    if( *it != NULL )
      clonedKSWindow->recordsByTimeThread.push_back( (*it)->clone() );
    else
      clonedKSWindow->recordsByTimeThread.push_back( NULL );
  }

  return clonedKSWindow;
}


void KSingleWindow::initSemanticFunctions()
{
  functions[ 0 ] = NULL;

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
  if( functions[ TOPCOMPOSE1 ] != NULL )
    delete functions[ TOPCOMPOSE1 ];
  if( functions[ TOPCOMPOSE2 ] != NULL )
    delete functions[ TOPCOMPOSE2 ];

  if( functions[ COMPOSEWORKLOAD ] != NULL )
    delete functions[ COMPOSEWORKLOAD ];
  if( functions[ WORKLOAD ] != NULL )
    delete functions[ WORKLOAD ];
  if( functions[ COMPOSEAPPLICATION ] != NULL )
    delete functions[ COMPOSEAPPLICATION ];
  if( functions[ APPLICATION ] != NULL )
    delete functions[ APPLICATION ];
  if( functions[ COMPOSETASK ] != NULL )
    delete functions[ COMPOSETASK ];
  if( functions[ TASK ] != NULL )
    delete functions[ TASK ];
  if( functions[ COMPOSETHREAD ] != NULL )
    delete functions[ COMPOSETHREAD ];
  if( functions[ THREAD ] != NULL )
    delete functions[ THREAD ];

  if( functions[ COMPOSESYSTEM ] != NULL )
    delete functions[ COMPOSESYSTEM ];
  if( functions[ SYSTEM ] != NULL )
    delete functions[ SYSTEM ];
  if( functions[ COMPOSENODE ] != NULL )
    delete functions[ COMPOSENODE ];
  if( functions[ NODE ] != NULL )
    delete functions[ NODE ];
  if( functions[ COMPOSECPU ] != NULL )
    delete functions[ COMPOSECPU ];
  if( functions[ CPU ] != NULL )
    delete functions[ CPU ];

  if( functions[ DERIVED ] != NULL )
    delete functions[ DERIVED ];
}

void KDerivedWindow::setup( KTrace* whichTrace )
{
  if( whichTrace == NULL )
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
  if( functions[ whichLevel ] != NULL )
    delete functions[ whichLevel ];

  functions[ whichLevel ] =
    ( FunctionManagement<SemanticFunction>::getInstance() )->
    getFunction( whichFunction );

  if( functions[ whichLevel ] == NULL )
    return false;

  return true;
}


string KDerivedWindow::getLevelFunction( TWindowLevel whichLevel )
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
    return functions[ getComposeLevel( getLevel() )]->getName();

  if( getLevel() == getMinAcceptableLevel() )
    return functions[ DERIVED ]->getName();

  return functions[ getLevel() ]->getName();
}

TWindowLevel KDerivedWindow::getFirstFreeCompose() const
{
  if( typeid( *functions[ getComposeLevel( getLevel() )] ) == typeid( ComposeAsIs ) )
    return getComposeLevel( getLevel() );
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
  if( typeid( *functions[ getComposeLevel( getLevel() ) ] ) != typeid( ComposeAsIs ) )
    return functions[ getComposeLevel( getLevel() ) ];

  if( getLevel() == getMinAcceptableLevel() )
    return functions[ DERIVED ];

  return functions[ getLevel() ];
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

  vector< IntervalCompose * > vCompose = extraCompose[ whichLevel ];
  vCompose.push_back( new IntervalCompose() );

  vector< SemanticFunction * > vComposeFunction = extraComposeFunctions[ whichLevel ];
  vComposeFunction.push_back( new ComposeAsIs() );
}

void KDerivedWindow::removeExtraCompose( TWindowLevel whichLevel )
{
  if( !( whichLevel >= TOPCOMPOSE1 && whichLevel <= DERIVED ) )
    return; // Should throw an Exception

  delete extraCompose[ whichLevel ].back();
  extraCompose[ whichLevel ].pop_back();

  delete extraComposeFunctions[ whichLevel ].back();
  extraComposeFunctions[ whichLevel ].pop_back();
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

  if( it->second[ whichPosition ] == NULL )
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

bool KDerivedWindow::initFromBegin() const
{
  bool tmp = false;

  tmp = tmp || functions[ TOPCOMPOSE1 ]->getInitFromBegin();
  tmp = tmp || functions[ TOPCOMPOSE2 ]->getInitFromBegin();

  if( level == WORKLOAD )
  {
    tmp = tmp || functions[ COMPOSEWORKLOAD ]->getInitFromBegin();
    tmp = tmp || functions[ WORKLOAD ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSEAPPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ APPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == APPLICATION )
  {
    tmp = tmp || functions[ COMPOSEAPPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ APPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == TASK )
  {
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if( level == SYSTEM )
  {
    tmp = tmp || functions[ COMPOSESYSTEM ]->getInitFromBegin();
    tmp = tmp || functions[ SYSTEM ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSENODE ]->getInitFromBegin();
    tmp = tmp || functions[ NODE ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }
  else if( level == NODE )
  {
    tmp = tmp || functions[ COMPOSENODE ]->getInitFromBegin();
    tmp = tmp || functions[ NODE ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }
  else if( level == CPU )
  {
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
  }
  else if( level == THREAD )
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
    if( functions[ i ] != NULL )
      functions[ i ]->init( this );
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
  intervalTopCompose1[ whichRow ].init( initialTime, create );
}

RecordList *KDerivedWindow::calcNext( TObjectOrder whichObject, bool updateLimits )
{
  return intervalTopCompose1[ whichObject ].calcNext();
}


RecordList *KDerivedWindow::calcPrev( TObjectOrder whichObject, bool updateLimits )
{
  return intervalTopCompose1[ whichObject ].calcPrev();
}


TRecordTime KDerivedWindow::getBeginTime( TObjectOrder whichObject ) const
{
  return intervalTopCompose1[ whichObject ].getBeginTime();
}


TRecordTime KDerivedWindow::getEndTime( TObjectOrder whichObject ) const
{
  return intervalTopCompose1[ whichObject ].getEndTime();
}


TSemanticValue KDerivedWindow::getValue( TObjectOrder whichObject ) const
{
  return intervalTopCompose1[ whichObject ].getValue();
}


Interval *KDerivedWindow::getLevelInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  if( whichLevel == getMinAcceptableLevel() )
    whichLevel = DERIVED;

  if( whichLevel == TOPCOMPOSE1 )
    return &intervalTopCompose1[ whichOrder ];
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
  return NULL;
}

void KDerivedWindow::setParent( PRV_UINT16 whichParent, Window *whichWindow )
{
  parents[ whichParent ] = ( KWindow* )whichWindow;
  if( parents[ 0 ] != NULL && parents[ 1 ] != NULL )
    setup( NULL );
}

Window *KDerivedWindow::getParent( PRV_UINT16 whichParent ) const
{
  return parents[whichParent];
}

void KDerivedWindow::setLevel( TWindowLevel whichLevel )
{
  if( whichLevel <= getMinAcceptableLevel() )
    level = whichLevel;
}

TWindowLevel KDerivedWindow::getMinAcceptableLevel() const
{
  TWindowLevel tmp = NONE;

  for( PRV_UINT16 i = 0; i < parents.size(); i++ )
  {
    if( parents[ i ] != NULL && parents[ i ]->getLevel() > tmp )
      tmp = parents[ i ]->getLevel();
  }

  if( tmp == NONE )
    return THREAD;
  return tmp;
}

KWindow *KDerivedWindow::clone( bool recursiveClone )
{
  KDerivedWindow *clonedKDerivedWindow = new KDerivedWindow();

  for( size_t i = 0; i < parents.size(); ++i )
  {
    if( recursiveClone )
      clonedKDerivedWindow->parents[ i ] = parents[ i ]->clone( recursiveClone );
    else
      clonedKDerivedWindow->parents[ i ] = NULL;
    clonedKDerivedWindow->factor[ i ] = factor[ i ];
  }

  for ( PRV_UINT16 i = 0; i <= DERIVED; ++i )
  {
    if( functions[ i ] != NULL )
    {
      clonedKDerivedWindow->functions[ i ] = functions[ i ]->clone();
    }
    else
      clonedKDerivedWindow->functions[ i ] = NULL;
  }

  clonedKDerivedWindow->setup( myTrace );
  clonedKDerivedWindow->level = getLevel();
  clonedKDerivedWindow->timeUnit = timeUnit;

  return clonedKDerivedWindow;
}


SemanticInfoType KDerivedWindow::getSemanticInfoType() const
{
  if( functions[ TOPCOMPOSE1 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ TOPCOMPOSE1 ]->getSemanticInfoType();
  if( functions[ TOPCOMPOSE2 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ TOPCOMPOSE2 ]->getSemanticInfoType();

  if( level >= SYSTEM && level <= CPU )
  {
    switch( level )
    {
      case SYSTEM:
        if( functions[ COMPOSESYSTEM ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSESYSTEM ]->getSemanticInfoType();
        if( getMinAcceptableLevel() > SYSTEM && functions[ SYSTEM ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ SYSTEM ]->getSemanticInfoType();
      case NODE:
        if( functions[ COMPOSENODE ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSENODE ]->getSemanticInfoType();
        if( getMinAcceptableLevel() > NODE && functions[ NODE ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ NODE ]->getSemanticInfoType();
      case CPU:
        if( functions[ COMPOSECPU ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSECPU ]->getSemanticInfoType();
      default:
        break;
    }
  }
  else if( level >= WORKLOAD && level <= THREAD )
  {
    switch( level )
    {
      case WORKLOAD:
        if( getMinAcceptableLevel() > WORKLOAD && functions[ COMPOSEWORKLOAD ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSEWORKLOAD ]->getSemanticInfoType();
        if( functions[ WORKLOAD ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ WORKLOAD ]->getSemanticInfoType();
      case APPLICATION:
        if( functions[ COMPOSEAPPLICATION ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSEAPPLICATION ]->getSemanticInfoType();
        if( getMinAcceptableLevel() > APPLICATION && functions[ APPLICATION ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ APPLICATION ]->getSemanticInfoType();
      case TASK:
        if( functions[ COMPOSETASK ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSETASK ]->getSemanticInfoType();
        if( getMinAcceptableLevel() > TASK && functions[ TASK ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ TASK ]->getSemanticInfoType();
      default:
        break;
    }
  }
  if( functions[ COMPOSETHREAD ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ COMPOSETHREAD ]->getSemanticInfoType();
  return parents[ 0 ]->getSemanticInfoType();
}


void KDerivedWindow::initSemanticFunctions()
{
  functions[ 0 ] = NULL;

  functions[ TOPCOMPOSE1 ] = new ComposeAsIs();
  functions[ TOPCOMPOSE2 ] = new ComposeAsIs();

  functions[ COMPOSEWORKLOAD ] = new ComposeAsIs();
  functions[ WORKLOAD ] = new Adding();

  functions[ COMPOSEAPPLICATION ] = new ComposeAsIs();
  functions[ APPLICATION ] = new Adding();

  functions[ COMPOSETASK ] = new ComposeAsIs();
  functions[ TASK ] = new Adding();

  functions[ COMPOSETHREAD ] = new ComposeAsIs();
  functions[ THREAD ] = NULL;

  functions[ COMPOSESYSTEM ] = new ComposeAsIs();
  functions[ SYSTEM ] = new Adding();

  functions[ COMPOSENODE ] = new ComposeAsIs();
  functions[ NODE ] = new Adding();

  functions[ COMPOSECPU ] = new ComposeAsIs();
  functions[ CPU ] = new ActiveThread();

  functions[ DERIVED ] = NULL;//new DerivedProduct();
}


