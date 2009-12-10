#include "plaintrace.h"
#include "plainblocks.h"
#include "processmodel.h"
#include "resourcemodel.h"

using namespace Plain;
using namespace std;

PlainTrace::PlainTrace( const ProcessModel& whichProcessModel,
                        const ResourceModel& whichResourceModel ):
    processModel( whichProcessModel ), resourceModel( whichResourceModel )

{
  numThreads = processModel.totalThreads();
  numCPUs = resourceModel.totalCPUs();

  for ( TThreadOrder i = 0; i < numThreads; ++i )
    traceIndex.push_back( Index<pair<UINT32, UINT32> >( 1000 ) );
}

void PlainTrace::insert( MemoryBlocks *blocks )
{
  myBlocks = dynamic_cast<PlainBlocks *>( blocks );

  for ( UINT16 i = 0; i < blocks->getCountInserted(); i++ )
  {
    TLastRecord *tmpLast = ( TLastRecord * )blocks->getLastRecord( i );
    traceIndex[ tmpLast->thread ].indexRecord( tmpLast->time,
        pair<UINT32, UINT32>( tmpLast->block,
                              tmpLast->pos )
                                             );
  }

  blocks->resetCountInserted();
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
  return new PlainTrace::ThreadIterator( myBlocks, 0, 0, whichThread );
}

MemoryTrace::iterator* PlainTrace::threadEnd( TThreadOrder whichThread ) const
{
  UINT32 block = myBlocks->blocks[ whichThread ].size() - 1;
  UINT32 pos = myBlocks->currentRecord[ whichThread ];
  return new PlainTrace::ThreadIterator( myBlocks, block, pos, whichThread );
}

MemoryTrace::iterator* PlainTrace::CPUBegin( TCPUOrder whichCPU ) const
{
  UINT32 *block;
  UINT32 *pos;
  TThreadOrder *threads;
  TThreadOrder numThreads;
  vector<TThreadOrder> tmpThreads;
  TNodeOrder tmpNode;
  TCPUOrder tmpCPU;

  resourceModel.getCPULocation( whichCPU, tmpNode, tmpCPU );
  processModel.getThreadsPerNode( tmpNode, tmpThreads );

  numThreads = tmpThreads.size();
  threads = new TThreadOrder[ numThreads ];
  block = new UINT32[ numThreads ];
  pos = new UINT32[ numThreads ];
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    block[ iThread ] = 0;
    pos[ iThread ] = 0;
    threads[ iThread ] = tmpThreads[ iThread ];
  }

  return new PlainTrace::CPUIterator( myBlocks, block, pos, numThreads, threads, whichCPU );
}

MemoryTrace::iterator* PlainTrace::CPUEnd( TCPUOrder whichCPU ) const
{
  UINT32 *block;
  UINT32 *pos;
  TThreadOrder *threads;
  TThreadOrder numThreads;
  vector<TThreadOrder> tmpThreads;
  TNodeOrder tmpNode;
  TCPUOrder tmpCPU;

  resourceModel.getCPULocation( whichCPU, tmpNode, tmpCPU );
  processModel.getThreadsPerNode( tmpNode, tmpThreads );

  numThreads = tmpThreads.size();
  threads = new TThreadOrder[ numThreads ];
  block = new UINT32[ numThreads ];
  pos = new UINT32[ numThreads ];
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    block[ iThread ] = myBlocks->blocks[ iThread ].size() - 1;;
    pos[ iThread ] = myBlocks->currentRecord[ iThread ];
    threads[ iThread ] = tmpThreads[ iThread ];
  }

  return new PlainTrace::CPUIterator( myBlocks, block, pos, numThreads, threads, whichCPU );
}

void PlainTrace::getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                        TRecordTime whichTime ) const
{
  for ( TThreadOrder ii = 0; ii < numThreads; ++ii )
  {
    if ( listIter[ ii ] != NULL )
    {
      delete listIter[ ii ];
      listIter[ ii ] = NULL;
    }
  }

  for ( TThreadOrder ii = 0; ii < numThreads; ++ii )
  {
    pair<UINT32, UINT32> blockPos = traceIndex[ ii ].findRecord( whichTime );
    ThreadIterator *tmpIt = new ThreadIterator( myBlocks, blockPos.first, blockPos.second, ii );
    while ( !tmpIt->isNull() && tmpIt->getTime() > whichTime )
      --( *tmpIt );
    if ( tmpIt->isNull() )
    {
      delete tmpIt;
      tmpIt = ( PlainTrace::ThreadIterator * )threadBegin( ii );
    }
    listIter[ ii ] = tmpIt;
  }
}

void PlainTrace::getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                     TRecordTime whichTime ) const
{
  for ( TCPUOrder ii = 0; ii < numCPUs; ++ii )
  {
    if ( listIter[ ii ] != NULL )
    {
      delete listIter[ ii ];
      listIter[ ii ] = NULL;
    }
  }

  for ( TCPUOrder ii = 0; ii < numCPUs; ++ii )
  {
    UINT32 *block;
    UINT32 *pos;
    TThreadOrder *threads;
    TThreadOrder numThreads;
    vector<TThreadOrder> tmpThreads;
    TNodeOrder tmpNode;
    TCPUOrder tmpCPU;

    resourceModel.getCPULocation( ii, tmpNode, tmpCPU );
    processModel.getThreadsPerNode( tmpNode, tmpThreads );

    numThreads = tmpThreads.size();
    threads = new TThreadOrder[ numThreads ];
    block = new UINT32[ numThreads ];
    pos = new UINT32[ numThreads ];
    for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
    {
      pair<UINT32, UINT32> blockPos = traceIndex[ iThread ].findRecord( whichTime );
      block[ iThread ] = 0;
      pos[ iThread ] = 0;
      threads[ iThread ] = tmpThreads[ iThread ];
    }

    CPUIterator *tmpIt = new PlainTrace::CPUIterator( myBlocks, block, pos, numThreads, threads, ii );

    while ( !tmpIt->isNull() && tmpIt->getTime() > whichTime )
      --( *tmpIt );
    if ( tmpIt->isNull() )
    {
      delete tmpIt;
      tmpIt = ( PlainTrace::CPUIterator * )CPUBegin( ii );
    }
    listIter[ ii ] = tmpIt;
  }
}

PlainTrace::iterator::iterator( PlainBlocks *whichBlocks )
    : blocks( whichBlocks )
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

inline TCPUOrder    PlainTrace::iterator::getCPU() const
{
  return ( ( TRecord * )record )->CPU;
}

inline TObjectOrder PlainTrace::iterator::getOrder() const
{
  return 0;
}

inline TObjectOrder PlainTrace::iterator::getThread() const
{
  return 0;
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

PlainTrace::ThreadIterator::ThreadIterator( PlainBlocks *whichBlocks, UINT32 whichBlock, UINT32 whichPos,
    TThreadOrder whichThread )
    : PlainTrace::iterator( whichBlocks ), thread( whichThread ), block( whichBlock ), pos( whichPos )
{
  record = &blocks->blocks[ thread ][ block ][ pos ];
  lastBlock = blocks->blocks[ thread ].size() - 1;
  lastPos = blocks->currentRecord[ thread ];
}

inline TThreadOrder PlainTrace::ThreadIterator::getThread() const
{
  return thread;
}

inline TObjectOrder PlainTrace::ThreadIterator::getOrder() const
{
  return thread;
}

inline void PlainTrace::ThreadIterator::operator++()
{
  if ( pos == lastPos && block == lastBlock )
  {
    record = NULL;
    return;
  }

  if ( pos == PlainBlocks::blockSize - 1 )
  {
    ++block;
    pos = 0;
    record = &blocks->blocks[ thread ][ block ][ pos ];
    return;
  }

  ++pos;
  record += sizeof( TRecord );
}

inline void PlainTrace::ThreadIterator::operator--()
{
  if ( pos == 0 && block == 0  )
  {
    record = NULL;
    return;
  }

  if ( pos == 0 )
  {
    pos = PlainBlocks::blockSize - 1;
    --block;
    record = &blocks->blocks[ thread ][ block ][ pos ];
    return;
  }

  --pos;
  record -= sizeof( TRecord );
}

/**************************************************************************
 * MemoryTrace Inherited CPUIterator.
 **************************************************************************/

PlainTrace::CPUIterator::CPUIterator( PlainBlocks *whichBlocks, UINT32 *whichBlock, UINT32 *whichPos,
                                      TThreadOrder whichNumThreads, TThreadOrder *whichThreads, TCPUOrder whichCPU )
    : PlainTrace::iterator( whichBlocks ), cpu( whichCPU ), numThreads( whichNumThreads ),
    threads( whichThreads ), block( whichBlock ), pos( whichPos )
{
  lastBlock = new UINT32[ numThreads ];
  lastPos = new UINT32[ numThreads ];

  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    lastBlock[ iThread ] = blocks->blocks[ threads[ iThread ] ].size() - 1;
    lastPos[ iThread ] = blocks->currentRecord[ threads[ iThread ] ];
  }

  lastThread = minThread();
  record = &blocks->blocks[ threads[ lastThread ] ][ block[ lastThread ] ][ pos[ lastThread ] ];
}

PlainTrace::CPUIterator::~CPUIterator()
{
  delete[] threads;
  delete[] block;
  delete[] pos;
  delete[] lastBlock;
  delete[] lastPos;
}

inline TThreadOrder PlainTrace::CPUIterator::getThread() const
{
  return lastThread;
}

inline TObjectOrder PlainTrace::CPUIterator::getOrder() const
{
  return cpu;
}

inline void PlainTrace::CPUIterator::operator++()
{
  TThreadOrder inLastPos = 0;
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( pos[ iThread ] == lastPos[ iThread ] && block[ iThread ] == lastBlock[ iThread ] )
      ++inLastPos;
  }
  if ( inLastPos == numThreads )
  {
    record = NULL;
    return;
  }

  if ( pos[ lastThread ] == PlainBlocks::blockSize - 1 )
  {
    ++block[ lastThread ];
    pos[ lastThread ] = 0;
    lastThread = minThread();
    record = &blocks->blocks[ threads[ lastThread ] ][ block[ lastThread ] ][ pos[ lastThread ] ];
    return;
  }

  ++pos[ lastThread ];
  lastThread = minThread();
  record = &blocks->blocks[ threads[ lastThread ] ][ block[ lastThread ] ][ pos[ lastThread ] ];
}

inline void PlainTrace::CPUIterator::operator--()
{
  TThreadOrder inFirstPos = 0;
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( pos[ iThread ] == 0 && block[ iThread ] == 0 )
      ++inFirstPos;
  }
  if ( inFirstPos == numThreads )
  {
    record = NULL;
    return;
  }

  if ( pos[ lastThread ] == 0 )
  {
    pos[ lastThread ] = PlainBlocks::blockSize - 1;
    --block[ lastThread ];
    lastThread = maxThread();
    record = &blocks->blocks[ threads[ lastThread ] ][ block[ lastThread ] ][ pos[ lastThread ] ];
    return;
  }

  --pos[ lastThread ];
  lastThread = maxThread();
  record = &blocks->blocks[ threads[ lastThread ] ][ block[ lastThread ] ][ pos[ lastThread ] ];
}

struct ltrecord
{
  bool operator()( TRecord *r1, TRecord *r2 ) const
  {
    if ( r1->time < r2->time )
      return true;
    if ( getTypeOrdered( r1 ) < getTypeOrdered( r2 ) )
      return true;
    return false;
  }
};

inline TThreadOrder PlainTrace::CPUIterator::minThread()
{
  map<TRecord *, TThreadOrder, ltrecord> sortedRecords;

  setToMyCPUForward();
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( pos[ iThread ] <= lastPos[ iThread ] && block[ iThread ] <= lastBlock[ iThread ] )
      sortedRecords[ &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ] ] = iThread;
  }

  return sortedRecords.begin()->second;
}

inline TThreadOrder PlainTrace::CPUIterator::maxThread()
{
  map<TRecord *, TThreadOrder, ltrecord> sortedRecords;

  setToMyCPUBackward();
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( pos[ iThread ] <= lastPos[ iThread ] && block[ iThread ] <= lastBlock[ iThread ] )
      sortedRecords[ &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ] ] = iThread;
  }

  map<TRecord *, TThreadOrder, ltrecord>::iterator it = sortedRecords.end();
  --it;
  return it->second;
}

inline void PlainTrace::CPUIterator::setToMyCPUForward()
{
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( pos[ iThread ] <= lastPos[ iThread ] && block[ iThread ] <= lastBlock[ iThread ] )
    {
      TRecord *tmpRec = &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ];
      while ( tmpRec->CPU != cpu && pos[ iThread ] <= lastPos[ iThread ] && block[ iThread ] <= lastBlock[ iThread ] )
      {
        if ( pos[ iThread ] == PlainBlocks::blockSize - 1 )
        {
          ++block[ iThread ];
          pos[ iThread ] = 0;
        }
        else
          ++pos[ iThread ];
        record = &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ];
      }
    }
  }
}

inline void PlainTrace::CPUIterator::setToMyCPUBackward()
{
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( pos[ iThread ] <= lastPos[ iThread ] && block[ iThread ] <= lastBlock[ iThread ] )
    {
      TRecord *tmpRec = &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ];
      while ( tmpRec->CPU != cpu )
      {
        if ( pos[ iThread ] == 0 )
        {
          --block[ iThread ];
          pos[ iThread ] = PlainBlocks::blockSize - 1;
        }
        else
          --pos[ iThread ];
        record = &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ];
      }
    }
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
