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


#include <fstream>
#include <map>
#include "memoryblocks.h"
#include "resourcemodel.h"
#include "processmodel.h"
#include "index.h"
#include "plaintypes.h"
#include "tracebodyio.h"
#include "tracebodyiofactory.h"
#include "tracestream.h"

using Plain::TRecord;
using Plain::TCommInfo;

namespace NoLoad
{
  class NoLoadBlocks: public MemoryBlocks
  {
    public:
      NoLoadBlocks( const ResourceModel<>& resource, const ProcessModel<>& process )
        : resourceModel( resource ), processModel( process )
      {
        body = nullptr;
        file = nullptr;
      }

      NoLoadBlocks( const ResourceModel<>& resource, const ProcessModel<>& process,
                    TraceBodyIO< PARAM_TRACEBODY_CLASS > *whichBody, TraceStream *whichFile, TRecordTime endTime );

      virtual ~NoLoadBlocks();

      virtual TData *getLastRecord( PRV_UINT16 position ) const override;
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

      virtual void getBeginRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos );
      virtual void getEndRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos );
      virtual void getBeginThreadRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos );
      virtual void getEndThreadRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos );

      // Must be used with TraceBodyIO_v1
      virtual void getNextRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos );
      virtual void getPrevRecord( TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos );

      // Must be used with TraceBodyIO_v2
      virtual void getNextRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos );
      virtual void getPrevRecord( TThreadOrder whichThread, TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos );

      virtual void getThreadRecordByTime( TThreadOrder whichThread, TRecordTime whichTime,
                                          TRecord **record, PRV_INT64& offset, PRV_UINT16& recPos );

      virtual void incNumUseds( PRV_INT64 offset );
      virtual void decNumUseds( PRV_INT64 offset );

      virtual void setFileLoaded( TRecordTime traceEndTime ) override;
      virtual void setFirstOffset( PRV_INT64 whichOffset );

    protected:

    private:
      struct fileLineData
      {
        PRV_INT16 numUseds;
        PRV_INT64 endOffset;
        TThreadOrder thread;
        std::vector<TRecord> records;
      };

      const ResourceModel<>& resourceModel;
      const ProcessModel<>& processModel;
      TraceBodyIO< PARAM_TRACEBODY_CLASS > *body;
      TraceStream *file;
      PRV_INT64 endFileOffset;
      PRV_INT64 initialOffset;

      std::vector<Index<PRV_INT64> > traceIndex;
      std::map<PRV_INT64, fileLineData *> blocks;
      std::vector<PRV_INT64> beginThread;

      TRecord globalBeginRec;
      TRecord globalEndRec;

      std::vector<TRecord> emptyBeginRecords;
      std::vector<TRecord> emptyEndRecords;

      std::vector<TCommInfo *> communications;
      TCommID currentComm;
      TRecord *logSend;
      TRecord *logRecv;
      TRecord *phySend;
      TRecord *phyRecv;

      fileLineData *lastData;
      PRV_INT16 lastRecord;
      PRV_INT64 lastPos;

      bool fileLoaded;
      TRecord loadingRec;
      TThreadOrder loadingThread;

      std::unordered_set<TState> notUsedStates;
      std::unordered_set<TEventType> notUsedEvents;

      MetadataManager dummyTraceInfo;
      TTime dummyEndTime;

      void goToPrevLine();
  };
}


