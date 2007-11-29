#ifndef HISTOGRAMSTATISTIC_H_INCLUDED
#define HISTOGRAMSTATISTIC_H_INCLUDED

#include "paraverkerneltypes.h"

class Histogram;

struct CalculateData;

class HistogramStatistic
{
  public:
    HistogramStatistic();
    virtual ~HistogramStatistic();

    virtual bool createComms() const = 0;
    virtual void init( Histogram *whichHistogram ) = 0;
    virtual TSemanticValue execute( CalculateData *data ) = 0;
    virtual void finish() = 0;

//    virtual string getName() = 0;
//    virtual SemanticFunction *clone() = 0;

  protected:

  private:

};


class StatTime: public HistogramStatistic
{
  public:
    StatTime();
    ~StatTime();
  protected:

  private:

};


class StatPercTime: public HistogramStatistic
{
  public:
    StatPercTime();
    ~StatPercTime();
  protected:

  private:

};

#endif // HISTOGRAMSTATISTIC_H_INCLUDED
