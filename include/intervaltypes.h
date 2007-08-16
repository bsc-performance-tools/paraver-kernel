#ifndef INTERVALTYPES_H_INCLUDED
#define INTERVALTYPES_H_INCLUDED

#include "memorytrace.h"

// r1 is less than r2?
struct ltrecord
{
  bool operator()( MemoryTrace::iterator *r1, MemoryTrace::iterator *r2 ) const
  {
    if ( r1->getTime() < r2->getTime() )
      return true;
    else if ( r1->getTime() == r2->getTime() )
    {
      if ( r1->getThread() < r2->getThread() )
        return true;
      else if ( r1->getThread() == r2->getThread() )
      {
        if ( ( r1->getType() & COMM ) && ( r2->getType() & EVENT ) )
          return true;
      }
    }
    return false;
  }
};
typedef set<MemoryTrace::iterator *, ltrecord> RecordList;

typedef UINT8 TCreateList;
static const TCreateList NOCREATE = 0x00;
static const TCreateList CREATEEVENTS = 0x01;
static const TCreateList CREATECOMMS = 0x02;

#endif // INTERVALTYPES_H_INCLUDED
