#include <string>
#include <iostream>

using namespace std;

template <typename ValueType>
inline void Statistic_2D<ValueType>::init()
{
  memset( ( void* ) &value, 0, sizeof( ValueType ) );
}


template <typename ValueType>
inline void Statistic_2D<ValueType>::setValue( ValueType semval )
{
  value = semval;
}


template <typename ValueType>
inline void Statistic_2D<ValueType>::addValue( ValueType semval )
{
  value += semval;
}


template <typename ValueType>
inline void Statistic_2D<ValueType>::finish()
{

}


template <typename ValueType>
inline ValueType Statistic_2D<ValueType>::getValue()
{
  return value;
}


template <typename ValueType>
inline void Statistic_2D<ValueType>::print()
{
  cout << value;
}
