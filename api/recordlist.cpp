#include "recordlist.h"

RecordList *RecordList::create( RecordList *whichList )
{
  return new RecordListProxy( whichList );
}

RecordListProxy::RecordListProxy( RecordList *whichList )
    : myRecordList( whichList )
{}

void RecordListProxy::clear()
{
  myRecordList->clear();
}

void RecordListProxy::erase( iterator first, iterator last )
{
  myRecordList->erase( first, last );
}

RecordListProxy::iterator RecordListProxy::begin() const
{
  return myRecordList->begin();
}

RecordListProxy::iterator RecordListProxy::end() const
{
  return myRecordList->end();
}

bool RecordListProxy::newRecords() const
{
  return myRecordList->newRecords();
}


