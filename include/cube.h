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


#ifndef _CUBE_H
#define _CUBE_H

#include <vector>
#include "matrix.h"

template <typename ValueType>
class Cube
{
  public:
    Cube( PRV_UINT32 numPlanes, PRV_UINT32 numCols, PRV_UINT16 numStats );
    Cube( Cube<ValueType>& source );
    ~Cube();

    void init( PRV_UINT16 idStat );
    void init( );
    void setValue( PRV_UINT32 plane, PRV_UINT32 col, PRV_UINT16 idStat, ValueType semVal );
    void setValue( PRV_UINT32 plane, PRV_UINT32 col, ValueType semVal );
    void setValue( PRV_UINT32 plane, PRV_UINT32 col, const std::vector<ValueType>& semVal, bool isNotZeroValue = true );
    void addValue( PRV_UINT32 plane, PRV_UINT32 col, PRV_UINT16 idStat, ValueType semVal );
    void addValue( PRV_UINT32 plane, PRV_UINT32 col, ValueType semVal );
    void addValue( PRV_UINT32 plane, PRV_UINT32 col, const std::vector<ValueType>& semVal );
    ValueType getCurrentValue( PRV_UINT32 plane, PRV_UINT32 col, PRV_UINT16 idStat ) const;
    std::vector<ValueType> getCurrentValue( PRV_UINT32 plane, PRV_UINT32 col ) const;
    PRV_UINT32 getCurrentRow( PRV_UINT32 plane, PRV_UINT32 col ) const;
    PRV_UINT32 getCurrentRow( ) const;
    bool currentCellModified( PRV_UINT32 plane, PRV_UINT32 col ) const;
    void newRow( );
    void newRow( PRV_UINT32 plane, PRV_UINT32 col, PRV_UINT32 row );
    void finish( );
    void setNextCell( PRV_UINT32 plane, PRV_UINT32 col );
    void setFirstCell( PRV_UINT32 plane, PRV_UINT32 col );
    bool endCell( PRV_UINT32 plane, PRV_UINT32 col );
    bool planeWithValues( PRV_UINT32 plane ) const;
    PRV_UINT32 getPlanes( ) const;
    void eraseColumns( PRV_UINT32 ini_col, PRV_UINT32 fin_col );
    void erasePlanes( PRV_UINT32 ini_plane, PRV_UINT32 fin_plane );

    bool getCellValue( ValueType& semVal, PRV_UINT32 whichPlane, int whichRow, PRV_UINT32 whichCol, PRV_UINT16 idStat ) const;
    bool getCellValue( std::vector<ValueType>& semVal, PRV_UINT32 whichPlane, int whichRow, PRV_UINT32 whichCol ) const;

    void print() const;

  private:
    std::vector<Matrix<ValueType> *> planes;
    PRV_UINT32 nplanes;
    PRV_UINT32 ncols;
    PRV_UINT32 nstat;
    PRV_UINT32 crow;
};

#include "src/cube.cpp"

#endif // _CUBE_H
