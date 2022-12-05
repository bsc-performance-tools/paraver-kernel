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

#include <vector>

#include "memorytrace.h"

class Trace;
class VectorBlocks;

class VectorTrace : public MemoryTrace
{
  public:
    class iterator : public MemoryTrace::iterator
    {
      public:
        iterator( std::vector<Plain::TRecord>::iterator whichRecord, const Trace *whichTrace, VectorBlocks *whichBlocks );
        virtual void operator++() override;
        virtual void operator--() override;
        virtual MemoryTrace::iterator& operator=( const MemoryTrace::iterator& copy ) override;

        virtual bool operator==( const MemoryTrace::iterator &it ) const override;
        virtual bool operator!=( const MemoryTrace::iterator &it ) const override;
        virtual bool isNull() const override;

        virtual iterator *clone() const override;

        virtual TRecordType    getType() const override;
        virtual TRecordTime    getTime() const override;
        virtual TThreadOrder   getThread() const override;
        virtual TCPUOrder      getCPU() const override;
        virtual TObjectOrder   getOrder() const override;
        virtual TEventType     getEventType() const override;
        virtual TSemanticValue getEventValue() const override;
        virtual TEventValue    getEventValueAsIs() const override;
        virtual TState         getState() const override;
        virtual TRecordTime    getStateEndTime() const override;
        virtual TCommID        getCommIndex() const override;

        virtual void setTime( const TRecordTime time ) override;
        virtual void setType( const TRecordType whichType ) override;
        virtual void setStateEndTime( const TRecordTime whichEndTime ) override;

      private:
        std::vector<Plain::TRecord>::iterator it;
        VectorBlocks *myBlocks;
        TThreadOrder myThread;
    };

    virtual void insert( MemoryBlocks *blocks ) override;
    virtual TTime finish( TTime headerTime, Trace *whichTrace ) override;

    virtual MemoryTrace::iterator* empty() const override;
    virtual MemoryTrace::iterator* begin() const override;
    virtual MemoryTrace::iterator* end() const override;

    virtual MemoryTrace::iterator* threadBegin( TThreadOrder whichThread ) const override;
    virtual MemoryTrace::iterator* threadEnd( TThreadOrder whichThread ) const override;
    virtual MemoryTrace::iterator* CPUBegin( TCPUOrder whichCPU ) const override;
    virtual MemoryTrace::iterator* CPUEnd( TCPUOrder whichCPU ) const override;

    virtual void getRecordByTimeThread( std::vector<MemoryTrace::iterator *>& listIter,
                                        TRecordTime whichTime ) const override;
    virtual void getRecordByTimeCPU( std::vector<MemoryTrace::iterator *>& listIter,
                                     TRecordTime whichTime ) const override;

  private:
    VectorBlocks *myBlocks;
    Trace *myTrace;
};
