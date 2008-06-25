#include "kwindow.h"
#include "intervalnotthread.h"

KRecordList *IntervalNotThread::init( TRecordTime initialTime, TCreateList create,
                                     KRecordList *displayList )
{
  TRecordTime myInitTime;
  SemanticHighInfo info;

  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticNotThread * ) window->getLevelFunction( level );

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

  for ( TObjectOrder i = 0; i < childIntervals.size(); i++ )
  {
    childIntervals[ i ]->init( myInitTime, createList, displayList );

    if ( begin == NULL ||
         childIntervals[ i ]->getBegin()->getTime() > begin->getTime() )
    {
      if ( begin != NULL )
        delete begin;
      if ( level >= SYSTEM )
        begin = window->copyCPUIterator( childIntervals[ i ]->getBegin() );
      else
        begin = window->copyThreadIterator( childIntervals[ i ]->getBegin() );
    }

    if ( end == NULL ||
         childIntervals[ i ]->getEnd()->getTime() < end->getTime() )
    {
      if ( end != NULL )
        delete end;
      if ( level >= SYSTEM )
        end = window->copyCPUIterator( childIntervals[ i ]->getEnd() );
      else
        end = window->copyThreadIterator( childIntervals[ i ]->getEnd() );
    }

    info.values.push_back( childIntervals[ i ]->getValue() );
  }
  currentValue = function->execute( &info );

  while ( end->getTime() < initialTime )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalNotThread::calcNext( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( begin != NULL )
    delete begin;
  if ( level >= SYSTEM )
    begin = window->copyCPUIterator( end );
  else
    begin = window->copyThreadIterator( end );

  if( end != NULL )
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
      if ( level >= SYSTEM )
        end = window->copyCPUIterator( childIntervals[ i ]->getEnd() );
      else
        end = window->copyThreadIterator( childIntervals[ i ]->getEnd() );
    }

    info.values.push_back( childIntervals[ i ]->getValue() );
  }
  currentValue = function->execute( &info );

  return displayList;
}


KRecordList *IntervalNotThread::calcPrev( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( end != NULL )
    delete end;
  if ( level >= SYSTEM )
    end = window->copyCPUIterator( begin );
  else
    end = window->copyThreadIterator( begin );

  if( begin != NULL )
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
      if ( level >= SYSTEM )
        begin = window->copyCPUIterator( childIntervals[ i ]->getBegin() );
      else
        begin = window->copyThreadIterator( childIntervals[ i ]->getBegin() );
    }

    info.values.push_back( childIntervals[ i ]->getValue() );
  }
  currentValue = function->execute( &info );

  return displayList;
}


TWindowLevel IntervalNotThread::getWindowLevel() const
{
  return window->getLevel();
}


Interval *IntervalNotThread::getWindowInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}


bool IntervalNotThread::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}


TWindowLevel IntervalNotThread::getComposeLevel( TWindowLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


KTrace *IntervalNotThread::getWindowTrace() const
{
  return window->getTrace();
}
