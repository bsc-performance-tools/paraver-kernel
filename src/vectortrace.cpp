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

#include <algorithm>

#include "ktrace.h"
#include "vectorblocks.h"
#include "vectortrace.h"

using Plain::TRecord;

/********************************************************/
/*                iterator                              */
/********************************************************/
VectorTrace::iterator::iterator( TThreadRecordContainer::iterator whichRecord, const Trace *whichTrace, VectorBlocks *whichBlocks, TThreadOrder whichThread )
  : it( whichRecord ), MemoryTrace::iterator( whichTrace ), myBlocks( whichBlocks ), myThread( whichThread )
{}

void VectorTrace::iterator::operator++()
{
  if( it != myBlocks->threadRecords[ myThread ].end() )
  {
    record = nullptr;
    ++it;
  }
}

void VectorTrace::iterator::operator--()
{
  if( it != myBlocks->threadRecords[ myThread ].begin() )
  {
    record = nullptr;
    --it;
  }
}

MemoryTrace::iterator& VectorTrace::iterator::operator=( const MemoryTrace::iterator& copy )
{
  const auto &tmpCopy = dynamic_cast<const VectorTrace::iterator&>( copy );
  
  myThread = tmpCopy.myThread;
  it = tmpCopy.it;
  myBlocks = tmpCopy.myBlocks;
  myTrace = tmpCopy.myTrace;
  record = tmpCopy.record;

  return *this;
}

bool VectorTrace::iterator::operator==( const MemoryTrace::iterator &whichit ) const
{
  if ( record != nullptr )
    return record == dynamic_cast<const VectorTrace::iterator&>( whichit ).record;

  return it == dynamic_cast<const VectorTrace::iterator&>( whichit ).it;
}

bool VectorTrace::iterator::operator!=( const MemoryTrace::iterator &whichit ) const
{
  if ( record != nullptr )
    return record != dynamic_cast<const VectorTrace::iterator&>( whichit ).record;

  return it != dynamic_cast<const VectorTrace::iterator&>( whichit ).it;
}

bool VectorTrace::iterator::isNull() const
{
  return record == nullptr &&
         ( it == myBlocks->threadRecords[ myThread ].end() ||
           it == myBlocks->threadRecords[ myThread ].begin() ||
           it->type == EMPTYREC );
}


VectorTrace::iterator *VectorTrace::iterator::clone() const
{
  auto tmpIt = new VectorTrace::iterator( it, myTrace, myBlocks, myThread );
  tmpIt->record = record;

  return tmpIt;
}

TRecordType VectorTrace::iterator::getRecordType() const
{
  return record == nullptr ? it->type : static_cast<const Plain::TRecord *>( record )->type;
}

TRecordTime VectorTrace::iterator::getTime() const
{
  return record == nullptr ? it->time : static_cast<const Plain::TRecord *>( record )->time;
}

TThreadOrder VectorTrace::iterator::getThread() const
{
  return myThread;
}

TCPUOrder VectorTrace::iterator::getCPU() const
{
  return record == nullptr ? it->CPU : static_cast<const Plain::TRecord *>( record )->CPU;
}

TObjectOrder VectorTrace::iterator::getOrder() const
{
  return myThread;
}

TEventType VectorTrace::iterator::getEventType() const
{
  return record == nullptr ? it->URecordInfo.eventRecord.type : static_cast<const Plain::TRecord *>( record )->URecordInfo.eventRecord.type;
}

TSemanticValue VectorTrace::iterator::getEventValue() const
{
  double tmpPrecision = myTrace->getEventTypePrecision( getEventType() );
  if( tmpPrecision != 0.0 )
    return getEventValueAsIs() * tmpPrecision;

  return getEventValueAsIs();
}

TEventValue VectorTrace::iterator::getEventValueAsIs() const
{
  return record == nullptr ? it->URecordInfo.eventRecord.value : static_cast<const Plain::TRecord *>( record )->URecordInfo.eventRecord.value;
}

TState VectorTrace::iterator::getState() const
{
  return record == nullptr ? it->URecordInfo.stateRecord.state : static_cast<const Plain::TRecord *>( record )->URecordInfo.stateRecord.state;
}

TRecordTime VectorTrace::iterator::getStateEndTime() const
{
  return record == nullptr ? it->URecordInfo.stateRecord.endTime : static_cast<const Plain::TRecord *>( record )->URecordInfo.stateRecord.endTime;
}

TCommID VectorTrace::iterator::getCommIndex() const
{
  return record == nullptr ? it->URecordInfo.commRecord.index : static_cast<const Plain::TRecord *>( record )->URecordInfo.commRecord.index;
}

void VectorTrace::iterator::setTime( const TRecordTime whichTime )
{
  if ( record != nullptr )
    static_cast<Plain::TRecord *>( record )->time = whichTime;
  else
    it->time = whichTime;
}

void VectorTrace::iterator::setRecordType( const TRecordType whichType )
{
  if ( record != nullptr )
    static_cast<Plain::TRecord *>( record )->type = whichType;
  else
    it->type = whichType;
}

void VectorTrace::iterator::setStateEndTime( const TRecordTime whichEndTime )
{
  if ( record != nullptr )
    static_cast<Plain::TRecord *>( record )->URecordInfo.stateRecord.endTime = whichEndTime;
  else
    it->URecordInfo.stateRecord.endTime = whichEndTime;
}


/********************************************************/
/*                CPUIterator                           */
/********************************************************/
VectorTrace::CPUIterator::CPUIterator( TCPURecordContainer::iterator whichRecord, const Trace *whichTrace, VectorBlocks *whichBlocks, TCPUOrder whichCPU )
  : it( whichRecord ), MemoryTrace::iterator( whichTrace ), myBlocks( whichBlocks ), myCPU( whichCPU )
{}

void VectorTrace::CPUIterator::operator++()
{
  if( it != myBlocks->cpuRecords[ myCPU ].end() )
  {
    record = nullptr;
    ++it;
  }
}

void VectorTrace::CPUIterator::operator--()
{
  if( it != myBlocks->cpuRecords[ myCPU ].begin() )
  {
    record = nullptr;
    --it;
  }
}

MemoryTrace::iterator& VectorTrace::CPUIterator::operator=( const MemoryTrace::iterator& copy )
{
  const auto &tmpCopy = dynamic_cast<const VectorTrace::CPUIterator&>( copy );
  
  myCPU = tmpCopy.myCPU;
  it = tmpCopy.it;
  myBlocks = tmpCopy.myBlocks;
  myTrace = tmpCopy.myTrace;
  record = tmpCopy.record;

  return *this;
}

bool VectorTrace::CPUIterator::operator==( const MemoryTrace::iterator &whichit ) const
{
  if ( record != nullptr )
    return record == dynamic_cast<const VectorTrace::CPUIterator&>( whichit ).record;

  return it == dynamic_cast<const VectorTrace::CPUIterator&>( whichit ).it;
}

bool VectorTrace::CPUIterator::operator!=( const MemoryTrace::iterator &whichit ) const
{
  if ( record != nullptr )
    return record != dynamic_cast<const VectorTrace::CPUIterator&>( whichit ).record;

  return it != dynamic_cast<const VectorTrace::CPUIterator&>( whichit ).it;
}

bool VectorTrace::CPUIterator::isNull() const
{
  return record == nullptr &&
         ( it == myBlocks->cpuRecords[ myCPU ].end() ||
           it == myBlocks->cpuRecords[ myCPU ].begin() ||
           (*it)->type == EMPTYREC );
}


VectorTrace::CPUIterator *VectorTrace::CPUIterator::clone() const
{
  auto tmpIt = new VectorTrace::CPUIterator( it, myTrace, myBlocks, myCPU );
  tmpIt->record = record;

  return tmpIt;
}

TRecordType VectorTrace::CPUIterator::getRecordType() const
{
  return record == nullptr ? (*it)->type : static_cast<const Plain::TRecord *>( record )->type;
}

TRecordTime VectorTrace::CPUIterator::getTime() const
{
  return record == nullptr ? (*it)->time : static_cast<const Plain::TRecord *>( record )->time;
}

TThreadOrder VectorTrace::CPUIterator::getThread() const
{
  return record == nullptr ? (*it)->thread : static_cast<const Plain::TRecord *>( record )->thread;
}

TCPUOrder VectorTrace::CPUIterator::getCPU() const
{
  return myCPU + 1;
}

TObjectOrder VectorTrace::CPUIterator::getOrder() const
{
  return myCPU + 1;
}

TEventType VectorTrace::CPUIterator::getEventType() const
{
  return record == nullptr ? (*it)->URecordInfo.eventRecord.type : static_cast<const Plain::TRecord *>( record )->URecordInfo.eventRecord.type;
}

TSemanticValue VectorTrace::CPUIterator::getEventValue() const
{
  double tmpPrecision = myTrace->getEventTypePrecision( getEventType() );
  if( tmpPrecision != 0.0 )
    return getEventValueAsIs() * tmpPrecision;

  return getEventValueAsIs();
}

TEventValue VectorTrace::CPUIterator::getEventValueAsIs() const
{
  return record == nullptr ? (*it)->URecordInfo.eventRecord.value : static_cast<const Plain::TRecord *>( record )->URecordInfo.eventRecord.value;
}

TState VectorTrace::CPUIterator::getState() const
{
  return record == nullptr ? (*it)->URecordInfo.stateRecord.state : static_cast<const Plain::TRecord *>( record )->URecordInfo.stateRecord.state;
}

TRecordTime VectorTrace::CPUIterator::getStateEndTime() const
{
  return record == nullptr ? (*it)->URecordInfo.stateRecord.endTime : static_cast<const Plain::TRecord *>( record )->URecordInfo.stateRecord.endTime;
}

TCommID VectorTrace::CPUIterator::getCommIndex() const
{
  return record == nullptr ? (*it)->URecordInfo.commRecord.index : static_cast<const Plain::TRecord *>( record )->URecordInfo.commRecord.index;
}

void VectorTrace::CPUIterator::setTime( const TRecordTime whichTime )
{
  if ( record != nullptr )
    static_cast<Plain::TRecord *>( record )->time = whichTime;
  else
    (*it)->time = whichTime;
}

void VectorTrace::CPUIterator::setRecordType( const TRecordType whichType )
{
  if ( record != nullptr )
    static_cast<Plain::TRecord *>( record )->type = whichType;
  else
    (*it)->type = whichType;
}

void VectorTrace::CPUIterator::setStateEndTime( const TRecordTime whichEndTime )
{
  if ( record != nullptr )
    static_cast<Plain::TRecord *>( record )->URecordInfo.stateRecord.endTime = whichEndTime;
  else
    (*it)->URecordInfo.stateRecord.endTime = whichEndTime;
}


/********************************************************/
/*                VectorTrace                           */
/********************************************************/
void VectorTrace::insert( MemoryBlocks *blocks )
{
  myBlocks = dynamic_cast<VectorBlocks *>( blocks );
  blocks->resetCountInserted();
}

TTime VectorTrace::finish( TTime headerTime, Trace *whichTrace )
{
  myTrace = whichTrace;
  return ( headerTime > myBlocks->getLastRecordTime() ? headerTime : myBlocks->getLastRecordTime() );
}

MemoryTrace::iterator* VectorTrace::empty() const
{
  return threadBegin( 0 );
}

MemoryTrace::iterator* VectorTrace::begin() const
{
  return threadBegin( 0 );
}

MemoryTrace::iterator* VectorTrace::end() const
{
  return threadEnd( 0 );
}

MemoryTrace::iterator* VectorTrace::threadBegin( TThreadOrder whichThread ) const
{
  return new VectorTrace::iterator( myBlocks->threadRecords[ whichThread ].begin(), myTrace, myBlocks, whichThread );
}

MemoryTrace::iterator* VectorTrace::threadEnd( TThreadOrder whichThread ) const
{
  return new VectorTrace::iterator( --myBlocks->threadRecords[ whichThread ].end(), myTrace, myBlocks, whichThread );
}

MemoryTrace::iterator* VectorTrace::CPUBegin( TCPUOrder whichCPU ) const
{
  return new VectorTrace::CPUIterator( myBlocks->cpuRecords[ whichCPU ].begin(), myTrace, myBlocks, whichCPU );
}

MemoryTrace::iterator* VectorTrace::CPUEnd( TCPUOrder whichCPU ) const
{
  return new VectorTrace::CPUIterator( --myBlocks->cpuRecords[ whichCPU ].end(), myTrace, myBlocks, whichCPU );
}

void VectorTrace::getRecordByTimeThread( std::vector<MemoryTrace::iterator *>& listIter, TRecordTime whichTime ) const
{
  size_t iThread = 0;
  for( auto& v : myBlocks->threadRecords )
  {
    auto it = std::lower_bound( v.begin(), v.end(), whichTime, []( const auto& el, const auto& time ) { return el.time < time; } );
    if( it == v.end() || ( it != v.begin() && it->time > whichTime ) ) --it;
    listIter[ iThread ] = new VectorTrace::iterator( it, myTrace, myBlocks, iThread );

    ++iThread;
  }
}

void VectorTrace::getRecordByTimeCPU( std::vector<MemoryTrace::iterator *>& listIter, TRecordTime whichTime ) const
{
  size_t iCPU = 0;
  for( auto& v : myBlocks->cpuRecords )
  {
    auto it = std::lower_bound( v.begin(), v.end(), whichTime, []( const auto& el, const auto& time ) { return el->time < time; } );
    if( it == v.end() || ( it != v.begin() && (*it)->time > whichTime ) ) --it;
    listIter[ iCPU ] = new VectorTrace::CPUIterator( it, myTrace, myBlocks, iCPU );

    ++iCPU;
  }
}

