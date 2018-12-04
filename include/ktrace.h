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

#ifndef KTRACE_H_INCLUDED
#define KTRACE_H_INCLUDED

#include <string>
#include <vector>
#include <set>
#include "paraverkerneltypes.h"
#include "trace.h"
#include "memorytrace.h"
#include "processmodel.h"
#include "resourcemodel.h"

#include "ParaverMetadataManager.h"

class TraceBodyIO;

/*
class TraceInfo
{
  public:
    PRV_UINT64 cutterOffset;
    PRV_UINT64 cutterBeginTime;
    PRV_UINT64 cutterEndTime;

};
*/

class KTrace: public Trace
{
  public:
    KTrace()
    {
      ready = false;
    }

    KTrace( const std::string& whichFile, ProgressController *progress, bool noLoad );

    ~KTrace();

    std::string getFileName() const;
    std::string getTraceName() const;

    void dumpFileHeader( std::fstream& file, bool newFormat = false, PRV_INT32 numIter = 1 ) const;
    void dumpFile( const std::string& whichFile, PRV_INT32 numIter = 1 ) const;

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
    TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask ) const;
    void getThreadsPerNode( TNodeOrder inNode, std::vector<TThreadOrder>& onVector ) const;

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

    bool isSameObjectStruct( Trace *compareTo ) const;

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

    inline void setEndTime( TTime whichTraceEndTime )
    {
      traceEndTime = whichTraceEndTime;
    }

    inline TTimeUnit getTimeUnit() const
    {
      return traceTimeUnit;
    }

    inline const ptime& getTraceTime() const
    {
      return myTraceTime;
    }

    bool eventLoaded( TEventType whichType ) const;
    bool anyEventLoaded( TEventType firstType, TEventType lastType ) const;

    // Forward MemoryTrace iterator functions
    MemoryTrace::iterator* empty() const; // Dummy iterator set to EMPTYREC
    MemoryTrace::iterator* begin() const;
    MemoryTrace::iterator* end() const;
    MemoryTrace::iterator* threadBegin( TThreadOrder whichThread ) const;
    MemoryTrace::iterator* threadEnd( TThreadOrder whichThread ) const;
    MemoryTrace::iterator* CPUBegin( TCPUOrder whichCPU ) const;
    MemoryTrace::iterator* CPUEnd( TCPUOrder whichCPU ) const;

    void getRecordByTimeThread( std::vector<MemoryTrace::iterator *>& listIter,
                                TRecordTime whichTime ) const;
    void getRecordByTimeCPU( std::vector<MemoryTrace::iterator *>& listIter,
                             TRecordTime whichTime ) const;

    const std::set<TEventType>& getLoadedEvents() const;

    bool findLastEventValue( TThreadOrder whichThread,
                             TRecordTime whichTime,
                             const std::vector<TEventType>& whichEvent,
                             TEventType& returnType,
                             TEventValue& returnValue ) const;

    bool findNextEvent( TThreadOrder whichThread,
                        TRecordTime whichTime,
                        TEventType whichEvent,
                        TRecordTime& foundTime ) const;

    virtual bool getFillStateGaps() const;
    virtual void setFillStateGaps( bool fill );

    virtual PRV_UINT64 getCutterOffset();
    virtual PRV_UINT64 getCutterBeginTime();
    virtual PRV_UINT64 getCutterEndTime();

    virtual void setLogicalSend( TCommID whichComm, TRecordTime whichTime );
    virtual void setLogicalReceive( TCommID whichComm, TRecordTime whichTime );
    virtual void setPhysicalSend( TCommID whichComm, TRecordTime whichTime );
    virtual void setPhysicalReceive( TCommID whichComm, TRecordTime whichTime );


  protected:
    bool ready;
    ProcessModel traceProcessModel;
    ResourceModel traceResourceModel;
    TTime traceEndTime;
    TTimeUnit traceTimeUnit;
    MemoryBlocks *blocks;
    MemoryTrace *memTrace;
    TraceBodyIO *body;
    ptime myTraceTime;

  private:
    std::string fileName;
    std::vector<std::string> communicators;
    std::set<TEventType> events;
    bool fillStateGaps;
    MetadataManager myTraceInfo;

    void parseDateTime( string &whichDateTime );

};

#endif // KTRACE_H_INCLUDED
