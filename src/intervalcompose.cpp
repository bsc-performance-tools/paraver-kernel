#include <typeinfo>
#include "intervalcompose.h"

RecordList *IntervalCompose::init( TRecordTime initialTime, TCreateList create,
                                   RecordList *displayList )
{
  TRecordTime myInitTime;
  SemanticHighInfo info;

  createList = create;
  currentValue = 0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticCompose * ) window->getLevelFunction( level );

  setChilds();

  if ( function->getInitFromBegin() )
    myInitTime = 0;
  else
    myInitTime = initialTime;

  childIntervals[ 0 ]->init( myInitTime, create, displayList );
  begin = childIntervals[ 0 ]->getBegin();
  end = childIntervals[ 0 ]->getEnd();
  info.values.push_back( childIntervals[ 0 ]->getValue() );
  info.callingInterval = this;
  currentValue = function->execute( &info );

  if ( function->getInitFromBegin() )
  {
    while ( end->getTime() <= initialTime )
      calcNext( displayList );
  }

  return displayList;
}


RecordList *IntervalCompose::calcNext( RecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  childIntervals[ 0 ]->calcNext( displayList );
  begin = childIntervals[ 0 ]->getBegin();
  end = childIntervals[ 0 ]->getEnd();
  info.values.push_back( childIntervals[ 0 ]->getValue() );
  info.callingInterval = this;
  currentValue = function->execute( &info );

  return displayList;
}


RecordList *IntervalCompose::calcPrev( RecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  childIntervals[ 0 ]->calcPrev( displayList );
  begin = childIntervals[ 0 ]->getBegin();
  end = childIntervals[ 0 ]->getEnd();
  info.values.push_back( childIntervals[ 0 ]->getValue() );
  info.callingInterval = this;
  currentValue = function->execute( &info );

  return displayList;
}


TWindowLevel IntervalCompose::getWindowLevel() const
{
  return window->getLevel();
}


Interval *IntervalCompose::getWindowInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}


bool IntervalCompose::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}


TWindowLevel IntervalCompose::getComposeLevel( TWindowLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


Trace *IntervalCompose::getWindowTrace() const
{
  return window->getTrace();
}
