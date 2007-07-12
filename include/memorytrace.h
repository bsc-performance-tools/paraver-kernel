#ifndef MEMORYTRACE_H_INCLUDED
#define MEMORYTRACE_H_INCLUDED

#include <vector>
#include "paraverkerneltypes.h"
#include "memoryblocks.h"

using namespace std;

class MemoryTrace
{
  public:
    class iterator
    {
      public:
        iterator();
        virtual ~iterator() = 0;

        virtual void operator++() = 0;
        virtual void operator--() = 0;
        virtual void nextByThread() = 0;
        virtual void prevByThread() = 0;
        virtual void nextByCPU() = 0;
        virtual void prevByCPU() = 0;

        virtual TRecordType  getType() const = 0;
        virtual TRecordTime  getTime() const = 0;
        virtual TThreadOrder getThread() const = 0;
        virtual TCPUOrder    getCPU() const = 0;
        virtual TEventType   getEventType() const = 0;
        virtual TEventValue  getEventValue() const = 0;
        virtual TState       getState() const = 0;
        virtual TRecordTime  getStateEndTime() const = 0;
        virtual TCommID      getCommIndex() const = 0;

      protected:

      private:

    };

    MemoryTrace();
    virtual ~MemoryTrace() = 0;

    virtual iterator& getFirstRecord() const = 0;
    virtual iterator& getLastRecord() const = 0;
    virtual iterator& getFirstRecord( TThreadOrder whichThread ) const = 0;
    virtual iterator& getLastRecord( TThreadOrder whichThread ) const = 0;
    virtual void getRecordByTime( vector<iterator>& listIter,
                                  TRecordTime whichTime ) const = 0;

  protected:
    MemoryBlocks& blocks;

  private:

};



#endif // MEMORYTRACE_H_INCLUDED
