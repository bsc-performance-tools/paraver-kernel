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
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

#include "ktraceoptions.h"
#include "paraverconfig.h"

#ifdef WIN32
#define atoll _atoi64
#endif

KTraceOptions::KTraceOptions()
{
  init();
}

KTraceOptions::KTraceOptions( const TraceOptions &auxwhichTraceOptions ): TraceOptions( auxwhichTraceOptions )
{
  init();
}

KTraceOptions::KTraceOptions( const KTraceOptions *whichTraceOptions )
{
  // Global Default Options
  set_max_trace_size( whichTraceOptions->get_max_trace_size() );

  // Cutter Default Options
  set_by_time( whichTraceOptions->get_by_time() );
  set_min_cutting_time( whichTraceOptions->get_min_cutting_time() );
  set_max_cutting_time( whichTraceOptions->get_max_cutting_time() );
  set_minimum_time_percentage( whichTraceOptions->get_minimum_time_percentage() );
  set_maximum_time_percentage( whichTraceOptions->get_maximum_time_percentage() );
  TTasksList list;
  whichTraceOptions->get_tasks_list( list );
  set_tasks_list( list );
  set_original_time( whichTraceOptions->get_original_time() );
  set_break_states( whichTraceOptions->get_break_states() );
  set_remFirstStates( whichTraceOptions->get_remFirstStates() );
  set_remLastStates( whichTraceOptions->get_remLastStates() );

  // Filter Default Options: states
  set_filter_states( whichTraceOptions->get_filter_states() );
  set_all_states( whichTraceOptions->get_all_states() );
  if ( !whichTraceOptions->get_all_states() )
  {
    TStateNames namesList;
    whichTraceOptions->get_state_names( namesList );
    set_state_names( namesList );
  }
  else
    init_state_names();
  set_min_state_time( whichTraceOptions->get_min_state_time() );

  // Filter Default Options: events
  set_filter_events( whichTraceOptions->get_filter_events() );
  TFilterTypes eventTypes;
  whichTraceOptions->get_filter_types( eventTypes );
  set_filter_types( eventTypes );
  set_filter_last_type( whichTraceOptions->get_filter_last_type() );
  set_discard_given_types( whichTraceOptions->get_discard_given_types() );

  // Filter Default Options: comms
  set_filter_comms( whichTraceOptions->get_filter_comms() );
  set_min_comm_size( whichTraceOptions->get_min_comm_size() );

  // Software Counters Default Options
  set_sc_onInterval( whichTraceOptions->get_sc_onInterval() );

  if ( whichTraceOptions->get_sc_onInterval() )
    set_sc_sampling_interval( whichTraceOptions->get_sc_sampling_interval() );
  else
    set_sc_minimum_burst_time( whichTraceOptions->get_sc_minimum_burst_time() );

  set_sc_types( whichTraceOptions->get_sc_types() );

  set_sc_acumm_counters( whichTraceOptions->get_sc_acumm_counters() ); // accumulate or count?
  set_sc_remove_states( whichTraceOptions->get_sc_remove_states() );
  set_sc_summarize_states( whichTraceOptions->get_sc_summarize_states() );
  set_sc_global_counters( whichTraceOptions->get_sc_global_counters() );
  set_sc_only_in_bursts( whichTraceOptions->get_sc_only_in_bursts() );
  //set_sc_frequency( whichTraceOptions->getSoftwareCountersFrequency ); // not used!!!
  set_sc_types_kept( whichTraceOptions->get_sc_types_kept() );
}


KTraceOptions::KTraceOptions( const KernelConnection *whichKernel ): TraceOptions( whichKernel )
{
  init();
}


KTraceOptions::~KTraceOptions()
{}


void KTraceOptions::init_filter_types()
{
  for ( unsigned int i = 0; i < 20; ++i )
  {
    filter_types[ i ].type = 0;
    filter_types[ i ].max_type = 0;
    filter_types[ i ].min_call_time = 0;
    for ( unsigned int j = 0; j < 20; ++j )
      filter_types[ i ].value[j] = 0;
    filter_types[ i ].last_value = 0;
  }
}

void KTraceOptions::init()
{
  // Global Default Options
  set_max_trace_size( ParaverConfig::getInstance()->getFiltersFilterTraceUpToMB() );

  // Cutter Default Options
  set_by_time( ParaverConfig::getInstance()->getCutterByTime() );
  set_min_cutting_time( ParaverConfig::getInstance()->getCutterMinimumTime() );
  set_max_cutting_time( ParaverConfig::getInstance()->getCutterMaximumTime() );
  set_minimum_time_percentage( ParaverConfig::getInstance()->getCutterMinimumTimePercentage() );
  set_maximum_time_percentage( ParaverConfig::getInstance()->getCutterMaximumTimePercentage() );
  init_tasks_list();
  set_original_time( ParaverConfig::getInstance()->getCutterOriginalTime() );
  set_break_states( ParaverConfig::getInstance()->getCutterBreakStates() );
  set_remFirstStates( ParaverConfig::getInstance()->getCutterRemoveFirstStates() );
  set_remLastStates( ParaverConfig::getInstance()->getCutterRemoveLastStates() );

  // Filter Default Options
// problem --> derived fields?; minimum default info?
  set_filter_states( !ParaverConfig::getInstance()->getFilterDiscardStates() );
  set_all_states( false );
  //set_filter_by_call_time( false );
  init_state_names();
  set_min_state_time( 0 ); // Not present in ParaverConfig.

  set_filter_events( !ParaverConfig::getInstance()->getFilterDiscardEvents() );
  set_discard_given_types( false );
  init_filter_types();
  set_filter_last_type( 0 );

  set_filter_comms( !ParaverConfig::getInstance()->getFilterDiscardCommunications() );
  set_min_comm_size( ParaverConfig::getInstance()->getFilterCommunicationsMinimumSize() );

  // Software Counters Default Options
  set_sc_onInterval( ParaverConfig::getInstance()->getSoftwareCountersInvervalsOrStates() );

//  if ( ParaverConfig::getInstance()->getSoftwareCountersInvervalsOrStates() )
    set_sc_sampling_interval( ParaverConfig::getInstance()->getSoftwareCountersSamplingInterval() );
//  else
    set_sc_minimum_burst_time( ParaverConfig::getInstance()->getSoftwareCountersMinimumBurstTime() );

  set_sc_types( (char *)ParaverConfig::getInstance()->getSoftwareCountersTypes().c_str() );

  set_sc_acumm_counters( ParaverConfig::getInstance()->getSoftwareCountersCountEventsOrAcummulateValues() ); // accumulate or count?
  set_sc_remove_states( ParaverConfig::getInstance()->getSoftwareCountersRemoveStates() );
  set_sc_summarize_states( ParaverConfig::getInstance()->getSoftwareCountersSummarizeStates() );
  set_sc_global_counters( ParaverConfig::getInstance()->getSoftwareCountersGlobalCounters() );
  set_sc_only_in_bursts( ParaverConfig::getInstance()->getSoftwareCountersOnlyInBursts() );
  //set_sc_frequency( ParaverConfig::getInstance()->getSoftwareCountersFrequency ); // not used!!!
  set_sc_types_kept( (char *)ParaverConfig::getInstance()->getSoftwareCountersTypesKept().c_str() );
}


void KTraceOptions::parse_type( xmlDocPtr doc, xmlNodePtr cur, struct TraceOptions::allowed_types *types, int *last_type )
{
  xmlChar *word;
  int index;
  char *c;

  /* Get properties */
  word = xmlGetProp( cur, ( const xmlChar * )"min_time" );
  if ( word != NULL )
  {
    filter_by_call_time = 1;
    types[*last_type].min_call_time = atoll( ( char * )word );

    xmlFree( word );
  }
  else
    types[*last_type].min_call_time = 0;

  /* Get values */
  word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );


  if ( ( c = strchr( ( char * )word, '-' ) ) != NULL )
  {
    *c = '\0';
    types[*last_type].type = atoll( ( char * )word );
    types[*last_type].max_type = atoll( ++c );
    ( *last_type )++;

    return;
  }
  else
  {
    types[*last_type].type = atoll( ( char * )word );
    types[*last_type].max_type = 0;
  }
  index = 0;

  cur = cur->xmlChildrenNode;
  while ( cur != NULL )
  {
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"value" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      types[*last_type].value[index] = atoll( ( char * )word );
      index++;
    }

    cur = cur->next;
  }

  types[*last_type].last_value = index;
  ( *last_type )++;
}


void KTraceOptions::parse_filter_params( xmlDocPtr doc, xmlNodePtr cur )
{
  xmlNodePtr child;
  xmlChar *word;
  char *word_aux;
  unsigned int i;

// this should be read from ParaverConfig.
  bool discardStates = ParaverConfig::getInstance()->getFilterDiscardStates();
  bool discardEvents = ParaverConfig::getInstance()->getFilterDiscardEvents();
  bool discardCommunications = ParaverConfig::getInstance()->getFilterDiscardCommunications();

  bool foundDiscardStatesTag = false;
  bool foundDiscardEventsTag = false;
  bool foundDiscardCommunicationsTag = false;

  while ( cur != NULL )
  {
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"types" ) )
    {

      filter_events = 1;
      discard_given_types = 0;

      /* Get properties */
      word = xmlGetProp( cur, ( const xmlChar * )"use" );
      if ( word != NULL )
      {
        if ( !xmlStrcmp( word, ( const xmlChar * )"discard" ) )
          discard_given_types = 1;

        xmlFree( word );
      }

      /* Navigate throug nodes */
      child = cur->xmlChildrenNode;
      child = child->next;

      while ( child != NULL )
      {
        if ( !xmlStrcmp( child->name, ( const xmlChar * )"type" ) )
          parse_type( doc, child, filter_types, &( filter_last_type ) );

        child = child->next;
      }
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"comms" ) )
    {
      filter_comms = 1;
      child = cur->xmlChildrenNode;
      //child = child->next;  ??
      if ( child != NULL )
      {
        //word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 ); ??
        word = xmlNodeListGetString( doc, child, 1 );
        min_comm_size = atoi( ( char * )word );
        xmlFree( word );
      }
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"states" ) )
    {
      filter_states = 1;
      all_states = 0;
      child = cur->xmlChildrenNode;

      /* searching which states wants to keep */
      for ( i = 0; i < MAXSTATES; i++ )
        state_names[i] = NULL;

      word = xmlNodeListGetString( doc, child, 1 );

      bool onlyOneState = ( strstr( (char *)word, "," ) == NULL );

      word_aux = strtok( ( char * )word, "," );

      if ( strstr( word_aux, "All" ) != NULL )
        all_states = 1;
      else
      {
        /* Patch in order to allow tag without name */
        for ( i = 0; i < strlen( word_aux ); i++ )
          if ( isalnum( word_aux[i] ) ) break;

        if ( i == strlen( word_aux ) )
        {
          /* No label */
          state_names[0] = strdup( "Running" );
        }
        else
        {
          state_names[0] = strdup( word_aux );

          if ( !onlyOneState )
          {
            for ( i = 1; i < MAXSTATES; i++ )
            {
              if ( ( word_aux = strtok( NULL, "," ) ) == NULL )
                break;

              if ( !strcmp( word_aux, "All" ) )
                all_states = 1;
              else
                state_names[i] = strdup( word_aux );
            }
          }
        }
      }

      xmlFree( word );

      /* searching if a min time is required */
      child = child->next;

      if ( child != NULL )
      {
        word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
        min_state_time = atoll( ( char * )word );
        xmlFree( word );
      }
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"discard_states" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      discardStates = atoi( ( char * )word );
      xmlFree( word );
      foundDiscardStatesTag = true;
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"discard_events" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      discardEvents = atoi( ( char * )word );
      xmlFree( word );
      foundDiscardEventsTag = true;
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"discard_communications" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      discardCommunications = atoi( ( char * )word );
      xmlFree( word );
      foundDiscardCommunicationsTag = true;
    }

    cur = cur->next;
  }

  // POSTCOND: boolean variable discard{States, Events, Communications} is set,
  // no matter if xml tag is present
  // foundDiscard{States, Events, Communications}Tag is true only if we found "discard" xml tag
  // filter_{states, events, comms} is true if section found

  if ( foundDiscardCommunicationsTag )
    filter_comms = !discardCommunications;
  if ( foundDiscardEventsTag )
    filter_events = !discardEvents;
  if ( foundDiscardStatesTag )
    filter_states = !discardStates;
}


void KTraceOptions::parse_cutter_params( xmlDocPtr doc, xmlNodePtr cur )
{
  xmlChar *word;

  while ( cur != NULL )
  {
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"tasks" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      strcpy( tasks_list, ( const char * )word );

      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"original_time" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      original_time = atoi( ( char * )word );
      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"max_trace_size" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      max_trace_size = atoi( ( char * )word );
      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"by_time" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      by_time = atoi( ( char * )word );
      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"minimum_time" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      min_cutting_time = atoi( ( char * )word );
      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"maximum_time" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      max_cutting_time = atoi( ( char * )word );
      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"minimum_time_percentage" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      min_percentage = atoi( ( char * )word );
      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"maximum_time_percentage" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      max_percentage = atoi( ( char * )word );
      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"break_states" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      break_states = atoi( ( char * )word );
      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"remove_first_states" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      remFirstStates = atoi( ( char * )word );
      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"remove_last_states" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      remLastStates = atoi( ( char * )word );
      xmlFree( word );
    }

    cur = cur->next;
  }
}


void KTraceOptions::parse_software_counters_params( xmlDocPtr doc, xmlNodePtr cur )
{
  xmlNodePtr child;
  xmlChar *word;

/* PARAMETERS TO FILL
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
*/
  while ( cur != NULL )
  {
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"range" ) )
    {
      /* Navigate throug nodes */
      child = cur->xmlChildrenNode;
      child = child->next;

      while ( child != NULL )
      {
        if ( !xmlStrcmp( child->name, ( const xmlChar * )"by_intervals_vs_by_states" ) )
        {
          word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
          sc_onInterval = atoi( ( char * )word );
          xmlFree( word );
        }

        if ( !xmlStrcmp( child->name, ( const xmlChar * )"sampling_interval" ) )
        {
          word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
          sc_sampling_interval = atoll( ( char * )word );
          xmlFree( word );
        }

        if ( !xmlStrcmp( child->name, ( const xmlChar * )"minimum_burst_time" ) )
        {
          word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
          sc_minimum_burst_time = atoll( ( char * )word );
          xmlFree( word );
        }

        if ( !xmlStrcmp( child->name, ( const xmlChar * )"events" ) )
        {
          /* Navigate throug nodes */
          word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
          types = strdup( (char *)word );
          xmlFree( word );
        }

        child = child->next;
      }
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"algorithm" ) )
    {
      /* Navigate throug nodes */
      child = cur->xmlChildrenNode;
      child = child->next;

      while ( child != NULL )
      {
        if ( !xmlStrcmp( child->name, ( const xmlChar * )"count_events_vs_acummulate_values" ) )
        {
          word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
          sc_acumm_counters = atoi( ( char * )word );
          xmlFree( word );
        }

        if ( !xmlStrcmp( child->name, ( const xmlChar * )"remove_states" ) )
        {
          word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
          sc_remove_states = atoi( ( char * )word );
          xmlFree( word );
        }

        if ( !xmlStrcmp( child->name, ( const xmlChar * )"summarize_useful_states" ) )
        {
          word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
          sc_summarize_states = atoi( ( char * )word );
          xmlFree( word );
        }

        if ( !xmlStrcmp( child->name, ( const xmlChar * )"global_counters" ) )
        {
          word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
          sc_global_counters = atoi( ( char * )word );
          xmlFree( word );
        }

        if ( !xmlStrcmp( child->name, ( const xmlChar * )"only_in_burst_counting" ) )
        {
          word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
          sc_only_in_bursts = atoi( ( char * )word );
          xmlFree( word );
        }

        if ( !xmlStrcmp( child->name, ( const xmlChar * )"keep_events" ) )
        {
          word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
          types_kept = strdup ( (char *)word );
          xmlFree( word );
        }

        child = child->next;
      }
    }

    cur = cur->next;
  }
}


void KTraceOptions::parse_comm_fusion_params( xmlDocPtr doc, xmlNodePtr cur )
{
  xmlChar *word;

  reduce_comms = 1;

  while ( cur != NULL )
  {
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"sample_interval" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      comm_fusion_big_interval = atoi( ( char * )word );
      xmlFree( word );
    }


    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"inter_comm_interval" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      comm_fusion_small_interval = atoi( ( char * )word );
      xmlFree( word );
    }

    cur = cur->next;
  }
}

// The real constructor
vector<int> KTraceOptions::parseDoc( char *docname )
{
  vector< int > order;

  xmlDocPtr doc;
  xmlNodePtr cur;

  /* Ini Data */
  min_state_time = 0;
  min_comm_size = 0;
  filter_last_type = 0;

  original_time = 1;
  tasks_list[0] = '\0';

  filter_by_call_time = 0;

  if ( docname == NULL ) return order;

  doc = xmlParseFile( docname );

  if ( doc == NULL ) return order;

  cur = xmlDocGetRootElement( doc );

  if ( cur == NULL )
  {
    fprintf( stderr, "empty document\n" );
    xmlFreeDoc( doc );
    return order;
  }

  if ( xmlStrcmp( cur->name, ( const xmlChar * ) "config" ) )
  {
    fprintf( stderr, "document of the wrong type, root node != config" );
    xmlFreeDoc( doc );
    return order;
  }

  cur = cur->xmlChildrenNode;
  while ( cur != NULL )
  {
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"cutter" ) )
    {
      parse_cutter_params( doc, cur->xmlChildrenNode );
      order.push_back( INC_CHOP_COUNTER );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"filter" ) )
    {
      parse_filter_params( doc, cur->xmlChildrenNode );
      order.push_back( INC_FILTER_COUNTER );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"software_counters" ) )
    {
      parse_comm_fusion_params( doc, cur->xmlChildrenNode );
      order.push_back( INC_SC_COUNTER );
    }

    /*
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"comm_fusion" ) )
      parse_comm_fusion_params( doc, cur->xmlChildrenNode );
    */

    cur = cur->next;
  }

  xmlFreeDoc( doc );

  return order;
}
