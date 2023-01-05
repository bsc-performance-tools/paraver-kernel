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
#include "vectortrace.h"
#include "vectortracedefines.h"

#include "utils/traceparser/processmodel.h"
#include "utils/traceparser/resourcemodel.h"
#include "utils/traceparser/tracetypes.h"


class VectorBlocks : public MemoryBlocks
{
  public:
    VectorBlocks( const ResourceModel<>& resource, const ProcessModel<>& process,
                  TRecordTime endTime );

    virtual TData *getLastRecord( PRV_UINT16 position ) const override;
    virtual void newRecord() override;
    virtual void newRecord( TThreadOrder whichThread ) override;
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
    virtual void newComm( TThreadOrder whichSenderThread, TThreadOrder whichReceiverThread, bool createRecords = true ) override;
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
    
  private:
    std::vector< TThreadRecordContainer > threadRecords;
    std::vector< TCPURecordContainer > cpuRecords;

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
    static const TRecordType commTypes[ commTypeSize ];
    std::vector<Plain::TCommInfo> communications;
    std::array<Plain::TRecord*, commTypeSize> commRecords;

    const ResourceModel<>& resourceModel;
    const ProcessModel<>& processModel;

    TThreadOrder insertedOnThread;
    TRecordTime lastRecordTime = 0;

    friend class VectorTrace;
    friend class VectorTrace::iterator;

};