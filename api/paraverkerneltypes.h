#ifndef PARAVERKERNELTYPES_H_INCLUDED
#define PARAVERKERNELTYPES_H_INCLUDED

#include <vector>
#include "types.h"

using namespace std;

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

// Semantic function definitions
typedef UINT16 TParamIndex;
typedef vector<double> TParamValue;

// Semantic record list definitions
typedef UINT16 TCreateList;
static const TCreateList NOCREATE = 0x00;
static const TCreateList CREATEEVENTS = 0x01;
static const TCreateList CREATECOMMS = 0x02;

// Histogram definitions
typedef double THistogramLimit;
typedef UINT32 THistogramColumn;

typedef enum THistoTotals
{
  TOTAL = 0, AVERAGE, MAXIMUM, MINIMUM, STDEV, AVGDIVMAX, NUMTOTALS
};

typedef enum SemanticInfoType
{
  NO_TYPE = 0, // without units
  SAME_TYPE, // same type as previous function
  OBJECT_TYPE, // window same level object
  APPL_TYPE,
  TASK_TYPE,
  THREAD_TYPE,
  NODE_TYPE,
  CPU_TYPE,
  TIME_TYPE,
  STATE_TYPE,
  EVENTTYPE_TYPE,
  EVENTVALUE_TYPE,
  COMMSIZE_TYPE,
  COMMTAG_TYPE,
  BANDWIDTH_TYPE,
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
