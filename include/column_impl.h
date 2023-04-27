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
Column<ValueType, NStats>::Column( bool *mat_finished ):
  modified( false ), finished( mat_finished )
{
  current_cell = Cell<ValueType, NStats>( 0 );
}


template <typename ValueType, size_t NStats>
Column<ValueType, NStats>::Column( int currentRow, bool *mat_finished ):
  modified( false ), finished( mat_finished )
{
  current_cell = Cell<ValueType, NStats>( currentRow );
}


template <typename ValueType, size_t NStats>
Column<ValueType, NStats>::Column( const Column<ValueType, NStats>& source ):
  modified( source.modified ), finished( source.finished )
{
  current_cell = Cell<ValueType, NStats>( source.current_cell );

  cells = source.cells;
}


template <typename ValueType, size_t NStats>
inline void Column<ValueType, NStats>::init( short idStat )
{
  current_cell.init( idStat );
}


template <typename ValueType, size_t NStats>
inline void Column<ValueType, NStats>::init( )
{
  current_cell.init( );
}


template <typename ValueType, size_t NStats>
inline void Column<ValueType, NStats>::setValue( short idStat, ValueType semVal )
{
  if ( *finished )
  {
    it_cell->setValue( idStat, semVal );
  }
  else
  {
    if ( modified == false )
    {
      cells.push_back( current_cell );
      modified = true;
    }
    cells.back().setValue( idStat, semVal );
  }
}


template <typename ValueType, size_t NStats>
inline void Column<ValueType, NStats>::setValue( const std::array<ValueType, NStats>& semVal, bool isNotZeroValue )
{
  if ( *finished )
  {
    it_cell->setValue( semVal, isNotZeroValue );
  }
  else
  {
    if ( modified == false )
    {
      cells.push_back( current_cell );
      modified = true;
    }
    cells.back().setValue( semVal, isNotZeroValue );
  }
}


template <typename ValueType, size_t NStats>
inline void Column<ValueType, NStats>::addValue( short idStat, ValueType semVal )
{
  if ( modified == false )
  {
    cells.push_back( current_cell );
    modified = true;
  }
  cells.back().addValue( idStat, semVal );
}


template <typename ValueType, size_t NStats>
inline void Column<ValueType, NStats>::addValue( const std::array<ValueType, NStats>& semVal )
{
  if ( modified == false )
  {
    cells.push_back( current_cell );
    modified = true;
  }
  cells.back().addValue( semVal );
}


template <typename ValueType, size_t NStats>
inline ValueType Column<ValueType, NStats>::getCurrentValue( short idStat ) const
{
  if ( *finished )
  {
    return it_cell->getValue( idStat );
  }

  if( modified )
    return cells.back().getValue( idStat );

  return current_cell.getValue( idStat );
}


template <typename ValueType, size_t NStats>
inline std::array<ValueType, NStats> Column<ValueType, NStats>::getCurrentValue() const
{
  if ( *finished )
  {
    return it_cell->getValue();
  }

  if( modified )
    return cells.back().getValue();

  return current_cell.getValue();
}


template <typename ValueType, size_t NStats>
inline int Column<ValueType, NStats>::getCurrentRow( ) const
{
  if ( *finished )
  {
    if ( cells.empty() )
      return -1;
    else
      return it_cell->getRow();
  }

  if( modified )
    return cells.back().getRow();

  return current_cell.getRow();
}


template <typename ValueType, size_t NStats>
inline bool Column<ValueType, NStats>::currentCellModified( ) const
{
  return modified;
}


template <typename ValueType, size_t NStats>
inline void Column<ValueType, NStats>::newRow( )
{
  int tmp_row = getCurrentRow();

  if ( modified )
  {
    current_cell = Cell<ValueType, NStats>( tmp_row + 1 );
    modified = false;
  }
  else
  {
    current_cell.setRow( tmp_row + 1 );
  }
}


template <typename ValueType, size_t NStats>
inline void Column<ValueType, NStats>::newRow( int row )
{
  if ( modified )
  {
    current_cell = Cell<ValueType, NStats>( row );
    modified = false;
  }
  else
  {
    current_cell.setRow( row );
  }
}


template <typename ValueType, size_t NStats>
inline void Column<ValueType, NStats>::setNextCell( )
{
  ++it_cell;
}


template <typename ValueType, size_t NStats>
inline void Column<ValueType, NStats>::setFirstCell( )
{
  it_cell = cells.begin();
}


template <typename ValueType, size_t NStats>
inline bool Column<ValueType, NStats>::endCell( )
{
  return ( it_cell == cells.end() );
}


template <typename ValueType, size_t NStats>
inline bool Column<ValueType, NStats>::getCellValue( ValueType& semVal,
                                                     int whichRow,
                                                     short idStat ) const
{
  Cell<ValueType, NStats> tmpCell( whichRow );

  typename std::vector<Cell<ValueType, NStats> >::const_iterator result;
  for( result = cells.begin(); result != cells.end(); ++result )
    if( result->getRow() == whichRow ) break;

  if( result == cells.end() )
    return false;
  else
    semVal = result->getValue( idStat );

  return true;
}


template <typename ValueType, size_t NStats>
inline bool Column<ValueType, NStats>::getNotZeroValue( int whichRow, short idStat ) const
{
  Cell<ValueType, NStats> tmpCell( whichRow );

  typename std::vector<Cell<ValueType, NStats> >::const_iterator result;
  for( result = cells.begin(); result != cells.end(); ++result )
    if( result->getRow() == whichRow ) break;

  if( result != cells.end() )
    return result->getNotZeroValue( idStat );

  return false;
}


template <typename ValueType, size_t NStats>
inline bool Column<ValueType, NStats>::getCellValue( std::array<ValueType, NStats>& semVal,
                                                     int whichRow ) const
{
  Cell<ValueType, NStats> tmpCell( whichRow, 0 );

  typename std::vector<Cell<ValueType, NStats> >::iterator result = find( cells.begin(), cells.end(), tmpCell );
  if( result == cells.end() )
    return false;
  else
    semVal = result->getValue();

  return true;
}
