#include <string>
#include <iostream>

using namespace std;

#include "matrix.h"


template <typename TipusBasic>
Matrix2D<TipusBasic>::Matrix2D( int numCols, short numStats )
{
  Column<TipusBasic> *tmp_col;
  
  finished = false;
  
  for( int ii = 0; ii < numCols; ii++ )
  {
    tmp_col = new Column<TipusBasic>( numStats, &finished );
    tmp_col->Init();
    cols.push_back( tmp_col );
  }
}


template <typename TipusBasic>
Matrix2D<TipusBasic>::Matrix2D( int currentRow, int numCols, short numStats )
{
  Column<TipusBasic> *tmp_col;
  
  finished = false;
  
  for( int ii = 0; ii < numCols; ii++ )
  {
    tmp_col = new Column<TipusBasic>( currentRow, numStats, &finished );
    cols.push_back( tmp_col );
  }
}


template <typename TipusBasic>
Matrix2D<TipusBasic>::~Matrix2D()
{
  for( unsigned int ii = 0; ii < cols.size(); ii++ )
  {
    delete cols[ ii ];
  }
  cols.clear();
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::Init( short idStat )
{
  for( unsigned int ii = 0; ii < cols.size(); ii++ )
  {
    cols[ ii ]->Init( idStat );
  }
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::Init( )
{
  for( unsigned int ii = 0; ii < cols.size(); ii++ )
  {
    cols[ ii ]->Init( );
  }
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::SetValue( int col, short idStat, TipusBasic semVal )
{
  cols[ col ]->SetValue( idStat, semVal );
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::SetValue( int col, TipusBasic semVal )
{
  cols[ col ]->SetValue( semVal );
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::AddValue( int col, short idStat, TipusBasic semVal )
{
  cols[ col ]->AddValue( idStat, semVal );
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::AddValue( int col, TipusBasic semVal )
{
  cols[ col ]->AddValue( semVal );
}


template <typename TipusBasic>
TipusBasic Matrix2D<TipusBasic>::GetCurrentValue( int col, short idStat ) const
{
  return cols[ col ]->GetCurrentValue( idStat );
}


template <typename TipusBasic>
int Matrix2D<TipusBasic>::GetCurrentRow( int col ) const
{
  return cols[ col ]->GetCurrentRow();
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::NewRow( )
{
  for( unsigned int ii = 0; ii < cols.size(); ii++ )
  {
    cols[ ii ]->NewRow();
  }
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::NewRow( int col, int row )
{
  cols[ col ]->NewRow( row );
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::Finish( )
{
  finished = true;
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::SetNextCell( int col )
{
  cols[ col ]->SetNextCell();
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::SetFirstCell( int col )
{
  cols[ col ]->SetFirstCell();
}


template <typename TipusBasic>
bool Matrix2D<TipusBasic>::EndCell( int col )
{
  return cols[ col ]->EndCell();
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::EraseColumns( int ini_col, int fin_col )
{
  if( fin_col < ini_col )
    return;
  
  if( ini_col < 0 )
    return;
  
  if( fin_col >= (int) cols.size() )
    return;
  
  typename vector<Column<TipusBasic> *>::iterator it_ini, it_fin;
  int i;
  
  it_ini = cols.begin();
  for( i = 0; i < ini_col; i++, it_ini++ );
    
  for( it_fin = it_ini; i < fin_col; i++, it_fin++ )
    delete cols[ i ];
  
  cols.erase( it_ini, it_fin );
}


template <typename TipusBasic>
void Matrix2D<TipusBasic>::Print() const
{
  for( unsigned int ii = 0; ii < cols.size(); ii++ )
  {
    cout << "----------Column " << ii << "----------" << endl;
    cols[ ii ]->Print();
  }
}
