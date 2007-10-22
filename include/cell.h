#ifndef _CELL_H
#define _CELL_H


#include <vector>

using std::vector;

#include "paraverkerneltypes.h"
#include "statistic_2d.h"

template <typename ValueType>
class Cell
{
  public:
    Cell( TObjectOrder idRow, UINT16 numStats );
    ~Cell();

    void Init( UINT16 idStat );
    void Init( );
    void SetValue( UINT16 idStat, ValueType semVal );
    void SetValue( ValueType semVal );
    void AddValue( UINT16 idStat, ValueType semVal );
    void AddValue( ValueType semVal );
    ValueType GetValue( UINT16 idStat ) const;
    TObjectOrder GetRow( ) const;
    void SetRow( TObjectOrder row );

    void Print() const;

  private:
    TObjectOrder row;
    vector<Statistic_2D<ValueType> *>  values;
};

#include "src/cell.cpp"

#endif //_CELL_H
