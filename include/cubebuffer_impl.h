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


template< size_t NStats >
CubeBuffer<NStats>::CubeBuffer( PRV_UINT32 numPlanes, PRV_UINT32 numRows )
  : planeNotEmpty( numPlanes )
{
  std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > > tmpRow;
  std::vector< std::unordered_map< THistogramColumn, std::array< TSemanticValue, NStats > > > tmpMatrix( numRows, tmpRow );

  std::unordered_map< THistogramColumn, bool > tmpRowNotZeroValue;
  std::vector< std::unordered_map< THistogramColumn, bool > > tmpMatrixNotZeroValue( numRows, tmpRowNotZeroValue );
  std::unordered_map< THistogramColumn, PRV_UINT32 > tmpCurrentCell {};

  buffer.insert( buffer.begin(), numPlanes, tmpMatrix );
  bufferNotZeroValue.insert( bufferNotZeroValue.begin(), numPlanes, tmpMatrixNotZeroValue );
  currentCell.insert( currentCell.begin(), numPlanes, tmpCurrentCell );
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
    planeNotEmpty[ plane ] = true;
  }
}


template< size_t NStats >
void CubeBuffer<NStats>::setValue( PRV_UINT32 plane, PRV_UINT32 row, THistogramColumn col, const std::array< TSemanticValue, NStats >& semVal )
{
  ( ( buffer[ plane ] )[ row ] )[ col ] = semVal;
  planeNotEmpty[ plane ] = true;
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

template< size_t NStats >
void CubeBuffer<NStats>::setFirstCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  // currentCell = std::make_tuple( plane, col, 0 );
  ( currentCell[ plane ] )[ col ] = 0;
}

template< size_t NStats >
void CubeBuffer<NStats>::setNextCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  // auto currentRow = std::get<2>( currentCell );
  // currentCell = std::tie( plane, col, ++currentRow );
  const std::unordered_map< THistogramColumn, PRV_UINT32 >& currentPlane = currentCell[ plane ];
  typename std::unordered_map< THistogramColumn, PRV_UINT32 >::const_iterator it = currentPlane.find( col );

  if ( it == currentPlane.end() )
    currentCell[ plane ][ col ] = 0;
  else
    ++( currentCell[ plane ][ col ] );
}

template< size_t NStats >
bool CubeBuffer<NStats>::endCell( PRV_UINT32 col, PRV_UINT32 plane ) const
{
  // return std::get<2>( currentCell ) > buffer[ plane ].size();
  const std::unordered_map< THistogramColumn, PRV_UINT32 >& currentPlane = currentCell[ plane ];
  typename std::unordered_map< THistogramColumn, PRV_UINT32 >::const_iterator it = currentPlane.find( col );

  if ( it == currentPlane.end() )
    return false;

  // return ( currentCell[ plane ] )[ col ] > buffer[ plane ].size();
  return it->second > buffer[ plane ].size();
}

template< size_t NStats >
bool CubeBuffer<NStats>::planeWithValues( PRV_UINT32 plane ) const
{
  return planeNotEmpty[ plane ];
}


template< size_t NStats >
TSemanticValue CubeBuffer<NStats>::getCurrentValue( PRV_UINT32 plane, PRV_UINT32 col, PRV_UINT16 idStat ) const
{
  std::array<TSemanticValue, NStats> tmpValues = {};

  const std::unordered_map< THistogramColumn, PRV_UINT32 >& currentPlane = currentCell[ plane ];
  typename std::unordered_map< THistogramColumn, PRV_UINT32 >::const_iterator it = currentPlane.find( col );

  getCellValue( tmpValues, plane, col, it->second );

  return tmpValues[ idStat ];
}

template< size_t NStats >
std::array<TSemanticValue, NStats> CubeBuffer<NStats>::getCurrentValue( PRV_UINT32 plane, PRV_UINT32 col ) const
{
  std::array<TSemanticValue, NStats> tmpValues = {};

  const std::unordered_map< THistogramColumn, PRV_UINT32 >& currentPlane = currentCell[ plane ];
  typename std::unordered_map< THistogramColumn, PRV_UINT32 >::const_iterator it = currentPlane.find( col );

  getCellValue( tmpValues, plane, col, it->second );

  return tmpValues;
}

template< size_t NStats >
PRV_UINT32 CubeBuffer<NStats>::getCurrentRow( PRV_UINT32 plane, PRV_UINT32 col ) const
{
  const std::unordered_map< THistogramColumn, PRV_UINT32 >& currentPlane = currentCell[ plane ];
  typename std::unordered_map< THistogramColumn, PRV_UINT32 >::const_iterator it = currentPlane.find( col );

  return it->second;
}
