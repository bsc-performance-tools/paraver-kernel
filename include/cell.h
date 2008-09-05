#ifndef _CELL_H
#define _CELL_H


#include "paraverkerneltypes.h"
#include "statistic_2d.h"

template <typename ValueType>
class Cell
{
  public:
    Cell( TObjectOrder idRow, UINT16 numStats );
    ~Cell();

    void init( UINT16 idStat );
    void init( );
    void setValue( UINT16 idStat, ValueType semVal );
    void setValue( ValueType semVal );
    void addValue( UINT16 idStat, ValueType semVal );
    void addValue( ValueType semVal );
    ValueType getValue( UINT16 idStat ) const;
    TObjectOrder getRow( ) const;
    void setRow( TObjectOrder row );

    void print() const;

  private:
    TObjectOrder row;
    UINT16 nStats;
    Statistic_2D<ValueType> *values;
};

#include "src/cell.cpp"

#endif //_CELL_H
