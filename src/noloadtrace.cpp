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

#include "noloadtrace.h"

using namespace NoLoad;

NoLoadTrace::NoLoadTrace()
{}

NoLoadTrace::~NoLoadTrace()
{}

void NoLoadTrace::insert( MemoryBlocks *blocks )
{}

TTime NoLoadTrace::finish( TTime headerTime )
{
  return 0;
}

MemoryTrace::iterator* NoLoadTrace::begin() const
{
  return NULL;
}

MemoryTrace::iterator* NoLoadTrace::end() const
{
  return NULL;
}

MemoryTrace::iterator* NoLoadTrace::threadBegin( TThreadOrder whichThread ) const
{
  return NULL;
}

MemoryTrace::iterator* NoLoadTrace::threadEnd( TThreadOrder whichThread ) const
{
  return NULL;
}

MemoryTrace::iterator* NoLoadTrace::CPUBegin( TCPUOrder whichCPU ) const
{
  return NULL;
}

MemoryTrace::iterator* NoLoadTrace::CPUEnd( TCPUOrder whichCPU ) const
{
  return NULL;
}

void NoLoadTrace::getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
    TRecordTime whichTime ) const
{}

void NoLoadTrace::getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                      TRecordTime whichTime ) const
{}


MemoryTrace::iterator *NoLoadTrace::copyIterator( MemoryTrace::iterator *it )
{
  return NULL;
}

MemoryTrace::iterator *NoLoadTrace::copyThreadIterator( MemoryTrace::iterator *it )
{
  return NULL;
}

MemoryTrace::iterator *NoLoadTrace::copyCPUIterator( MemoryTrace::iterator *it )
{
  return NULL;
}

NoLoadTrace::iterator::iterator()
{}

NoLoadTrace::iterator::~iterator()
{}

void NoLoadTrace::iterator::operator++()
{}

void NoLoadTrace::iterator::operator--()
{}

TRecordType NoLoadTrace::iterator::getType() const
{
  return 0;
}

TRecordTime NoLoadTrace::iterator::getTime() const
{
  return 0;
}

TThreadOrder NoLoadTrace::iterator::getThread() const
{
  return 0;
}

TCPUOrder NoLoadTrace::iterator::getCPU() const
{
  return 0;
}

TObjectOrder NoLoadTrace::iterator::getOrder() const
{
  return 0;
}

TEventType NoLoadTrace::iterator::getEventType() const
{
  return 0;
}

TEventValue NoLoadTrace::iterator::getEventValue() const
{
  return 0;
}

TState NoLoadTrace::iterator::getState() const
{
  return 0;
}

TRecordTime NoLoadTrace::iterator::getStateEndTime() const
{
  return 0;
}

TCommID NoLoadTrace::iterator::getCommIndex() const
{
  return 0;
}
