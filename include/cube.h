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

#ifndef _CUBE_H
#define _CUBE_H

#include <vector>
#include "matrix.h"

template <typename ValueType>
class Cube
{
  public:
    Cube( UINT32 numPlanes, UINT32 numCols, UINT16 numStats );
    Cube( Cube<ValueType>& source );
    ~Cube();

    void init( UINT16 idStat );
    void init( );
    void setValue( UINT32 plane, UINT32 col, UINT16 idStat, ValueType semVal );
    void setValue( UINT32 plane, UINT32 col, ValueType semVal );
    void setValue( UINT32 plane, UINT32 col, const vector<ValueType>& semVal );
    void addValue( UINT32 plane, UINT32 col, UINT16 idStat, ValueType semVal );
    void addValue( UINT32 plane, UINT32 col, ValueType semVal );
    void addValue( UINT32 plane, UINT32 col, const vector<ValueType>& semVal );
    ValueType getCurrentValue( UINT32 plane, UINT32 col, UINT16 idStat ) const;
    vector<ValueType> getCurrentValue( UINT32 plane, UINT32 col ) const;
    UINT32 getCurrentRow( UINT32 plane, UINT32 col ) const;
    UINT32 getCurrentRow( ) const;
    bool currentCellModified( UINT32 plane, UINT32 col ) const;
    void newRow( );
    void newRow( UINT32 plane, UINT32 col, UINT32 row );
    void finish( );
    void setNextCell( UINT32 plane, UINT32 col );
    void setFirstCell( UINT32 plane, UINT32 col );
    bool endCell( UINT32 plane, UINT32 col );
    bool planeWithValues( UINT32 plane ) const;
    UINT32 getPlanes( ) const;
    void eraseColumns( UINT32 ini_col, UINT32 fin_col );
    void erasePlanes( UINT32 ini_plane, UINT32 fin_plane );

    void print() const;

  private:
    vector<Matrix<ValueType> *> planes;
    UINT32 nplanes;
    UINT32 ncols;
    UINT32 nstat;
    UINT32 crow;
};

#include "src/cube.cpp"

#endif // _CUBE_H
