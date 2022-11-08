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

#include <vector>

#include "memoryblocks.h"
#include "plaintypes.h"

class VectorBlocks : public MemoryBlocks
{
  public:
    virtual TData *getLastRecord( PRV_UINT16 position ) const;
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

  private:
    std::vector< std::vector<Plain::TRecord> > threadRecords;
    std::vector< std::vector<Plain::TRecord *> > cpuRecords;

};