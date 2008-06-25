#include "kernelconnection.h"
#include "trace.h"

Trace *Trace::create( KernelConnection *whichKernel, const string& whichFile )
{
  return new TraceProxy( whichKernel, whichFile );
}

Trace::Trace( KernelConnection *whichKernel ):
    myKernel( whichKernel )
{}

TraceProxy::TraceProxy( KernelConnection *whichKernel, const string& whichFile ):
    Trace( whichKernel )
{
  myTrace = myKernel->newTrace( whichFile );
}

TraceProxy::~TraceProxy()
{
  delete myTrace;
}

void TraceProxy::dumpFile( const string& whichFile ) const
{
  myTrace->dumpFile( whichFile );
}

TApplOrder TraceProxy::totalApplications() const
{
  return myTrace->totalApplications();
}

TTaskOrder TraceProxy::totalTasks() const
{
  return myTrace->totalTasks();
}

TTaskOrder TraceProxy::getGlobalTask( const TApplOrder& inAppl,
                                      const TTaskOrder& inTask ) const
{
  return myTrace->getGlobalTask( inAppl, inTask );
}

void TraceProxy::getTaskLocation( TTaskOrder globalTask,
                                  TApplOrder& inAppl,
                                  TTaskOrder& inTask ) const
{
  myTrace->getTaskLocation( globalTask, inAppl, inTask );
}

TTaskOrder TraceProxy::getFirstTask( TApplOrder inAppl ) const
{
  return myTrace->getFirstTask( inAppl );
}

TTaskOrder TraceProxy::getLastTask( TApplOrder inAppl ) const
{
  return myTrace->getLastTask( inAppl );
}

TThreadOrder TraceProxy::totalThreads() const
{
  return myTrace->totalThreads();
}

TThreadOrder TraceProxy::getGlobalThread( const TApplOrder& inAppl,
    const TTaskOrder& inTask,
    const TThreadOrder& inThread ) const
{
  return myTrace->getGlobalThread( inAppl, inTask, inThread );
}

void TraceProxy::getThreadLocation( TThreadOrder globalThread,
                                    TApplOrder& inAppl,
                                    TTaskOrder& inTask,
                                    TThreadOrder& inThread ) const
{
  myTrace->getThreadLocation( globalThread, inAppl, inTask, inThread );
}

TThreadOrder TraceProxy::getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const
{
  return myTrace->getFirstThread( inAppl, inTask );
}

TThreadOrder TraceProxy::getLastThread( TApplOrder inAppl, TTaskOrder inTask ) const
{
  return myTrace->getLastThread( inAppl, inTask );
}

bool TraceProxy::existResourceInfo() const
{
  return myTrace->existResourceInfo();
}

TNodeOrder TraceProxy::totalNodes() const
{
  return myTrace->totalNodes();
}

TCPUOrder TraceProxy::totalCPUs() const
{
  return myTrace->totalCPUs();
}

TCPUOrder TraceProxy::getGlobalCPU( const TNodeOrder& inNode,
                                    const TCPUOrder& inCPU ) const
{
  return myTrace->getGlobalCPU( inNode, inCPU );
}

void TraceProxy::getCPULocation( TCPUOrder globalCPU,
                                 TNodeOrder& inNode,
                                 TCPUOrder& inCPU ) const
{
  myTrace->getCPULocation( globalCPU, inNode, inCPU );
}

TCPUOrder TraceProxy::getFirstCPU( TNodeOrder inNode ) const
{
  return myTrace->getFirstCPU( inNode );
}

TCPUOrder TraceProxy::getLastCPU( TNodeOrder inNode ) const
{
  return myTrace->getLastCPU( inNode );
}

TObjectOrder TraceProxy::getFirst( TObjectOrder globalOrder,
                                   TWindowLevel fromLevel,
                                   TWindowLevel toLevel ) const
{
  return myTrace->getFirst( globalOrder, fromLevel, toLevel );
}

TObjectOrder TraceProxy::getLast( TObjectOrder globalOrder,
                                  TWindowLevel fromLevel,
                                  TWindowLevel toLevel ) const
{
  return myTrace->getLast( globalOrder, fromLevel, toLevel );
}

TThreadOrder TraceProxy::getSenderThread( TCommID whichComm ) const
{
  return myTrace->getSenderThread( whichComm );
}

TCPUOrder TraceProxy::getSenderCPU( TCommID whichComm ) const
{
  return myTrace->getSenderCPU( whichComm );
}

TThreadOrder TraceProxy::getReceiverThread( TCommID whichComm ) const
{
  return myTrace->getReceiverThread( whichComm );
}

TCPUOrder TraceProxy::getReceiverCPU( TCommID whichComm ) const
{
  return myTrace->getReceiverCPU( whichComm );
}

TCommTag TraceProxy::getCommTag( TCommID whichComm ) const
{
  return myTrace->getCommTag( whichComm );
}

TCommSize TraceProxy::getCommSize( TCommID whichComm ) const
{
  return myTrace->getCommSize( whichComm );
}

TRecordTime TraceProxy::getLogicalSend( TCommID whichComm ) const
{
  return myTrace->getLogicalSend( whichComm );
}

TRecordTime TraceProxy::getLogicalReceive( TCommID whichComm ) const
{
  return myTrace->getLogicalReceive( whichComm );
}

TRecordTime TraceProxy::getPhysicalSend( TCommID whichComm ) const
{
  return myTrace->getPhysicalSend( whichComm );
}

TRecordTime TraceProxy::getPhysicalReceive( TCommID whichComm ) const
{
  return myTrace->getPhysicalReceive( whichComm );
}

TTime TraceProxy::getEndTime() const
{
  return myTrace->getEndTime();
}

TTimeUnit TraceProxy::getTimeUnit() const
{
  return myTrace->getTimeUnit();
}

Trace *TraceProxy::getConcrete() const
{
  return myTrace;
}
