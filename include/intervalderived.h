#ifndef INTERVALDERIVED_H_INCLUDED
#define INTERVALDERIVED_H_INCLUDED

#include "intervalhigh.h"
#include "kwindow.h"
#include "semanticderived.h"

class KWindow;
class KDerivedWindow;
class SemanticDerived;

class IntervalDerived: public IntervalHigh
{
  public:
    IntervalDerived()
    {
      function = NULL;
    }

    IntervalDerived( KDerivedWindow *whichWindow, TWindowLevel whichLevel,
                     TObjectOrder whichOrder ):
        IntervalHigh( whichLevel, whichOrder ), window( whichWindow )
    {
      function = NULL;
    }

    virtual ~IntervalDerived()
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

  protected:
    KDerivedWindow *window;
    SemanticDerived *function;
    TCreateList createList;

    virtual void setChilds();

    virtual Trace *getWindowTrace() const;
    virtual TWindowLevel getWindowLevel() const;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel,
                                         TObjectOrder whichOrder );
    virtual bool IsDerivedWindow() const;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const;

  private:

};

#endif // INTERVALDERIVED_H_INCLUDED
