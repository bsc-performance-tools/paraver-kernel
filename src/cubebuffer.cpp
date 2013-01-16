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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "cubebuffer.h"

using std::vector;
using std::map;


CubeBuffer::CubeBuffer( PRV_UINT32 numPlanes, PRV_UINT32 numRows )
{
  map< THistogramColumn, vector< TSemanticValue > > tmpRow;
  vector< map< THistogramColumn, vector< TSemanticValue > > > tmpMatrix( numRows, tmpRow );
  buffer.insert( buffer.begin(), numPlanes, tmpMatrix );
}


CubeBuffer::~CubeBuffer()
{}


void CubeBuffer::addValue( PRV_UINT32 plane, PRV_UINT32 row, THistogramColumn col, const std::vector< TSemanticValue >& semVal )
{
  map< THistogramColumn, vector< TSemanticValue > >::iterator it = ( ( buffer[ plane ] )[ row ] ).find( col );
  if ( it != ( ( buffer[ plane ] )[ row ] ).end() )
  {
    for( PRV_UINT32 i = 0; i < semVal.size(); ++i )
      ( it->second )[ i ] += semVal[ i ];
  }
  else
  {
    ( ( buffer[ plane ] )[ row ] )[ col ] = semVal;
  }
}


bool CubeBuffer::getCellValue( std::vector< TSemanticValue >& semVal, PRV_UINT32 plane, PRV_UINT32 row, PRV_UINT32 col ) const
{
  map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = ( ( buffer[ plane ] )[ row ] ).find( col );
  if ( it == ( ( buffer[ plane ] )[ row ] ).end() )
    return false;

  semVal = it->second;
  return true;
}
