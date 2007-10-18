#ifndef _COLUMN_H
#define _COLUMN_H

/*#include <deque>

using std::deque;*/
#include <vector>

using std::vector;

#include "cell.h"


template <typename TipusBasic>
class Column
{
  public:
    Column( short numStats, bool *mat_finished );
    Column( int currentRow, short numStats, bool *mat_finished );
    ~Column();

    void Init( short idStat );
    void Init( );
    void SetValue( short idStat, TipusBasic semVal );
    void SetValue( TipusBasic semVal );
    void AddValue( short idStat, TipusBasic semVal );
    void AddValue( TipusBasic semVal );
    TipusBasic GetCurrentValue( short idStat ) const;
    int GetCurrentRow( ) const;
    void NewRow( );
    void NewRow( int row );
    void SetNextCell( );
    void SetFirstCell( );
    bool EndCell( );
    void Print() const;
    
  private:
/*    deque<Cell<TipusBasic> *> cells;
    typename deque<Cell<TipusBasic> *>::iterator it_cell;*/
    vector<Cell<TipusBasic> *> cells;
    typename vector<Cell<TipusBasic> *>::iterator it_cell;

    short nstat;
    Cell<TipusBasic> *current_cell;
    bool modified;
    unsigned int n_cells;
    bool *finished;
};

#include "column.C"

#endif //_COLUMN_H
