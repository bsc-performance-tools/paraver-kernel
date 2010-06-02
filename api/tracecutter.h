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

//#include "traceoptions.h"
class TraceOptions;
class KernelConnection;

class TraceCutter
{
  public:
    static TraceCutter *create( KernelConnection *whichKernel,
                                char *traceIn,
                                char *traceOut,
                                TraceOptions *options );

    virtual ~TraceCutter()
    {}

    virtual void set_by_time( bool byTime )
    {}
    virtual void set_min_cutting_time( unsigned long long minCutTime )
    {}
    virtual void set_max_cutting_time( unsigned long long maxCutTime )
    {}
    virtual void set_minimum_time_percentage( unsigned long long minimumPercentage )
    {}
    virtual void set_maximum_time_percentage( unsigned long long maximumPercentage )
    {}
    virtual void set_tasks_list( char *tasksList )
    {}
    virtual void set_original_time( char originalTime )
    {}
    virtual void set_max_trace_size( int traceSize )
    {}
    virtual void set_break_states( int breakStates )
    {}
    virtual void set_remFirstStates( int remStates )
    {}
    virtual void set_remLastStates( int remStates )
    {}
    virtual void execute()
    {}
};

class TraceCutterProxy : public TraceCutter
{
  public:
    virtual ~TraceCutterProxy();

    virtual void set_by_time( bool byTime );
    virtual void set_min_cutting_time( unsigned long long minCutTime );
    virtual void set_max_cutting_time( unsigned long long maxCutTime );
    virtual void set_minimum_time_percentage( unsigned long long minimumPercentage );
    virtual void set_maximum_time_percentage( unsigned long long maximumPercentage );
    virtual void set_tasks_list( char *tasksList );
    virtual void set_original_time( char originalTime );
    virtual void set_max_trace_size( int traceSize );
    virtual void set_break_states( int breakStates );
    virtual void set_remFirstStates( int remStates );
    virtual void set_remLastStates( int remStates );

    virtual void execute();

  private:
    TraceCutter *myTraceCutter;

    TraceCutterProxy( KernelConnection *whichKernel,
                      char *traceIn,
                      char *traceOut,
                      TraceOptions *options );

    friend TraceCutter *TraceCutter::create( KernelConnection *kernelConnection,
                                             char *traceIn,
                                             char *traceOut,
                                             TraceOptions *options );
};


#endif // TRACECUTTER_H_INCLUDED
