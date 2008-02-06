#include "kwindow.h"
#include "intervalcontrolderived.h"

RecordList *IntervalControlDerived::init( TRecordTime initialTime, TCreateList create,
    RecordList *displayList )
{
  TRecordTime myInitTime;
  SemanticHighInfo info;

  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticDerived * ) window->getLevelFunction( level );

  setChilds();

  if ( begin != NULL )
  {
    delete begin;
    begin = NULL;
  }
  if ( end != NULL )
  {
    delete end;
    end = NULL;
  }

  if ( function->getInitFromBegin() )
    myInitTime = 0.0;
  else
    myInitTime = initialTime;

  info.callingInterval = this;

  childIntervals[ 0 ]->init( myInitTime, createList, displayList );
  childIntervals[ 1 ]->init( myInitTime, createList, displayList );

  if ( window->getLevel() >= SYSTEM )
  {
    begin = window->copyCPUIterator( childIntervals[ 0 ]->getBegin() );
    end = window->copyCPUIterator( childIntervals[ 0 ]->getEnd() );
  }
  else
  {
    begin = window->copyThreadIterator( childIntervals[ 0 ]->getBegin() );
    end = window->copyThreadIterator( childIntervals[ 0 ]->getEnd() );
  }

  while( childIntervals[ 1 ]->getEnd()->getTime() <= begin->getTime() )
    childIntervals[ 1 ]->calcNext( displayList );

  if( childIntervals[ 1 ]->getEnd()->getTime() > begin->getTime() )
  {
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 1 ]->getValue() *
                           window->getFactor( 1 ) );
    currentValue = function->execute( &info );
  }

  while( childIntervals[ 1 ]->getEnd()->getTime() < end->getTime() )
  {
    childIntervals[ 1 ]->calcNext( displayList );
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 1 ]->getValue() *
                           window->getFactor( 1 ) );
    currentValue = function->execute( &info );
  }

  while ( end->getTime() < initialTime )
    calcNext( displayList );

  return displayList;
}


RecordList *IntervalControlDerived::calcNext( RecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  currentValue = 0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( begin != NULL )
  {
    delete begin;
    begin = NULL;
  }
  if ( end != NULL )
  {
    delete end;
    end = NULL;
  }

  info.callingInterval = this;

  childIntervals[ 0 ]->calcNext( displayList );

  if ( window->getLevel() >= SYSTEM )
  {
    begin = window->copyCPUIterator( childIntervals[ 0 ]->getBegin() );
    end = window->copyCPUIterator( childIntervals[ 0 ]->getEnd() );
  }
  else
  {
    begin = window->copyThreadIterator( childIntervals[ 0 ]->getBegin() );
    end = window->copyThreadIterator( childIntervals[ 0 ]->getEnd() );
  }

  while( childIntervals[ 1 ]->getEnd()->getTime() <= begin->getTime() )
    childIntervals[ 1 ]->calcNext( displayList );

  if( childIntervals[ 1 ]->getEnd()->getTime() > begin->getTime() )
  {
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 1 ]->getValue() *
                           window->getFactor( 1 ) );
    currentValue = function->execute( &info );
  }

  while( childIntervals[ 1 ]->getEnd()->getTime() < end->getTime() )
  {
    childIntervals[ 1 ]->calcNext( displayList );
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 1 ]->getValue() *
                           window->getFactor( 1 ) );
    currentValue = function->execute( &info );
  }

  return displayList;
}


RecordList *IntervalControlDerived::calcPrev( RecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  currentValue = 0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( begin != NULL )
  {
    delete begin;
    begin = NULL;
  }
  if ( end != NULL )
  {
    delete end;
    end = NULL;
  }

  info.callingInterval = this;

  childIntervals[ 0 ]->calcPrev( displayList );

  if ( window->getLevel() >= SYSTEM )
  {
    begin = window->copyCPUIterator( childIntervals[ 0 ]->getBegin() );
    end = window->copyCPUIterator( childIntervals[ 0 ]->getEnd() );
  }
  else
  {
    begin = window->copyThreadIterator( childIntervals[ 0 ]->getBegin() );
    end = window->copyThreadIterator( childIntervals[ 0 ]->getEnd() );
  }

  while( childIntervals[ 1 ]->getEnd()->getTime() <= begin->getTime() )
    childIntervals[ 1 ]->calcPrev( displayList );

  if( childIntervals[ 1 ]->getEnd()->getTime() > begin->getTime() )
  {
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 1 ]->getValue() *
                           window->getFactor( 1 ) );
    currentValue = function->execute( &info );
  }

  while( childIntervals[ 1 ]->getEnd()->getTime() < end->getTime() )
  {
    childIntervals[ 1 ]->calcPrev( displayList );
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 1 ]->getValue() *
                           window->getFactor( 1 ) );
    currentValue = function->execute( &info );
  }

  return displayList;
}


TWindowLevel IntervalControlDerived::getWindowLevel() const
{
  return window->getLevel();
}


Interval *IntervalControlDerived::getWindowInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}


bool IntervalControlDerived::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}


TWindowLevel IntervalControlDerived::getComposeLevel( TWindowLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


Trace *IntervalControlDerived::getWindowTrace() const
{
  return window->getTrace();
}

void IntervalControlDerived::setChilds()
{
  KWindow *window1;
  KWindow *window2;
  TApplOrder tmpAppl;
  TTaskOrder tmpTask;
  TThreadOrder tmpThread;
  TNodeOrder tmpNode;
  TCPUOrder tmpCPU;

  childIntervals.clear();

  if ( window->getParent( 0 )->getLevel() > window->getParent( 1 )->getLevel() )
  {
    window1 = (KWindow *) window->getParent( 0 );
    window2 = (KWindow *) window->getParent( 1 );
  }
  else
  {
    window1 = (KWindow *) window->getParent( 1 );
    window2 = (KWindow *) window->getParent( 0 );
  }

  if ( window1->getLevel() == APPLICATION )
  {
    tmpAppl = order;
  }
  else if ( window1->getLevel() == TASK )
  {
    window1->getTrace()->getTaskLocation( order, tmpAppl, tmpTask );
  }
  else if ( window1->getLevel() == THREAD )
  {
    window1->getTrace()->getThreadLocation( order, tmpAppl, tmpTask, tmpThread );
  }
  else if ( window1->getLevel() == NODE )
  {
    tmpNode = order;
  }
  else if ( window1->getLevel() == CPU )
  {
    window1->getTrace()->getCPULocation( order, tmpNode, tmpCPU );
  }

  if ( window1 == window->getParent( 0 ) )
    childIntervals.push_back( window1->getLevelInterval( TOPCOMPOSE1, order ) );

  if ( window2->getLevel() == WORKLOAD )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, 0 ) );
  }
  else if ( window2->getLevel() == APPLICATION )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, tmpAppl ) );
  }
  else if ( window2->getLevel() == TASK )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                              window2->getTrace()->getGlobalTask( tmpAppl, tmpTask ) ) );
  }
  else if ( window2->getLevel() == THREAD )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                              window2->getTrace()->getGlobalThread( tmpAppl, tmpTask, tmpThread ) ) );
  }
  else if ( window2->getLevel() == SYSTEM )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, 0 ) );
  }
  else if ( window2->getLevel() == NODE )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, tmpNode ) );
  }
  else if ( window2->getLevel() == CPU )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                              window2->getTrace()->getGlobalCPU( tmpNode, tmpCPU ) ) );
  }

  if ( window1 == window->getParent( 1 ) )
    childIntervals.push_back( window1->getLevelInterval( TOPCOMPOSE1, order ) );

}
