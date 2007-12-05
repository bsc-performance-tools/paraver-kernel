#ifndef INTERVAL_H_INCLUDED
#define INTERVAL_H_INCLUDED

#include <set>
#include "intervaltypes.h"
#include "memorytrace.h"

class KWindow;

using namespace std;

class Interval
{
  public:
    Interval()
    {
      begin = NULL;
      end = NULL;
      currentValue = 0.0;
    }

    Interval( TWindowLevel whichLevel, TObjectOrder whichOrder ):
        level( whichLevel ), order( whichOrder )
    {
      begin = NULL;
      end = NULL;
      currentValue = 0.0;
    }

    virtual ~Interval()
    {
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

    MemoryTrace::iterator *getBegin() const
    {
      return begin;
    }

    MemoryTrace::iterator *getEnd() const
    {
      return end;
    }

    TWindowLevel getLevel()
    {
      return level;
    }

    TObjectOrder getOrder()
    {
      return order;
    }

    RecordList *getRecordList()
    {
      return &myDisplayList;
    }

    virtual RecordList *init( TRecordTime initialTime, TCreateList create,
                              RecordList *displayList = NULL ) = 0;
    virtual RecordList *calcNext( RecordList *displayList = NULL, bool initCalc = false ) = 0;
    virtual RecordList *calcPrev( RecordList *displayList = NULL, bool initCalc = false ) = 0;

    virtual KWindow *getWindow() = 0;

  protected:
    TWindowLevel level;
    TObjectOrder order;
    MemoryTrace::iterator *begin;
    MemoryTrace::iterator *end;
    TSemanticValue currentValue;
    RecordList myDisplayList;
  private:

};


#endif // INTERVAL_H_INCLUDED
