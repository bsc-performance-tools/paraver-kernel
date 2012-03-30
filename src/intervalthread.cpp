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
#include "intervalthread.h"

KRecordList *IntervalThread::init( TRecordTime initialTime, TCreateList create,
                                   KRecordList *displayList )
{
  createList = NOCREATE;
  currentValue = 0.0;
  info.callingInterval = this;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if( !notWindowInits )
    function = ( SemanticThread * ) window->getSemanticFunction( level );

  if ( begin != NULL )
    delete begin;
  if ( end != NULL )
    delete end;

  begin = window->copyThreadIterator( window->getThreadRecordByTime( order ) );
  end = window->copyThreadIterator( begin );

  if ( ( !function->getInitFromBegin() ) && ( initialTime > 0.0 ) )
    calcPrev( displayList, true );

  createList = create;
  calcNext( displayList, true );
  while ( ( !end->isNull() ) && ( end->getTime() <= initialTime ) )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalThread::calcNext( KRecordList *displayList, bool initCalc )
{
  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( !initCalc )
  {
    *begin = *end;
  }

  info.it = begin;
  currentValue = function->execute( &info );
  end = getNextRecord( end, displayList );

  return displayList;
}


KRecordList *IntervalThread::calcPrev( KRecordList *displayList, bool initCalc )
{
  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( !initCalc )
  {
    *end = *begin;
  }

  begin = getPrevRecord( begin, displayList );
  info.it = begin;
  currentValue = function->execute( &info );

  if ( initCalc )
  {
    *end = *begin;
  }

  return displayList;
}


MemoryTrace::iterator *IntervalThread::getNextRecord( MemoryTrace::iterator *it,
    KRecordList *displayList )
{
  ++( *it );
  while ( !it->isNull() )
  {
    if ( window->passFilter( it ) )
    {
      if ( ( ( createList & CREATEEVENTS ) && ( it->getType() & EVENT ) )
           ||
           ( ( createList & CREATECOMMS ) && ( it->getType() & COMM ) ) )
        displayList->insert( window, it );

      if ( function->validRecord( it ) )
        break;
    }
    ++( *it );
  }

  if ( it->isNull() )
  {
    delete it;
    it = window->getThreadEndRecord( order );
  }

  return it;
}


MemoryTrace::iterator *IntervalThread::getPrevRecord( MemoryTrace::iterator *it,
    KRecordList *displayList )
{
  --( *it );
  while ( !it->isNull() )
  {
    if ( window->passFilter( it ) )
    {
      if ( ( ( createList & CREATEEVENTS ) && ( it->getType() & EVENT ) )
           ||
           ( ( createList & CREATECOMMS ) && ( it->getType() & COMM ) ) )
        displayList->insert( window, it );

      if ( function->validRecord( it ) )
        break;
    }
    --( *it );
  }

  if ( it->isNull() )
  {
    delete it;
    it = window->getThreadBeginRecord( order );
  }

  return it;
}
