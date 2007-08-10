#ifndef INTERVALTHREAD_H_INCLUDED
#define INTERVALTHREAD_H_INCLUDED

#include "interval.h"
#include "semanticthread.h"

class IntervalThread: public Interval
{
  public:
    IntervalThread()
    {
      function = NULL;
    }

    IntervalThread( KSingleWindow *whichWindow, TObjectOrder whichOrder ):
        Interval( whichOrder ), window( whichWindow )
    {
      function = NULL;
    }

    SemanticThread *getSemanticFunction() const
    {
      return function;
    }

    void setSemanticFunction( SemanticThread *whichFunction )
    {
      function = whichFunction;
    }

    virtual RecordList *init( TRecordTime initialTime, TCreateList create,
                              RecordList *displayList = NULL );
    virtual RecordList *calcNext( RecordList *displayList = NULL, bool initCalc = false );
    virtual RecordList *calcPrev( RecordList *displayList = NULL, bool initCalc = false );

  protected:
    KSingleWindow *window;
    SemanticThread *function;
    TCreateList createList;

  private:
    TSemanticValue nextValue;

    virtual void getNextRecord( MemoryTrace::iterator *it,
                                RecordList *displayList );
    virtual void getPrevRecord( MemoryTrace::iterator *it,
                                RecordList *displayList );

};


#endif // INTERVALTHREAD_H_INCLUDED
