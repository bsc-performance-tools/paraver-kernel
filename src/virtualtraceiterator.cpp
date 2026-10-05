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

#include "virtualtraceiterator.h"

VirtualTraceIterator::~VirtualTraceIterator()
{
  if( assignedContent != nullptr )
    delete assignedContent;
}

void VirtualTraceIterator::operator++()
{
}

void VirtualTraceIterator::operator--()
{
}

MemoryTrace::iterator &VirtualTraceIterator::operator=( const MemoryTrace::iterator &copy )
{
  virtualRecord.time = copy.getTime();

  if( assignedContent != nullptr )
    delete assignedContent;
  assignedContent = copy.clone();

  return *this;
}

bool VirtualTraceIterator::operator==( const MemoryTrace::iterator &it ) const
{
  if( assignedContent != nullptr && *assignedContent == it )
    return true;

  return false;
}

bool VirtualTraceIterator::operator!=( const MemoryTrace::iterator &it ) const
{
  if( assignedContent != nullptr && *assignedContent != it )
    return true;

  return true;
}

bool VirtualTraceIterator::isNull() const
{
  return false;
}

MemoryTrace::iterator *VirtualTraceIterator::clone() const
{
  VirtualTraceIterator *retIt = new VirtualTraceIterator();
  retIt->virtualRecord        = virtualRecord;

  return retIt;
}

TRecordType VirtualTraceIterator::getRecordType() const
{
  return virtualRecord.type;
}

TRecordTime VirtualTraceIterator::getTime() const
{
  return virtualRecord.time;
}

TThreadOrder VirtualTraceIterator::getThread() const
{
  return virtualRecord.thread;
}

TCPUOrder VirtualTraceIterator::getCPU() const
{
  return virtualRecord.CPU;
}

TObjectOrder VirtualTraceIterator::getOrder() const
{
  return virtualRecord.thread;
}

TEventType VirtualTraceIterator::getEventType() const
{
  return 0;
}

TSemanticValue VirtualTraceIterator::getEventValue() const
{
  return 0;
}

TEventValue VirtualTraceIterator::getEventValueAsIs() const
{
  return 0;
}

TState VirtualTraceIterator::getState() const
{
  return virtualRecord.URecordInfo.stateRecord.state;
}

TRecordTime VirtualTraceIterator::getStateEndTime() const
{
  return virtualRecord.URecordInfo.stateRecord.endTime;
}

TCommID VirtualTraceIterator::getCommIndex() const
{
  return 0;
}

TThreadOrder VirtualTraceIterator::getSenderThread() const
{
  return 0;
}

TCPUOrder VirtualTraceIterator::getSenderCPU() const
{
  return 0;
}

TThreadOrder VirtualTraceIterator::getReceiverThread() const
{
  return 0;
}

TCPUOrder VirtualTraceIterator::getReceiverCPU() const
{
  return 0;
}

TCommTag VirtualTraceIterator::getCommTag() const
{
  return 0;
}

TCommSize VirtualTraceIterator::getCommSize() const
{
  return 0;
}

TRecordTime VirtualTraceIterator::getLogicalSend() const
{
  return 0;
}

TRecordTime VirtualTraceIterator::getLogicalReceive() const
{
  return 0;
}

TRecordTime VirtualTraceIterator::getPhysicalSend() const
{
  return 0;
}

TRecordTime VirtualTraceIterator::getPhysicalReceive() const
{
  return 0;
}

void VirtualTraceIterator::setTime( const TRecordTime time )
{
  virtualRecord.time = time;
}

void VirtualTraceIterator::setRecordType( const TRecordType whichType )
{
  virtualRecord.type = whichType;
}

void VirtualTraceIterator::setStateEndTime( const TRecordTime whichEndTime )
{
  virtualRecord.URecordInfo.stateRecord.endTime = whichEndTime;
}

void VirtualTraceIterator::setRecord( TData *whichRecord )
{
  virtualRecord = *static_cast< Plain::TRecord * >( whichRecord );
  record        = &virtualRecord;
}
