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

#ifndef _KTRACEOPTIONS_H_
#define _KTRACEOPTIONS_H_

#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "traceoptions.h"

#define MAX_TRACE_HEADER 131072
#define MAX_HEADER_SIZE 131072
#define MAX_APPL 2
#define MAX_TASK 20000
#define MAX_THREAD 10
#define MAX_SELECTED_TASKS 30
#define MAX_LINE_SIZE 8192
/* sofware counters */
/*#define MAX_TYPES 100 */
#define MAX_VALUES 100
#define MAX_THREADS 20000
#define MAXSTATES 20

// only for cout, delete this 2 lines
#include <iostream>
using namespace std;

class KTraceOptions: public TraceOptions
{
  public:
    /* Global parameters */
    int max_trace_size;

    /* Parameters for filtering */
    char filter_events;
    char filter_states;
    char filter_comms;
    char discard_given_types;
    char filter_by_call_time;
    char *state_names[MAXSTATES];
    char all_states;
    unsigned long long min_state_time;
    int min_comm_size; // ---->    should be TCommSize
    TFilterTypes filter_types;
    int filter_last_type;

    /* Parameters for cutting */
    bool by_time;
    unsigned long long min_cutting_time;
    unsigned long long max_cutting_time;
    int min_percentage;
    int max_percentage;
    char original_time;
    int break_states;
    int remFirstStates;
    int remLastStates;
    char tasks_list[256];

    // not used?
    // char *logfile;

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

  //  KTraceOptions( const KernelConnection *whichKernel, char *xmldocname );
    KTraceOptions();
    KTraceOptions( const TraceOptions &whichTraceOptions );
    KTraceOptions( const KTraceOptions *whichTraceOptions );
    KTraceOptions( const KernelConnection *whichKernel );
    ~KTraceOptions();

    /* Global Settings */
    inline void set_max_trace_size( int whichTraceSize )
    {
      max_trace_size = whichTraceSize;
    }

    inline int get_max_trace_size() const
    {
      return max_trace_size;
    }

    /* Sets for Cutter */
    inline void set_by_time( bool whichByTime )
    {
      by_time = whichByTime;
    }

    inline void set_min_cutting_time( unsigned long long whichMinCutTime )
    {
      min_cutting_time = whichMinCutTime;
    }

    inline void set_max_cutting_time( unsigned long long whichMaxCutTime )
    {
      max_cutting_time = whichMaxCutTime;
    }

    inline void set_minimum_time_percentage( unsigned long long whichMinimumTimePercentage )
    {
      min_percentage = whichMinimumTimePercentage;
    }

    inline void set_maximum_time_percentage( unsigned long long whichMaximumTimePercentage )
    {
      max_percentage = whichMaximumTimePercentage;
    }

    inline void init_tasks_list()
    {
      for ( unsigned int i = 0; i < 256; ++i )
      {
        tasks_list[ i ] = '\0';
      }
    }

    inline void set_tasks_list( char whichTasksList[256] )
    {
      init_tasks_list();

      for ( unsigned int i = 0; i < 256; ++i )
      {
        if ( whichTasksList[ i ] == '\0' )
          break;
        else
          tasks_list[ i ] = whichTasksList[ i ];
      }
    }

    inline void set_original_time( char whichOriginalTime )
    {
      original_time = whichOriginalTime;
    }

    inline void set_break_states( int whichBreakStates )
    {
      break_states = whichBreakStates;
    }

    inline void set_remFirstStates( int whichRemStates )
    {
      remFirstStates = whichRemStates;
    }

    inline void set_remLastStates( int whichRemStates )
    {
      remLastStates = whichRemStates;
    }

    inline bool get_by_time() const
    {
      return by_time;
    }

    inline unsigned long long get_min_cutting_time() const
    {
      return min_cutting_time;
    }

    inline unsigned long long get_max_cutting_time() const
    {
      return max_cutting_time;
    }

    inline unsigned long long get_minimum_time_percentage() const
    {
      return min_percentage;
    }

    inline unsigned long long get_maximum_time_percentage() const
    {
      return max_percentage;
    }

    inline void get_tasks_list( TTasksList &whichTasksList ) const
    {
      for ( unsigned int i = 0; i < 256; ++i )
      {
        whichTasksList[ i ] = '\0';
      }

      for ( unsigned int i = 0; i < 256; ++i )
      {
        if ( tasks_list[ i ] == '\0' )
          break;
        else
          whichTasksList[ i ] = tasks_list[ i ];
      }
    }

    inline char get_original_time() const
    {
      return original_time;
    }

    inline int get_break_states() const
    {
      return break_states;
    }

    inline int get_remFirstStates() const
    {
      return remFirstStates;
    }

    inline int get_remLastStates() const
    {
      return remLastStates;
    }

    /* Sets for filtering */
    inline void set_filter_events( char whichFilterEvents )
    {
      filter_events = whichFilterEvents;
    }

    inline void set_filter_states( char whichFilterStates )
    {
      filter_states = whichFilterStates;
    }

    inline void set_filter_comms( char whichFilterComms )
    {
      filter_comms = whichFilterComms;
    }

    inline void set_discard_given_types( char whichDiscardGivenTypes )
    {
      discard_given_types = whichDiscardGivenTypes;
    }

    inline void set_filter_by_call_time( char whichFilterByCallTime )
    {
      filter_by_call_time = whichFilterByCallTime;
    }

    inline void init_state_names()
    {
      for ( unsigned int i = 0; i < MAXSTATES; ++i )
      {
        state_names[ i ] = NULL;
      }
    }

    inline void set_state_names( TStateNames whichStateNames )
    {
      for ( unsigned int i = 0; i < MAXSTATES; ++i )
      {
        if ( whichStateNames[ i ] != NULL )
          state_names[ i ] = strdup( whichStateNames[ i ] );
        else
          state_names[ i ] = NULL;
      }
    }

    inline void set_all_states( char whichAllStates )
    {
      all_states = whichAllStates;
    }

    inline void set_min_state_time( unsigned long long whichMinStateTime )
    {
      min_state_time = whichMinStateTime;
    }

    inline void set_min_comm_size( int whichMinCommSize )
    {
      min_comm_size = whichMinCommSize;
    }

    inline void set_filter_types( TFilterTypes whichFilterTypes )
    {
      for ( unsigned int i = 0; i < 20; ++i )
      {
        filter_types[ i ] = whichFilterTypes[ i ];
      }
    }

    inline void set_filter_last_type(  int whichFilterLastType )
    {
      filter_last_type = whichFilterLastType;
    }

    inline char get_filter_events() const
    {
      return filter_events;
    }

    inline char get_filter_states() const
    {
      return filter_states;
    }

    inline char get_filter_comms() const
    {
      return filter_comms;
    }

    inline char get_discard_given_types() const
    {
      return discard_given_types;
    }

    inline char get_filter_by_call_time() const
    {
      return filter_by_call_time;
    }

    inline void get_state_names( TStateNames &whichStateNames ) const
    {
      for( unsigned int i = 0; i < MAXSTATES; ++i )
      {
        if ( state_names[ i ] != NULL )
          whichStateNames[ i ] = strdup( state_names[ i ] );
        else
          whichStateNames[ i ] = NULL;
      }
    }

    inline char get_all_states() const
    {
      return all_states;
    }

    inline unsigned long long get_min_state_time() const
    {
      return min_state_time;
    }

    inline int get_min_comm_size() const
    {
      return min_comm_size;
    }

    inline void get_filter_types( TFilterTypes &whichFilterTypes ) const
    {
      for ( unsigned int i = 0; i < 20; ++i )
      {
        whichFilterTypes[ i ] = filter_types[ i ];
      }
    }

    inline int get_filter_last_type() const
    {
      return filter_last_type;
    }


    /* Sets for Software Counters */
    inline void set_sc_onInterval( int whichSCOnInterval )
    {
      sc_onInterval = whichSCOnInterval;
    }

    inline void set_sc_global_counters( int whichSCGlobalCounters )
    {
      sc_global_counters = whichSCGlobalCounters;
    }

    inline void set_sc_acumm_counters( int whichSCAcummCounters )
    {
      sc_acumm_counters = whichSCAcummCounters;
    }

    inline void set_sc_summarize_states( int whichSCSummarizeStates )
    {
      sc_summarize_states = whichSCSummarizeStates;
    }

    inline void set_sc_only_in_bursts( int whichSCOnlyInBursts )
    {
      sc_only_in_bursts = whichSCOnlyInBursts;
    }

    inline void set_sc_remove_states( int whichSCRemoveStates )
    {
      sc_remove_states = whichSCRemoveStates;
    }

    inline void set_sc_interval( unsigned long long whichSCInterval )
    {
      sc_interval = whichSCInterval;
    }

    inline void set_sc_frequency( int whichSCFrequency )
    {
      sc_frequency = whichSCFrequency;
    }

    inline void set_types( char *whichTypes )
    {
      types = whichTypes;
    }

    inline void set_types_kept( char *whichTypesKept )
    {
      types_kept = whichTypesKept;
    }


    /* Sets for comm_fusion */
    inline void set_reduce_comms( char whichReduceComms )
    {
      reduce_comms = whichReduceComms;
    }

    inline void set_comm_fusion_big_interval( unsigned long long whichBigInterval )
    {
      comm_fusion_big_interval = whichBigInterval;
    }

    inline void set_comm_fusion_small_interval( unsigned long long whichSmallInterval )
    {
      comm_fusion_small_interval = whichSmallInterval;
    }

    /* Parameters for Stats */
    inline void set_bursts_plot( int whichBurstsPlot )
    {
      bursts_plot = whichBurstsPlot;
    }

    inline void set_comms_plot( int whichCommsPlot )
    {
      comms_plot = whichCommsPlot;
    }

    inline void set_events_plot( int whichEventsPlot )
    {
      events_plot = whichEventsPlot;
    }


    void parseDoc( char *docname );

  private:
    void init();
    void init_filter_types();
    void parse_type( xmlDocPtr doc, xmlNodePtr cur, struct TraceOptions::allowed_types *types, int *last_type );
    void parse_filter_params( xmlDocPtr doc, xmlNodePtr cur );
    void parse_cutter_params( xmlDocPtr doc, xmlNodePtr cur );
    void parse_comm_fusion_params( xmlDocPtr doc, xmlNodePtr cur );
};

#endif // _KTRACEOPTIONS_H_
