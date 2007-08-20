#ifndef INTERVALCOMPOSE_H_INCLUDED
#define INTERVALCOMPOSE_H_INCLUDED

#include "intervalhigh.h"
#include "kwindow.h"
#include "semanticcompose.h"

class KWindow;

class IntervalCompose: public IntervalHigh
{
  public:
    IntervalCompose()
    {
      function = NULL;
    }

    IntervalCompose( KWindow *whichWindow, TObjectOrder whichOrder ):
        IntervalHigh( whichOrder ), window( whichWindow )
    {
      function = NULL;
    }

    virtual ~IntervalCompose()
    {}

    SemanticCompose *getSemanticFunction() const
    {
      return function;
    }

    void setSemanticFunction( SemanticCompose *whichFunction )
    {
      function = whichFunction;
    }

    virtual RecordList *init( TRecordTime initialTime, TCreateList create,
                              RecordList *displayList = NULL );
    virtual RecordList *calcNext( RecordList *displayList = NULL, bool initCalc = false );
    virtual RecordList *calcPrev( RecordList *displayList = NULL, bool initCalc = false );

  protected:
    KWindow *window;
    SemanticCompose *function;
    TCreateList createList;

  private:
};


#endif // INTERVALCOMPOSE_H_INCLUDED
