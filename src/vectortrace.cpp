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

#include "vectortrace.h"

void VectorTrace::iterator::operator++()
{

}

void VectorTrace::iterator::operator--()
{

}

VectorTrace::iterator *VectorTrace::iterator::clone() const
{

}

TRecordType VectorTrace::iterator::getType() const
{

}

TRecordTime VectorTrace::iterator::getTime() const
{

}

TThreadOrder VectorTrace::iterator::getThread() const
{

}

TCPUOrder VectorTrace::iterator::getCPU() const
{

}

TObjectOrder VectorTrace::iterator::getOrder() const
{

}

TEventType VectorTrace::iterator::getEventType() const
{

}

TSemanticValue VectorTrace::iterator::getEventValue() const
{

}

TEventValue VectorTrace::iterator::getEventValueAsIs() const
{

}

TState VectorTrace::iterator::getState() const
{

}

TRecordTime VectorTrace::iterator::getStateEndTime() const
{

}

TCommID VectorTrace::iterator::getCommIndex() const
{

}

void VectorTrace::iterator::setTime( const TRecordTime time )
{

}

void VectorTrace::iterator::setType( const TRecordType whichType )
{

}

void VectorTrace::iterator::setStateEndTime( const TRecordTime whichEndTime )
{

}


void VectorTrace::insert( MemoryBlocks *blocks )
{

}

TTime VectorTrace::finish( TTime headerTime, Trace *whichTrace )
{

}

MemoryTrace::iterator* VectorTrace::empty() const
{

}

MemoryTrace::iterator* VectorTrace::begin() const
{

}

MemoryTrace::iterator* VectorTrace::end() const
{

}

MemoryTrace::iterator* VectorTrace::threadBegin( TThreadOrder whichThread ) const
{

}

MemoryTrace::iterator* VectorTrace::threadEnd( TThreadOrder whichThread ) const
{

}

MemoryTrace::iterator* VectorTrace::CPUBegin( TCPUOrder whichCPU ) const
{

}

MemoryTrace::iterator* VectorTrace::CPUEnd( TCPUOrder whichCPU ) const
{

}

void VectorTrace::getRecordByTimeThread( std::vector<MemoryTrace::iterator *>& listIter, TRecordTime whichTime ) const
{

}

void VectorTrace::getRecordByTimeCPU( std::vector<MemoryTrace::iterator *>& listIter, TRecordTime whichTime ) const
{

}

