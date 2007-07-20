#ifndef BPLUSTREETYPES_H_INCLUDED
#define BPLUSTREETYPES_H_INCLUDED

#include "paraverkerneltypes.h"

namespace bplustree
{
  typedef struct TEventRecord
  {
    TEventType      type;
    TEventValue     value;
//  struct TRecord *nextEvent;
  }
  TEventRecord;


  typedef struct TStateRecord
  {
    TState      state;
    TRecordTime endTime;
  }
  TStateRecord;


  typedef struct TCommRecord
  {
    TCommID index;
  }
  TCommRecord;


  typedef struct TCommInfo
  {
    TCommTag      tag;
    TCommSize     size;
    TCPUOrder     senderCPU;
    TThreadOrder  senderThread;
    TCPUOrder     receiverCPU;
    TThreadOrder  receiverThread;
    TRecordTime   logicalSendTime;
    TRecordTime   physicalSendTime;
    TRecordTime   logicalReceiveTime;
    TRecordTime   physicalReceiveTime;
  }
  TCommInfo;


  typedef struct TRecord
  {
    TRecordType  type;
    TRecordTime  time;
    TThreadOrder thread;
    TCPUOrder    CPU;
    union
    {
      TStateRecord stateRecord;
      TEventRecord eventRecord;
      TCommRecord  commRecord;
    } URecordInfo;
    TRecord *next, *prev;
    TRecord *threadNext, *threadPrev;
  }
  TRecord;

  static inline TRecordTime getTime( TRecord *record )
  {
    return record->time;
  }
//#define getTime(record) (record->time)

  /*
  if (Is_state_end(r))
    ret  = 0;
  else if (Is_physical_receive(r))
    ret  = 1;
  else if (isRReceive(r))
    ret  = 2;
  else if (isRSend(r))
    ret  = 3;
  else if (Is_logical_send(r))
    ret  = 4;
  else if (Is_physical_send(r))
    ret  = 5;
  else if (Is_logical_receive(r) || isEvent(r))
    ret  = 6;
  else if (isGlobComm(r))
    ret  = 7;
  else if (Is_state(r) && !isEnd(r))
    ret  = 8;
  else
    ret  = 9;
  */
  static inline bool isComm( TRecord *record )
  {
    return record->type & COMM;
  }

  static inline bool isEvent( TRecord *record )
  {
    return record->type & EVENT;
  }

  static inline bool isState( TRecord *record )
  {
    return record->type & STATE;
  }

  static inline bool isSend( TRecord *record )
  {
    return record->type & SEND;
  }

  static inline bool isReceive( TRecord *record )
  {
    return record->type & RECV;
  }

  static inline bool isLogical( TRecord *record )
  {
    return record->type & LOG;
  }

  static inline bool isPhysical( TRecord *record )
  {
    return record->type & PHY;
  }

  static inline bool isRReceive( TRecord *record )
  {
    return record->type & RRECV;
  }

  static inline bool isRSend( TRecord *record )
  {
    return record->type & RSEND;
  }

  static inline bool isGlobComm( TRecord *record )
  {
    return record->type & GLOBCOMM;
  }

  static inline bool isEnd( TRecord *record )
  {
    return record->type & END;
  }

  static inline UINT8 getTypeOrdered( TRecord *r )
  {
    UINT8 ret;

    if ( isState( r ) )
    {
      if ( isEnd( r ) )
        ret = 0;
      else
        ret = 8;
    }
    else if ( isEvent( r ) )
      ret = 6;
    else if ( isComm( r ) )
    {
      if ( isLogical( r ) )
      {
        if ( isSend( r ) )
          ret = 4;
        else
          ret = 6;
      }
      else
      {
        if ( isSend( r ) )
          ret = 5;
        else
          ret = 1;
      }
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
  /*
  #define getTypeOrdered(r) \
    { \
      UINT8 ret; \
      \
      if (Is_state(r)) \
      { \
        if (isEnd(r)) \
          ret = 0; \
        else \
          ret = 8; \
      } \
      else if (isEvent(r)) \
        ret = 6; \
      else if (isComm(r)) \
      { \
        if (isLogical(r)) \
        { \
          if (isSend(r)) \
            ret = 4; \
          else \
            ret = 6; \
        } \
        else \
        { \
          if (isSend(r)) \
            ret = 5; \
          else \
            ret = 1; \
        } \
      } \
      else if (isRReceive(r)) \
        ret  = 2; \
      else if (isRSend(r)) \
        ret  = 3; \
      else if (isGlobComm(r)) \
        ret = 7; \
      else \
        ret = 9; \
    }
    */

}

#endif // BPLUSTREETYPES_H_INCLUDED
