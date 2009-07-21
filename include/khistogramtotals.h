#ifndef KHISTOGRAMTOTALS_H_INCLUDED
#define KHISTOGRAMTOTALS_H_INCLUDED

#include <vector>
#include "paraverkerneltypes.h"
#include "vectorutils.h"
#include "histogramtotals.h"

using namespace std;

class KHistogramTotals: public HistogramTotals
{
  public:
    KHistogramTotals( KHistogramTotals *& source );

    KHistogramTotals( UINT16 numStat, THistogramColumn numColumns,
                      THistogramColumn numPlanes );
    ~KHistogramTotals();

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
    // planes<stats<columns<value> > >
    vector<vector<vector<TSemanticValue> > > total;
    vector<vector<vector<TSemanticValue> > > average;
    vector<vector<vector<TSemanticValue> > > maximum;
    vector<vector<vector<TSemanticValue> > > minimum;
    vector<vector<vector<TSemanticValue> > > stdev;

    SortIndex<TSemanticValue> *sort;
    vector<int> nullSort;
};


#endif // KHISTOGRAMTOTALS_H_INCLUDED
