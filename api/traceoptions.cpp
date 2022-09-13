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


#include <fstream>

#include "kernelconnection.h"
#include "paraverkernelexception.h"
#include "traceoptions.h"

// Only for getIDsAvailableTraceTools
#include "tracefilter.h"
#include "tracecutter.h"
#include "tracesoftwarecounters.h"
#include "traceshifter.h"
#include "eventdrivencutter.h"
#include "eventtranslator.h"

using namespace std;


vector< string > TraceOptionsProxy::IDsAvailableTraceTools;

//TraceOptions *TraceOptions::create( KernelConnection *whichKernel, char *xmldocname )
TraceOptions *TraceOptions::create( const KernelConnection *whichKernel )
{
//  return new TraceOptionsProxy( whichKernel, xmldocname );
  return new TraceOptionsProxy( whichKernel );
}

// TODO: Smarter detections welcome!
bool TraceOptions::isTraceToolsOptionsFile( const string& xmlFileName )
{
  string auxName( xmlFileName );
  string suffix( "" );

  if ( auxName.length() > TRACE_TOOL_OPTIONS_SUFFIX.length() )
    suffix = auxName.substr( auxName.length() - TRACE_TOOL_OPTIONS_SUFFIX.length() );

  return ( suffix.compare( TRACE_TOOL_OPTIONS_SUFFIX ) == 0 );
}

bool TraceOptions::validTraceToolsOptionsFile( const string& xmlFileName )
{
  ifstream xmlFile( xmlFileName.c_str() );
  bool result = xmlFile.good();
  xmlFile.close();

  return result;
}

// TraceOptionsProxy::TraceOptionsProxy( const KernelConnection *whichKernel, char *xmldocname )
TraceOptionsProxy::TraceOptionsProxy( const KernelConnection *whichKernel )
{
//  myTraceOptions = whichKernel->newTraceOptions( xmldocname );
  myTraceOptions = whichKernel->newTraceOptions();

  IDsAvailableTraceTools.push_back( TraceCutter::getID() );
  IDsAvailableTraceTools.push_back( TraceFilter::getID() );
  IDsAvailableTraceTools.push_back( TraceSoftwareCounters::getID() );
  IDsAvailableTraceTools.push_back( TraceShifter::getID() );

  //if ( xmldocname != nullptr )
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
  myTraceOptions->set_max_cutting_time( maxCutTime );
}

void TraceOptionsProxy::set_minimum_time_percentage( unsigned long long whichMinimumTimePercentage )
{
  myTraceOptions->set_minimum_time_percentage( whichMinimumTimePercentage );
}

void TraceOptionsProxy::set_maximum_time_percentage( unsigned long long whichMaximumTimePercentage )
{
  myTraceOptions->set_maximum_time_percentage( whichMaximumTimePercentage );
}

void TraceOptionsProxy::set_tasks_list( char *tasksList )
{
  myTraceOptions->set_tasks_list( tasksList );
}

void TraceOptionsProxy::set_original_time( bool originalTime )
{
  myTraceOptions->set_original_time ( originalTime );
}

void TraceOptionsProxy::set_break_states( bool breakStates )
{
  myTraceOptions->set_break_states( breakStates );
}

void TraceOptionsProxy::set_remFirstStates( bool remStates )
{
  myTraceOptions->set_remFirstStates ( remStates );
}

void TraceOptionsProxy::set_remLastStates( bool remStates )
{
  myTraceOptions->set_remLastStates( remStates );
}

void TraceOptionsProxy::set_keep_boundary_events( bool keepEvents )
{
   myTraceOptions->set_keep_boundary_events( keepEvents );
}

void TraceOptionsProxy::set_keep_all_events( bool keepAllEvents )
{
   myTraceOptions->set_keep_all_events( keepAllEvents );
}

void TraceOptionsProxy::set_max_cut_time_to_finish_of_first_appl( bool setOption )
{
  myTraceOptions->set_max_cut_time_to_finish_of_first_appl( setOption );
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

bool TraceOptionsProxy::get_original_time() const
{
  return myTraceOptions->get_original_time();
}

bool TraceOptionsProxy::get_break_states() const
{
  return myTraceOptions->get_break_states();
}

bool TraceOptionsProxy::get_remFirstStates() const
{
  return myTraceOptions->get_remFirstStates();
}

bool TraceOptionsProxy::get_remLastStates() const
{
  return myTraceOptions->get_remLastStates();
}

bool TraceOptionsProxy::get_keep_boundary_events() const
{
  return myTraceOptions->get_keep_boundary_events();
}

bool TraceOptionsProxy::get_keep_all_events() const
{
  return myTraceOptions->get_keep_all_events();
}

// PROFET
bool TraceOptionsProxy::get_max_cut_time_to_finish_of_first_appl() const
{
  return myTraceOptions->get_max_cut_time_to_finish_of_first_appl();
}


/* Sets for filtering */
void TraceOptionsProxy::set_filter_events( bool filterEvents )
{
  myTraceOptions->set_filter_events( filterEvents );
}

void TraceOptionsProxy::set_filter_states( bool filterStates )
{
  myTraceOptions->set_filter_states( filterStates );
}

void TraceOptionsProxy::set_filter_comms( bool filterComms )
{
  myTraceOptions->set_filter_comms( filterComms );
}

void TraceOptionsProxy::set_discard_given_types( bool discardGivenTypes )
{
  myTraceOptions->set_discard_given_types( discardGivenTypes );
}

void TraceOptionsProxy::set_filter_by_call_time( bool filterByCallTime )
{
  myTraceOptions->set_filter_by_call_time( filterByCallTime );
}

void TraceOptionsProxy::set_state_names( char *stateNames[20] )
{
  myTraceOptions->set_state_names( stateNames );
}

void TraceOptionsProxy::set_all_states( bool allStates )
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

//void TraceOptionsProxy::set_filter_types( struct allowed_types filterTypes[20] )
void TraceOptionsProxy::set_filter_types( TFilterTypes filterTypes )
{
  myTraceOptions->set_filter_types( filterTypes );
}

void TraceOptionsProxy::set_filter_last_type( int filterLastType )
{
  myTraceOptions->set_filter_last_type( filterLastType );
}

bool TraceOptionsProxy::get_filter_events() const
{
  return myTraceOptions->get_filter_events();
}

bool TraceOptionsProxy::get_filter_states() const
{
  return myTraceOptions->get_filter_states();
}

bool TraceOptionsProxy::get_filter_comms() const
{
  return myTraceOptions->get_filter_comms();
}

bool TraceOptionsProxy::get_discard_given_types() const
{
  return myTraceOptions->get_discard_given_types();
}

bool TraceOptionsProxy::get_filter_by_call_time() const
{
  return myTraceOptions->get_filter_by_call_time();
}

void TraceOptionsProxy::get_state_names( TStateNames &stateNames ) const
{
  return myTraceOptions->get_state_names( stateNames );
}

void TraceOptionsProxy::get_state_names( string &stateList ) const
{
  return myTraceOptions->get_state_names( stateList );
}

bool TraceOptionsProxy::get_all_states() const
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
void TraceOptionsProxy::set_sc_onInterval( bool scOnInterval )
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

void TraceOptionsProxy::set_sc_global_counters( bool scGlobalCounters )
{
  myTraceOptions->set_sc_global_counters( scGlobalCounters );
}

void TraceOptionsProxy::set_sc_acumm_counters( bool scAcummCounters )
{
  myTraceOptions->set_sc_acumm_counters( scAcummCounters );
}

void TraceOptionsProxy::set_sc_summarize_states( bool scSummarizeStates )
{
  myTraceOptions->set_sc_summarize_states( scSummarizeStates );
}

void TraceOptionsProxy::set_sc_only_in_bursts( bool scOnlyInBursts )
{
  myTraceOptions->set_sc_only_in_bursts( scOnlyInBursts );
}

void TraceOptionsProxy::set_sc_remove_states( bool scRemoveStates )
{
  myTraceOptions->set_sc_remove_states( scRemoveStates );
}

void TraceOptionsProxy::set_sc_types( char *whichTypes )
{
  myTraceOptions->set_sc_types( whichTypes );
}

void TraceOptionsProxy::set_sc_types_kept( char *typesKept )
{
  myTraceOptions->set_sc_types_kept( typesKept );
}


bool TraceOptionsProxy::get_sc_onInterval() const
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

bool TraceOptionsProxy::get_sc_global_counters() const
{
  return myTraceOptions->get_sc_global_counters();
}

bool TraceOptionsProxy::get_sc_acumm_counters() const
{
  return myTraceOptions->get_sc_acumm_counters();
}

bool TraceOptionsProxy::get_sc_summarize_states() const
{
  return myTraceOptions->get_sc_summarize_states();
}

bool TraceOptionsProxy::get_sc_only_in_bursts() const
{
  return myTraceOptions->get_sc_only_in_bursts();
}

bool TraceOptionsProxy::get_sc_remove_states() const
{
  return myTraceOptions->get_sc_remove_states();
}

char *TraceOptionsProxy::get_sc_types() const
{
  return myTraceOptions->get_sc_types();
}

char *TraceOptionsProxy::get_sc_types_kept() const
{
  return myTraceOptions->get_sc_types_kept();
}


vector< string > TraceOptionsProxy::parseDoc( char *docname )
{
  return myTraceOptions->parseDoc( docname );
}

bool TraceOptionsProxy::saveXML( vector< string > &filterOrder, string fileName )
{
  return myTraceOptions->saveXML( filterOrder, fileName );
}

TraceOptions *TraceOptionsProxy::getConcrete()
{
  return myTraceOptions;
}

vector< string > TraceOptionsProxy::getIDsAvailableTraceTools()
{
  return IDsAvailableTraceTools;
}

string TraceOptionsProxy::getTraceToolName( const string& toolID )
{
  string toolStr;

  if ( toolID == TraceCutter::getID() )
  {
    toolStr = TraceCutter::getName();
  }
  else if ( toolID == TraceFilter::getID() )
  {
    toolStr = TraceFilter::getName();
  }
  else if ( toolID == TraceSoftwareCounters::getID() )
  {
    toolStr = TraceSoftwareCounters::getName();
  }
  else if ( toolID == TraceShifter::getID() )
  {
    toolStr = TraceShifter::getName();
  }
  else if ( toolID == EventDrivenCutter::getID() )
  {
    toolStr = EventDrivenCutter::getName();
  }
  else if ( toolID == EventTranslator::getID() )
  {
    toolStr = EventTranslator::getName();
  }
  else
  {
    throw ParaverKernelException( TErrorCode::undefinedToolName );
  }

  return toolStr;
}


string TraceOptionsProxy::getTraceToolExtension( const string& toolID )
{
  string toolStr;

  if ( toolID == TraceCutter::getID() )
  {
    toolStr = TraceCutter::getExtension();
  }
  else if ( toolID == TraceFilter::getID() )
  {
    toolStr = TraceFilter::getExtension();
  }
  else if ( toolID == TraceSoftwareCounters::getID() )
  {
    toolStr = TraceSoftwareCounters::getExtension();
  }
  else if ( toolID == TraceShifter::getID() )
  {
    toolStr = TraceShifter::getExtension();
  }
  else if ( toolID == EventDrivenCutter::getID() )
  {
    toolStr = EventDrivenCutter::getExtension();
  }
  else if ( toolID == EventTranslator::getID() )
  {
    toolStr = EventTranslator::getExtension();
  }
  else
  {
    throw ParaverKernelException( TErrorCode::undefinedToolID );
  }

  return toolStr;
}
