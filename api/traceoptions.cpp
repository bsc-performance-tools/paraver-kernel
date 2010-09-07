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

#include "kernelconnection.h"
#include "traceoptions.h"

//TraceOptions *TraceOptions::create( KernelConnection *whichKernel, char *xmldocname )
TraceOptions *TraceOptions::create( KernelConnection *whichKernel )
{
//  return new TraceOptionsProxy( whichKernel, xmldocname );
  return new TraceOptionsProxy( whichKernel );
}

// TraceOptionsProxy::TraceOptionsProxy( const KernelConnection *whichKernel, char *xmldocname )
TraceOptionsProxy::TraceOptionsProxy( const KernelConnection *whichKernel )
{
//  myTraceOptions = whichKernel->newTraceOptions( xmldocname );
  myTraceOptions = whichKernel->newTraceOptions();

  //if ( xmldocname != NULL )
  //  myTraceOptions->parseDoc( xmldocname );
}


TraceOptionsProxy::~TraceOptionsProxy()
{
  delete myTraceOptions;
}

/* GLOBAL SETS & GETS */
void TraceOptionsProxy::set_max_trace_size( int traceSize )
{
  myTraceOptions->set_max_trace_size( traceSize );
}

int TraceOptionsProxy::get_max_trace_size() const
{
  return myTraceOptions->get_max_trace_size();
}

/* Sets for Cutter */
void TraceOptionsProxy::set_by_time( bool whichByTime )
{
  myTraceOptions->set_by_time( whichByTime );
}

void TraceOptionsProxy::set_min_cutting_time( unsigned long long minCutTime )
{
  myTraceOptions->set_min_cutting_time( minCutTime );
}

void TraceOptionsProxy::set_max_cutting_time( unsigned long long maxCutTime )
{
  myTraceOptions->set_min_cutting_time( maxCutTime );
}

void TraceOptionsProxy::set_minimum_time_percentage( unsigned long long whichMinimumTimePercentage )
{
  myTraceOptions->set_minimum_time_percentage( whichMinimumTimePercentage );
}

void TraceOptionsProxy::set_maximum_time_percentage( unsigned long long whichMaximumTimePercentage )
{
  myTraceOptions->set_maximum_time_percentage( whichMaximumTimePercentage );
}

void TraceOptionsProxy::set_tasks_list( TTasksList &tasksList )
{
  myTraceOptions->set_tasks_list( tasksList );
}

void TraceOptionsProxy::set_original_time( char originalTime )
{
  myTraceOptions->set_original_time ( originalTime );
}

void TraceOptionsProxy::set_break_states( int breakStates )
{
  myTraceOptions->set_break_states( breakStates );
}

void TraceOptionsProxy::set_remFirstStates( int remStates )
{
  myTraceOptions->set_remFirstStates ( remStates );
}

void TraceOptionsProxy::set_remLastStates( int remStates )
{
  myTraceOptions->set_remLastStates( remStates );
}

bool TraceOptionsProxy::get_by_time() const
{
  return myTraceOptions->get_by_time();
}

unsigned long long TraceOptionsProxy::get_min_cutting_time() const
{
  return myTraceOptions->get_min_cutting_time();
}

unsigned long long TraceOptionsProxy::get_max_cutting_time() const
{
  return myTraceOptions->get_max_cutting_time();
}

unsigned long long TraceOptionsProxy::get_minimum_time_percentage() const
{
  return myTraceOptions->get_minimum_time_percentage();
}

unsigned long long TraceOptionsProxy::get_maximum_time_percentage() const
{
  return myTraceOptions->get_maximum_time_percentage();
}

void TraceOptionsProxy::get_tasks_list( TTasksList &whichTaskList ) const
{
  myTraceOptions->get_tasks_list( whichTaskList );
}

char TraceOptionsProxy::get_original_time() const
{
  return myTraceOptions->get_original_time();
}

int TraceOptionsProxy::get_break_states() const
{
  return myTraceOptions->get_break_states();
}

int TraceOptionsProxy::get_remFirstStates() const
{
  return myTraceOptions->get_remFirstStates();
}

int TraceOptionsProxy::get_remLastStates() const
{
  return myTraceOptions->get_remLastStates();
}


/* Sets for filtering */
void TraceOptionsProxy::set_filter_events( char filterEvents )
{
  myTraceOptions->set_filter_events( filterEvents );
}

void TraceOptionsProxy::set_filter_states( char filterStates )
{
  myTraceOptions->set_filter_states( filterStates );
}

void TraceOptionsProxy::set_filter_comms( char filterComms )
{
  myTraceOptions->set_filter_comms( filterComms );
}

void TraceOptionsProxy::set_discard_given_types( char discardGivenTypes )
{
  myTraceOptions->set_discard_given_types( discardGivenTypes );
}

void TraceOptionsProxy::set_filter_by_call_time( char filterByCallTime )
{
  myTraceOptions->set_filter_by_call_time( filterByCallTime );
}

void TraceOptionsProxy::set_state_names( char *stateNames[20] )
{
  myTraceOptions->set_state_names( stateNames );
}

void TraceOptionsProxy::set_all_states( char allStates )
{
  myTraceOptions->set_all_states( allStates );
}

void TraceOptionsProxy::set_min_state_time( unsigned long long minStateTime )
{
  myTraceOptions->set_min_state_time( minStateTime );
}

void TraceOptionsProxy::set_min_comm_size( int minCommSize )
{
  myTraceOptions->set_min_comm_size( minCommSize );
}

void TraceOptionsProxy::set_filter_types( struct allowed_types filterTypes[20] )
{
  myTraceOptions->set_filter_types( filterTypes );
}

void TraceOptionsProxy::set_filter_last_type(  int filterLastType )
{
  myTraceOptions->set_filter_last_type( filterLastType );
}

char TraceOptionsProxy::get_filter_events() const
{
  return myTraceOptions->get_filter_events();
}

char TraceOptionsProxy::get_filter_states() const
{
  return myTraceOptions->get_filter_states();
}

char TraceOptionsProxy::get_filter_comms() const
{
  return myTraceOptions->get_filter_comms();
}

char TraceOptionsProxy::get_discard_given_types() const
{
  return myTraceOptions->get_discard_given_types();
}

char TraceOptionsProxy::get_filter_by_call_time() const
{
  return myTraceOptions->get_filter_by_call_time();
}

void TraceOptionsProxy::get_state_names( TStateNames &stateNames ) const
{
  return myTraceOptions->get_state_names( stateNames );
}

char TraceOptionsProxy::get_all_states() const
{
  return myTraceOptions->get_all_states();
}

unsigned long long TraceOptionsProxy::get_min_state_time() const
{
  return myTraceOptions->get_min_state_time();
}

int TraceOptionsProxy::get_min_comm_size() const
{
  return myTraceOptions->get_min_comm_size();
}

void TraceOptionsProxy::get_filter_types( TFilterTypes &filterTypes ) const
{
  return myTraceOptions->get_filter_types( filterTypes );
}

int TraceOptionsProxy::get_filter_last_type() const
{
  return myTraceOptions->get_filter_last_type();
}


/* Sets for Software Counters */
void TraceOptionsProxy::set_sc_onInterval( int scOnInterval )
{
  myTraceOptions->set_sc_onInterval( scOnInterval );
}

void TraceOptionsProxy::set_sc_sampling_interval( unsigned long long scInterval )
{
  myTraceOptions->set_sc_sampling_interval( scInterval );
}

void TraceOptionsProxy::set_sc_minimum_burst_time( unsigned long long scInterval )
{
  myTraceOptions->set_sc_minimum_burst_time( scInterval );
}

void TraceOptionsProxy::set_sc_global_counters( int scGlobalCounters )
{
  myTraceOptions->set_sc_global_counters( scGlobalCounters );
}

void TraceOptionsProxy::set_sc_acumm_counters( int scAcummCounters )
{
  myTraceOptions->set_sc_acumm_counters( scAcummCounters );
}

void TraceOptionsProxy::set_sc_summarize_states( int scSummarizeStates )
{
  myTraceOptions->set_sc_summarize_states( scSummarizeStates );
}

void TraceOptionsProxy::set_sc_only_in_bursts( int scOnlyInBursts )
{
  myTraceOptions->set_sc_only_in_bursts( scOnlyInBursts );
}

void TraceOptionsProxy::set_sc_remove_states( int scRemoveStates )
{
  myTraceOptions->set_sc_remove_states( scRemoveStates );
}

void TraceOptionsProxy::set_sc_frequency( int scFrequency )
{
  myTraceOptions->set_sc_frequency( scFrequency );
}

void TraceOptionsProxy::set_sc_types( char *whichTypes )
{
  myTraceOptions->set_sc_types( whichTypes );
}

void TraceOptionsProxy::set_sc_types_kept( char *typesKept )
{
  myTraceOptions->set_sc_types_kept( typesKept );
}


int TraceOptionsProxy::get_sc_onInterval() const
{
  return myTraceOptions->get_sc_onInterval();
}

unsigned long long TraceOptionsProxy::get_sc_sampling_interval() const
{
  return myTraceOptions->get_sc_sampling_interval();
}

unsigned long long TraceOptionsProxy::get_sc_minimum_burst_time() const
{
  return myTraceOptions->get_sc_minimum_burst_time();
}

int TraceOptionsProxy::get_sc_global_counters() const
{
  return myTraceOptions->get_sc_global_counters();
}

int TraceOptionsProxy::get_sc_acumm_counters() const
{
  return myTraceOptions->get_sc_acumm_counters();
}

int TraceOptionsProxy::get_sc_summarize_states() const
{
  return myTraceOptions->get_sc_summarize_states();
}

int TraceOptionsProxy::get_sc_only_in_bursts() const
{
  return myTraceOptions->get_sc_only_in_bursts();
}

int TraceOptionsProxy::get_sc_remove_states() const
{
  return myTraceOptions->get_sc_remove_states();
}

/*
void TraceOptionsProxy::set_sc_frequency()
{
  myTraceOptions->set_sc_frequency( scFrequency );
}
*/
char *TraceOptionsProxy::get_sc_types() const
{
  return myTraceOptions->get_sc_types();
}

char *TraceOptionsProxy::get_sc_types_kept() const
{
  return myTraceOptions->get_sc_types_kept();
}


/* Sets for comm_fusion */
void TraceOptionsProxy::set_reduce_comms( char reduceComms )
{
  myTraceOptions->set_reduce_comms( reduceComms );
}

void TraceOptionsProxy::set_comm_fusion_big_interval( unsigned long long bigInterval )
{
  myTraceOptions->set_comm_fusion_big_interval( bigInterval );
}

void TraceOptionsProxy::set_comm_fusion_small_interval( unsigned long long smallInterval )
{
  myTraceOptions->set_comm_fusion_small_interval( smallInterval );
}

/* Parameters for Stats */
void TraceOptionsProxy::set_bursts_plot( int burstsPlot )
{
  myTraceOptions->set_bursts_plot( burstsPlot );
}

void TraceOptionsProxy::set_comms_plot( int commsPlot )
{
  myTraceOptions->set_comms_plot( commsPlot );
}

void TraceOptionsProxy::set_events_plot( int eventsPlot )
{
  myTraceOptions->set_events_plot( eventsPlot );
}

vector< int > TraceOptionsProxy::parseDoc( char *docname )
{
  return myTraceOptions->parseDoc( docname );
}

TraceOptions *TraceOptionsProxy::getConcrete()
{
  return myTraceOptions;
}

