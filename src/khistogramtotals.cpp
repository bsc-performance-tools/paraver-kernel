#include <math.h>
#include "khistogramtotals.h"

KHistogramTotals::KHistogramTotals( UINT16 numStat,
                                    THistogramColumn numColumns,
                                    THistogramColumn numPlanes ):
    columns( numColumns ), stats( numStat )
{
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    total.push_back( NULL );
    average.push_back( NULL );
    maximum.push_back( NULL );
    minimum.push_back( NULL );
    stdev.push_back( NULL );
  }

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
  if ( total[ whichPlane ] == NULL )
  {
    total[ whichPlane ] = new vector<vector<TSemanticValue> *>();
    average[ whichPlane ] = new vector<vector<TSemanticValue> *>();
    maximum[ whichPlane ] = new vector<vector<TSemanticValue> *>();
    minimum[ whichPlane ] = new vector<vector<TSemanticValue> *>();
    stdev[ whichPlane ] = new vector<vector<TSemanticValue> *>();
    for ( UINT16 iStat = 0; iStat < stats; iStat++ )
    {
      total[ whichPlane ]->push_back( NULL );
      average[ whichPlane ]->push_back( NULL );
      maximum[ whichPlane ]->push_back( NULL );
      minimum[ whichPlane ]->push_back( NULL );
      stdev[ whichPlane ]->push_back( NULL );
    }
  }

  if ( ( *total[ whichPlane ] )[ idStat ] == NULL )
  {
    ( *total[ whichPlane ] )[ idStat ] = new vector<TSemanticValue>();
    ( *average[ whichPlane ] )[ idStat ] = new vector<TSemanticValue>();
    ( *maximum[ whichPlane ] )[ idStat ] = new vector<TSemanticValue>();
    ( *minimum[ whichPlane ] )[ idStat ] = new vector<TSemanticValue>();
    ( *stdev[ whichPlane ] )[ idStat ] = new vector<TSemanticValue>();
    for ( THistogramColumn iColumn = 0; iColumn < columns; iColumn++ )
    {
      ( *total[ whichPlane ] )[ idStat ]->push_back( 0.0 );
      ( *average[ whichPlane ] )[ idStat ]->push_back( 0.0 );
      ( *maximum[ whichPlane ] )[ idStat ]->push_back( std::numeric_limits<double>::min() );
      ( *minimum[ whichPlane ] )[ idStat ]->push_back( std::numeric_limits<double>::max() );
      ( *stdev[ whichPlane ] )[ idStat ]->push_back( 0.0 );
    }
  }

  ( *( *total[ whichPlane ] )[ idStat ] )[ whichColumn ] += whichValue;

  ( *( *average[ whichPlane ] )[ idStat ] )[ whichColumn ] += 1;

  if ( whichValue > ( *( *maximum[ whichPlane ] )[ idStat ] )[ whichColumn ] )
    ( *( *maximum[ whichPlane ] )[ idStat ] )[ whichColumn ] = whichValue;

  if ( whichValue != 0.0 &&
       whichValue < ( *( *minimum[ whichPlane ] )[ idStat ] )[ whichColumn ] )
    ( *( *minimum[ whichPlane ] )[ idStat ] )[ whichColumn ] = whichValue;

  ( *( *stdev[ whichPlane ] )[ idStat ] )[ whichColumn ] +=
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
        if ( ( ( *average[ iPlane ] )[ iStat ] != NULL ) )
        {
          TSemanticValue n = ( *( *average[ iPlane ] )[ iStat ] )[ iColumn ];

          ( *( *average[ iPlane ] )[ iStat ] )[ iColumn ] =
            ( *( *total[ iPlane ] )[ iStat ] )[ iColumn ] / n;

          TSemanticValue avgQ = ( *( *average[ iPlane ] )[ iStat ] )[ iColumn ] *
                                ( *( *average[ iPlane ] )[ iStat ] )[ iColumn ];

        /*  ( *( *stdev[ iPlane ] )[ iStat ] )[ iColumn ] -= ( 2 *
              ( *( *average[ iPlane ] )[ iStat ] )[ iColumn ] *
              ( *( *total[ iPlane ] )[ iStat ] )[ iColumn ] );

          ( *( *stdev[ iPlane ] )[ iStat ] )[ iColumn ] += avgQ;*/
          ( *( *stdev[ iPlane ] )[ iStat ] )[ iColumn ] -= ( n * avgQ );

          ( *( *stdev[ iPlane ] )[ iStat ] )[ iColumn ] /= ( n );

          if ( ( *( *stdev[ iPlane ] )[ iStat ] )[ iColumn ] < 0.0 )
            ( *( *stdev[ iPlane ] )[ iStat ] )[ iColumn ] *= -1.0;

            ( *( *stdev[ iPlane ] )[ iStat ] )[ iColumn ] =
              sqrt( ( *( *stdev[ iPlane ] )[ iStat ] )[ iColumn ] );
          }
      }
    }
  }
}


TSemanticValue KHistogramTotals::getTotal( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( total[ whichPlane ] != NULL )
  {
    if ( ( *total[ whichPlane ] )[ idStat ] != NULL )
      return ( *( *total[ whichPlane ] )[ idStat ] )[ whichColumn ];
  }

  return 0.0;
}


TSemanticValue KHistogramTotals::getAverage( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( average[ whichPlane ] != NULL )
  {
    if ( ( *average[ whichPlane ] )[ idStat ] != NULL )
      return ( *( *average[ whichPlane ] )[ idStat ] )[ whichColumn ];
  }

  return 0.0;
}


TSemanticValue KHistogramTotals::getMaximum( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( maximum[ whichPlane ] != NULL )
  {
    if ( ( *maximum[ whichPlane ] )[ idStat ] != NULL )
      return ( *( *maximum[ whichPlane ] )[ idStat ] )[ whichColumn ];
  }

  return 0.0;
}


TSemanticValue KHistogramTotals::getMinimum( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( minimum[ whichPlane ] != NULL )
  {
    if ( ( *minimum[ whichPlane ] )[ idStat ] != NULL )
      return ( *( *minimum[ whichPlane ] )[ idStat ] )[ whichColumn ];
  }

  return 0.0;
}


TSemanticValue KHistogramTotals::getStdev( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( stdev[ whichPlane ] != NULL )
  {
    if ( ( *stdev[ whichPlane ] )[ idStat ] != NULL )
      return ( *( *stdev[ whichPlane ] )[ idStat ] )[ whichColumn ];
  }

  return 0.0;
}


TSemanticValue KHistogramTotals::getAvgDivMax( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( average[ whichPlane ] != NULL )
  {
    if ( ( *average[ whichPlane ] )[ idStat ] != NULL )
      return ( *( *average[ whichPlane ] )[ idStat ] )[ whichColumn ] /
             ( *( *maximum[ whichPlane ] )[ idStat ] )[ whichColumn ];
  }

  return 0.0;
}


void KHistogramTotals::getAll( vector<TSemanticValue>& where,
                               UINT16 idStat,
                               THistogramColumn whichColumn,
                               THistogramColumn whichPlane ) const
{
  if ( total[ whichPlane ] != NULL &&
       ( *total[ whichPlane ] )[ idStat ] != NULL )
  {
    where.push_back( ( *( *total[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
    where.push_back( ( *( *average[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
    where.push_back( ( *( *maximum[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
    where.push_back( ( *( *minimum[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
    where.push_back( ( *( *stdev[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
    where.push_back( ( *( *average[ whichPlane ] )[ idStat ] ) [ whichColumn ] /
                     ( *( *maximum[ whichPlane ] )[ idStat ] ) [ whichColumn ] );
  }
  else
  {
    where.push_back( 0.0 );
    where.push_back( 0.0 );
    where.push_back( 0.0 );
    where.push_back( 0.0 );
    where.push_back( 0.0 );
    where.push_back( 0.0 );
  }
}


vector<int>& KHistogramTotals::sortByTotal( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( total[ whichPlane ] == NULL ||
       ( *total[ whichPlane ] )[ idStat ] == NULL )
    return nullSort;

  if ( sort != NULL )
    delete sort;

  sort = new SortIndex<TSemanticValue>( *( *total[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByAverage( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( average[ whichPlane ] == NULL ||
       ( *average[ whichPlane ] )[ idStat ] == NULL )
    return nullSort;

  if ( sort != NULL )
    delete sort;

  sort = new SortIndex<TSemanticValue>( *( *average[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByMaximum( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( maximum[ whichPlane ] == NULL ||
       ( *maximum[ whichPlane ] )[ idStat ] == NULL )
    return nullSort;

  if ( sort != NULL )
    delete sort;

  sort = new SortIndex<TSemanticValue>( *( *maximum[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByMinimum( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( minimum[ whichPlane ] == NULL ||
       ( *minimum[ whichPlane ] )[ idStat ] == NULL )
    return nullSort;

  if ( sort != NULL )
    delete sort;

  sort = new SortIndex<TSemanticValue>( *( *minimum[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByStdev( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( stdev[ whichPlane ] == NULL ||
       ( *stdev[ whichPlane ] )[ idStat ] == NULL )
    return nullSort;

  if ( sort != NULL )
    delete sort;

  sort = new SortIndex<TSemanticValue>( *( *stdev[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByAvgDivMax( UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( average[ whichPlane ] == NULL ||
       ( *average[ whichPlane ] )[ idStat ] == NULL )
    return nullSort;

  if ( sort != NULL )
    delete sort;

  vector<TSemanticValue> tmpV;

  for ( THistogramColumn iColumn = 0;
        iColumn < columns;
        iColumn++ )
  {
    tmpV.push_back( ( *( *average[ whichPlane ] )[ idStat ] ) [ iColumn ] /
                    ( *( *maximum[ whichPlane ] )[ idStat ] ) [ iColumn ] );
  }

  sort = new SortIndex<TSemanticValue>( tmpV );
  return sort->sort();
}
