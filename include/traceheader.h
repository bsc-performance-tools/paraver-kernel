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

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "paraverkerneltypes.h"
#include "traceheaderexception.h"

template< class TraceStreamT,
          typename TimeT,
          class ResourceModelT,
          class ProcessModelT >
void parseTraceHeader( TraceStreamT traceStream,
                       std::string& traceDate,
                       TTimeUnit& traceTimeUnit,
                       TimeT& traceEndTime,
                       ResourceModelT& traceResourceModel,
                       ProcessModelT& traceProcessModel,
                       std::vector< std::string >& communicators )
{
  std::string tmpstr;

  traceStream->getline( tmpstr );
  if ( tmpstr.compare( "new format" ) == 0 )
    traceStream->getline( tmpstr );

  std::istringstream header( tmpstr );

  std::getline( header, traceDate, ')' );
  traceDate = traceDate.substr( traceDate.find_first_of( '(' ) + 1 );

  header.get();

  std::getline( header, tmpstr, ':' );
  size_t pos = tmpstr.find( '_' );
  if ( pos == string::npos )
  {
    // No '_' char found. The trace is in us.
    traceTimeUnit = US;
    std::istringstream stringEndTime( tmpstr );
    if ( !( stringEndTime >> traceEndTime ) )
    {
      throw TraceHeaderException( TTraceHeaderErrorCode::invalidTime,
                                  tmpstr.c_str() );
    }
  }
  else
  {
    string strTimeUnit( tmpstr.substr( pos, tmpstr.length() ) );
    if ( strTimeUnit == "_ns" )
      traceTimeUnit = NS;
    else if ( strTimeUnit == "_ms" )
      traceTimeUnit = MS;
    else //if ( strTimeUnit == "_us" )
      traceTimeUnit = US;

    std::istringstream stringEndTime( tmpstr.substr( 0, pos ) );
    if ( !( stringEndTime >> traceEndTime ) )
    {
      throw TraceHeaderException( TTraceHeaderErrorCode::invalidTime,
                                  tmpstr.c_str() );
    }
  }

  traceResourceModel = ResourceModel<>( header );
  traceProcessModel = ProcessModel<>( header, traceResourceModel.isReady() );

  // Communicators
  PRV_UINT32 numberComm = 0;
  if ( !header.eof() )
  {
    std::getline( header, tmpstr );
    if ( tmpstr != "" )
    {
      std::istringstream streamComm( tmpstr );

      if ( !( streamComm >> numberComm ) )
      {
        throw TraceHeaderException( TTraceHeaderErrorCode::invalidCommNumber,
                                    tmpstr.c_str() );
      }
    }
  }

  for ( PRV_UINT32 count = 0; count < numberComm; count++ )
  {
    traceStream->getline( tmpstr );
    if ( tmpstr[0] != 'C' && tmpstr[0] != 'c' && tmpstr[0] != 'I' && tmpstr[0] != 'i' )
    {
      throw TraceHeaderException( TTraceHeaderErrorCode::unknownCommLine,
                                  tmpstr.c_str() );
    }
    communicators.push_back( tmpstr );
  }
}

