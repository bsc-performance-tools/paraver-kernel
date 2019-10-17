/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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


#include "histogramtotals.h"

using std::vector;

HistogramTotals *HistogramTotals::create( HistogramTotals *whichTotals )
{
  return new HistogramTotalsProxy( whichTotals );
}

HistogramTotalsProxy::HistogramTotalsProxy( HistogramTotals *whichTotals )
    : myTotals( whichTotals )
{}

TSemanticValue HistogramTotalsProxy::getTotal( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getTotal( idStat, whichColumn, whichPlane );
}

TSemanticValue HistogramTotalsProxy::getAverage( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getAverage( idStat, whichColumn, whichPlane );
}

TSemanticValue HistogramTotalsProxy::getMaximum( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getMaximum( idStat, whichColumn, whichPlane );
}

TSemanticValue HistogramTotalsProxy::getMinimum( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getMinimum( idStat, whichColumn, whichPlane );
}

TSemanticValue HistogramTotalsProxy::getStdev( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getStdev( idStat, whichColumn, whichPlane );
}

TSemanticValue HistogramTotalsProxy::getAvgDivMax( PRV_UINT16 idStat,
    THistogramColumn whichColumn,
    THistogramColumn whichPlane ) const
{
  return myTotals->getAvgDivMax( idStat, whichColumn, whichPlane );
}

void HistogramTotalsProxy::getAll( vector<TSemanticValue>& where,
                                   PRV_UINT16 idStat,
                                   THistogramColumn whichColumn,
                                   THistogramColumn whichPlane ) const
{
  myTotals->getAll( where, idStat, whichColumn, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByTotal( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByTotal( idStat, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByAverage( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByAverage( idStat, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByMaximum( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByMaximum( idStat, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByMinimum( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByMinimum( idStat, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByStdev( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByStdev( idStat, whichPlane );
}

vector<int>& HistogramTotalsProxy::sortByAvgDivMax( PRV_UINT16 idStat,
    THistogramColumn whichPlane )
{
  return myTotals->sortByAvgDivMax( idStat, whichPlane );
}
