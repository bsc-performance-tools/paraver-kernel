#ifndef _COLUMN_H
#define _COLUMN_H

/*#include <deque>

using std::deque;*/
#include <vector>

using std::vector;

#include "cell.h"


template <typename ValueType>
class Column
{
  public:
    Column( short numStats, bool *mat_finished );
    Column( int currentRow, short numStats, bool *mat_finished );
    ~Column();

    void Init( short idStat );
    void Init( );
    void SetValue( short idStat, ValueType semVal );
    void SetValue( ValueType semVal );
    void AddValue( short idStat, ValueType semVal );
    void AddValue( ValueType semVal );
    ValueType GetCurrentValue( short idStat ) const;
    int GetCurrentRow( ) const;
    void NewRow( );
    void NewRow( int row );
    void SetNextCell( );
    void SetFirstCell( );
    bool EndCell( );
    void Print() const;

  private:
/*    deque<Cell<ValueType> *> cells;
    typename deque<Cell<ValueType> *>::iterator it_cell;*/
    vector<Cell<ValueType> *> cells;
    typename vector<Cell<ValueType> *>::iterator it_cell;

    short nstat;
    Cell<ValueType> *current_cell;
    bool modified;
    unsigned int n_cells;
    bool *finished;
};

#include "src/column.cpp"

#endif //_COLUMN_H
