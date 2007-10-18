#include <string>
#include <iostream>

using namespace std;

#include "column.h"


template <typename TipusBasic>
Column<TipusBasic>::Column( short numStats, bool *mat_finished )
{
  nstat = numStats;
  current_cell = new Cell<TipusBasic>( 0, nstat );
  current_cell->Init();
  n_cells = 0;
  modified = false;
  finished = mat_finished;
}


template <typename TipusBasic>
Column<TipusBasic>::Column( int currentRow, short numStats, bool *mat_finished )
{
  nstat = numStats;
  current_cell = new Cell<TipusBasic>( currentRow, nstat );
  current_cell->Init();
  n_cells = 0;
  modified = false;
  finished = mat_finished;
}


template <typename TipusBasic>
Column<TipusBasic>::~Column()
{
  for( unsigned int ii = 0; ii < n_cells; ii++ )
    delete cells[ ii ];
  cells.clear();
  
  if( !modified )
    delete current_cell;
}


template <typename TipusBasic>
void Column<TipusBasic>::Init( short idStat )
{
  current_cell->Init( idStat );
}


template <typename TipusBasic>
void Column<TipusBasic>::Init( )
{
  current_cell->Init( );
}


template <typename TipusBasic>
void Column<TipusBasic>::SetValue( short idStat, TipusBasic semVal )
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


template <typename TipusBasic>
void Column<TipusBasic>::SetValue( TipusBasic semVal )
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


template <typename TipusBasic>
void Column<TipusBasic>::AddValue( short idStat, TipusBasic semVal )
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


template <typename TipusBasic>
void Column<TipusBasic>::AddValue( TipusBasic semVal )
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


template <typename TipusBasic>
TipusBasic Column<TipusBasic>::GetCurrentValue( short idStat ) const
{
  if( *finished )
  {
    return (*it_cell)->GetValue( idStat );
  }
  
  return current_cell->GetValue( idStat );
}


template <typename TipusBasic>
int Column<TipusBasic>::GetCurrentRow( ) const
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


template <typename TipusBasic>
void Column<TipusBasic>::NewRow( )
{
  int tmp_row = GetCurrentRow();
  
  if( modified )
  {
    current_cell = new Cell<TipusBasic>( tmp_row + 1, nstat );
    current_cell->Init();
    modified = false;
  }
  else
  {
    current_cell->SetRow( tmp_row + 1 );
  }
}
  

template <typename TipusBasic>
void Column<TipusBasic>::NewRow( int row )
{
  if( modified )
  {
    current_cell = new Cell<TipusBasic>( row, nstat );
    current_cell->Init();
    modified = false;
  }
  else
  {
    current_cell->SetRow( row );
  }
}


template <typename TipusBasic>
void Column<TipusBasic>::SetNextCell( )
{
  it_cell++;
}


template <typename TipusBasic>
void Column<TipusBasic>::SetFirstCell( )
{
  it_cell = cells.begin();
}


template <typename TipusBasic>
bool Column<TipusBasic>::EndCell( )
{
  if( n_cells == 0 )
    return true;
  
  return ( it_cell == cells.end() );
//    return true;
//  return false;
}


template <typename TipusBasic>
void Column<TipusBasic>::Print() const
{
  for( unsigned int ii = 0; ii < n_cells; ii++ )
  {
    cells[ ii ]->Print();
    cout << endl;
  }
}
