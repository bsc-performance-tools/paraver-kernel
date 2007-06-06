#ifndef TRACE_H_INCLUDED
#define TRACE_H_INCLUDED

#include <string>
#include <vector>
#include "paraverkerneltypes.h"
#include "processmodel.h"
#include "resourcemodel.h"
#include "memorytrace.h"

class Trace
{
  public:
    Trace()
    {
      ready = false;
    }

    Trace( const string& whichFile );

    ~Trace()
    {};

    TApplOrder totalApplications() const;

    TTaskOrder totalTasks() const;
    TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                              const TTaskOrder& inTask ) const;

    TThreadOrder totalThreads() const;
    TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                  const TTaskOrder& inTask,
                                  const TThreadOrder& inThread ) const;

    TNodeOrder totalNodes() const;

    TCPUOrder totalCPUs() const;
    TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                            const TCPUOrder& inCPU ) const;

    TTime getEndTime() const;
    TTimeUnit getTimeUnit() const;

  protected:
    bool ready;
    ProcessModel traceProcessModel;
    ResourceModel traceResourceModel;
    TTime traceEndTime;
    TTimeUnit traceTimeUnit;

  private:
    string fileName;
    string date;
    vector<string> communicators;
};


#endif // TRACE_H_INCLUDED
