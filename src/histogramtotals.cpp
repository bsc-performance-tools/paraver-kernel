#include "histogramtotals.h"

HistogramTotals::HistogramTotals( THistogramColumn numColumns,
                                  THistogramColumn numPlanes )
{
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    total.push_back( vector<TSemanticValue>() );
    average.push_back( vector<TSemanticValue>() );
    maximum.push_back( vector<TSemanticValue>() );
    minimum.push_back( vector<TSemanticValue>() );
    stdev.push_back( vector<TSemanticValue>() );
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; iColumn++ )
    {
      total[ iPlane ].push_back( 0.0 );
      average[ iPlane ].push_back( 0.0 );
      maximum[ iPlane ].push_back( 0.0 );
      minimum[ iPlane ].push_back( 0.0 );
      stdev[ iPlane ].push_back( 0.0 );
    }
  }
}


void HistogramTotals::newValue( TSemanticValue whichValue,
                                THistogramColumn whichColumn,
                                THistogramColumn whichPlane )
{

}


void HistogramTotals::finish()
{}



TSemanticValue HistogramTotals::getTotal( THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( total[ whichPlane ] )[ whichColumn ];
}


TSemanticValue HistogramTotals::getAverage( THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( average[ whichPlane ] )[ whichColumn ];
}


TSemanticValue HistogramTotals::getMaximum( THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( maximum[ whichPlane ] )[ whichColumn ];
}


TSemanticValue HistogramTotals::getMinimum( THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( minimum[ whichPlane ] )[ whichColumn ];
}


TSemanticValue HistogramTotals::getStdev( THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( stdev[ whichPlane ] )[ whichColumn ];
}


TSemanticValue HistogramTotals::getAvgDivMax( THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( average[ whichPlane ] )[ whichColumn ] /
         ( maximum[ whichPlane ] )[ whichColumn ];
}


void HistogramTotals::getAll( vector<TSemanticValue>& where,
                              THistogramColumn whichColumn,
                              THistogramColumn whichPlane ) const
{
  where.push_back( ( total[ whichPlane ] )[ whichColumn ] );
  where.push_back( ( average[ whichPlane ] )[ whichColumn ] );
  where.push_back( ( maximum[ whichPlane ] )[ whichColumn ] );
  where.push_back( ( minimum[ whichPlane ] )[ whichColumn ] );
  where.push_back( ( stdev[ whichPlane ] )[ whichColumn ] );
  where.push_back( ( average[ whichPlane ] )[ whichColumn ] /
                   ( maximum[ whichPlane ] )[ whichColumn ] );
}
