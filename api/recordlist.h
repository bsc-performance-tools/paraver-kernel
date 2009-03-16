#ifndef RECORDLIST_H_INCLUDED
#define RECORDLIST_H_INCLUDED

#include <set>
#include "paraverkerneltypes.h"

class KernelConnection;

using std::set;

struct RLEvent
{
  TEventType type;
  TEventValue value;
};

struct RLComm
{
  TObjectOrder partnerObject;
  TRecordTime partnerTime;
  TCommSize size;
  TCommTag tag;
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
  TEventValue getEventValue() const
  {
    return UInfo.event.value;
  }
  TObjectOrder getCommPartnerObject() const
  {
    return UInfo.comm.partnerObject;
  }
  TRecordTime getCommPartnerTime() const
  {
    return UInfo.comm.partnerTime;
  }
  TCommSize getCommSize() const
  {
    return UInfo.comm.size;
  }
  TCommTag getCommTag() const
  {
    return UInfo.comm.tag;
  }
  void setType( TRecordType whichType )
  {
    type = whichType;
  }
  void setTime( TRecordTime whichTime )
  {
    time = whichTime;
  }
  void setOrder( TObjectOrder whichOrder )
  {
    order = whichOrder;
  }
  void setEventType( TEventType whichType )
  {
    UInfo.event.type = whichType;
  }
  void setEventValue( TEventValue whichValue )
  {
    UInfo.event.value = whichValue;
  }
  void setCommPartnerObject( TObjectOrder whichOrder )
  {
    UInfo.comm.partnerObject = whichOrder;
  }
  void setCommPartnerTime( TRecordTime whichTime )
  {
    UInfo.comm.partnerTime = whichTime;
  }
  void setCommSize( TCommSize whichSize )
  {
    UInfo.comm.size = whichSize;
  }
  void setCommTag( TCommTag whichTag )
  {
    UInfo.comm.tag = whichTag;
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

// r1 is less than r2?
struct ltrecord
{
  bool operator()( const RLRecord& r1, const RLRecord& r2 )
  {
    if ( r1.getTime() < r2.getTime() )
      return true;
    else if ( r1.getTime() == r2.getTime() )
    {
      if ( r1.getOrder() < r2.getOrder() )
        return true;
      else if ( r1.getOrder() == r2.getOrder() )
      {
        if ( ( r1.getType() & COMM ) && ( r2.getType() & EVENT ) )
          return true;
      }
    }
    return false;
  }
};


class RecordList
{
  public:
    typedef set<RLRecord,ltrecord>::iterator iterator;

    static RecordList *create( RecordList *whichList );

    virtual ~RecordList() {}

    virtual void clear() = 0;
    virtual void erase( iterator first, iterator last ) = 0;
    virtual RecordList::iterator begin() = 0;
    virtual RecordList::iterator end() = 0;
    virtual bool newRecords() const = 0;
    virtual RecordList *clone()
    {
      return NULL;
    };
};

class RecordListProxy: public RecordList
{
  public:
    virtual ~RecordListProxy() {};

    virtual void clear();
    virtual void erase( iterator first, iterator last );
    virtual RecordList::iterator begin();
    virtual RecordList::iterator end();
    virtual bool newRecords() const;
    virtual RecordList *clone();

  private:
    RecordListProxy( RecordList *whichList );

    RecordList *myRecordList;

    friend RecordList *RecordList::create( RecordList * );
};

#endif // RECORDLIST_H_INCLUDED
