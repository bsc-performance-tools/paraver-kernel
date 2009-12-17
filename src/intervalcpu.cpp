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
#include "intervalcpu.h"


KRecordList *IntervalCPU::init( TRecordTime initialTime, TCreateList create,
                                KRecordList *displayList )
{
  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticCPU * ) window->getSemanticFunction( level );
  functionThread = ( SemanticThread * ) window->getSemanticFunction( THREAD );

  if ( begin != NULL )
    delete begin;
  if ( end != NULL )
    delete end;

  begin = window->copyCPUIterator( window->getThreadRecordByTime( order ) );
  end = window->copyCPUIterator( begin );

  if ( ( !function->getInitFromBegin() ) && ( !functionThread->getInitFromBegin() ) &&
       ( initialTime > 0.0 ) )
    calcPrev( displayList, true );

  calcNext( displayList, true );
  while ( ( !end->isNull() ) && ( end->getTime() <= initialTime ) )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalCPU::calcNext( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo highInfo;
  SemanticThreadInfo threadInfo;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( !initCalc )
  {
    *begin = *end;
  }

  threadInfo.callingInterval = this;
  highInfo.callingInterval = this;
  threadInfo.it = begin;
  highInfo.values.push_back( functionThread->execute( &threadInfo ) );
  currentValue = function->execute( &highInfo );
  end = getNextRecord( end, displayList );

  return displayList;
}


KRecordList *IntervalCPU::calcPrev( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo highInfo;
  SemanticThreadInfo threadInfo;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( !initCalc )
  {
    *end = *begin;
  }

  begin = getPrevRecord( begin, displayList );
  highInfo.callingInterval = this;
  threadInfo.callingInterval = this;
  threadInfo.it = begin;
  highInfo.values.push_back( functionThread->execute( &threadInfo ) );
  currentValue = function->execute( &highInfo );

  if ( initCalc )
  {
    *end = *begin;
  }


  return displayList;
}


MemoryTrace::iterator *IntervalCPU::getNextRecord( MemoryTrace::iterator *it,
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

      if ( functionThread->validRecord( it ) )
        break;
    }
    ++( *it );
  }

  if ( it->isNull() )
  {
    delete it;
    it = window->getCPUEndRecord( order );
  }

  return it;
}


MemoryTrace::iterator *IntervalCPU::getPrevRecord( MemoryTrace::iterator *it,
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

      if ( functionThread->validRecord( it ) )
        break;
    }
    --( *it );
  }

  if ( it->isNull() )
  {
    delete it;
    it = window->getCPUBeginRecord( order );
  }

  return it;
}

TWindowLevel IntervalCPU::getWindowLevel() const
{
  return window->getLevel();
}


Interval *IntervalCPU::getWindowInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}


bool IntervalCPU::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}


TWindowLevel IntervalCPU::getComposeLevel( TWindowLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


KTrace *IntervalCPU::getWindowTrace() const
{
  return ( KTrace* )window->getTrace();
}

