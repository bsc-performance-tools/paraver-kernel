#ifndef INTERVALHIGH_H_INCLUDED
#define INTERVALHIGH_H_INCLUDED

#include "interval.h"
#include "ktrace.h"

class IntervalHigh: public Interval
{
  public:
    IntervalHigh()
    {}

    IntervalHigh( TWindowLevel whichLevel, TObjectOrder whichOrder ):
        Interval( whichLevel, whichOrder ), lastLevel( NONE )
    {}

    ~IntervalHigh()
    {}

  protected:
    vector<Interval *> childIntervals;
    TWindowLevel lastLevel;

    virtual void setChilds() = 0;

    virtual TWindowLevel getWindowLevel() const = 0;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel,
                                         TObjectOrder whichOrder ) = 0;
    virtual bool IsDerivedWindow() const = 0;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const = 0;
    virtual KTrace *getWindowTrace() const = 0;

  private:

};


#endif // INTERVALHIGH_H_INCLUDED
