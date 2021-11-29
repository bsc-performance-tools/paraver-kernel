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


#include "memorytrace.h"
#include "ktrace.h"
#include <iostream>

using namespace std;
MemoryTrace::iterator::iterator()
{}

MemoryTrace::iterator::iterator( const Trace *whichTrace ) : myTrace( whichTrace )
{}

bool MemoryTrace::iterator::operator==( const MemoryTrace::iterator &it ) const
{
  return ( this->record == it.record );
}

bool MemoryTrace::iterator::operator!=( const MemoryTrace::iterator &it ) const
{
  return ( this->record != it.record );
}

MemoryTrace::iterator& MemoryTrace::iterator::operator=( const MemoryTrace::iterator& copy )
{
  if( this != &copy )
  {
    record = copy.record;
    myTrace = copy.myTrace;
  }
  return *this;
}

bool MemoryTrace::iterator::isNull() const
{
  return ( record == nullptr );
}

TThreadOrder MemoryTrace::iterator::getSenderThread() const
{
  return myTrace->getSenderThread( getCommIndex() );
}

TCPUOrder MemoryTrace::iterator::getSenderCPU() const
{
  return myTrace->getSenderCPU( getCommIndex() );
}

TThreadOrder MemoryTrace::iterator::getReceiverThread() const
{
  return myTrace->getReceiverThread( getCommIndex() );
}

TCPUOrder MemoryTrace::iterator::getReceiverCPU() const
{
  return myTrace->getReceiverCPU( getCommIndex() );
}

TCommTag MemoryTrace::iterator::getCommTag() const
{
  return myTrace->getCommTag( getCommIndex() );
}

TCommSize MemoryTrace::iterator::getCommSize() const
{
  return myTrace->getCommSize( getCommIndex() );
}

TRecordTime MemoryTrace::iterator::getLogicalSend() const
{
  return myTrace->getLogicalSend( getCommIndex() );
}

TRecordTime MemoryTrace::iterator::getLogicalReceive() const
{
  return myTrace->getLogicalReceive( getCommIndex() );
}

TRecordTime MemoryTrace::iterator::getPhysicalSend() const
{
  return myTrace->getPhysicalSend( getCommIndex() );
}

TRecordTime MemoryTrace::iterator::getPhysicalReceive() const
{
  return myTrace->getPhysicalReceive( getCommIndex() );
}
