#ifndef INTERVAL_H_INCLUDED
#define INTERVAL_H_INCLUDED

#include <set>
#include "intervaltypes.h"
#include "memorytrace.h"

using namespace std;

class Interval
{
  public:
    Interval()
    {
      begin = NULL;
      end = NULL;
      currentValue = 0;
    }

    Interval( TObjectOrder whichOrder ):
        order( whichOrder )
    {
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

    virtual RecordList *init( TRecordTime initialTime, TCreateList create,
                              RecordList *displayList = NULL ) = 0;
    virtual RecordList *calcNext( RecordList *displayList = NULL, bool initCalc = false ) = 0;
    virtual RecordList *calcPrev( RecordList *displayList = NULL, bool initCalc = false ) = 0;

  protected:
    TObjectOrder order;
    MemoryTrace::iterator *begin;
    MemoryTrace::iterator *end;
    TSemanticValue currentValue;
    RecordList myDisplayList;
  private:

};


#endif // INTERVAL_H_INCLUDED
