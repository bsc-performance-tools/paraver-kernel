#include "kwindow.h"
#include "intervalderived.h"

RecordList *IntervalDerived::init( TRecordTime initialTime, TCreateList create,
                                   RecordList *displayList )
{
  TRecordTime myInitTime;
  SemanticHighInfo info;

  createList = create;
  currentValue = 0;

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
    myInitTime = 0;
  else
    myInitTime = initialTime;

  info.callingInterval = this;

  for ( TObjectOrder i = 0; i < childIntervals.size(); i++ )
  {
    childIntervals[ i ]->init( myInitTime, createList, displayList );

    if ( begin == NULL ||
         childIntervals[ i ]->getBegin()->getTime() > begin->getTime() )
    {
      if ( begin != NULL )
        delete begin;
      if ( window->getLevel() >= SYSTEM )
        begin = window->copyCPUIterator( childIntervals[ i ]->getBegin() );
      else
        begin = window->copyThreadIterator( childIntervals[ i ]->getBegin() );
    }

    if ( end == NULL ||
         childIntervals[ i ]->getEnd()->getTime() < end->getTime() )
    {
      if ( end != NULL )
        delete end;
      if ( window->getLevel() >= SYSTEM )
        end = window->copyCPUIterator( childIntervals[ i ]->getEnd() );
      else
        end = window->copyThreadIterator( childIntervals[ i ]->getEnd() );
    }

    info.values.push_back( childIntervals[ i ]->getValue() *
                           window->getFactor( i ) );
  }
  currentValue = function->execute( &info );

  while ( end->getTime() < initialTime )
    calcNext( displayList );

  return displayList;
}


RecordList *IntervalDerived::calcNext( RecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( begin != NULL )
    delete begin;
  if ( window->getLevel() >= SYSTEM )
    begin = window->copyCPUIterator( end );
  else
    begin = window->copyThreadIterator( end );

  if ( end != NULL )
  {
    delete end;
    end = NULL;
  }

  info.callingInterval = this;

  for ( TObjectOrder i = 0; i < childIntervals.size(); i++ )
  {
    if ( childIntervals[ i ]->getEnd()->getTime() <= begin->getTime() )
      childIntervals[ i ]->calcNext( displayList );

    if ( end == NULL ||
         childIntervals[ i ]->getEnd()->getTime() < end->getTime() )
    {
      if ( end != NULL )
        delete end;
      if ( window->getLevel() >= SYSTEM )
        end = window->copyCPUIterator( childIntervals[ i ]->getEnd() );
      else
        end = window->copyThreadIterator( childIntervals[ i ]->getEnd() );
    }

    info.values.push_back( childIntervals[ i ]->getValue() *
                           window->getFactor( i ) );
  }
  currentValue = function->execute( &info );

  return displayList;
}


RecordList *IntervalDerived::calcPrev( RecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( end != NULL )
    delete end;
  if ( window->getLevel() >= SYSTEM )
    end = window->copyCPUIterator( begin );
  else
    end = window->copyThreadIterator( begin );

  if ( begin != NULL )
  {
    delete begin;
    begin = NULL;
  }

  info.callingInterval = this;

  for ( TObjectOrder i = 0; i < childIntervals.size(); i++ )
  {
    if ( childIntervals[ i ]->getBegin()->getTime() >= end->getTime() )
      childIntervals[ i ]->calcPrev( displayList );

    if ( begin == NULL ||
         childIntervals[ i ]->getBegin()->getTime() > begin->getTime() )
    {
      if ( begin != NULL )
        delete begin;
      if ( window->getLevel() >= SYSTEM )
        begin = window->copyCPUIterator( childIntervals[ i ]->getBegin() );
      else
        begin = window->copyThreadIterator( childIntervals[ i ]->getBegin() );
    }

    info.values.push_back( childIntervals[ i ]->getValue() *
                           window->getFactor( i ) );
  }
  currentValue = function->execute( &info );

  return displayList;
}


TWindowLevel IntervalDerived::getWindowLevel() const
{
  return window->getLevel();
}


Interval *IntervalDerived::getWindowInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}


bool IntervalDerived::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}


TWindowLevel IntervalDerived::getComposeLevel( TWindowLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


Trace *IntervalDerived::getWindowTrace() const
{
  return window->getTrace();
}

void IntervalDerived::setChilds()
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
    window1 = window->getParent( 0 );
    window2 = window->getParent( 1 );
  }
  else
  {
    window1 = window->getParent( 1 );
    window2 = window->getParent( 0 );
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
