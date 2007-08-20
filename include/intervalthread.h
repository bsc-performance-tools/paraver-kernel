#ifndef INTERVALTHREAD_H_INCLUDED
#define INTERVALTHREAD_H_INCLUDED

#include "interval.h"
#include "kwindow.h"
#include "semanticthread.h"

class KSingleWindow;
class SemanticThread;

class IntervalThread: public Interval
{
  public:
    IntervalThread()
    {
      begin = NULL;
      end = NULL;
      function = NULL;
    }

    IntervalThread( KSingleWindow *whichWindow, TObjectOrder whichOrder ):
        Interval( whichOrder ), window( whichWindow )
    {
      function = NULL;
    }

    virtual ~IntervalThread()
    {
      if( begin != NULL )
        delete begin;
      if( end != NULL )
        delete end;
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
    virtual void getNextRecord( MemoryTrace::iterator *it,
                                RecordList *displayList );
    virtual void getPrevRecord( MemoryTrace::iterator *it,
                                RecordList *displayList );

};


#endif // INTERVALTHREAD_H_INCLUDED
