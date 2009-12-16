#ifndef NOLOADTRACE_H_INCLUDED
#define NOLOADTRACE_H_INCLUDED

#include "memorytrace.h"

namespace NoLoad
{
  class NoLoadTrace: public MemoryTrace
  {
    public:
    class iterator: public MemoryTrace::iterator
      {
        public:
          iterator();

          virtual ~iterator();

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

      };

      NoLoadTrace();

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

  };

}


#endif // NOLOADTRACE_H_INCLUDED
