#include "kwindow.h"
#include "intervalcompose.h"
#include "semanticcomposefunctions.h"

KRecordList *IntervalCompose::init( TRecordTime initialTime, TCreateList create,
                                    KRecordList *displayList )
{
  TRecordTime myInitTime;
  SemanticHighInfo info;

  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticCompose * ) window->getLevelFunction( level );

  if ( endRecord != NULL )
  {
    delete endRecord;
    endRecord = NULL;
  }
  if ( beginRecord != NULL )
  {
    delete beginRecord;
    beginRecord = NULL;
  }

  if ( typeid( *function ) == typeid( ComposeJoinBursts ) )
  {
    joinBursts = true;
    endRecord = ( ( KSingleWindow * ) window )->getThreadEndRecord( order );
    beginRecord = ( ( KSingleWindow * ) window )->getThreadBeginRecord( order );
  }
  else
    joinBursts = false;

  setChilds();

  if ( function->getInitFromBegin() )
    myInitTime = 0.0;
  else
    myInitTime = initialTime;

  childIntervals[ 0 ]->init( myInitTime, create, displayList );

  if ( joinBursts )
  {
    TSemanticValue tmpValue;

    begin = childIntervals[ 0 ]->getBegin();
    end = childIntervals[ 0 ]->getEnd();
    tmpValue = childIntervals[ 0 ]->getValue();
    childIntervals[ 0 ]->calcNext( displayList );
    while ( tmpValue == childIntervals[ 0 ]->getValue() )
    {
      end = childIntervals[ 0 ]->getEnd();

      if ( *end == *endRecord )
        break;

      childIntervals[ 0 ]->calcNext( displayList );
    }
    currentValue = tmpValue;
  }
  else
  {
    begin = childIntervals[ 0 ]->getBegin();
    end = childIntervals[ 0 ]->getEnd();
    info.values.push_back( childIntervals[ 0 ]->getValue() );
    info.callingInterval = this;
    currentValue = function->execute( &info );
  }

  if ( function->getInitFromBegin() )
  {
    while ( end->getTime() <= initialTime )
      calcNext( displayList );
  }

  return displayList;
}


KRecordList *IntervalCompose::calcNext( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( joinBursts )
  {
    TSemanticValue tmpValue;

    begin = childIntervals[ 0 ]->getBegin();
    end = childIntervals[ 0 ]->getEnd();
    tmpValue = childIntervals[ 0 ]->getValue();
    childIntervals[ 0 ]->calcNext( displayList );
    while ( tmpValue == childIntervals[ 0 ]->getValue() )
    {
      end = childIntervals[ 0 ]->getEnd();

      if ( *end == *endRecord )
        break;

      childIntervals[ 0 ]->calcNext( displayList );
    }
    currentValue = tmpValue;
  }
  else
  {
    childIntervals[ 0 ]->calcNext( displayList );
    begin = childIntervals[ 0 ]->getBegin();
    end = childIntervals[ 0 ]->getEnd();
    info.values.push_back( childIntervals[ 0 ]->getValue() );
    info.callingInterval = this;
    currentValue = function->execute( &info );
  }

  return displayList;
}


KRecordList *IntervalCompose::calcPrev( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( joinBursts )
  {
    TSemanticValue tmpValue;

    begin = childIntervals[ 0 ]->getBegin();
    end = childIntervals[ 0 ]->getEnd();
    tmpValue = childIntervals[ 0 ]->getValue();
    childIntervals[ 0 ]->calcPrev( displayList );
    while ( tmpValue == childIntervals[ 0 ]->getValue() )
    {
      begin = childIntervals[ 0 ]->getBegin();

      if ( *begin == *beginRecord )
        break;

      childIntervals[ 0 ]->calcPrev( displayList );
    }
  }
  else
  {
    childIntervals[ 0 ]->calcPrev( displayList );
    begin = childIntervals[ 0 ]->getBegin();
    end = childIntervals[ 0 ]->getEnd();
    info.values.push_back( childIntervals[ 0 ]->getValue() );
    info.callingInterval = this;
    currentValue = function->execute( &info );
  }

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
