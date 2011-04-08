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

#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector>
#include "column.h"
#include "paraverkerneltypes.h"

template <typename ValueType>
class Matrix
{
  public:
    Matrix( PRV_UINT32 numCols, PRV_UINT16 numStats );
    Matrix( TObjectOrder currentRow, PRV_UINT32 numCols, PRV_UINT16 numStats );
    Matrix( Matrix<ValueType>& source );
    ~Matrix();

    void init( PRV_UINT16 idStat );
    void init( );
    void setValue( PRV_UINT32 col, PRV_UINT16 idStat, ValueType semVal );
    void setValue( PRV_UINT32 col, ValueType semVal );
    void setValue( PRV_UINT32 col, const vector<ValueType>& semVal );
    void addValue( PRV_UINT32 col, PRV_UINT16 idStat, ValueType semVal );
    void addValue( PRV_UINT32 col, ValueType semVal );
    void addValue( PRV_UINT32 col, const vector<ValueType>& semVal );
    ValueType getCurrentValue( PRV_UINT32 col, PRV_UINT16 idStat ) const;
    vector<ValueType> getCurrentValue( PRV_UINT32 col ) const;
    TObjectOrder getCurrentRow( PRV_UINT32 col ) const;
    bool currentCellModified( PRV_UINT32 col ) const;
    void newRow( );
    void newRow( PRV_UINT32 col, TObjectOrder row );
    void finish( );
    void setNextCell( PRV_UINT32 col );
    void setFirstCell( PRV_UINT32 col );
    bool endCell( PRV_UINT32 col );
    void eraseColumns( PRV_UINT32 ini_col, PRV_UINT32 fin_col );

    bool getCellValue( ValueType& semVal, int whichRow, PRV_UINT32 whichCol, PRV_UINT16 idStat ) const;
    bool getCellValue( vector<ValueType>& semVal, int whichRow, PRV_UINT32 whichCol ) const;

    void print() const;

  private:
    vector<Column<ValueType> > cols;
    bool finished;
};

#include "src/matrix.cpp"

#endif // _MATRIX_H
