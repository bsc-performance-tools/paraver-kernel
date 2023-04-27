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

template <typename ValueType, size_t NStats>
Matrix<ValueType, NStats>::Matrix( PRV_UINT32 numCols ):
  finished( false )
{
  cols.insert( cols.begin(), numCols, Column<ValueType, NStats>( &finished ) );
}


template <typename ValueType, size_t NStats>
Matrix<ValueType, NStats>::Matrix( TObjectOrder currentRow, PRV_UINT32 numCols ):
  finished( false )
{
  cols.insert( cols.begin(), numCols, Column<ValueType, NStats>( currentRow, &finished ) );
}


template <typename ValueType, size_t NStats>
Matrix<ValueType, NStats>::Matrix( Matrix<ValueType, NStats>& source ):
  finished( source.finished )
{
  cols = source.cols;
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::init( PRV_UINT16 idStat )
{
  for ( PRV_UINT32 ii = 0; ii < ( PRV_UINT32 )cols.size(); ii++ )
  {
    cols[ ii ].init( idStat );
  }
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::init( )
{
  for ( PRV_UINT32 ii = 0; ii < ( PRV_UINT32 )cols.size(); ii++ )
  {
    cols[ ii ].init( );
  }
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::setValue( PRV_UINT32 col, PRV_UINT16 idStat, ValueType semVal )
{
  cols[ col ].setValue( idStat, semVal );
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::setValue( PRV_UINT32 col, const std::array<ValueType, NStats>& semVal, bool isNotZeroValue )
{
  cols[ col ].setValue( semVal, isNotZeroValue );
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::addValue( PRV_UINT32 col, PRV_UINT16 idStat, ValueType semVal )
{
  cols[ col ].addValue( idStat, semVal );
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::addValue( PRV_UINT32 col, const std::array<ValueType, NStats>& semVal )
{
  cols[ col ].addValue( semVal );
}


template <typename ValueType, size_t NStats>
inline ValueType Matrix<ValueType, NStats>::getCurrentValue( PRV_UINT32 col, PRV_UINT16 idStat ) const
{
  return cols[ col ].getCurrentValue( idStat );
}


template <typename ValueType, size_t NStats>
inline std::array<ValueType, NStats> Matrix<ValueType, NStats>::getCurrentValue( PRV_UINT32 col ) const
{
  return cols[ col ].getCurrentValue();
}


template <typename ValueType, size_t NStats>
inline TObjectOrder Matrix<ValueType, NStats>::getCurrentRow( PRV_UINT32 col ) const
{
  return cols[ col ].getCurrentRow();
}


template <typename ValueType, size_t NStats>
inline bool Matrix<ValueType, NStats>::currentCellModified( PRV_UINT32 col ) const
{
  return cols[ col ].currentCellModified();
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::newRow( )
{
  for ( PRV_UINT32 ii = 0; ii < ( PRV_UINT32 )cols.size(); ii++ )
  {
    cols[ ii ].newRow();
  }
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::newRow( PRV_UINT32 col, TObjectOrder row )
{
  cols[ col ].newRow( row );
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::finish( )
{
  finished = true;
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::setNextCell( PRV_UINT32 col )
{
  cols[ col ].setNextCell();
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::setFirstCell( PRV_UINT32 col )
{
  cols[ col ].setFirstCell();
}


template <typename ValueType, size_t NStats>
inline bool Matrix<ValueType, NStats>::endCell( PRV_UINT32 col )
{
  return cols[ col ].endCell();
}


template <typename ValueType, size_t NStats>
inline void Matrix<ValueType, NStats>::eraseColumns( PRV_UINT32 ini_col, PRV_UINT32 fin_col )
{
  if ( fin_col < ini_col )
    return;

  if ( fin_col >= ( int ) cols.size() )
    return;

  typename std::vector<Column<ValueType, NStats> >::iterator it_ini, it_fin;
  PRV_UINT32 i;

  it_ini = cols.begin();
  for ( i = 0; i < ini_col; i++, ++it_ini );

  for ( it_fin = it_ini; i < fin_col; i++, ++it_fin );

  cols.erase( it_ini, it_fin );
}


template <typename ValueType, size_t NStats>
inline bool Matrix<ValueType, NStats>::getCellValue( ValueType& semVal,
                                                     int whichRow,
                                                     PRV_UINT32 whichCol,
                                                     PRV_UINT16 idStat ) const
{
  return cols[ whichCol ].getCellValue( semVal, whichRow, idStat );
}

template <typename ValueType, size_t NStats>
inline bool Matrix<ValueType, NStats>::getNotZeroValue( int whichRow,
                                                        PRV_UINT32 whichCol,
                                                        PRV_UINT16 idStat ) const
{
  return cols[ whichCol ].getNotZeroValue( whichRow, idStat );
}

template <typename ValueType, size_t NStats>
inline bool Matrix<ValueType, NStats>::getCellValue( std::array<ValueType, NStats>& semVal,
                                                     int whichRow,
                                                     PRV_UINT32 whichCol ) const
{
  return cols[ whichCol ].getCellValue( semVal, whichRow );
}
