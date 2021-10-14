/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/


#pragma once


#include <set>
#include "paraverkerneltypes.h"

class KernelConnection;

struct RLEvent
{
  TEventType type;
  TSemanticValue value;
};

struct RLComm
{
  TObjectOrder partnerObject;
  TRecordTime partnerTime;
  TCommSize size;
  TCommTag tag;
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
  TSemanticValue getEventValue() const
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
  TCommID getCommId() const
  {
    return UInfo.comm.id;
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
  void setEventValue( TSemanticValue whichValue )
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
  void setCommId( TCommID whichID )
  {
    UInfo.comm.id = whichID;
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
  bool operator()( const RLRecord& r1, const RLRecord& r2 ) const
  {
    if ( r1.getTime() < r2.getTime() )
      return true;
/*    else if ( r1.getTime() == r2.getTime() )
    {
      if ( r1.getOrder() < r2.getOrder() )
        return true;
      else if ( r1.getOrder() == r2.getOrder() )
      {
        if ( ( r1.getType() & COMM ) && ( r2.getType() & EVENT ) )
          return true;
        else if ( r1.getType() == r2.getType() && ( r1.getType() & COMM ) )
        {
          if ( r1.getCommId() != r2.getCommId() )
            return true;
        }
        else if ( r1.getType() == r2.getType() && ( r1.getType() & EVENT ) )
        {
          if ( r1.getEventType() != r2.getEventType() )
            return true;
          else if ( r1.getEventValue() != r2.getEventValue() )
            return true;
        }
      }
    }*/
    return false;
  }
};


class RecordList
{
  public:
    typedef std::multiset<RLRecord, ltrecord>::iterator iterator;

    static RecordList *create( RecordList *whichList );

    virtual ~RecordList() {}

    virtual void clear() = 0;
    virtual void erase( iterator first, iterator last ) = 0;
    virtual RecordList::iterator begin() = 0;
    virtual RecordList::iterator end() = 0;
    virtual bool newRecords() const = 0;
    virtual RecordList *clone()
    {
      return nullptr;
    };
};

class RecordListProxy: public RecordList
{
  public:
    virtual ~RecordListProxy() {};

    virtual void clear() override;
    virtual void erase( iterator first, iterator last ) override;
    virtual RecordList::iterator begin() override;
    virtual RecordList::iterator end() override;
    virtual bool newRecords() const override;
    virtual RecordList *clone() override;

  private:
    RecordListProxy( RecordList *whichList );

    RecordList *myRecordList;

    friend RecordList *RecordList::create( RecordList * );
};


