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


#pragma once

#include <vector>
#include "cell.h"


template <typename ValueType, size_t NStats>
class Column
{
  public:
    Column( bool *mat_finished );
    Column( int currentRow, bool *mat_finished );
    Column( const Column<ValueType, NStats>& source );

    void init( short idStat );
    void init();
    void setValue( short idStat, ValueType semVal );
    void setValue( const std::array<ValueType, NStats>& semVal, bool isNotZeroValue = true );
    void addValue( short idStat, ValueType semVal );
    void addValue( const std::array<ValueType, NStats>& semVal );
    ValueType getCurrentValue( short idStat ) const;
    std::array<ValueType, NStats> getCurrentValue() const;
    int getCurrentRow( ) const;
    bool currentCellModified( ) const;
    void newRow( );
    void newRow( int row );
    void setNextCell( );
    void setFirstCell( );
    bool endCell( );

    bool getCellValue( ValueType& semVal, int whichRow, short idStat ) const;
    bool getNotZeroValue( int whichRow, short idStat ) const;
    bool getCellValue( std::array<ValueType, NStats>& semVal, int whichRow ) const;

  private:
    std::vector<Cell<ValueType, NStats> > cells;
    typename std::vector<Cell<ValueType, NStats> >::iterator it_cell;

    Cell<ValueType, NStats> current_cell;
    bool modified;
    bool *finished;
};

#include "column_impl.h"
