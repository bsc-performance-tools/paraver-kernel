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

//#include "cubebuffer.h"

template< size_t NStats >
CubeBuffer<NStats>::CubeBuffer( PRV_UINT32 numPlanes, PRV_UINT32 numRows )
{
  std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > > tmpRow;
  std::vector< std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > > > tmpMatrix( numRows, tmpRow );

  std::unordered_map< THistogramColumn, bool > tmpRowNotZeroValue;
  std::vector< std::unordered_map< THistogramColumn, bool > > tmpMatrixNotZeroValue( numRows, tmpRowNotZeroValue );

  buffer.insert( buffer.begin(), numPlanes, tmpMatrix );
  bufferNotZeroValue.insert( bufferNotZeroValue.begin(), numPlanes, tmpMatrixNotZeroValue );
}


template< size_t NStats >
void CubeBuffer<NStats>::addValue( PRV_UINT32 plane, PRV_UINT32 row, THistogramColumn col, const std::array< TSemanticValue, NStats >& semVal, bool isNotZeroValue )
{
  std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > >& currentRow = ( ( buffer[ plane ] )[ row ] );
  typename std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > >::iterator it = currentRow.find( col );

  std::unordered_map< THistogramColumn, bool >& currentRowNotZeroValue = ( ( bufferNotZeroValue[ plane ] )[ row ] );
  std::unordered_map< THistogramColumn, bool >::iterator itNotZeroValue = currentRowNotZeroValue.find( col );

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


template< size_t NStats >
void CubeBuffer<NStats>::setValue( PRV_UINT32 plane, PRV_UINT32 row, THistogramColumn col, const std::array< TSemanticValue, NStats >& semVal )
{
  ( ( buffer[ plane ] )[ row ] )[ col ] = semVal;
}


template< size_t NStats >
bool CubeBuffer<NStats>::getCellValue( std::array< TSemanticValue, NStats >& semVal, PRV_UINT32 plane, PRV_UINT32 row, PRV_UINT32 col ) const
{
  const std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > >& currentRow = ( ( buffer[ plane ] )[ row ] );
  typename std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > >::const_iterator it = currentRow.find( col );

  if ( it == currentRow.end() )
    return false;

  semVal = it->second;
  return true;
}


template< size_t NStats >
const std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > >& CubeBuffer<NStats>::getRowValues( PRV_UINT32 plane, PRV_UINT32 row ) const
{
  return ( ( buffer[ plane ] )[ row ] );
}

template< size_t NStats >
const std::unordered_map< THistogramColumn, bool >& CubeBuffer<NStats>::getNotZeroValue( PRV_UINT32 plane, PRV_UINT32 row ) const
{
  return ( ( bufferNotZeroValue[ plane ] )[ row ] );
}
