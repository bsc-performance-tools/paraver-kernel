/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/


#include <limits>
#include <math.h>
#include "khistogramtotals.h"
#include "paraverkerneltypes.h"

using namespace std;

KHistogramTotals::KHistogramTotals( KHistogramTotals *& source )
{
  columns = source->columns;
  stats = source->stats;

  sort = nullptr;
  nullSort = source->nullSort;
}

KHistogramTotals::KHistogramTotals( PRV_UINT16 numStat,
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

  sort = nullptr;
}


KHistogramTotals::~KHistogramTotals()
{
  if ( sort != nullptr )
    delete sort;
}


void KHistogramTotals::newValue( TSemanticValue whichValue,
                                 PRV_UINT16 idStat,
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
    for ( PRV_UINT16 iStat = 0; iStat < stats; iStat++ )
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


TSemanticValue KHistogramTotals::getTotal( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( total[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


TSemanticValue KHistogramTotals::getAverage( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( average[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


TSemanticValue KHistogramTotals::getMaximum( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( maximum[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


TSemanticValue KHistogramTotals::getMinimum( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( minimum[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


TSemanticValue KHistogramTotals::getStdev( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( stdev[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


TSemanticValue KHistogramTotals::getAvgDivMax( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return ( ( average[ whichPlane ] )[ idStat ] )[ whichColumn ] /
         ( ( maximum[ whichPlane ] )[ idStat ] )[ whichColumn ];
}


void KHistogramTotals::getAll( vector<TSemanticValue>& where,
                               PRV_UINT16 idStat,
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


vector<int>& KHistogramTotals::sortByTotal( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != nullptr )
    delete sort;

  sort = new SortIndex<TSemanticValue>( ( total[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByAverage( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != nullptr )
    delete sort;

  sort = new SortIndex<TSemanticValue>( ( average[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByMaximum( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != nullptr )
    delete sort;

  sort = new SortIndex<TSemanticValue>( ( maximum[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByMinimum( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != nullptr )
    delete sort;

  sort = new SortIndex<TSemanticValue>( ( minimum[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByStdev( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != nullptr )
    delete sort;

  sort = new SortIndex<TSemanticValue>( ( stdev[ whichPlane ] )[ idStat ] );
  return sort->sort();
}


vector<int>& KHistogramTotals::sortByAvgDivMax( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  if ( sort != nullptr )
    delete sort;

  vector<TSemanticValue> tmpV;

  for ( THistogramColumn iColumn = 0;
        iColumn < columns;
        ++iColumn )
  {
    if( ( ( maximum[ whichPlane ] )[ idStat ] ) [ iColumn ] != 0.0 )
      tmpV.push_back( ( ( average[ whichPlane ] )[ idStat ] ) [ iColumn ] /
                      ( ( maximum[ whichPlane ] )[ idStat ] ) [ iColumn ] );
    else
      tmpV.push_back( 0.0 );
  }

  sort = new SortIndex<TSemanticValue>( tmpV );
  return sort->sort();
}
