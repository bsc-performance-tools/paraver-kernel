#ifndef PLAINTYPES_H_INCLUDED
#define PLAINTYPES_H_INCLUDED

#include "paraverkerneltypes.h"
#include "memorytrace.h"

namespace Plain
{
  typedef struct TEventRecord
  {
    TEventType      type;
    TEventValue     value;
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


  typedef struct TRecord : public TData
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
  }
  TRecord;

  static inline TRecordTime getTime( TRecord *record )
  {
    return record->time;
  }

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
}

#endif // PLAINTYPES_H_INCLUDED
