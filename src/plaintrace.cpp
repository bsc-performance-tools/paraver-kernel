#include "plaintrace.h"
#include "bplustreeblocks.h"

using bplustree::TRecord;

PlainTrace::PlainTrace( const TThreadOrder totalThreads,
                        const TCPUOrder totalCPUs ):
    numThreads( totalThreads ), numCPUs( totalCPUs )

{
  TRecord tmpBegin, tmpEnd;

  first.time = 0;
  first.type = EMPTYREC;
  first.thread = 0;
  first.CPU = 0;
  first.threadNext = NULL;
  first.threadPrev = NULL;
  first.next = NULL;
  first.prev = NULL;

  last.time = 0;
  last.type = EMPTYREC;
  last.thread = 0;
  last.CPU = 0;
  last.threadNext = NULL;
  last.threadPrev = NULL;
  last.next = NULL;
  last.prev = NULL;

  emptyThreadBegin.reserve( numThreads );
  emptyThreadEnd.reserve( numThreads );

  tmpBegin.time = 0;
  tmpEnd.time = 0;
  tmpBegin.type = EMPTYREC;
  tmpEnd.type = EMPTYREC;
  tmpBegin.threadNext = NULL;
  tmpBegin.threadPrev = NULL;
  tmpEnd.threadNext = NULL;
  tmpEnd.threadPrev = NULL;
  for ( TThreadOrder i = 0; i < numThreads; i++ )
  {
    tmpBegin.thread = i;
    tmpEnd.thread = i;
    emptyThreadBegin.push_back( tmpBegin );
    emptyThreadEnd.push_back( tmpEnd );
  }

  emptyCPUBegin.reserve( numCPUs );
  emptyCPUEnd.reserve( numCPUs );

  tmpBegin.next = NULL;
  tmpBegin.prev = NULL;
  tmpEnd.next = NULL;
  tmpEnd.prev = NULL;
  for ( TCPUOrder i = 0; i < numCPUs; i++ )
  {
    tmpBegin.CPU = i;
    tmpEnd.CPU = i;
    emptyCPUBegin.push_back( tmpBegin );
    emptyCPUEnd.push_back( tmpEnd );
  }
}

void PlainTrace::insert( MemoryBlocks *blocks )
{
  TRecord *tmp;

  if ( blocks->getCountInserted() > 0 )
  {
    tmp = ( TRecord * )blocks->getLastRecord( 0 );
    if ( first.next == NULL )
    {
      first.next = tmp;
      first.threadNext = tmp;
    }
    for ( UINT16 i = 0; i < blocks->getCountInserted(); i++ )
    {
      tmp = ( TRecord * )blocks->getLastRecord( i );
      traceIndex.indexRecord( tmp );

      if ( emptyThreadBegin[ tmp->thread ].threadNext == NULL )
        emptyThreadBegin[ tmp->thread ].threadNext = tmp;
      if ( numCPUs > 0 && emptyCPUBegin[ tmp->CPU ].next == NULL )
        emptyCPUBegin[ tmp->CPU ].next = tmp;

      if ( last.prev != NULL )
        last.prev->next = tmp;
      if ( emptyThreadEnd[ tmp->thread ].threadPrev != NULL )
        emptyThreadEnd[ tmp->thread ].threadPrev->threadNext = tmp;
      tmp->prev = last.prev;
      tmp->threadPrev = emptyThreadEnd[ tmp->thread ].threadPrev;

      emptyThreadEnd[ tmp->thread ].threadPrev = tmp;
      if ( numCPUs > 0 )
        emptyCPUEnd[ tmp->CPU ].prev = tmp;
      last.prev = tmp;
      last.threadPrev = tmp;
    }

    blocks->resetCountInserted();
  }
}

TTime PlainTrace::finish( TTime headerTime )
{
  for ( vector<TRecord>::iterator it = emptyThreadEnd.begin();
        it != emptyThreadEnd.end(); ++it )
    ( *it ).time = headerTime;

  for ( vector<TRecord>::iterator it = emptyCPUEnd.begin();
        it != emptyCPUEnd.end(); ++it )
    ( *it ).time = headerTime;

  last.time = headerTime;

  return headerTime;
}

MemoryTrace::iterator* PlainTrace::begin() const
{
  return new PlainTrace::iterator( ( TRecord * )&first );
}

MemoryTrace::iterator* PlainTrace::end() const
{
  return new PlainTrace::iterator( ( TRecord * )&last );
}

MemoryTrace::iterator* PlainTrace::threadBegin( TThreadOrder whichThread ) const
{
  return new PlainTrace::ThreadIterator( ( TRecord * )&emptyThreadBegin[ whichThread ] );
}

MemoryTrace::iterator* PlainTrace::threadEnd( TThreadOrder whichThread ) const
{
  return new PlainTrace::ThreadIterator( ( TRecord * )&emptyThreadEnd[ whichThread ] );
}

MemoryTrace::iterator* PlainTrace::CPUBegin( TCPUOrder whichCPU ) const
{
  return new PlainTrace::CPUIterator( ( TRecord * )&emptyCPUBegin[ whichCPU ] );
}

MemoryTrace::iterator* PlainTrace::CPUEnd( TCPUOrder whichCPU ) const
{
  return new PlainTrace::CPUIterator( ( TRecord * )&emptyCPUEnd[ whichCPU ] );
}

void PlainTrace::getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                        TRecordTime whichTime ) const
{
  TRecord *current;
  TThreadOrder filled = 0;

  for ( TThreadOrder ii = 0; ii < numThreads; ii++ )
  {
    if ( listIter[ ii ] != NULL )
    {
      delete listIter[ ii ];
      listIter[ ii ] = NULL;
    }
  }

  // Search for especific time.
  current = traceIndex.findRecord( whichTime );
  while ( current != NULL )
  {
    if ( current->time < whichTime )
      break;
    current = current->prev;
  }

  // Backward search filling vector of iterators.
  while ( ( current != NULL ) && ( filled < numThreads ) )
  {
    if ( listIter[ current->thread ] == NULL )
    {
      listIter[ current->thread ] = new PlainTrace::ThreadIterator( current );
      filled++;
    }
    current = current->prev;
  }

  // Fill the rest.
  if ( filled < numThreads )
  {
    for ( TThreadOrder i = 0; i < numThreads; i++ )
      if ( listIter[ i ] == NULL )
        listIter[ i ] = threadBegin( i );
  }
}

void PlainTrace::getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                     TRecordTime whichTime ) const
{
  TRecord *current;
  TCPUOrder filled = 0;

  for ( TCPUOrder ii = 0; ii < numCPUs; ii++ )
  {
    if ( listIter[ ii ] != NULL )
      delete listIter[ ii ];
  }

  // Search for especific time.
  current = traceIndex.findRecord( whichTime );
  while ( current != NULL )
  {
    if ( current->time < whichTime )
      break;
    current = current->prev;
  }

  // Backward search filling vector of iterators.
  while ( ( current != NULL ) && ( filled < numCPUs ) )
  {
    if ( listIter[ current->CPU ] == NULL )
    {
      listIter[ current->CPU ] = new PlainTrace::CPUIterator( current );
      filled++;
    }
    current = current->prev;
  }

  // Fill the rest.
  if ( filled < numCPUs )
  {
    for ( TCPUOrder i = 0; i < numCPUs; i++ )
      if ( listIter[ i ] == NULL )
        listIter[ i ] = CPUBegin( i );
  }
}

inline PlainTrace::iterator::iterator( TRecord *whichRecord )
{
  record = whichRecord;
}

inline void PlainTrace::iterator::operator++()
{
  record = ( ( TRecord * )record )->next;
}

inline void PlainTrace::iterator::operator--()
{
  record = ( ( TRecord * )record )->prev;
}


inline TRecordType  PlainTrace::iterator::getType() const
{
  return ( ( TRecord * )record )->type;
}

inline TRecordTime  PlainTrace::iterator::getTime() const
{
  return ( ( TRecord * )record )->time;
}

inline TThreadOrder PlainTrace::iterator::getThread() const
{
  return ( ( TRecord * )record )->thread;
}

inline TCPUOrder    PlainTrace::iterator::getCPU() const
{
  return ( ( TRecord * )record )->CPU;
}

inline TObjectOrder PlainTrace::iterator::getOrder() const
{
  return ( ( TRecord * )record )->thread;
}

inline TEventType   PlainTrace::iterator::getEventType() const
{
  return ( ( TRecord * )record )->URecordInfo.eventRecord.type;
}

inline TEventValue  PlainTrace::iterator::getEventValue() const
{
  return ( ( TRecord * )record )->URecordInfo.eventRecord.value;
}

inline TState       PlainTrace::iterator::getState() const
{
  return ( ( TRecord * )record )->URecordInfo.stateRecord.state;
}

inline TRecordTime  PlainTrace::iterator::getStateEndTime() const
{
  return ( ( TRecord * )record )->URecordInfo.stateRecord.endTime;
}

inline TCommID      PlainTrace::iterator::getCommIndex() const
{
  return ( ( TRecord * )record )->URecordInfo.commRecord.index;
}


/**************************************************************************
 * MemoryTrace Inherited ThreadIterator.
 **************************************************************************/

inline TObjectOrder PlainTrace::ThreadIterator::getOrder() const
{
  return ( ( TRecord * )record )->thread;
}

inline void PlainTrace::ThreadIterator::operator++()
{
  record = ( ( TRecord * )record )->threadNext;
}

inline void PlainTrace::ThreadIterator::operator--()
{
  record = ( ( TRecord * )record )->threadPrev;
}

/**************************************************************************
 * MemoryTrace Inherited CPUIterator.
 **************************************************************************/

inline TObjectOrder PlainTrace::CPUIterator::getOrder() const
{
  return ( ( TRecord * )record )->CPU;
}

inline void PlainTrace::CPUIterator::operator++()
{
  TRecord *myRecord = ( TRecord * )this->record; // Keep current, maybe it's the last one.

  // Forward search, looking for next record with same CPU.
  record = ( ( TRecord * )record )->next;
  while ( record != NULL )
  {
    if ( ( ( TRecord * )record )->CPU == myRecord->CPU )
      break;
    record = ( ( TRecord * )record )->next;
  }
}

inline void PlainTrace::CPUIterator::operator--()
{
  TRecord *myRecord = ( TRecord * )this->record; // Keep current, maybe it's the last one.

  // Backward search, looking for previous record with same CPU.
  record = ( ( TRecord * )record )->prev;
  while ( record != NULL )
  {
    if ( ( ( TRecord * )record )->CPU == myRecord->CPU )
      break;
    record = ( ( TRecord * )record )->prev;
  }
}


MemoryTrace::iterator *PlainTrace::copyIterator( MemoryTrace::iterator *it )
{
  return new PlainTrace::iterator( *( ( PlainTrace::iterator * ) it ) );
}
MemoryTrace::iterator *PlainTrace::copyThreadIterator( MemoryTrace::iterator *it )
{
  return new PlainTrace::ThreadIterator( *( ( PlainTrace::ThreadIterator * ) it ) );
}
MemoryTrace::iterator *PlainTrace::copyCPUIterator( MemoryTrace::iterator *it )
{
  return new PlainTrace::CPUIterator( *( ( PlainTrace::CPUIterator * ) it ) );
}
