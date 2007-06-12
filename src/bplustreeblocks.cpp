#include "bplustreeblocks.h"

using namespace BPlusTree;

void BPlusTreeBlocks::newRecord()
{}

void BPlusTreeBlocks::setType( TRecordType whichType )
{
  currentBlock[currentRecord].type = whichType;
}

void BPlusTreeBlocks::setTime( TRecordTime whichTime )
{
  currentBlock[currentRecord].time = whichTime;
}

void BPlusTreeBlocks::setThread( TThreadOrder whichThread )
{
  currentBlock[currentRecord].thread = whichThread;
}

void BPlusTreeBlocks::setThread( TApplOrder whichAppl,
                                 TTaskOrder whichTask,
                                 TThreadOrder whichThread )
{}

void BPlusTreeBlocks::setCPU( TCPUOrder whichCPU )
{
  currentBlock[currentRecord].CPU = whichCPU;
}

void BPlusTreeBlocks::setEventType( TEventType whichType )
{
  currentBlock[currentRecord].URecordInfo.eventRecord.type = whichType;
}

void BPlusTreeBlocks::setEventValue( TEventValue whichValue )
{
  currentBlock[currentRecord].URecordInfo.eventRecord.value = whichValue;
}

void BPlusTreeBlocks::setState( TState whichState )
{
  currentBlock[currentRecord].URecordInfo.stateRecord.state = whichState;
}

void BPlusTreeBlocks::setStateEndTime( TRecordTime whichTime )
{
  currentBlock[currentRecord].URecordInfo.stateRecord.endTime = whichTime;
}

void BPlusTreeBlocks::setCommIndex( TCommID whichID )
{
  currentBlock[currentRecord].URecordInfo.commRecord.info = whichID;
}

void BPlusTreeBlocks::newComm( bool createRecords )
{}

void BPlusTreeBlocks::setSenderThread( TThreadOrder whichThread )
{}

void BPlusTreeBlocks::setSenderThread( TApplOrder whichAppl,
                                       TTaskOrder whichTask,
                                       TThreadOrder whichThread )
{}

void BPlusTreeBlocks::setSenderCPU( TCPUOrder whichCPU )
{}

void BPlusTreeBlocks::setReceiverThread( TThreadOrder whichThread )
{}

void BPlusTreeBlocks::setReceiverThread( TApplOrder whichAppl,
    TTaskOrder whichTask,
    TThreadOrder whichThread )
{}

void BPlusTreeBlocks::setReceiverCPU( TCPUOrder whichCPU )
{}

void BPlusTreeBlocks::setCommTag( TCommTag whichTag )
{}

void BPlusTreeBlocks::setCommSize( TCommSize whichSize )
{}

void BPlusTreeBlocks::setLogicalSend( TRecordTime whichTime )
{}

void BPlusTreeBlocks::setLogicalReceive( TRecordTime whichTime )
{}

void BPlusTreeBlocks::setPhysicalSend( TRecordTime whichTime )
{}

void BPlusTreeBlocks::setPhysicalReceive( TRecordTime whichTime )
{}
