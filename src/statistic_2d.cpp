#include <string>
#include <iostream>

using namespace std;

template <typename ValueType>
void Statistic_2D<ValueType>::init()
{
  bzero( ( void* ) &value, sizeof( ValueType ) );
}


template <typename ValueType>
void Statistic_2D<ValueType>::setValue( ValueType semval )
{
  value = semval;
}


template <typename ValueType>
void Statistic_2D<ValueType>::addValue( ValueType semval )
{
  value += semval;
}


template <typename ValueType>
void Statistic_2D<ValueType>::finish()
{

}


template <typename ValueType>
ValueType Statistic_2D<ValueType>::getValue()
{
  return value;
}


template <typename ValueType>
void Statistic_2D<ValueType>::print()
{
  cout << value;
}
