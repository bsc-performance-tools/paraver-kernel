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

#pragma once


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

    std::string getFileName() const override;
    std::string getTraceName() const override;

    void dumpFileHeader( std::fstream& file, bool newFormat = false, PRV_INT32 numIter = 1 ) const override;
    void dumpFile( const std::string& whichFile, PRV_INT32 numIter = 1 ) const override;

    TApplOrder totalApplications() const override;
    TTaskOrder totalTasks() const override;
    TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                              const TTaskOrder& inTask ) const override;
    void getTaskLocation( TTaskOrder globalTask,
                          TApplOrder& inAppl,
                          TTaskOrder& inTask ) const override;
    TTaskOrder getFirstTask( TApplOrder inAppl ) const override;
    TTaskOrder getLastTask( TApplOrder inAppl ) const override;

    TThreadOrder totalThreads() const override;
    TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                  const TTaskOrder& inTask,
                                  const TThreadOrder& inThread ) const override;
    void getThreadLocation( TThreadOrder globalThread,
                            TApplOrder& inAppl,
                            TTaskOrder& inTask,
                            TThreadOrder& inThread ) const override;
    TThreadOrder getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const override;
    TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask ) const override;
    void getThreadsPerNode( TNodeOrder inNode, std::vector<TThreadOrder>& onVector ) const override;

    bool existResourceInfo() const override;
    TNodeOrder totalNodes() const override;
    TCPUOrder totalCPUs() const override;
    TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                            const TCPUOrder& inCPU ) const override;
    void getCPULocation( TCPUOrder globalCPU,
                         TNodeOrder& inNode,
                         TCPUOrder& inCPU ) const override;
    TCPUOrder getFirstCPU( TNodeOrder inNode ) const override;
    TCPUOrder getLastCPU( TNodeOrder inNode ) const override;

    TObjectOrder getFirst( TObjectOrder globalOrder,
                           TWindowLevel fromLevel,
                           TWindowLevel toLevel ) const override;
    TObjectOrder getLast( TObjectOrder globalOrder,
                          TWindowLevel fromLevel,
                          TWindowLevel toLevel ) const override;

    bool isSameObjectStruct( Trace *compareTo, bool compareProcessModel ) const override;
    bool isSubsetObjectStruct( Trace *compareTo, bool compareProcessModel ) const override;

    // Communication info getters
    TCommID getTotalComms() const;
    TThreadOrder getSenderThread( TCommID whichComm ) const override;
    TCPUOrder getSenderCPU( TCommID whichComm ) const override;
    TThreadOrder getReceiverThread( TCommID whichComm ) const override;
    TCPUOrder getReceiverCPU( TCommID whichComm ) const override;
    TCommTag getCommTag( TCommID whichComm ) const override;
    TCommSize getCommSize( TCommID whichComm ) const override;
    TRecordTime getLogicalSend( TCommID whichComm ) const override;
    TRecordTime getLogicalReceive( TCommID whichComm ) const override;
    TRecordTime getPhysicalSend( TCommID whichComm ) const override;
    TRecordTime getPhysicalReceive( TCommID whichComm ) const override;

    inline TTime getEndTime() const override
    {
      return traceEndTime;
    }

    inline void setEndTime( TTime whichTraceEndTime ) override
    {
      traceEndTime = whichTraceEndTime;
    }

    inline TTimeUnit getTimeUnit() const override
    {
      return traceTimeUnit;
    }

    inline ptime getTraceTime() const override
    {
      return myTraceTime +
             boost::posix_time::nanoseconds(
               (long)traceUnitsToCustomUnits( myTraceInfo.GetCutterTotalOffset(), NS ) );
    }

    TRecordTime customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const override;
    TRecordTime traceUnitsToCustomUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const override;


    bool eventLoaded( TEventType whichType ) const override;
    bool anyEventLoaded( TEventType firstType, TEventType lastType ) const override;

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

    const std::set<TState>& getLoadedStates() const override;
    const std::set<TEventType>& getLoadedEvents() const override;

    bool findLastEventValue( TThreadOrder whichThread,
                             TRecordTime whichTime,
                             const std::vector<TEventType>& whichEvent,
                             TEventType& returnType,
                             TEventValue& returnValue ) const override;

    bool findNextEvent( TThreadOrder whichThread,
                        TRecordTime whichTime,
                        TEventType whichEvent,
                        TRecordTime& foundTime ) const override;

    virtual bool getFillStateGaps() const override;
    virtual void setFillStateGaps( bool fill ) override;

    virtual PRV_UINT64 getCutterOffset() override;
    virtual PRV_UINT64 getCutterLastOffset() override;
    virtual PRV_UINT64 getCutterLastBeginTime() override;
    virtual PRV_UINT64 getCutterLastEndTime() override;
    virtual PRV_UINT64 getCutterBeginTime() override;
    virtual PRV_UINT64 getCutterEndTime() override;

    virtual void setLogicalSend( TCommID whichComm, TRecordTime whichTime ) override;
    virtual void setLogicalReceive( TCommID whichComm, TRecordTime whichTime ) override;
    virtual void setPhysicalSend( TCommID whichComm, TRecordTime whichTime ) override;
    virtual void setPhysicalReceive( TCommID whichComm, TRecordTime whichTime ) override;

    virtual void   setEventTypePrecision( TEventType whichType, double whichPrecision ) override;
    virtual double getEventTypePrecision( TEventType whichType ) const override;

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
    std::set<TState> states;
    std::map<TEventType, double> eventsPrecision;
    bool fillStateGaps;
    MetadataManager myTraceInfo;

    void parseDateTime( string &whichDateTime );

};


