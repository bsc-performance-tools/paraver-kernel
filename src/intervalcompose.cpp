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


#include "intervalcompose.h"
#include "kwindow.h"
#include "memorytrace.h"
#include "semanticcomposefunctions.h"

KRecordList *IntervalCompose::init( TRecordTime initialTime, TCreateList create,
                                    KRecordList *displayList )
{
  TRecordTime myInitTime;
  SemanticHighInfo info;
  info.callingInterval = this;

  createList = create;
  currentValue = 0.0;

  if ( displayList == nullptr )
  {
    displayList = &myDisplayList;
    displayList->clear();
  }

  if( !notWindowInits )
    function = ( SemanticCompose * ) window->getSemanticFunction( level );

  if ( endRecord != nullptr )
  {
    delete endRecord;
    endRecord = nullptr;
  }
  if ( beginRecord != nullptr )
  {
    delete beginRecord;
    beginRecord = nullptr;
  }

  if ( typeid( *function ) == typeid( ComposeJoinBursts ) )
  {
    behaviour = TBehaviour::JOIN_BURSTS;
    endRecord = ( ( KSingleWindow * ) window )->getEndRecord();
    beginRecord = ( ( KSingleWindow * ) window )->getBeginRecord();
  }
  else if ( typeid( *function ) == typeid( ComposeTimer ) )
    behaviour = TBehaviour::TIMER;
  else
    behaviour = TBehaviour::REGULAR;

  if( !notWindowInits )
    setChildren();

  if ( function->getInitFromBegin() )
    myInitTime = 0.0;
  else
    myInitTime = initialTime;

  childIntervals[ 0 ]->init( myInitTime, create, displayList );

  if( begin != nullptr && begin != beginRecord )
    delete begin;
  begin = childIntervals[ 0 ]->getBegin()->clone();

  if( end != nullptr )
    delete end;
  end = childIntervals[ 0 ]->getEnd()->clone();

  switch( behaviour )
  {
    case TBehaviour::REGULAR:
      info.values.push_back( childIntervals[ 0 ]->getValue() );
      currentValue = function->execute( &info );
      break;

    case TBehaviour::JOIN_BURSTS:
      initJoinBursts( displayList );
      break;

    case TBehaviour::TIMER:
      initTimer( displayList );
      break;

    default:
      break;
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
  info.callingInterval = this;

  if ( displayList == nullptr )
    displayList = &myDisplayList;

  switch( behaviour )
  {
    case TBehaviour::REGULAR:
      childIntervals[ 0 ]->calcNext( displayList );
      *begin = *childIntervals[ 0 ]->getBegin();
      *end = *childIntervals[ 0 ]->getEnd();

      info.values.push_back( childIntervals[ 0 ]->getValue() );
      currentValue = function->execute( &info );
      break;
      
    case TBehaviour::JOIN_BURSTS:
      calcNextJoinBursts( displayList );
      break;
    
    case TBehaviour::TIMER:
      calcNextTimer( displayList );
      break;

    default:
      break;
  }
  
  return displayList;
}


KRecordList *IntervalCompose::calcPrev( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;
  info.callingInterval = this;

  if ( displayList == nullptr )
    displayList = &myDisplayList;

  switch( behaviour )
  {
    case TBehaviour::REGULAR:
      childIntervals[ 0 ]->calcPrev( displayList );
      *begin = *childIntervals[ 0 ]->getBegin();
      *end = *childIntervals[ 0 ]->getEnd();

      info.values.push_back( childIntervals[ 0 ]->getValue() );
      currentValue = function->execute( &info );
      break;
      
    case TBehaviour::JOIN_BURSTS:
      calcPrevJoinBursts( displayList );
      break;
    
    case TBehaviour::TIMER:
      calcPrevTimer( displayList );
      break;

    default:
      break;
  }

  return displayList;
}


void IntervalCompose::initJoinBursts( KRecordList *displayList )
{
  MemoryTrace::iterator *lastEnd = endRecord->clone();

  currentValue = childIntervals[ 0 ]->getValue();
  childIntervals[ 0 ]->calcNext( displayList );
  while ( currentValue == childIntervals[ 0 ]->getValue() )
  {
    *end = *childIntervals[ 0 ]->getEnd();

    if (( *end == *endRecord ) || ( *end == *lastEnd ))
      break;

    // lastEnd to control loop!
    *lastEnd = *end;

    childIntervals[ 0 ]->calcNext( displayList );
  }

  if( lastEnd != nullptr )
    delete lastEnd;
}

void IntervalCompose::calcNextJoinBursts( KRecordList *displayList )
{
  *begin = *childIntervals[ 0 ]->getBegin();
  *end = *childIntervals[ 0 ]->getEnd();
  currentValue = childIntervals[ 0 ]->getValue();

  childIntervals[ 0 ]->calcNext( displayList );

  if ( *end == *endRecord )
    return;

  MemoryTrace::iterator *lastEnd = endRecord->clone();

  while ( currentValue == childIntervals[ 0 ]->getValue() )
  {
    *end = *childIntervals[ 0 ]->getEnd();

    if (( *end == *endRecord ) || ( *end == *lastEnd ))
      break;

    // lastEnd to control loop!
    *lastEnd = *end;

    childIntervals[ 0 ]->calcNext( displayList );
  }

  if( lastEnd != nullptr )
    delete lastEnd;
}

void IntervalCompose::calcPrevJoinBursts( KRecordList *displayList )
{
  MemoryTrace::iterator *firstBegin = beginRecord->clone();

  *begin = *childIntervals[ 0 ]->getBegin();
  *end = *childIntervals[ 0 ]->getEnd();
  currentValue = childIntervals[ 0 ]->getValue();
  childIntervals[ 0 ]->calcPrev( displayList );
  while ( currentValue == childIntervals[ 0 ]->getValue() )
  {
    *begin = *childIntervals[ 0 ]->getBegin();

    if (( *begin == *beginRecord ) || ( *begin == *firstBegin ))
      break;

    // firstBegin to control loop!
    *firstBegin = *begin;

    childIntervals[ 0 ]->calcPrev( displayList );
  }

  if( firstBegin != nullptr )
    delete firstBegin;
}

void IntervalCompose::initTimer( KRecordList *displayList )
{
  currentValue = childIntervals[ 0 ]->getValue();

  accumulatedDeltas = 0.0;
  if( currentValue != 0 )
  {
    timerBeginTime = begin->getTime();
    accumulatedDeltas += end->getTime() - begin->getTime();
    if( accumulatedDeltas > function->getParam( ComposeTimer::DELTA_TIME )[ 0 ] )
    {
      copyRecordContent( end );
      virtualRecord.time = timerBeginTime + function->getParam( ComposeTimer::DELTA_TIME )[ 0 ];
      end->setRecord( &virtualRecord );
      accumulatedDeltas = 0.0;
    }
  }
}

void IntervalCompose::calcNextTimer( KRecordList *displayList )
{
  if( end->getRecord() == &virtualRecord )
  {
    // Stage 2: reset zone to child interval end
    currentValue = 0.0;
    *end = *childIntervals[ 0 ]->getEnd();
    begin->setRecord( &virtualRecord );
    return;
  }

  childIntervals[ 0 ]->calcNext( displayList );
  currentValue = childIntervals[ 0 ]->getValue();
  *begin = *childIntervals[ 0 ]->getBegin();
  *end = *childIntervals[ 0 ]->getEnd();

  if( currentValue != 0 )
  {
    if( accumulatedDeltas == 0.0 )
      timerBeginTime = begin->getTime();
    accumulatedDeltas += end->getTime() - begin->getTime();
  }

  if( currentValue != 0 &&
      accumulatedDeltas > function->getParam( ComposeTimer::DELTA_TIME )[ 0 ] )
  {
    // Stage 1: current value zone [ beginning, +DELTA_TIME ]
    copyRecordContent( end );
    virtualRecord.time = timerBeginTime + function->getParam( ComposeTimer::DELTA_TIME )[ 0 ];
    end->setRecord( &virtualRecord );
    accumulatedDeltas = 0.0;
  }
  else if( currentValue == 0 ||
           accumulatedDeltas == function->getParam( ComposeTimer::DELTA_TIME )[ 0 ] )
    accumulatedDeltas = 0.0;
}

void IntervalCompose::calcPrevTimer( KRecordList *displayList )
{
  // Not needed because ComposeTimer has initFromBegin == true
}


void IntervalCompose::setChildren()
{
  if ( level == TOPCOMPOSE1 )
  {
    if ( lastLevel != TOPCOMPOSE2 )
    {
      lastLevel = TOPCOMPOSE2;
      childIntervals.push_back( getWindowInterval( TOPCOMPOSE2, order ) );
    }
  }
  else if ( level == TOPCOMPOSE2 )
  {
    if ( lastLevel != static_cast<TWindowLevel>( getWindowLevel() ) )
    {
      childIntervals.clear();
      lastLevel = static_cast<TWindowLevel>( getWindowLevel() );
      childIntervals.push_back( getWindowInterval(
                                getComposeLevel( getWindowLevel() ), order ) );
    }
  }
  else if ( level == COMPOSEWORKLOAD )
  {
    if ( lastLevel != WORKLOAD || window->isDerivedWindow() )
    {
      if ( window->isDerivedWindow() )
        childIntervals.clear();

      lastLevel = WORKLOAD;
      childIntervals.push_back( getWindowInterval( WORKLOAD, order ) );
    }
  }
  else if ( level == COMPOSEAPPLICATION )
  {
    if ( lastLevel != APPLICATION || window->isDerivedWindow() )
    {
      if ( window->isDerivedWindow() )
        childIntervals.clear();

      lastLevel = APPLICATION;
      childIntervals.push_back( getWindowInterval( APPLICATION, order ) );
    }
  }
  else if ( level == COMPOSETASK )
  {
    if ( lastLevel != TASK || window->isDerivedWindow() )
    {
      if ( window->isDerivedWindow() )
        childIntervals.clear();

      lastLevel = TASK;
      childIntervals.push_back( getWindowInterval( TASK, order ) );
    }
  }
  else if ( level == COMPOSETHREAD )
  {
    if ( lastLevel != THREAD || window->isDerivedWindow() )
    {

      lastLevel = THREAD;
      if ( window->isDerivedWindow() )
        childIntervals.clear();

      childIntervals.push_back( getWindowInterval( THREAD, order ) );
    }
  }
  else if ( level == COMPOSESYSTEM )
  {
    if ( lastLevel != SYSTEM || window->isDerivedWindow() )
    {
      if ( window->isDerivedWindow() )
        childIntervals.clear();

      lastLevel = SYSTEM;
      childIntervals.push_back( getWindowInterval( SYSTEM, order ) );
    }
  }
  else if ( level == COMPOSENODE )
  {
    if ( lastLevel != NODE || window->isDerivedWindow() )
    {
      if ( window->isDerivedWindow() )
        childIntervals.clear();

      lastLevel = NODE;
      childIntervals.push_back( getWindowInterval( NODE, order ) );
    }
  }
  else if ( level == COMPOSECPU )
  {
    if ( lastLevel != CPU || window->isDerivedWindow() )
    {
      if ( window->isDerivedWindow() )
        childIntervals.clear();

      lastLevel = CPU;
      childIntervals.push_back( getWindowInterval( CPU, order ) );
    }
  }
}



TTraceLevel IntervalCompose::getWindowLevel() const
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


TWindowLevel IntervalCompose::getComposeLevel( TTraceLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


KTrace *IntervalCompose::getWindowTrace() const
{
  return (KTrace*)window->getTrace();
}

void IntervalCompose::copyRecordContent( MemoryTrace::iterator *whichRecord )
{
  virtualRecord.type   = whichRecord->getType();
  virtualRecord.time   = whichRecord->getTime();
  virtualRecord.thread = whichRecord->getThread();
  virtualRecord.CPU    = whichRecord->getCPU();
  if( virtualRecord.type & STATE )
  {
    virtualRecord.URecordInfo.stateRecord.state   = whichRecord->getState();
    virtualRecord.URecordInfo.stateRecord.endTime = whichRecord->getStateEndTime();
  }
  else if( virtualRecord.type & EVENT )
  {
    virtualRecord.URecordInfo.eventRecord.type  = whichRecord->getEventType();
    virtualRecord.URecordInfo.eventRecord.value = whichRecord->getEventValueAsIs();
  }
  else if( virtualRecord.type & COMM )
  {
    virtualRecord.URecordInfo.commRecord.index = whichRecord->getCommIndex();
  }
}
