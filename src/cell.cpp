#include <string>
#include <iostream>

#include "paraverkerneltypes.h"

using namespace std;

template <typename ValueType>
Cell<ValueType>::Cell( TObjectOrder idRow, UINT16 numStats )
{
  Statistic_2D<ValueType> *tmp_stat;

  row = idRow;
  for ( UINT16 ii = 0; ii < numStats; ii++ )
  {
    tmp_stat = new Statistic_2D<ValueType>;
    values.push_back( tmp_stat );
  }
}


template <typename ValueType>
Cell<ValueType>::~Cell()
{
  for ( UINT16 ii = 0; ii < ( UINT16 )values.size(); ii++ )
    delete values[ ii ];
  values.clear();
}


template <typename ValueType>
void Cell<ValueType>::init( UINT16 idStat )
{
  if ( ( UINT16 )values.size() > idStat )
    values[ idStat ]->init();
}


template <typename ValueType>
void Cell<ValueType>::init( )
{
  for ( UINT16 ii = 0; ii < ( UINT16 )values.size(); ii++ )
    values[ ii ]->init();
}


template <typename ValueType>
void Cell<ValueType>::setValue( UINT16 idStat, ValueType semVal )
{
  if ( ( UINT16 )values.size() > idStat )
    values[ idStat ]->setValue( semVal );
}


template <typename ValueType>
void Cell<ValueType>::setValue( ValueType semVal )
{
  for ( UINT16 ii = 0; ii < ( UINT16 )values.size(); ii++ )
    values[ ii ]->setValue( semVal );
}


template <typename ValueType>
void Cell<ValueType>::addValue( UINT16 idStat, ValueType semVal )
{
  if ( ( UINT16 )values.size() > idStat )
    values[ idStat ]->addValue( semVal );
}


template <typename ValueType>
void Cell<ValueType>::addValue( ValueType semVal )
{
  for ( UINT16 ii = 0; ii < ( UINT16 )values.size(); ii++ )
    values[ ii ]->AddValue( semVal );
}


template <typename ValueType>
ValueType Cell<ValueType>::getValue( UINT16 idStat ) const
{
  if ( ( UINT16 )values.size() > idStat )
    return values[idStat]->getValue();

  ValueType tmp;

  bzero( ( void * ) &tmp, sizeof( ValueType ) );
  return tmp;
}


template <typename ValueType>
TObjectOrder Cell<ValueType>::getRow( ) const
{
  return row;
}


template <typename ValueType>
void Cell<ValueType>::setRow( TObjectOrder row )
{
  this->row = row;
}


template <typename ValueType>
void Cell<ValueType>::print( ) const
{
  cout << "[" << row << "] " ;
  for ( UINT16 ii = 0; ii < ( UINT16 )values.size(); ii++ )
  {
    values[ii]->print();
    cout << " ";
  }

  cout << endl;
}
