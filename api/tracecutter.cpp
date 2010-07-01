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
//#include "ktraceoptions.h"
#include "traceoptions.h"
//#include "ktracecutter.h"
#include "kernelconnection.h"

TraceCutter *TraceCutter::create(  KernelConnection *whichKernel,
                                   char *traceIn,
                                   char *traceOut,
                                   TraceOptions *options )
{
  return new TraceCutterProxy( whichKernel, traceIn, traceOut, options );
}


TraceCutterProxy::TraceCutterProxy( const KernelConnection *whichKernel,
                                    char *traceIn,
                                    char *traceOut,
                                    TraceOptions *options )
{
  myTraceCutter = whichKernel->newTraceCutter( traceIn, traceOut, options );
}


TraceCutterProxy::~TraceCutterProxy()
{
  delete myTraceCutter;
}

void TraceCutterProxy::set_by_time( bool whichByTime )
{
  myTraceCutter->set_by_time( whichByTime );
}


void TraceCutterProxy::set_min_cutting_time( unsigned long long minCutTime )
{
  myTraceCutter->set_min_cutting_time( minCutTime );
}

void TraceCutterProxy::set_max_cutting_time( unsigned long long maxCutTime )
{
  myTraceCutter->set_min_cutting_time( maxCutTime );
}

void TraceCutterProxy::set_minimum_time_percentage( unsigned long long minimumPercentage )
{
  myTraceCutter->set_minimum_time_percentage( minimumPercentage );
}

void TraceCutterProxy::set_maximum_time_percentage( unsigned long long maximumPercentage )
{
  myTraceCutter->set_maximum_time_percentage( maximumPercentage );
}

void TraceCutterProxy::set_tasks_list( char *tasksList )
{
  myTraceCutter->set_tasks_list( tasksList );
}

void TraceCutterProxy::set_original_time( char originalTime )
{
  myTraceCutter->set_original_time ( originalTime );
}

void TraceCutterProxy::set_max_trace_size( int traceSize )
{
  myTraceCutter->set_max_trace_size( traceSize );
}

void TraceCutterProxy::set_break_states( int breakStates )
{
  myTraceCutter->set_break_states( breakStates );
}

void TraceCutterProxy::set_remFirstStates( int remStates )
{
  myTraceCutter->set_remFirstStates ( remStates );
}

void TraceCutterProxy::set_remLastStates( int remStates )
{
  myTraceCutter->set_remLastStates( remStates );
}

void TraceCutterProxy::execute()
{
//  KTraceCutter cutter( traceIn, traceOut, options->exec_options );
}

