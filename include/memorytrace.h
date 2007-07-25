#ifndef MEMORYTRACE_H_INCLUDED
#define MEMORYTRACE_H_INCLUDED

#include <vector>
#include "paraverkerneltypes.h"

using namespace std;

class MemoryTrace
{
  public:
    class iterator
    {
      public:
        iterator();
        //iterator( TRecord *whichRecord );

        virtual ~iterator() = 0;

        virtual void operator++() = 0;
        virtual void operator--() = 0;

        virtual bool operator==( const iterator &it );
        virtual bool operator!=( const iterator &it );
        virtual bool isNull() const = 0;

        virtual TRecordType  getType() const = 0;
        virtual TRecordTime  getTime() const = 0;
        virtual TThreadOrder getThread() const = 0;
        virtual TCPUOrder    getCPU() const = 0;
        virtual TEventType   getEventType() const = 0;
        virtual TEventValue  getEventValue() const = 0;
        virtual TState       getState() const = 0;
        virtual TRecordTime  getStateEndTime() const = 0;
        virtual TCommID      getCommIndex() const = 0;
    };

    class ThreadIterator : public iterator
    {
      public:
        ThreadIterator();
        //ThreadIterator( TRecord *whichRecord );

        virtual ~ThreadIterator();

        virtual void operator++();
        virtual void operator--();
    };

    class CPUIterator : public iterator
    {
      public:
        CPUIterator();
        //CPUIterator( TRecord *whichRecord );

        virtual ~CPUIterator();

        virtual void operator++();
        virtual void operator--();
    };


    MemoryTrace()
    {}
    virtual ~MemoryTrace()
    {}


    virtual MemoryTrace::iterator* begin() const = 0;
    virtual MemoryTrace::iterator* end() const = 0;
    virtual MemoryTrace::ThreadIterator* threadBegin( TThreadOrder whichThread ) const = 0;
    virtual MemoryTrace::ThreadIterator* threadEnd( TThreadOrder whichThread ) const = 0;
    virtual MemoryTrace::CPUIterator* CPUBegin( TCPUOrder whichCPU ) const = 0;
    virtual MemoryTrace::CPUIterator* CPUEnd( TCPUOrder whichCPU ) const = 0;

    virtual void getRecordByTimeThread( vector<MemoryTrace::ThreadIterator *>& listIter,
                                        TRecordTime whichTime ) const = 0;
    virtual void getRecordByTimeCPU( vector<MemoryTrace::CPUIterator *>& listIter,
                                     TRecordTime whichTime ) const = 0;

  protected:

  private:

};



#endif // MEMORYTRACE_H_INCLUDED
