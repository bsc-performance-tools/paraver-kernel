#include <string>
#include <iostream>

#include "paraverkerneltypes.h"

using namespace std;

template <typename ValueType>
Cell<ValueType>::Cell( TObjectOrder idRow, UINT16 numStats ):
    row( idRow ), nStats( numStats )
{
/*  Statistic_2D<ValueType> *tmp_stat;

  for ( UINT16 ii = 0; ii < numStats; ii++ )
  {
    tmp_stat = new Statistic_2D<ValueType>;
    values.push_back( tmp_stat );
  }*/
  values = new Statistic_2D<ValueType>[ nStats ];
}

template <typename ValueType>
Cell<ValueType>::Cell( Cell< ValueType >& source ):
    row( source.row ), nStats( source.nStats )
{
  values = new Statistic_2D<ValueType>[ nStats ];
  for ( UINT16 i = 0; i < nStats; i++ )
    values[ i ] = source.values[ i ];
}

template <typename ValueType>
Cell<ValueType>::~Cell()
{
  delete[] values;
}


template <typename ValueType>
inline void Cell<ValueType>::init( UINT16 idStat )
{
  if ( nStats > idStat )
    values[ idStat ].init();
}


template <typename ValueType>
inline void Cell<ValueType>::init( )
{
  for ( UINT16 ii = 0; ii < nStats; ii++ )
    values[ ii ].init();
}


template <typename ValueType>
inline void Cell<ValueType>::setValue( UINT16 idStat, ValueType semVal )
{
  if ( nStats > idStat )
    values[ idStat ].setValue( semVal );
}


template <typename ValueType>
inline void Cell<ValueType>::setValue( ValueType semVal )
{
  for ( UINT16 ii = 0; ii < nStats; ii++ )
    values[ ii ].setValue( semVal );
}


template <typename ValueType>
inline void Cell<ValueType>::addValue( UINT16 idStat, ValueType semVal )
{
  if ( nStats > idStat )
    values[ idStat ].addValue( semVal );
}


template <typename ValueType>
inline void Cell<ValueType>::addValue( ValueType semVal )
{
  for ( UINT16 ii = 0; ii < nStats; ii++ )
    values[ ii ].addValue( semVal );
}


template <typename ValueType>
inline ValueType Cell<ValueType>::getValue( UINT16 idStat ) const
{
  if ( nStats > idStat )
    return values[idStat].getValue();

  ValueType tmp;

  memset( ( void * ) &tmp, 0, sizeof( ValueType ) );
  return tmp;
}


template <typename ValueType>
inline TObjectOrder Cell<ValueType>::getRow( ) const
{
  return row;
}


template <typename ValueType>
inline void Cell<ValueType>::setRow( TObjectOrder row )
{
  this->row = row;
}


template <typename ValueType>
inline void Cell<ValueType>::print( ) const
{
  cout << "[" << row << "] " ;
  for ( UINT16 ii = 0; ii < nStats; ii++ )
  {
    values[ii].print();
    cout << " ";
  }

  cout << endl;
}
