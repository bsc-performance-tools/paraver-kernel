#ifndef TRACE_H_INCLUDED
#define TRACE_H_INCLUDED

#include <set>
#include "paraverkerneltypes.h"
#include "semanticcolor.h"
#include "eventlabels.h"
#include "statelabels.h"
#include "rowlabels.h"

class KernelConnection;
class ProgressController;

class Trace
{
  public:
    static Trace *create( KernelConnection *whichKernel, const string& whichFile,
                          ProgressController *progress );

    Trace() {}
    Trace( KernelConnection *whichKernel );
    virtual ~Trace() {}

    TObjectOrder getLevelObjects( TWindowLevel onLevel ) const;

    virtual string getFileName() const = 0;
    virtual string getTraceName() const = 0;

    virtual void dumpFile( const string& whichFile ) const = 0;

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
    virtual TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask )const = 0;

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
    virtual TTimeUnit getTimeUnit() const = 0;

    virtual bool eventLoaded( TEventType whichType ) const = 0;
    virtual const set<TEventType>& getLoadedEvents() const = 0;


    // Specific methods for TraceProxy only
    virtual Trace *getConcrete() const
    {
      return NULL;
    }
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
    virtual string getRowLabel( TWindowLevel whichLevel, TObjectOrder whichRow ) const
    {
      return "";
    }

    virtual string getDefaultSemanticFunc( TWindowLevel whichLevel ) const
    {
      return "";
    }

  protected:
    KernelConnection *myKernel;

};

class TraceProxy: public Trace
{
  public:
    virtual ~TraceProxy();

    virtual string getFileName() const;
    virtual string getTraceName() const;

    virtual void dumpFile( const string& whichFile ) const;

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
    virtual TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask )const;

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
    virtual TTimeUnit getTimeUnit() const;

    virtual Trace *getConcrete() const;
    virtual const CodeColor& getCodeColor() const;
    virtual const GradientColor& getGradientColor() const;
    virtual const EventLabels& getEventLabels() const;
    virtual const StateLabels& getStateLabels() const;
    virtual string getRowLabel( TWindowLevel whichLevel, TObjectOrder whichRow ) const;

    virtual string getDefaultSemanticFunc( TWindowLevel whichLevel ) const;

    virtual bool eventLoaded( TEventType whichType ) const;
    virtual const set<TEventType>& getLoadedEvents() const;


  private:
    Trace *myTrace;

    CodeColor myCodeColor;
    GradientColor myGradientColor;

    EventLabels myEventLabels;
    StateLabels myStateLabels;
    RowLabels myRowLabels;

    string myDefaultTaskSemanticFunc;
    string myDefaultThreadSemanticFunc;

    TraceProxy( KernelConnection *whichKernel, const string& whichFile,
                ProgressController *progress );

    void parsePCF( const string& whichFile );
    void parseROW( const string& whichFile );

    friend Trace *Trace::create( KernelConnection *, const string&, ProgressController * );
};

#endif // TRACE_H_INCLUDED
