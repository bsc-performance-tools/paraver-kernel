#include "noloadblocks.h"

using namespace NoLoad;

NoLoadBlocks::NoLoadBlocks()
{}

NoLoadBlocks::~NoLoadBlocks()
{}

TData *NoLoadBlocks::getLastRecord( UINT16 position ) const
{
  return NULL;
}

void NoLoadBlocks::newRecord()
{}

void NoLoadBlocks::setType( TRecordType whichType )
{}

void NoLoadBlocks::setTime( TRecordTime whichTime )
{}

void NoLoadBlocks::setThread( TThreadOrder whichThread )
{}

void NoLoadBlocks::setThread( TApplOrder whichAppl,
                              TTaskOrder whichTask,
                              TThreadOrder whichThread )
{}

void NoLoadBlocks::setCPU( TCPUOrder whichCPU )
{}

void NoLoadBlocks::setEventType( TEventType whichType )
{}

void NoLoadBlocks::setEventValue( TEventValue whichValue )
{}

void NoLoadBlocks::setState( TState whichState )
{}

void NoLoadBlocks::setStateEndTime( TRecordTime whichTime )
{}

void NoLoadBlocks::setCommIndex( TCommID whichID )
{}


void NoLoadBlocks::newComm( bool createRecords )
{}

void NoLoadBlocks::setSenderThread( TThreadOrder whichThread )
{}

void NoLoadBlocks::setSenderThread( TApplOrder whichAppl,
                                    TTaskOrder whichTask,
                                    TThreadOrder whichThread )
{}

void NoLoadBlocks::setSenderCPU( TCPUOrder whichCPU )
{}

void NoLoadBlocks::setReceiverThread( TThreadOrder whichThread )
{}

void NoLoadBlocks::setReceiverThread( TApplOrder whichAppl,
                                      TTaskOrder whichTask,
                                      TThreadOrder whichThread )
{}

void NoLoadBlocks::setReceiverCPU( TCPUOrder whichCPU )
{}

void NoLoadBlocks::setCommTag( TCommTag whichTag )
{}

void NoLoadBlocks::setCommSize( TCommSize whichSize )
{}

void NoLoadBlocks::setLogicalSend( TRecordTime whichTime )
{}

void NoLoadBlocks::setLogicalReceive( TRecordTime whichTime )
{}

void NoLoadBlocks::setPhysicalSend( TRecordTime whichTime )
{}

void NoLoadBlocks::setPhysicalReceive( TRecordTime whichTime )
{}

TCommID NoLoadBlocks::getTotalComms() const
{
  return 0;
}

TThreadOrder NoLoadBlocks::getSenderThread( TCommID whichComm ) const
{
  return 0;
}

TCPUOrder NoLoadBlocks::getSenderCPU( TCommID whichComm ) const
{
  return 0;
}

TThreadOrder NoLoadBlocks::getReceiverThread( TCommID whichComm ) const
{
  return 0;
}

TCPUOrder NoLoadBlocks::getReceiverCPU( TCommID whichComm ) const
{
  return 0;
}

TCommTag NoLoadBlocks::getCommTag( TCommID whichComm ) const
{
  return 0;
}

TCommSize NoLoadBlocks::getCommSize( TCommID whichComm ) const
{
  return 0;
}

TRecordTime NoLoadBlocks::getLogicalSend( TCommID whichComm ) const
{
  return 0;
}

TRecordTime NoLoadBlocks::getLogicalReceive( TCommID whichComm ) const
{
  return 0;
}

TRecordTime NoLoadBlocks::getPhysicalSend( TCommID whichComm ) const
{
  return 0;
}

TRecordTime NoLoadBlocks::getPhysicalReceive( TCommID whichComm ) const
{
  return 0;
}

TRecordTime NoLoadBlocks::getLastRecordTime() const
{
  return 0;
}
