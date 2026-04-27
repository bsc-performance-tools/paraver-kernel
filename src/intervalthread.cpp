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


#include "intervalthread.h"

#include "kwindow.h"
#include "semanticthreadfunctions.h"

KRecordList *IntervalThread::init( TRecordTime initialTime, TCreateList create, KRecordList *displayList )
{
  createList   = NOCREATE;
  currentValue = 0.0;

  if( displayList == nullptr )
  {
    displayList = &myDisplayList;
    displayList->clear();
  }

  if( !notWindowInits )
    function = (SemanticThread *)window->getSemanticFunction( THREAD );

  if( begin != nullptr )
    delete begin;
  if( end != nullptr )
    delete end;

  if( initialTime >= window->getTrace()->getEndTime() )
  {
    begin = window->getThreadEndRecord( order );
    end   = begin->clone();
    return displayList;
  }

  behaviour = TBehaviour::REGULAR;

  begin = window->getThreadRecordByTime( order )->clone();
  if( !function->validRecord( begin ) )
    begin = getPrevRecord( begin, displayList );
  end = begin->clone();

  if( typeid( *function ) == typeid( PeriodicBurst ) )
  {
    createList = create;
    behaviour  = TBehaviour::PERIODIC_BURST;
    return initPeriodicBurst( initialTime, displayList );
  }

  if( ( !function->getInitFromBegin() ) && ( initialTime > 0.0 ) )
    calcPrev( displayList, true );

  createList = create;
  calcNext( displayList, true );
  while( ( !end->isNull() ) && ( end->getTime() <= initialTime ) )
    calcNext( displayList );

  return displayList;
}

MemoryTrace::iterator *IntervalThread::getTraceEnd() const
{
  return window->getThreadEndRecord( order );
}


KRecordList *IntervalThread::calcNext( KRecordList *displayList, bool initCalc )
{
  if( behaviour == TBehaviour::PERIODIC_BURST )
    return calcNextPeriodicBurst( displayList );

  SemanticThreadInfo info;
  info.callingInterval = this;

  if( displayList == nullptr )
    displayList = &myDisplayList;

  if( !initCalc )
  {
    *begin = *end;
  }

  info.it      = begin;
  currentValue = function->execute( &info );
  end          = getNextRecord( end, displayList );

  return displayList;
}


KRecordList *IntervalThread::calcPrev( KRecordList *displayList, bool initCalc )
{
  if( behaviour == TBehaviour::PERIODIC_BURST )
    return calcPrevPeriodicBurst( displayList );

  SemanticThreadInfo info;
  info.callingInterval = this;

  if( displayList == nullptr )
    displayList = &myDisplayList;

  if( !initCalc )
  {
    *end = *begin;
  }

  begin        = getPrevRecord( begin, displayList );
  info.it      = begin;
  currentValue = function->execute( &info );

  if( initCalc )
  {
    *end = *begin;
  }

  return displayList;
}


KRecordList *IntervalThread::initPeriodicBurst( TRecordTime initialTime, KRecordList *displayList )
{
  currentRecordListIt = window->getThreadRecordByTime( order )->clone();
  if( !function->validRecord( currentRecordListIt ) )
    currentRecordListIt = getPrevRecord( currentRecordListIt, displayList );

  virtualBeginRecord.type   = STATE;
  virtualBeginRecord.time   = 0;
  virtualBeginRecord.thread = currentRecordListIt->getThread();
  virtualBeginRecord.CPU    = currentRecordListIt->getCPU();

  virtualEndRecord.type   = STATE;
  virtualEndRecord.time   = function->getParam( PeriodicBurst::DURATION )[ 0 ];
  virtualEndRecord.thread = currentRecordListIt->getThread();
  virtualEndRecord.CPU    = currentRecordListIt->getCPU();

  begin->setRecord( &virtualBeginRecord );
  end->setRecord( &virtualEndRecord );

  currentValue = 1.0;

  while( end->getTime() <= initialTime )
  {
    calcNextPeriodicBurst( displayList );
  }

  return displayList;
}


KRecordList *IntervalThread::calcNextPeriodicBurst( KRecordList *displayList )
{
  virtualBeginRecord.time = virtualEndRecord.time;
  if( currentValue == 0.0 )
  {
    virtualEndRecord.time += function->getParam( PeriodicBurst::DURATION )[ 0 ];
    currentValue = 1.0;
  }
  else if( currentValue == 1.0 )
  {
    if( function->getParam( PeriodicBurst::PERIOD )[ 0 ] > function->getParam( PeriodicBurst::DURATION )[ 0 ] )
    {
      virtualEndRecord.time += ( function->getParam( PeriodicBurst::PERIOD )[ 0 ] - function->getParam( PeriodicBurst::DURATION )[ 0 ] );
      currentValue = 0.0;
    }
    else
      virtualEndRecord.time = virtualBeginRecord.time + function->getParam( PeriodicBurst::DURATION )[ 0 ];
  }

  while( currentRecordListIt->getTime() <= end->getTime() && currentRecordListIt->getTime() < window->getTrace()->getEndTime() )
  {
    currentRecordListIt = getNextRecord( currentRecordListIt, displayList );
  }

  return displayList;
}


KRecordList *IntervalThread::calcPrevPeriodicBurst( KRecordList *displayList )
{
  // Will be never executed
  return displayList;
}


MemoryTrace::iterator *IntervalThread::getNextRecord( MemoryTrace::iterator *it, KRecordList *displayList )
{
  ++( *it );
  while( !it->isNull() )
  {
    if( window->passFilter( it ) )
    {
      if( ( ( createList & CREATEEVENTS ) && ( it->getRecordType() & EVENT ) ) || ( ( createList & CREATECOMMS ) && ( it->getRecordType() & COMM ) ) )
        displayList->insert( window, it );

      if( function->validRecord( it ) )
        break;
    }
    ++( *it );
  }

  if( it->isNull() )
  {
    delete it;
    it = window->getThreadEndRecord( order );
  }

  return it;
}


MemoryTrace::iterator *IntervalThread::getPrevRecord( MemoryTrace::iterator *it, KRecordList *displayList )
{
  --( *it );
  while( !it->isNull() )
  {
    if( window->passFilter( it ) )
    {
      if( ( ( createList & CREATEEVENTS ) && ( it->getRecordType() & EVENT ) ) || ( ( createList & CREATECOMMS ) && ( it->getRecordType() & COMM ) ) )
        displayList->insert( window, it );

      if( function->validRecord( it ) )
        break;
    }
    --( *it );
  }

  if( it->isNull() )
  {
    delete it;
    it = window->getThreadBeginRecord( order );
  }

  return it;
}
