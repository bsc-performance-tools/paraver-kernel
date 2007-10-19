#include <string>
#include <iostream>

using namespace std;

//#include "column.h"

template <typename ValueType>
Column<ValueType>::Column( short numStats, bool *mat_finished )
{
  nstat = numStats;
  current_cell = new Cell<ValueType>( 0, nstat );
  current_cell->Init();
  n_cells = 0;
  modified = false;
  finished = mat_finished;
}


template <typename ValueType>
Column<ValueType>::Column( int currentRow, short numStats, bool *mat_finished )
{
  nstat = numStats;
  current_cell = new Cell<ValueType>( currentRow, nstat );
  current_cell->Init();
  n_cells = 0;
  modified = false;
  finished = mat_finished;
}


template <typename ValueType>
Column<ValueType>::~Column()
{
  for( unsigned int ii = 0; ii < n_cells; ii++ )
    delete cells[ ii ];
  cells.clear();

  if( !modified )
    delete current_cell;
}


template <typename ValueType>
void Column<ValueType>::Init( short idStat )
{
  current_cell->Init( idStat );
}


template <typename ValueType>
void Column<ValueType>::Init( )
{
  current_cell->Init( );
}


template <typename ValueType>
void Column<ValueType>::SetValue( short idStat, ValueType semVal )
{
  if( *finished )
  {
    (*it_cell)->SetValue( idStat, semVal );
  }
  else
  {
    current_cell->SetValue( idStat, semVal );
    if( modified == false )
    {
      cells.push_back( current_cell );
      n_cells++;
      modified = true;
      it_cell = cells.begin();
    }
  }
}


template <typename ValueType>
void Column<ValueType>::SetValue( ValueType semVal )
{
  if( *finished )
  {
    (*it_cell)->SetValue( semVal );
  }
  else
  {
    current_cell->SetValue( semVal );
    if( modified == false )
    {
      cells.push_back( current_cell );
      n_cells++;
      modified = true;
      it_cell = cells.begin();
    }
  }
}


template <typename ValueType>
void Column<ValueType>::AddValue( short idStat, ValueType semVal )
{
  current_cell->AddValue( idStat, semVal );
  if( modified == false )
  {
    cells.push_back( current_cell );
    n_cells++;
    modified = true;
    it_cell = cells.begin();
  }
}


template <typename ValueType>
void Column<ValueType>::AddValue( ValueType semVal )
{
  current_cell->AddValue( semVal );
  if( modified == false )
  {
    cells.push_back( current_cell );
    n_cells++;
    modified = true;
    it_cell = cells.begin();
  }
}


template <typename ValueType>
ValueType Column<ValueType>::GetCurrentValue( short idStat ) const
{
  if( *finished )
  {
    return (*it_cell)->GetValue( idStat );
  }

  return current_cell->GetValue( idStat );
}


template <typename ValueType>
int Column<ValueType>::GetCurrentRow( ) const
{
  if( *finished )
  {
    if( n_cells == 0 )
      return -1;
    else
      return (*it_cell)->GetRow();
  }

  return current_cell->GetRow();
}


template <typename ValueType>
void Column<ValueType>::NewRow( )
{
  int tmp_row = GetCurrentRow();

  if( modified )
  {
    current_cell = new Cell<ValueType>( tmp_row + 1, nstat );
    current_cell->Init();
    modified = false;
  }
  else
  {
    current_cell->SetRow( tmp_row + 1 );
  }
}


template <typename ValueType>
void Column<ValueType>::NewRow( int row )
{
  if( modified )
  {
    current_cell = new Cell<ValueType>( row, nstat );
    current_cell->Init();
    modified = false;
  }
  else
  {
    current_cell->SetRow( row );
  }
}


template <typename ValueType>
void Column<ValueType>::SetNextCell( )
{
  it_cell++;
}


template <typename ValueType>
void Column<ValueType>::SetFirstCell( )
{
  it_cell = cells.begin();
}


template <typename ValueType>
bool Column<ValueType>::EndCell( )
{
  if( n_cells == 0 )
    return true;

  return ( it_cell == cells.end() );
//    return true;
//  return false;
}


template <typename ValueType>
void Column<ValueType>::Print() const
{
  for( unsigned int ii = 0; ii < n_cells; ii++ )
  {
    cells[ ii ]->Print();
    cout << endl;
  }
}
