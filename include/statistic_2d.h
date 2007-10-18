#ifndef _STATISTIC_2D_H
#define _STATISTIC_2D_H

template <typename TipusBasic>
class Statistic_2D
{
  public:
    Statistic_2D(){};
    ~Statistic_2D(){};
  
    void Init();
    void SetValue( TipusBasic semval );
    void AddValue( TipusBasic semval );
    void Finish();

    TipusBasic GetValue();

    void Print();

  protected:
    TipusBasic value;

};

#include "statistic_2d.C"

#endif //_STATISTIC_2D_H
