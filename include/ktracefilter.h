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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef _KTRACEFILTER_H_
#define _KTRACEFILTER_H_

#include <map>

#include "ktraceoptions.h"
#include "zlib.h"
#include "tracefilter.h"

class KTraceFilter: public TraceFilter
{
  public:
    KTraceFilter( char *trace_in,
                  char *trace_out,
                  TraceOptions *options,
                  const std::map< TTypeValuePair, TTypeValuePair >& whichTranslationTable,
                  ProgressController *progress );
    virtual ~KTraceFilter();
    virtual void execute( char *trace_in, char *trace_out, ProgressController *progress );

  private:
    /* Buffer for reading trace records */
    char line[MAX_LINE_SIZE];

    /* Trace in and trace out */
    FILE *infile;
    FILE *outfile;
    gzFile gzInfile;

    /* Parameters for showing percentage */
    unsigned long long total_trace_size;
    unsigned long long current_read_size;
    // unsigned long total_cutter_iters;
    unsigned long total_iters;

    /* Vars for saving the HC that will appear on the trace */
//    unsigned long long counters[50];
//    int last_counter;

    KTraceOptions *exec_options;

    /* Execution parameters */
    bool show_states;
    bool show_comms;
    bool show_events;
    bool filter_all_types;
    bool all_states;
    bool filter_by_call_time;
    unsigned long long min_state_time;
    int min_comm_size;
    bool is_zip_filter;

    struct states_info
    {
      int ids[20];
      int last_id;
    };

    struct states_info states_info;

    struct buffer_elem
    {
      char *record;
      bool dump;
      TApplOrder appl;
      TTaskOrder task;
      TThreadOrder thread;
      TRecordTime event_time;
      struct buffer_elem *next;
    };

    struct buffer_elem *buffer_first;
    struct buffer_elem *buffer_last;

    struct buffer_elem *thread_call_info[MAX_APPL][MAX_TASK][MAX_THREAD];

    std::map<TTypeValuePair, TTypeValuePair> translationTable;

    void read_params();
    void filter_process_header( char *header );
    int filter_allowed_type(  TApplOrder appl,
                              TTaskOrder task,
                              TThreadOrder thread,
                              TRecordTime time,
                              TEventType type,
                              TEventValue value );
    void ini_progress_bar( char *file_name, ProgressController *progress );
    void show_progress_bar( ProgressController *progress );
    void load_pcf( char *pcf_name );
    void dump_buffer();
};

#endif // _KTRACEFILTER_H_


