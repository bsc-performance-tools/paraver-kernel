#ifndef _STATISTIC_2D_H
#define _STATISTIC_2D_H

template <typename ValueType>
class Statistic_2D
{
  public:
    Statistic_2D()
    {}
    ~Statistic_2D()
    {}

    void init();
    void setValue( ValueType semval );
    void addValue( ValueType semval );
    void finish();

    ValueType getValue();

    void print();

  protected:
    ValueType value;

};

#include "src/statistic_2d.cpp"

#endif //_STATISTIC_2D_H
