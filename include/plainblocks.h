#ifndef PLAINBLOCKS_H_INCLUDED
#define PLAINBLOCKS_H_INCLUDED

#include "memoryblocks.h"
#include "resourcemodel.h"
#include "processmodel.h"
#include "plaintypes.h"
#include "plaintrace.h"

namespace Plain
{
  struct TLastRecord: public TData
  {
    TRecordTime time;
    TThreadOrder thread;
    UINT32 block;
    UINT32 pos;
  };

  class PlainBlocks: public MemoryBlocks
  {
    public:
      PlainBlocks( const ResourceModel& resource, const ProcessModel& process );

      ~PlainBlocks();

      virtual TData *getLastRecord( UINT16 position ) const;
      virtual void resetCountInserted();
      virtual void newRecord();
      virtual void setType( TRecordType whichType );
      virtual void setTime( TRecordTime whichTime );
      virtual void setThread( TThreadOrder whichThread );
      virtual void setThread( TApplOrder whichAppl,
                              TTaskOrder whichTask,
                              TThreadOrder whichThread );
      virtual void setCPU( TCPUOrder whichCPU );
      virtual void setEventType( TEventType whichType );
      virtual void setEventValue( TEventValue whichValue );
      virtual void setState( TState whichState );
      virtual void setStateEndTime( TRecordTime whichTime );
      virtual void setCommIndex( TCommID whichID );

      // If you have a block defining communications, probably you want
      // to create records separately by your own.
      // Then you must call newComm( false )
      // If not, the function creates all necessary records by default.
      virtual void newComm( bool createRecords = true );
      virtual void setSenderThread( TThreadOrder whichThread );
      virtual void setSenderThread( TApplOrder whichAppl,
                                    TTaskOrder whichTask,
                                    TThreadOrder whichThread );
      virtual void setSenderCPU( TCPUOrder whichCPU );
      virtual void setReceiverThread( TThreadOrder whichThread );
      virtual void setReceiverThread( TApplOrder whichAppl,
                                      TTaskOrder whichTask,
                                      TThreadOrder whichThread );
      virtual void setReceiverCPU( TCPUOrder whichCPU );
      virtual void setCommTag( TCommTag whichTag );
      virtual void setCommSize( TCommSize whichSize );
      virtual void setLogicalSend( TRecordTime whichTime );
      virtual void setLogicalReceive( TRecordTime whichTime );
      virtual void setPhysicalSend( TRecordTime whichTime );
      virtual void setPhysicalReceive( TRecordTime whichTime );

      // Communication info getters
      virtual TCommID getTotalComms() const;
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

      virtual TRecordTime getLastRecordTime() const;

    protected:

    private:
      typedef enum
      {
        logicalSend = 0,
        logicalReceive,
        physicalSend,
        physicalReceive,
        remoteLogicalSend,
        remoteLogicalReceive,
        remotePhysicalSend,
        remotePhysicalReceive,
        commTypeSize
      } TCommType;
      static const TRecordType commTypes[commTypeSize];
      static const UINT32 blockSize = 10000;
      vector<UINT32> currentRecord;
      vector<TRecord *> currentBlock;
      vector<TLastRecord> lastRecords;
      vector<vector<TRecord *> > blocks;
      vector<TCommInfo *> communications;
      TCommID currentComm;
      const ResourceModel& resourceModel;
      const ProcessModel& processModel;
      TRecord tmpRecord;
      bool inserted;
      TThreadOrder insertedOnThread;

      friend class PlainTrace;
      friend class PlainTrace::iterator;
      friend class PlainTrace::ThreadIterator;
      friend class PlainTrace::CPUIterator;
  };
}

#endif // PLAINBLOCKS_H_INCLUDED
