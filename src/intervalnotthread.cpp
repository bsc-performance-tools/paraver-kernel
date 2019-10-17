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
  orderedChilds.clear();

  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticNotThread * ) window->getSemanticFunction( level );

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

  for ( TObjectOrder i = 0; i < childIntervals.size(); ++i )
  {
    childIntervals[ i ]->init( myInitTime, createList, displayList );

    if ( begin == NULL ||
         childIntervals[ i ]->getBegin()->getTime() > begin->getTime() )
    {
      if ( begin != NULL )
        delete begin;
      begin = childIntervals[ i ]->getBegin()->clone();
    }

    if ( end == NULL ||
         childIntervals[ i ]->getEnd()->getTime() < end->getTime() )
    {
      if ( end != NULL )
        delete end;
      end = childIntervals[ i ]->getEnd()->clone();
    }

    info.values.push_back( childIntervals[ i ]->getValue() );
    pair<TRecordTime,TObjectOrder> tmpChild( childIntervals[ i ]->getEnd()->getTime(), i );
    orderedChilds.insert( tmpChild );
  }
  currentValue = function->execute( &info );

  while ( end->getTime() < initialTime && begin->getTime() < window->getTrace()->getEndTime() )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalNotThread::calcNext( KRecordList *displayList, bool initCalc )
{
  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( begin != NULL )
    delete begin;
  begin = end->clone();

  if( end != NULL )
  {
    delete end;
    end = NULL;
  }

  TObjectOrder i = 0;
  std::multimap<TRecordTime,TObjectOrder>::iterator itChild = orderedChilds.begin();
  while( itChild->first == begin->getTime() )
  {
    if ( childIntervals[ itChild->second ]->getEnd()->getTime() <= begin->getTime() )
    {
      childIntervals[ itChild->second ]->calcNext( displayList );
      info.lastChanged = itChild->second;
    }

    info.values[ itChild->second ] = childIntervals[ itChild->second ]->getValue();
    pair<TRecordTime,TObjectOrder> tmpChild( childIntervals[ itChild->second ]->getEnd()->getTime(), itChild->second );
    orderedChilds.erase( itChild );
    orderedChilds.insert( tmpChild );
    itChild = orderedChilds.begin();

    ++i;
    if( i >= childIntervals.size() ) break;
  }
  if ( end != NULL )
    delete end;
  end = childIntervals[ itChild->second ]->getEnd()->clone();

  currentValue = function->execute( &info );

  return displayList;
}


KRecordList *IntervalNotThread::calcPrev( KRecordList *displayList, bool initCalc )
{
  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( end != NULL )
    delete end;
  end = begin->clone();

  if( begin != NULL )
  {
    delete begin;
    begin = NULL;
  }

  TObjectOrder i = 0;
  std::multimap<TRecordTime,TObjectOrder>::iterator itChild = orderedChilds.begin();
  while( itChild->first == begin->getTime() )
  {
    if ( childIntervals[ itChild->second ]->getBegin()->getTime() >= end->getTime() )
      childIntervals[ itChild->second ]->calcPrev( displayList );

    info.values[ itChild->second ] = childIntervals[ itChild->second ]->getValue();
    pair<TRecordTime,TObjectOrder> tmpChild( childIntervals[ itChild->second ]->getEnd()->getTime(), itChild->second );
    orderedChilds.erase( itChild );
    orderedChilds.insert( tmpChild );
    itChild = orderedChilds.begin();

    ++i;
    if( i >= childIntervals.size() ) break;
  }
  if ( begin != NULL )
    delete begin;
  begin = childIntervals[ itChild->second ]->getBegin()->clone();

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
