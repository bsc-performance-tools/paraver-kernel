/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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

#include "tracecutter.h"
#include "ktraceoptions.h"
#include "ktracecutter.h"

TraceCutter::TraceCutter( char *trace_in, char *trace_out, char *xmldocname = NULL )
{
  // some extra file checks?
  traceIn = strdup( trace_in );
  traceOut = strdup( trace_out );

  if ( xmldocname != NULL )
    options.parseDoc( xmldocname );
}

TraceCutter::~TraceCutter()
{
  free( traceIn );
  free( traceOut );
}

void TraceCutter::set_min_cutting_time( unsigned long long minCutTime )
{
  options.set_min_cutting_time( minCutTime );
}

void TraceCutter::set_max_cutting_time( unsigned long long maxCutTime )
{
  options.set_min_cutting_time( maxCutTime );
}

void TraceCutter::set_tasks_list( char *tasksList[256] )
{
  options.set_tasks_list( tasksList );
}

void TraceCutter::set_original_time( char originalTime )
{
  options.set_original_time ( originalTime );
}

void TraceCutter::set_max_trace_size( int traceSize )
{
  options.set_max_trace_size( traceSize );
}

void TraceCutter::set_break_states( int breakStates )
{
  options.set_break_states( breakStates );
}

void TraceCutter::set_remFirstStates( int remStates )
{
  options.set_remFirstStates ( remStates );
}

void TraceCutter::set_remLastStates( int remStates )
{
  options.set_remLastStates( remStates );
}

void TraceCutter::execute()
{
  KTraceCutter cutter( traceIn, traceOut, options.exec_options );
}
