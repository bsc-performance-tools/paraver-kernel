#ifndef INTERVALHIGH_H_INCLUDED
#define INTERVALHIGH_H_INCLUDED

#include "interval.h"

class IntervalHigh: public Interval
{
  public:
    IntervalHigh();

    IntervalHigh( TObjectOrder whichOrder ):
        Interval( whichOrder )
    {}

    ~IntervalHigh();

  protected:
    vector<Interval *> childIntervals;

    void setChilds( TWindowLevel whichLevel, TObjectOrder whichOrder )
    {

    }
  private:

};


#endif // INTERVALHIGH_H_INCLUDED
