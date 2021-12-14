/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/


#pragma once


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
    PRV_UINT32 block;
    PRV_UINT32 pos;
  };

  class PlainBlocks: public MemoryBlocks
  {
    public:
      PlainBlocks( const ResourceModel<>& resource, const ProcessModel<>& process,
                   TRecordTime endTime );

      ~PlainBlocks();

      virtual TData *getLastRecord( PRV_UINT16 position ) const override;
      virtual void resetCountInserted() override;
      virtual void newRecord() override;
      virtual void setType( TRecordType whichType ) override;
      virtual void setTime( TRecordTime whichTime ) override;
      virtual void setThread( TThreadOrder whichThread ) override;
      virtual void setThread( TApplOrder whichAppl,
                              TTaskOrder whichTask,
                              TThreadOrder whichThread ) override;
      virtual void setCPU( TCPUOrder whichCPU ) override;
      virtual void setEventType( TEventType whichType ) override;
      virtual void setEventValue( TEventValue whichValue ) override;
      virtual void setState( TState whichState ) override;
      virtual void setStateEndTime( TRecordTime whichTime ) override;
      virtual void setCommIndex( TCommID whichID ) override;

      // If you have a block defining communications, probably you want
      // to create records separately by your own.
      // Then you must call newComm( false )
      // If not, the function creates all necessary records by default.
      virtual void newComm( bool createRecords = true ) override;
      virtual void setSenderThread( TThreadOrder whichThread ) override;
      virtual void setSenderThread( TApplOrder whichAppl,
                                    TTaskOrder whichTask,
                                    TThreadOrder whichThread ) override;
      virtual void setSenderCPU( TCPUOrder whichCPU ) override;
      virtual void setReceiverThread( TThreadOrder whichThread ) override;
      virtual void setReceiverThread( TApplOrder whichAppl,
                                      TTaskOrder whichTask,
                                      TThreadOrder whichThread ) override;
      virtual void setReceiverCPU( TCPUOrder whichCPU ) override;
      virtual void setCommTag( TCommTag whichTag ) override;
      virtual void setCommSize( TCommSize whichSize ) override;
      virtual void setLogicalSend( TRecordTime whichTime ) override;
      virtual void setLogicalReceive( TRecordTime whichTime ) override;
      virtual void setPhysicalSend( TRecordTime whichTime ) override;
      virtual void setPhysicalReceive( TRecordTime whichTime ) override;
      virtual void setLogicalSend( TCommID whichComm, TRecordTime whichTime ) override;
      virtual void setLogicalReceive( TCommID whichComm, TRecordTime whichTime ) override;
      virtual void setPhysicalSend( TCommID whichComm, TRecordTime whichTime ) override;
      virtual void setPhysicalReceive( TCommID whichComm, TRecordTime whichTime ) override;

      // Communication info getters
      virtual TCommID getTotalComms() const override;
      virtual TThreadOrder getSenderThread( TCommID whichComm ) const override;
      virtual TCPUOrder getSenderCPU( TCommID whichComm ) const override;
      virtual TThreadOrder getReceiverThread( TCommID whichComm ) const override;
      virtual TCPUOrder getReceiverCPU( TCommID whichComm ) const override;
      virtual TCommTag getCommTag( TCommID whichComm ) const override;
      virtual TCommSize getCommSize( TCommID whichComm ) const override;
      virtual TRecordTime getLogicalSend( TCommID whichComm ) const override;
      virtual TRecordTime getLogicalReceive( TCommID whichComm ) const override;
      virtual TRecordTime getPhysicalSend( TCommID whichComm ) const override;
      virtual TRecordTime getPhysicalReceive( TCommID whichComm ) const override;

      virtual TRecordTime getLastRecordTime() const override;
      virtual void setFileLoaded( TRecordTime traceEndTime ) override;
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
      static const PRV_UINT32 blockSize = 10000;
      std::vector<PRV_UINT32> currentRecord;
      std::vector<TRecord *> currentBlock;
      std::vector<TLastRecord> lastRecords;
      std::vector<std::vector<TRecord *> > blocks;
      std::vector<TCommInfo *> communications;
      TCommID currentComm;
      const ResourceModel<>& resourceModel;
      const ProcessModel<>& processModel;
      TRecord tmpRecord;
      bool inserted;
      TThreadOrder insertedOnThread;

      friend class PlainTrace;
      friend class PlainTrace::iterator;
      friend class PlainTrace::ThreadIterator;
      friend class PlainTrace::CPUIterator;
  };
}


