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
  info.values.clear();
  info.callingInterval = this;

  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

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

    // lastEnd to control loop!
    lastEnd = endRecord;
    firstBegin = beginRecord;
  }
  else
  {
    joinBursts = false;
  }

  setChilds();

  if ( function->getInitFromBegin() )
    myInitTime = 0.0;
  else
    myInitTime = initialTime;

  childIntervals[ 0 ]->init( myInitTime, create, displayList );

  if ( joinBursts )
  {
    TSemanticValue tmpValue;

    if( begin != NULL && begin != beginRecord )
      delete begin;
    if( window->getLevel() >= APPLICATION && window->getLevel() <= THREAD )
      begin = window->copyThreadIterator( childIntervals[ 0 ]->getBegin() );
    else
      begin = window->copyCPUIterator( childIntervals[ 0 ]->getBegin() );
    if( end != NULL )
      delete end;
    if( window->getLevel() >= APPLICATION && window->getLevel() <= THREAD )
      end = window->copyThreadIterator( childIntervals[ 0 ]->getEnd() );
    else
      end = window->copyCPUIterator(  childIntervals[ 0 ]->getEnd() );
    tmpValue = childIntervals[ 0 ]->getValue();
    childIntervals[ 0 ]->calcNext( displayList );
    while ( tmpValue == childIntervals[ 0 ]->getValue() )
    {
      if( end != NULL )
        delete end;
      if( window->getLevel() >= APPLICATION && window->getLevel() <= THREAD )
        end = window->copyThreadIterator( childIntervals[ 0 ]->getEnd() );
      else
        end = window->copyCPUIterator( childIntervals[ 0 ]->getEnd() );

      // somehow, this break never is executed
      // if ( *end == *endRecord )
      if (( *end == *endRecord ) || ( *end == *lastEnd ))
        break;

      // lastEnd to control loop!
      if( window->getLevel() >= APPLICATION && window->getLevel() <= THREAD )
        lastEnd = window->copyThreadIterator( end );
      else
        lastEnd = window->copyCPUIterator( end );

      childIntervals[ 0 ]->calcNext( displayList );
    }
    currentValue = tmpValue;
  }
  else
  {
    begin = childIntervals[ 0 ]->getBegin();
    end = childIntervals[ 0 ]->getEnd();
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
  info.values.clear();

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( joinBursts )
  {
    TSemanticValue tmpValue;

    // lastEnd to control loop! Initialized with endRecord.
    lastEnd = endRecord;

    if( begin != NULL && begin != beginRecord )
      delete begin;
    if( window->getLevel() >= APPLICATION && window->getLevel() <= THREAD )
      begin = window->copyThreadIterator( childIntervals[ 0 ]->getBegin() );
    else
      begin = window->copyCPUIterator( childIntervals[ 0 ]->getBegin() );
    if( end != NULL )
      delete end;
    if( window->getLevel() >= APPLICATION && window->getLevel() <= THREAD )
      end = window->copyThreadIterator( childIntervals[ 0 ]->getEnd() );
    else
      end = window->copyCPUIterator(  childIntervals[ 0 ]->getEnd() );
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
      if( window->getLevel() >= APPLICATION && window->getLevel() <= THREAD )
        end = window->copyThreadIterator( childIntervals[ 0 ]->getEnd() );
      else
        end = window->copyCPUIterator( childIntervals[ 0 ]->getEnd() );

      // somehow, this break never is executed
      // if ( *end == *endRecord )
      if (( *end == *endRecord ) || ( *end == *lastEnd ))
        break;

      // lastEnd to control loop!
      if( window->getLevel() >= APPLICATION && window->getLevel() <= THREAD )
        lastEnd = window->copyThreadIterator( end );
      else
        lastEnd = window->copyCPUIterator( end );

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
    currentValue = function->execute( &info );
  }

  return displayList;
}


KRecordList *IntervalCompose::calcPrev( KRecordList *displayList, bool initCalc )
{
  info.values.clear();

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( joinBursts )
  {
    TSemanticValue tmpValue;

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
      if( window->getLevel() >= APPLICATION && window->getLevel() <= THREAD )
        firstBegin = window->copyThreadIterator( begin );
      else
        firstBegin = window->copyCPUIterator( begin );

      childIntervals[ 0 ]->calcPrev( displayList );
    }
  }
  else
  {
    childIntervals[ 0 ]->calcPrev( displayList );
    begin = childIntervals[ 0 ]->getBegin();
    end = childIntervals[ 0 ]->getEnd();
    info.values.push_back( childIntervals[ 0 ]->getValue() );
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


KTrace *IntervalCompose::getWindowTrace() const
{
  return (KTrace*)window->getTrace();
}
