#ifndef PLAINTRACE_H_INCLUDED
#define PLAINTRACE_H_INCLUDED

#include "memorytrace.h"
#include "index.h"

class PlainTrace: public MemoryTrace
{
  public:
    class iterator: public MemoryTrace::iterator
    {
      public:
        iterator()
        {}

        iterator( TRecord *whichRecord );

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

    };

    class ThreadIterator : public PlainTrace::iterator
    {
      public:
        ThreadIterator()
        {}

        // Constructor declaration needed for iterators.
        ThreadIterator( TRecord *whichRecord ) : PlainTrace::iterator( whichRecord )
        {}

        virtual ~ThreadIterator()
        {}

        virtual TObjectOrder getOrder() const;

        virtual void operator++();
        virtual void operator--();
    };

    class CPUIterator : public PlainTrace::iterator
    {
      public:
        CPUIterator()
        {}

        // Constructor declaration needed for iterators.
        CPUIterator( TRecord *whichRecord ): PlainTrace::iterator( whichRecord )
        {}

        virtual ~CPUIterator()
        {}

        virtual TObjectOrder getOrder() const;

        virtual void operator++();
        virtual void operator--();
    };

    PlainTrace()
    {}
    PlainTrace( const TThreadOrder totalThreads,
                const TCPUOrder totalCPUs );
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
    TThreadOrder numThreads;
    TCPUOrder numCPUs;
    Index traceIndex;
    vector<TRecord> emptyThreadBegin;
    vector<TRecord> emptyThreadEnd;
    vector<TRecord> emptyCPUBegin;
    vector<TRecord> emptyCPUEnd;
    TRecord first;
    TRecord last;
};

#endif // PLAINTRACE_H_INCLUDED
