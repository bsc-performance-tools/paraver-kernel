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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#ifndef WIN32
#include <unistd.h>
#endif

#include "ktracesoftwarecounters.h"
#include "ktraceoptions.h"
#include "kprogresscontroller.h"
//#include "filters_wait_window.h"

#ifdef WIN32
#define atoll _atoi64
#endif

KTraceSoftwareCounters::KTraceSoftwareCounters( char *trace_in,
                                                char *trace_out,
                                                TraceOptions *options,
                                                ProgressController *progress )
{
  min_state_time = 0;
  last_type_mark = -1;
  type_of_counters = false;
  keep_events = false;
  frequency = 1;
  total_iters = 0;
  first_state_elem = NULL;

  exec_options = new KTraceOptions( (KTraceOptions *) options );

  execute( trace_in, trace_out, progress );
}


KTraceSoftwareCounters::~KTraceSoftwareCounters()
{
  delete exec_options;
}


/* Function for parsing program arguments */
void KTraceSoftwareCounters::read_sc_args()
{
  int i, j, k;
  char *words[16], *word_type, *word_values, *word_value;

  all_types = true;
  global_counters = false;
  acumm_values = false;
  remove_states = false;
  only_in_bursts = false;
  summarize_bursts = false;

  type_of_counters = exec_options->sc_onInterval;

  if ( exec_options->sc_onInterval )
  {
    interval = exec_options->sc_sampling_interval;
    last_time = interval;
  }
  else
  {
    min_state_time = exec_options->sc_minimum_burst_time;
    last_time = min_state_time;
  }

  if ( strlen( exec_options->types ) > 0 )
  {
    all_types = false;
    words[0] = strtok( exec_options->types, ";" );

    i = 1;
    while ( ( words[i] = strtok( NULL, ";" ) ) != NULL )
      i++;

    k = 0;
    while ( k < i )
    {
      word_type = strtok( words[k], ":" );
      types.type_values[types.next_free_slot].type = atoll( word_type );
      if ( ( word_values = strtok( NULL, ":" ) ) == NULL )
      {
        types.type_values[types.next_free_slot].all_values = true;
      }
      else
      {
        word_value = strtok( word_values, "," );
        types.type_values[types.next_free_slot].all_values = false;
        types.type_values[types.next_free_slot].values[0] = atoll( word_value );
        types.type_values[types.next_free_slot].values[1] = 0;
        j = 1;
        while ( ( word_value = strtok( NULL, "," ) ) != NULL )
        {
          types.type_values[types.next_free_slot].values[j] = atoll( word_value );
          j++;
          types.type_values[types.next_free_slot].values[j] = 0;
        }
      }
      types.next_free_slot++;
      k++;
    }

    types.next_free_slot++;
    free( exec_options->types );
  }

  if ( strlen( exec_options->types_kept ) > 0 )
  {
    keep_events = true;
    words[0] = strtok( exec_options->types_kept, ";" );
    types_to_keep.type[types_to_keep.next_free_slot] = atoll( words[0] );
    types_to_keep.next_free_slot++;

    while ( ( words[0] = strtok( NULL, ";" ) ) != NULL )
    {
      types_to_keep.type[types_to_keep.next_free_slot] = atoll( words[0] );
      types_to_keep.next_free_slot++;
    }

    free( exec_options->types_kept );
  }

  global_counters = exec_options->sc_global_counters;

  acumm_values = exec_options->sc_acumm_counters;

  remove_states = exec_options->sc_remove_states;

  only_in_bursts = exec_options->sc_only_in_bursts;

  if ( ( summarize_bursts = exec_options->sc_summarize_states ) )
    remove_states = true;
}


/* For processing the Paraver header */
void KTraceSoftwareCounters::proces_header( char *header, FILE *in, FILE *out )
{
  int num_comms;
  char *word;

  fprintf( out, "%s", header );

  /* Obtaining the total trace time */
  word = strtok( header, ":" );
  word = strtok( NULL, ":" );
  word = strtok( NULL, ":" );
  word[strlen( word )-3] = '\0';
  trace_time = atoll( word );

  /* Obtaining the number of communicators */
  word = strtok( NULL, "\n" );
  word = strrchr( word, ',' );

  strcpy( header, word + 1 );

  /* No communicators */
  if ( strchr( header, ')' ) != NULL )
    return;

  num_comms = atoi( header );

  while ( num_comms > 0 )
  {
    fgets( header, MAX_HEADER_SIZE, in );
    fprintf( out, "%s", header );
    num_comms--;
  }
}


/* For copy .pcf and add the counter types and values */
void KTraceSoftwareCounters::write_pcf( char *file_out )
{
  char *c, *trace_out;
  FILE *pcf;

  trace_out = strdup( file_out );

  c = strrchr( trace_out, '.' );
  *c = '\0';

  printf( "\n" );

  sprintf( line, "%s.pcf", trace_out );

  if ( ( pcf = fopen( line, "a" ) ) == NULL )
    return;

  fprintf( pcf, "\n\nEVENT_TYPE\n" );
  fprintf( pcf, "9   15001059    MPI_Waitany_counter\n" );
  fprintf( pcf, "9   15001004    MPI_Irecv_counter\n" );
  fprintf( pcf, "9   15001003    MPI_Isend_counter\n" );
  fprintf( pcf, "9   15001002    MPI_Recv_counter\n" );
  fprintf( pcf, "9   15001001    MPI_Send_counter\n" );
  fprintf( pcf, "9   15001041    MPI_Sendrecv_counter\n" );
  fprintf( pcf, "9   15001006    MPI_Waitall_counter\n" );
  fprintf( pcf, "9   15001005    MPI_Wait_counter\n" );
  fprintf( pcf, "9   15001062    MPI_Iprobe_counter\n" );
  fprintf( pcf, "9   15002010    MPI_Allreduce_counter\n" );
  fprintf( pcf, "9   15002007    MPI_Bcast_counter\n" );
  fprintf( pcf, "9   15002018    MPI_Allgatherv_counter\n" );
  fprintf( pcf, "9   15002013    MPI_Gather_counter\n" );
  fprintf( pcf, "9   15003019    MPI_Comm_rank_counter\n" );
  fprintf( pcf, "9   15003020    MPI_Comm_size_counter\n" );
  fprintf( pcf, "9   15003031    MPI_Init_counter\n" );
  fprintf( pcf, "9   15003032    MPI_Finalize_counter\n" );
  fprintf( pcf, "9      25001    MPI_Point-to-point_global_counter\n" );
  fprintf( pcf, "9      25002    MPI_Collective_comm_global_counter\n" );
  fprintf( pcf, "9      25003    MPI_Other_global_counter\n" );

  if ( summarize_bursts )
    fprintf( pcf, "9          1    Total_burst_time\n" );

  fclose( pcf );
}


/* Return 0: not allowd call type; Return 1: allowed call type */
bool KTraceSoftwareCounters::allowed_type( unsigned long long type, unsigned long long value )
{
  int i, j;

  if ( value == 0 )
    return false;

  /* Searching for that type and value */
  for ( i = 0; i < types.next_free_slot; i++ )
  {
    if ( types.type_values[i].type == type )
    {
      if ( types.type_values[i].all_values )
        return true;


// BUG EN POTENCIA: NO RECORRE TODA LA LISTA DE VALORES!!!
      for ( j = 0; j < 16; j++ )
      {
        if ( types.type_values[i].values[j] == 0 )
          return false;

        if ( types.type_values[i].values[j] == value )
          return true;
      }
    }
  }

  return false;
}

bool KTraceSoftwareCounters::allowed_type_mark( unsigned long long type )
{
  int i;

  /*Searching for that type in the marks array */
  for ( i = 0; i <= last_type_mark; i++ )
  {
    if ( type_marks[i] == type )
      return true;
  }

  return false;
}


/* Increment of a counter for a given type and value */
int KTraceSoftwareCounters::inc_counter( int appl, int task, int thread, unsigned long long type, unsigned long long value )
{
  int i, j;

  /* If the pointer is -1, we haven't registered yet the thread into the */
  /* threads struct. We create the thread in the struct saving:appl, task, */
  /* thread, create a empty counters list, and an empty call stack */
  if ( ( i = thread_pointer[appl][task][thread] ) == -1 )
  {
    threads[next_thread_slot].appl = appl;
    threads[next_thread_slot].task = task;
    threads[next_thread_slot].thread = thread;
    threads[next_thread_slot].next_free_counter = 0;
    threads[next_thread_slot].calls.top = -1;
    i = next_thread_slot;
    next_thread_slot++;
    thread_pointer[appl][task][thread] = i;
  }

  if ( ( all_types && value > 0 ) || allowed_type( type, value ) )
  {
    /* Searching of the specified counter for the given thread */
    for ( j = 0; j < threads[i].next_free_counter; j++ )
    {
      if ( threads[i].counters[j].type == type && ( threads[i].counters[j].value == value || global_counters || acumm_values ) )
      {
        if ( !acumm_values )
          threads[i].counters[j].num++;
        else
          threads[i].counters[j].num += value;
        break;
      }
    }

    /* The counter doesn't exist. Create it */
    if ( j == threads[i].next_free_counter )
    {
      threads[i].counters[j].type = type;
      threads[i].counters[j].value = value;
      threads[i].counters[j].last_is_zero = false;

      if ( !acumm_values )
        threads[i].counters[j].num = 1;
      else
        threads[i].counters[j].num = value;

      threads[i].next_free_counter++;
    }
  }

  return i;
}


/* Function for putting a 0 counter value before a counter > 0 */
void KTraceSoftwareCounters::put_zeros( void )
{
  int i, j;
  unsigned long long type_mask;

  /* We pass over all the threads in the struct */
  for ( i = 0; i < next_thread_slot; i++ )
  {
    /* For every thread, we look over all its counters */
    for ( j = 0; j < threads[i].next_free_counter; j++ )
    {
      /* If we have to put a counter and in the last interval we haven't */
      /* put any counter of the same type and value, it's time to put */
      /* a counter with 0 value on the last sampling period */
      if ( threads[i].counters[j].num >= ( unsigned long long )frequency && threads[i].counters[j].last_is_zero && last_time - interval > 0 )
      {
        if ( !global_counters )
        {
          type_mask = ( ( ( threads[i].counters[j].type ) / 10000 ) + threads[i].counters[j].type % 10000 + 10000 ) * 1000;
          type_mask += threads[i].counters[j].value;
        }
        else
          type_mask = ( threads[i].counters[j].type / 10000 + threads[i].counters[j].type % 10000 + 20000 );

        fprintf( outfile, "2:0:%d:%d:%d:%lld:%lld:0\n", threads[i].appl, threads[i].task, threads[i].thread, last_time - interval, type_mask );
      }
    }
  }
}


/* Flushing of the events buffer */
void KTraceSoftwareCounters::flush_all_events( void )
{
  bool print_record;
  int i, j, thread_id;
  char record[256], record_aux[64];

  struct ParaverEvent *p, *q;

  p = first_Paraver_event;
  while ( p != NULL )
  {
    sprintf( record, "2:%d:%d:%d:%d:%lld", p->cpu, threads[p->thread_id].appl, threads[p->thread_id].task, threads[p->thread_id].thread, p->timestamp );

    print_record = false;
    for ( i = 0; i < 16; i++ )
    {
      if ( p->type[i] == 0 )
        break;

      /* Paraver in event */
      if ( p->value[i] > 0 )
      {
        /* Searching of the counter for that type-value in order to */
        /* see if the counter is greater than frequency and we have */
        /* to put or not to put that event */
        /* After that, we upgrade the call stack for the current */
        /* thread*/
        thread_id = p->thread_id;
        for ( j = 0; j < threads[thread_id].next_free_counter; j++ )
        {
          if ( threads[thread_id].counters[j].type == p->type[i] && ( threads[thread_id].counters[j].value == p->value[i] || global_counters ) )
          {
            if ( threads[thread_id].counters[j].num < ( unsigned long long )frequency )
            {
              sprintf( record_aux, ":%lld:%lld", p->type[i], p->value[i] );
              strcat( record, record_aux );
              print_record = false;
              threads[thread_id].calls.top++;
              threads[thread_id].calls.type[threads[thread_id].calls.top] = p->type[i];
              threads[thread_id].calls.valid[threads[thread_id].calls.top] = true;
            }
            else
            {
              threads[thread_id].calls.top++;
              threads[thread_id].calls.type[threads[thread_id].calls.top] = p->type[i];
              threads[thread_id].calls.valid[threads[thread_id].calls.top] = false;
            }
            break;
          }
        }

        /* Don't exist counter for that type-value, we have to put */
        /* this event on the trace */
        if ( j == threads[thread_id].next_free_counter )
        {
          sprintf( record_aux, ":%lld:%lld", p->type[i], p->value[i] );
          strcat( record, record_aux );
          print_record = true;

          threads[thread_id].calls.top++;
          threads[thread_id].calls.type[threads[thread_id].calls.top] = p->type[i];
          threads[thread_id].calls.valid[threads[thread_id].calls.top] = true;
        }
      }
      else
      {
        /* Paraver out event */
        /* We search in the call stack of the thread to see if we have */
        /* to put the Paraver-out event */
        thread_id = p->thread_id;
        if ( threads[thread_id].calls.type[threads[thread_id].calls.top] == p->type[i] )
        {
          if ( threads[thread_id].calls.valid[threads[thread_id].calls.top] )
          {
            sprintf( record_aux, ":%lld:0", p->type[i] );
            strcat( record, record_aux );
            print_record = true;
          }
          threads[thread_id].calls.top--;
        }
      }
    }

    if ( print_record )
    {
      fprintf( outfile, "%s\n", record );
      print_record = false;
    }

    q = p;
    p = p->next_event;
    free( q );
  }

  first_Paraver_event = NULL;
  last_Paraver_event = NULL;
}


/* Function for putting soft counters in the trace for every specified period */
void KTraceSoftwareCounters::put_all_counters( void )
{
  int i, j;
  unsigned long long type_mask;

  /* We pass over all the threads on the struct */
  for ( i = 0; i < next_thread_slot; i++ )
  {
    /* For every thread, we pass over all its counters */
    for ( j = 0; j < threads[i].next_free_counter; j++ )
    {
      if ( acumm_values )
        type_mask = threads[i].counters[j].type;
      else
      {
        if ( !global_counters )
        {
          type_mask = ( ( ( threads[i].counters[j].type ) / 10000 ) + threads[i].counters[j].type % 10000 + 10000 ) * 1000;
          type_mask += threads[i].counters[j].value;
        }
        else
          type_mask = threads[i].counters[j].type / 10000 + 20000 + threads[i].counters[j].type % 10000;
      }

      fprintf( outfile, "2:0:%d:%d:%d:%lld:%lld:%lld\n", threads[i].appl, threads[i].task, threads[i].thread, last_time, type_mask, threads[i].counters[j].num );

      /* If the counters is 0, we mark that in order to put a 0 counter */
      /* in the period before of a non-zero counter of the same type */
      /*         if(threads[i].counters[j].num == 0)
        threads[i].counters[j].last_is_zero = true;
               else {
        threads[i].counters[j].num=0;
        threads[i].counters[j].last_is_zero = false;
        }*/
    }
  }
}


void KTraceSoftwareCounters::put_counters_by_thread( int appl, int task, int thread, int cpu )
{
  int i, j;
  unsigned long long type_mask;
  struct counter_event *event;

  /* We search the thread on the struct */
  for ( i = 0; i < next_thread_slot; i++ )
  {
    if ( threads[i].appl == appl && threads[i].task == task && threads[i].thread == thread ) break;
  }

  if ( i == next_thread_slot )
    return;

  /* First of all put zeros if needed */
  if ( i == next_thread_slot )
    return;

  /* First of all put zeros if needed */
  for ( j = 0; j < threads[i].next_free_counter; j++ )
  {
    if ( !global_counters )
    {
      type_mask = ( ( ( threads[i].counters[j].type ) / 10000 ) + threads[i].counters[j].type % 10000 + 10000 ) * 1000;
      type_mask += threads[i].counters[j].value;
    }
    else
      type_mask = threads[i].counters[j].type / 10000 + 20000 + threads[i].counters[j].type % 10000;

    if ( threads[i].counters[j].num >= ( unsigned long long )frequency && !threads[i].counters[j].last_is_zero )
    {
      if ( ( event = ( struct counter_event * )malloc( sizeof( struct counter_event ) ) ) == NULL )
      {
        perror( "No more memory!!!!\n" );
        exit( 1 );
      }

      event->cpu = cpu;
      event->time = threads[i].last_time_of_sc;
      event->type = type_mask;
      event->value = 0;
      event->next = NULL;

      threads[i].counters[j].last_is_zero = true;

      if ( threads[thread_pointer[appl][task][thread]].first_event_counter == NULL )
      {
        threads[thread_pointer[appl][task][thread]].first_event_counter = event;
        threads[thread_pointer[appl][task][thread]].last_event_counter = event;
      }
      else
      {
        threads[thread_pointer[appl][task][thread]].last_event_counter->next = event;
        threads[thread_pointer[appl][task][thread]].last_event_counter = event;
      }
    }
  }

  /* we pass over all its counters */
  for ( j = 0; j < threads[i].next_free_counter; j++ )
  {
    if ( !global_counters )
    {
      type_mask = ( ( ( threads[i].counters[j].type ) / 10000 ) + threads[i].counters[j].type % 10000 + 10000 ) * 1000;
      type_mask += threads[i].counters[j].value;
    }
    else
      type_mask = threads[i].counters[j].type / 10000 + 20000 + threads[i].counters[j].type % 10000;

    if ( threads[i].counters[j].num >= ( unsigned long long )frequency )
    {
      if ( ( event = ( struct counter_event * )malloc( sizeof( struct counter_event ) ) ) == NULL )
      {
        perror( "No more memory!!!!\n" );
        exit( 1 );
      }

      event->cpu = cpu;
      event->time = last_time;
      event->type = type_mask;
      event->value = threads[i].counters[j].num;
      event->next = NULL;

      threads[i].counters[j].last_is_zero = false;

      if ( threads[thread_pointer[appl][task][thread]].first_event_counter == NULL )
      {
        threads[thread_pointer[appl][task][thread]].first_event_counter = event;
        threads[thread_pointer[appl][task][thread]].last_event_counter = event;
      }
      else
      {
        threads[thread_pointer[appl][task][thread]].last_event_counter->next = event;
        threads[thread_pointer[appl][task][thread]].last_event_counter = event;
      }
    }
    threads[i].counters[j].num = 0;
  }
}


void KTraceSoftwareCounters::ini_progress_bar( char *file_name, ProgressController *progress )
{
#if defined(__FreeBSD__)
  struct stat file_info;
#elif defined(WIN32)
  struct _stat64 file_info;
#else
  struct stat64 file_info;
#endif

#if defined(__FreeBSD__)
  if ( stat( file_name, &file_info ) < 0 )
  {
    perror( "Error calling stat" );
    exit( 1 );
  }
#elif defined(WIN32)
  if ( _stat64( file_name, &file_info ) < 0 )
  {
    perror( "Error calling stat64" );
    exit( 1 );
  }
#else
  if ( stat64( file_name, &file_info ) < 0 )
  {
    perror( "Error calling stat64" );
    exit( 1 );
  }
#endif
  total_trace_size = file_info.st_size;

  /* Depen mida traça mostrem percentatge amb un interval diferent de temps */
  if ( total_trace_size < 500000000 )
    total_iters = 10000;
  else
    total_iters = 100000;

  current_read_size = 0;

  if( progress != NULL)
    progress->setEndLimit( total_trace_size );
}


void KTraceSoftwareCounters::show_progress_bar( ProgressController *progress )
{
//  double current_showed, i, j;

#if defined(__FreeBSD__) || defined(__APPLE__)
  current_read_size = ftello( infile );
#elif defined(WIN32)
  current_read_size = _ftelli64( infile );
#else
  current_read_size = ftello64( infile );
#endif

/*  i = ( double )( current_read_size );
  j = ( double )( total_trace_size );
  current_showed = i / j;
*/

// DOESN'T WORK ON COMPRESSED TRACE
//  if ( is_zip_filter )
//    current_read_size = current_read_size / COMPRESSION_RATIO_GZIP;

  if( progress != NULL)
    progress->setCurrentProgress( current_read_size );
}


void KTraceSoftwareCounters::put_counters_on_state_by_thread( int appl, int task, int thread )
{
  int i, j;
  unsigned long long type_mask;

  /* We search the thread on the struct */
  for ( i = 0; i < next_thread_slot; i++ )
  {
    if ( threads[i].appl == appl && threads[i].task == task && threads[i].thread == thread )
      break;
  }

  if ( i == next_thread_slot )
    return;

  /* we pass over all its counters */
  for ( j = 0; j < threads[i].next_free_counter; j++ )
  {
    if ( acumm_values )
      type_mask = threads[i].counters[j].type;
    else
    {
      if ( !global_counters )
      {
        type_mask = ( ( ( threads[i].counters[j].type ) / 10000 ) + threads[i].counters[j].type % 10000 + 10000 ) * 1000;
        type_mask += threads[i].counters[j].value;
      }
      else
        type_mask = threads[i].counters[j].type / 10000 + 20000 + threads[i].counters[j].type % 10000;
    }

    /*
             if(threads[i].counters[j].num > 0 || !threads[i].counters[j].last_is_zero)*/
    fprintf( outfile, "2:0:%d:%d:%d:%lld:%lld:%lld\n", threads[i].appl, threads[i].task, threads[i].thread, last_time, type_mask, threads[i].counters[j].num );

    threads[i].counters[j].num = 0;
    /*
             if(threads[i].counters[j].num == 0)
                    threads[i].counters[j].last_is_zero = true;
             else {
                    threads[i].counters[j].num=0;
                    threads[i].counters[j].last_is_zero = false;
             }
      */
  }

  threads[i].last_time_of_sc = last_time;

  /* Put burst counters if needed */
  if ( summarize_bursts )
  {
    fprintf( outfile, "2:0:%d:%d:%d:%lld:1:%lld\n", threads[i].appl, threads[i].task, threads[i].thread, last_time, threads[i].total_burst_time );
    threads[i].total_burst_time = 0;
  }
}


void KTraceSoftwareCounters::sc_by_time( ProgressController *progress )
{
  int id, cpu, appl, task, thread, state;
  unsigned long long time_1, time_2, type, value;
  char *word, buffer[MAX_LINE_SIZE];
  bool print_line = false;
  int thread_id, i, j;
  unsigned long num_iters = 0;

  /* Trace processing */
  while ( fscanf( infile, "%d:%d:%d:%d:%d:%lld:", &id, &cpu, &appl, &task, &thread, &time_1 ) != EOF )
  {
    if ( num_iters == total_iters )
    {
      show_progress_bar( progress );
      num_iters = 0;
    }
    else
      num_iters++;

    switch ( id )
    {
      case 1:
        fscanf( infile, "%lld:%d\n", &time_2, &state );

        if ( state != 1 )
          break;

        if ( ( i = thread_pointer[appl][task][thread] ) == -1 )
        {
          threads[next_thread_slot].appl = appl;
          threads[next_thread_slot].task = task;
          threads[next_thread_slot].thread = thread;
          threads[next_thread_slot].next_free_counter = 0;
          threads[next_thread_slot].last_time_of_sc = 0;
          threads[next_thread_slot].total_burst_time = 0;
          i = next_thread_slot;
          next_thread_slot++;
          thread_pointer[appl][task][thread] = i;
        }

        threads[i].ini_burst_time = time_1;
        threads[i].end_burst_time = time_2;

        if ( time_2 >= ( threads[i].last_time_of_sc + interval ) )
        {
          last_time = time_1;
          put_counters_on_state_by_thread( appl, task, thread );
        }

        /* Saving the burst time in order to summarize */
        if ( summarize_bursts )
          threads[i].total_burst_time += ( time_2 - time_1 );


        if ( !remove_states )
          fprintf( outfile, "1:%d:%d:%d:%d:%lld:%lld:%d\n", cpu, appl, task, thread, time_1, time_2, state );

        break;

      case 2:
        if ( ( i = thread_pointer[appl][task][thread] ) == -1 )
        {
          threads[next_thread_slot].appl = appl;
          threads[next_thread_slot].task = task;
          threads[next_thread_slot].thread = thread;
          threads[next_thread_slot].next_free_counter = 0;
          threads[next_thread_slot].last_time_of_sc = 0;
          threads[next_thread_slot].ini_burst_time = 0;
          threads[next_thread_slot].end_burst_time = 0;
          threads[next_thread_slot].total_burst_time = 0;
          i = next_thread_slot;
          next_thread_slot++;
          thread_pointer[appl][task][thread] = i;
        }

        /* Flush buffer and put counters */
        /*                if(time_1 >= (threads[i].last_time_of_sc + interval)) {
                         last_time = time_1;
                         put_counters_on_state_by_thread(appl, task, thread);
                        }
        */
        fgets( line, sizeof( line ), infile );

        /* Event type and values */
        word = strtok( line, ":" );
        type = atoll( word );
        word = strtok( NULL, ":" );
        value = atoll( word );

        /* For keeping some events */
        if ( keep_events )
        {
          sprintf( buffer, "2:%d:%d:%d:%d:%lld:", cpu, appl, task, thread, time_1 );

          for ( j = 0; j < types_to_keep.next_free_slot; j++ )
          {
            if ( types_to_keep.type[i] == type )
            {
              sprintf( buffer, "%s%lld:%lld", buffer, type, value );
              print_line = true;
              break;
            }
          }
        }

        /* Counting events */
        if ( only_in_bursts )
        {
          if ( time_1 > threads[i].ini_burst_time && time_1 <= threads[i].end_burst_time )
            thread_id = inc_counter( appl, task, thread, type, value );
        }
        else
          thread_id = inc_counter( appl, task, thread, type, value );

        /* Parsing of multiple events on a single line */
        while ( ( word = strtok( NULL, ":" ) ) != NULL )
        {
          type = atoll( word );
          word = strtok( NULL, ":" );
          value = atoll( word );

          if ( keep_events )
          {
            int j;
            for ( j = 0; j < types_to_keep.next_free_slot; j++ )
            {
              if ( types_to_keep.type[i] == type )
              {
                sprintf( buffer, "%s%lld:%lld", buffer, type, value );
                print_line = true;
                break;
              }
            }
          }

          if ( only_in_bursts )
          {
            if ( time_1 > threads[i].ini_burst_time && time_1 <= threads[i].end_burst_time )
              thread_id = inc_counter( appl, task, thread, type, value );
          }
          else
            inc_counter( appl, task, thread, type, value );
        }

        if ( print_line )
        {
          fprintf( outfile, "%s\n", buffer );
          print_line = false;
        }

        break;

      default:
        fgets( line, sizeof( line ), infile );
        break;
    }
  }

  /* Put counters and flush events of the last interval */
  last_time = trace_time - 10;
  put_all_counters();
  // ok_sc_wait_window();
}


void KTraceSoftwareCounters::flush_counter_buffers( void )
{
  int i, end_flush = 0, current_thread = 0;
  unsigned long long current_time = 0;
  struct counter_event *printed_event;

  while ( !end_flush )
  {
    /* Take the first event */
    for ( i = 0; i < next_thread_slot; i++ )
    {
      if ( threads[i].first_event_counter != NULL )
      {
        current_time = threads[i].first_event_counter->time;
        current_thread = i;
        break;
      }
    }
    if ( i == next_thread_slot )
    {
      end_flush = 1;
      continue;
    }

    /* Take the event with min time */
    for ( i = 0; i < next_thread_slot; i++ )
    {
      if ( threads[i].first_event_counter != NULL )
      {
        if ( threads[i].first_event_counter->time < current_time )
        {
          current_time = threads[i].first_event_counter->time;
          current_thread = i;
        }
      }
    }

    /* Put the event on the trace */
    fprintf( outfile, "2:%d:%d:%d:%d:%lld:%lld:%lld\n", threads[current_thread].first_event_counter->cpu, threads[current_thread].appl, threads[current_thread].task, threads[current_thread].thread, current_time, threads[current_thread].first_event_counter->type, threads[current_thread].first_event_counter->value );

    printed_event = threads[current_thread].first_event_counter;
    threads[current_thread].first_event_counter = threads[current_thread].first_event_counter->next;
    free( printed_event );
  }
}


void KTraceSoftwareCounters::sc_by_event( ProgressController *progress )
{
  int id, cpu, appl, task, thread, i;
  unsigned long long time_1, type, value;
  char *word;
  struct counter_event *event;
  int thread_id, find_mark = 0;
  unsigned long num_iters = 0;

  /* Trace processing */
  while ( fscanf( infile, "%d:%d:%d:%d:%d:%lld:", &id, &cpu, &appl, &task, &thread, &time_1 ) != EOF )
  {
    if ( num_iters == total_iters )
    {
      show_progress_bar( progress );
      num_iters = 0;
    }
    else
      num_iters++;

    if ( id == 2 )
    {
      /* Saving of the current event on the buffer and upgrading */
      /* the value of the counters                               */
      fgets( line, sizeof( line ), infile );

      if ( ( i = thread_pointer[appl][task][thread] ) == -1 )
      {
        threads[next_thread_slot].appl = appl;
        threads[next_thread_slot].task = task;
        threads[next_thread_slot].thread = thread;
        threads[next_thread_slot].next_free_counter = 0;
        threads[next_thread_slot].last_time_of_sc = 0;
        threads[next_thread_slot].first_event_counter = NULL;
        threads[next_thread_slot].last_event_counter = NULL;
        threads[next_thread_slot].calls.top = -1;
        i = next_thread_slot;
        next_thread_slot++;
        thread_pointer[appl][task][thread] = i;
      }

      /* Event type and values */
      word = strtok( line, ":" );
      type = atoll( word );
      word = strtok( NULL, ":" );
      value = atoll( word );

      if ( allowed_type_mark( type ) )
      {
        find_mark = 1;
        if ( ( event = ( struct counter_event * )malloc( sizeof( struct counter_event ) ) ) == NULL )
        {
          perror( "No more memory!!!!\n" );
          exit( 1 );
        }

        event->cpu = cpu;
        event->time = time_1;
        event->type = type;
        event->value = value;
        event->next = NULL;

        if ( threads[thread_pointer[appl][task][thread]].first_event_counter == NULL )
        {
          threads[thread_pointer[appl][task][thread]].first_event_counter = event;
          threads[thread_pointer[appl][task][thread]].last_event_counter = event;
        }
        else
        {
          threads[thread_pointer[appl][task][thread]].last_event_counter->next = event;
          threads[thread_pointer[appl][task][thread]].last_event_counter = event;
        }

        last_time = time_1;
        put_counters_by_thread( appl, task, thread, cpu );
      }
      else
        thread_id = inc_counter( appl, task, thread, type, value );

      /* Parsing of multiple events on a single line */
      while ( ( word = strtok( NULL, ":" ) ) != NULL )
      {
        type = atoll( word );
        word = strtok( NULL, ":" );
        value = atoll( word );

        if ( allowed_type_mark( type ) )
        {
          find_mark = 1;
          if ( ( event = ( struct counter_event * )malloc( sizeof( struct counter_event ) ) ) == NULL )
          {
            perror( "No more memory!!!!\n" );
            exit( 1 );
          }

          event->cpu = cpu;
          event->time = time_1;
          event->type = type;
          event->value = value;
          event->next = NULL;

          if ( threads[thread_pointer[appl][task][thread]].first_event_counter == NULL )
          {
            threads[thread_pointer[appl][task][thread]].first_event_counter = event;
            threads[thread_pointer[appl][task][thread]].last_event_counter = event;
          }
          else
          {
            threads[thread_pointer[appl][task][thread]].last_event_counter->next = event;
            threads[thread_pointer[appl][task][thread]].last_event_counter = event;
          }

          last_time = time_1;
          put_counters_by_thread( appl, task, thread, cpu );
        }
        else
          thread_id = inc_counter( appl, task, thread, type, value );
      }
    }
    else
      fgets( line, sizeof( line ), infile );

    if ( find_mark )
    {
      threads[thread_pointer[appl][task][thread]].last_time_of_sc = time_1;
      find_mark = 0;
    }
  }

  flush_counter_buffers();
}


void KTraceSoftwareCounters::insert_in_queue_state( int thread_id, unsigned long long time )
{
  struct state_queue_elem *p, *q, *new_elem;
  p = NULL;
  q = NULL;
  new_elem = NULL;

  if ( ( new_elem = ( struct state_queue_elem * )malloc( sizeof( struct state_queue_elem ) ) ) == NULL )
  {
    perror( "No more memory for state queue!!!!\n" );
    exit( 1 );
  }

  new_elem->last_state_end_time = time;
  new_elem->thread_id = thread_id;
  new_elem->next = NULL;

  if ( first_state_elem == NULL )
  {
    first_state_elem = new_elem;
    return;
  }

  for ( p = first_state_elem; p != NULL; p = p->next )
  {
    if ( p->last_state_end_time > time )
    {
      if ( first_state_elem == p )
      {
        new_elem->next = p;
        first_state_elem = new_elem;
      }
      else
      {
        new_elem->next = p;
        q->next = new_elem;
      }
      break;
    }
    else
      q = p;
  }
  if ( p == NULL )
  {
    q->next = new_elem;
  }
}


void KTraceSoftwareCounters::put_counters_on_state( struct KTraceSoftwareCounters::state_queue_elem *p,
                                                    struct KTraceSoftwareCounters::state_queue_elem *q )
{
  int i, j;
  unsigned long long type_mask;

  i = p->thread_id;
  /* For every thread, we pass over all its counters */
  for ( j = 0; j < threads[i].next_free_counter; j++ )
  {
    if ( acumm_values )
      type_mask = threads[i].counters[j].type;
    else
    {
      if ( !global_counters )
      {
        type_mask = ( ( ( threads[i].counters[j].type ) / 10000 ) + threads[i].counters[j].type % 10000 + 10000 ) * 1000;
        type_mask += threads[i].counters[j].value;
      }
      else
        type_mask = threads[i].counters[j].type / 10000 + 20000 + threads[i].counters[j].type % 10000;
    }
    /*
      if(threads[i].counters[j].num > 0 || !threads[i].counters[j].last_is_zero)
    */
    fprintf( outfile, "2:0:%d:%d:%d:%lld:%lld:%lld\n", threads[i].appl, threads[i].task, threads[i].thread, p->last_state_end_time, type_mask, threads[i].counters[j].num );

    threads[i].counters[j].num = 0;
    /*
             if(threads[i].counters[j].num == 0)
                    threads[i].counters[j].last_is_zero = true;
             else {
                    threads[i].counters[j].num=0;
                    threads[i].counters[j].last_is_zero = false;
             }*/
  }

  if ( first_state_elem == p )
    first_state_elem = p->next;
  else
    q->next = p->next;

  free( p );
}


void KTraceSoftwareCounters::sc_by_states( ProgressController *progress )
{
  int id, cpu, appl, task, thread, state;
  unsigned long long time_1, time_2, type, value;
  char *word, buffer[MAX_LINE_SIZE];
  bool print_line = false;
  struct state_queue_elem *p, *q;
  int i, j;
  unsigned long num_iters = 0;

  p = NULL;
  q = NULL;

  /* Trace processing */
  while ( fscanf( infile, "%d:%d:%d:%d:%d:%lld:", &id, &cpu, &appl, &task, &thread, &time_1 ) != EOF )
  {
    if ( num_iters == total_iters )
    {
      show_progress_bar( progress );
      num_iters = 0;
    }
    else
      num_iters++;

    if ( id == 1 )
    {
      fscanf( infile, "%lld:%d\n", &time_2, &state );

      if ( ( i = thread_pointer[appl][task][thread] ) == -1 )
      {
        threads[next_thread_slot].appl = appl;
        threads[next_thread_slot].task = task;
        threads[next_thread_slot].thread = thread;
        threads[next_thread_slot].next_free_counter = 0;
        threads[next_thread_slot].last_time_of_sc = 0;
        i = next_thread_slot;
        next_thread_slot++;
        thread_pointer[appl][task][thread] = i;
      }


      /* Bolcar tots els threads que hagin de posar contadors */
      /*            for(p = first_state_elem; p!=NULL; p = p->next) {
        if(p->last_state_end_time <= time_1)
         put_counters_on_state(p, q);
        else {
         q = p;
         break;
        }
                  }
      */

      if ( ( min_state_time != 0 && ( time_2 - time_1 >= min_state_time ) && state == 1 ) || !min_state_time )
      {
        /* Insertar quan se li acaba l'estat */
        insert_in_queue_state( i, time_2 );

        /* Posar contadors per a aquest thread */
        last_time = time_1;
        put_counters_on_state_by_thread( appl, task, thread );

        /* Posem el record d'estat */
        fprintf( outfile, "1:%d:%d:%d:%d:%lld:%lld:1\n", cpu, appl, task, thread, time_1, time_2 );
      }

      continue;
    }

    if ( id == 2 )
    {
      /* Incrementing the counters */
      fgets( line, sizeof( line ), infile );

      if ( ( i = thread_pointer[appl][task][thread] ) == -1 )
      {
        threads[next_thread_slot].appl = appl;
        threads[next_thread_slot].task = task;
        threads[next_thread_slot].thread = thread;
        threads[next_thread_slot].next_free_counter = 0;
        threads[next_thread_slot].last_time_of_sc = 0;
        i = next_thread_slot;
        next_thread_slot++;
        thread_pointer[appl][task][thread] = i;
      }

      /* Bolcar tots els threads que hagin de posar contadors */
      for ( p = first_state_elem; p != NULL; p = p->next )
      {
        if ( p->last_state_end_time < time_1 )
          put_counters_on_state( p, q );
        else
        {
          q = p;
          break;
        }
      }

      /* Event type and values */
      word = strtok( line, ":" );
      type = atoll( word );
      word = strtok( NULL, ":" );
      value = atoll( word );

      if ( keep_events )
      {
        sprintf( buffer, "2:%d:%d:%d:%d:%lld:", cpu, appl, task, thread, time_1 );
        for ( j = 0; j < types_to_keep.next_free_slot; j++ )
        {
          if ( types_to_keep.type[j] == type )
          {
            sprintf( buffer, "%s%lld:%lld", buffer, type, value );
            print_line = true;
            break;
          }
        }
      }

      inc_counter( appl, task, thread, type, value );

      /* Parsing of multiple events on a single line */
      while ( ( word = strtok( NULL, ":" ) ) != NULL )
      {
        type = atoll( word );
        word = strtok( NULL, ":" );
        value = atoll( word );

        if ( keep_events )
        {
          int j;
          for ( j = 0; j < types_to_keep.next_free_slot; j++ )
          {
            if ( types_to_keep.type[i] == type )
            {
              sprintf( buffer, "%s%lld:%lld", buffer, type, value );
              print_line = true;
              break;
            }
          }
        }

        inc_counter( appl, task, thread, type, value );
      }

      if ( print_line )
      {
        fprintf( outfile, "%s\n", buffer );
        print_line = false;
      }

      continue;
    }
    /* Record ni d'estat ni event, el saltem */
    else
      fgets( line, sizeof( line ), infile );
  }


  /* Posem els contadors que falten */
  for ( p = first_state_elem; p != NULL; p = p->next )
  {
    put_counters_on_state( p, NULL );
  }

//  ok_sc_wait_window();
}


void KTraceSoftwareCounters::execute( char *trace_in, char *trace_out, ProgressController *progress )
{
  bool is_zip = false;
  char *tmp_dir = NULL, *c, trace_name[512], *trace_header;
  int i, j, k;

  /* Ini data */
  first_Paraver_event = NULL;
  last_Paraver_event = NULL;
  next_thread_slot = 0;
  types.next_free_slot = 0;
  types_to_keep.next_free_slot = 0;

  for ( i = 0;i < MAX_APPL;i++ )
    for ( j = 0;j < MAX_TASK;j++ )
      for ( k = 0;k < MAX_THREAD;k++ )
        thread_pointer[i][j][k] = -1;

  /* Reading of program args */
  read_sc_args();

  /* Open the files.  If NULL is returned there was an error */
  /* Is a zipped trace ? */
  if ( ( c = strrchr( trace_in, '.' ) ) != NULL )
  {
    /* The name finishes with -gz */
    if ( strlen( c ) == 3 )
    {
      tmp_dir = getenv( "TMP_DIR" );
      if ( tmp_dir != NULL )
      {
        sprintf( line, "gzip -dc %s > %s/tmp.prv", trace_in, tmp_dir );
        printf( "\nDecompressing zipped trace...\n" );
        system( line );
        sprintf( line, "%s/tmp.prv", tmp_dir );
        strcpy( trace_name, line );
        is_zip = true;
      }
      else
      {
        printf( "\nERROR: You have to set variable TMP_DIR in order to decompress de zipped trace\n" );
        exit( 1 );
      }
    }
    else
    {
      strcpy( trace_name, trace_in );
    }
  }

#if defined(__FreeBSD__) || defined(__APPLE__)
  if ( ( infile = fopen( trace_name, "r" ) ) == NULL )
  {
    printf( "Error Opening File %s\n", trace_name );
    exit( 1 );
  }
  if ( ( outfile = fopen( trace_out, "w" ) ) == NULL )
  {
    printf( "Error Opening File %s\n", trace_out );
    exit( 1 );
  }
#elif defined(WIN32)
  if ( fopen_s( &infile, trace_name, "r" ) != 0 )
  {
    printf( "Error Opening File %s\n", trace_name );
    exit( 1 );
  }

  if ( fopen_s( &outfile, trace_out, "w" ) != 0 )
  {
    printf( "Error Opening File %s\n", trace_out );
    exit( 1 );
  }
#else
  if ( ( infile = fopen64( trace_name, "r" ) ) == NULL )
  {
    printf( "Error Opening File %s\n", trace_name );
    exit( 1 );
  }

  if ( ( outfile = fopen64( trace_out, "w" ) ) == NULL )
  {
    printf( "Error Opening File %s\n", trace_out );
    exit( 1 );
  }
#endif

  write_pcf( trace_out );

  ini_progress_bar( trace_name, progress );

  /* Read header */
  trace_header = ( char * )malloc( sizeof( char ) * MAX_HEADER_SIZE );
  fgets( trace_header, MAX_HEADER_SIZE, infile );
  proces_header( trace_header, infile, outfile );
  free( trace_header );

  if ( type_of_counters )
    sc_by_time( progress );
  else
    sc_by_states( progress );

  /* Close the files */
  fclose( infile );
  fclose( outfile );
  if ( is_zip )
  {
    sprintf( line, "rm %s/tmp.prv", tmp_dir );
    system( line );
  }
}
