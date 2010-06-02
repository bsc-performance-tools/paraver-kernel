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

using namespace std;

#include "ktraceoptions.h"
#include "paraverconfig.h"

// void KTraceOptions::parseDoc( char *docname ) is the real contructor
// KTraceOptions::KTraceOptions( const KernelConnection *whichKernel, char *xmldocname ): TraceOptions( whichKernel, xmldocname )
KTraceOptions::KTraceOptions()
{
  init();
}

//KTraceOptions::KTraceOptions( const TraceOptions &whichTraceOptions )
KTraceOptions::KTraceOptions( const TraceOptions &auxwhichTraceOptions ): TraceOptions( auxwhichTraceOptions )
//KTraceOptions::KTraceOptions( const KTraceOptions &whichTraceOptions )
{
  init();
}

KTraceOptions::KTraceOptions( const KTraceOptions &whichTraceOptions )
{
  // Global Default Options
  set_max_trace_size( whichTraceOptions.get_max_trace_size() );

  // Cutter Default Options
  set_by_time( whichTraceOptions.get_by_time() );
  set_min_cutting_time( whichTraceOptions.get_min_cutting_time() );
  set_max_cutting_time( whichTraceOptions.get_max_cutting_time() );
  set_minimum_time_percentage( whichTraceOptions.get_minimum_time_percentage() );
  set_maximum_time_percentage( whichTraceOptions.get_maximum_time_percentage() );
  TTasksList list;
  whichTraceOptions.get_tasks_list( list );
  set_tasks_list( list );
  set_original_time( whichTraceOptions.get_original_time() );
  set_break_states( whichTraceOptions.get_break_states() );
  set_remFirstStates( whichTraceOptions.get_remFirstStates() );
  set_remLastStates( whichTraceOptions.get_remLastStates() );

  // Filter Default Options
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
/*
  set_filter_states( ParaverConfig::getInstance()->getFiltersDiscardStates() );
  set_all_states( ParaverConfig::getInstance()->getFilterAllStates() );
  set_filter_by_call_time( false );
  init_state_names();
  set_min_state_time( 10000000000 );

  set_filter_events( ParaverConfig::getInstance()->getFiltersDiscardEvents() );
  set_discard_given_types( false );
  set_filter_last_type( false );
  init_filter_types();

  set_filter_comms( ParaverConfig::getInstance()->getFiltersDiscardCommunications() );
  set_min_comm_size( ParaverConfig::getInstance()->getFilterCommunicationMinimumSize() )
*/
}


void KTraceOptions::parse_type( xmlDocPtr doc, xmlNodePtr cur, struct allowed_types *types, int *last_type )
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
cout << "FILTER PARAMS!" << endl;

  xmlNodePtr child;
  xmlChar *word;
  char *word_aux;
  unsigned int i;

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
      child = child->next;

      if ( child != NULL )
      {
        word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
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
      for ( i = 0; i < 20; i++ )
        state_names[i] = NULL;

      word = xmlNodeListGetString( doc, child, 1 );
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

          for ( i = 1; i < 20; i++ )
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

    cur = cur->next;
  }
}


void KTraceOptions::parse_cutter_params( xmlDocPtr doc, xmlNodePtr cur )
{
cout << "PARSING CUTTER PARAMS!" << endl;

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


void KTraceOptions::parse_comm_fusion_params( xmlDocPtr doc, xmlNodePtr cur )
{
cout << "COMM_FUSION PARAMS!" << endl;


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
void KTraceOptions::parseDoc( char *docname )
{
cout << "PARSE DOC!" << endl;


  xmlDocPtr doc;
  xmlNodePtr cur;

  /* Ini Data */
  min_state_time = 0;
  min_comm_size = 0;
  filter_last_type = 0;

  original_time = 1;
  tasks_list[0] = '\0';

  filter_by_call_time = 0;

  if ( docname == NULL ) return;

  doc = xmlParseFile( docname );

  if ( doc == NULL ) return;

  cur = xmlDocGetRootElement( doc );

  if ( cur == NULL )
  {
    fprintf( stderr, "empty document\n" );
    xmlFreeDoc( doc );
    return;
  }

  if ( xmlStrcmp( cur->name, ( const xmlChar * ) "config" ) )
  {
    fprintf( stderr, "document of the wrong type, root node != config" );
    xmlFreeDoc( doc );
    return;
  }

  cur = cur->xmlChildrenNode;
  while ( cur != NULL )
  {
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"filter" ) )
      parse_filter_params( doc, cur->xmlChildrenNode );

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"cutter" ) )
      parse_cutter_params( doc, cur->xmlChildrenNode );

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"comm_fusion" ) )
      parse_comm_fusion_params( doc, cur->xmlChildrenNode );

    cur = cur->next;
  }

  xmlFreeDoc( doc );
  return;
}
