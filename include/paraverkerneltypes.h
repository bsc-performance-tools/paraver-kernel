#ifndef PARAVERKERNELTYPES_H_INCLUDED
#define PARAVERKERNELTYPES_H_INCLUDED

#include "types.h"

typedef UINT16        TObjectOrder;

typedef TObjectOrder  TThreadOrder;
typedef TObjectOrder  TTaskOrder;
typedef TObjectOrder  TApplOrder;

typedef TObjectOrder  TNodeOrder;
typedef TObjectOrder  TCPUOrder;

typedef double        TTime;
typedef TTime         TRecordTime;
typedef UINT16         TTimeUnit;
/*
 *  TTimeUnit defines
 */
static const TTimeUnit NS   = 0;
static const TTimeUnit US   = 1;
static const TTimeUnit MS   = 2;
static const TTimeUnit SEC  = 3;
static const TTimeUnit MIN  = 4;
static const TTimeUnit HOUR = 5;
static const TTimeUnit DAY  = 6;

static const TRecordTime factorTable[ DAY + 1 ] =
{
  1, // ns
  1E3, // us
  1E3, // ms
  1E3, // seconds
  60, // minutes
  60, // hours
  24 // days
};


typedef UINT16        TRecordType;
/*
 * TRecordType defines.
 *  Comes from old paraver code.
 */
static const TRecordType BEGIN    = 0x0001;
static const TRecordType END      = 0x0002;
static const TRecordType STATE    = 0x0004;
static const TRecordType EVENT    = 0x0008;
static const TRecordType LOG      = 0x0010;
static const TRecordType PHY      = 0x0020;
static const TRecordType SEND     = 0x0040;
static const TRecordType RECV     = 0x0080;
static const TRecordType COMM     = 0x0100;
static const TRecordType GLOBCOMM = 0x0200;
static const TRecordType RRECV    = 0x0400;
static const TRecordType RSEND    = 0x0800;

static const TRecordType EMPTYREC = STATE + EVENT + COMM;
// Paraver old code
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
  NONE = 0,
  WORKLOAD, APPLICATION, TASK, THREAD,
  SYSTEM, NODE, CPU,
  // Compose levels (for setting the window functions only)
  TOPCOMPOSE1, TOPCOMPOSE2, COMPOSEWORKLOAD, COMPOSEAPPLICATION, COMPOSETASK,
  COMPOSETHREAD, COMPOSESYSTEM, COMPOSENODE, COMPOSECPU,
  // Derived window levels
  DERIVED
};

/*
 * PARAVER OLD CODE
 * Record class field defines for a state record.
 */

#define IDLE            0
#define RUNNING         1
#define STOPED          2
#define WAIT_MESS       3
#define WAIT_LINK       4
#define WAIT_CPU        5
#define WAIT_SEM        6
#define OVERHEAD        7
#define PROBE           8
#define SEND_OVH        9
#define RECV_OVH       10
#define STATE_IO       12

#endif // PARAVERKERNELTYPES_H_INCLUDED
