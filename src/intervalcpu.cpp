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
#include "intervalcpu.h"


IntervalCPU::IntervalCPU( KSingleWindow *whichWindow, TWindowLevel whichLevel,
                          TObjectOrder whichOrder ):
  IntervalHigh( whichLevel, whichOrder ), window( whichWindow )
{
  begin = nullptr;
  end = nullptr;
  function = nullptr;
  functionThread = nullptr;
  functionComposeThread = nullptr;

/*  TNodeOrder tmpNode;
  TCPUOrder tmpCPU;
  window->getTrace()->getCPULocation( whichOrder, tmpNode, tmpCPU );
  std::vector<TThreadOrder> tmpThreads;
  window->getTrace()->getThreadsPerNode( tmpNode + 1, tmpThreads );*/
}


KRecordList *IntervalCPU::init( TRecordTime initialTime, TCreateList create,
                                KRecordList *displayList )
{
  createList = create;
  currentValue = 0.0;
  currentInitialTime = initialTime;

  if( displayList == nullptr )
  {
    displayList = &myDisplayList;
    displayList->clear();
  }

  function = ( SemanticCPU * ) window->getSemanticFunction( level );
  if( functionThread != nullptr ) delete functionThread;
  functionThread = ( SemanticThread * ) window->getSemanticFunction( THREAD )->clone();
  if( functionComposeThread != nullptr ) delete functionComposeThread;
  functionComposeThread = ( SemanticCompose * ) window->getSemanticFunction( COMPOSETHREAD )->clone();

  if( begin != nullptr )
    delete begin;
  if( end != nullptr )
    delete end;

  if( initialTime >= window->getTrace()->getEndTime() )
  {
    begin = window->getCPUEndRecord( order - 1 );
    end = begin->clone();
    return displayList;
  }

  /*if( intervalCompose.empty() )
  {
    begin = window->getCPUBeginRecord( order - 1 )->clone();
    end = window->getCPUEndRecord( order - 1 )->clone();
    return displayList;
  }*/


  for( std::map<TThreadOrder, IntervalThread *>::iterator it = intervalThread.begin();
       it != intervalThread.end(); ++it )
    (*it).second->setSemanticFunction( functionThread );

  for( std::map<TThreadOrder, IntervalCompose *>::iterator it = intervalCompose.begin();
       it != intervalCompose.end(); ++it )
  {
    (*it).second->setSemanticFunction( functionComposeThread );
    (*it).second->init( initialTime, NOCREATE, nullptr );
  }

  begin = window->getCPURecordByTime( order - 1 )->clone();
  end = begin->clone();

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

  if( displayList == nullptr )
    displayList = &myDisplayList;

  if( !initCalc )
  {
    *begin = *end;
  }

/*  if( intervalCompose.empty() )
    return displayList;*/

  if( intervalCompose.find( begin->getThread() ) == intervalCompose.end() )
  {
    intervalThread[ begin->getThread() ] = new IntervalThread( window, THREAD, begin->getThread() );
    intervalThread[ begin->getThread() ]->setNotWindowInits( true );
    intervalThread[ begin->getThread() ]->setSemanticFunction( functionThread );

    intervalCompose[ begin->getThread() ] = new IntervalCompose( window, COMPOSETHREAD, begin->getThread() );
    intervalCompose[ begin->getThread() ]->setNotWindowInits( true );
    intervalCompose[ begin->getThread() ]->setCustomChild( intervalThread[ begin->getThread() ] );
    intervalCompose[ begin->getThread() ]->setSemanticFunction( functionComposeThread );

    intervalCompose[ begin->getThread() ]->init( currentInitialTime, NOCREATE, nullptr );
  }

  Interval *currentThread = intervalCompose[ begin->getThread() ];
  highInfo.callingInterval = this;

  if( begin->getType() == STATE + END )
    highInfo.values.push_back( 0.0 );
  else
  {
    while( currentThread->getEndTime() <= begin->getTime() &&
           currentThread->getBeginTime() < window->getTrace()->getEndTime() )
      currentThread->calcNext( nullptr );
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

  if( displayList == nullptr )
    displayList = &myDisplayList;

  if( !initCalc )
  {
    *end = *begin;
  }

/*  if( intervalCompose.empty() )
    return displayList;*/

  begin = getPrevRecord( begin, displayList );
  highInfo.callingInterval = this;

  if( intervalCompose.find( begin->getThread() ) == intervalCompose.end() )
  {
    intervalThread[ begin->getThread() ] = new IntervalThread( window, THREAD, begin->getThread() );
    intervalThread[ begin->getThread() ]->setNotWindowInits( true );
    intervalThread[ begin->getThread() ]->setSemanticFunction( functionThread );

    intervalCompose[ begin->getThread() ] = new IntervalCompose( window, COMPOSETHREAD, begin->getThread() );
    intervalCompose[ begin->getThread() ]->setNotWindowInits( true );
    intervalCompose[ begin->getThread() ]->setCustomChild( intervalThread[ begin->getThread() ] );
    intervalCompose[ begin->getThread() ]->setSemanticFunction( functionComposeThread );

    intervalCompose[ begin->getThread() ]->init( currentInitialTime, NOCREATE, nullptr );
  }

  Interval *currentThread = intervalCompose[ begin->getThread() ];
  while( currentThread->getBeginTime() >= begin->getTime() &&
         currentThread->getEndTime() > 0.0 )
    currentThread->calcPrev( nullptr );
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

