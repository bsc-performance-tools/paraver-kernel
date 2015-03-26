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


IntervalCPU::IntervalCPU( KSingleWindow *whichWindow, TWindowLevel whichLevel,
                          TObjectOrder whichOrder ):
  IntervalHigh( whichLevel, whichOrder ), window( whichWindow )
{
  begin = NULL;
  end = NULL;
  function = NULL;
  functionThread = NULL;
  functionComposeThread = NULL;

  TNodeOrder tmpNode;
  TCPUOrder tmpCPU;
  window->getTrace()->getCPULocation( whichOrder, tmpNode, tmpCPU );
  std::vector<TThreadOrder> tmpThreads;
  window->getTrace()->getThreadsPerNode( tmpNode, tmpThreads );

  if( tmpThreads .empty() )
    firstThreadOnCPU = 0;
  else
    firstThreadOnCPU = tmpThreads[ 0 ];

  intervalThread = std::vector<IntervalThread *>( tmpThreads.size(), NULL );
  intervalCompose = std::vector<IntervalCompose *>( tmpThreads.size(), NULL );
}


KRecordList *IntervalCPU::init( TRecordTime initialTime, TCreateList create,
                                KRecordList *displayList )
{
  createList = create;
  currentValue = 0.0;
  currentInitialTime = initialTime;

  if( displayList == NULL )
  {
    displayList = &myDisplayList;
    displayList->clear();
  }

  function = ( SemanticCPU * ) window->getSemanticFunction( level );
  if( functionThread != NULL ) delete functionThread;
  functionThread = ( SemanticThread * ) window->getSemanticFunction( THREAD )->clone();
  if( functionComposeThread != NULL ) delete functionComposeThread;
  functionComposeThread = ( SemanticCompose * ) window->getSemanticFunction( COMPOSETHREAD )->clone();

  if( begin != NULL )
    delete begin;
  if( end != NULL )
    delete end;

  if( initialTime >= window->getTrace()->getEndTime() )
  {
    begin = window->getCPUEndRecord( order - 1 );
    end = window->copyCPUIterator( begin );
    return displayList;
  }

  if( intervalCompose.empty() )
  {
    begin = window->copyCPUIterator( window->getCPUBeginRecord( order - 1 ) );
    end = window->copyCPUIterator( window->getCPUEndRecord( order - 1 ) );
    return displayList;
  }

  for( std::vector<IntervalCompose *>::iterator it = intervalCompose.begin();
       it != intervalCompose.end(); ++it )
  {
    if( (*it) != NULL )
      (*it)->init( initialTime, NOCREATE, NULL );
  }

  begin = window->copyCPUIterator( window->getCPURecordByTime( order - 1 ) );
  end = window->copyCPUIterator( begin );

  if(( !function->getInitFromBegin() ) && ( !functionThread->getInitFromBegin() ) &&
      ( !functionComposeThread->getInitFromBegin() ) && ( initialTime > 0.0 ) )
    calcPrev( displayList, true );

  calcNext( displayList, true );
  while(( !end->isNull() ) && ( end->getTime() <= initialTime ) )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalCPU::calcNext( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo highInfo;

  if( displayList == NULL )
    displayList = &myDisplayList;

  if( !initCalc )
  {
    *begin = *end;
  }

  if( intervalCompose.empty() )
    return displayList;

  if( intervalCompose[ begin->getThread() - firstThreadOnCPU ] == NULL )
  {
    int i = begin->getThread() - firstThreadOnCPU;
    intervalThread[ i ] = new IntervalThread( window, THREAD, begin->getThread() );
    intervalThread[ i ]->setNotWindowInits( true );
    intervalThread[ i ]->setSemanticFunction( functionThread );

    intervalCompose[ i ] = new IntervalCompose( window, COMPOSETHREAD, begin->getThread() );
    intervalCompose[ i ]->setNotWindowInits( true );
    intervalCompose[ i ]->setCustomChild( intervalThread[ i ] );
    intervalCompose[ i ]->setSemanticFunction( functionComposeThread );

    intervalCompose[ i ]->init( currentInitialTime, NOCREATE, NULL );
  }

  Interval *currentThread = intervalCompose[ begin->getThread() - firstThreadOnCPU ];
  highInfo.callingInterval = this;
  if( begin->getType() == STATE + END )
    highInfo.values.push_back( 0.0 );
  else
  {
    while( currentThread->getEndTime() <= begin->getTime() &&
           currentThread->getBeginTime() < window->getTrace()->getEndTime() )
      currentThread->calcNext( NULL );
    if( currentThread->getBegin()->getCPU() != order )
      highInfo.values.push_back( 0.0 );
    else
      highInfo.values.push_back( currentThread->getValue() );
  }
  currentValue = function->execute( &highInfo );
  end = getNextRecord( end, displayList );

  return displayList;
}


KRecordList *IntervalCPU::calcPrev( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo highInfo;

  if( displayList == NULL )
    displayList = &myDisplayList;

  if( !initCalc )
  {
    *end = *begin;
  }

  if( intervalCompose.empty() )
    return displayList;

  begin = getPrevRecord( begin, displayList );
  highInfo.callingInterval = this;

  if( intervalCompose[ begin->getThread() - firstThreadOnCPU ] == NULL )
  {
    int i = begin->getThread() - firstThreadOnCPU;
    intervalThread[ i ] = new IntervalThread( window, THREAD, begin->getThread() );
    intervalThread[ i ]->setNotWindowInits( true );
    intervalThread[ i ]->setSemanticFunction( functionThread );

    intervalCompose[ i ] = new IntervalCompose( window, COMPOSETHREAD, begin->getThread() );
    intervalCompose[ i ]->setNotWindowInits( true );
    intervalCompose[ i ]->setCustomChild( intervalThread[ i ] );
    intervalCompose[ i ]->setSemanticFunction( functionComposeThread );

    intervalCompose[ i ]->init( currentInitialTime, NOCREATE, NULL );
  }

  Interval *currentThread = intervalCompose[ begin->getThread() - firstThreadOnCPU ];
  while( currentThread->getBeginTime() >= begin->getTime() &&
         currentThread->getEndTime() > 0.0 )
    currentThread->calcPrev( NULL );
  highInfo.values.push_back( currentThread->getValue() );
  currentValue = function->execute( &highInfo );

  if( initCalc )
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
  while( !it->isNull() )
  {
    if( window->passFilter( it ) )
    {
      if((( createList & CREATEEVENTS ) && ( it->getType() & EVENT ) )
          ||
          (( createList & CREATECOMMS ) && ( it->getType() & COMM ) ) )
        displayList->insert( window, it );

      if( functionThread->validRecord( it ) )
        break;
    }
    if( !( it->getType() & RSEND || it->getType() & RRECV || it->getType() & COMM )
        && it->getThread() != threadOrder )
      break;
    ++( *it );
  }

  if( it->isNull() )
  {
    delete it;
    it = window->getCPUEndRecord( order - 1 );
  }

  return it;
}


MemoryTrace::iterator *IntervalCPU::getPrevRecord( MemoryTrace::iterator *it,
    KRecordList *displayList )
{
  TThreadOrder threadOrder = it->getThread();
  --( *it );
  while( !it->isNull() )
  {
    if( window->passFilter( it ) )
    {
      if((( createList & CREATEEVENTS ) && ( it->getType() & EVENT ) )
          ||
          (( createList & CREATECOMMS ) && ( it->getType() & COMM ) ) )
        displayList->insert( window, it );

      if( functionThread->validRecord( it ) )
        break;
    }
    if( !( it->getType() & RSEND || it->getType() & RRECV || it->getType() & COMM )
        && it->getThread() != threadOrder )
      break;
    --( *it );
  }

  if( it->isNull() )
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

