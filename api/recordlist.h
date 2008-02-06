#ifndef RECORDLIST_H_INCLUDED
#define RECORDLIST_H_INCLUDED

#include "paraverkerneltypes.h"

struct RLEvent
{
  TEventType type;
  TEventValue value;
};

struct RLComm
{
  TCommID id;
};

struct RLRecord
{
  TRecordType type;
  TRecordTime time;
  TObjectOrder order;
  union
  {
    RLEvent event;
    RLComm comm;
  } UInfo;
};


class RecordList
{
  public:
    virtual ~RecordList() {}
};

class RecordListProxy: public RecordList
{
  public:
    RecordListProxy();
    virtual ~RecordListProxy();
};

#endif // RECORDLIST_H_INCLUDED
