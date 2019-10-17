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
Matrix<ValueType>::Matrix( PRV_UINT32 numCols, PRV_UINT16 numStats ):
  finished( false )
{
  cols.insert( cols.begin(), numCols, Column<ValueType>( numStats, &finished ) );
}


template <typename ValueType>
Matrix<ValueType>::Matrix( TObjectOrder currentRow, PRV_UINT32 numCols, PRV_UINT16 numStats ):
  finished( false )
{
  cols.insert( cols.begin(), numCols, Column<ValueType>( currentRow, numStats, &finished ) );
}


template <typename ValueType>
Matrix<ValueType>::Matrix( Matrix<ValueType>& source ):
  finished( source.finished )
{
  cols = source.cols;
}


template <typename ValueType>
Matrix<ValueType>::~Matrix()
{}


template <typename ValueType>
inline void Matrix<ValueType>::init( PRV_UINT16 idStat )
{
  for ( PRV_UINT32 ii = 0; ii < ( PRV_UINT32 )cols.size(); ii++ )
  {
    cols[ ii ].init( idStat );
  }
}


template <typename ValueType>
inline void Matrix<ValueType>::init( )
{
  for ( PRV_UINT32 ii = 0; ii < ( PRV_UINT32 )cols.size(); ii++ )
  {
    cols[ ii ].init( );
  }
}


template <typename ValueType>
inline void Matrix<ValueType>::setValue( PRV_UINT32 col, PRV_UINT16 idStat, ValueType semVal )
{
  cols[ col ].setValue( idStat, semVal );
}


template <typename ValueType>
inline void Matrix<ValueType>::setValue( PRV_UINT32 col, ValueType semVal )
{
  cols[ col ].setValue( semVal );
}


template <typename ValueType>
inline void Matrix<ValueType>::setValue( PRV_UINT32 col, const std::vector<ValueType>& semVal )
{
  cols[ col ].setValue( semVal );
}


template <typename ValueType>
inline void Matrix<ValueType>::addValue( PRV_UINT32 col, PRV_UINT16 idStat, ValueType semVal )
{
  cols[ col ].addValue( idStat, semVal );
}


template <typename ValueType>
inline void Matrix<ValueType>::addValue( PRV_UINT32 col, ValueType semVal )
{
  cols[ col ].addValue( semVal );
}


template <typename ValueType>
inline void Matrix<ValueType>::addValue( PRV_UINT32 col, const std::vector<ValueType>& semVal )
{
  cols[ col ].addValue( semVal );
}


template <typename ValueType>
inline ValueType Matrix<ValueType>::getCurrentValue( PRV_UINT32 col, PRV_UINT16 idStat ) const
{
  return cols[ col ].getCurrentValue( idStat );
}


template <typename ValueType>
inline std::vector<ValueType> Matrix<ValueType>::getCurrentValue( PRV_UINT32 col ) const
{
  return cols[ col ].getCurrentValue();
}


template <typename ValueType>
inline TObjectOrder Matrix<ValueType>::getCurrentRow( PRV_UINT32 col ) const
{
  return cols[ col ].getCurrentRow();
}


template <typename ValueType>
inline bool Matrix<ValueType>::currentCellModified( PRV_UINT32 col ) const
{
  return cols[ col ].currentCellModified();
}


template <typename ValueType>
inline void Matrix<ValueType>::newRow( )
{
  for ( PRV_UINT32 ii = 0; ii < ( PRV_UINT32 )cols.size(); ii++ )
  {
    cols[ ii ].newRow();
  }
}


template <typename ValueType>
inline void Matrix<ValueType>::newRow( PRV_UINT32 col, TObjectOrder row )
{
  cols[ col ].newRow( row );
}


template <typename ValueType>
inline void Matrix<ValueType>::finish( )
{
  finished = true;
}


template <typename ValueType>
inline void Matrix<ValueType>::setNextCell( PRV_UINT32 col )
{
  cols[ col ].setNextCell();
}


template <typename ValueType>
inline void Matrix<ValueType>::setFirstCell( PRV_UINT32 col )
{
  cols[ col ].setFirstCell();
}


template <typename ValueType>
inline bool Matrix<ValueType>::endCell( PRV_UINT32 col )
{
  return cols[ col ].endCell();
}


template <typename ValueType>
inline void Matrix<ValueType>::eraseColumns( PRV_UINT32 ini_col, PRV_UINT32 fin_col )
{
  if ( fin_col < ini_col )
    return;

  if ( ini_col < 0 )
    return;

  if ( fin_col >= ( int ) cols.size() )
    return;

  typename std::vector<Column<ValueType> >::iterator it_ini, it_fin;
  PRV_UINT32 i;

  it_ini = cols.begin();
  for ( i = 0; i < ini_col; i++, ++it_ini );

  for ( it_fin = it_ini; i < fin_col; i++, ++it_fin );

  cols.erase( it_ini, it_fin );
}


template <typename ValueType>
inline bool Matrix<ValueType>::getCellValue( ValueType& semVal,
    int whichRow,
    PRV_UINT32 whichCol,
    PRV_UINT16 idStat ) const
{
  return cols[ whichCol ].getCellValue( semVal, whichRow, idStat );
}


template <typename ValueType>
inline bool Matrix<ValueType>::getCellValue( std::vector<ValueType>& semVal,
    int whichRow,
    PRV_UINT32 whichCol ) const
{
  return cols[ whichCol ].getCellValue( semVal, whichRow );
}


template <typename ValueType>
inline void Matrix<ValueType>::print() const
{
  for ( PRV_UINT32 ii = 0; ii < ( PRV_UINT32 )cols.size(); ii++ )
  {
    std::cout << "----------Column " << ii << "----------" << std::endl;
    cols[ ii ].print();
  }
}
