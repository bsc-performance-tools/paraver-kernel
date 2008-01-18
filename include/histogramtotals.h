#ifndef HISTOGRAMTOTALS_H_INCLUDED
#define HISTOGRAMTOTALS_H_INCLUDED

#include <vector>
#include "paraverkerneltypes.h"
#include "vectorutils.h"

using namespace std;

class HistogramTotals
{
  public:
    HistogramTotals( UINT16 numStat, THistogramColumn numColumns,
                     THistogramColumn numPlanes );
    ~HistogramTotals();

    void newValue( TSemanticValue whichValue,
                   UINT16 idStat,
                   THistogramColumn whichColumn,
                   THistogramColumn whichPlane = 0 );
    void finish();

    TSemanticValue getTotal( UINT16 idStat,
                             THistogramColumn whichColumn,
                             THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getAverage( UINT16 idStat,
                               THistogramColumn whichColumn,
                               THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getMaximum( UINT16 idStat,
                               THistogramColumn whichColumn,
                               THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getMinimum( UINT16 idStat,
                               THistogramColumn whichColumn,
                               THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getStdev( UINT16 idStat,
                             THistogramColumn whichColumn,
                             THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getAvgDivMax( UINT16 idStat,
                                 THistogramColumn whichColumn,
                                 THistogramColumn whichPlane = 0 ) const;
    void getAll( vector<TSemanticValue>& where,
                 UINT16 idStat,
                 THistogramColumn whichColumn,
                 THistogramColumn whichPlane = 0 ) const;

    vector<int>& sortByTotal( UINT16 idStat, THistogramColumn whichPlane = 0 );
    vector<int>& sortByAverage( UINT16 idStat, THistogramColumn whichPlane = 0 );
    vector<int>& sortByMaximum( UINT16 idStat, THistogramColumn whichPlane = 0 );
    vector<int>& sortByMinimum( UINT16 idStat, THistogramColumn whichPlane = 0 );
    vector<int>& sortByStdev( UINT16 idStat, THistogramColumn whichPlane = 0 );
    vector<int>& sortByAvgDivMax( UINT16 idStat, THistogramColumn whichPlane = 0 );

  protected:

  private:
    THistogramColumn columns;
    UINT16 stats;
    // planes<columns<stats<value> > >
    vector<vector<vector<TSemanticValue> *> *> total;
    vector<vector<vector<TSemanticValue> *> *> average;
    vector<vector<vector<TSemanticValue> *> *> maximum;
    vector<vector<vector<TSemanticValue> *> *> minimum;
    vector<vector<vector<TSemanticValue> *> *> stdev;

    SortIndex<TSemanticValue> *sort;
};


#endif // HISTOGRAMTOTALS_H_INCLUDED
