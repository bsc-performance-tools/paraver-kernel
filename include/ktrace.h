#ifndef KTRACE_H_INCLUDED
#define KTRACE_H_INCLUDED

#include <string>
#include <vector>
#include <set>
#include "paraverkerneltypes.h"
#include "trace.h"
#include "processmodel.h"
#include "resourcemodel.h"
#include "memorytrace.h"
#include "bplustree.h"
#include "bplustreeblocks.h"

using namespace std;

class KTrace: public Trace
{
  public:
    KTrace()
    {
      ready = false;
    }

    KTrace( const string& whichFile, ProgressController *progress );

    ~KTrace()
    {
      delete blocks;
      delete memTrace;
    }

    string getFileName() const;
    string getTraceName() const;

    MemoryTrace::iterator *copyIterator( MemoryTrace::iterator *it )
    {
      return memTrace->copyIterator( it );
    }
    MemoryTrace::iterator *copyThreadIterator( MemoryTrace::iterator *it )
    {
      return memTrace->copyThreadIterator( it );
    }
    MemoryTrace::iterator *copyCPUIterator( MemoryTrace::iterator *it )
    {
      return memTrace->copyCPUIterator( it );
    }

    void dumpFile( const string& whichFile ) const;

    TApplOrder totalApplications() const;
    TTaskOrder totalTasks() const;
    TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                              const TTaskOrder& inTask ) const;
    void getTaskLocation( TTaskOrder globalTask,
                          TApplOrder& inAppl,
                          TTaskOrder& inTask ) const;
    TTaskOrder getFirstTask( TApplOrder inAppl ) const;
    TTaskOrder getLastTask( TApplOrder inAppl ) const;

    TThreadOrder totalThreads() const;
    TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                  const TTaskOrder& inTask,
                                  const TThreadOrder& inThread ) const;
    void getThreadLocation( TThreadOrder globalThread,
                            TApplOrder& inAppl,
                            TTaskOrder& inTask,
                            TThreadOrder& inThread ) const;
    TThreadOrder getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const;
    TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask )const;

    bool existResourceInfo() const;
    TNodeOrder totalNodes() const;
    TCPUOrder totalCPUs() const;
    TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                            const TCPUOrder& inCPU ) const;
    void getCPULocation( TCPUOrder globalCPU,
                         TNodeOrder& inNode,
                         TCPUOrder& inCPU ) const;
    TCPUOrder getFirstCPU( TNodeOrder inNode ) const;
    TCPUOrder getLastCPU( TNodeOrder inNode ) const;

    TObjectOrder getFirst( TObjectOrder globalOrder,
                           TWindowLevel fromLevel,
                           TWindowLevel toLevel ) const;
    TObjectOrder getLast( TObjectOrder globalOrder,
                          TWindowLevel fromLevel,
                          TWindowLevel toLevel ) const;


    // Communication info getters
    TCommID getTotalComms() const;
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

    inline TTime getEndTime() const
    {
      return traceEndTime;
    }

    inline TTimeUnit getTimeUnit() const
    {
      return traceTimeUnit;
    }

    bool eventLoaded( TEventType whichType ) const;

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

    const set<TEventType>& getLoadedEvents() const;

    bool findLastEventValue( TThreadOrder whichThread,
                             TRecordTime whichTime,
                             const vector<TEventType>& whichEvent,
                             TEventType& returnType,
                             TEventValue& returnValue ) const;

    virtual bool getFillStateGaps() const;
    virtual void setFillStateGaps( bool fill );

  protected:
    bool ready;
    ProcessModel traceProcessModel;
    ResourceModel traceResourceModel;
    TTime traceEndTime;
    TTimeUnit traceTimeUnit;
    MemoryBlocks *blocks;
    MemoryTrace *memTrace;

  private:
    string fileName;
    string date;
    vector<string> communicators;
    set<TEventType> events;
    bool fillStateGaps;
};

#endif // KTRACE_H_INCLUDED
