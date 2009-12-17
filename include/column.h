/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

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
