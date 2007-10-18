#include <string>
#include <iostream>

using namespace std;

#include "matrix.h"


template <typename ValueType>
Matrix2D<ValueType>::Matrix2D( int numCols, short numStats )
{
  Column<ValueType> *tmp_col;

  finished = false;

  for( int ii = 0; ii < numCols; ii++ )
  {
    tmp_col = new Column<ValueType>( numStats, &finished );
    tmp_col->Init();
    cols.push_back( tmp_col );
  }
}


template <typename ValueType>
Matrix2D<ValueType>::Matrix2D( int currentRow, int numCols, short numStats )
{
  Column<ValueType> *tmp_col;

  finished = false;

  for( int ii = 0; ii < numCols; ii++ )
  {
    tmp_col = new Column<ValueType>( currentRow, numStats, &finished );
    cols.push_back( tmp_col );
  }
}


template <typename ValueType>
Matrix2D<ValueType>::~Matrix2D()
{
  for( unsigned int ii = 0; ii < cols.size(); ii++ )
  {
    delete cols[ ii ];
  }
  cols.clear();
}


template <typename ValueType>
void Matrix2D<ValueType>::Init( short idStat )
{
  for( unsigned int ii = 0; ii < cols.size(); ii++ )
  {
    cols[ ii ]->Init( idStat );
  }
}


template <typename ValueType>
void Matrix2D<ValueType>::Init( )
{
  for( unsigned int ii = 0; ii < cols.size(); ii++ )
  {
    cols[ ii ]->Init( );
  }
}


template <typename ValueType>
void Matrix2D<ValueType>::SetValue( int col, short idStat, ValueType semVal )
{
  cols[ col ]->SetValue( idStat, semVal );
}


template <typename ValueType>
void Matrix2D<ValueType>::SetValue( int col, ValueType semVal )
{
  cols[ col ]->SetValue( semVal );
}


template <typename ValueType>
void Matrix2D<ValueType>::AddValue( int col, short idStat, ValueType semVal )
{
  cols[ col ]->AddValue( idStat, semVal );
}


template <typename ValueType>
void Matrix2D<ValueType>::AddValue( int col, ValueType semVal )
{
  cols[ col ]->AddValue( semVal );
}


template <typename ValueType>
ValueType Matrix2D<ValueType>::GetCurrentValue( int col, short idStat ) const
{
  return cols[ col ]->GetCurrentValue( idStat );
}


template <typename ValueType>
int Matrix2D<ValueType>::GetCurrentRow( int col ) const
{
  return cols[ col ]->GetCurrentRow();
}


template <typename ValueType>
void Matrix2D<ValueType>::NewRow( )
{
  for( unsigned int ii = 0; ii < cols.size(); ii++ )
  {
    cols[ ii ]->NewRow();
  }
}


template <typename ValueType>
void Matrix2D<ValueType>::NewRow( int col, int row )
{
  cols[ col ]->NewRow( row );
}


template <typename ValueType>
void Matrix2D<ValueType>::Finish( )
{
  finished = true;
}


template <typename ValueType>
void Matrix2D<ValueType>::SetNextCell( int col )
{
  cols[ col ]->SetNextCell();
}


template <typename ValueType>
void Matrix2D<ValueType>::SetFirstCell( int col )
{
  cols[ col ]->SetFirstCell();
}


template <typename ValueType>
bool Matrix2D<ValueType>::EndCell( int col )
{
  return cols[ col ]->EndCell();
}


template <typename ValueType>
void Matrix2D<ValueType>::EraseColumns( int ini_col, int fin_col )
{
  if( fin_col < ini_col )
    return;

  if( ini_col < 0 )
    return;

  if( fin_col >= (int) cols.size() )
    return;

  typename vector<Column<ValueType> *>::iterator it_ini, it_fin;
  int i;

  it_ini = cols.begin();
  for( i = 0; i < ini_col; i++, it_ini++ );

  for( it_fin = it_ini; i < fin_col; i++, it_fin++ )
    delete cols[ i ];

  cols.erase( it_ini, it_fin );
}


template <typename ValueType>
void Matrix2D<ValueType>::Print() const
{
  for( unsigned int ii = 0; ii < cols.size(); ii++ )
  {
    cout << "----------Column " << ii << "----------" << endl;
    cols[ ii ]->Print();
  }
}
