#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector>
#include "column.h"
#include "paraverkerneltypes.h"

template <typename ValueType>
class Matrix
{
  public:
    Matrix( UINT32 numCols, UINT16 numStats );
    Matrix( TObjectOrder currentRow, UINT32 numCols, UINT16 numStats );
    ~Matrix();

    void init( UINT16 idStat );
    void init( );
    void setValue( UINT32 col, UINT16 idStat, ValueType semVal );
    void setValue( UINT32 col, ValueType semVal );
    void addValue( UINT32 col, UINT16 idStat, ValueType semVal );
    void addValue( UINT32 col, ValueType semVal );
    ValueType getCurrentValue( UINT32 col, UINT16 idStat ) const;
    TObjectOrder getCurrentRow( UINT32 col ) const;
    void newRow( );
    void newRow( UINT32 col, TObjectOrder row );
    void finish( );
    void setNextCell( UINT32 col );
    void setFirstCell( UINT32 col );
    bool endCell( UINT32 col );
    void eraseColumns( UINT32 ini_col, UINT32 fin_col );

    void print() const;

  private:
    vector<Column<ValueType> *> cols;
    bool finished;
};

#include "src/matrix.cpp"

#endif // _MATRIX_H
