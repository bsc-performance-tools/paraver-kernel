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

#pragma once

#include "memorytrace.h"

class VectorTrace : public MemoryTrace
{
  public:
    class iterator
    {
      public:
        virtual void operator++();
        virtual void operator--();

        virtual iterator *clone() const;

        virtual TRecordType    getType() const;
        virtual TRecordTime    getTime() const;
        virtual TThreadOrder   getThread() const;
        virtual TCPUOrder      getCPU() const;
        virtual TObjectOrder   getOrder() const;
        virtual TEventType     getEventType() const;
        virtual TSemanticValue getEventValue() const;
        virtual TEventValue    getEventValueAsIs() const;
        virtual TState         getState() const;
        virtual TRecordTime    getStateEndTime() const;
        virtual TCommID        getCommIndex() const;

        virtual void         setTime( const TRecordTime time );
        virtual void         setType( const TRecordType whichType );
        virtual void         setStateEndTime( const TRecordTime whichEndTime );

    };

    virtual void insert( MemoryBlocks *blocks );
    virtual TTime finish( TTime headerTime, Trace *whichTrace );

    virtual MemoryTrace::iterator* empty() const;
    virtual MemoryTrace::iterator* begin() const;
    virtual MemoryTrace::iterator* end() const;

    virtual MemoryTrace::iterator* threadBegin( TThreadOrder whichThread ) const;
    virtual MemoryTrace::iterator* threadEnd( TThreadOrder whichThread ) const;
    virtual MemoryTrace::iterator* CPUBegin( TCPUOrder whichCPU ) const;
    virtual MemoryTrace::iterator* CPUEnd( TCPUOrder whichCPU ) const;

    virtual void getRecordByTimeThread( std::vector<MemoryTrace::iterator *>& listIter,
                                        TRecordTime whichTime ) const;
    virtual void getRecordByTimeCPU( std::vector<MemoryTrace::iterator *>& listIter,
                                     TRecordTime whichTime ) const;

};
