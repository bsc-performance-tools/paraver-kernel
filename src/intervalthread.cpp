#include "kwindow.h"
#include "intervalthread.h"

KRecordList *IntervalThread::init( TRecordTime initialTime, TCreateList create,
                                   KRecordList *displayList )
{
  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticThread * ) window->getSemanticFunction( level );

  if ( begin != NULL )
    delete begin;
  if ( end != NULL )
    delete end;

  begin = window->copyThreadIterator( window->getThreadRecordByTime( order ) );
  end = window->copyThreadIterator( begin );

  if ( ( !function->getInitFromBegin() ) && ( initialTime > 0.0 ) )
    calcPrev( displayList, true );

  calcNext( displayList, true );
  while ( ( !end->isNull() ) && ( end->getTime() <= initialTime ) )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalThread::calcNext( KRecordList *displayList, bool initCalc )
{
  SemanticThreadInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( !initCalc )
  {
    *begin = *end;
  }

  info.callingInterval = this;
  info.it = begin;
  currentValue = function->execute( &info );
  end = getNextRecord( end, displayList );

  return displayList;
}


KRecordList *IntervalThread::calcPrev( KRecordList *displayList, bool initCalc )
{
  SemanticThreadInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( !initCalc )
  {
    *end = *begin;
  }

  begin = getPrevRecord( begin, displayList );
  info.callingInterval = this;
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
