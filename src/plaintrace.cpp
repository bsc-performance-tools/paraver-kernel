#include "plaintrace.h"

using namespace Plain;

PlainTrace::PlainTrace( const TThreadOrder totalThreads,
                        const TCPUOrder totalCPUs ):
    numThreads( totalThreads ), numCPUs( totalCPUs )

{
}

void PlainTrace::insert( MemoryBlocks *blocks )
{
  if ( blocks->getCountInserted() > 0 )
  {
    for ( UINT16 i = 0; i < blocks->getCountInserted(); i++ )
    {
    }

    blocks->resetCountInserted();
  }
}

TTime PlainTrace::finish( TTime headerTime )
{
  return headerTime;
}

MemoryTrace::iterator* PlainTrace::begin() const
{
  return new PlainTrace::iterator( );
}

MemoryTrace::iterator* PlainTrace::end() const
{
  return new PlainTrace::iterator( );
}

MemoryTrace::iterator* PlainTrace::threadBegin( TThreadOrder whichThread ) const
{
  return new PlainTrace::ThreadIterator( );
}

MemoryTrace::iterator* PlainTrace::threadEnd( TThreadOrder whichThread ) const
{
  return new PlainTrace::ThreadIterator( );
}

MemoryTrace::iterator* PlainTrace::CPUBegin( TCPUOrder whichCPU ) const
{
  return new PlainTrace::CPUIterator( );
}

MemoryTrace::iterator* PlainTrace::CPUEnd( TCPUOrder whichCPU ) const
{
  return new PlainTrace::CPUIterator( );
}

void PlainTrace::getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                        TRecordTime whichTime ) const
{
}

void PlainTrace::getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                     TRecordTime whichTime ) const
{
}

inline void PlainTrace::iterator::operator++()
{
}

inline void PlainTrace::iterator::operator--()
{
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
}

inline void PlainTrace::ThreadIterator::operator--()
{
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
}

inline void PlainTrace::CPUIterator::operator--()
{
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
