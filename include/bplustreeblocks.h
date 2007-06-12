#ifndef BPLUSTREEBLOCKS_H_INCLUDED
#define BPLUSTREEBLOCKS_H_INCLUDED

#include <vector>
#include "memoryblocks.h"
#include "bplustreetypes.h"

using namespace std;

namespace BPlusTree
{
  class BPlusTreeBlocks : public MemoryBlocks
  {
    public:
      BPlusTreeBlocks()
      {
        blocks.push_back( new TRecord[blockSize] );
        currentBlock = blocks[blocks.size() - 1];
        currentRecord = 0;
      }

      virtual ~BPlusTreeBlocks()
      {
        if ( blocks[ 0 ] != NULL )
        {
          for ( UINT32 i = 0; i < blocks.size(); i++ )
            delete[] blocks[i];
        }
        blocks.clear();
      }

      virtual TRecord *getLastRecord( UINT16 position ) const
      {
        return lastRecords[ position ];
      }

      virtual void resetCountInserted()
      {
        MemoryBlocks::resetCountInserted();
        lastRecords.clear();
      }

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

    protected:

    private:
      static const UINT32 blockSize = 100000;
      UINT32 currentRecord;
      TRecord *currentBlock;
      vector<TRecord *> lastRecords;
      vector<TRecord *> blocks;

  };
}

#endif // BPLUSTREEBLOCKS_H_INCLUDED
