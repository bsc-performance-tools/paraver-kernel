#ifndef INTERVAL_H_INCLUDED
#define INTERVAL_H_INCLUDED

#include <set>
#include "semanticfunction.h"
#include "memorytrace.h"
#include "kwindow.h"

using namespace std;

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

class Interval
{
  public:
    Interval()
    {
      window = NULL;
      function = NULL;
      begin = NULL;
      end = NULL;
      currentValue = 0;
    }

    virtual ~Interval()
    {}

    SemanticFunction *getSemanticFunction() const
    {
      return function;
    }

    void setSemanticFunction( SemanticFunction *whichFunction )
    {
      function = whichFunction;
    }

    TRecordTime getBeginTime() const
    {
      return begin->getTime();
    }

    TRecordTime getEndTime() const
    {
      return end->getTime();
    }

    TSemanticValue getValue() const
    {
      return currentValue;
    }

    virtual void init( TRecordTime initialTime, RecordList *displayList,  TCreateList create ) = 0;
    virtual RecordList *calcNext( RecordList *displayList ) = 0;
    virtual RecordList *calcPrev( RecordList *displayList ) = 0;

  protected:
    KWindow *window;
    SemanticFunction *function;
    MemoryTrace::iterator *begin;
    MemoryTrace::iterator *end;
    TSemanticValue currentValue;
    RecordList displayRecords;

  private:

};


#endif // INTERVAL_H_INCLUDED
