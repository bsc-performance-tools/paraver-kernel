#ifndef RECORDLIST_H_INCLUDED
#define RECORDLIST_H_INCLUDED

#include <set>
#include "paraverkerneltypes.h"

using std::set;

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
public:
  TRecordType getType() const
  {
    return type;
  }
  TRecordTime getTime() const
  {
    return time;
  }
  TObjectOrder getOrder() const
  {
    return order;
  }
  TEventType getEventType() const
  {
    return UInfo.event.type;
  }
  TEventType getEventValue() const
  {
    return UInfo.event.value;
  }
  TCommID getCommIndex() const
  {
    return UInfo.comm.id;
  }
private:
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
    typedef set<RLRecord>::iterator iterator;

    virtual ~RecordList() {}

    virtual void clear() = 0;
    virtual iterator begin() const = 0;
    virtual iterator end() const = 0;
};

class RecordListProxy: public RecordList
{
  public:
    RecordListProxy();
    virtual ~RecordListProxy();

    virtual void clear();
    virtual iterator begin() const;
    virtual iterator end() const;
};

#endif // RECORDLIST_H_INCLUDED
