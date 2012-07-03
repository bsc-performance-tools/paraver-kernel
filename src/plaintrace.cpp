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

#include "plaintrace.h"
#include "plainblocks.h"
#include "processmodel.h"
#include "resourcemodel.h"
#include <iostream>

using namespace Plain;
using namespace std;

PlainTrace::PlainTrace( const ProcessModel& whichProcessModel,
                        const ResourceModel& whichResourceModel ):
    processModel( whichProcessModel ), resourceModel( whichResourceModel )

{
  numThreads = processModel.totalThreads();
  numCPUs = resourceModel.totalCPUs();

  for ( TThreadOrder i = 0; i < numThreads; ++i )
    traceIndex.push_back( Index<pair<PRV_UINT32, PRV_UINT32> >( 1000 ) );
}

void PlainTrace::insert( MemoryBlocks *blocks )
{
  myBlocks = dynamic_cast<PlainBlocks *>( blocks );

  for ( PRV_UINT16 i = 0; i < blocks->getCountInserted(); i++ )
  {
    TLastRecord *tmpLast = ( TLastRecord * )blocks->getLastRecord( i );
    traceIndex[ tmpLast->thread ].indexRecord( tmpLast->time,
        pair<PRV_UINT32, PRV_UINT32>( tmpLast->block,
                              tmpLast->pos )
                                             );
  }

  blocks->resetCountInserted();
}

TTime PlainTrace::finish( TTime headerTime, Trace *whichTrace )
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
  PRV_UINT32 block = myBlocks->blocks[ whichThread ].size() - 1;
  PRV_UINT32 pos = myBlocks->currentRecord[ whichThread ];
  return new PlainTrace::ThreadIterator( myBlocks, block, pos, whichThread );
}

MemoryTrace::iterator* PlainTrace::CPUBegin( TCPUOrder whichCPU ) const
{
  vector<PRV_UINT32> block;
  vector<PRV_UINT32> pos;
  TThreadOrder numThreads;
  vector<TThreadOrder> threads;
  TNodeOrder tmpNode;
  TCPUOrder tmpCPU;

  resourceModel.getCPULocation( whichCPU, tmpNode, tmpCPU );
  processModel.getThreadsPerNode( tmpNode, threads );

  numThreads = threads.size();
  block.insert( block.begin(), numThreads, 0 );
  pos.insert( pos.begin(), numThreads, 0 );

  return new PlainTrace::CPUIterator( myBlocks, block, pos, numThreads, threads, whichCPU );
}

MemoryTrace::iterator* PlainTrace::CPUEnd( TCPUOrder whichCPU ) const
{
  vector<PRV_UINT32> block;
  vector<PRV_UINT32> pos;
  TThreadOrder numThreads;
  vector<TThreadOrder> threads;
  TNodeOrder tmpNode;
  TCPUOrder tmpCPU;

  resourceModel.getCPULocation( whichCPU, tmpNode, tmpCPU );
  processModel.getThreadsPerNode( tmpNode, threads );

  numThreads = threads.size();
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    block.push_back( myBlocks->blocks[ iThread ].size() - 1 );
    pos.push_back( myBlocks->currentRecord[ iThread ] );
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
    pair<PRV_UINT32, PRV_UINT32> blockPos;
    ThreadIterator *tmpIt;
    if ( traceIndex[ ii ].findRecord( whichTime, blockPos ) )
    {
      tmpIt = new ThreadIterator( myBlocks, blockPos.first, blockPos.second, ii );
      while ( !tmpIt->isNull() && tmpIt->getTime() > whichTime )
        --( *tmpIt );
      if ( tmpIt->isNull() )
      {
        delete tmpIt;
        tmpIt = ( PlainTrace::ThreadIterator * )threadBegin( ii );
      }
    }
    else
      tmpIt = ( PlainTrace::ThreadIterator * )threadBegin( ii );
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
    vector<PRV_UINT32> block;
    vector<PRV_UINT32> pos;
    TThreadOrder numThreads;
    vector<TThreadOrder> threads;
    TNodeOrder tmpNode;
    TCPUOrder tmpCPU;

    resourceModel.getCPULocation( ii, tmpNode, tmpCPU );
    processModel.getThreadsPerNode( tmpNode, threads );

    numThreads = threads.size();
    for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
    {
      pair<PRV_UINT32, PRV_UINT32> blockPos;
      if ( traceIndex[ iThread ].findRecord( whichTime, blockPos ) )
      {
        block.push_back( blockPos.first );
        pos.push_back( blockPos.second );
      }
      else
      {
        block.push_back( 0 );
        pos.push_back( 0 );
      }
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

PlainTrace::ThreadIterator::ThreadIterator( PlainBlocks *whichBlocks, PRV_UINT32 whichBlock, PRV_UINT32 whichPos,
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

MemoryTrace::iterator& PlainTrace::ThreadIterator::operator=( const MemoryTrace::iterator & copy )
{
  if ( this != &copy )
  {
    PlainTrace::ThreadIterator *tmpCopy = ( PlainTrace::ThreadIterator * ) & copy;
    thread = tmpCopy->thread;
    block = tmpCopy->block;
    pos = tmpCopy->pos;
    lastBlock = tmpCopy->lastBlock;
    lastPos = tmpCopy->lastPos;
    record = tmpCopy->getRecord();
  }
  return *this;
}
/**************************************************************************
 * MemoryTrace Inherited CPUIterator.
 **************************************************************************/

PlainTrace::CPUIterator::CPUIterator( PlainBlocks *whichBlocks, vector<PRV_UINT32>& whichBlock, vector<PRV_UINT32>& whichPos,
                                      TThreadOrder whichNumThreads, vector<TThreadOrder>& whichThreads, TCPUOrder whichCPU )
    : PlainTrace::iterator( whichBlocks ), cpu( whichCPU ), numThreads( whichNumThreads ),
    threads( whichThreads ), block( whichBlock ), pos( whichPos )
{
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    lastBlock.push_back( blocks->blocks[ threads[ iThread ] ].size() - 1 );
    lastPos.push_back( blocks->currentRecord[ threads[ iThread ] ] + 1 );
  }
  lastThread = minThread();
  record = &blocks->blocks[ threads[ lastThread ] ][ block[ lastThread ] ][ pos[ lastThread ] ];
}

PlainTrace::CPUIterator::~CPUIterator()
{}

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
  ++pos[ lastThread ];
  if ( pos[ lastThread ] >= PlainBlocks::blockSize )
  {
    if ( block[ lastThread ] < lastBlock[ lastThread ] )
    {
      ++block[ lastThread ];
      pos[ lastThread ] = 0;
    }
  }
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( pos[ iThread ] == lastPos[ iThread ] && block[ iThread ] == lastBlock[ iThread ] )
    {
      ++inLastPos;
    }
  }
  if ( inLastPos == numThreads )
  {
    record = NULL;
    return;
  }

  lastThread = minThread();
  record = &blocks->blocks[ threads[ lastThread ] ][ block[ lastThread ] ][ pos[ lastThread ] ];
}

inline void PlainTrace::CPUIterator::operator--()
{
  TThreadOrder inFirstPos = 0;
  if ( pos[ lastThread ] == 0 )
  {
    if ( block[ lastThread ] > 0 )
    {
      pos[ lastThread ] = PlainBlocks::blockSize - 1;
      --block[ lastThread ];
    }
  }
  else
    --pos[ lastThread ];
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

  lastThread = maxThread();
  record = &blocks->blocks[ threads[ lastThread ] ][ block[ lastThread ] ][ pos[ lastThread ] ];
}

MemoryTrace::iterator& PlainTrace::CPUIterator::operator=( const MemoryTrace::iterator & copy )
{
  if ( this != &copy )
  {
    PlainTrace::CPUIterator *tmpCopy = ( PlainTrace::CPUIterator * ) & copy;
    cpu = tmpCopy->cpu;
    numThreads = tmpCopy->numThreads;
    threads = tmpCopy->threads;
    block = tmpCopy->block;
    pos = tmpCopy->pos;
    lastBlock = tmpCopy->lastBlock;
    lastPos = tmpCopy->lastPos;
    lastThread = tmpCopy->lastThread;
    record = tmpCopy->getRecord();
  }
  return *this;
}

inline TThreadOrder PlainTrace::CPUIterator::minThread()
{
  map<TRecord *, TThreadOrder, Plain::ltrecord> sortedRecords;

  setToMyCPUForward();
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( !( pos[ iThread ] == lastPos[ iThread ] && block[ iThread ] == lastBlock[ iThread ] ) )
      sortedRecords[ &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ] ] = iThread;
  }

  if ( sortedRecords.begin() == sortedRecords.end() )
    return 0;

  return sortedRecords.begin()->second;
}

inline TThreadOrder PlainTrace::CPUIterator::maxThread()
{
  map<TRecord *, TThreadOrder, Plain::ltrecord> sortedRecords;

  setToMyCPUBackward();
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( !( pos[ iThread ] == 0 && block[ iThread ] == 0 ) )
      sortedRecords[ &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ] ] = iThread;
  }

  if ( sortedRecords.begin() == sortedRecords.end() )
    return 0;

  map<TRecord *, TThreadOrder, ltrecord>::iterator it = sortedRecords.end();
  --it;
  return it->second;
}

inline void PlainTrace::CPUIterator::setToMyCPUForward()
{
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( !( pos[ iThread ] == lastPos[ iThread ] && block[ iThread ] == lastBlock[ iThread ] ) )
    {
      TRecord *tmpRec = &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ];
      if( tmpRec->type == EMPTYREC )
        return;
      while ( !( pos[ iThread ] >= lastPos[ iThread ] && block[ iThread ] >= lastBlock[ iThread ] )
              && tmpRec->CPU != cpu )
      {
        ++pos[ iThread ];
        if ( pos[ iThread ] >= PlainBlocks::blockSize )
        {
          if ( block[ iThread ] < lastBlock[ iThread ] )
          {
            ++block[ iThread ];
            pos[ iThread ] = 0;
          }
        }
        tmpRec = &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ];
      }
    }
  }
}

inline void PlainTrace::CPUIterator::setToMyCPUBackward()
{
  for ( TThreadOrder iThread = 0; iThread < numThreads; ++iThread )
  {
    if ( !( pos[ iThread ] == 0 && block[ iThread ] == 0 ) )
    {
      TRecord *tmpRec = &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ];
      if( tmpRec->type == EMPTYREC )
        return;
      while ( !( pos[ iThread ] == 0 && block[ iThread ] == 0 ) && tmpRec->CPU != cpu )
      {
        if ( pos[ iThread ] == 0 )
        {
          if( block[ iThread ] > 0 )
          {
            --block[ iThread ];
            pos[ iThread ] = PlainBlocks::blockSize - 1;
          }
        }
        else
          --pos[ iThread ];
        tmpRec = &blocks->blocks[ threads[ iThread ] ][ block[ iThread ] ][ pos[ iThread ] ];
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
