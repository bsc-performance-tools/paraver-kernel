#ifndef INTERVALDERIVED_H_INCLUDED
#define INTERVALDERIVED_H_INCLUDED

#include "intervalhigh.h"
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

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                              KRecordList *displayList = NULL );
    virtual KRecordList *calcNext( KRecordList *displayList = NULL, bool initCalc = false );
    virtual KRecordList *calcPrev( KRecordList *displayList = NULL, bool initCalc = false );

    virtual KWindow *getWindow()
    {
      return ( KWindow * ) window;
    }

  protected:
    KDerivedWindow *window;
    SemanticDerived *function;
    TCreateList createList;

    virtual void setChilds();

    virtual KTrace *getWindowTrace() const;
    virtual TWindowLevel getWindowLevel() const;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel,
                                         TObjectOrder whichOrder );
    virtual bool IsDerivedWindow() const;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const;

  private:

};

#endif // INTERVALDERIVED_H_INCLUDED
