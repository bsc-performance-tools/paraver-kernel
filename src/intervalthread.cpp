#include "intervalthread.h"

RecordList *IntervalThread::init( TRecordTime initialTime, RecordList *displayList,
                                  TCreateList create )
{
  createList = create;
  currentValue = 0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if( begin != NULL )
    delete begin;
  if( end != NULL )
    delete end;

  begin = window->getThreadRecordByTime( order );
  end = dynamic_cast<MemoryTrace::iterator *> ( window->copyIterator( begin ) );

  if ( ( !function->getInitFromBegin() ) && ( initialTime > 0 ) )
    calcPrev( displayList, true );

  while ( ( !end->isNull() ) && ( end->getTime() < initialTime ) )
    calcNext( displayList, true );

  return displayList;
}


RecordList *IntervalThread::calcNext( RecordList *displayList, bool initCalc )
{
  SemanticThreadInfo info;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( !initCalc )
  {
    currentValue = nextValue;
    *begin = *end;
  }

  getNextRecord( end, displayList );
  info.callingInterval = this;
  info.it = end;
  nextValue = function->execute( info );

  return displayList;
}


RecordList *IntervalThread::calcPrev( RecordList *displayList, bool initCalc )
{
  if ( displayList == NULL )
    displayList = &myDisplayList;
  return displayList;
}


void IntervalThread::getNextRecord( MemoryTrace::iterator *it,
                                    RecordList *displayList )
{
  it++;
  while ( !it->isNull() )
  {
    if ( window->passFilter( it ) )
    {
      if ( ( ( createList & CREATEEVENTS ) && ( it->getType() & EVENT ) )
           ||
           ( ( createList & CREATECOMMS ) && ( it->getType() & COMM ) ) )
      {
        displayList->insert( it );
      }
      if ( function->validRecord( it ) )
      {
        break;
      }
    }
    it++;
  }

  if ( it->isNull() )
    ;//it = window->getThreadEndRecord();
}


void IntervalThread::getPrevRecord( MemoryTrace::iterator *it,
                                    RecordList *displayList )
{}
