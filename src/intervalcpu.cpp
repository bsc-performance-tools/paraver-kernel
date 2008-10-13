#include "kwindow.h"
#include "intervalcpu.h"


KRecordList *IntervalCPU::init( TRecordTime initialTime, TCreateList create,
                               KRecordList *displayList )
{
  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticCPU * ) window->getLevelFunction( level );
  functionThread = ( SemanticThread * ) window->getLevelFunction( THREAD );

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
        displayList->insert( window->copyCPUIterator( it ) );

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
        displayList->insert( window->copyCPUIterator( it ) );

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
  return (KTrace*)window->getTrace();
}

