#include <limits>
#include <math.h>
#include "khistogramtotals.h"
#include "paraverkerneltypes.h"


KHistogramTotals::KHistogramTotals( KHistogramTotals *& source )
{
  columns = source->columns;
  stats = source->stats;

  sort = NULL;
  nullSort = source->nullSort;
}

KHistogramTotals::KHistogramTotals( UINT16 numStat,
                                    THistogramColumn numColumns,
                                    THistogramColumn numPlanes ):
    columns( numColumns ), stats( numStat )
{
  vector<TSemanticValue> tmpColumns( columns, 0.0 );
  vector<TSemanticValue> tmpColumnsMax( columns, -numeric_limits<double>::max() );
  vector<TSemanticValue> tmpColumnsMin( columns, numeric_limits<double>::max() );
  vector<vector<TSemanticValue> > tmpStats( stats, tmpColumns );
  vector<vector<TSemanticValue> > tmpStatsMax( stats, tmpColumnsMax );
  vector<vector<TSemanticValue> > tmpStatsMin( stats, tmpColumnsMin );

  total.insert( total.begin(), numPlanes, tmpStats );
  average.insert( average.begin(), numPlanes, tmpStats );
  maximum.insert( maximum.begin(), numPlanes, tmpStatsMax );
  minimum.insert( minimum.begin(), numPlanes, tmpStatsMin );
  stdev.insert( stdev.begin(), numPlanes, tmpStats );

  for ( THistogramColumn iColumn = 0; iColumn < numColumns; iColumn++ )
    nullSort.push_back( iColumn );

  sort = NULL;
}


KHistogramTotals::~KHistogramTotals()
{
  if ( sort != NULL )
    delete sort;
}


void KHistogramTotals::newValue( TSemanticValue whichValue,
                                 UINT16 idStat,
                                 THistogramColumn whichColumn,
                                 THistogramColumn whichPlane )
{
  ( ( total[ whichPlane ] )[ idStat ] )[ whichColumn ] += whichValue;

  ( ( average[ whichPlane ] )[ idStat ] )[ whichColumn ] += 1;

  if ( whichValue > ( ( maximum[ whichPlane ] )[ idStat ] )[ whichColumn ] )
    ( ( maximum[ whichPlane ] )[ idStat ] )[ whichColumn ] = whichValue;

  if ( whichValue != 0.0 &&
       whichValue < ( ( minimum[ whichPlane ] )[ idStat ] )[ whichColumn ] )
    ( ( minimum[ whichPlane ] )[ idStat ] )[ whichColumn ] = whichValue;

  ( ( stdev[ whichPlane ] )[ idStat ] )[ whichColumn ] +=
    ( whichValue * whichValue );
}


void KHistogramTotals::finish()
{
  for ( THistogramColumn iPlane = 0; iPlane < total.size(); iPlane++ )
  {
    for ( UINT16 iStat = 0; iStat < stats; iStat++ )
    {
      for ( THistogramColumn iColumn = 0; iColumn < columns; iColumn++ )
      {
        TSemanticValue n = ( ( average[ iPlane ] )[ iStat ] )[ iColumn ];

        if ( n == 0 )
        {
          ( ( total[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;
          ( ( average[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;
          ( ( maximum[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;
          ( ( minimum[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;
          ( ( stdev[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;
        }
        else
        {
          if( ( ( minimum[ iPlane ] )[ iStat ] )[ iColumn ] ==
              numeric_limits<double>::max() )
            ( ( minimum[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;

          ( ( average[ iPlane ] )[ iStat ] )[ iColumn ] =
            ( ( total[ iPlane ] )[ iStat ] )[ iColumn ] / n;

          TSemanticValue avgQ = ( ( average[ iPlane ] )[ iStat ] )[ iColumn ] *
                                ( ( average[ iPlane ] )[ iStat ] )[ iColumn ];

          ( ( stdev[ iPlane ] )[ iStat ] )[ iColumn ] -= ( n * avgQ );

          ( ( stdev[ iPlane ] )[ iStat ] )[ iColumn ] /= ( n );

          if ( ( ( stdev[ iPlane ] )[ iStat ] )[ iColumn ] < 0.0 )
            ( ( stdev[ iPlane ] )[ iStat ] )[ iColumn ] *= -1.0;

          ( ( stdev[ iPlane ] )[ iStat ] )[ iColumn ] =
            sqrt( ( ( stdev[ iPlane ] )[ iStat ] )[ iColumn ] );
        }
      }
    }
  }
}


TSemanticValue KHistogramTotals::getTotal( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( total[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


TSemanticValue KHistogramTotals::getAverage( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( average[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


TSemanticValue KHistogramTotals::getMaximum( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( maximum[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


TSemanticValue KHistogramTotals::getMinimum( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( minimum[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


TSemanticValue KHistogramTotals::getStdev( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( stdev[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


TSemanticValue KHistogramTotals::getAvgDivMax( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( average[ whichPlane ] )[ idStat ] )[ whichColumn ] /
         ( ( maximum[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


void KHistogramTotals::getAll( vector<TSemanticValue>& where,
                               UINT16 idStat,
                               THistogramColumn whichColumn,
                               THistogramColumn whichPlane ) const
{
  where.push_back( ( ( total[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
  where.push_back( ( ( average[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
  where.push_back( ( ( maximum[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
  where.push_back( ( ( minimum[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
  where.push_back( ( ( stdev[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
  where.push_back( ( ( average[ whichPlane ] )[ idStat ] ) [ whichColumn ] /
                   ( ( maximum[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
}


vector<int>& KHistogramTotals::sortByTotal( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != NULL )
    delete sort;

  sort = new SortIndex<TSemanticValue>( ( total[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByAverage( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != NULL )
    delete sort;

  sort = new SortIndex<TSemanticValue>( ( average[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByMaximum( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != NULL )
    delete sort;

  sort = new SortIndex<TSemanticValue>( ( maximum[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByMinimum( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != NULL )
    delete sort;

  sort = new SortIndex<TSemanticValue>( ( minimum[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByStdev( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != NULL )
    delete sort;

  sort = new SortIndex<TSemanticValue>( ( stdev[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByAvgDivMax( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != NULL )
    delete sort;

  vector<TSemanticValue> tmpV;

  for ( THistogramColumn iColumn = 0;
        iColumn < columns;
        iColumn++ )
  {
    tmpV.push_back( ( ( average[ whichPlane ] )[ idStat ] ) [ iColumn ] /
                    ( ( maximum[ whichPlane ] )[ idStat ] ) [ iColumn ] );
  }

  sort = new SortIndex<TSemanticValue>( tmpV );
  return sort->sort();
}
