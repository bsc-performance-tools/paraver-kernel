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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <zlib.h>

//#include "filters_wait_window.h"
#include "ktracefilter.h"
#include "kprogresscontroller.h"
#include "tracestream.h" // for GZIP_COMPRESSION_RATIO


#ifdef WIN32
#define atoll _atoi64
#endif

#include <iostream>
#include <sstream>

KTraceFilter::KTraceFilter( char *trace_in,
                            char *trace_out,
                            TraceOptions *options,
                            const std::map< TTypeValuePair, TTypeValuePair >& whichTranslationTable,
                            ProgressController *progress )
{
  is_zip_filter = false;

  exec_options = new KTraceOptions( (KTraceOptions *)options );
  translationTable = whichTranslationTable;
  execute( trace_in, trace_out, progress );
}


KTraceFilter::~KTraceFilter()
{
  delete exec_options;
}


/* Function for parsing program parameters */
void KTraceFilter::read_params()
{
  if ( exec_options->filter_states )
  {
    show_states = true;
    all_states = exec_options->all_states;

    if ( exec_options->min_state_time != 0 )
      min_state_time = exec_options->min_state_time;
  }

  if ( exec_options->filter_comms )
  {
    show_comms = true;

    if ( exec_options->min_comm_size != 0 )
      min_comm_size = exec_options->min_comm_size;
  }

  if ( exec_options->filter_events )
  {
    show_events = true;

    if ( exec_options->filter_last_type == 0 )
      filter_all_types = true;
  }

  if ( exec_options->filter_by_call_time )
    filter_by_call_time = true;
  else
    filter_by_call_time = false;
}


/* For processing the Paraver header */
void KTraceFilter::filter_process_header( char *header )
{
  int num_comms;
  char *word;

  /* Obtaining the total trace time */
  word = strtok( header, ":" );
  word = strtok( NULL,   ":" );
  word = strtok( NULL,   ":" );

  word = strtok( NULL,   "\n" );
  word = strrchr( word,  ','  );

  if ( word != NULL )
  {
    /* Obtaining th number of communicators */
    strcpy( line, word + 1 );
    if ( strchr( line, ')' ) != NULL )
      return;
    num_comms = atoi( line );

    while ( num_comms > 0 )
    {
      if ( !is_zip_filter )
        fgets( header, MAX_HEADER_SIZE, infile );
      else
        gzgets( gzInfile, header, MAX_HEADER_SIZE );

      fprintf( outfile, "%s", header );
      num_comms--;
    }
  }
}


/* 0:  Type not allowed */
/* 1:  Type allowed, whitout min_time */
/* 2:  Type allowed, with min_time */
int KTraceFilter::filter_allowed_type(  TApplOrder appl,
                                        TTaskOrder task,
                                        TThreadOrder thread,
                                        TRecordTime time,
                                        TEventType type,
                                        TEventValue value )
{
  int i, j, type_allowed;

  /* First, searching if this type and value are allowed */
  type_allowed = 0;
  for ( i = 0; i < exec_options->filter_last_type; i++ )
  {
    if ( exec_options->filter_types[i].max_type != 0 )
    {
      if ( type >= exec_options->filter_types[i].type && type <= exec_options->filter_types[i].max_type )
      {
        if ( exec_options->discard_given_types )
          type_allowed = 0;
        else
          type_allowed = 1;

        break;
      }
    }


    if ( exec_options->filter_types[i].type == type )
    {
      if ( exec_options->filter_types[i].last_value == 0 )
      {
        if ( exec_options->discard_given_types )
          type_allowed = 0;
        else
          type_allowed = 1;

        break;
      }

      for ( j = 0; j < exec_options->filter_types[i].last_value; j++ )
      {
        if ( exec_options->filter_types[i].value[j] == value )
        {
          if ( exec_options->discard_given_types )
            type_allowed = 0;
          else
            type_allowed = 1;

          break;
        }
      }

      if( j < exec_options->filter_types[i].last_value )
        break;
    }
  }

  if ( i == exec_options->filter_last_type && exec_options->discard_given_types )
  {
    type_allowed = 1;
  }

#if 0
  /* second, we look if event have a min_time property */
  if ( !exec_options->discard_given_types &&
       type_allowed && exec_options->filter_types[i].min_call_time > 0 )
  {
    /* Event d'entrada */
    if ( value > 0 )
      type_allowed = 2;
    else
    {
      /* Event de sortida, cal mirar si supera el temps minim */
      if ( thread_call_info[appl][task][thread] == NULL )
        type_allowed = 0;
      else
      {
        if ( time - thread_call_info[appl][task][thread]->event_time >= exec_options->filter_types[i].min_call_time )
          type_allowed = 2;
        else
        {
          type_allowed = 0;
          thread_call_info[appl][task][thread] = NULL;
        }
      }
    }
  }
#endif

  return type_allowed;
}


void KTraceFilter::ini_progress_bar( char *file_name, ProgressController *progress )
{
  struct stat file_info;

  if ( stat( file_name, &file_info ) < 0 )
  {
    perror( "Error calling stat64" );
    exit( 1 );
  }

  total_trace_size = file_info.st_size;

  if ( total_trace_size < 500000000 )
    total_iters = 10000;
  else
    total_iters = 100000;

  current_read_size = 0;

  if( progress != NULL)
    progress->setEndLimit( total_trace_size );
}


void KTraceFilter::show_progress_bar( ProgressController *progress )
{
//  double current_showed, i, j;
#if defined(__FreeBSD__) || defined(__APPLE__)
  if ( !is_zip_filter )
    current_read_size = ( unsigned long long )ftello( infile );
  else
    current_read_size = ( unsigned long )gztell( gzInfile );
#elif defined(WIN32)
  if ( !is_zip_filter )
    current_read_size = ( unsigned long long )_ftelli64( infile );
  else
    current_read_size = ( unsigned long )gztell( gzInfile );
#else
  if ( !is_zip_filter )
    current_read_size = ( unsigned long long )ftello64( infile );
  else
    current_read_size = ( unsigned long )gztell( gzInfile );
#endif

/*  i = ( double )( current_read_size );
  j = ( double )( total_trace_size );

  current_showed = i / j;
*/
  if ( is_zip_filter )
    current_read_size = current_read_size / TraceStream::GZIP_COMPRESSION_RATIO;

  if( progress != NULL)
    progress->setCurrentProgress( current_read_size );
}


void KTraceFilter::load_pcf( char *pcf_name )
{
  FILE *infile;
  unsigned int state_id;
  // char state_name[128];
  char *state_name;

  state_name = (char *) malloc( sizeof(char) * MAX_STATE_NAME_SIZE );

  /* Open the files.  If NULL is returned there was an error */
  if ( ( infile = fopen( pcf_name, "r" ) ) == NULL )
  {
    printf( "Can't open file %s. Keeping all the states of the trace\n", pcf_name );
    all_states = 1;
    return;
  }

  /* Loading of state ids */
  while ( fgets( line, sizeof( line ), infile ) != NULL )
  {
    if ( !strcmp( line, "STATES\n" ) )
    {
      int i;
      while ( fgets( line, sizeof( line ), infile ) != NULL )
      {
        if ( !strcmp( line, "\n" ) ) return;

        sscanf( line, "%d %[^\n]", &state_id, state_name );

        i = 0;
        while ( i < 20 && exec_options->state_names[i] != NULL )
        {
          if ( strstr( exec_options->state_names[i], state_name ) != NULL )
          {
            states_info.ids[states_info.last_id] = state_id;
            states_info.last_id++;
            break;
          }
          i++;
        }
      }
    }
  }

  free( state_name );
}


void KTraceFilter::dump_buffer()
{
  struct buffer_elem *elem, *elem_aux;

  elem = buffer_first;
  while ( elem != NULL && elem->dump )
  {
    if ( elem->dump )
      fputs( elem->record, outfile );

    free( elem->record );
    elem_aux = elem;
    elem = elem->next;
    free( elem_aux );
  }

  buffer_first = elem;
  if ( buffer_first == NULL )
    buffer_last = NULL;
}


void KTraceFilter::execute( char *trace_in, char *trace_out,ProgressController *progress )
{
  bool end_line, print_record;
  int i, j, k, num_char;

  TRecordType id;
  TCPUOrder cpu;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;
  unsigned long long time_1, time_2;
  TState state;
  TEventType type;
  TEventValue value;
  TCommSize size;
  TCommTag tag;

  char *word, *trace_name, *c, *trace_header;
  char *pcf_file;
  unsigned long num_iters = 0;
  bool end_parsing = false;
  bool dump_event_buffer, call_in;
  struct buffer_elem *new_elem, *elem_aux;

  //event_record = (char *) malloc( sizeof(char) * MAX_LINE_SIZE );

  trace_name   = (char *) malloc( sizeof(char) * MAX_FILENAME_SIZE );
  pcf_file     = (char *) malloc( sizeof(char) * MAX_FILENAME_SIZE );

  /* ini vars. */
  show_states = false;
  show_events = false;
  show_comms = false;
  filter_all_types = false;
  min_state_time = 0;
  min_comm_size = 0;
  states_info.last_id = 0;
  buffer_first = NULL;
  buffer_last = NULL;

  for ( i = 0; i < MAX_APPL; i++ )
    for ( j = 0; j < MAX_TASK; j++ )
      for ( k = 0; k < MAX_THREAD; k++ )
        thread_call_info[i][j][k] = NULL;

  /* Reading of the program arguments */

  read_params();
  strcpy( trace_name, trace_in );

  /* Is the trace zipped ? */
  if ( ( c = strrchr( trace_in, '.' ) ) != NULL )
  {
    /* The names finishes with .gz */
    if ( !strcmp( c, ".gz" ) )
      is_zip_filter = true;
    else
      is_zip_filter = false;
  }

  /* Open the files.  If NULL is returned there was an error */
  if ( !is_zip_filter )
  {
#if defined(__FreeBSD__) || defined(__APPLE__)
    if ( ( infile = fopen( trace_name, "r" ) ) == NULL )
    {
      perror( "ERROR" );
      printf( "Error Opening File %s\n", trace_in );
      exit( 1 );
    }
#elif defined(WIN32)
    if ( fopen_s( &infile, trace_name, "r" ) != 0 )
    {
      perror( "ERROR" );
      printf( "Error Opening File %s\n", trace_in );
      exit( 1 );
    }
#else
    if ( ( infile = fopen64( trace_name, "r" ) ) == NULL )
    {
      perror( "ERROR" );
      printf( "Error Opening File %s\n", trace_in );
      exit( 1 );
    }
#endif
  }
  else
  {
    if ( ( gzInfile = gzopen( trace_name, "rb" ) ) == NULL )
    {
      printf( "Filter: Error opening compressed trace\n" );
      exit( 1 );
    }
  }

#if defined(__FreeBSD__) || defined(__APPLE__)
  if ( ( outfile = fopen( trace_out, "w" ) ) == NULL )
  {
    printf( "Error Opening File %s\n", trace_out );
    exit( 1 );
  }
#elif defined(WIN32)
  if ( fopen_s( &outfile, trace_out, "w" ) != 0 )
  {
    printf( "Error Opening File %s\n", trace_out );
    exit( 1 );
  }
#else
  if ( ( outfile = fopen64( trace_out, "w" ) ) == NULL )
  {
    printf( "Error Opening File %s\n", trace_out );
    exit( 1 );
  }
#endif

  ini_progress_bar( trace_name, progress );

  /* Symbol loading of the .pcf file */
  if ( show_states && !all_states )
  {
    strcpy( pcf_file, trace_name );
    c = strrchr( pcf_file, '.' );
    if (is_zip_filter)
    {
      // twice, for ".prv" and for ".gz" in ".prv.gz"
      *c = '\0';
      c = strrchr( pcf_file, '.' );
    }

    *c = '\0';
    strcat( pcf_file, ".pcf" );
    load_pcf( pcf_file );
  }

  /* Process header */
  trace_header = ( char * ) malloc( sizeof( char ) * MAX_HEADER_SIZE );
  if ( !is_zip_filter ) fgets( trace_header, MAX_HEADER_SIZE, infile );
  else
  {
    gzgets( gzInfile, trace_header, MAX_HEADER_SIZE );
  }

  fprintf( outfile, "%s", trace_header );
  filter_process_header( trace_header );
  free( trace_header );

  if ( progress != NULL )
    end_parsing = progress->getStop();


  /* Processing the trace records */
  //int tmpline =0;
  while ( !end_parsing )
  {
    if ( progress != NULL )
    {
      end_parsing = progress->getStop();
      if ( end_parsing )
        continue;
    }

    /* Read one more record is possible */
    if ( !is_zip_filter )
    {
      if ( fgets( line, sizeof( line ), infile ) == NULL )
      {
        end_parsing = true;
        continue;
      }
    }
    else
    {
      if ( gzgets( gzInfile, line, sizeof( line ) ) == NULL )
      {
        end_parsing = true;
        continue;
      }
    }

    if ( num_iters == total_iters )
    {
      show_progress_bar( progress );
      num_iters = 0;
    }
    else
      num_iters++;

    std::ostringstream event_record;

    // std::cout << tmpline++ << std::endl;

    /* 1: state; 2: event; 3: comm; 4: global comm */
    switch ( line[0] )
    {
      case '1':
        if ( !show_states )
          break;

        sscanf( line, "%*d:%*d:%*d:%*d:%*d:%lld:%lld:%d\n", &time_1, &time_2, &state );

        if ( !all_states )
        {
          for ( i = 0; i < states_info.last_id; i++ )
            if ( states_info.ids[i] == state )
              break;

          if ( i == states_info.last_id )
            break;
        }

        if ( ( !min_state_time ) || ( time_2 - time_1 >= min_state_time ) )
        {
          if ( !filter_by_call_time )
            fputs( line, outfile );
          else
          {
            /* Insert on event buffer */
            if ( ( new_elem = ( struct buffer_elem * )malloc( sizeof( struct buffer_elem ) ) ) == NULL )
            {
              printf( "NO MORE MEMORY!\n" );
              exit( 1 );
            }

            new_elem->record = strdup( line );
            new_elem->dump = true;
            new_elem->appl = appl;
            new_elem->task = task;
            new_elem->thread = thread;
            new_elem->next = NULL;

            if ( buffer_first == NULL )
            {
              buffer_first = new_elem;
              buffer_last = new_elem;
            }

            buffer_last->next = new_elem;
            buffer_last = new_elem;
          }
        }

        break;

      case '2':
        if ( !show_events )
          break;

        if ( filter_all_types )
        {
          fputs( line, outfile );
          break;
        }

        sscanf( line, "%*d:%d:%d:%d:%d:%lld:%*s\n", &cpu, &appl, &task, &thread, &time_1 );

        i = 0;
        num_char = 0;
        while ( 1 )
        {
          if ( line[i] == ':' )
          {
            num_char++;
            if ( num_char == 6 )
            {
              // line[i] = '\0';
              break;
            }
          }
          i++;
        }

        //sprintf( event_record, "2:%d:%d:%d:%d:%lld", cpu, appl, task, thread, time_1 );
        event_record << "2:" << cpu << ":" << appl << ":" << task << ":" << thread << ":" << time_1;

        call_in = false;
        dump_event_buffer = false;

        /* Event type and values */
        end_line = false;
        print_record = false;
        word = strtok( &line[i+1], ":" );
        type = atoll( word );
        word = strtok( NULL, ":" );
        value = atoll( word );

        if ( translationTable.size() > 0 )
        {
          TTypeValuePair p = std::make_pair( type, value );

          std::map< TTypeValuePair, TTypeValuePair >::const_iterator it = translationTable.find(p);
          if ( it != translationTable.end() )
          {
            type  = it->second.first;
            value = it->second.second;
          }
        }

        if ( ( i = filter_allowed_type( appl, task, thread, time_1, type, value ) ) > 0 )
        {
          print_record = true;
          //sprintf( event_record, "%s:%lld:%lld", event_record, type, value );
          event_record << ":" << type << ":" << value;

          if ( i == 2 )
          {
            if ( value > 0 ) call_in = true;
            else dump_event_buffer = true;
          }
        }

        while ( !end_line )
        {
          if ( ( word = strtok( NULL, ":" ) ) != NULL )
          {
            type = atoll( word );
            word = strtok( NULL, ":" );
            value = atoll( word );

            if ( translationTable.size() > 0 )
            {
              TTypeValuePair p = std::make_pair( type, value );

              std::map< TTypeValuePair, TTypeValuePair >::const_iterator it = translationTable.find(p);
              if ( it != translationTable.end() )
              {
                type  = it->second.first;
                value = it->second.second;
              }
            }

            if ( ( i = filter_allowed_type( appl, task, thread, time_1, type, value ) ) > 0 )
            {
              print_record = true;
              //sprintf( event_record, "%s:%lld:%lld", event_record, type, value );
              event_record << ":" << type << ":" << value;

              if ( i == 2 )
              {
                if ( value > 0 )
                  call_in = true;
                else
                  dump_event_buffer = true;
              }
            }
          }
          else
          {
            end_line = true;
            //sprintf( event_record, "%s\n", event_record );
            event_record << std::endl;
          }
        }

        if ( print_record )
        {
          if ( !filter_by_call_time )
          {
            fputs( event_record.str().c_str(), outfile );
          }
          else
          {
            /* Insert on buffer */
            if ( ( new_elem = ( struct buffer_elem * )malloc( sizeof( struct buffer_elem ) ) ) == NULL )
            {
              printf( "NO MORE MEMORY!!\n" );
              exit( 1 );
            }

            new_elem->record = strdup( event_record.str().c_str() );

            if ( call_in )
              new_elem->dump = false;
            else
              new_elem->dump = true;

            new_elem->appl = appl;
            new_elem->task = task;
            new_elem->thread = thread;
            new_elem->event_time = time_1;
            new_elem->next = NULL;

            if ( buffer_first == NULL )
              buffer_first = new_elem;

            if ( buffer_last == NULL )
              buffer_last = new_elem;
            else
            {
              buffer_last->next = new_elem;
              buffer_last = new_elem;
            }

            if ( call_in )
              thread_call_info[appl][task][thread] = new_elem;

            if ( dump_event_buffer )
            {
              if ( thread_call_info[appl][task][thread] != NULL )
                thread_call_info[appl][task][thread]->dump = true;

              dump_buffer();
              thread_call_info[appl][task][thread] = NULL;
            }
          }
        }

        break;

      case '3':
        if ( !show_comms )
          break;

        if ( exec_options->min_comm_size > 0 )
        {
          sscanf( line, "%*d:%*d:%*d:%*d:%*d:%*d:%*d:%*d:%*d:%*d:%*d:%*d:%*d:%lu:%*d\n", &size );

          if ( size < exec_options->min_comm_size )
            break;
        }


        if ( !filter_by_call_time )
          fputs( line, outfile );
        else
        {
          /* Insert on event buffer */
          if ( ( new_elem = ( struct buffer_elem * )malloc( sizeof( struct buffer_elem ) ) ) == NULL )
          {
            printf( "NO MORE MEMORY!!!\n" );
            exit( 1 );
          }

          new_elem->record = strdup( line );
          new_elem->dump = true;
          new_elem->appl = appl;
          new_elem->task = task;
          new_elem->thread = thread;
          new_elem->next = NULL;

          if ( buffer_first == NULL )
          {
            buffer_first = new_elem;
            buffer_last = new_elem;
          }

          buffer_last->next = new_elem;
          buffer_last = new_elem;
        }
        break;


      case '4':
        if ( !filter_by_call_time )
          fputs( line, outfile );
        else
        {
          /* Insert on event buffer */
          if ( ( new_elem = ( struct buffer_elem * )malloc( sizeof( struct buffer_elem ) ) ) == NULL )
          {
            printf( "NO MORE MEMORY!!!!\n" );
            exit( 1 );
          }

          new_elem->record = strdup( line );
          new_elem->dump = true;
          new_elem->appl = appl;
          new_elem->task = task;
          new_elem->thread = thread;
          new_elem->next = NULL;

          if ( buffer_first == NULL )
          {
            buffer_first = new_elem;
            buffer_last = new_elem;
          }

          buffer_last->next = new_elem;
          buffer_last = new_elem;
        }
        break;

      default:
        break;
    }
  }

  /* Dumping the elems left in the buffer */
  if ( filter_by_call_time )
  {
    new_elem = buffer_first;
    while ( new_elem != NULL )
    {
      if ( new_elem->dump )
        fputs( new_elem->record, outfile );

      free( new_elem->record );
      elem_aux = new_elem;
      new_elem = new_elem->next;
      free( elem_aux );
    }
  }

//  ok_filter_wait_window();

  /* Close the files */
  fclose( outfile );
  if ( !is_zip_filter )
    fclose( infile );
  else
    gzclose( gzInfile );

//  free( event_record );
  free( trace_name );
  free( pcf_file );
}
