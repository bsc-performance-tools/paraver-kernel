#include "krecordlist.h"

KRecordList::KRecordList()
{}

KRecordList::~KRecordList()
{}

void KRecordList::clear()
{
  list.clear();
}

void KRecordList::erase( iterator first, iterator last )
{
  list.erase( first, last );
}

KRecordList::iterator KRecordList::begin() const
{
  return list.begin();
}

KRecordList::iterator KRecordList::end() const
{
  return list.end();
}

void KRecordList::insert( MemoryTrace::iterator *it )
{}

