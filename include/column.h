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

    void init( short idStat );
    void init( );
    void setValue( short idStat, ValueType semVal );
    void setValue( ValueType semVal );
    void addValue( short idStat, ValueType semVal );
    void addValue( ValueType semVal );
    ValueType getCurrentValue( short idStat ) const;
    int getCurrentRow( ) const;
    bool currentCellModified( ) const;
    void newRow( );
    void newRow( int row );
    void setNextCell( );
    void setFirstCell( );
    bool endCell( );
    void print() const;

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
