#ifndef MEMORYBLOCKS_H_INCLUDED
#define MEMORYBLOCKS_H_INCLUDED

#include "paraverkerneltypes.h"


class MemoryBlocks
{
  public:
    MemoryBlocks();
    virtual ~MemoryBlocks() = 0;

    virtual void newRecord() = 0;
    virtual void setType( const TRecordType whichType ) = 0;
    virtual void setTime( const TRecordTime whichTime ) = 0;
    virtual void setThread( const TThreadOrder whichThread ) = 0;
    virtual void setThread( const TApplOrder whichAppl,
                            const TTaskOrder whichTask,
                            const TThreadOrder whichThread ) = 0;
    virtual void setCPU( const TCPUOrder whichCPU ) = 0;
    virtual void setEventType( const TEventType whichType ) = 0;
    virtual void setEventValue( const TEventValue whichValue ) = 0;
    virtual void setState( const TState whichState ) = 0;
    virtual void setStateEndTime( const TRecordTime whichTime ) = 0;
    virtual void setCommTag( const TCommTag whichTag ) = 0;
    virtual void setCommSize( const TCommSize whichSize ) = 0;

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
