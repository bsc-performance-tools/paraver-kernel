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

    void Init( UINT16 idStat );
    void Init( );
    void SetValue( UINT32 col, UINT16 idStat, ValueType semVal );
    void SetValue( UINT32 col, ValueType semVal );
    void AddValue( UINT32 col, UINT16 idStat, ValueType semVal );
    void AddValue( UINT32 col, ValueType semVal );
    ValueType GetCurrentValue( UINT32 col, UINT16 idStat ) const;
    TObjectOrder GetCurrentRow( UINT32 col ) const;
    void NewRow( );
    void NewRow( UINT32 col, TObjectOrder row );
    void Finish( );
    void SetNextCell( UINT32 col );
    void SetFirstCell( UINT32 col );
    bool EndCell( UINT32 col );
    void EraseColumns( UINT32 ini_col, UINT32 fin_col );

    void Print() const;

  private:
    vector<Column<ValueType> *> cols;
    bool finished;
};

#include "src/matrix.cpp"

#endif // _MATRIX_H
