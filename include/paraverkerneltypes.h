#ifndef PARAVERKERNELTYPES_H_INCLUDED
#define PARAVERKERNELTYPES_H_INCLUDED

#include "types.h"

typedef UINT16        TObjectOrder;

typedef TObjectOrder  TThreadOrder;
typedef TObjectOrder  TTaskOrder;
typedef TObjectOrder  TApplOrder;

typedef TObjectOrder  TNodeOrder;
typedef TObjectOrder  TCPUOrder;

typedef INT64         TTime;
typedef TTime         TRecordTime;
typedef UINT8         TTimeUnit;
/*
 *  TTimeUnit defines
 */
static const TTimeUnit NS   = 0x00;
static const TTimeUnit US   = 0x01;
static const TTimeUnit MS   = 0x02;
static const TTimeUnit SEC  = 0x04;
static const TTimeUnit MIN  = 0x08;
static const TTimeUnit HOUR = 0x10;
static const TTimeUnit DAY  = 0x20;


typedef UINT16        TRecordType;
/*
 * TRecordType defines.
 *  Comes from old paraver code.
 */
static const TRecordType BEGIN    = 0x0000;
static const TRecordType END      = 0x0001;
static const TRecordType STATE    = 0x0002;
static const TRecordType EVENT    = 0x0004;
static const TRecordType LOG      = 0x0008;
static const TRecordType PHY      = 0x0010;
static const TRecordType SEND     = 0x0020;
static const TRecordType RECV     = 0x0040;
static const TRecordType COMM     = 0x0080;
static const TRecordType GLOBCOMM = 0x0100;
static const TRecordType RRECV    = 0x0200;
static const TRecordType RSEND    = 0x0400;

static const TRecordType EMPTYREC = STATE + EVENT + COMM;
#if 0
#define BEGIN        (0x0000)          /* Begin record. */
#define END          (0x0001)          /* End record. Used in states
record and communication records. */
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

#define EMPTYREC     STATE + EVENT + COMM /* Empty Record. */
#endif

typedef UINT64        TCommID;
typedef INT64         TCommSize;
typedef INT64         TCommTag;
typedef UINT32        TEventType;
typedef UINT64        TEventValue;
typedef UINT32        TState;

typedef double        TSemanticValue;

typedef enum TWindowLevel
{
  WORKLOAD = 0, APPLICATION, TASK, THREAD,
  SYSTEM, NODE, CPU
};


#endif // PARAVERKERNELTYPES_H_INCLUDED
