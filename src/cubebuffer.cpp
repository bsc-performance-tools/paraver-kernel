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


#include "cubebuffer.h"
#include <functional>

using std::vector;
using std::map;
using std::unordered_map;


CubeBuffer::CubeBuffer( PRV_UINT32 numPlanes, PRV_UINT32 numRows )
{
#ifdef PARALLEL_ENABLED
  map< THistogramColumn, vector< TSemanticValue > > tmpRow;
  vector< map< THistogramColumn, vector< TSemanticValue > > > tmpMatrix( numRows, tmpRow );

  map< THistogramColumn, bool > tmpRowNotZeroValue;
  vector< map< THistogramColumn, bool > > tmpMatrixNotZeroValue( numRows, tmpRowNotZeroValue );
#else
  unordered_map< THistogramColumn, vector< TSemanticValue > > tmpRow;
  vector< unordered_map< THistogramColumn, vector< TSemanticValue > > > tmpMatrix( numRows, tmpRow );

  unordered_map< THistogramColumn, bool > tmpRowNotZeroValue;
  vector< unordered_map< THistogramColumn, bool > > tmpMatrixNotZeroValue( numRows, tmpRowNotZeroValue );
#endif
  buffer.insert( buffer.begin(), numPlanes, tmpMatrix );
  bufferNotZeroValue.insert( bufferNotZeroValue.begin(), numPlanes, tmpMatrixNotZeroValue );
}


CubeBuffer::~CubeBuffer()
{}


void CubeBuffer::addValue( PRV_UINT32 plane, PRV_UINT32 row, THistogramColumn col, const std::vector< TSemanticValue >& semVal, bool isNotZeroValue )
{
#ifdef PARALLEL_ENABLED
  map< THistogramColumn, vector< TSemanticValue > >& currentRow = ( ( buffer[ plane ] )[ row ] );
  map< THistogramColumn, vector< TSemanticValue > >::iterator it = currentRow.find( col );

  map< THistogramColumn, bool >& currentRowNotZeroValue = ( ( bufferNotZeroValue[ plane ] )[ row ] );
  map< THistogramColumn, bool >::iterator itNotZeroValue = currentRowNotZeroValue.find( col );
#else
  unordered_map< THistogramColumn, vector< TSemanticValue > >& currentRow = ( ( buffer[ plane ] )[ row ] );
  unordered_map< THistogramColumn, vector< TSemanticValue > >::iterator it = currentRow.find( col );

  unordered_map< THistogramColumn, bool >& currentRowNotZeroValue = ( ( bufferNotZeroValue[ plane ] )[ row ] );
  unordered_map< THistogramColumn, bool >::iterator itNotZeroValue = currentRowNotZeroValue.find( col );
#endif
  if ( it != currentRow.end() )
  {
    for( PRV_UINT32 i = 0; i < semVal.size(); ++i )
      ( it->second )[ i ] += semVal[ i ];

    itNotZeroValue->second = itNotZeroValue->second || isNotZeroValue;
  }
  else
  {
    currentRow[ col ] = semVal;
    currentRowNotZeroValue[ col ] = isNotZeroValue;
  }
}


void CubeBuffer::setValue( PRV_UINT32 plane, PRV_UINT32 row, THistogramColumn col, const std::vector< TSemanticValue >& semVal )
{
  ( ( buffer[ plane ] )[ row ] )[ col ] = semVal;
}


bool CubeBuffer::getCellValue( std::vector< TSemanticValue >& semVal, PRV_UINT32 plane, PRV_UINT32 row, PRV_UINT32 col ) const
{
#ifdef PARALLEL_ENABLED
  const map< THistogramColumn, vector< TSemanticValue > >& currentRow = ( ( buffer[ plane ] )[ row ] );
  map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = currentRow.find( col );
#else
  const unordered_map< THistogramColumn, vector< TSemanticValue > >& currentRow = ( ( buffer[ plane ] )[ row ] );
  unordered_map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = currentRow.find( col );
#endif

  if ( it == currentRow.end() )
    return false;

  semVal = it->second;
  return true;
}


#ifdef PARALLEL_ENABLED
const map< THistogramColumn, vector< TSemanticValue > >& CubeBuffer::getRowValues( PRV_UINT32 plane, PRV_UINT32 row ) const
#else
const unordered_map< THistogramColumn, vector< TSemanticValue > >& CubeBuffer::getRowValues( PRV_UINT32 plane, PRV_UINT32 row ) const
#endif
{
  return ( ( buffer[ plane ] )[ row ] );
}

#ifdef PARALLEL_ENABLED
const map< THistogramColumn, bool >& CubeBuffer::getNotZeroValue( PRV_UINT32 plane, PRV_UINT32 row ) const
#else
const unordered_map< THistogramColumn, bool >& CubeBuffer::getNotZeroValue( PRV_UINT32 plane, PRV_UINT32 row ) const
#endif
{
  return ( ( bufferNotZeroValue[ plane ] )[ row ] );
}
