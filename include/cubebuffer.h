/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                            libparaver-kernel                              *
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

#include <array>
#include <unordered_map>
#include <vector>

#include "paraverkerneltypes.h" 

template< size_t NStats >
class CubeBuffer
{
  public:
    CubeBuffer( PRV_UINT32 numPlanes, PRV_UINT32 numRows );

    void addValue( PRV_UINT32 plane, PRV_UINT32 row, THistogramColumn col, const std::array< TSemanticValue, NStats >& semVal, bool isNotZeroValue = true );
    void setValue( PRV_UINT32 plane, PRV_UINT32 row, THistogramColumn col, const std::array< TSemanticValue, NStats >& semVal );
    bool getCellValue( std::array< TSemanticValue, NStats >& semVal, PRV_UINT32 plane, PRV_UINT32 row, PRV_UINT32 col ) const;

    const std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > >& getRowValues( PRV_UINT32 plane, PRV_UINT32 row ) const;
    const std::unordered_map< THistogramColumn, bool >& getNotZeroValue( PRV_UINT32 plane, PRV_UINT32 row ) const;

  private:
    std::vector< std::vector< std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > > > > buffer;
    std::vector< std::vector< std::unordered_map< THistogramColumn, bool > > > bufferNotZeroValue;
};

#include "src/cubebuffer.cpp"
