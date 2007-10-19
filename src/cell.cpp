#include <string>
#include <iostream>

using namespace std;

//#include "cell.h"

template <typename ValueType>
Cell<ValueType>::Cell( int idRow, short numStats )
{
  Statistic_2D<ValueType> *tmp_stat;

  row = idRow;
  for( int ii=0; ii< numStats; ii++ )
  {
    tmp_stat = new Statistic_2D<ValueType>;
    values.push_back(tmp_stat);
  }
}


template <typename ValueType>
Cell<ValueType>::~Cell()
{
  for( unsigned int ii = 0; ii < values.size(); ii++ )
    delete values[ ii ];
  values.clear();
}


template <typename ValueType>
void Cell<ValueType>::Init( short idStat )
{
  if( (short)values.size() > idStat )
    values[ idStat ]->Init();
}


template <typename ValueType>
void Cell<ValueType>::Init( )
{
  for( unsigned int ii=0; ii< values.size(); ii++ )
    values[ ii ]->Init();
}


template <typename ValueType>
void Cell<ValueType>::SetValue( short idStat, ValueType semVal )
{
  if( (short)values.size() > idStat )
    values[ idStat ]->SetValue( semVal );
}


template <typename ValueType>
void Cell<ValueType>::SetValue( ValueType semVal )
{
  for( unsigned int ii=0; ii< values.size(); ii++ )
    values[ ii ]->SetValue( semVal );
}


template <typename ValueType>
void Cell<ValueType>::AddValue( short idStat, ValueType semVal )
{
  if( (short)values.size() > idStat )
    values[ idStat ]->AddValue( semVal );
}


template <typename ValueType>
void Cell<ValueType>::AddValue( ValueType semVal )
{
  for( unsigned int ii=0; ii< values.size(); ii++ )
    values[ ii ]->AddValue( semVal );
}


template <typename ValueType>
ValueType Cell<ValueType>::GetValue( short idStat ) const
{
  if ( (short)values.size() > idStat )
    return values[idStat]->GetValue();

  ValueType tmp;

  bzero( (void *) &tmp, sizeof( ValueType ) );
  return tmp;
}


template <typename ValueType>
int Cell<ValueType>::GetRow( ) const
{
  return row;
}


template <typename ValueType>
void Cell<ValueType>::SetRow( int row )
{
  this->row = row;
}


template <typename ValueType>
void Cell<ValueType>::Print( ) const
{
  cout << "[" << row << "] " ;
  for( unsigned int ii = 0; ii< values.size(); ii++ )
  {
    values[ii]->Print();
    cout << " ";
  }

  cout << endl;
}
