#ifndef INTERVAL_H_INCLUDED
#define INTERVAL_H_INCLUDED

#include <set>
#include "memorytrace.h"
#include "krecordlist.h"

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

    KRecordList *getRecordList()
    {
      return &myDisplayList;
    }

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                              KRecordList *displayList = NULL ) = 0;
    virtual KRecordList *calcNext( KRecordList *displayList = NULL, bool initCalc = false ) = 0;
    virtual KRecordList *calcPrev( KRecordList *displayList = NULL, bool initCalc = false ) = 0;

    virtual KWindow *getWindow() = 0;

  protected:
    TWindowLevel level;
    TObjectOrder order;
    MemoryTrace::iterator *begin;
    MemoryTrace::iterator *end;
    TSemanticValue currentValue;
    KRecordList myDisplayList;
  private:

};


#endif // INTERVAL_H_INCLUDED
