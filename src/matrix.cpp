#include <string>
#include <iostream>

using namespace std;


template <typename ValueType>
Matrix<ValueType>::Matrix( UINT32 numCols, UINT16 numStats )
{
  Column<ValueType> *tmp_col;

  finished = false;

  for( UINT32 ii = 0; ii < numCols; ii++ )
  {
    tmp_col = new Column<ValueType>( numStats, &finished );
    tmp_col->Init();
    cols.push_back( tmp_col );
  }
}


template <typename ValueType>
Matrix<ValueType>::Matrix( TObjectOrder currentRow, UINT32 numCols, UINT16 numStats )
{
  Column<ValueType> *tmp_col;

  finished = false;

  for( UINT32 ii = 0; ii < numCols; ii++ )
  {
    tmp_col = new Column<ValueType>( currentRow, numStats, &finished );
    cols.push_back( tmp_col );
  }
}


template <typename ValueType>
Matrix<ValueType>::~Matrix()
{
  for( UINT32 ii = 0; ii < (UINT32)cols.size(); ii++ )
  {
    delete cols[ ii ];
  }
  cols.clear();
}


template <typename ValueType>
void Matrix<ValueType>::Init( UINT16 idStat )
{
  for( UINT32 ii = 0; ii < (UINT32)cols.size(); ii++ )
  {
    cols[ ii ]->Init( idStat );
  }
}


template <typename ValueType>
void Matrix<ValueType>::Init( )
{
  for( UINT32 ii = 0; ii < (UINT32)cols.size(); ii++ )
  {
    cols[ ii ]->Init( );
  }
}


template <typename ValueType>
void Matrix<ValueType>::SetValue( UINT32 col, UINT16 idStat, ValueType semVal )
{
  cols[ col ]->SetValue( idStat, semVal );
}


template <typename ValueType>
void Matrix<ValueType>::SetValue( UINT32 col, ValueType semVal )
{
  cols[ col ]->SetValue( semVal );
}


template <typename ValueType>
void Matrix<ValueType>::AddValue( UINT32 col, UINT16 idStat, ValueType semVal )
{
  cols[ col ]->AddValue( idStat, semVal );
}


template <typename ValueType>
void Matrix<ValueType>::AddValue( UINT32 col, ValueType semVal )
{
  cols[ col ]->AddValue( semVal );
}


template <typename ValueType>
ValueType Matrix<ValueType>::GetCurrentValue( UINT32 col, UINT16 idStat ) const
{
  return cols[ col ]->GetCurrentValue( idStat );
}


template <typename ValueType>
TObjectOrder Matrix<ValueType>::GetCurrentRow( UINT32 col ) const
{
  return cols[ col ]->GetCurrentRow();
}


template <typename ValueType>
void Matrix<ValueType>::NewRow( )
{
  for( UINT32 ii = 0; ii < (UINT32)cols.size(); ii++ )
  {
    cols[ ii ]->NewRow();
  }
}


template <typename ValueType>
void Matrix<ValueType>::NewRow( UINT32 col, TObjectOrder row )
{
  cols[ col ]->NewRow( row );
}


template <typename ValueType>
void Matrix<ValueType>::Finish( )
{
  finished = true;
}


template <typename ValueType>
void Matrix<ValueType>::SetNextCell( UINT32 col )
{
  cols[ col ]->SetNextCell();
}


template <typename ValueType>
void Matrix<ValueType>::SetFirstCell( UINT32 col )
{
  cols[ col ]->SetFirstCell();
}


template <typename ValueType>
bool Matrix<ValueType>::EndCell( UINT32 col )
{
  return cols[ col ]->EndCell();
}


template <typename ValueType>
void Matrix<ValueType>::EraseColumns( UINT32 ini_col, UINT32 fin_col )
{
  if( fin_col < ini_col )
    return;

  if( ini_col < 0 )
    return;

  if( fin_col >= (int) cols.size() )
    return;

  typename vector<Column<ValueType> *>::iterator it_ini, it_fin;
  UINT32 i;

  it_ini = cols.begin();
  for( i = 0; i < ini_col; i++, it_ini++ );

  for( it_fin = it_ini; i < fin_col; i++, it_fin++ )
    delete cols[ i ];

  cols.erase( it_ini, it_fin );
}


template <typename ValueType>
void Matrix<ValueType>::Print() const
{
  for( UINT32 ii = 0; ii < (UINT32)cols.size(); ii++ )
  {
    cout << "----------Column " << ii << "----------" << endl;
    cols[ ii ]->Print();
  }
}
