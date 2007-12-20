#include <math.h>
#include "histogramtotals.h"

HistogramTotals::HistogramTotals( UINT16 numStat,
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
}


void HistogramTotals::newValue( TSemanticValue whichValue,
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
    for ( THistogramColumn iColumn = 0; iColumn < columns; iColumn++ )
    {
      total[ whichPlane ]->push_back( NULL );
      average[ whichPlane ]->push_back( NULL );
      maximum[ whichPlane ]->push_back( NULL );
      minimum[ whichPlane ]->push_back( NULL );
      stdev[ whichPlane ]->push_back( NULL );
    }
  }

  if ( ( *total[ whichPlane ] )[ whichColumn ] == NULL )
  {
    ( *total[ whichPlane ] )[ whichColumn ] = new vector<TSemanticValue>();
    ( *average[ whichPlane ] )[ whichColumn ] = new vector<TSemanticValue>();
    ( *maximum[ whichPlane ] )[ whichColumn ] = new vector<TSemanticValue>();
    ( *minimum[ whichPlane ] )[ whichColumn ] = new vector<TSemanticValue>();
    ( *stdev[ whichPlane ] )[ whichColumn ] = new vector<TSemanticValue>();
    for ( UINT16 iStat = 0; iStat < stats; iStat++ )
    {
      ( *total[ whichPlane ] )[ whichColumn ]->push_back( 0.0 );
      ( *average[ whichPlane ] )[ whichColumn ]->push_back( 0.0 );
      ( *maximum[ whichPlane ] )[ whichColumn ]->push_back( 0.0 );
      ( *minimum[ whichPlane ] )[ whichColumn ]->push_back( 0.0 );
      ( *stdev[ whichPlane ] )[ whichColumn ]->push_back( 0.0 );
    }
  }

  ( *( *total[ whichPlane ] )[ whichColumn ] )[ idStat ] += whichValue;

  ( *( *average[ whichPlane ] )[ whichColumn ] )[ idStat ] += 1;

  if ( whichValue > ( *( *maximum[ whichPlane ] )[ whichColumn ] ) [ idStat ] )
    ( *( *maximum[ whichPlane ] )[ whichColumn ] ) [ idStat ] = whichValue;

  if ( whichValue != 0.0 &&
       whichValue < ( *( *minimum[ whichPlane ] )[ whichColumn ] ) [ idStat ] )
    ( *( *minimum[ whichPlane ] )[ whichColumn ] )[ idStat ] = whichValue;

  ( *( *stdev[ whichPlane ] )[ whichColumn ] )[ idStat ] =
    ( whichValue * whichValue );
}


void HistogramTotals::finish()
{
  for ( THistogramColumn iPlane = 0; iPlane < total.size(); iPlane++ )
  {
    for ( THistogramColumn iColumn = 0; iColumn < columns; iColumn++ )
    {
      for ( UINT16 iStat = 0; iStat < stats; iStat++ )
      {
        if ( ( ( *average[ iPlane ] )[ iColumn ] != NULL ) )
        {
          ( *( *stdev[ iPlane ] )[ iColumn ] )[ iStat ] /=
            ( *( *average[ iPlane ] )[ iColumn ] )[ iStat ];

          ( *( *average[ iPlane ] )[ iColumn ] )[ iStat ] =
            ( *( *total[ iPlane ] )[ iColumn ] )[ iStat ] /
            ( *( *average[ iPlane ] )[ iColumn ] )[ iStat ];

          TSemanticValue avgQ = ( *( *average[ iPlane ] )[ iColumn ] )[ iStat ] *
                                ( *( *average[ iPlane ] )[ iColumn ] )[ iStat ];

          ( *( *stdev[ iPlane ] )[ iColumn ] )[ iStat ] -= avgQ;

          if ( ( *( *stdev[ iPlane ] )[ iColumn ] )[ iStat ] < 0.0 )
            ( *( *stdev[ iPlane ] )[ iColumn ] )[ iStat ] *= -1.0;

          ( *( *stdev[ iPlane ] )[ iColumn ] )[ iStat ] =
            sqrt( ( *( *stdev[ iPlane ] )[ iColumn ] )[ iStat ] );
        }
      }
    }
  }
}


TSemanticValue HistogramTotals::getTotal( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( total[ whichPlane ] != NULL )
  {
    if ( ( *total[ whichPlane ] )[ whichColumn ] != NULL )
      return ( *( *total[ whichPlane ] )[ whichColumn ] )[ idStat ];
  }

  return 0.0;
}


TSemanticValue HistogramTotals::getAverage( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( average[ whichPlane ] != NULL )
  {
    if ( ( *average[ whichPlane ] )[ whichColumn ] != NULL )
      return ( *( *average[ whichPlane ] )[ whichColumn ] )[ idStat ];
  }

  return 0.0;
}


TSemanticValue HistogramTotals::getMaximum( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( maximum[ whichPlane ] != NULL )
  {
    if ( ( *maximum[ whichPlane ] )[ whichColumn ] != NULL )
      return ( *( *maximum[ whichPlane ] )[ whichColumn ] )[ idStat ];
  }

  return 0.0;
}


TSemanticValue HistogramTotals::getMinimum( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( minimum[ whichPlane ] != NULL )
  {
    if ( ( *minimum[ whichPlane ] )[ whichColumn ] != NULL )
      return ( *( *minimum[ whichPlane ] )[ whichColumn ] )[ idStat ];
  }

  return 0.0;
}


TSemanticValue HistogramTotals::getStdev( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( stdev[ whichPlane ] != NULL )
  {
    if ( ( *stdev[ whichPlane ] )[ whichColumn ] != NULL )
      return ( *( *stdev[ whichPlane ] )[ whichColumn ] )[ idStat ];
  }

  return 0.0;
}


TSemanticValue HistogramTotals::getAvgDivMax( UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  if ( average[ whichPlane ] != NULL )
  {
    if ( ( *average[ whichPlane ] )[ whichColumn ] != NULL )
      return ( *( *average[ whichPlane ] )[ whichColumn ] )[ idStat ] /
             ( *( *maximum[ whichPlane ] )[ whichColumn ] )[ idStat ];
  }

  return 0.0;
}


void HistogramTotals::getAll( vector<TSemanticValue>& where,
                              UINT16 idStat,
                              THistogramColumn whichColumn,
                              THistogramColumn whichPlane ) const
{
  if ( total[ whichPlane ] != NULL &&
       ( *total[ whichPlane ] )[ whichColumn ] != NULL )
  {
    where.push_back( ( *( *total[ whichPlane ] )[ whichColumn ] ) [ idStat ] );
    where.push_back( ( *( *average[ whichPlane ] )[ whichColumn ] ) [ idStat ] );
    where.push_back( ( *( *maximum[ whichPlane ] )[ whichColumn ] ) [ idStat ] );
    where.push_back( ( *( *minimum[ whichPlane ] )[ whichColumn ] ) [ idStat ] );
    where.push_back( ( *( *stdev[ whichPlane ] )[ whichColumn ] ) [ idStat ] );
    where.push_back( ( *( *average[ whichPlane ] )[ whichColumn ] ) [ idStat ] /
                     ( *( *maximum[ whichPlane ] )[ whichColumn ] ) [ idStat ] );
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
