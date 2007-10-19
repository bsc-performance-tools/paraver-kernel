#ifndef _CELL_H
#define _CELL_H


#include <vector>

using std::vector;

#include "statistic_2d.h"

template <typename ValueType>
class Cell
{
  public:
    Cell( int idRow, short numStats );
    ~Cell();

    void Init( short idStat );
    void Init( );
    void SetValue( short idStat, ValueType semVal );
    void SetValue( ValueType semVal );
    void AddValue( short idStat, ValueType semVal );
    void AddValue( ValueType semVal );
    ValueType GetValue( short idStat ) const;
    int GetRow( ) const;
    void SetRow( int row );

    void Print() const;

  private:
    int row;
    vector<Statistic_2D<ValueType> *>  values;
};

#include "src/cell.cpp"

#endif //_CELL_H
