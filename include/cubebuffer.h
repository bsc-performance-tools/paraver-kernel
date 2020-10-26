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



#ifndef CUBEBUFFER_H_INCLUDED
#define CUBEBUFFER_H_INCLUDED

#include <vector>
#include <map>
#include <ext/hash_map>

#include "paraverkerneltypes.h"

using namespace __gnu_cxx;

class CubeBuffer
{
  public:
    CubeBuffer( PRV_UINT32 numPlanes, PRV_UINT32 numRows );
    ~CubeBuffer();

    void addValue( PRV_UINT32 plane, PRV_UINT32 row, THistogramColumn col, const std::vector< TSemanticValue >& semVal );
    void setValue( PRV_UINT32 plane, PRV_UINT32 row, THistogramColumn col, const std::vector< TSemanticValue >& semVal );
    bool getCellValue( std::vector< TSemanticValue >& semVal, PRV_UINT32 plane, PRV_UINT32 row, PRV_UINT32 col ) const;
#ifdef PARALLEL_ENABLED
    const std::map< THistogramColumn, std::vector< TSemanticValue > >& getRowValues( PRV_UINT32 plane, PRV_UINT32 row ) const;
#else
    const hash_map< THistogramColumn, std::vector< TSemanticValue > >& getRowValues( PRV_UINT32 plane, PRV_UINT32 row ) const;
#endif

  private:
#ifdef PARALLEL_ENABLED
    std::vector< std::vector< std::map< THistogramColumn, std::vector< TSemanticValue > > > > buffer;
#else
    std::vector< std::vector< hash_map< THistogramColumn, std::vector< TSemanticValue > > > > buffer;
#endif
};


#endif // CUBEBUFFER_H_INCLUDED
