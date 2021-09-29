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


#include <string>
#include <iostream>

template <typename ValueType>
Cube<ValueType>::Cube( PRV_UINT32 numPlanes, PRV_UINT32 numCols, PRV_UINT16 numStats ):
  nplanes( 0 ), ncols( numCols ), nstat( numStats ), crow( 0 )
{
  planes.assign( numPlanes, nullptr );
}


template <typename ValueType>
Cube<ValueType>::Cube( Cube<ValueType>& source ):
  nplanes( source.nplanes ), ncols( source.ncols ), nstat( source.nstat ), crow( source.crow )
{
  typename std::vector< Matrix<ValueType> *>::iterator it_mat;

  for ( it_mat = source.planes.begin(); it_mat != source.planes.end(); ++it_mat )
    if ( *it_mat != nullptr )
      planes.push_back( new Matrix<ValueType>( **it_mat ) );
    else
      planes.push_back( nullptr );
}


template <typename ValueType>
Cube<ValueType>::~Cube()
{
  if ( nplanes > 0 )
  {
    for ( PRV_UINT32 ii = 0; ii < planes.size(); ++ii )
    {
      if ( planes[ ii ] != nullptr )
        delete planes[ ii ];
    }
  }
  planes.clear();
}


template <typename ValueType>
inline void Cube<ValueType>::init( PRV_UINT16 idStat )
{
  if ( nplanes > 0 )
  {
    for ( PRV_UINT32 ii = 0; ii < planes.size(); ++ii )
    {
      if ( planes[ ii ] != nullptr )
        planes[ ii ]->init( idStat );
    }
  }
}


template <typename ValueType>
inline void Cube<ValueType>::init( )
{
  if ( nplanes > 0 )
  {
    for ( PRV_UINT32 ii = 0; ii < planes.size(); ++ii )
    {
      if ( planes[ ii ] != nullptr )
        planes[ ii ]->init( );
    }
  }
}


template <typename ValueType>
inline void Cube<ValueType>::setValue( PRV_UINT32 plane, PRV_UINT32 col, PRV_UINT16 idStat, ValueType semVal )
{
  if ( planes[ plane ] == nullptr )
  {
    planes[ plane ] = new Matrix<ValueType>( crow, ncols, nstat );
    ++nplanes;
  }

  planes[ plane ]->setValue( col, idStat, semVal );
}


template <typename ValueType>
inline void Cube<ValueType>::setValue( PRV_UINT32 plane, PRV_UINT32 col, ValueType semVal )
{
  if ( planes[ plane ] == nullptr )
  {
    planes[ plane ] = new Matrix<ValueType>( crow, ncols, nstat );
    ++nplanes;
  }

  planes[ plane ]->setValue( col, semVal );
}


template <typename ValueType>
inline void Cube<ValueType>::setValue( PRV_UINT32 plane, PRV_UINT32 col, const std::vector<ValueType>& semVal, bool isNotZeroValue )
{
  if ( planes[ plane ] == nullptr )
  {
    planes[ plane ] = new Matrix<ValueType>( crow, ncols, nstat );
    ++nplanes;
  }

  planes[ plane ]->setValue( col, semVal, isNotZeroValue );
}


template <typename ValueType>
inline void Cube<ValueType>::addValue( PRV_UINT32 plane, PRV_UINT32 col, PRV_UINT16 idStat, ValueType semVal )
{
  if ( planes[ plane ] == nullptr )
  {
    planes[ plane ] = new Matrix<ValueType>( crow, ncols, nstat );
    ++nplanes;
  }

  planes[ plane ]->addValue( col, idStat, semVal );
}


template <typename ValueType>
inline void Cube<ValueType>::addValue( PRV_UINT32 plane, PRV_UINT32 col, ValueType semVal )
{
  if ( planes[ plane ] == nullptr )
  {
    planes[ plane ] = new Matrix<ValueType>( crow, ncols, nstat );
    ++nplanes;
  }

  planes[ plane ]->addValue( col, semVal );
}


template <typename ValueType>
inline void Cube<ValueType>::addValue( PRV_UINT32 plane, PRV_UINT32 col, const std::vector<ValueType>& semVal )
{
  if ( planes[ plane ] == nullptr )
  {
    planes[ plane ] = new Matrix<ValueType>( crow, ncols, nstat );
    ++nplanes;
  }

  planes[ plane ]->addValue( col, semVal );
}


template <typename ValueType>
inline ValueType Cube<ValueType>::getCurrentValue( PRV_UINT32 plane, PRV_UINT32 col, PRV_UINT16 idStat ) const
{
  if ( planes[ plane ] != nullptr )
    return planes[ plane ]->getCurrentValue( col, idStat );

  ValueType tmp;

  memset( ( void * ) &tmp, 0, sizeof( ValueType ) );
  return tmp;
}


template <typename ValueType>
inline std::vector<ValueType> Cube<ValueType>::getCurrentValue( PRV_UINT32 plane, PRV_UINT32 col ) const
{
  if ( planes[ plane ] != nullptr )
    return planes[ plane ]->getCurrentValue( col );

  std::vector<ValueType> tmp;

  return tmp;
}


template <typename ValueType>
inline PRV_UINT32 Cube<ValueType>::getCurrentRow( PRV_UINT32 plane, PRV_UINT32 col ) const
{
  if ( planes[ plane ] != nullptr )
    return planes[ plane ]->getCurrentRow( col );
  return -1;
}


template <typename ValueType>
inline PRV_UINT32 Cube<ValueType>::getCurrentRow( ) const
{
  return crow;
}

template <typename ValueType>
inline bool Cube<ValueType>::currentCellModified( PRV_UINT32 plane, PRV_UINT32 col ) const
{
  if ( planes[ plane ] != nullptr )
    return planes[ plane ]->currentCellModified( col );
  return false;
}


template <typename ValueType>
inline void Cube<ValueType>::newRow( )
{
  if ( nplanes > 0 )
  {
    for ( PRV_UINT32 ii = 0; ii < planes.size(); ++ii )
    {
      if ( planes[ ii ] != nullptr )
        planes[ ii ]->newRow( );
    }
  }
  ++crow;
}


template <typename ValueType>
inline void Cube<ValueType>::newRow( PRV_UINT32 plane, PRV_UINT32 col, PRV_UINT32 row )
{
  if ( nplanes > 0 )
  {
    if ( planes[ plane ] != nullptr )
      planes[ plane ]->newRow( col, row );
  }

  crow = row;
}


template <typename ValueType>
inline void Cube<ValueType>::finish( )
{
  if ( nplanes > 0 )
  {
    for ( PRV_UINT32 ii = 0; ii < planes.size(); ++ii )
    {
      if ( planes[ ii ] != nullptr )
        planes[ ii ]->finish();
    }
  }
}


template <typename ValueType>
inline void Cube<ValueType>::setNextCell( PRV_UINT32 plane, PRV_UINT32 col )
{
  if ( nplanes == 0 )
    return;

  if ( planes[ plane ] != nullptr )
    planes[ plane ]->setNextCell( col );
}


template <typename ValueType>
inline void Cube<ValueType>::setFirstCell( PRV_UINT32 plane, PRV_UINT32 col )
{
  if ( nplanes == 0 )
    return;

  if ( planes[ plane ] != nullptr )
    planes[ plane ]->setFirstCell( col );
}


template <typename ValueType>
inline bool Cube<ValueType>::endCell( PRV_UINT32 plane, PRV_UINT32 col )
{
  if ( nplanes == 0 )
    return true;

  if ( planes[ plane ] != nullptr )
    return planes[ plane ]->endCell( col );
  return true;
}


template <typename ValueType>
inline bool Cube<ValueType>::planeWithValues( PRV_UINT32 plane ) const
{
  if ( nplanes == 0 )
    return false;

  return ( planes[ plane ] != nullptr );
}


template <typename ValueType>
inline PRV_UINT32 Cube<ValueType>::getPlanes( ) const
{
  if ( nplanes > 0 )
    return planes.size();

  return nplanes;
}


template <typename ValueType>
inline void Cube<ValueType>::eraseColumns( PRV_UINT32 ini_col, PRV_UINT32 fin_col )
{
  if ( fin_col < ini_col )
    return;

  if ( fin_col >= ncols )
    return;

  for ( PRV_UINT32 ii = 0; ii < planes.size(); ++ii )
  {
    if ( planes[ ii ] != nullptr )
      planes[ ii ]->eraseColumns( ini_col, fin_col );
  }

  ncols = ncols - ( fin_col - ini_col );
}


template <typename ValueType>
inline void Cube<ValueType>::erasePlanes( PRV_UINT32 ini_plane, PRV_UINT32 fin_plane )
{
  if ( fin_plane < ini_plane )
    return;

  if ( fin_plane >= ( PRV_UINT32 ) planes.size() )
    return;

  typename std::vector<Matrix<ValueType> *>::iterator it_ini, it_fin;
  PRV_UINT32 i;

  it_ini = planes.begin();
  for ( i = 0; i < ini_plane; ++i, ++it_ini );

  for ( it_fin = it_ini; i < fin_plane; ++i, ++it_fin )
  {
    if ( planes[ i ] != nullptr )
    {
      delete planes[ i ];
      nplanes--;
    }
  }

  planes.erase( it_ini, it_fin );
}


template <typename ValueType>
inline bool Cube<ValueType>::getCellValue( ValueType& semVal,
                                           PRV_UINT32 whichPlane,
                                           int whichRow,
                                           PRV_UINT32 whichCol,
                                           PRV_UINT16 idStat ) const
{
  if( !planeWithValues( whichPlane ) )
    return false;

  return planes[ whichPlane ]->getCellValue( semVal, whichRow, whichCol, idStat );
}

template <typename ValueType>
inline bool Cube<ValueType>::getNotZeroValue( PRV_UINT32 whichPlane,
                                              int whichRow,
                                              PRV_UINT32 whichCol, 
                                              PRV_UINT16 idStat ) const
{
  if( !planeWithValues( whichPlane ) )
    return false;

  return planes[ whichPlane ]->getNotZeroValue( whichRow, whichCol, idStat );
}


template <typename ValueType>
inline bool Cube<ValueType>::getCellValue( std::vector<ValueType>& semVal,
                                           PRV_UINT32 whichPlane,
                                           int whichRow,
                                           PRV_UINT32 whichCol ) const
{
  if( !planeWithValues( whichPlane ) )
    return false;

  return planes[ whichPlane ]->getCellValue( semVal, whichRow, whichCol );
}


template <typename ValueType>
inline void Cube<ValueType>::print() const
{
  if ( nplanes > 0 )
  {
    for ( PRV_UINT32 ii = 0; ii < planes.size(); ++ii )
    {
      std::cout << std::endl;
      std::cout << "******************************************************" << std::endl;
      std::cout << "----------Plane " << ii << "----------" << std::endl;
      if ( planes[ ii ] != nullptr )
        planes[ ii ]->print();
    }
  }
}
