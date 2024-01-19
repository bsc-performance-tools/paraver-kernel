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


#pragma once


#include <vector>
#include "paraverkerneltypes.h"
#include "utils/include/vectorutils.h"
#include "histogramtotals.h"

class KHistogramTotals: public HistogramTotals
{
  public:
    KHistogramTotals( const KHistogramTotals& source );

    KHistogramTotals( PRV_UINT16 numStat, THistogramColumn numColumns,
                      THistogramColumn numPlanes );
    ~KHistogramTotals();

    void newValue( TSemanticValue whichValue,
                   PRV_UINT16 idStat,
                   THistogramColumn whichColumn,
                   THistogramColumn whichPlane = 0 );
    void finish();

    TSemanticValue getTotal( PRV_UINT16 idStat,
                             THistogramColumn whichColumn,
                             THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getAverage( PRV_UINT16 idStat,
                               THistogramColumn whichColumn,
                               THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getMaximum( PRV_UINT16 idStat,
                               THistogramColumn whichColumn,
                               THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getMinimum( PRV_UINT16 idStat,
                               THistogramColumn whichColumn,
                               THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getStdev( PRV_UINT16 idStat,
                             THistogramColumn whichColumn,
                             THistogramColumn whichPlane = 0 ) const;
    TSemanticValue getAvgDivMax( PRV_UINT16 idStat,
                                 THistogramColumn whichColumn,
                                 THistogramColumn whichPlane = 0 ) const;
    void getAll( std::vector<TSemanticValue>& where,
                 PRV_UINT16 idStat,
                 THistogramColumn whichColumn,
                 THistogramColumn whichPlane = 0 ) const;

    const std::vector<int>& sortByTotal( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );
    const std::vector<int>& sortByAverage( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );
    const std::vector<int>& sortByMaximum( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );
    const std::vector<int>& sortByMinimum( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );
    const std::vector<int>& sortByStdev( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );
    const std::vector<int>& sortByAvgDivMax( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );

  protected:

  private:
    THistogramColumn columns;
    PRV_UINT16 stats;
    // planes<stats<columns<value> > >
    std::vector<std::vector<std::vector<TSemanticValue> > > total;
    std::vector<std::vector<std::vector<TSemanticValue> > > average;
    std::vector<std::vector<std::vector<TSemanticValue> > > maximum;
    std::vector<std::vector<std::vector<TSemanticValue> > > minimum;
    std::vector<std::vector<std::vector<TSemanticValue> > > stdev;

    SortIndex<TSemanticValue> *sort;
    std::vector<int> nullSort;
};



