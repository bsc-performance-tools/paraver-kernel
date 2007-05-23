#ifndef PARAVERKERNELTYPES_H_INCLUDED
#define PARAVERKERNELTYPES_H_INCLUDED

#include "types.h"

typedef UINT16        TObjectOrder;

typedef TObjectOrder  TThreadOrder;
typedef TObjectOrder  TTaskOrder;
typedef TObjectOrder  TApplOrder;

typedef TObjectOrder  TNodeOrder;
typedef TObjectOrder  TCPUOrder;

typedef UINT64        TTime;
typedef TTime         TRecordTime;
typedef UINT8         TTimeUnit;
/*
 *  TTimeUnit defines
 */
#define NS            (0x00)
#define US            (0x01)
#define MS            (0x02)
#define SEC           (0x04)
#define MIN           (0x08)
#define HOUR          (0x10)
#define DAY           (0x20)


typedef UINT16        TRecordType;
/*
 * TRecordType defines.
 */
#define BEGIN        (0x0000)          /* Begin record. */
#define END          (0x0001)          /* End record. Used in states record
                                          and communication records. */
#define STATE        (0x0002)          /* State record. */
#define EVENT        (0x0004)          /* Event record. */
#define LOG          (0x0008)          /* Logical communication. */
#define PHY          (0x0010)          /* Physical communication. */
#define SEND         (0x0020)          /* Communication send. */
#define RECV         (0x0040)          /* Communication receive. */
#define COMM         (0x0080)          /* Communication record. */
#define GLOBCOMM     (0x0100)          /* Global Communication record. */
#define RRECV        (0x0200)          /* Partner receive. */
#define RSEND        (0x0400)          /* Partner send. */

#define EMPTYREC     (0x0800)          /* Empty Record. */

typedef UINT64        TCommID;
typedef INT64         TCommSize;
typedef INT64         TCommTag;
typedef UINT32        TEventType;
typedef INT64         TEventValue;
typedef UINT32        TState;

typedef struct TEventRecord
{
  TEventType      type;
  TEventValue     value;
  struct TRecord *nextEv;
}TEventRecord;

typedef struct TStateRecord
{
  TState      state;
  TRecordTime end_time;
}TStateRecord;

typedef struct TCommRecord
{
  TCommID info;
}TCommRecord;


#endif // PARAVERKERNELTYPES_H_INCLUDED
