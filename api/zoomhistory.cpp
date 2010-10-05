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

template <typename Dimension1, typename Dimension2>
ZoomHistory<Dimension1,Dimension2>::ZoomHistory( )
{
  currentZoom = -1;
}

template <typename Dimension1, typename Dimension2>
ZoomHistory<Dimension1,Dimension2>::~ZoomHistory( )
{
}

template <typename Dimension1, typename Dimension2>
pair<Dimension1, Dimension1> ZoomHistory<Dimension1,Dimension2>::getFirstDimension() const
{
  return zooms[ currentZoom ].first;
}

template <typename Dimension1, typename Dimension2>
pair<Dimension2, Dimension2> ZoomHistory<Dimension1,Dimension2>::getSecondDimension() const
{
  return zooms[ currentZoom ].second;
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::setFirstDimension( pair<Dimension1, Dimension1> &dim )
{
  zooms[ currentZoom ].first = dim;
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::setSecondDimension(  pair<Dimension2, Dimension2> &dim )
{
  zooms[ currentZoom ].second = dim;
}


template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::addZoom( Dimension1 begin1, Dimension1 end1,
                                                  Dimension2 begin2, Dimension2 end2 )
{
  if ( currentZoom < (int)zooms.size() - 1 )
    zooms.resize( currentZoom + 1 );

  zooms.push_back( make_pair( make_pair(begin1,end1), make_pair(begin2,end2)));
  currentZoom = zooms.size() - 1;
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::addZoom( Dimension1 begin, Dimension1 end )
{
  if ( currentZoom < (int)zooms.size() - 1 )
    zooms.resize( currentZoom + 1 );

  zooms.push_back( make_pair( make_pair( begin,end ),
                              make_pair( zooms[currentZoom].second.first, zooms[currentZoom].second.second )));
  ++currentZoom;
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::addZoom( Dimension2 begin, Dimension2 end )
{
  if ( currentZoom < (int)zooms.size() - 1 )
    zooms.resize( currentZoom + 1 );

  zooms.push_back( make_pair( make_pair( zooms[currentZoom].first.first, zooms[currentZoom].first.second ),
                              make_pair( begin, end )));
  ++currentZoom;
}

template <typename Dimension1, typename Dimension2>
bool ZoomHistory<Dimension1,Dimension2>::isEmpty() const
{
  return ( zooms.begin() == zooms.end() );
}

template <typename Dimension1, typename Dimension2>
bool ZoomHistory<Dimension1,Dimension2>::emptyNextZoom() const
{
  return ( currentZoom == int( zooms.size() - 1 ) );
}

template <typename Dimension1, typename Dimension2>
bool ZoomHistory<Dimension1,Dimension2>::emptyPrevZoom() const
{
  return ( currentZoom <= 0 );
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::firstZoom( )
{
  currentZoom = 0;
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::prevZoom( )
{
  if ( currentZoom > 0 )
    currentZoom--;
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::nextZoom( )
{
  if ( currentZoom < int(zooms.size() - 1) )
    currentZoom++;
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::clear()
{
  zooms.clear();
}
