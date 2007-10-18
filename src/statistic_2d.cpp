#include <string>
#include <iostream>

using namespace std;

#include "statistic_2d.h"

template <typename ValueType>
void Statistic_2D<ValueType>::Init()
{
  bzero( (void*) &value, sizeof(ValueType) );
}


template <typename ValueType>
void Statistic_2D<ValueType>::SetValue( ValueType semval )
{
  value = semval;
}


template <typename ValueType>
void Statistic_2D<ValueType>::AddValue( ValueType semval )
{
  value += semval;
}


template <typename ValueType>
void Statistic_2D<ValueType>::Finish()
{

}


template <typename ValueType>
ValueType Statistic_2D<ValueType>::GetValue()
{
  return value;
}


template <typename ValueType>
void Statistic_2D<ValueType>::Print()
{
  cout << value;
}
