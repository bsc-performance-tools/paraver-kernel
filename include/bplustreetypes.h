#ifndef BPLUSTREETYPES_H_INCLUDED
#define BPLUSTREETYPES_H_INCLUDED

#include "paraverkerneltypes.h"

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

typedef struct TRecord
{
  TRecordType type;
  TRecordTime time;
  union
  {
    struct TStateRecord stateRec;
    struct TEventRecord eventRec;
    struct TCommRecord  commRec;
  } URecordInfo;
  struct TRecord *next, *prev;
  struct TRecord *threadNext, *threadPrev;
}TRecord;

#define bPlusTreeGetTime(record) (record->time)

  /*
  if (Is_state_end(r))
    ret  = 0;
  else if (Is_physical_receive(r))
    ret  = 1;
  else if (Is_rreceive(r))
    ret  = 2;
  else if (Is_rsend(r))
    ret  = 3;
  else if (Is_logical_send(r))
    ret  = 4;
  else if (Is_physical_send(r))
    ret  = 5;
  else if (Is_logical_receive(r) || Is_event(r))
    ret  = 6;
  else if (Is_globcomm(r))
    ret  = 7;
  else if (Is_state(r) && !Is_end(r))
    ret  = 8;
  else
    ret  = 9;
  */
#define bPlusTreeGetTypeOrdered(r) \
{ \
  UINT8 ret; \
\
  if (Is_state(r)) \
  { \
    if (Is_end(r)) \
      ret = 0; \
    else \
      ret = 8; \
  } \
  else if (Is_event(r)) \
    ret = 6; \
  else if (Is_comm(r)) \
  { \
    if (Is_logical(r)) \
    { \
      if (Is_send(r)) \
        ret = 4; \
      else \
        ret = 6; \
    } \
    else \
    { \
      if (Is_send(r)) \
        ret = 5; \
      else \
        ret = 1; \
    } \
  } \
  else if (Is_rreceive(r)) \
    ret  = 2; \
  else if (Is_rsend(r)) \
    ret  = 3; \
  else if (Is_globcomm(r)) \
    ret = 7; \
  else \
    ret = 9; \
}





#endif // BPLUSTREETYPES_H_INCLUDED
