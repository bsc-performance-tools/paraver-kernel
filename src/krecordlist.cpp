#include "krecordlist.h"

KRecordList::KRecordList()
{}

KRecordList::~KRecordList()
{}

void KRecordList::clear()
{
  list.clear();
}

KRecordList::iterator KRecordList::begin() const
{
  return list.begin();
}

KRecordList::iterator KRecordList::end() const
{
  return list.end();
}

void insert( MemoryTrace::iterator *it )
{}

