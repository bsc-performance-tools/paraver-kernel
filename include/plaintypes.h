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


#include "paraverkerneltypes.h"
#include "memorytrace.h"

namespace Plain
{
  typedef struct TEventRecord
  {
    TEventValue     value;
    TEventType      type;
  }
  TEventRecord;


  typedef struct TStateRecord
  {
    TRecordTime endTime;
    TState      state;
  }
  TStateRecord;


  typedef struct TCommRecord
  {
    TCommID index;
  }
  TCommRecord;


  typedef struct TCommInfo
  {
    TRecordTime   logicalSendTime;
    TRecordTime   physicalSendTime;
    TRecordTime   logicalReceiveTime;
    TRecordTime   physicalReceiveTime;
    TCommTag      tag;
    TCommSize     size;
    TCPUOrder     senderCPU;
    TThreadOrder  senderThread;
    TCPUOrder     receiverCPU;
    TThreadOrder  receiverThread;
  }
  TCommInfo;


  typedef struct TRecord : public TData
  {
    TRecordTime  time;
    TRecordType  type;
    TThreadOrder thread; // Needed by trace edit sequence
    TCPUOrder    CPU;
    union
    {
      TStateRecord stateRecord;
      TEventRecord eventRecord;
      TCommRecord  commRecord;
    } URecordInfo;
  }
  TRecord;

  static inline TRecordTime getTime( const TRecord *record )
  {
    return record->time;
  }

  static inline bool isComm( const TRecord *record )
  {
    return record->type & COMM;
  }

  static inline bool isEvent( const TRecord *record )
  {
    return record->type & EVENT;
  }

  static inline bool isState( const TRecord *record )
  {
    return record->type & STATE;
  }

  static inline bool isSend( const TRecord *record )
  {
    return record->type & SEND;
  }

  static inline bool isReceive( const TRecord *record )
  {
    return record->type & RECV;
  }

  static inline bool isLogical( const TRecord *record )
  {
    return record->type & LOG;
  }

  static inline bool isPhysical( const TRecord *record )
  {
    return record->type & PHY;
  }

  static inline bool isRReceive( const TRecord *record )
  {
    return record->type & RRECV;
  }

  static inline bool isRSend( const TRecord *record )
  {
    return record->type & RSEND;
  }

  static inline bool isGlobComm( const TRecord *record )
  {
    return record->type & GLOBCOMM;
  }

  static inline bool isEnd( const TRecord *record )
  {
    return record->type & END;
  }

  static inline PRV_UINT16 getTypeOrdered( const TRecord *r )
  {
    PRV_UINT16 ret;

    if ( isEvent( r ) )
      ret = 6;
    else if ( isState( r ) )
    {
      if ( isEnd( r ) )
        ret = 0;
      else
        ret = 8;
    }
    else if ( isPhysical( r ) )
    {
      if ( isReceive( r ) )
        ret = 1;
      else // isSend( r )
        ret = 5;
    }
    else if ( isLogical( r ) )
    {
      if ( isSend( r ) )
        ret = 4;
      else // isReceive( r ) )
        ret = 6;
    }
    else if ( isRReceive( r ) )
      ret  = 2;
    else if ( isRSend( r ) )
      ret  = 3;
    else if ( isGlobComm( r ) )
      ret = 7;
    else
      ret = 9;

    return ret;
  }

}


