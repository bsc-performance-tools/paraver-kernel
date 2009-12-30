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
          iterator()
          {}

          iterator( NoLoadBlocks *whichBlocks );

          virtual ~iterator();

          virtual void operator++();
          virtual void operator--();
          virtual MemoryTrace::iterator& operator=( const MemoryTrace::iterator& copy )
          {
            return *this;
          }

          virtual TRecordType  getType() const;
          virtual TRecordTime  getTime() const;
          virtual TThreadOrder getThread() const;
          virtual TCPUOrder    getCPU() const;
          virtual TObjectOrder getOrder() const;
          virtual TEventType   getEventType() const;
          virtual TEventValue  getEventValue() const;
          virtual TState       getState() const;
          virtual TRecordTime  getStateEndTime() const;
          virtual TCommID      getCommIndex() const;

        protected:
          NoLoadBlocks *blocks;

      };

    class ThreadIterator : public NoLoadTrace::iterator
      {
        public:
          ThreadIterator()
          {}

          ThreadIterator( NoLoadBlocks *whichBlocks, TThreadOrder whichThread,
                          TRecord *whichRecord, INT64 whichOffset, INT16 whichPos );

          virtual ~ThreadIterator();

          virtual TThreadOrder getThread() const;
          virtual TObjectOrder getOrder() const;

          virtual void operator++();
          virtual void operator--();
          virtual MemoryTrace::iterator& operator=( const MemoryTrace::iterator& copy );

        private:
          TThreadOrder thread;
          INT64 offset;
          UINT16 recPos;

          friend class NoLoadTrace;
      };

    class CPUIterator : public NoLoadTrace::iterator
      {
        public:
          CPUIterator()
          {}

          CPUIterator( NoLoadBlocks *whichBlocks, TCPUOrder whichCPU,
                       vector<TThreadOrder>& whichThreads, vector<TRecord *>& whichRecords,
                       vector<INT64>& whichOffsets, vector<UINT16>& whichPos, bool notMove = false );

          virtual ~CPUIterator();

          virtual TThreadOrder getThread() const;
          virtual TObjectOrder getOrder() const;

          virtual void operator++();
          virtual void operator--();
          virtual MemoryTrace::iterator& operator=( const MemoryTrace::iterator& copy );

        private:
          TCPUOrder cpu;
          vector<TThreadOrder> threads;
          vector<TRecord *> threadRecords;
          vector<INT64> offset;
          vector<UINT16> recPos;
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

      virtual void insert( MemoryBlocks *blocks );
      virtual TTime finish( TTime headerTime );
      virtual MemoryTrace::iterator* begin() const;
      virtual MemoryTrace::iterator* end() const;

      virtual MemoryTrace::iterator* threadBegin( TThreadOrder whichThread ) const;
      virtual MemoryTrace::iterator* threadEnd( TThreadOrder whichThread ) const;
      virtual MemoryTrace::iterator* CPUBegin( TCPUOrder whichCPU ) const;
      virtual MemoryTrace::iterator* CPUEnd( TCPUOrder whichCPU ) const;


      virtual void getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                          TRecordTime whichTime ) const;
      virtual void getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                       TRecordTime whichTime ) const;

      virtual MemoryTrace::iterator *copyIterator( MemoryTrace::iterator *it );
      virtual MemoryTrace::iterator *copyThreadIterator( MemoryTrace::iterator *it );
      virtual MemoryTrace::iterator *copyCPUIterator( MemoryTrace::iterator *it );

    protected:

    private:
      const ProcessModel& processModel;
      const ResourceModel& resourceModel;
      NoLoadBlocks *blocks;
  };

}


#endif // NOLOADTRACE_H_INCLUDED
