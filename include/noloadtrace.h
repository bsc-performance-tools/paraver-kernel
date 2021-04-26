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


#ifndef NOLOADTRACE_H_INCLUDED
#define NOLOADTRACE_H_INCLUDED

#include "memorytrace.h"
#include "plaintypes.h"

using Plain::TRecord;

class ProcessModel;
class ResourceModel;

namespace NoLoad
{
  class NoLoadBlocks;

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

  class NoLoadTrace: public MemoryTrace
  {
    public:
    class iterator: public MemoryTrace::iterator
      {
        public:
          iterator() :
            destroyed( false )
          {}

          iterator( NoLoadBlocks *whichBlocks );

          iterator( NoLoadBlocks *whichBlocks, TThreadOrder whichThread,
                    TRecord *whichRecord, PRV_INT64 whichOffset, PRV_INT16 whichPos );

          virtual ~iterator();

          virtual void operator++() override;
          virtual void operator--() override;
          virtual MemoryTrace::iterator& operator=( const MemoryTrace::iterator& copy ) override;

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

          virtual void           setTime( const TRecordTime whichTime ) override;
          virtual void           setType( const TRecordType whichType ) override;
          virtual void           setStateEndTime( const TRecordTime whichEndTime ) override;

        protected:
          NoLoadBlocks *blocks;

          TThreadOrder thread;
          PRV_INT64 offset;
          PRV_UINT16 recPos;

          bool destroyed;

        private:
          friend class NoLoadTrace;

      };

    class ThreadIterator : public NoLoadTrace::iterator
      {
        public:
          ThreadIterator()
          {}

          ThreadIterator( NoLoadBlocks *whichBlocks, TThreadOrder whichThread,
                          TRecord *whichRecord, PRV_INT64 whichOffset, PRV_INT16 whichPos );

          virtual ~ThreadIterator();

          virtual TThreadOrder getThread() const override;
          virtual TObjectOrder getOrder() const override;

          virtual void operator++() override;
          virtual void operator--() override;
          virtual MemoryTrace::iterator& operator=( const MemoryTrace::iterator& copy ) override;

          virtual ThreadIterator *clone() const override;

        private:
          friend class NoLoadTrace;
      };

    class CPUIterator : public NoLoadTrace::iterator
      {
        public:
          CPUIterator()
          {}

          CPUIterator( NoLoadBlocks *whichBlocks, TCPUOrder whichCPU,
                       std::vector<TThreadOrder>& whichThreads, std::vector<TRecord *>& whichRecords,
                       std::vector<PRV_INT64>& whichOffsets, std::vector<PRV_UINT16>& whichPos, bool notMove = false );

          virtual ~CPUIterator();

          virtual TThreadOrder getThread() const override;
          virtual TObjectOrder getOrder() const override;

          virtual void operator++() override;
          virtual void operator--() override;
          virtual MemoryTrace::iterator& operator=( const MemoryTrace::iterator& copy ) override;

          virtual CPUIterator *clone() const override;

        private:
          TCPUOrder cpu;
          std::vector<TThreadOrder> threads;
          std::vector<TRecord *> threadRecords;
          std::vector<PRV_INT64> offset;
          std::vector<PRV_UINT16> recPos;
          TThreadOrder lastThread;

          TThreadOrder minThread();
          TThreadOrder maxThread();
          void setToMyCPUForward();
          void setToMyCPUBackward();

          friend class NoLoadTrace;
      };

      NoLoadTrace( MemoryBlocks *whichBlocks,
                   const ProcessModel& whichProcessModel,
                   const ResourceModel& whichResourceModel );

      virtual ~NoLoadTrace();

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

    protected:

    private:
      const ProcessModel& processModel;
      const ResourceModel& resourceModel;
      NoLoadBlocks *blocks;
  };

}


#endif // NOLOADTRACE_H_INCLUDED
