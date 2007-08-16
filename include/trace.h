#ifndef TRACE_H_INCLUDED
#define TRACE_H_INCLUDED

#include <string>
#include <vector>
#include "paraverkerneltypes.h"
#include "processmodel.h"
#include "resourcemodel.h"
#include "memorytrace.h"
#include "bplustree.h"
#include "bplustreeblocks.h"

using namespace bplustree;

class Trace
{
  public:
    Trace()
    {
      ready = false;
    }

    Trace( const string& whichFile );

    ~Trace()
    {
      delete blocks;
    }

    MemoryTrace::iterator *copyIterator( MemoryTrace::iterator *it )
    {
      return new BPlusTree::iterator( *( dynamic_cast<BPlusTree::iterator *> ( it ) ) );
    }
    MemoryTrace::iterator *copyThreadIterator( MemoryTrace::iterator *it )
    {
      return new BPlusTree::ThreadIterator( *( dynamic_cast<BPlusTree::ThreadIterator *> ( it ) ) );
    }
    MemoryTrace::iterator *copyCPUIterator( MemoryTrace::iterator *it )
    {
      return new BPlusTree::CPUIterator( *( dynamic_cast<BPlusTree::CPUIterator *> ( it ) ) );
    }

    void dumpFile( const string& whichFile ) const;

    TApplOrder totalApplications() const;
    TTaskOrder totalTasks() const;
    TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                              const TTaskOrder& inTask ) const;
    TThreadOrder totalThreads() const;
    TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                  const TTaskOrder& inTask,
                                  const TThreadOrder& inThread ) const;
    void getThreadLocation( TThreadOrder globalThread,
                            TApplOrder& inAppl,
                            TTaskOrder& inTask,
                            TThreadOrder& inThread ) const;

    bool existResourceInfo() const;
    TNodeOrder totalNodes() const;
    TCPUOrder totalCPUs() const;
    TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                            const TCPUOrder& inCPU ) const;


    // Communication info getters
    TThreadOrder getSenderThread( TCommID whichComm ) const;
    TCPUOrder getSenderCPU( TCommID whichComm ) const;
    TThreadOrder getReceiverThread( TCommID whichComm ) const;
    TCPUOrder getReceiverCPU( TCommID whichComm ) const;
    TCommTag getCommTag( TCommID whichComm ) const;
    TCommSize getCommSize( TCommID whichComm ) const;
    TRecordTime getLogicalSend( TCommID whichComm ) const;
    TRecordTime getLogicalReceive( TCommID whichComm ) const;
    TRecordTime getPhysicalSend( TCommID whichComm ) const;
    TRecordTime getPhysicalReceive( TCommID whichComm ) const;

    TTime getEndTime() const;
    TTimeUnit getTimeUnit() const;

    // Forward MemoryTrace iterator functions
    MemoryTrace::iterator* begin() const;
    MemoryTrace::iterator* end() const;
    MemoryTrace::iterator* threadBegin( TThreadOrder whichThread ) const;
    MemoryTrace::iterator* threadEnd( TThreadOrder whichThread ) const;
    MemoryTrace::iterator* CPUBegin( TCPUOrder whichCPU ) const;
    MemoryTrace::iterator* CPUEnd( TCPUOrder whichCPU ) const;

    void getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                TRecordTime whichTime ) const;
    void getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                             TRecordTime whichTime ) const;

  protected:
    bool ready;
    ProcessModel traceProcessModel;
    ResourceModel traceResourceModel;
    TTime traceEndTime;
    TTimeUnit traceTimeUnit;
    BPlusTreeBlocks *blocks;
    BPlusTree *btree;

  private:
    string fileName;
    string date;
    vector<string> communicators;
};


#endif // TRACE_H_INCLUDED
