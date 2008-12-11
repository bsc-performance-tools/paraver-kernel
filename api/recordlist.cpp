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

RecordList::iterator RecordListProxy::begin()
{
  return myRecordList->begin();
}

RecordList::iterator RecordListProxy::end()
{
  return myRecordList->end();
}

bool RecordListProxy::newRecords() const
{
  return myRecordList->newRecords();
}


