#include <typeinfo>
#include "kwindow.h"
#include "semanticcomposefunctions.h"
#include "semanticderived.h"
#include "functionmanagement.h"


TObjectOrder KWindow::cpuObjectToWindowObject( TCPUOrder whichCPU )
{
  TObjectOrder tmpObject = 0;

  if ( level == CPU )
    tmpObject = whichCPU;
  else if ( level == NODE )
  {
    TCPUOrder myCPU;
    TNodeOrder myNode;

    myTrace->getCPULocation( whichCPU, myNode, myCPU );
    tmpObject = myNode;
  }
  else if ( level == SYSTEM )
  {
    tmpObject = 1;
  }

  return tmpObject;
}


TObjectOrder KWindow::threadObjectToWindowObject( TThreadOrder whichThread )
{
  TObjectOrder tmpObject = 0;

  if ( level == THREAD )
    tmpObject = whichThread;
  else if ( level == TASK )
  {
    TThreadOrder myThread;
    TTaskOrder myTask;
    TApplOrder myAppl;

    myTrace->getThreadLocation( whichThread, myAppl, myTask, myThread );
    tmpObject = myTrace->getGlobalTask( myAppl, myTask );
  }
  else if ( level == APPLICATION )
  {
    TThreadOrder myThread;
    TTaskOrder myTask;
    TApplOrder myAppl;

    myTrace->getThreadLocation( whichThread, myAppl, myTask, myThread );
    tmpObject = myAppl;
  }
  else if ( level == WORKLOAD )
  {
    tmpObject = 1;
  }

  return tmpObject;
}


TObjectOrder KWindow::getWindowLevelObjects()
{
  TObjectOrder objectSize = 0;
  TWindowLevel whichLevel = getLevel();

  if ( whichLevel == WORKLOAD )
    objectSize = 1;
  else if ( whichLevel == APPLICATION )
    objectSize = myTrace->totalApplications();
  else if ( whichLevel == TASK )
    objectSize = myTrace->totalTasks();
  else if ( whichLevel == THREAD )
    objectSize = myTrace->totalThreads();
  else if ( whichLevel == SYSTEM )
    objectSize = 1;
  else if ( whichLevel == NODE )
    objectSize = myTrace->totalNodes();
  else if ( whichLevel == CPU )
    objectSize = myTrace->totalCPUs();

  return objectSize;
}


TRecordTime KWindow::traceUnitsToWindowUnits( TRecordTime whichTime )
{
  TRecordTime tmpTime;
  TRecordTime factor = 1;

  if ( myTrace->getTimeUnit() == timeUnit )
    tmpTime = whichTime;
  else
  {
    UINT16 from = myTrace->getTimeUnit() > timeUnit ? timeUnit : myTrace->getTimeUnit();
    UINT16 to = myTrace->getTimeUnit() > timeUnit ? myTrace->getTimeUnit() : timeUnit;

    for ( UINT16 i = from + 1; i <= to; i++ )
      factor *= factorTable[ i ];

    if ( myTrace->getTimeUnit() > timeUnit )
      tmpTime = whichTime * factor;
    else
      tmpTime = whichTime / factor;
  }

  return tmpTime;
}


RecordList *KWindow::getRecordList( TObjectOrder whichObject )
{
  return intervalTopCompose1[ whichObject ].getRecordList();
}

/**********************************************************************
 *  KSingleWindow implementation
 **********************************************************************/

KSingleWindow::KSingleWindow( Trace *whichTrace ): KWindow( whichTrace )
{
  functions[ TOPCOMPOSE1 ] = new ComposeAsIs();
  functions[ TOPCOMPOSE2 ] = new ComposeAsIs();

  functions[ COMPOSEWORKLOAD ] = new ComposeAsIs();
  functions[ WORKLOAD ] = NULL;
  functions[ COMPOSEAPPLICATION ] = new ComposeAsIs();
  functions[ APPLICATION ] = NULL;
  functions[ COMPOSETASK ] = new ComposeAsIs();
  functions[ TASK ] = NULL;
  functions[ COMPOSETHREAD ] = new ComposeAsIs();
  functions[ THREAD ] = NULL;

  functions[ COMPOSESYSTEM ] = new ComposeAsIs();
  functions[ SYSTEM ] = NULL;
  functions[ COMPOSENODE ] = new ComposeAsIs();
  functions[ NODE ] = NULL;
  functions[ COMPOSECPU ] = new ComposeAsIs();
  functions[ CPU ] = NULL;

  if ( myTrace->totalThreads() > myTrace->totalCPUs() )
  {
    recordsByTime.reserve( myTrace->totalThreads() );
    intervalTopCompose1.reserve( myTrace->totalThreads() );
    intervalTopCompose2.reserve( myTrace->totalThreads() );
    for ( TThreadOrder i = 0; i < myTrace->totalThreads(); i++ )
    {
      recordsByTime.push_back( NULL );
      intervalTopCompose1.push_back( IntervalCompose( this, TOPCOMPOSE1, i ) );
      intervalTopCompose2.push_back( IntervalCompose( this, TOPCOMPOSE2, i ) );
    }
  }
  else
  {
    recordsByTime.reserve( myTrace->totalCPUs() );
    intervalTopCompose1.reserve( myTrace->totalCPUs() );
    intervalTopCompose2.reserve( myTrace->totalCPUs() );
    for ( TCPUOrder i = 0; i < myTrace->totalCPUs(); i++ )
    {
      recordsByTime.push_back( NULL );
      intervalTopCompose1.push_back( IntervalCompose( this, TOPCOMPOSE1, i ) );
      intervalTopCompose2.push_back( IntervalCompose( this, TOPCOMPOSE2, i ) );
    }
  }

  intervalWorkload.push_back( IntervalNotThread( this, WORKLOAD, 0 ) );
  intervalComposeWorkload.push_back( IntervalCompose( this, COMPOSEWORKLOAD, 0 ) );

  intervalApplication.reserve( myTrace->totalApplications() );
  intervalComposeApplication.reserve( myTrace->totalApplications() );
  for ( TApplOrder i = 0; i < myTrace->totalApplications(); i++ )
  {
    intervalApplication.push_back( IntervalNotThread( this, APPLICATION, i ) );
    intervalComposeApplication.push_back( IntervalCompose( this, COMPOSEAPPLICATION, i ) );
  }

  intervalTask.reserve( myTrace->totalTasks() );
  intervalComposeTask.reserve( myTrace->totalTasks() );
  for ( TTaskOrder i = 0; i < myTrace->totalTasks(); i++ )
  {
    intervalTask.push_back( IntervalNotThread( this, TASK, i ) );
    intervalComposeTask.push_back( IntervalCompose( this, COMPOSETASK, i ) );
  }

  intervalThread.reserve( myTrace->totalThreads() );
  intervalComposeThread.reserve( myTrace->totalThreads() );
  for ( TThreadOrder i = 0; i < myTrace->totalThreads(); i++ )
  {
    intervalThread.push_back( IntervalThread( this, THREAD, i ) );
    intervalComposeThread.push_back( IntervalCompose( this, COMPOSETHREAD, i ) );
  }

  intervalSystem.push_back( IntervalNotThread( this, SYSTEM, 0 ) );
  intervalComposeSystem.push_back( IntervalCompose( this, COMPOSESYSTEM, 0 ) );

  intervalNode.reserve( myTrace->totalNodes() );
  intervalComposeNode.reserve( myTrace->totalNodes() );
  for ( TNodeOrder i = 0; i < myTrace->totalNodes(); i++ )
  {
    intervalNode.push_back( IntervalNotThread( this, NODE, i ) );
    intervalComposeNode.push_back( IntervalCompose( this, COMPOSENODE, i ) );
  }

  intervalCPU.reserve( myTrace->totalCPUs() );
  intervalComposeCPU.reserve( myTrace->totalCPUs() );
  for ( TCPUOrder i = 0; i < myTrace->totalCPUs(); i++ )
  {
    intervalCPU.push_back( IntervalCPU( this, CPU, i ) );
    intervalComposeCPU.push_back( IntervalCompose( this, COMPOSECPU, i ) );
  }

  myFilter = new KFilter( this );
}


KSingleWindow::~KSingleWindow()
{
  if ( functions[ TOPCOMPOSE1 ] != NULL )
    delete functions[ TOPCOMPOSE1 ];
  if ( functions[ TOPCOMPOSE2 ] != NULL )
    delete functions[ TOPCOMPOSE2 ];

  if ( functions[ COMPOSEWORKLOAD ] != NULL )
    delete functions[ COMPOSEWORKLOAD ];
  if ( functions[ WORKLOAD ] != NULL )
    delete functions[ WORKLOAD ];
  if ( functions[ COMPOSEAPPLICATION ] != NULL )
    delete functions[ COMPOSEAPPLICATION ];
  if ( functions[ APPLICATION ] != NULL )
    delete functions[ APPLICATION ];
  if ( functions[ COMPOSETASK ] != NULL )
    delete functions[ COMPOSETASK ];
  if ( functions[ TASK ] != NULL )
    delete functions[ TASK ];
  if ( functions[ COMPOSETHREAD ] != NULL )
    delete functions[ COMPOSETHREAD ];
  if ( functions[ THREAD ] != NULL )
    delete functions[ THREAD ];

  if ( functions[ COMPOSESYSTEM ] != NULL )
    delete functions[ COMPOSESYSTEM ];
  if ( functions[ SYSTEM ] != NULL )
    delete functions[ SYSTEM ];
  if ( functions[ COMPOSENODE ] != NULL )
    delete functions[ COMPOSENODE ];
  if ( functions[ NODE ] != NULL )
    delete functions[ NODE ];
  if ( functions[ COMPOSECPU ] != NULL )
    delete functions[ COMPOSECPU ];
  if ( functions[ CPU ] != NULL )
    delete functions[ CPU ];

  for ( TObjectOrder i = 0; i < recordsByTime.size(); i++ )
  {
    if ( recordsByTime[ i ] != NULL )
      delete recordsByTime[ i ];
  }
  recordsByTime.clear();

  delete myFilter;
}

void KSingleWindow::init( TRecordTime initialTime, TCreateList create )
{
  TObjectOrder objectSize = 0;

  for ( UINT8 i = WORKLOAD; i <= COMPOSECPU; i++ )
  {
    if ( functions[ i ] != NULL )
      functions[ i ]->init( this );
  }

  if ( level >= SYSTEM )
  {
    if ( initialTime > 0 && !initFromBegin() )
      myTrace->getRecordByTimeCPU( recordsByTime, initialTime );
    else
    {
      for ( TCPUOrder i = 0; i < myTrace->totalCPUs(); i++ )
      {
        if ( recordsByTime[ i ] != NULL )
          delete recordsByTime[ i ];
        recordsByTime[ i ] = myTrace->CPUBegin( i );
      }
    }
  }
  else
  {
    if ( initialTime > 0 && !initFromBegin() )
      myTrace->getRecordByTimeThread( recordsByTime, initialTime );
    else
    {
      for ( TThreadOrder i = 0; i < myTrace->totalThreads(); i++ )
      {
        if ( recordsByTime[ i ] != NULL )
          delete recordsByTime[ i ];
        recordsByTime[ i ] = myTrace->threadBegin( i );
      }
    }
  }

  objectSize = getWindowLevelObjects();

  for ( TObjectOrder i = 0; i < objectSize; i++ )
    intervalTopCompose1[ i ].init( initialTime, create );
}


bool KSingleWindow::setLevelFunction( TWindowLevel whichLevel,
                                      const string& whichFunction )
{
  if ( whichLevel == DERIVED )
    return false;

  if ( functions[ whichLevel ] != NULL )
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
  if ( typeid( *functions[ TOPCOMPOSE1 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE1 ]->getName();
  if ( typeid( *functions[ TOPCOMPOSE2 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE2 ]->getName();
  if ( typeid( *functions[ getComposeLevel( getLevel() ) ] ) != typeid( ComposeAsIs ) )
    return functions[ getComposeLevel( getLevel() ) ]->getName();

  return functions[ getLevel() ]->getName();
}


SemanticFunction *KSingleWindow::getFirstSemUsefulFunction()
{
  if ( typeid( *functions[ TOPCOMPOSE1 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE1 ];
  if ( typeid( *functions[ TOPCOMPOSE2 ] ) != typeid( ComposeAsIs ) )
    return functions[ TOPCOMPOSE2 ];
  if ( typeid( *functions[ getComposeLevel( getLevel() ) ] ) != typeid( ComposeAsIs ) )
    return functions[ getComposeLevel( getLevel() ) ];

  return functions[ getLevel() ];
}


void KSingleWindow::setFunctionParam( TWindowLevel whichLevel,
                                      TParamIndex whichParam,
                                      const TParamValue& newValue )
{
  functions[ whichLevel ]->setParam( whichParam, newValue );
}


bool KSingleWindow::initFromBegin() const
{
  bool tmp = false;

  tmp = tmp || functions[ TOPCOMPOSE1 ]->getInitFromBegin();
  tmp = tmp || functions[ TOPCOMPOSE2 ]->getInitFromBegin();

  if ( level == WORKLOAD )
  {
    tmp = tmp || functions[ COMPOSEWORKLOAD ]->getInitFromBegin();
    tmp = tmp || functions[ WORKLOAD ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSEAPPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ APPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if ( level == APPLICATION )
  {
    tmp = tmp || functions[ COMPOSEAPPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ APPLICATION ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if ( level == TASK )
  {
    tmp = tmp || functions[ COMPOSETASK ]->getInitFromBegin();
    tmp = tmp || functions[ TASK ]->getInitFromBegin();
  }
  else if ( level == SYSTEM )
  {
    tmp = tmp || functions[ COMPOSESYSTEM ]->getInitFromBegin();
    tmp = tmp || functions[ SYSTEM ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSENODE ]->getInitFromBegin();
    tmp = tmp || functions[ NODE ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }
  else if ( level == NODE )
  {
    tmp = tmp || functions[ COMPOSENODE ]->getInitFromBegin();
    tmp = tmp || functions[ NODE ]->getInitFromBegin();
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }
  else if ( level == CPU )
  {
    tmp = tmp || functions[ COMPOSECPU ]->getInitFromBegin();
    tmp = tmp || functions[ CPU ]->getInitFromBegin();
  }

  tmp = tmp || functions[ COMPOSETHREAD ]->getInitFromBegin();
  tmp = tmp || functions[ THREAD ]->getInitFromBegin();

  return tmp;
}


RecordList *KSingleWindow::calcNext( TObjectOrder whichObject )
{
  return intervalTopCompose1[ whichObject ].calcNext();
}


RecordList *KSingleWindow::calcPrev( TObjectOrder whichObject )
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
  if ( whichLevel == TOPCOMPOSE1 )
    return &intervalTopCompose1[ whichOrder ];
  else if ( whichLevel == TOPCOMPOSE2 )
    return &intervalTopCompose2[ whichOrder ];
  else if ( whichLevel == COMPOSEWORKLOAD )
    return &intervalComposeWorkload[ whichOrder ];
  else if ( whichLevel == WORKLOAD )
    return &intervalWorkload[ whichOrder ];
  else if ( whichLevel == COMPOSEAPPLICATION )
    return &intervalComposeApplication[ whichOrder ];
  else if ( whichLevel == APPLICATION )
    return &intervalApplication[ whichOrder ];
  else if ( whichLevel == COMPOSETASK )
    return &intervalComposeTask[ whichOrder ];
  else if ( whichLevel == TASK )
    return &intervalTask[ whichOrder ];
  else if ( whichLevel == COMPOSETHREAD )
    return &intervalComposeThread[ whichOrder ];
  else if ( whichLevel == THREAD )
    return &intervalThread[ whichOrder ];
  else if ( whichLevel == COMPOSESYSTEM )
    return &intervalComposeSystem[ whichOrder ];
  else if ( whichLevel == SYSTEM )
    return &intervalSystem[ whichOrder ];
  else if ( whichLevel == COMPOSENODE )
    return &intervalComposeNode[ whichOrder ];
  else if ( whichLevel == NODE )
    return &intervalNode[ whichOrder ];
  else if ( whichLevel == COMPOSECPU )
    return &intervalComposeCPU[ whichOrder ];
  else if ( whichLevel == CPU )
    return &intervalCPU[ whichOrder ];
  return NULL;
}

SemanticInfoType KSingleWindow::getSemanticInfoType() const
{
  if ( functions[ TOPCOMPOSE1 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ TOPCOMPOSE1 ]->getSemanticInfoType();
  if ( functions[ TOPCOMPOSE2 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ TOPCOMPOSE2 ]->getSemanticInfoType();

  if ( level >= SYSTEM && level <= CPU )
  {
    switch ( level )
    {
      case SYSTEM:
        if ( functions[ COMPOSESYSTEM ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSESYSTEM ]->getSemanticInfoType();
        if ( functions[ SYSTEM ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ SYSTEM ]->getSemanticInfoType();
      case NODE:
        if ( functions[ COMPOSENODE ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSENODE ]->getSemanticInfoType();
        if ( functions[ NODE ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ NODE ]->getSemanticInfoType();
      case CPU:
        if ( functions[ COMPOSECPU ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSECPU ]->getSemanticInfoType();
        if ( functions[ CPU ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ CPU ]->getSemanticInfoType();
      default:
        break;
    }
  }
  else if ( level >= WORKLOAD && level <= THREAD )
  {
    switch ( level )
    {
      case WORKLOAD:
        if ( functions[ COMPOSEWORKLOAD ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSEWORKLOAD ]->getSemanticInfoType();
        if ( functions[ WORKLOAD ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ WORKLOAD ]->getSemanticInfoType();
      case APPLICATION:
        if ( functions[ COMPOSEAPPLICATION ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSEAPPLICATION ]->getSemanticInfoType();
        if ( functions[ APPLICATION ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ APPLICATION ]->getSemanticInfoType();
      case TASK:
        if ( functions[ COMPOSETASK ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ COMPOSETASK ]->getSemanticInfoType();
        if ( functions[ TASK ]->getSemanticInfoType() != SAME_TYPE )
          return functions[ TASK ]->getSemanticInfoType();
      default:
        break;
    }
  }
  if ( functions[ COMPOSETHREAD ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ COMPOSETHREAD ]->getSemanticInfoType();
  return functions[ THREAD ]->getSemanticInfoType();
}

/**********************************************************************
 *  KDerivedWindow implementation
 **********************************************************************/

void KDerivedWindow::setup()
{
  myTrace = ( KTrace* )parents[ 0 ]->getTrace();

  if ( functions[ 0 ] == NULL )
    functions[ 0 ] = new ComposeAsIs();
  else if ( functions[ 1 ] == NULL )
    functions[ 1 ] = new ComposeAsIs();

  intervalTopCompose1.clear();
  intervalTopCompose2.clear();
  intervalDerived.clear();
  intervalControlDerived.clear();
  if ( myTrace->totalThreads() > myTrace->totalCPUs() )
  {
    intervalTopCompose1.reserve( myTrace->totalThreads() );
    intervalTopCompose2.reserve( myTrace->totalThreads() );
    intervalDerived.reserve( myTrace->totalThreads() );
    intervalControlDerived.reserve( myTrace->totalThreads() );
    for ( TThreadOrder i = 0; i < myTrace->totalThreads(); i++ )
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
    for ( TCPUOrder i = 0; i < myTrace->totalCPUs(); i++ )
    {
      intervalTopCompose1.push_back( IntervalCompose( this, TOPCOMPOSE1, i ) );
      intervalTopCompose2.push_back( IntervalCompose( this, TOPCOMPOSE2, i ) );
      intervalDerived.push_back( IntervalDerived( this, DERIVED, i ) );
      intervalControlDerived.push_back( IntervalControlDerived( this, DERIVED, i ) );
    }
  }
}


bool KDerivedWindow::setLevelFunction( TWindowLevel whichLevel,
                                       const string& whichFunction )
{
  if ( whichLevel == TOPCOMPOSE1 )
    whichLevel = ( TWindowLevel ) 0;
  else if ( whichLevel == TOPCOMPOSE2 )
    whichLevel = ( TWindowLevel ) 1;
  else if ( whichLevel == DERIVED )
    whichLevel = ( TWindowLevel ) 2;
  else
    return false;

  if ( functions[ whichLevel ] != NULL )
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
  if ( whichLevel == TOPCOMPOSE1 )
    whichLevel = ( TWindowLevel ) 0;
  else if ( whichLevel == TOPCOMPOSE2 )
    whichLevel = ( TWindowLevel ) 1;
  else if ( whichLevel == DERIVED )
    whichLevel = ( TWindowLevel ) 2;

  return functions[ whichLevel ]->getName();
}

SemanticFunction *KDerivedWindow::getSemanticFunction( TWindowLevel whichLevel )
{
  if ( whichLevel == TOPCOMPOSE1 )
    whichLevel = ( TWindowLevel ) 0;
  else if ( whichLevel == TOPCOMPOSE2 )
    whichLevel = ( TWindowLevel ) 1;
  else if ( whichLevel == DERIVED )
    whichLevel = ( TWindowLevel ) 2;

  return functions[ whichLevel ];
}

string KDerivedWindow::getFirstUsefulFunction()
{
  if ( typeid( *functions[ ( TWindowLevel ) 0 ] ) != typeid( ComposeAsIs ) )
    return functions[ ( TWindowLevel ) 0 ]->getName();
  if ( typeid( *functions[ ( TWindowLevel ) 1 ] ) != typeid( ComposeAsIs ) )
    return functions[ ( TWindowLevel ) 1 ]->getName();

  return functions[ ( TWindowLevel ) 2 ]->getName();
}

SemanticFunction *KDerivedWindow::getFirstSemUsefulFunction()
{
  if ( typeid( *functions[ ( TWindowLevel ) 0 ] ) != typeid( ComposeAsIs ) )
    return functions[ ( TWindowLevel ) 0 ];
  if ( typeid( *functions[ ( TWindowLevel ) 1 ] ) != typeid( ComposeAsIs ) )
    return functions[ ( TWindowLevel ) 1 ];

  return functions[ ( TWindowLevel ) 2 ];
}

void KDerivedWindow::setFunctionParam( TWindowLevel whichLevel,
                                       TParamIndex whichParam,
                                       const TParamValue& newValue )
{
  if ( whichLevel == TOPCOMPOSE1 )
    whichLevel = ( TWindowLevel ) 0;
  else if ( whichLevel == TOPCOMPOSE2 )
    whichLevel = ( TWindowLevel ) 1;
  else if ( whichLevel == DERIVED )
    whichLevel = ( TWindowLevel ) 2;

  functions[ whichLevel ]->setParam( whichParam, newValue );
}


bool KDerivedWindow::initFromBegin() const
{
  bool tmp = false;

  tmp = tmp || functions[ 0 ]->getInitFromBegin();
  tmp = tmp || functions[ 1 ]->getInitFromBegin();
  tmp = tmp || functions[ 2 ]->getInitFromBegin();

  return tmp;
}


void KDerivedWindow::init( TRecordTime initialTime, TCreateList create )
{
  TObjectOrder objectSize = 0;
  TWindowLevel tmpLevel = getLevel();

  functions[ 0 ]->init( this );
  functions[ 1 ]->init( this );
  functions[ 2 ]->init( this );

  if ( tmpLevel == WORKLOAD )
    objectSize = 1;
  else if ( tmpLevel == APPLICATION )
    objectSize = myTrace->totalApplications();
  else if ( tmpLevel == TASK )
    objectSize = myTrace->totalTasks();
  else if ( tmpLevel == THREAD )
    objectSize = myTrace->totalThreads();
  else if ( tmpLevel == SYSTEM )
    objectSize = 1;
  else if ( tmpLevel == NODE )
    objectSize = myTrace->totalNodes();
  else if ( tmpLevel == CPU )
    objectSize = myTrace->totalCPUs();

  for ( UINT16 i = 0; i < parents.size(); i++ )
    parents[ i ]->init( initialTime, create );

  for ( TObjectOrder i = 0; i < objectSize; i++ )
    intervalTopCompose1[ i ].init( initialTime, create );
}


RecordList *KDerivedWindow::calcNext( TObjectOrder whichObject )
{
  return intervalTopCompose1[ whichObject ].calcNext();
}


RecordList *KDerivedWindow::calcPrev( TObjectOrder whichObject )
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
  if ( whichLevel == TOPCOMPOSE1 )
    return &intervalTopCompose1[ whichOrder ];
  else if ( whichLevel == TOPCOMPOSE2 )
    return &intervalTopCompose2[ whichOrder ];
  else if ( whichLevel == DERIVED )
  {
    if ( ( ( SemanticDerived * )functions[ 2 ] )->isControlDerived() )
      return &intervalControlDerived[ whichOrder ];
    else
      return &intervalDerived[ whichOrder ];
  }
  return NULL;
}

void KDerivedWindow::setParent( UINT16 whichParent, Window *whichWindow )
{
  parents[ whichParent ] = ( KWindow* )whichWindow;
  if ( parents[ 0 ] != NULL && parents[ 1 ] != NULL )
    setup();
}

Window *KDerivedWindow::getParent( UINT16 whichParent ) const
{
  return parents[whichParent];
}

TWindowLevel KDerivedWindow::getLevel() const
{
  TWindowLevel tmp = NONE;

  for ( UINT16 i = 0; i < parents.size(); i++ )
  {
    if ( parents[ i ]->getLevel() > tmp )
      tmp = parents[ i ]->getLevel();
  }

  return tmp;
}

SemanticInfoType KDerivedWindow::getSemanticInfoType() const
{
  if ( functions[ 0 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ 0 ]->getSemanticInfoType();
  if ( functions[ 1 ]->getSemanticInfoType() != SAME_TYPE )
    return functions[ 1 ]->getSemanticInfoType();

  return functions[ 2 ]->getSemanticInfoType();
}
