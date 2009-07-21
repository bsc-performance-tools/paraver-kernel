#include <string>
#include <iostream>
#include <string.h>

#include "paraverkerneltypes.h"

using namespace std;

template <typename ValueType>
Cell<ValueType>::Cell( TObjectOrder idRow, UINT16 numStats ):
    row( idRow ), nStats( numStats )
{
  values.insert( values.begin(), nStats, (ValueType) 0 );
}

template <typename ValueType>
Cell<ValueType>::Cell( Cell< ValueType >& source ):
    row( source.row ), nStats( source.nStats )
{
  values = source.values;
}

template <typename ValueType>
Cell<ValueType>::~Cell()
{}


template <typename ValueType>
inline void Cell<ValueType>::init( UINT16 idStat )
{
  if ( nStats > idStat )
    values[ idStat ] = (ValueType) 0;
}


template <typename ValueType>
inline void Cell<ValueType>::init( )
{
  for ( UINT16 ii = 0; ii < nStats; ++ii )
    values[ ii ] = (ValueType) 0;
}


template <typename ValueType>
inline void Cell<ValueType>::setValue( UINT16 idStat, ValueType semVal )
{
  if ( nStats > idStat )
    values[ idStat ] = semVal;
}


template <typename ValueType>
inline void Cell<ValueType>::setValue( ValueType semVal )
{
  for ( UINT16 ii = 0; ii < nStats; ++ii )
    values[ ii ] = semVal;
}


template <typename ValueType>
inline void Cell<ValueType>::setValue( const vector<ValueType>& semVal )
{
  values = semVal;
}


template <typename ValueType>
inline void Cell<ValueType>::addValue( UINT16 idStat, ValueType semVal )
{
  if ( nStats > idStat )
    values[ idStat ] += semVal;
}


template <typename ValueType>
inline void Cell<ValueType>::addValue( ValueType semVal )
{
  for ( UINT16 ii = 0; ii < nStats; ++ii )
    values[ ii ] += semVal;
}


template <typename ValueType>
inline void Cell<ValueType>::addValue( const vector<ValueType>& semVal )
{
  for ( UINT16 ii = 0; ii < nStats; ++ii )
    values[ ii ] += semVal[ ii ];
}


template <typename ValueType>
inline ValueType Cell<ValueType>::getValue( UINT16 idStat ) const
{
  if ( nStats > idStat )
    return values[ idStat ];

  ValueType tmp;

  memset( ( void * ) &tmp, 0, sizeof( ValueType ) );
  return tmp;
}


template <typename ValueType>
inline vector<ValueType> Cell<ValueType>::getValue() const
{
  return values;
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
  for ( UINT16 ii = 0; ii < nStats; ++ii )
  {
    cout << values[ ii ];
    cout << " ";
  }

  cout << endl;
}
