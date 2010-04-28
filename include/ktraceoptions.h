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

#ifndef _KPARSERXML_H_
#define _KPARSERXML_H_

#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define MAX_TRACE_HEADER 131072
#define MAX_HEADER_SIZE 131072
#define MAX_APPL 2
#define MAX_TASK 20000
#define MAX_THREAD 10
#define MAX_SELECTED_TASKS 30

/* sofware counters */
/*#define MAX_TYPES 100 */
#define MAX_VALUES 100
#define MAX_THREADS 20000

class KTraceOptions
{
  public:

    struct allowed_types
    {
      unsigned long long type;
      unsigned long long max_type;  /* For range of types */
      unsigned long long min_call_time; /* For filtering calls by time */
      unsigned long long value[20];
      int last_value;
    };

    struct utilities_options
    {
      /* Parameters for filtering */
      char filter_events;
      char filter_states;
      char filter_comms;
      char discard_given_types;
      char filter_by_call_time;

      char *state_names[20];
      char all_states;
      unsigned long long min_state_time;
      int min_comm_size;
      struct allowed_types filter_types[20];
      int filter_last_type;

      /* Parameters for cutting */
      unsigned long long min_cutting_time;
      unsigned long long max_cutting_time;

      // not used?
      // int min_percentage;
      // int max_percentage;

      char tasks_list[256];
      char original_time;

      // not used?
      // char *logfile;

      int max_trace_size;
      int break_states;
      int remFirstStates;
      int remLastStates;

      /* Parameters for comm_fusion */
      char reduce_comms;
      unsigned long long comm_fusion_big_interval;
      unsigned long long comm_fusion_small_interval;

      /* Parameters for software counters */
      int sc_onInterval;
      int sc_global_counters;
      int sc_acumm_counters;
      int sc_summarize_states;
      int sc_only_in_bursts;
      int sc_remove_states;
      unsigned long long sc_interval;
      int sc_frequency;
      char *types;
      char *types_kept;

      /* Parameters for Stats */
      int bursts_plot;
      int comms_plot;
      int events_plot;
    };

    struct utilities_options exec_options;

    KTraceOptions();
    ~KTraceOptions();

    /* Sets for filtering */
    inline void set_filter_events( char filterEvents )
    {
      exec_options.filter_events = filterEvents;
    }

    inline void set_filter_states( char filterStates )
    {
      exec_options.filter_states = filterStates;
    }

    inline void set_filter_comms( char filterComms )
    {
      exec_options.filter_comms = filterComms;
    }

    inline void set_discard_given_types( char discardGivenTypes )
    {
      exec_options.discard_given_types = discardGivenTypes;
    }

    inline void set_filter_by_call_time( char filterByCallTime )
    {
      exec_options.filter_by_call_time = filterByCallTime;
    }

    inline void set_state_names( char *stateNames[20] )
    {
      for ( unsigned int i = 0; i < 20; ++i )
      {
        exec_options.state_names[ i ] = strdup( stateNames[ i ] );
      }
    }

    inline void set_all_states( char allStates )
    {
      exec_options.all_states = allStates;
    }

    inline void set_min_state_time( unsigned long long minStateTime )
    {
      exec_options.min_state_time = minStateTime;
    }

    inline void set_min_comm_size( int minCommSize )
    {
      exec_options.min_comm_size = minCommSize;
    }

    inline void set_filter_types( struct allowed_types filterTypes[20] )
    {
      for ( unsigned int i = 0; i < 20; ++i )
      {
        exec_options.filter_types[ i ] = filterTypes[ i ];
      }
    }

    inline void set_filter_last_type(  int filterLastType )
    {
      exec_options.filter_last_type = filterLastType;
    }

    /* Sets for Cutter */
    inline void set_min_cutting_time( unsigned long long minCutTime )
    {
      exec_options.min_cutting_time = minCutTime;
    }

    inline void set_max_cutting_time( unsigned long long maxCutTime )
    {
      exec_options.min_cutting_time = maxCutTime;
    }

    inline void set_tasks_list( char *tasksList[256] )
    {
      for ( unsigned int i = 0; i < 256; ++i )
      {
        exec_options.tasks_list[ i ] = *tasksList[ i ];
      }
    }

    inline void set_original_time( char originalTime )
    {
      exec_options.original_time = originalTime;
    }

    inline void set_max_trace_size( int traceSize )
    {
      exec_options.max_trace_size = traceSize;
    }

    inline void set_break_states( int breakStates )
    {
      exec_options.break_states = breakStates;
    }

    inline void set_remFirstStates( int remStates )
    {
      exec_options.remFirstStates = remStates;
    }

    inline void set_remLastStates( int remStates )
    {
      exec_options.remLastStates = remStates;
    }

    /* Sets for Sofware Counters */
    inline void set_sc_onInterval( int scOnInterval )
    {
      exec_options.sc_onInterval = scOnInterval;
    }

    inline void set_sc_global_counters( int scGlobalCounters )
    {
      exec_options.sc_global_counters = scGlobalCounters;
    }

    inline void set_sc_acumm_counters( int scAcummCounters )
    {
      exec_options.sc_acumm_counters = scAcummCounters;
    }

    inline void set_sc_summarize_states( int scSummarizeStates )
    {
      exec_options.sc_summarize_states = scSummarizeStates;
    }

    inline void set_sc_only_in_bursts( int scOnlyInBursts )
    {
      exec_options.sc_only_in_bursts = scOnlyInBursts;
    }

    inline void set_sc_remove_states( int scRemoveStates )
    {
      exec_options.sc_remove_states = scRemoveStates;
    }

    inline void set_sc_interval( unsigned long long scInterval )
    {
      exec_options.sc_interval = scInterval;
    }

    inline void set_sc_frequency( int scFrequency )
    {
      exec_options.sc_frequency = scFrequency;
    }

    inline void set_types( char *whichTypes )
    {
      exec_options.types = whichTypes;
    }

    inline void set_types_kept( char *typesKept )
    {
      exec_options.types_kept = typesKept;
    }


    /* Sets for comm_fusion */
    inline void set_reduce_comms( char reduceComms )
    {
      exec_options.reduce_comms = reduceComms;
    }

    inline void set_comm_fusion_big_interval( unsigned long long bigInterval )
    {
      exec_options.comm_fusion_big_interval = bigInterval;
    }

    inline void set_comm_fusion_small_interval( unsigned long long smallInterval )
    {
      exec_options.comm_fusion_small_interval = smallInterval;
    }

    /* Parameters for Stats */
    inline void set_bursts_plot( int burstsPlot )
    {
      exec_options.bursts_plot = burstsPlot;
    }

    inline void set_comms_plot( int commsPlot )
    {
      exec_options.comms_plot = commsPlot;
    }

    inline void set_events_plot( int eventsPlot )
    {
      exec_options.events_plot = eventsPlot;
    }


    void parseDoc( char *docname );

  private:

    void parse_type( xmlDocPtr doc, xmlNodePtr cur, struct allowed_types *types, int *last_type );
    void parse_filter_params( xmlDocPtr doc, xmlNodePtr cur );
    void parse_cutter_params( xmlDocPtr doc, xmlNodePtr cur );
    void parse_comm_fusion_params( xmlDocPtr doc, xmlNodePtr cur );
};

#endif // _KPARSERXML_H_
