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

    virtual TRecordTime getLastRecordTime() const = 0;

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
