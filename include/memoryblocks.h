#ifndef MEMORYBLOCKS_H_INCLUDED
#define MEMORYBLOCKS_H_INCLUDED

#include "paraverkerneltypes.h"

// It manages the memory blocks where records are inserted.
// It depends on MemoryTrace derived classes structures.
class MemoryBlocks
{
  public:
    MemoryBlocks()
    {
      countInserted = 0;
    }

    virtual ~MemoryBlocks()
    {}

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

    // If you have a block defining communications, probably you want
    // to create records separately by your own.
    // Then you must call newComm( false )
    // If not, the function creates all necessary records by default.
    virtual void newComm( bool createRecords = true ) = 0;
    virtual void setSenderThread( TThreadOrder whichThread ) = 0;
    virtual void setSenderThread( TApplOrder whichAppl,
                                  TTaskOrder whichTask,
                                  TThreadOrder whichThread ) = 0;
    virtual void setSenderCPU( TCPUOrder whichCPU ) = 0;
    virtual void setReceiverThread( TThreadOrder whichThread ) = 0;
    virtual void setReceiverThread( TApplOrder whichAppl,
                                  TTaskOrder whichTask,
                                  TThreadOrder whichThread ) = 0;
    virtual void setReceiverCPU( TCPUOrder whichCPU ) = 0;
    virtual void setCommTag( TCommTag whichTag ) = 0;
    virtual void setCommSize( TCommSize whichSize ) = 0;
    virtual void setLogicalSend( TRecordTime whichTime ) = 0;
    virtual void setLogicalReceive( TRecordTime whichTime ) = 0;
    virtual void setPhysicalSend( TRecordTime whichTime ) = 0;
    virtual void setPhysicalReceive( TRecordTime whichTime ) = 0;

    virtual UINT32 getCountInserted() const
    {
      return countInserted;
    }

    virtual void resetCountInserted()
    {
      countInserted = 0;
    }

  protected:
    // Number of new records inserted in the last operation
    UINT32 countInserted;

  private:
};


#endif // MEMORYBLOCKS_H_INCLUDED
