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


#include <map>

template<typename Dimension1, typename Dimension2, typename Dimension3, typename T>
class CubeContainer
{
  public:
    struct Index
    {
      Dimension1 dim1;
      Dimension2 dim2;
      Dimension3 dim3;

      Index( Dimension1 whichDim1, Dimension2 whichDim2, Dimension3 whichDim3 );
      bool operator<( const Index& whichIndex ) const;
    };

    typedef typename std::map<Index, T>::iterator iterator;
    typedef typename std::map<Index, T>::iterator const_iterator;

    T& operator()( const Dimension1& dim1, const Dimension2& dim2, const Dimension3& dim3 );
    const T& operator()( const Dimension1& dim1, const Dimension2& dim2, const Dimension3& dim3 ) const;

    size_t size() const;
    typename CubeContainer::const_iterator find( const Dimension1& dim1, const Dimension2& dim2, const Dimension3& dim3 ) const;
    typename CubeContainer::iterator find( const Dimension1& dim1, const Dimension2& dim2, const Dimension3& dim3 );

    typename CubeContainer::iterator begin();
    typename CubeContainer::iterator end();

  private:
    std::map<Index, T> cube;

};

#include "cubecontainer.cpp"


