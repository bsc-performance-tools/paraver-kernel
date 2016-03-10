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

template<typename Dimension1, typename Dimension2, typename Dimension3, typename T>
CubeContainer<Dimension1, Dimension2, Dimension3, T>::Index::Index( Dimension1 whichDim1, Dimension2 whichDim2, Dimension3 whichDim3 ) :
  dim1( whichDim1 ), dim2( whichDim2 ), dim3( whichDim3 )
{
}

template<typename Dimension1, typename Dimension2, typename Dimension3, typename T>
#ifdef WIN32
bool typename CubeContainer<Dimension1, Dimension2, Dimension3, T>::Index::operator<( const typename CubeContainer<Dimension1, Dimension2, Dimension3, T>::Index& whichIndex ) const
#else
bool CubeContainer<Dimension1, Dimension2, Dimension3, T>::Index::operator<( const CubeContainer<Dimension1, Dimension2, Dimension3, T>::Index& whichIndex ) const
#endif
{
  if( dim1 > whichIndex.dim1 )
    return false;
  else if( dim1 == whichIndex.dim1 )
  {
    if( dim2 > whichIndex.dim2 )
      return false;
    else if( dim2 == whichIndex.dim2 )
    {
      if( dim3 >= whichIndex.dim3 )
        return false;
    }
  }

  return true;
}

template<typename Dimension1, typename Dimension2, typename Dimension3, typename T>
T& CubeContainer<Dimension1, Dimension2, Dimension3, T>::operator()( const Dimension1& dim1, const Dimension2& dim2, const Dimension3& dim3 )
{
  return cube[ typename CubeContainer::Index( dim1, dim2, dim3 ) ];
}

template<typename Dimension1, typename Dimension2, typename Dimension3, typename T>
const T& CubeContainer<Dimension1, Dimension2, Dimension3, T>::operator()( const Dimension1& dim1, const Dimension2& dim2, const Dimension3& dim3 ) const
{
  return cube[ typename CubeContainer::Index( dim1, dim2, dim3 ) ];
}

template<typename Dimension1, typename Dimension2, typename Dimension3, typename T>
size_t CubeContainer<Dimension1, Dimension2, Dimension3, T>::size() const
{
  return cube.size();
}

template<typename Dimension1, typename Dimension2, typename Dimension3, typename T>
typename CubeContainer<Dimension1, Dimension2, Dimension3, T>::const_iterator CubeContainer<Dimension1, Dimension2, Dimension3, T>::find( const Dimension1& dim1, const Dimension2& dim2, const Dimension3& dim3 ) const
{
  return cube.find( typename CubeContainer::Index( dim1, dim2, dim3 ) );
}

template<typename Dimension1, typename Dimension2, typename Dimension3, typename T>
typename CubeContainer<Dimension1, Dimension2, Dimension3, T>::iterator CubeContainer<Dimension1, Dimension2, Dimension3, T>::find( const Dimension1& dim1, const Dimension2& dim2, const Dimension3& dim3 )
{
  return cube.find( typename CubeContainer::Index( dim1, dim2, dim3 ) );
}

template<typename Dimension1, typename Dimension2, typename Dimension3, typename T>
typename CubeContainer<Dimension1, Dimension2, Dimension3, T>::iterator CubeContainer<Dimension1, Dimension2, Dimension3, T>::begin()
{
  return cube.begin();
}

template<typename Dimension1, typename Dimension2, typename Dimension3, typename T>
typename CubeContainer<Dimension1, Dimension2, Dimension3, T>::iterator CubeContainer<Dimension1, Dimension2, Dimension3, T>::end()
{
  return cube.end();
}

