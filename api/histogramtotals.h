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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef HISTOGRAMTOTALS_H_INCLUDED
#define HISTOGRAMTOTALS_H_INCLUDED

#include "paraverkerneltypes.h"

class HistogramTotals
{
  public:
    static HistogramTotals *create( HistogramTotals *whichTotals );

    virtual ~HistogramTotals() {}

    virtual TSemanticValue getTotal( PRV_UINT16 idStat,
                                     THistogramColumn whichColumn,
                                     THistogramColumn whichPlane = 0 ) const = 0;
    virtual TSemanticValue getAverage( PRV_UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const = 0;
    virtual TSemanticValue getMaximum( PRV_UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const = 0;
    virtual TSemanticValue getMinimum( PRV_UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const = 0;
    virtual TSemanticValue getStdev( PRV_UINT16 idStat,
                                     THistogramColumn whichColumn,
                                     THistogramColumn whichPlane = 0 ) const = 0;
    virtual TSemanticValue getAvgDivMax( PRV_UINT16 idStat,
                                         THistogramColumn whichColumn,
                                         THistogramColumn whichPlane = 0 ) const = 0;
    virtual void getAll( std::vector<TSemanticValue>& where,
                         PRV_UINT16 idStat,
                         THistogramColumn whichColumn,
                         THistogramColumn whichPlane = 0 ) const = 0;

    virtual std::vector<int>& sortByTotal( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;
    virtual std::vector<int>& sortByAverage( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;
    virtual std::vector<int>& sortByMaximum( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;
    virtual std::vector<int>& sortByMinimum( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;
    virtual std::vector<int>& sortByStdev( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;
    virtual std::vector<int>& sortByAvgDivMax( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 ) = 0;

  protected:

  private:

};

class HistogramTotalsProxy: public HistogramTotals
{
  public:
    virtual ~HistogramTotalsProxy() {}

    virtual TSemanticValue getTotal( PRV_UINT16 idStat,
                                     THistogramColumn whichColumn,
                                     THistogramColumn whichPlane = 0 ) const;
    virtual TSemanticValue getAverage( PRV_UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const;
    virtual TSemanticValue getMaximum( PRV_UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const;
    virtual TSemanticValue getMinimum( PRV_UINT16 idStat,
                                       THistogramColumn whichColumn,
                                       THistogramColumn whichPlane = 0 ) const;
    virtual TSemanticValue getStdev( PRV_UINT16 idStat,
                                     THistogramColumn whichColumn,
                                     THistogramColumn whichPlane = 0 ) const;
    virtual TSemanticValue getAvgDivMax( PRV_UINT16 idStat,
                                         THistogramColumn whichColumn,
                                         THistogramColumn whichPlane = 0 ) const;
    virtual void getAll( std::vector<TSemanticValue>& where,
                         PRV_UINT16 idStat,
                         THistogramColumn whichColumn,
                         THistogramColumn whichPlane = 0 ) const;

    virtual std::vector<int>& sortByTotal( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );
    virtual std::vector<int>& sortByAverage( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );
    virtual std::vector<int>& sortByMaximum( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );
    virtual std::vector<int>& sortByMinimum( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );
    virtual std::vector<int>& sortByStdev( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );
    virtual std::vector<int>& sortByAvgDivMax( PRV_UINT16 idStat, THistogramColumn whichPlane = 0 );

  protected:

  private:
    HistogramTotalsProxy( HistogramTotals *whichTotals );

    HistogramTotals *myTotals;

    friend HistogramTotals *HistogramTotals::create( HistogramTotals * );
};

#endif // HISTOGRAMTOTALS_H_INCLUDED
