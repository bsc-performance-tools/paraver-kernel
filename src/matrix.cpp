#include <string>
#include <iostream>

using namespace std;


template <typename ValueType>
Matrix<ValueType>::Matrix( UINT32 numCols, UINT16 numStats ):
  finished( false )
{
  cols.insert( cols.begin(), numCols, Column<ValueType>( numStats, &finished ) );
}


template <typename ValueType>
Matrix<ValueType>::Matrix( TObjectOrder currentRow, UINT32 numCols, UINT16 numStats ):
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
inline void Matrix<ValueType>::init( UINT16 idStat )
{
  for ( UINT32 ii = 0; ii < ( UINT32 )cols.size(); ii++ )
  {
    cols[ ii ].init( idStat );
  }
}


template <typename ValueType>
inline void Matrix<ValueType>::init( )
{
  for ( UINT32 ii = 0; ii < ( UINT32 )cols.size(); ii++ )
  {
    cols[ ii ].init( );
  }
}


template <typename ValueType>
inline void Matrix<ValueType>::setValue( UINT32 col, UINT16 idStat, ValueType semVal )
{
  cols[ col ].setValue( idStat, semVal );
}


template <typename ValueType>
inline void Matrix<ValueType>::setValue( UINT32 col, ValueType semVal )
{
  cols[ col ].setValue( semVal );
}


template <typename ValueType>
inline void Matrix<ValueType>::setValue( UINT32 col, const vector<ValueType>& semVal )
{
  cols[ col ].setValue( semVal );
}


template <typename ValueType>
inline void Matrix<ValueType>::addValue( UINT32 col, UINT16 idStat, ValueType semVal )
{
  cols[ col ].addValue( idStat, semVal );
}


template <typename ValueType>
inline void Matrix<ValueType>::addValue( UINT32 col, ValueType semVal )
{
  cols[ col ].addValue( semVal );
}


template <typename ValueType>
inline void Matrix<ValueType>::addValue( UINT32 col, const vector<ValueType>& semVal )
{
  cols[ col ].addValue( semVal );
}


template <typename ValueType>
inline ValueType Matrix<ValueType>::getCurrentValue( UINT32 col, UINT16 idStat ) const
{
  return cols[ col ].getCurrentValue( idStat );
}


template <typename ValueType>
inline vector<ValueType> Matrix<ValueType>::getCurrentValue( UINT32 col ) const
{
  return cols[ col ].getCurrentValue();
}


template <typename ValueType>
inline TObjectOrder Matrix<ValueType>::getCurrentRow( UINT32 col ) const
{
  return cols[ col ].getCurrentRow();
}


template <typename ValueType>
inline bool Matrix<ValueType>::currentCellModified( UINT32 col ) const
{
  return cols[ col ].currentCellModified();
}


template <typename ValueType>
inline void Matrix<ValueType>::newRow( )
{
  for ( UINT32 ii = 0; ii < ( UINT32 )cols.size(); ii++ )
  {
    cols[ ii ].newRow();
  }
}


template <typename ValueType>
inline void Matrix<ValueType>::newRow( UINT32 col, TObjectOrder row )
{
  cols[ col ].newRow( row );
}


template <typename ValueType>
inline void Matrix<ValueType>::finish( )
{
  finished = true;
}


template <typename ValueType>
inline void Matrix<ValueType>::setNextCell( UINT32 col )
{
  cols[ col ].setNextCell();
}


template <typename ValueType>
inline void Matrix<ValueType>::setFirstCell( UINT32 col )
{
  cols[ col ].setFirstCell();
}


template <typename ValueType>
inline bool Matrix<ValueType>::endCell( UINT32 col )
{
  return cols[ col ].endCell();
}


template <typename ValueType>
inline void Matrix<ValueType>::eraseColumns( UINT32 ini_col, UINT32 fin_col )
{
  if ( fin_col < ini_col )
    return;

  if ( ini_col < 0 )
    return;

  if ( fin_col >= ( int ) cols.size() )
    return;

  typename vector<Column<ValueType> >::iterator it_ini, it_fin;
  UINT32 i;

  it_ini = cols.begin();
  for ( i = 0; i < ini_col; i++, ++it_ini );

  for ( it_fin = it_ini; i < fin_col; i++, ++it_fin );

  cols.erase( it_ini, it_fin );
}


template <typename ValueType>
inline void Matrix<ValueType>::print() const
{
  for ( UINT32 ii = 0; ii < ( UINT32 )cols.size(); ii++ )
  {
    cout << "----------Column " << ii << "----------" << endl;
    cols[ ii ].print();
  }
}
