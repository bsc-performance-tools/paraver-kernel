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

#include <string.h>
#include <ctype.h>

#include "ktraceoptions.h"

// void KTraceOptions::parseDoc( char *docname ) is the real contructor
KTraceOptions::KTraceOptions()
{
}


KTraceOptions::~KTraceOptions()
{
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
    exec_options.filter_by_call_time = 1;
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

  while ( cur != NULL )
  {
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"types" ) )
    {

      exec_options.filter_events = 1;
      exec_options.discard_given_types = 0;

      /* Get properties */
      word = xmlGetProp( cur, ( const xmlChar * )"use" );
      if ( word != NULL )
      {
        if ( !xmlStrcmp( word, ( const xmlChar * )"discard" ) )
          exec_options.discard_given_types = 1;

        xmlFree( word );
      }

      /* Navigate throug nodes */
      child = cur->xmlChildrenNode;
      child = child->next;

      while ( child != NULL )
      {
        if ( !xmlStrcmp( child->name, ( const xmlChar * )"type" ) )
          parse_type( doc, child, exec_options.filter_types, &( exec_options.filter_last_type ) );

        child = child->next;
      }
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"comms" ) )
    {
      exec_options.filter_comms = 1;
      child = cur->xmlChildrenNode;
      child = child->next;

      if ( child != NULL )
      {
        word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
        exec_options.min_comm_size = atoi( ( char * )word );
        xmlFree( word );
      }
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"states" ) )
    {
      exec_options.filter_states = 1;
      exec_options.all_states = 0;
      child = cur->xmlChildrenNode;

      /* searching which states wants to keep */
      for ( i = 0; i < 20; i++ )
        exec_options.state_names[i] = NULL;

      word = xmlNodeListGetString( doc, child, 1 );
      word_aux = strtok( ( char * )word, "," );

      if ( strstr( word_aux, "All" ) != NULL )
        exec_options.all_states = 1;
      else
      {
        /* Patch in order to allow tag without name */
        for ( i = 0; i < strlen( word_aux ); i++ )
          if ( isalnum( word_aux[i] ) ) break;

        if ( i == strlen( word_aux ) )
        {
          /* No label */
          exec_options.state_names[0] = strdup( "Running" );
        }
        else
        {
          exec_options.state_names[0] = strdup( word_aux );

          for ( i = 1; i < 20; i++ )
          {
            if ( ( word_aux = strtok( NULL, "," ) ) == NULL )
              break;

            if ( !strcmp( word_aux, "All" ) )
              exec_options.all_states = 1;
            else
              exec_options.state_names[i] = strdup( word_aux );
          }
        }
      }

      xmlFree( word );

      /* searching if a min time is required */
      child = child->next;

      if ( child != NULL )
      {
        word = xmlNodeListGetString( doc, child->xmlChildrenNode, 1 );
        exec_options.min_state_time = atoll( ( char * )word );
        xmlFree( word );
      }
    }

    cur = cur->next;
  }
}


void KTraceOptions::parse_cutter_params( xmlDocPtr doc, xmlNodePtr cur )
{
  xmlChar *word;

  while ( cur != NULL )
  {
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"tasks" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      strcpy( exec_options.tasks_list, ( const char * )word );

      xmlFree( word );
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"original_time" ) )
    {
      /* Te mes prioritat els parametres de la GUI */
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"time_percentage" ) )
    {
      /* Te mes prioritat la GUI */
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"time" ) )
    {
      /* Te prioritat la GUI */
    }

    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"max_trace_size" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      exec_options.max_trace_size = atoi( ( char * )word );
      xmlFree( word );
    }

    cur = cur->next;
  }
}


void KTraceOptions::parse_comm_fusion_params( xmlDocPtr doc, xmlNodePtr cur )
{
  xmlChar *word;

  exec_options.reduce_comms = 1;

  while ( cur != NULL )
  {
    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"sample_interval" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      exec_options.comm_fusion_big_interval = atoi( ( char * )word );
      xmlFree( word );
    }


    if ( !xmlStrcmp( cur->name, ( const xmlChar * )"inter_comm_interval" ) )
    {
      word = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
      exec_options.comm_fusion_small_interval = atoi( ( char * )word );
      xmlFree( word );
    }

    cur = cur->next;
  }
}

// The real constructor
void KTraceOptions::parseDoc( char *docname )
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  /* Ini Data */
  exec_options.min_state_time = 0;
  exec_options.min_comm_size = 0;
  exec_options.filter_last_type = 0;

  exec_options.original_time = 1;
  exec_options.tasks_list[0] = '\0';

  exec_options.filter_by_call_time = 0;

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
