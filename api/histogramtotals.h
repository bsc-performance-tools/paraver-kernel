#ifndef HISTOGRAMTOTALS_H_INCLUDED
#define HISTOGRAMTOTALS_H_INCLUDED

#include "paraverkerneltypes.h"

class HistogramTotals
{
  public:
    static HistogramTotals *create( HistogramTotals *whichTotals );

    virtual ~HistogramTotals() {}

    virtual TSemanticValue getTotal( UINT16 idStat,
                                     THistogramColumn whichColumn,
                                     THistogramColumn whichPlane = 0 ) const = 0;
    virtual TSemanticValue getAverage( UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const = 0;
    virtual TSemanticValue getMaximum( UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const = 0;
    virtual TSemanticValue getMinimum( UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const = 0;
    virtual TSemanticValue getStdev( UINT16 idStat,
                                     THistogramColumn whichColumn,
                                     THistogramColumn whichPlane = 0 ) const = 0;
    virtual TSemanticValue getAvgDivMax( UINT16 idStat,
                                         THistogramColumn whichColumn,
                                         THistogramColumn whichPlane = 0 ) const = 0;
    virtual void getAll( vector<TSemanticValue>& where,
                         UINT16 idStat,
                         THistogramColumn whichColumn,
                         THistogramColumn whichPlane = 0 ) const = 0;

    virtual vector<int>& sortByTotal( UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;
    virtual vector<int>& sortByAverage( UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;
    virtual vector<int>& sortByMaximum( UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;
    virtual vector<int>& sortByMinimum( UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;
    virtual vector<int>& sortByStdev( UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;
    virtual vector<int>& sortByAvgDivMax( UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;

  protected:

  private:

};

class HistogramTotalsProxy: public HistogramTotals
{
  public:
    virtual ~HistogramTotalsProxy() {}

    virtual TSemanticValue getTotal( UINT16 idStat,
                                     THistogramColumn whichColumn,
                                     THistogramColumn whichPlane = 0 ) const;
    virtual TSemanticValue getAverage( UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const;
    virtual TSemanticValue getMaximum( UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const;
    virtual TSemanticValue getMinimum( UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const;
    virtual TSemanticValue getStdev( UINT16 idStat,
                                     THistogramColumn whichColumn,
                                     THistogramColumn whichPlane = 0 ) const;
    virtual TSemanticValue getAvgDivMax( UINT16 idStat,
                                         THistogramColumn whichColumn,
                                         THistogramColumn whichPlane = 0 ) const;
    virtual void getAll( vector<TSemanticValue>& where,
                         UINT16 idStat,
                         THistogramColumn whichColumn,
                         THistogramColumn whichPlane = 0 ) const;

    virtual vector<int>& sortByTotal( UINT16 idStat, THistogramColumn whichPlane = 0 );
    virtual vector<int>& sortByAverage( UINT16 idStat, THistogramColumn whichPlane = 0 );
    virtual vector<int>& sortByMaximum( UINT16 idStat, THistogramColumn whichPlane = 0 );
    virtual vector<int>& sortByMinimum( UINT16 idStat, THistogramColumn whichPlane = 0 );
    virtual vector<int>& sortByStdev( UINT16 idStat, THistogramColumn whichPlane = 0 );
    virtual vector<int>& sortByAvgDivMax( UINT16 idStat, THistogramColumn whichPlane = 0 );

  protected:

  private:
    HistogramTotalsProxy( HistogramTotals *whichTotals );

    HistogramTotals *myTotals;

    friend HistogramTotals *HistogramTotals::create( HistogramTotals * );
};

#endif // HISTOGRAMTOTALS_H_INCLUDED
