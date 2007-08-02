#ifndef MEMORYTRACE_H_INCLUDED
#define MEMORYTRACE_H_INCLUDED

#include <vector>
#include "paraverkerneltypes.h"

using namespace std;

typedef struct {} TData;

class MemoryTrace
{
  public:
    class iterator
    {
      public:
        iterator();

        virtual ~iterator();

        virtual void operator++() = 0;
        virtual void operator--() = 0;

        virtual bool operator==( const iterator &it ) const;
        virtual bool operator!=( const iterator &it ) const;
        virtual bool isNull() const;

        virtual TRecordType  getType() const = 0;
        virtual TRecordTime  getTime() const = 0;
        virtual TThreadOrder getThread() const = 0;
        virtual TCPUOrder    getCPU() const = 0;
        virtual TEventType   getEventType() const = 0;
        virtual TEventValue  getEventValue() const = 0;
        virtual TState       getState() const = 0;
        virtual TRecordTime  getStateEndTime() const = 0;
        virtual TCommID      getCommIndex() const = 0;

      protected :
        TData *record;
    };
/*
    class ThreadIterator : virtual public iterator
    {
      public:
        ThreadIterator();

        virtual ~ThreadIterator();

        virtual void operator++();
        virtual void operator--();
    };

    class CPUIterator : virtual public iterator
    {
      public:
        CPUIterator();

        virtual ~CPUIterator();

        virtual void operator++();
        virtual void operator--();
    };
*/

    MemoryTrace()
    {}
    virtual ~MemoryTrace()
    {}


    virtual MemoryTrace::iterator* begin() const = 0;
    virtual MemoryTrace::iterator* end() const = 0;

    virtual MemoryTrace::iterator* threadBegin( TThreadOrder whichThread ) const = 0;
    virtual MemoryTrace::iterator* threadEnd( TThreadOrder whichThread ) const = 0;
    virtual MemoryTrace::iterator* CPUBegin( TCPUOrder whichCPU ) const = 0;
    virtual MemoryTrace::iterator* CPUEnd( TCPUOrder whichCPU ) const = 0;


    virtual void getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                        TRecordTime whichTime ) const = 0;
    virtual void getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                     TRecordTime whichTime ) const = 0;

  protected:

  private:

};



#endif // MEMORYTRACE_H_INCLUDED
