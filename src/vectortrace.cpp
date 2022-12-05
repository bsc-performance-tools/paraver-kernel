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

VectorTrace::iterator::iterator( std::vector<Plain::TRecord>::iterator whichRecord, const Trace *whichTrace, VectorBlocks *whichBlocks )
  : it( whichRecord ), MemoryTrace::iterator( whichTrace ), myBlocks( whichBlocks )
{}

void VectorTrace::iterator::operator++()
{
  ++it;
}

void VectorTrace::iterator::operator--()
{
  --it;
}

MemoryTrace::iterator& VectorTrace::iterator::operator=( const MemoryTrace::iterator& copy )
{
  it = dynamic_cast<const VectorTrace::iterator&>( copy ).it;
  myBlocks = dynamic_cast<const VectorTrace::iterator&>( copy ).myBlocks;
  myTrace = dynamic_cast<const VectorTrace::iterator&>( copy ).myTrace;

  return *this;
}

bool VectorTrace::iterator::operator==( const MemoryTrace::iterator &whichit ) const
{
  return it == dynamic_cast<const VectorTrace::iterator&>( whichit ).it;
}

bool VectorTrace::iterator::operator!=( const MemoryTrace::iterator &whichit ) const
{
  return it != dynamic_cast<const VectorTrace::iterator&>( whichit ).it;
}

bool VectorTrace::iterator::isNull() const
{
  return it == myBlocks->threadRecords[ it->thread ].end();
}


VectorTrace::iterator *VectorTrace::iterator::clone() const
{
  return new VectorTrace::iterator( it, myTrace, myBlocks );
}

TRecordType VectorTrace::iterator::getType() const
{
  return it->type;
}

TRecordTime VectorTrace::iterator::getTime() const
{
  return it->time;
}

TThreadOrder VectorTrace::iterator::getThread() const
{
  return it->thread;
}

TCPUOrder VectorTrace::iterator::getCPU() const
{
  return it->CPU;
}

TObjectOrder VectorTrace::iterator::getOrder() const
{
  return it->thread;
}

TEventType VectorTrace::iterator::getEventType() const
{
  return it->URecordInfo.eventRecord.type;
}

TSemanticValue VectorTrace::iterator::getEventValue() const
{
  double tmpPrecision = myTrace->getEventTypePrecision( it->URecordInfo.eventRecord.type );
  if( tmpPrecision != 0.0 )
    return it->URecordInfo.eventRecord.value * tmpPrecision;
  return it->URecordInfo.eventRecord.value;

  return it->URecordInfo.eventRecord.value;
}

TEventValue VectorTrace::iterator::getEventValueAsIs() const
{
  return it->URecordInfo.eventRecord.value;
}

TState VectorTrace::iterator::getState() const
{
  return it->URecordInfo.stateRecord.state;
}

TRecordTime VectorTrace::iterator::getStateEndTime() const
{
  return it->URecordInfo.stateRecord.endTime;
}

TCommID VectorTrace::iterator::getCommIndex() const
{
  return it->URecordInfo.commRecord.index;
}

void VectorTrace::iterator::setTime( const TRecordTime whichTime )
{
  it->time = whichTime;
}

void VectorTrace::iterator::setType( const TRecordType whichType )
{
  it->type = whichType;
}

void VectorTrace::iterator::setStateEndTime( const TRecordTime whichEndTime )
{
  it->URecordInfo.stateRecord.endTime = whichEndTime;
}


void VectorTrace::insert( MemoryBlocks *blocks )
{
  myBlocks = dynamic_cast<VectorBlocks *>( blocks );
  blocks->resetCountInserted();
}

TTime VectorTrace::finish( TTime headerTime, Trace *whichTrace )
{
  myTrace = whichTrace;
  return headerTime;
}

MemoryTrace::iterator* VectorTrace::empty() const
{
  return nullptr;
}

MemoryTrace::iterator* VectorTrace::begin() const
{
  return nullptr;
}

MemoryTrace::iterator* VectorTrace::end() const
{
  return nullptr;
}

MemoryTrace::iterator* VectorTrace::threadBegin( TThreadOrder whichThread ) const
{
  return new VectorTrace::iterator( myBlocks->threadRecords[ whichThread ].begin(), myTrace, myBlocks);
}

MemoryTrace::iterator* VectorTrace::threadEnd( TThreadOrder whichThread ) const
{
  return new VectorTrace::iterator( --myBlocks->threadRecords[ whichThread ].end(), myTrace, myBlocks );
}

MemoryTrace::iterator* VectorTrace::CPUBegin( TCPUOrder whichCPU ) const
{
  return nullptr;
}

MemoryTrace::iterator* VectorTrace::CPUEnd( TCPUOrder whichCPU ) const
{
  return nullptr;
}

void VectorTrace::getRecordByTimeThread( std::vector<MemoryTrace::iterator *>& listIter, TRecordTime whichTime ) const
{
  for( auto& v : myBlocks->threadRecords )
  {
    auto it = std::find_if( v.begin(), v.end(), [whichTime]( const auto& e ) { return e.time == whichTime; } );
    if( it == v.end() ) --it;
    VectorTrace::iterator *retIt = new VectorTrace::iterator( it, myTrace, myBlocks );
    listIter.emplace_back( retIt );
  }
}

void VectorTrace::getRecordByTimeCPU( std::vector<MemoryTrace::iterator *>& listIter, TRecordTime whichTime ) const
{

}

