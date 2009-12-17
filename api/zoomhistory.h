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

#ifndef _ZOOMHISTORY_H
#define _ZOOMHISTORY_H

using namespace std;

//#include <utility>
#include <vector>

template <typename Dimension1, typename Dimension2>
class ZoomHistory
{
  public:
    ZoomHistory();
    ~ZoomHistory();

    void addZoom( Dimension1 begin1, Dimension1 end1,
                  Dimension2 begin2, Dimension2 end2 );
    void addZoom( Dimension2 begin, Dimension2 end );
    void addZoom( Dimension1 begin, Dimension1 end );

    pair<Dimension1, Dimension1> getFirstDimension() const;
    pair<Dimension2, Dimension2> getSecondDimension() const;

    bool isEmpty() const;
    bool emptyNextZoom() const;
    bool emptyPrevZoom() const;

    void firstZoom();
    void nextZoom();
    void prevZoom();

    void clear();
  private:
    int currentZoom;
    vector< pair< pair<Dimension1,Dimension1>, pair<Dimension2, Dimension2> > > zooms;
};

#include "zoomhistory.cpp"

#endif // _ZOOMHISTORY_H
