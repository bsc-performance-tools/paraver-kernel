#ifndef INTERVAL_H_INCLUDED
#define INTERVAL_H_INCLUDED

#include <set>
#include "semanticfunction.h"
#include "memorytrace.h"
#include "kwindow.h"

class SemanticFunction;

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

    Interval( KWindow *whichWindow, TObjectOrder whichOrder ):
        window( whichWindow ), order( whichOrder )
    {
      function = NULL;
      begin = NULL;
      end = NULL;
      currentValue = 0;
    }

    virtual ~Interval()
    {
      delete begin;
      delete end;
      RecordList::iterator it = myDisplayList.begin();
      while ( it != myDisplayList.end() )
        delete *it;
      myDisplayList.clear();
    }

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

    virtual RecordList *init( TRecordTime initialTime, RecordList *displayList, TCreateList create ) = 0;
    virtual RecordList *calcNext( RecordList *displayList ) = 0;
    virtual RecordList *calcPrev( RecordList *displayList ) = 0;

  protected:
    KWindow *window;
    TObjectOrder order;
    SemanticFunction *function;
    MemoryTrace::iterator *begin;
    MemoryTrace::iterator *end;
    TSemanticValue currentValue;
    RecordList myDisplayList;
  private:

};


#endif // INTERVAL_H_INCLUDED
