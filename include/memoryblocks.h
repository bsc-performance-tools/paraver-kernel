#ifndef MEMORYBLOCKS_H_INCLUDED
#define MEMORYBLOCKS_H_INCLUDED

#include "paraverkerneltypes.h"


class MemoryBlocks
{
  public:
    MemoryBlocks();
    virtual ~MemoryBlocks() = 0;

    virtual void newRecord() = 0;
    virtual void setType( TRecordType whichType ) = 0;
    virtual void setTime( TRecordTime whichTime ) = 0;
    virtual void setThread( TThreadOrder whichThread ) = 0;
    virtual void setThread( TApplOrder whichAppl,
                            TTaskOrder whichTask,
                            TThreadOrder whichThread ) = 0;
    virtual void setCPU( TCPUOrder whichCPU ) = 0;
    virtual void setEventType( TEventType whichType ) = 0;
    virtual void setEventValue( TEventValue whichValue ) = 0;
    virtual void setState( TState whichState ) = 0;
    virtual void setStateEndTime( TRecordTime whichTime ) = 0;
    virtual void setCommIndex( TCommID whichID ) = 0;

    virtual TCommID newComm() = 0;
    virtual void setCommTag( TCommTag whichTag ) = 0;
    virtual void setCommSize( TCommSize whichSize ) = 0;
    virtual void setLogicalSend( TRecordTime whichTime ) = 0;
    virtual void setLogicalReceive( TRecordTime whichTime ) = 0;
    virtual void setPhysicalSend( TRecordTime whichTime ) = 0;
    virtual void setPhysicalReceive( TRecordTime whichTime ) = 0;

    UINT32 getCountInserted() const
    {
      return countInserted;
    }

  protected:
    // Number of new records inserted in the last operation
    UINT32 countInserted;

  private:
};


#endif // MEMORYBLOCKS_H_INCLUDED
