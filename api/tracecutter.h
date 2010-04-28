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

#ifndef TRACECUTTER_H_INCLUDED
#define TRACECUTTER_H_INCLUDED

#include "ktraceoptions.h"

class TraceCutter
{
  public:
    TraceCutter( char *trace_in, char *trace_out, char *xmldocname );
    ~TraceCutter();

    void set_min_cutting_time( unsigned long long minCutTime );
    void set_max_cutting_time( unsigned long long maxCutTime );
    void set_tasks_list( char *tasksList[256] );
    void set_original_time( char originalTime );
    void set_max_trace_size( int traceSize );
    void set_break_states( int breakStates );
    void set_remFirstStates( int remStates );
    void set_remLastStates( int remStates );

    void execute();

  private:
    char *traceIn;
    char *traceOut;
    KTraceOptions options;
};

#endif // TRACECUTTER_H_INCLUDED
