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

#include "kwindow.h"
#include "intervalcompose.h"
#include "semanticcomposefunctions.h"

KRecordList *IntervalCompose::init( TRecordTime initialTime, TCreateList create,
                                    KRecordList *displayList )
{
  TRecordTime myInitTime;
  SemanticHighInfo info;
  info.callingInterval = this;

  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
  {
    displayList = &myDisplayList;
    displayList->clear();
  }

  if( !notWindowInits )
    function = ( SemanticCompose * ) window->getSemanticFunction( level );

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
    endRecord = ( ( KSingleWindow * ) window )->getEndRecord();
    beginRecord = ( ( KSingleWindow * ) window )->getBeginRecord();
  }
  else
  {
    joinBursts = false;
  }

  if( !notWindowInits )
    setChilds();

  if ( function->getInitFromBegin() )
    myInitTime = 0.0;
  else
    myInitTime = initialTime;

  childIntervals[ 0 ]->init( myInitTime, create, displayList );

  if( begin != NULL && begin != beginRecord )
    delete begin;
  begin = childIntervals[ 0 ]->getBegin()->clone();

  if( end != NULL )
    delete end;
  end = childIntervals[ 0 ]->getEnd()->clone();

  if ( joinBursts )
  {
    TSemanticValue tmpValue;
    MemoryTrace::iterator *lastEnd = endRecord;

    tmpValue = childIntervals[ 0 ]->getValue();
    childIntervals[ 0 ]->calcNext( displayList );
    while ( tmpValue == childIntervals[ 0 ]->getValue() )
    {
      if( end != NULL )
        delete end;
      end = childIntervals[ 0 ]->getEnd()->clone();

      // somehow, this break never is executed
      // if ( *end == *endRecord )
      if (( *end == *endRecord ) || ( *end == *lastEnd ))
        break;

      // lastEnd to control loop!
      if( lastEnd != NULL && lastEnd != endRecord )
        delete lastEnd;
      lastEnd = end->clone();

      childIntervals[ 0 ]->calcNext( displayList );
    }
    currentValue = tmpValue;
    if( lastEnd != NULL && lastEnd != endRecord )
      delete lastEnd;
  }
  else
  {
    info.values.push_back( childIntervals[ 0 ]->getValue() );
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
  info.callingInterval = this;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( joinBursts )
  {
    TSemanticValue tmpValue;
    MemoryTrace::iterator *lastEnd;

    // lastEnd to control loop! Initialized with endRecord.
    lastEnd = endRecord;

    if( begin != NULL && begin != beginRecord )
      delete begin;
    begin = childIntervals[ 0 ]->getBegin()->clone();

    if( end != NULL )
      delete end;
    end = childIntervals[ 0 ]->getEnd()->clone();

    tmpValue = childIntervals[ 0 ]->getValue();
    if ( *end == *endRecord )
    {
      currentValue = tmpValue;
      return displayList;
    }

    childIntervals[ 0 ]->calcNext( displayList );

    while ( tmpValue == childIntervals[ 0 ]->getValue() )
    {
      if( end != NULL )
        delete end;
      end = childIntervals[ 0 ]->getEnd()->clone();

      // somehow, this break never is executed
      // if ( *end == *endRecord )
      if (( *end == *endRecord ) || ( *end == *lastEnd ))
        break;

      // lastEnd to control loop!
      if( lastEnd != NULL && lastEnd != endRecord )
        delete lastEnd;
      lastEnd = end->clone();

      childIntervals[ 0 ]->calcNext( displayList );
    }
    currentValue = tmpValue;
    if( lastEnd != NULL && lastEnd != endRecord )
      delete lastEnd;
  }
  else
  {
    childIntervals[ 0 ]->calcNext( displayList );
    if( begin != NULL && begin != beginRecord )
      delete begin;
    begin = childIntervals[ 0 ]->getBegin()->clone();

    if( end != NULL )
      delete end;
    end = childIntervals[ 0 ]->getEnd()->clone();

    info.values.push_back( childIntervals[ 0 ]->getValue() );
    currentValue = function->execute( &info );
  }

  return displayList;
}


KRecordList *IntervalCompose::calcPrev( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;
  info.callingInterval = this;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( joinBursts )
  {
    TSemanticValue tmpValue;
    MemoryTrace::iterator *firstBegin;

    // firstBegin to control loop! Initialized with beginRecord.
    firstBegin = beginRecord;

    begin = childIntervals[ 0 ]->getBegin();
    end = childIntervals[ 0 ]->getEnd();
    tmpValue = childIntervals[ 0 ]->getValue();
    childIntervals[ 0 ]->calcPrev( displayList );
    while ( tmpValue == childIntervals[ 0 ]->getValue() )
    {
      begin = childIntervals[ 0 ]->getBegin();

      // somehow, this break never is executed
      // if ( *begin == *beginRecord )
      if (( *begin == *beginRecord ) || ( *begin == *firstBegin ))
        break;

      // firstBegin to control loop!
      if( firstBegin != NULL && firstBegin != beginRecord )
        delete firstBegin;
      firstBegin = begin->clone();

      childIntervals[ 0 ]->calcPrev( displayList );
    }
    if( firstBegin != NULL && firstBegin != beginRecord )
      delete firstBegin;
  }
  else
  {
    childIntervals[ 0 ]->calcPrev( displayList );
    if( begin != NULL && begin != beginRecord )
      delete begin;
    begin = childIntervals[ 0 ]->getBegin()->clone();

    if( end != NULL )
      delete end;
    end = childIntervals[ 0 ]->getEnd()->clone();

    info.values.push_back( childIntervals[ 0 ]->getValue() );
    currentValue = function->execute( &info );
  }

  return displayList;
}


void IntervalCompose::setChilds()
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
    if ( lastLevel != getWindowLevel() )
    {
      childIntervals.clear();
      lastLevel = getWindowLevel();
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


KTrace *IntervalCompose::getWindowTrace() const
{
  return (KTrace*)window->getTrace();
}
