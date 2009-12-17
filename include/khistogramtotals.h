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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

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
