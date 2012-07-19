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
#include "intervalnotthread.h"

KRecordList *IntervalNotThread::init( TRecordTime initialTime, TCreateList create,
                                      KRecordList *displayList )
{
  TRecordTime myInitTime;
  info.values.clear();
  info.callingInterval = this;
  info.lastChanged = 0;

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

  while ( end->getTime() < initialTime && begin->getTime() < window->getTrace()->getEndTime() )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalNotThread::calcNext( KRecordList *displayList, bool initCalc )
{
  info.values.clear();

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

  for ( TObjectOrder i = 0; i < childIntervals.size(); i++ )
  {
    if ( childIntervals[ i ]->getEnd()->getTime() <= begin->getTime() )
    {
      childIntervals[ i ]->calcNext( displayList );
      info.lastChanged = i;
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

  return displayList;
}


KRecordList *IntervalNotThread::calcPrev( KRecordList *displayList, bool initCalc )
{
  info.values.clear();

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
  return (KTrace*)window->getTrace();
}
