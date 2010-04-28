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

#ifndef _KTRACECUTTER_H_
#define _KTRACECUTTER_H_

#include "ktraceoptions.h"
#include "zlib.h"

class KTraceCutter
{
  public:
    KTraceCutter( char *trace_in, char *trace_out, KTraceOptions::utilities_options options );
    ~KTraceCutter();

  private:
    /* Buffer for reading trace records */
    char line[4096];

    /* Trace in and trace out */
    FILE *infile;
    FILE *outfile;
    gzFile gzInfile;

    unsigned int min_perc;
    unsigned int max_perc;
    unsigned int by_time;
    unsigned int old_times;
    unsigned int max_size;
    unsigned int is_zip;
    unsigned int cut_tasks;
    unsigned int break_states;
    unsigned long long time_min;
    unsigned long long time_max;
    unsigned long long total_time;
    unsigned long long first_record_time;
    unsigned long long last_record_time;
    unsigned long long current_size;
    unsigned long long total_size;
    unsigned long long trace_time;
    int num_tasks;
    int usefull_tasks;
    int init_task_counter;
    unsigned int remFirstStates;
    unsigned int remLastStates;
    unsigned int first_time_caught;

    /* Parameters for showing percentage */
    unsigned long long total_trace_size;
    unsigned long long current_readed_size;
    unsigned long total_cutter_iters;

    /* Vars for saving the HC that will appear on the trace */
    unsigned long long counters[50];
    int last_counter;

    /* Struct for the case of MAX_TRACE_SIZE */
    struct thread_info
    {
      int appl;
      int task;
      int thread;
      unsigned long long last_time;
      char finished;
      unsigned long long event_queue[20];
      int first;
      int last;
      struct thread_info *next;
      struct thread_info *previous;
    };

    struct thread_info *tasks[MAX_APPL][MAX_TASK][MAX_THREAD];
    struct thread_info *first;
    struct thread_info *last;


    /* struct for cutting only selected tasks */
    struct selected_tasks
    {
      int min_task_id;
      int max_task_id;
      int range;
    };

    struct selected_tasks wanted_tasks[MAX_SELECTED_TASKS];
    struct KTraceOptions::utilities_options exec_options;

    void read_cutter_params();
    void proces_cutter_header( char *header,
                               char *trace_in_name,
                               char *trace_out_name );
    void adjust_to_final_time();
    void ini_cutter_progress_bar( char *file_name );
    void show_cutter_progress_bar();
    void update_queue( int appl, int task, int thread,
                       unsigned long long type,
                       unsigned long long value );
    void load_counters_of_pcf( char *trace_name );
    void shift_trace_to_zero( char *nameIn, char *nameOut );
    int is_selected_task( int task_id );

    void execute( char *trace_in, char *trace_out );
};

#endif // _KTRACECUTTER_H_


