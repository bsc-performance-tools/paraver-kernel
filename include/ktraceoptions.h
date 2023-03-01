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


#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xmlwriter.h>

#include "traceoptions.h"

constexpr size_t MAX_FILENAME_SIZE = 2048;

#ifdef EXTENDED_OBJECTS_ENABLED
  constexpr size_t MAX_TRACE_HEADER = 1572864;
  constexpr size_t MAX_HEADER_SIZE =  1572864;
#else
  constexpr size_t MAX_TRACE_HEADER = 1048576;
  constexpr size_t MAX_HEADER_SIZE =  1048576;
#endif

constexpr size_t MAX_APPL =     32;
#ifdef EXTENDED_OBJECTS_ENABLED
  constexpr size_t MAX_TASK = 147456;
#else
  constexpr size_t MAX_TASK =  65536;
#endif

constexpr size_t MAX_THREAD = 16;
constexpr size_t MAX_SELECTED_TASKS = 30;
constexpr size_t MAX_LINE_SIZE = MAX_HEADER_SIZE;

/* sofware counters */
constexpr size_t MAX_VALUES = 100;

#ifdef EXTENDED_OBJECTS_ENABLED
  constexpr size_t MAX_THREADS = 147456;
#else
  constexpr size_t MAX_THREADS =  65536;
#endif

constexpr size_t MAXSTATES = 20;
constexpr size_t MAX_STATE_NAME_SIZE = 256;


class KTraceOptions: public TraceOptions
{
  public:
    /* Global parameters */
    int max_trace_size;

    /* Parameters for filtering */
    bool filter_events;
    bool filter_states;
    bool filter_comms;
    bool discard_given_types;
    bool filter_by_call_time;
    char *state_names[MAXSTATES];
    bool all_states;
    unsigned long long min_state_time;
    int min_comm_size; // ---->    should be TCommSize
    TFilterTypes filter_types;
    int filter_last_type; // just a counter

    /* Parameters for cutting */
    bool by_time;
    unsigned long long min_cutting_time;
    unsigned long long max_cutting_time;
    int min_percentage;
    int max_percentage;
    bool original_time;
    bool break_states;
    bool remFirstStates;
    bool remLastStates;
    bool keep_boundary_events;
    bool keep_all_events;
    bool max_cut_time_to_finish_of_first_appl; // PROFET
    char tasks_list[256];

    /* Parameters for software counters */
    bool sc_onInterval;
    bool sc_global_counters;
    bool sc_acumm_counters;
    bool sc_summarize_states;
    bool sc_only_in_bursts;
    bool sc_remove_states;

    // unsigned long long sc_interval; splits in next two
    unsigned long long sc_sampling_interval;
    unsigned long long sc_minimum_burst_time;

    char *types;
    char *types_kept;

  //  KTraceOptions( const KernelConnection *whichKernel, char *xmldocname );
    KTraceOptions();
    KTraceOptions( const TraceOptions &whichTraceOptions );
    KTraceOptions( const KTraceOptions *whichTraceOptions );
    KTraceOptions( const KernelConnection *whichKernel );
    ~KTraceOptions();

    /* Global Settings */
    inline void set_max_trace_size( int whichTraceSize ) override
    {
      max_trace_size = whichTraceSize;
    }

    inline int get_max_trace_size() const override
    {
      return max_trace_size;
    }

    /* Sets for Cutter */
    inline void set_by_time( bool whichByTime ) override
    {
      by_time = whichByTime;
    }

    inline void set_min_cutting_time( unsigned long long whichMinCutTime ) override
    {
      min_cutting_time = whichMinCutTime;
    }

    inline void set_max_cutting_time( unsigned long long whichMaxCutTime ) override
    {
      max_cutting_time = whichMaxCutTime;
    }

    inline void set_minimum_time_percentage( unsigned long long whichMinimumTimePercentage ) override
    {
      min_percentage = whichMinimumTimePercentage;
    }

    inline void set_maximum_time_percentage( unsigned long long whichMaximumTimePercentage ) override
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

    inline void set_tasks_list( char whichTasksList[256] ) override
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

    inline void set_original_time( bool whichOriginalTime ) override
    {
      original_time = whichOriginalTime;
    }

    inline void set_break_states( bool whichBreakStates ) override
    {
      break_states = whichBreakStates;
    }

    inline void set_remFirstStates( bool whichRemStates ) override
    {
      remFirstStates = whichRemStates;
    }

    inline void set_remLastStates( bool whichRemStates ) override
    {
      remLastStates = whichRemStates;
    }

    inline void set_keep_boundary_events( bool whichKeepBoundaryEvents ) override
    {
      keep_boundary_events = whichKeepBoundaryEvents;
    }

    inline void set_keep_all_events( bool whichKeepAllEvents ) override
    {
      keep_all_events = whichKeepAllEvents;
    }

    // PROFET
    inline void set_max_cut_time_to_finish_of_first_appl( bool setOptions ) override
    {
      max_cut_time_to_finish_of_first_appl = setOptions;
    }

    inline bool get_by_time() const override
    {
      return by_time;
    }

    inline unsigned long long get_min_cutting_time() const override
    {
      return min_cutting_time;
    }

    inline unsigned long long get_max_cutting_time() const override
    {
      return max_cutting_time;
    }

    inline unsigned long long get_minimum_time_percentage() const override
    {
      return min_percentage;
    }

    inline unsigned long long get_maximum_time_percentage() const override
    {
      return max_percentage;
    }

    inline void get_tasks_list( TTasksList &whichTasksList ) const override
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

    inline bool get_original_time() const override
    {
      return original_time;
    }

    inline bool get_break_states() const override
    {
      return break_states;
    }

    inline bool get_remFirstStates() const override
    {
      return remFirstStates;
    }

    inline bool get_remLastStates() const override
    {
      return remLastStates;
    }

    inline bool get_keep_boundary_events() const override
    {
      return keep_boundary_events;
    }

    inline bool get_keep_all_events() const override
    {
      return keep_all_events;
    }

    // PROFET
    inline bool get_max_cut_time_to_finish_of_first_appl() const override
    {
      return max_cut_time_to_finish_of_first_appl;
    }

    /* Sets for filtering */
    inline void set_filter_events( bool whichFilterEvents ) override
    {
      filter_events = whichFilterEvents;
    }

    inline void set_filter_states( bool whichFilterStates ) override
    {
      filter_states = whichFilterStates;
    }

    inline void set_filter_comms( bool whichFilterComms ) override
    {
      filter_comms = whichFilterComms;
    }

    inline void set_discard_given_types( bool whichDiscardGivenTypes ) override
    {
      discard_given_types = whichDiscardGivenTypes;
    }

    inline void set_filter_by_call_time( bool whichFilterByCallTime ) override
    {
      filter_by_call_time = whichFilterByCallTime;
    }

    inline void init_state_names()
    {
      for ( unsigned int i = 0; i < MAXSTATES; ++i )
      {
        state_names[ i ] = nullptr;
      }
    }

    inline void set_state_names( TStateNames whichStateNames ) override
    {
      for ( unsigned int i = 0; i < MAXSTATES; ++i )
      {
        if ( whichStateNames[ i ] != nullptr )
          state_names[ i ] = strdup( whichStateNames[ i ] );
        else
          state_names[ i ] = nullptr;
      }
    }

    inline void set_all_states( bool whichAllStates ) override
    {
      all_states = whichAllStates;
    }

    inline void set_min_state_time( unsigned long long whichMinStateTime ) override
    {
      min_state_time = whichMinStateTime;
    }

    inline void set_min_comm_size( int whichMinCommSize ) override
    {
      min_comm_size = whichMinCommSize;
    }

    inline void set_filter_types( TFilterTypes whichFilterTypes ) override
    {
      for ( unsigned int i = 0; i < 20; ++i )
      {
        filter_types[ i ] = whichFilterTypes[ i ];
      }
    }

    inline void set_filter_last_type( int whichFilterLastType ) override
    {
      filter_last_type = whichFilterLastType;
    }

    inline bool get_filter_events() const override
    {
      return filter_events;
    }

    inline bool get_filter_states() const override
    {
      return filter_states;
    }

    inline bool get_filter_comms() const override
    {
      return filter_comms;
    }

    inline bool get_discard_given_types() const override
    {
      return discard_given_types;
    }

    inline bool get_filter_by_call_time() const override
    {
      return filter_by_call_time;
    }

    inline void get_state_names( TStateNames &whichStateNames ) const override
    {
      for( unsigned int i = 0; i < MAXSTATES; ++i )
      {
        if ( state_names[ i ] != nullptr )
          whichStateNames[ i ] = strdup( state_names[ i ] );
        else
          whichStateNames[ i ] = nullptr;
      }
    }

    inline void get_state_names( std::string &stateList ) const override
    {
      for( unsigned int i = 0; i < MAXSTATES; ++i )
      {
        if ( state_names[ i ] != nullptr )
        {
          if ( i > 0 )
          {
            stateList = stateList + std::string( "," );
          }

          stateList =  stateList + std::string( state_names[ i ] );
        }
        else
          break;
      }
    }

    inline bool get_all_states() const override
    {
      return all_states;
    }

    inline unsigned long long get_min_state_time() const override
    {
      return min_state_time;
    }

    inline int get_min_comm_size() const override
    {
      return min_comm_size;
    }

    inline void get_filter_types( TFilterTypes &whichFilterTypes ) const override
    {
      for ( unsigned int i = 0; i < 20; ++i )
      {
        whichFilterTypes[ i ] = filter_types[ i ];
      }
    }

    inline int get_filter_last_type() const override
    {
      return filter_last_type;
    }

    /* Sets for Software Counters */
    inline void set_sc_onInterval( bool whichSCOnInterval ) override
    {
      sc_onInterval = whichSCOnInterval;
    }

    inline void set_sc_sampling_interval( unsigned long long whichSCInterval ) override
    {
      sc_sampling_interval = whichSCInterval;
    }

    inline void set_sc_minimum_burst_time( unsigned long long whichSCMinimumBurstTime ) override
    {
      sc_minimum_burst_time = whichSCMinimumBurstTime;
    }

    inline void set_sc_global_counters( bool whichSCGlobalCounters ) override
    {
      sc_global_counters = whichSCGlobalCounters;
    }

    inline void set_sc_acumm_counters( bool whichSCAcummCounters ) override
    {
      sc_acumm_counters = whichSCAcummCounters;
    }

    inline void set_sc_summarize_states( bool whichSCSummarizeStates ) override
    {
      sc_summarize_states = whichSCSummarizeStates;
    }

    inline void set_sc_only_in_bursts( bool whichSCOnlyInBursts ) override
    {
      sc_only_in_bursts = whichSCOnlyInBursts;
    }

    inline void set_sc_remove_states( bool whichSCRemoveStates ) override
    {
      sc_remove_states = whichSCRemoveStates;
    }

    inline void set_sc_types( char *whichTypes ) override
    {
      types = whichTypes;
    }

    inline void set_sc_types_kept( char *whichTypesKept ) override
    {
      types_kept = whichTypesKept;
    }

    inline bool get_sc_onInterval() const override
    {
      return sc_onInterval;
    }

    inline unsigned long long get_sc_sampling_interval() const override
    {
      return sc_sampling_interval;
    }

    inline unsigned long long get_sc_minimum_burst_time() const override
    {
      return sc_minimum_burst_time;
    }

    inline bool get_sc_global_counters() const override
    {
      return sc_global_counters;
    }

    inline bool get_sc_acumm_counters() const override
    {
      return sc_acumm_counters;
    }

    inline bool get_sc_summarize_states() const override
    {
      return sc_summarize_states;
    }

    inline bool get_sc_only_in_bursts() const override
    {
      return sc_only_in_bursts;
    }

    inline bool get_sc_remove_states() const override
    {
      return sc_remove_states;
    }

    inline char *get_sc_types() const override
    {
      return strdup( types );
    }

    inline char *get_sc_types_kept() const override
    {
      return strdup( types_kept );
    }

    std::vector< std::string > parseDoc( char *docname ) override;
    bool saveXML( std::vector< std::string > &filterOrder, std::string fileName ) override;

  private:
    void init();
    void init_filter_types();
    void parse_type( xmlDocPtr doc,
                     xmlNodePtr cur,
                     struct TraceOptions::allowed_types *types,
                     int &last_type );
    void parse_filter_params( xmlDocPtr doc, xmlNodePtr cur );
    void parse_cutter_params( xmlDocPtr doc, xmlNodePtr cur );
    void parse_software_counters_params( xmlDocPtr doc, xmlNodePtr cur );

    void pushBackUniqueFilterIdentifier( std::string filterID, std::vector< std::string > &order );

    void saveXMLCutter( xmlTextWriterPtr &writer );
    void saveXMLFilter( xmlTextWriterPtr &writer );
    void saveXMLSoftwareCounters( xmlTextWriterPtr &writer );
};


