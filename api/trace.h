/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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

#ifndef TRACE_H_INCLUDED
#define TRACE_H_INCLUDED

#include <set>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "paraverkerneltypes.h"
#include "semanticcolor.h"
#include "eventlabels.h"
#include "statelabels.h"
#include "rowlabels.h"

using boost::posix_time::ptime;

class KernelConnection;
class ProgressController;

class Trace
{
  public:
    static Trace *create( KernelConnection *whichKernel, const std::string& whichFile,
                          bool noLoad, ProgressController *progress );
    static bool isTraceFile( const std::string& filename );
    static bool isOTF2TraceFile( const std::string& filename );


    Trace() {}
    Trace( KernelConnection *whichKernel );
    virtual ~Trace() {}

    TObjectOrder getLevelObjects( TWindowLevel onLevel ) const;

    virtual std::string getFileName() const = 0;
    virtual std::string getTraceName() const = 0;

    virtual void dumpFileHeader( std::fstream& file, bool newFormat = false, PRV_INT32 numIter = 1 ) const = 0;
    virtual void dumpFile( const std::string& whichFile, PRV_INT32 numIter = 1 ) const = 0;

    virtual TApplOrder totalApplications() const = 0;
    virtual TTaskOrder totalTasks() const = 0;
    virtual TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                                      const TTaskOrder& inTask ) const = 0;
    virtual void getTaskLocation( TTaskOrder globalTask,
                                  TApplOrder& inAppl,
                                  TTaskOrder& inTask ) const = 0;
    virtual TTaskOrder getFirstTask( TApplOrder inAppl ) const = 0;
    virtual TTaskOrder getLastTask( TApplOrder inAppl ) const = 0;

    virtual TThreadOrder totalThreads() const = 0;
    virtual TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                          const TTaskOrder& inTask,
                                          const TThreadOrder& inThread ) const = 0;
    virtual void getThreadLocation( TThreadOrder globalThread,
                                    TApplOrder& inAppl,
                                    TTaskOrder& inTask,
                                    TThreadOrder& inThread ) const = 0;
    virtual TThreadOrder getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const = 0;
    virtual TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask ) const = 0;
    virtual void getThreadsPerNode( TNodeOrder inNode, std::vector<TThreadOrder>& onVector ) const = 0;

    virtual bool existResourceInfo() const = 0;
    virtual TNodeOrder totalNodes() const = 0;
    virtual TCPUOrder totalCPUs() const = 0;
    virtual TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                                    const TCPUOrder& inCPU ) const = 0;
    virtual void getCPULocation( TCPUOrder globalCPU,
                                 TNodeOrder& inNode,
                                 TCPUOrder& inCPU ) const = 0;
    virtual TCPUOrder getFirstCPU( TNodeOrder inNode ) const = 0;
    virtual TCPUOrder getLastCPU( TNodeOrder inNode ) const = 0;

    virtual TObjectOrder getFirst( TObjectOrder globalOrder,
                                   TWindowLevel fromLevel,
                                   TWindowLevel toLevel ) const = 0;
    virtual TObjectOrder getLast( TObjectOrder globalOrder,
                                  TWindowLevel fromLevel,
                                  TWindowLevel toLevel ) const = 0;

    virtual bool isSameObjectStruct( Trace *compareTo ) const = 0;

    // Communication info getters
    virtual TThreadOrder getSenderThread( TCommID whichComm ) const = 0;
    virtual TCPUOrder getSenderCPU( TCommID whichComm ) const = 0;
    virtual TThreadOrder getReceiverThread( TCommID whichComm ) const = 0;
    virtual TCPUOrder getReceiverCPU( TCommID whichComm ) const = 0;
    virtual TCommTag getCommTag( TCommID whichComm ) const = 0;
    virtual TCommSize getCommSize( TCommID whichComm ) const = 0;
    virtual TRecordTime getLogicalSend( TCommID whichComm ) const = 0;
    virtual TRecordTime getLogicalReceive( TCommID whichComm ) const = 0;
    virtual TRecordTime getPhysicalSend( TCommID whichComm ) const = 0;
    virtual TRecordTime getPhysicalReceive( TCommID whichComm ) const = 0;

    virtual TTime getEndTime() const = 0;
    virtual void setEndTime( TTime whichTraceEndTime ) = 0;
    virtual TTimeUnit getTimeUnit() const = 0;
    virtual const ptime& getTraceTime() const = 0;

    virtual bool eventLoaded( TEventType whichType ) const = 0;
    virtual bool anyEventLoaded( TEventType firstType, TEventType lastType ) const = 0;
    virtual const std::set<TEventType>& getLoadedEvents() const = 0;

    virtual bool findLastEventValue( TThreadOrder whichThread,
                                     TRecordTime whichTime,
                                     const std::vector<TEventType>& whichEvent,
                                     TEventType& returnType,
                                     TEventValue& returnValue ) const = 0;

    virtual bool findNextEvent( TThreadOrder whichThread,
                                TRecordTime whichTime,
                                TEventType whichEvent,
                                TRecordTime& foundTime ) const = 0;

    virtual bool getFillStateGaps() const = 0;

    virtual void setFillStateGaps( bool fill ) = 0;

    virtual PRV_UINT64 getCutterOffset() = 0;
    virtual PRV_UINT64 getCutterBeginTime() = 0;
    virtual PRV_UINT64 getCutterEndTime() = 0;

    virtual void setLogicalSend( TCommID whichComm, TRecordTime whichTime ) = 0;
    virtual void setLogicalReceive( TCommID whichComm, TRecordTime whichTime ) = 0;
    virtual void setPhysicalSend( TCommID whichComm, TRecordTime whichTime ) = 0;
    virtual void setPhysicalReceive( TCommID whichComm, TRecordTime whichTime ) = 0;


    // Specific methods for TraceProxy only
    virtual bool getUnload() const
    {
      return false;
    }
    virtual void setUnload( bool newValue ) {}
    virtual Trace *getConcrete() const
    {
      return NULL;
    }
    virtual std::string getFileNameNumbered() const
    {
      return "";
    }
    virtual std::string getTraceNameNumbered() const
    {
      return "";
    }
    virtual void setInstanceNumber( PRV_UINT32 whichInstanceNumber ) {}
    virtual const CodeColor& getCodeColor() const
    {
      CodeColor *tmp = NULL;
      return *tmp;
    }
    virtual const GradientColor& getGradientColor() const
    {
      GradientColor *tmp = NULL;
      return *tmp;
    }
    virtual const EventLabels& getEventLabels() const
    {
      EventLabels *tmp = NULL;
      return *tmp;
    }
    virtual const StateLabels& getStateLabels() const
    {
      StateLabels *tmp = NULL;
      return *tmp;
    }
    virtual std::string getRowLabel( TWindowLevel whichLevel, TObjectOrder whichRow ) const
    {
      return "";
    }

    virtual size_t getMaxLengthRow( TWindowLevel whichLevel = NONE ) const
    {
      return 0;
    }

    virtual std::string getDefaultSemanticFunc( TWindowLevel whichLevel ) const
    {
      return "";
    }

    virtual void setShowProgressBar( bool whichShow )
    {}

    virtual bool getShowProgressBar() const
    {
      return true;
    }

    /*
        virtual bool getFillStateGaps() const
        {
          return false;
        }

        void setFillStateGaps( bool fill ) {}
    */

  protected:
    KernelConnection *myKernel;

};

class TraceProxy: public Trace
{
  public:
    virtual ~TraceProxy();

    virtual std::string getFileName() const;
    virtual std::string getTraceName() const;
    virtual std::string getFileNameNumbered() const;
    virtual std::string getTraceNameNumbered() const;
    virtual void setInstanceNumber( PRV_UINT32 whichInstanceNumber );

    virtual void dumpFileHeader( std::fstream& file, bool newFormat = false, PRV_INT32 numIter = 1 ) const;
    virtual void dumpFile( const std::string& whichFile, PRV_INT32 numIter = 1 ) const;

    virtual TApplOrder totalApplications() const;
    virtual TTaskOrder totalTasks() const;
    virtual TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                                      const TTaskOrder& inTask ) const;
    virtual void getTaskLocation( TTaskOrder globalTask,
                                  TApplOrder& inAppl,
                                  TTaskOrder& inTask ) const;
    virtual TTaskOrder getFirstTask( TApplOrder inAppl ) const;
    virtual TTaskOrder getLastTask( TApplOrder inAppl ) const;

    virtual TThreadOrder totalThreads() const;
    virtual TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                          const TTaskOrder& inTask,
                                          const TThreadOrder& inThread ) const;
    virtual void getThreadLocation( TThreadOrder globalThread,
                                    TApplOrder& inAppl,
                                    TTaskOrder& inTask,
                                    TThreadOrder& inThread ) const;
    virtual TThreadOrder getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const;
    virtual TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask ) const;
    virtual void getThreadsPerNode( TNodeOrder inNode, std::vector<TThreadOrder>& onVector ) const;

    virtual bool existResourceInfo() const;
    virtual TNodeOrder totalNodes() const;
    virtual TCPUOrder totalCPUs() const;
    virtual TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                                    const TCPUOrder& inCPU ) const;
    virtual void getCPULocation( TCPUOrder globalCPU,
                                 TNodeOrder& inNode,
                                 TCPUOrder& inCPU ) const;
    virtual TCPUOrder getFirstCPU( TNodeOrder inNode ) const;
    virtual TCPUOrder getLastCPU( TNodeOrder inNode ) const;

    virtual TObjectOrder getFirst( TObjectOrder globalOrder,
                                   TWindowLevel fromLevel,
                                   TWindowLevel toLevel ) const;
    virtual TObjectOrder getLast( TObjectOrder globalOrder,
                                  TWindowLevel fromLevel,
                                  TWindowLevel toLevel ) const;

    virtual bool isSameObjectStruct( Trace *compareTo ) const;

    // Communication info getters
    virtual TThreadOrder getSenderThread( TCommID whichComm ) const;
    virtual TCPUOrder getSenderCPU( TCommID whichComm ) const;
    virtual TThreadOrder getReceiverThread( TCommID whichComm ) const;
    virtual TCPUOrder getReceiverCPU( TCommID whichComm ) const;
    virtual TCommTag getCommTag( TCommID whichComm ) const;
    virtual TCommSize getCommSize( TCommID whichComm ) const;
    virtual TRecordTime getLogicalSend( TCommID whichComm ) const;
    virtual TRecordTime getLogicalReceive( TCommID whichComm ) const;
    virtual TRecordTime getPhysicalSend( TCommID whichComm ) const;
    virtual TRecordTime getPhysicalReceive( TCommID whichComm ) const;

    virtual TTime getEndTime() const;
    virtual void setEndTime( TTime whichTraceEndTime );
    virtual TTimeUnit getTimeUnit() const;
    virtual const ptime& getTraceTime() const;

    virtual bool getUnload() const;
    virtual void setUnload( bool newValue );
    virtual Trace *getConcrete() const;
    virtual const CodeColor& getCodeColor() const;
    virtual const GradientColor& getGradientColor() const;
    virtual const EventLabels& getEventLabels() const;
    virtual const StateLabels& getStateLabels() const;
    virtual std::string getRowLabel( TWindowLevel whichLevel, TObjectOrder whichRow ) const;
    virtual size_t getMaxLengthRow( TWindowLevel whichLevel ) const;

//    virtual std::string getDefaultSemanticFunc( TWindowLevel whichLevel ) const;
    virtual void setShowProgressBar( bool whichShow );
    virtual bool getShowProgressBar() const;

    virtual bool eventLoaded( TEventType whichType ) const;
    virtual bool anyEventLoaded( TEventType firstType, TEventType lastType ) const;
    virtual const std::set<TEventType>& getLoadedEvents() const;

    virtual bool findLastEventValue( TThreadOrder whichThread,
                                     TRecordTime whichTime,
                                     const std::vector<TEventType>& whichEvent,
                                     TEventType& returnType,
                                     TEventValue& returnValue ) const;

    virtual bool findNextEvent( TThreadOrder whichThread,
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


  private:
    Trace *myTrace;

    bool unload;
    PRV_UINT32 instanceNumber;

    CodeColor myCodeColor;
    GradientColor myGradientColor;

    EventLabels myEventLabels;
    StateLabels myStateLabels;
    RowLabels myRowLabels;

    bool showProgressBar;

//    std::string myDefaultTaskSemanticFunc;
//    std::string myDefaultThreadSemanticFunc;

    TraceProxy( KernelConnection *whichKernel, const std::string& whichFile,
                bool noLoad, ProgressController *progress );

    void parsePCF( const std::string& whichFile );
    void parseROW( const std::string& whichFile );

#ifdef FIXED_LABELS
    void setFixedLabels();
#endif

    friend Trace *Trace::create( KernelConnection *, const std::string&, bool noLoad, ProgressController * );
};

#endif // TRACE_H_INCLUDED
