#ifndef _CELL_H
#define _CELL_H


#include <vector>

using std::vector;

#include "statistic_2d.h"

template <typename TipusBasic>
class Cell
{
  public:
    Cell( int idRow, short numStats );
    ~Cell();

    void Init( short idStat );
    void Init( );
    void SetValue( short idStat, TipusBasic semVal );
    void SetValue( TipusBasic semVal );
    void AddValue( short idStat, TipusBasic semVal );
    void AddValue( TipusBasic semVal );
    TipusBasic GetValue( short idStat ) const;
    int GetRow( ) const;
    void SetRow( int row );

    void Print() const;

  private:
    int row;
    vector<Statistic_2D<TipusBasic> *>  values;
};

#include "cell.C"

#endif //_CELL_H
