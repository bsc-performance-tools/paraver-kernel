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
    Column( const Column<ValueType>& source );
    ~Column();

    void init( short idStat );
    void init( );
    void setValue( short idStat, ValueType semVal );
    void setValue( ValueType semVal );
    void setValue( const vector<ValueType>& semVal );
    void addValue( short idStat, ValueType semVal );
    void addValue( ValueType semVal );
    void addValue( const vector<ValueType>& semVal );
    ValueType getCurrentValue( short idStat ) const;
    vector<ValueType> getCurrentValue() const;
    int getCurrentRow( ) const;
    bool currentCellModified( ) const;
    void newRow( );
    void newRow( int row );
    void setNextCell( );
    void setFirstCell( );
    bool endCell( );
    void print() const;

  private:
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
