#include "krecordlist.h"
#include "paraverkernelexception.h"

KRecordList::KRecordList()
{
  newRec = false;
}

KRecordList::~KRecordList()
{}

void KRecordList::clear()
{
  list.clear();
  newRec = false;
}

void KRecordList::erase( iterator first, iterator last )
{
  list.erase( first, last );
  newRec = false;
}

KRecordList::iterator KRecordList::begin() const
{
  return list.begin();
}

KRecordList::iterator KRecordList::end() const
{
  return list.end();
}

bool KRecordList::newRecords() const
{
  return newRec;
}

void KRecordList::insert( MemoryTrace::iterator *it )
{
  RLRecord tmp;
  tmp.setType( it->getType() );
  tmp.setTime( it->getTime() );
  tmp.setOrder( it->getOrder() );
  if ( tmp.getType() & EVENT )
  {
    tmp.setEventType( it->getEventType() );
    tmp.setEventValue( it->getEventValue() );
  }
  else if ( tmp.getType() & COMM )
    tmp.setCommIndex( it->getCommIndex() );
  else
    throw ParaverKernelException();

  list.insert( tmp );

  newRec = true;
}

