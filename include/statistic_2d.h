#ifndef _STATISTIC_2D_H
#define _STATISTIC_2D_H

template <typename ValueType>
class Statistic_2D
{
  public:
    Statistic_2D(){};
    ~Statistic_2D(){};

    void Init();
    void SetValue( ValueType semval );
    void AddValue( ValueType semval );
    void Finish();

    ValueType GetValue();

    void Print();

  protected:
    ValueType value;

};

#include "statistic_2d.cpp"

#endif //_STATISTIC_2D_H
