#ifndef PLAINTRACE_H_INCLUDED
#define PLAINTRACE_H_INCLUDED

#include "memorytrace.h"
#include "index.h"

class ProcessModel;
class ResourceModel;

namespace Plain
{
  class PlainBlocks;

  class PlainTrace: public MemoryTrace
  {
    public:
    class iterator: public MemoryTrace::iterator
      {
        public:
          iterator()
          {}

          iterator( PlainBlocks *whichBlocks );

          virtual ~iterator()
          {}

          virtual void operator++();
          virtual void operator--();

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
          PlainBlocks *blocks;
      };

    class ThreadIterator : public PlainTrace::iterator
      {
        public:
          ThreadIterator()
          {}

          ThreadIterator( PlainBlocks *whichBlocks, UINT32 whichBlock, UINT32 whichPos,
                          TThreadOrder whichThread );

          virtual ~ThreadIterator()
          {}

          virtual TThreadOrder getThread() const;
          virtual TObjectOrder getOrder() const;

          virtual void operator++();
          virtual void operator--();

        private:
          TThreadOrder thread;
          UINT32 block;
          UINT32 pos;
          UINT32 lastBlock;
          UINT32 lastPos;

      };

    class CPUIterator : public PlainTrace::iterator
      {
        public:
          CPUIterator()
          {}

          CPUIterator( PlainBlocks *whichBlocks, vector<UINT32>& whichBlock, vector<UINT32>& whichPos,
                       TThreadOrder whichNumThreads, vector<TThreadOrder>& whichThreads, TCPUOrder whichCPU );

          virtual ~CPUIterator();

          virtual TThreadOrder getThread() const;
          virtual TObjectOrder getOrder() const;

          virtual void operator++();
          virtual void operator--();

        private:
          TCPUOrder cpu;
          TThreadOrder numThreads;
          vector<TThreadOrder> threads;
          vector<UINT32> block;
          vector<UINT32> pos;
          vector<UINT32> lastBlock;
          vector<UINT32> lastPos;
          TThreadOrder lastThread;

          TThreadOrder minThread();
          TThreadOrder maxThread();
          void setToMyCPUForward();
          void setToMyCPUBackward();
      };

      PlainTrace( const ProcessModel& whichProcessModel,
                  const ResourceModel& whichResourceModel );
      virtual ~PlainTrace()
      {}

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

      MemoryTrace::iterator *copyIterator( MemoryTrace::iterator *it );
      MemoryTrace::iterator *copyThreadIterator( MemoryTrace::iterator *it );
      MemoryTrace::iterator *copyCPUIterator( MemoryTrace::iterator *it );

    protected:

    private:
      const ProcessModel& processModel;
      const ResourceModel& resourceModel;
      TThreadOrder numThreads;
      TCPUOrder numCPUs;
      vector<Index<pair<UINT32, UINT32> > > traceIndex;
      PlainBlocks *myBlocks;
  };
}

#endif // PLAINTRACE_H_INCLUDED
