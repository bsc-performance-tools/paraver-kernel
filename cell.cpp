#include <string>
#include <iostream>

using namespace std;

#include "cell.h"

template <typename TipusBasic>
Cell<TipusBasic>::Cell( int idRow, short numStats )
{
  Statistic_2D<TipusBasic> *tmp_stat;

  row = idRow;
  for( int ii=0; ii< numStats; ii++ )
  {
    tmp_stat = new Statistic_2D<TipusBasic>;
    values.push_back(tmp_stat);
  }
}


template <typename TipusBasic>
Cell<TipusBasic>::~Cell()
{
  for( unsigned int ii = 0; ii < values.size(); ii++ )
    delete values[ ii ];
  values.clear();
}


template <typename TipusBasic>
void Cell<TipusBasic>::Init( short idStat )
{
  if( (short)values.size() > idStat )
    values[ idStat ]->Init();
}


template <typename TipusBasic>
void Cell<TipusBasic>::Init( )
{
  for( unsigned int ii=0; ii< values.size(); ii++ )
    values[ ii ]->Init();
}


template <typename TipusBasic>
void Cell<TipusBasic>::SetValue( short idStat, TipusBasic semVal )
{
  if( (short)values.size() > idStat )
    values[ idStat ]->SetValue( semVal );
}


template <typename TipusBasic>
void Cell<TipusBasic>::SetValue( TipusBasic semVal )
{
  for( unsigned int ii=0; ii< values.size(); ii++ )
    values[ ii ]->SetValue( semVal );
}


template <typename TipusBasic>
void Cell<TipusBasic>::AddValue( short idStat, TipusBasic semVal )
{
  if( (short)values.size() > idStat )
    values[ idStat ]->AddValue( semVal );
}


template <typename TipusBasic>
void Cell<TipusBasic>::AddValue( TipusBasic semVal )
{
  for( unsigned int ii=0; ii< values.size(); ii++ )
    values[ ii ]->AddValue( semVal );
}


template <typename TipusBasic>
TipusBasic Cell<TipusBasic>::GetValue( short idStat ) const
{
  if ( (short)values.size() > idStat )
    return values[idStat]->GetValue();

  TipusBasic tmp;
  
  bzero( (void *) &tmp, sizeof( TipusBasic ) );
  return tmp;
}


template <typename TipusBasic>
int Cell<TipusBasic>::GetRow( ) const
{
  return row;
}


template <typename TipusBasic>
void Cell<TipusBasic>::SetRow( int row )
{
  this->row = row;
}


template <typename TipusBasic>
void Cell<TipusBasic>::Print( ) const
{
  cout << "[" << row << "] " ;
  for( unsigned int ii = 0; ii< values.size(); ii++ )
  {
    values[ii]->Print();
    cout << " ";
  }

  cout << endl;
}
