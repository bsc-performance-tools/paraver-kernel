#include <string>
#include <iostream>

#include "paraverkerneltypes.h"

using namespace std;

template <typename ValueType>
Cell<ValueType>::Cell( TObjectOrder idRow, UINT16 numStats )
{
  Statistic_2D<ValueType> *tmp_stat;

  row = idRow;
  for( UINT16 ii=0; ii< numStats; ii++ )
  {
    tmp_stat = new Statistic_2D<ValueType>;
    values.push_back(tmp_stat);
  }
}


template <typename ValueType>
Cell<ValueType>::~Cell()
{
  for( UINT16 ii = 0; ii < (UINT16)values.size(); ii++ )
    delete values[ ii ];
  values.clear();
}


template <typename ValueType>
void Cell<ValueType>::Init( UINT16 idStat )
{
  if( (UINT16)values.size() > idStat )
    values[ idStat ]->Init();
}


template <typename ValueType>
void Cell<ValueType>::Init( )
{
  for( UINT16 ii=0; ii< (UINT16)values.size(); ii++ )
    values[ ii ]->Init();
}


template <typename ValueType>
void Cell<ValueType>::SetValue( UINT16 idStat, ValueType semVal )
{
  if( (UINT16)values.size() > idStat )
    values[ idStat ]->SetValue( semVal );
}


template <typename ValueType>
void Cell<ValueType>::SetValue( ValueType semVal )
{
  for( UINT16 ii=0; ii< (UINT16)values.size(); ii++ )
    values[ ii ]->SetValue( semVal );
}


template <typename ValueType>
void Cell<ValueType>::AddValue( UINT16 idStat, ValueType semVal )
{
  if( (UINT16)values.size() > idStat )
    values[ idStat ]->AddValue( semVal );
}


template <typename ValueType>
void Cell<ValueType>::AddValue( ValueType semVal )
{
  for( UINT16 ii=0; ii< (UINT16)values.size(); ii++ )
    values[ ii ]->AddValue( semVal );
}


template <typename ValueType>
ValueType Cell<ValueType>::GetValue( UINT16 idStat ) const
{
  if ( (UINT16)values.size() > idStat )
    return values[idStat]->GetValue();

  ValueType tmp;

  bzero( (void *) &tmp, sizeof( ValueType ) );
  return tmp;
}


template <typename ValueType>
TObjectOrder Cell<ValueType>::GetRow( ) const
{
  return row;
}


template <typename ValueType>
void Cell<ValueType>::SetRow( TObjectOrder row )
{
  this->row = row;
}


template <typename ValueType>
void Cell<ValueType>::Print( ) const
{
  cout << "[" << row << "] " ;
  for( UINT16 ii = 0; ii< (UINT16)values.size(); ii++ )
  {
    values[ii]->Print();
    cout << " ";
  }

  cout << endl;
}
