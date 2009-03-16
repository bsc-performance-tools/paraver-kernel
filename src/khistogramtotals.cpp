#include <limits>
#include <math.h>
#include "khistogramtotals.h"
#include "paraverkerneltypes.h"


KHistogramTotals::KHistogramTotals( KHistogramTotals *& source )
{
  for ( size_t iPlane = 0; iPlane < source->total.size(); iPlane++ )
  {
    if (  source->total[ iPlane ] == NULL )
    {
      total.push_back( NULL );
      average.push_back( NULL );
      maximum.push_back( NULL );
      minimum.push_back( NULL );
      stdev.push_back( NULL );
    }
    else
    {
      total.push_back( new vector< vector< TSemanticValue > * >() );
      average.push_back( new vector< vector< TSemanticValue > * >() );
      maximum.push_back( new vector< vector< TSemanticValue > * >() );
      minimum.push_back( new vector< vector< TSemanticValue > * >() );
      stdev.push_back( new vector< vector< TSemanticValue > * >() );

      for ( UINT16 iStat = 0; iStat < source->stats; iStat++ )
      {
        if ( ( *source->total[ iPlane] )[ iStat ] == NULL )
        {
          total[ iPlane]->push_back( NULL );
          average[ iPlane ]->push_back( NULL );
          maximum[ iPlane ]->push_back( NULL );
          minimum[ iPlane ]->push_back( NULL );
          stdev[ iPlane ]->push_back( NULL );
        }
        else
        {
          total[ iPlane]->push_back( new vector<TSemanticValue>( ( *( *source->total[ iPlane ] )[ iStat ] )));
          average[ iPlane ]->push_back( new vector<TSemanticValue>( ( *( *source->average[ iPlane ] )[ iStat ] )));
          maximum[ iPlane ]->push_back( new vector<TSemanticValue>( ( *( *source->maximum[ iPlane ] )[ iStat ] )));
          minimum[ iPlane ]->push_back( new vector<TSemanticValue>( ( *( *source->minimum[ iPlane ] )[ iStat ] )));
          stdev[ iPlane ]->push_back( new vector<TSemanticValue>( ( *( *source->stdev[ iPlane ] )[ iStat ] )));
        }
/*
      ( *total[ iPlane] )[ iStat ]     = new vector<TSemanticValue>();
      ( *average[  iPlane ] )[ iStat ] = new vector<TSemanticValue>();
      ( *maximum[  iPlane ] )[ iStat ] = new vector<TSemanticValue>();
      ( *minimum[  iPlane ] )[ iStat ] = new vector<TSemanticValue>();
      ( *stdev[ iPlane ] )[ iStat ]    = new vector<TSemanticValue>();
      for ( THistogramColumn iCol = 0; iCol < source->columns; iCol++ )
      {
        ( *( *total[ iPlane ] )[ iStat ] )[ iCol ] = ( *( *source->total[ iPlane ] )[ iStat ] )[ iCol ];
        ( *( *average[ iPlane ] )[ iStat ] )[ iCol ] = ( *( *source->average[ iPlane ] )[ iStat ] )[ iCol ];
        ( *( *maximum[ iPlane ] )[ iStat ] )[ iCol ] = ( *( *source->maximum[ iPlane ] )[ iStat ] )[ iCol ];
        ( *( *minimum[ iPlane ] )[ iStat ] )[ iCol ] = ( *( *source->minimum[ iPlane ] )[ iStat ] )[ iCol ];
        ( *( *stdev[ iPlane ] )[ iStat ] )[ iCol ] = ( *( *source->stdev[ iPlane ] )[ iStat ] )[ iCol ];
      }
*/
      }
    }
  }

  columns = source->columns;
  stats = source->stats;

/*
  if ( source->sort != NULL )
    sort = new SortIndex<TSemanticValue>( *source->sort );
*/
  sort = NULL;
  nullSort = source->nullSort;
}

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
      ( *maximum[ whichPlane ] )[ idStat ]->push_back( numeric_limits<double>::min() );
      ( *minimum[ whichPlane ] )[ idStat ]->push_back( numeric_limits<double>::max() );
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
    // No values
    if ( total[ iPlane ] == NULL )
      continue;

    for ( UINT16 iStat = 0; iStat < stats; iStat++ )
    {
      for ( THistogramColumn iColumn = 0; iColumn < columns; iColumn++ )
      {
        if ( ( ( *average[ iPlane ] )[ iStat ] != NULL ) )
        {
          TSemanticValue n = ( *( *average[ iPlane ] )[ iStat ] )[ iColumn ];

          if ( n == 0 )
          {
            ( *( *total[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;
            ( *( *average[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;
            ( *( *maximum[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;
            ( *( *minimum[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;
            ( *( *stdev[ iPlane ] )[ iStat ] )[ iColumn ] = 0.0;
          }
          else
          {
            ( *( *average[ iPlane ] )[ iStat ] )[ iColumn ] =
              ( *( *total[ iPlane ] )[ iStat ] )[ iColumn ] / n;

            TSemanticValue avgQ = ( *( *average[ iPlane ] )[ iStat ] )[ iColumn ] *
                                  ( *( *average[ iPlane ] )[ iStat ] )[ iColumn ];

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
