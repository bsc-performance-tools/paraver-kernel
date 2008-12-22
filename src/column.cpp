#include <string>
#include <iostream>

using namespace std;

//#include "column.h"

template <typename ValueType>
Column<ValueType>::Column( short numStats, bool *mat_finished ):
    nstat( numStats ), modified( false ), n_cells( 0 ), finished( mat_finished )
{
  current_cell = new Cell<ValueType>( 0, nstat );
  current_cell->init();
}


template <typename ValueType>
Column<ValueType>::Column( int currentRow, short numStats, bool *mat_finished ):
    nstat( numStats ), modified( false ), n_cells( 0 ), finished( mat_finished )
{
  current_cell = new Cell<ValueType>( currentRow, nstat );
  current_cell->init();
}


template <typename ValueType>
Column<ValueType>::~Column()
{
  for ( unsigned int ii = 0; ii < n_cells; ii++ )
    delete cells[ ii ];
  cells.clear();

  if ( !modified )
    delete current_cell;
}


template <typename ValueType>
inline void Column<ValueType>::init( short idStat )
{
  current_cell->init( idStat );
}


template <typename ValueType>
inline void Column<ValueType>::init( )
{
  current_cell->init( );
}


template <typename ValueType>
inline void Column<ValueType>::setValue( short idStat, ValueType semVal )
{
  if ( *finished )
  {
    ( *it_cell )->setValue( idStat, semVal );
  }
  else
  {
    current_cell->setValue( idStat, semVal );
    if ( modified == false )
    {
      cells.push_back( current_cell );
      n_cells++;
      modified = true;
      it_cell = cells.begin();
    }
  }
}


template <typename ValueType>
inline void Column<ValueType>::setValue( ValueType semVal )
{
  if ( *finished )
  {
    ( *it_cell )->setValue( semVal );
  }
  else
  {
    current_cell->setValue( semVal );
    if ( modified == false )
    {
      cells.push_back( current_cell );
      n_cells++;
      modified = true;
      it_cell = cells.begin();
    }
  }
}


template <typename ValueType>
inline void Column<ValueType>::addValue( short idStat, ValueType semVal )
{
  current_cell->addValue( idStat, semVal );
  if ( modified == false )
  {
    cells.push_back( current_cell );
    n_cells++;
    modified = true;
    it_cell = cells.begin();
  }
}


template <typename ValueType>
inline void Column<ValueType>::addValue( ValueType semVal )
{
  current_cell->addValue( semVal );
  if ( modified == false )
  {
    cells.push_back( current_cell );
    n_cells++;
    modified = true;
    it_cell = cells.begin();
  }
}


template <typename ValueType>
inline ValueType Column<ValueType>::getCurrentValue( short idStat ) const
{
  if ( *finished )
  {
    return ( *it_cell )->getValue( idStat );
  }

  return current_cell->getValue( idStat );
}


template <typename ValueType>
inline int Column<ValueType>::getCurrentRow( ) const
{
  if ( *finished )
  {
    if ( n_cells == 0 )
      return -1;
    else
      return ( *it_cell )->getRow();
  }

  return current_cell->getRow();
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
    current_cell = new Cell<ValueType>( tmp_row + 1, nstat );
    current_cell->init();
    modified = false;
  }
  else
  {
    current_cell->setRow( tmp_row + 1 );
  }
}


template <typename ValueType>
inline void Column<ValueType>::newRow( int row )
{
  if ( modified )
  {
    current_cell = new Cell<ValueType>( row, nstat );
    current_cell->init();
    modified = false;
  }
  else
  {
    current_cell->setRow( row );
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
  if ( n_cells == 0 )
    return true;

  return ( it_cell == cells.end() );
//    return true;
//  return false;
}


template <typename ValueType>
inline void Column<ValueType>::print() const
{
  for ( unsigned int ii = 0; ii < n_cells; ii++ )
  {
    cells[ ii ]->print();
    cout << endl;
  }
}
