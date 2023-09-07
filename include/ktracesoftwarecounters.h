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


#pragma once

#include <fstream>

#include "cubecontainer.h"
#include "ktraceoptions.h"
#include "tracesoftwarecounters.h"
#include "tracestream.h"

class KTraceSoftwareCounters : public TraceSoftwareCounters
{
  public:
    KTraceSoftwareCounters( char *trace_in,
                            char *trace_out,
                            TraceOptions *options,
                            ProgressController *progress );
    virtual ~KTraceSoftwareCounters();
    void execute( char *trace_in, char *trace_out, ProgressController *progress );

  private:

    struct counter
    {
      counter( unsigned long long whichType, unsigned long long whichValue, unsigned long long whichNum, bool whichLast_is_zero )
        : type( whichType ), value( whichValue ), num( whichNum ), last_is_zero( whichLast_is_zero )
      {}

      unsigned long long type;
      unsigned long long value;
      unsigned long long num;
      bool last_is_zero;
    };

    struct ThreadInfo
    {
      int appl;
      int task;
      int thread;
      std::vector<counter> counters;
      unsigned long long last_time_of_sc = 0;
      unsigned long long ini_burst_time = 0;
      unsigned long long end_burst_time = 0;
      unsigned long long total_burst_time = 0; /* To summarize bursts */
    };

    struct type_values
    {
      unsigned long long type;
      bool all_values;
      unsigned long long values[16];
    };

    struct sc_allowed_types
    {
      struct type_values type_values[16];
      int next_free_slot;
    };

    struct sc_kept_types
    {
      unsigned long long type[16];
      int next_free_slot;
    };

    struct LastStateEndTime
    {
      int appl;
      int task;
      int thread;
      unsigned long long end_time;
    };

    std::string line;  /* Buffer for reading trace records */

    /* Execution parameters */
    bool all_types;
    bool global_counters;
    bool acumm_values;
    bool remove_states;
    bool only_in_bursts;
    bool summarize_bursts;
    unsigned long long interval;
    unsigned long long last_time;
    unsigned long long trace_time;
    unsigned long long min_state_time;
    struct sc_allowed_types types;
    bool type_of_counters;
    bool keep_events;

    /* Trace in and trace out */
    TraceStream *infile;
    std::fstream outfile;
    KTraceOptions *exec_options;

    /* Info and counters of the threads */
    using SCThreadInfo = CubeContainer<TApplOrder, TTaskOrder, TThreadOrder, ThreadInfo>;
    SCThreadInfo threadsInfo;

    /* Parameters for showing percentage */
    unsigned long long total_trace_size;
    unsigned long long current_read_size;
    unsigned long total_iters;

    /* Struct needed for the mode SC_BY_STATE */
    using LastStateEndTimeContainer = CubeContainer<TApplOrder, TTaskOrder, TThreadOrder, LastStateEndTime>;
    LastStateEndTimeContainer lastStateEndTime;

    /* Estruct for keeping some types on trace */
    struct sc_kept_types types_to_keep;

    void read_sc_args();
    void parseInHeaderAndDumpOut();
    void write_pcf( char *file_out );
    bool allowed_type( unsigned long long type, unsigned long long value );
    void inc_counter( int appl, int task, int thread,
                      unsigned long long type, unsigned long long value );
    void put_all_counters( void );
    void ini_progress_bar( char *file_name, ProgressController *progress );
    void show_progress_bar( ProgressController *progress );
    void put_counters_on_state_by_thread( int appl, int task, int thread );
    SCThreadInfo::iterator findThreadInfo( int appl, int task, int thread );
    void sc_by_time( ProgressController *progress );
    void insert_in_queue_state( int appl, int task, int thread, unsigned long long time );
    void put_counters_on_state( LastStateEndTimeContainer::iterator itLastState );
    void resumeStateCounters( unsigned long long time );
    void sc_by_states( ProgressController *progress );
};

