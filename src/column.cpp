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
Column<ValueType>::Column( short numStats, bool *mat_finished ):
  nstat( numStats ), modified( false ), n_cells( 0 ), finished( mat_finished )
{
  current_cell = Cell<ValueType>( 0, nstat );
  //current_cell->init();
}


template <typename ValueType>
Column<ValueType>::Column( int currentRow, short numStats, bool *mat_finished ):
  nstat( numStats ), modified( false ), n_cells( 0 ), finished( mat_finished )
{
  current_cell = Cell<ValueType>( currentRow, nstat );
  //current_cell->init();
}


template <typename ValueType>
Column<ValueType>::Column( const Column<ValueType>& source ):
  nstat( source.nstat ), modified( source.modified ), n_cells( source.n_cells ), finished( source.finished )
{
  current_cell = Cell<ValueType>( source.current_cell );

  typename std::vector<Cell<ValueType> >::const_iterator it_aux;
  for ( it_aux = source.cells.begin(); it_aux != source.cells.end(); ++it_aux )
    cells.push_back( Cell<ValueType>( *it_aux ) );
}


template <typename ValueType>
Column<ValueType>::~Column()
{
  cells.clear();
}


template <typename ValueType>
inline void Column<ValueType>::init( short idStat )
{
  current_cell.init( idStat );
}


template <typename ValueType>
inline void Column<ValueType>::init( )
{
  current_cell.init( );
}


template <typename ValueType>
inline void Column<ValueType>::setValue( short idStat, ValueType semVal )
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
      ++n_cells;
      modified = true;
      //it_cell = cells.begin();
    }
    cells[ cells.size() - 1 ].setValue( idStat, semVal );
  }
}


template <typename ValueType>
inline void Column<ValueType>::setValue( ValueType semVal )
{
  if ( *finished )
  {
    it_cell->setValue( semVal );
  }
  else
  {
    if ( modified == false )
    {
      cells.push_back( current_cell );
      ++n_cells;
      modified = true;
      //it_cell = cells.begin();
    }
    cells[ cells.size() - 1 ].setValue( semVal );
  }
}


template <typename ValueType>
inline void Column<ValueType>::setValue( const std::vector<ValueType>& semVal, bool isNotZeroValue )
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
      ++n_cells;
      modified = true;
      //it_cell = cells.begin();
    }
    cells[ cells.size() - 1 ].setValue( semVal, isNotZeroValue );
  }
}


template <typename ValueType>
inline void Column<ValueType>::addValue( short idStat, ValueType semVal )
{
  if ( modified == false )
  {
    cells.push_back( current_cell );
    ++n_cells;
    modified = true;
    //it_cell = cells.begin();
  }
  cells[ cells.size() - 1 ].addValue( idStat, semVal );
}


template <typename ValueType>
inline void Column<ValueType>::addValue( ValueType semVal )
{
  if ( modified == false )
  {
    cells.push_back( current_cell );
    ++n_cells;
    modified = true;
    //it_cell = cells.begin();
  }
  cells[ cells.size() - 1 ].addValue( semVal );
}


template <typename ValueType>
inline void Column<ValueType>::addValue( const std::vector<ValueType>& semVal )
{
  if ( modified == false )
  {
    cells.push_back( current_cell );
    ++n_cells;
    modified = true;
    //it_cell = cells.begin();
  }
  cells[ cells.size() - 1 ].addValue( semVal );
}


template <typename ValueType>
inline ValueType Column<ValueType>::getCurrentValue( short idStat ) const
{
  if ( *finished )
  {
    return it_cell->getValue( idStat );
  }

  if( modified )
    return cells[ cells.size() - 1 ].getValue( idStat );

  return current_cell.getValue( idStat );
}


template <typename ValueType>
inline std::vector<ValueType> Column<ValueType>::getCurrentValue() const
{
  if ( *finished )
  {
    return it_cell->getValue();
  }

  if( modified )
    return cells[ cells.size() - 1 ].getValue();

  return current_cell.getValue();
}


template <typename ValueType>
inline int Column<ValueType>::getCurrentRow( ) const
{
  if ( *finished )
  {
    if ( n_cells == 0 )
      return -1;
    else
      return it_cell->getRow();
  }

  if( modified )
    return cells[ cells.size() - 1 ].getRow();

  return current_cell.getRow();
}


template <typename ValueType>
inline bool Column<ValueType>::currentCellModified( ) const
{
  return modified;
}


template <typename ValueType>
inline void Column<ValueType>::newRow( )
{
  int tmp_row = getCurrentRow();

  if ( modified )
  {
    current_cell = Cell<ValueType>( tmp_row + 1, nstat );
    //current_cell->init();
    modified = false;
  }
  else
  {
    current_cell.setRow( tmp_row + 1 );
  }
}


template <typename ValueType>
inline void Column<ValueType>::newRow( int row )
{
  if ( modified )
  {
    current_cell = Cell<ValueType>( row, nstat );
    //current_cell->init();
    modified = false;
  }
  else
  {
    current_cell.setRow( row );
  }
}


template <typename ValueType>
inline void Column<ValueType>::setNextCell( )
{
  ++it_cell;
}


template <typename ValueType>
inline void Column<ValueType>::setFirstCell( )
{
  it_cell = cells.begin();
}


template <typename ValueType>
inline bool Column<ValueType>::endCell( )
{
  return ( it_cell == cells.end() );
}

template <typename ValueType>
inline bool Column<ValueType>::getCellValue( ValueType& semVal,
    int whichRow,
    short idStat ) const
{
  Cell<ValueType> tmpCell( whichRow, 0 );

  typename std::vector<Cell<ValueType> >::const_iterator result;
  for( result = cells.begin(); result != cells.end(); ++result )
    if( result->getRow() == whichRow ) break;

  if( result == cells.end() )
    return false;
  else
    semVal = result->getValue( idStat );

  return true;
}


template <typename ValueType>
inline bool Column<ValueType>::getCellValue( std::vector<ValueType>& semVal,
    int whichRow ) const
{
  Cell<ValueType> tmpCell( whichRow, 0 );

  typename std::vector<Cell<ValueType> >::iterator result = find( cells.begin(), cells.end(), tmpCell );
  if( result == cells.end() )
    return false;
  else
    semVal = result->getValue();

  return true;
}


template <typename ValueType>
inline void Column<ValueType>::print() const
{
  for ( unsigned int ii = 0; ii < n_cells; ii++ )
  {
    cells[ ii ].print();
    std::cout << std::endl;
  }
}
