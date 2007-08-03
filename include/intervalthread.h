#ifndef INTERVALTHREAD_H_INCLUDED
#define INTERVALTHREAD_H_INCLUDED

#include "interval.h"
#include "semanticthread.h"

class IntervalThread: public Interval
{
  public:
    virtual RecordList *init( TRecordTime initialTime, RecordList *displayList,
                              TCreateList create );
    virtual RecordList *calcNext( RecordList *displayList, bool initCalc = false );
    virtual RecordList *calcPrev( RecordList *displayList, bool initCalc = false );

  protected:
    KSingleWindow *window;
    SemanticThread *function;
    MemoryTrace::iterator *begin;
    MemoryTrace::iterator *end;
    TCreateList createList;
  private:
    TSemanticValue nextValue;

    virtual void getNextRecord( MemoryTrace::iterator *it,
                                RecordList *displayList );
    virtual void getPrevRecord( MemoryTrace::iterator *it,
                                RecordList *displayList );

};


#endif // INTERVALTHREAD_H_INCLUDED
