#ifndef INTERVALTHREAD_H_INCLUDED
#define INTERVALTHREAD_H_INCLUDED

#include "interval.h"
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

    IntervalThread( KSingleWindow *whichWindow, TWindowLevel whichLevel,
                    TObjectOrder whichOrder ):
        Interval( whichLevel, whichOrder ), window( whichWindow )
    {
      function = NULL;
    }

    virtual ~IntervalThread()
    {
      if ( begin != NULL )
        delete begin;
      if ( end != NULL )
        delete end;
    }

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                              KRecordList *displayList = NULL );
    virtual KRecordList *calcNext( KRecordList *displayList = NULL, bool initCalc = false );
    virtual KRecordList *calcPrev( KRecordList *displayList = NULL, bool initCalc = false );

    virtual KWindow *getWindow()
    {
      return ( KWindow * ) window;
    }

  protected:
    KSingleWindow *window;
    SemanticThread *function;
    TCreateList createList;

  private:
    virtual MemoryTrace::iterator *getNextRecord( MemoryTrace::iterator *it,
        KRecordList *displayList );
    virtual MemoryTrace::iterator *getPrevRecord( MemoryTrace::iterator *it,
        KRecordList *displayList );

};


#endif // INTERVALTHREAD_H_INCLUDED
