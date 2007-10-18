#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector>
#include "column.h"

template <typename TipusBasic>
class Matrix2D
{
  public:
    Matrix2D( int numCols, short numStats );
    Matrix2D( int currentRow, int numCols, short numStats );
    ~Matrix2D();
  
    void Init( short idStat );
    void Init( );
    void SetValue( int col, short idStat, TipusBasic semVal );
    void SetValue( int col, TipusBasic semVal );
    void AddValue( int col, short idStat, TipusBasic semVal );
    void AddValue( int col, TipusBasic semVal );
    TipusBasic GetCurrentValue( int col, short idStat ) const;
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
    vector<Column<TipusBasic> *> cols;
    bool finished;
};

#include "matrix.C"

#endif // _MATRIX_H
