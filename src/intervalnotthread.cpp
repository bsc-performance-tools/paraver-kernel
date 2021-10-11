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


#include "kwindow.h"
#include "intervalnotthread.h"

using std::multimap;
using std::pair;

KRecordList *IntervalNotThread::init( TRecordTime initialTime, TCreateList create,
                                      KRecordList *displayList )
{
  TRecordTime myInitTime;
  info.values.clear();
  info.callingInterval = this;
  info.lastChanged = 0;
  orderedChildren.clear();

  createList = create;
  currentValue = 0.0;

  if ( displayList == nullptr )
    displayList = &myDisplayList;

  function = ( SemanticNotThread * ) window->getSemanticFunction( level );

  setChildren();
  if ( begin != nullptr )
  {
    delete begin;
    begin = nullptr;
  }
  if ( end != nullptr )
  {
    delete end;
    end = nullptr;
  }

  if ( function->getInitFromBegin() )
    myInitTime = 0.0;
  else
    myInitTime = initialTime;

  info.callingInterval = this;

  for ( TObjectOrder i = 0; i < childIntervals.size(); ++i )
  {
    childIntervals[ i ]->init( myInitTime, createList, displayList );

    if ( begin == nullptr ||
         childIntervals[ i ]->getBegin()->getTime() > begin->getTime() )
    {
      if ( begin == nullptr )
        begin = childIntervals[ i ]->getBegin()->clone();
      else
        *begin = *childIntervals[ i ]->getBegin();
    }

    if ( end == nullptr ||
         childIntervals[ i ]->getEnd()->getTime() < end->getTime() )
    {
      if ( end == nullptr )
        end = childIntervals[ i ]->getEnd()->clone();
        *end = *childIntervals[ i ]->getEnd();
    }

    info.values.push_back( childIntervals[ i ]->getValue() );
    pair<TRecordTime,TObjectOrder> tmpChild( childIntervals[ i ]->getEnd()->getTime(), i );
    orderedChildren.insert( tmpChild );
  }
  currentValue = function->execute( &info );

  while ( end->getTime() < initialTime && begin->getTime() < window->getTrace()->getEndTime() )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalNotThread::calcNext( KRecordList *displayList, bool initCalc )
{
  if ( displayList == nullptr )
    displayList = &myDisplayList;

  *begin = *end;

  TObjectOrder i = 0;
  std::multimap<TRecordTime,TObjectOrder>::iterator itChild = orderedChildren.begin();
  while( itChild->first == begin->getTime() )
  {
    if ( childIntervals[ itChild->second ]->getEnd()->getTime() <= begin->getTime() )
    {
      childIntervals[ itChild->second ]->calcNext( displayList );
      info.lastChanged = itChild->second;
    }

    info.values[ itChild->second ] = childIntervals[ itChild->second ]->getValue();
    pair<TRecordTime,TObjectOrder> tmpChild( childIntervals[ itChild->second ]->getEnd()->getTime(), itChild->second );
    orderedChildren.erase( itChild );
    orderedChildren.insert( tmpChild );
    itChild = orderedChildren.begin();

    ++i;
    if( i >= childIntervals.size() ) break;
  }

  *end = *childIntervals[ itChild->second ]->getEnd();

  currentValue = function->execute( &info );

  return displayList;
}


KRecordList *IntervalNotThread::calcPrev( KRecordList *displayList, bool initCalc )
{
  if ( displayList == nullptr )
    displayList = &myDisplayList;

  *end = *begin;

  TObjectOrder i = 0;
  std::multimap<TRecordTime,TObjectOrder>::iterator itChild = orderedChildren.begin();
  while( itChild->first == end->getTime() )
  {
    if ( childIntervals[ itChild->second ]->getBegin()->getTime() >= end->getTime() )
      childIntervals[ itChild->second ]->calcPrev( displayList );

    info.values[ itChild->second ] = childIntervals[ itChild->second ]->getValue();
    pair<TRecordTime,TObjectOrder> tmpChild( childIntervals[ itChild->second ]->getEnd()->getTime(), itChild->second );
    orderedChildren.erase( itChild );
    orderedChildren.insert( tmpChild );
    itChild = orderedChildren.begin();

    ++i;
    if( i >= childIntervals.size() ) break;
  }

  *begin = *childIntervals[ itChild->second ]->getBegin();

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
  return (KTrace*)window->getTrace();
}
