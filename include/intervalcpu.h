#ifndef INTERVALCPU_H_INCLUDED
#define INTERVALCPU_H_INCLUDED

#include "intervalhigh.h"
#include "semanticcpu.h"
#include "semanticthread.h"

class KSingleWindow;
class SemanticCPU;

class IntervalCPU: public IntervalHigh
{
  public:
    IntervalCPU()
    {
      begin = NULL;
      end = NULL;
      function = NULL;
    }

    IntervalCPU( KSingleWindow *whichWindow, TWindowLevel whichLevel,
                 TObjectOrder whichOrder ):
        IntervalHigh( whichLevel, whichOrder ), window( whichWindow )
    {
      begin = NULL;
      end = NULL;
      function = NULL;
    }

    virtual ~IntervalCPU()
    {
      if ( begin != NULL )
        delete begin;
      if ( end != NULL )
        delete end;
    }

    virtual RecordList *init( TRecordTime initialTime, TCreateList create,
                              RecordList *displayList = NULL );
    virtual RecordList *calcNext( RecordList *displayList = NULL, bool initCalc = false );
    virtual RecordList *calcPrev( RecordList *displayList = NULL, bool initCalc = false );

    virtual KWindow *getWindow()
    {
      return ( KWindow * ) window;
    }

  protected:
    KSingleWindow *window;
    SemanticCPU *function;
    SemanticThread *functionThread;
    TCreateList createList;

  private:
    virtual MemoryTrace::iterator *getNextRecord( MemoryTrace::iterator *it,
        RecordList *displayList );
    virtual MemoryTrace::iterator *getPrevRecord( MemoryTrace::iterator *it,
        RecordList *displayList );

    virtual void setChilds() {}
    virtual Trace *getWindowTrace() const;
    virtual TWindowLevel getWindowLevel() const;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel,
                                         TObjectOrder whichOrder );
    virtual bool IsDerivedWindow() const;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const;

};


#endif // INTERVALCPU_H_INCLUDED
