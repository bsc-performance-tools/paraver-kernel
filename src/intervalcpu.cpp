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
  if( functionThread != NULL ) delete functionThread;
  functionThread = ( SemanticThread * ) window->getSemanticFunction( THREAD )->clone();
  if( functionComposeThread != NULL ) delete functionComposeThread;
  functionComposeThread = ( SemanticCompose * ) window->getSemanticFunction( COMPOSETHREAD )->clone();

  if ( begin != NULL )
    delete begin;
  if ( end != NULL )
    delete end;

  threadState.clear();
  threadState.insert( threadState.begin(), window->getTrace()->totalThreads(), 0.0 );

  begin = window->copyCPUIterator( window->getThreadRecordByTime( order - 1 ) );
  end = window->copyCPUIterator( begin );

  if ( ( !function->getInitFromBegin() ) && ( !functionThread->getInitFromBegin() ) &&
       ( !functionComposeThread->getInitFromBegin() ) && ( initialTime > 0.0 ) )
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

  threadInfo.callingInterval = getWindowInterval( THREAD, begin->getOrder() );
  highInfo.callingInterval = this;
  threadInfo.it = begin;
  if( !( window->passFilter( begin ) && functionThread->validRecord( begin ) ) )
    highInfo.values.push_back( threadState[ begin->getThread() ] );
  else if( begin->getType() == STATE + END )
    highInfo.values.push_back( 0.0 );
  else
  {
    SemanticHighInfo tmpHighInfo;
    tmpHighInfo.callingInterval = getWindowInterval( COMPOSETHREAD, begin->getThread() );
    tmpHighInfo.values.push_back( functionThread->execute( &threadInfo ) );
    threadState[ begin->getThread() ] = functionComposeThread->execute( &tmpHighInfo );
    highInfo.values.push_back( threadState[ begin->getThread() ] );
  }
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
  TThreadOrder threadOrder = it->getThread();
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
    if( !( it->getType() & RSEND || it->getType() & RRECV || it->getType() & COMM )
        && it->getThread() != threadOrder )
      break;
    ++( *it );
  }

  if ( it->isNull() )
  {
    delete it;
    it = window->getCPUEndRecord( order - 1 );
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
    it = window->getCPUBeginRecord( order - 1 );
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

