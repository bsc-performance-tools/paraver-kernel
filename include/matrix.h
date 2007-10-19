#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector>
#include "column.h"

template <typename ValueType>
class Matrix
{
  public:
    Matrix( int numCols, short numStats );
    Matrix( int currentRow, int numCols, short numStats );
    ~Matrix();

    void Init( short idStat );
    void Init( );
    void SetValue( int col, short idStat, ValueType semVal );
    void SetValue( int col, ValueType semVal );
    void AddValue( int col, short idStat, ValueType semVal );
    void AddValue( int col, ValueType semVal );
    ValueType GetCurrentValue( int col, short idStat ) const;
    int GetCurrentRow( int col ) const;
    void NewRow( );
    void NewRow( int col, int row );
    void Finish( );
    void SetNextCell( int col );
    void SetFirstCell( int col );
    bool EndCell( int col );
    void EraseColumns( int ini_col, int fin_col );

    void Print() const;

  private:
    vector<Column<ValueType> *> cols;
    bool finished;
};

#include "src/matrix.cpp"

#endif // _MATRIX_H
