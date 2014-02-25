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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef MEMORYBLOCKS_H_INCLUDED
#define MEMORYBLOCKS_H_INCLUDED

#include "paraverkerneltypes.h"
#include "memorytrace.h"

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

    virtual TData *getLastRecord( PRV_UINT16 position ) const = 0;
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
    virtual void setLogicalSend( TCommID whichComm, TRecordTime whichTime )
    {}
    virtual void setLogicalReceive( TCommID whichComm, TRecordTime whichTime )
    {}
    virtual void setPhysicalSend( TCommID whichComm, TRecordTime whichTime )
    {}
    virtual void setPhysicalReceive( TCommID whichComm, TRecordTime whichTime )
    {}

    // Communication info getters
    virtual TCommID getTotalComms() const = 0;
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

    virtual PRV_UINT32 getCountInserted() const
    {
      return countInserted;
    }

    virtual void resetCountInserted()
    {
      countInserted = 0;
    }

    virtual void setFileLoaded()
    {}

  protected:
    // Number of new records inserted in the last operation
    PRV_UINT32 countInserted;

  private:
};


#endif // MEMORYBLOCKS_H_INCLUDED
