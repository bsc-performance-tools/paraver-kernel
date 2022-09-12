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


class KernelConnection;
#include "localkernel.h"

class TraceOptions
{
  public:
    struct allowed_types
    {
      TEventType type;
      TEventType max_type;  /* For range of types */
      unsigned long long min_call_time; /* For filtering calls by time */
      TEventValue value[20];
      int last_value;
    };

    typedef char TTasksList[256];
    typedef char *TStateNames[20];
    typedef struct allowed_types TFilterTypes[20];

    static TraceOptions *create( const KernelConnection *whichKernel );

    static bool isTraceToolsOptionsFile( const std::string& xmlFileName );
    static bool validTraceToolsOptionsFile( const std::string& xmlFileName );

    TraceOptions() {}
    TraceOptions( const KernelConnection *whichKernel ) {}
    virtual ~TraceOptions() {}

    /* Global Settings */
    virtual void set_max_trace_size( int traceSize ) = 0;
    virtual int get_max_trace_size( ) const = 0;

    /* Sets for Cutter */
    virtual void set_by_time( bool whichByTime ) = 0;
    virtual void set_min_cutting_time( unsigned long long minCutTime ) = 0;
    virtual void set_max_cutting_time( unsigned long long maxCutTime ) = 0;
    virtual void set_minimum_time_percentage( unsigned long long whichMinimumTimePercentage ) = 0;
    virtual void set_maximum_time_percentage( unsigned long long whichMaximumTimePercentage ) = 0;
    virtual void set_tasks_list( char *tasksList ) = 0;
    virtual void set_original_time( bool originalTime ) = 0;
    virtual void set_break_states( bool breakStates ) = 0;
    virtual void set_remFirstStates( bool remStates ) = 0;
    virtual void set_remLastStates( bool remStates ) = 0;
    virtual void set_keep_boundary_events( bool keepEvents ) = 0;
    virtual void set_keep_all_events( bool keepAllEvents ) = 0;
    virtual void set_max_cut_time_to_first_finished_appl( bool setOption ) = 0;

    virtual bool get_by_time() const = 0;
    virtual unsigned long long get_min_cutting_time() const = 0;
    virtual unsigned long long get_max_cutting_time() const = 0;
    virtual unsigned long long get_minimum_time_percentage() const = 0;
    virtual unsigned long long get_maximum_time_percentage() const = 0;
    virtual void get_tasks_list( TTasksList &whichTasksList ) const = 0;
    virtual bool get_original_time() const = 0;
    virtual bool get_break_states() const = 0;
    virtual bool get_remFirstStates() const = 0;
    virtual bool get_remLastStates() const = 0;
    virtual bool get_keep_boundary_events() const = 0;
    virtual bool get_keep_all_events() const = 0;
    virtual bool get_max_cut_time_to_first_finished_appl() const = 0;

    /* Sets for filtering */
    virtual void set_filter_events( bool filterEvents ) = 0;
    virtual void set_filter_states( bool filterStates ) = 0;
    virtual void set_filter_comms( bool filterComms ) = 0;
    virtual void set_discard_given_types( bool discardGivenTypes ) = 0;
    virtual void set_filter_by_call_time( bool filterByCallTime ) = 0;
    virtual void set_state_names( TStateNames stateNames ) = 0;
    virtual void set_all_states( bool allStates ) = 0;
    virtual void set_min_state_time( unsigned long long minStateTime ) = 0;
    virtual void set_min_comm_size( int minCommSize ) = 0;
    virtual void set_filter_types( TFilterTypes filterTypes ) = 0;
    virtual void set_filter_last_type( int filterLastType ) = 0;

    virtual bool get_filter_events() const = 0;
    virtual bool get_filter_states() const = 0;
    virtual bool get_filter_comms() const = 0;
    virtual bool get_discard_given_types() const = 0;
    virtual bool get_filter_by_call_time() const = 0;
    virtual void get_state_names( TStateNames &stateNames ) const = 0;
    virtual void get_state_names( std::string &stateList ) const = 0;
    virtual bool get_all_states() const = 0;
    virtual unsigned long long get_min_state_time() const = 0;
    virtual int get_min_comm_size() const = 0;
    virtual void get_filter_types( TFilterTypes &filterTypes ) const = 0;
    virtual int get_filter_last_type() const = 0;

    /* Sets for Software Counters */
    virtual void set_sc_onInterval( bool scOnInterval ) = 0;
    virtual void set_sc_sampling_interval( unsigned long long scInterval ) = 0;
    virtual void set_sc_minimum_burst_time( unsigned long long scInterval ) = 0;
    virtual void set_sc_global_counters( bool scGlobalCounters ) = 0;
    virtual void set_sc_acumm_counters( bool scAcummCounters ) = 0;
    virtual void set_sc_summarize_states( bool scSummarizeStates ) = 0;
    virtual void set_sc_only_in_bursts( bool scOnlyInBursts ) = 0;
    virtual void set_sc_remove_states( bool scRemoveStates ) = 0;
    virtual void set_sc_types( char *whichTypes ) = 0;
    virtual void set_sc_types_kept( char *typesKept ) = 0;

    virtual bool get_sc_onInterval() const = 0;
    virtual unsigned long long get_sc_sampling_interval() const = 0;
    virtual unsigned long long get_sc_minimum_burst_time() const = 0;
    virtual bool get_sc_global_counters() const = 0;
    virtual bool get_sc_acumm_counters() const = 0;
    virtual bool get_sc_summarize_states() const = 0;
    virtual bool get_sc_only_in_bursts() const = 0;
    virtual bool get_sc_remove_states() const = 0;
    virtual char *get_sc_types() const = 0;
    virtual char *get_sc_types_kept() const = 0;

    virtual std::vector< std::string > parseDoc( char *docname ) = 0;

    virtual bool saveXML( std::vector< std::string > &filterOrder, std::string fileName ) = 0;


    virtual TraceOptions *getConcrete()
    {
      return nullptr;
    }

  protected:
  //  KernelConnection *myKernel;
};


class TraceOptionsProxy :public TraceOptions
{
  public:
    virtual ~TraceOptionsProxy();

    /* Global Settings */
    virtual void set_max_trace_size( int traceSize ) override;
    virtual int get_max_trace_size() const override;

    /* Sets for Cutter */
    virtual void set_by_time( bool whichByTime ) override;
    virtual void set_min_cutting_time( unsigned long long minCutTime ) override;
    virtual void set_max_cutting_time( unsigned long long maxCutTime ) override;
    virtual void set_minimum_time_percentage( unsigned long long whichMinimumTimePercentage ) override;
    virtual void set_maximum_time_percentage( unsigned long long whichMaximumTimePercentage ) override;
    virtual void set_tasks_list( char *tasksList ) override;
    virtual void set_original_time( bool originalTime ) override;
    virtual void set_break_states( bool breakStates ) override;
    virtual void set_remFirstStates( bool remStates ) override;
    virtual void set_remLastStates( bool remStates ) override;
    virtual void set_keep_boundary_events( bool keepEvents ) override;
    virtual void set_keep_all_events( bool keepAllEvents ) override;
    virtual void set_max_cut_time_to_first_finished_appl( bool setOption ) override;

    virtual bool get_by_time() const override;
    virtual unsigned long long get_min_cutting_time() const override;
    virtual unsigned long long get_max_cutting_time() const override;
    virtual unsigned long long get_minimum_time_percentage() const override;
    virtual unsigned long long get_maximum_time_percentage() const override;
    virtual void get_tasks_list( TTasksList &whichTasksList ) const override;
    virtual bool get_original_time() const override;
    virtual bool get_break_states() const override;
    virtual bool get_remFirstStates() const override;
    virtual bool get_remLastStates() const override;
    virtual bool get_keep_boundary_events() const override;
    virtual bool get_keep_all_events() const override;
    virtual bool get_max_cut_time_to_first_finished_appl() const override;

    /* Sets for filtering */
    virtual void set_filter_events( bool filterEvents ) override;
    virtual void set_filter_states( bool filterStates ) override;
    virtual void set_filter_comms( bool filterComms ) override;
    virtual void set_discard_given_types( bool discardGivenTypes ) override;
    virtual void set_filter_by_call_time( bool filterByCallTime ) override;
    virtual void set_state_names( TStateNames stateNames ) override;
    virtual void set_all_states( bool allStates ) override;
    virtual void set_min_state_time( unsigned long long minStateTime ) override;
    virtual void set_min_comm_size( int minCommSize ) override;
    virtual void set_filter_types( TFilterTypes filterTypes ) override;
    virtual void set_filter_last_type( int filterLastType ) override;

    virtual bool get_filter_events() const override;
    virtual bool get_filter_states() const override;
    virtual bool get_filter_comms() const override;
    virtual bool get_discard_given_types() const override;
    virtual bool get_filter_by_call_time() const override;
    virtual void get_state_names( TStateNames &stateNames ) const override;
    virtual void get_state_names( std::string &stateList ) const override;
    virtual bool get_all_states() const override;
    virtual unsigned long long get_min_state_time() const override;
    virtual int get_min_comm_size() const override;
    virtual void get_filter_types( TFilterTypes &filterTypes ) const override;
    virtual int get_filter_last_type() const override;

    /* Sets for Software Counters */
    virtual void set_sc_onInterval( bool scOnInterval ) override;
    virtual void set_sc_sampling_interval( unsigned long long scInterval ) override;
    virtual void set_sc_minimum_burst_time( unsigned long long scInterval ) override;
    virtual void set_sc_global_counters( bool scGlobalCounters ) override;
    virtual void set_sc_acumm_counters( bool scAcummCounters ) override;
    virtual void set_sc_summarize_states( bool scSummarizeStates ) override;
    virtual void set_sc_only_in_bursts( bool scOnlyInBursts ) override;
    virtual void set_sc_remove_states( bool scRemoveStates ) override;
    virtual void set_sc_types( char *whichTypes ) override;
    virtual void set_sc_types_kept( char *typesKept ) override;

    virtual bool get_sc_onInterval() const override;
    virtual unsigned long long get_sc_sampling_interval() const override;
    virtual unsigned long long get_sc_minimum_burst_time() const override;
    virtual bool get_sc_global_counters() const override;
    virtual bool get_sc_acumm_counters() const override;
    virtual bool get_sc_summarize_states() const override;
    virtual bool get_sc_only_in_bursts() const override;
    virtual bool get_sc_remove_states() const override;
    virtual char *get_sc_types() const override;
    virtual char *get_sc_types_kept() const override;

    virtual std::vector< std::string > parseDoc( char *docname ) override;
    bool saveXML( std::vector< std::string > &filterOrder, std::string fileName ) override;

    virtual TraceOptions *getConcrete() override;

    static std::vector< std::string > getIDsAvailableTraceTools();
    static std::string getTraceToolName( const std::string& toolID );
    static std::string getTraceToolExtension( const std::string& toolID );

  protected:

  private:
    TraceOptions *myTraceOptions;

    static std::vector< std::string > IDsAvailableTraceTools;

    // TraceOptionsProxy( const KernelConnection *whichKernel, char *xmldocname );
    TraceOptionsProxy( const KernelConnection *whichKernel );

    // friend TraceOptions *TraceOptions::create( KernelConnection *, char * );
    friend TraceOptions *TraceOptions::create( const KernelConnection * );
    friend TraceCutter *LocalKernel::newTraceCutter( TraceOptions *options,
                                         const std::vector< TEventType > &whichTypesWithValuesZero ) const;
    friend TraceFilter *LocalKernel::newTraceFilter( char *trace_in,
                                          char *trace_out,
                                          TraceOptions *options,
                                          const std::map< TTypeValuePair, TTypeValuePair >& whichTranslationTable,
                                          ProgressController *progress ) const;
};



