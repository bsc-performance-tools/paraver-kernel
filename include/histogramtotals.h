#ifndef HISTOGRAMTOTALS_H_INCLUDED
#define HISTOGRAMTOTALS_H_INCLUDED

#include <vector>
#include "paraverkerneltypes.h"

using namespace std;

class HistogramTotals
{
  public:
    HistogramTotals( THistogramColumn numColumns, THistogramColumn numPlanes );
    ~HistogramTotals();

    void newValue( TSemanticValue whichValue,
                   THistogramColumn whichColumn,
                   THistogramColumn whichPlane = 0 );
    void finish();

    TSemanticValue getTotal( THistogramColumn whichColumn,
                             THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getAverage( THistogramColumn whichColumn,
                               THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getMaximum( THistogramColumn whichColumn,
                               THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getMinimum( THistogramColumn whichColumn,
                               THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getStdev( THistogramColumn whichColumn,
                             THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getAvgDivMax( THistogramColumn whichColumn,
                                 THistogramColumn whichPlane = 0 ) const;
    void getAll( vector<TSemanticValue>& where,
                 THistogramColumn whichColumn,
                 THistogramColumn whichPlane = 0 ) const;

  protected:

  private:
    vector<vector<TSemanticValue> > total;
    vector<vector<TSemanticValue> > average;
    vector<vector<TSemanticValue> > maximum;
    vector<vector<TSemanticValue> > minimum;
    vector<vector<TSemanticValue> > stdev;

};


#endif // HISTOGRAMTOTALS_H_INCLUDED
