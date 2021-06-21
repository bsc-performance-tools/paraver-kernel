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


#ifndef _CELL_H
#define _CELL_H


#include "paraverkerneltypes.h"

template <typename ValueType>
class Cell
{
  public:
    Cell();
    Cell( TObjectOrder idRow, PRV_UINT16 numStats );
    Cell( const Cell< ValueType >& source );
    ~Cell();

    void init( PRV_UINT16 idStat );
    void init( );
    void setValue( PRV_UINT16 idStat, ValueType semVal );
    void setValue( ValueType semVal );
    void setValue( const std::vector<ValueType>& semVal, bool isNotZeroValue = true );
    void addValue( PRV_UINT16 idStat, ValueType semVal );
    void addValue( ValueType semVal );
    void addValue( const std::vector<ValueType>& semVal );
    ValueType getValue( PRV_UINT16 idStat ) const;
    bool getNotZeroValue( PRV_UINT16 idStat ) const;
    std::vector<ValueType> getValue() const;
    TObjectOrder getRow( ) const;
    void setRow( TObjectOrder row );

    bool operator==( const ValueType& anotherCell ) const;
    bool operator<( const ValueType& anotherCell ) const;

    void print() const;

  private:
    TObjectOrder row;
    PRV_UINT16 nStats;
    std::vector<ValueType> values;
    bool isNotZeroValue;
};

#include "src/cell.cpp"

#endif //_CELL_H
