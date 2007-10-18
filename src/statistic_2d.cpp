#include <string>
#include <iostream>

using namespace std;

#include "statistic_2d.h"

template <typename TipusBasic>
void Statistic_2D<TipusBasic>::Init()
{
  bzero( (void*) &value, sizeof(TipusBasic) );
}


template <typename TipusBasic>
void Statistic_2D<TipusBasic>::SetValue( TipusBasic semval )
{
  value = semval;
}


template <typename TipusBasic>
void Statistic_2D<TipusBasic>::AddValue( TipusBasic semval )
{
  value += semval;
}


template <typename TipusBasic>
void Statistic_2D<TipusBasic>::Finish()
{
  
}


template <typename TipusBasic>
TipusBasic Statistic_2D<TipusBasic>::GetValue()
{
  return value;
}


template <typename TipusBasic>
void Statistic_2D<TipusBasic>::Print()
{
  cout << value;
}
