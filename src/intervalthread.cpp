#include "kwindow.h"
#include "intervalthread.h"

RecordList *IntervalThread::init( TRecordTime initialTime, TCreateList create,
                                  RecordList *displayList )
{
  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticThread * ) window->getLevelFunction( level );

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


RecordList *IntervalThread::calcNext( RecordList *displayList, bool initCalc )
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
  getNextRecord( end, displayList );

  return displayList;
}


RecordList *IntervalThread::calcPrev( RecordList *displayList, bool initCalc )
{
  SemanticThreadInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( !initCalc )
  {
    *end = *begin;
  }

  getPrevRecord( begin, displayList );
  info.callingInterval = this;
  info.it = begin;
  currentValue = function->execute( &info );

  if ( initCalc )
  {
    *end = *begin;
  }

  return displayList;
}


void IntervalThread::getNextRecord( MemoryTrace::iterator *it,
                                    RecordList *displayList )
{
  ++( *it );
  while ( !it->isNull() )
  {
    if ( ( ( createList & CREATEEVENTS ) && ( it->getType() & EVENT ) )
         ||
         ( ( createList & CREATECOMMS ) && ( it->getType() & COMM ) ) )
    {
      if ( window->passFilter( it ) )
      {
        displayList->insert( it );

        if ( function->validRecord( it ) )
          break;
      }
    }
    ++( *it );
  }

  if ( it->isNull() )
  {
    delete it;
    it = window->getThreadEndRecord( order );
  }
}


void IntervalThread::getPrevRecord( MemoryTrace::iterator *it,
                                    RecordList *displayList )
{
  --( *it );
  while ( !it->isNull() )
  {
    if ( ( ( createList & CREATEEVENTS ) && ( it->getType() & EVENT ) )
         ||
         ( ( createList & CREATECOMMS ) && ( it->getType() & COMM ) ) )
    {
      if ( window->passFilter( it ) )
      {
        displayList->insert( it );

        if ( function->validRecord( it ) )
          break;
      }
    }
    --( *it );
  }

  if ( it->isNull() )
  {
    delete it;
    it = window->getThreadBeginRecord( order );
  }
}
