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


#pragma once


//#include <utility>
#include <vector>

enum class TZoomPosition { PREV_ZOOM, CURRENT_ZOOM, NEXT_ZOOM };

template <typename Dimension1, typename Dimension2>
class ZoomHistory
{
  public:

    ZoomHistory();
    ~ZoomHistory();

    void addZoom( Dimension1 begin1, Dimension1 end1,
                  Dimension2 begin2, Dimension2 end2 );
    void addZoom( Dimension1 begin, Dimension1 end );
    void addZoom( Dimension2 begin, Dimension2 end );

    void setFirstDimension( std::pair<Dimension1, Dimension1> &dim );
    void setSecondDimension( std::pair<Dimension2, Dimension2> &dim );

    std::pair<Dimension1, Dimension1> getFirstDimension( TZoomPosition pos = TZoomPosition::CURRENT_ZOOM ) const;
    std::pair<Dimension2, Dimension2> getSecondDimension( TZoomPosition pos = TZoomPosition::CURRENT_ZOOM ) const;

    bool isEmpty( TZoomPosition pos = TZoomPosition::CURRENT_ZOOM ) const;
    // bool emptyNextZoom() const;
    // bool emptyPrevZoom() const;

    void firstZoom();
    void nextZoom();
    void prevZoom();

    void clear();

  private:
    int currentZoom;
    std::vector< std::pair< std::pair<Dimension1,Dimension1>, std::pair<Dimension2, Dimension2> > > zooms;

    bool sameZoomAsCurrent( Dimension1 begin1, Dimension1 end1,
                            Dimension2 begin2, Dimension2 end2 ) const;
    bool sameZoomAsCurrent( Dimension1 begin, Dimension1 end ) const;
    bool sameZoomAsCurrent( Dimension2 begin, Dimension2 end ) const;
};

#include "zoomhistory_impl.h"


