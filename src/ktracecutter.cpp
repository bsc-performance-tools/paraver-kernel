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
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include <zlib.h>

#include "ktracecutter.h"
#include "kprogresscontroller.h"

#ifdef WIN32
#define atoll _atoi64
#endif

KTraceCutter::KTraceCutter( char *&trace_in,
                            char *&trace_out,
                            TraceOptions *options,
                            ProgressController *progress )
{
  total_cutter_iters = 0;
  exec_options = new KTraceOptions( (KTraceOptions *) options );
  execute( trace_in, trace_out, progress );
}


KTraceCutter::~KTraceCutter()
{
}


void KTraceCutter::set_by_time( bool byTime )
{
}


void KTraceCutter::set_min_cutting_time( unsigned long long minCutTime )
{
}


void KTraceCutter::set_max_cutting_time( unsigned long long maxCutTime )
{
}


void KTraceCutter::set_minimum_time_percentage( unsigned long long minimumPercentage )
{
}


void KTraceCutter::set_maximum_time_percentage( unsigned long long maximumPercentage )
{
}


void KTraceCutter::set_tasks_list( char *tasksList )
{
}


void KTraceCutter::set_original_time( bool originalTime )
{
}


void KTraceCutter::set_max_trace_size( int traceSize )
{
}


void KTraceCutter::set_break_states( bool breakStates )
{
}


void KTraceCutter::set_remFirstStates( bool remStates )
{
}


void KTraceCutter::set_remLastStates( bool remStates )
{
}


/* Function for parsing program parameters */
void KTraceCutter::read_cutter_params()
{
  char *word, *buffer;

  //by_time = true;
  by_time = exec_options->by_time;
  time_min = exec_options->min_cutting_time;
  time_max = exec_options->max_cutting_time;
  total_time = time_max - time_min;
  min_perc = exec_options->min_percentage;
  max_perc = exec_options->max_percentage;

  if ( exec_options->original_time )
    old_times = true;

  if ( exec_options->tasks_list[0] != '\0' )
  {
    cut_tasks = true;
    int j = 0;

    word = strtok( exec_options->tasks_list, "," );
    do
    {
      if ( ( buffer = strchr( word, '-' ) ) != NULL )
      {
        *buffer = '\0';
        wanted_tasks[j].min_task_id = atoll( word );
        wanted_tasks[j].max_task_id = atoll( ++buffer );
        wanted_tasks[j].range = 1;
//cout << "T-> " <<wanted_tasks[j].min_task_id << "-" << wanted_tasks[j].max_task_id << endl;

      }
      else
      {
        wanted_tasks[j].min_task_id = atoll( word );
        wanted_tasks[j].range = 0;
//cout << "T-> " << wanted_tasks[j].min_task_id << endl;
      }

      j++;
    }
    while ( ( word = strtok( NULL, "," ) ) != NULL );
  }

  if ( exec_options->max_trace_size != 0 )
    max_size = exec_options->max_trace_size * 1000000;

  break_states = exec_options->break_states;
  remFirstStates = exec_options->remFirstStates;
  remLastStates = exec_options->remLastStates;
}


/* For processing the Paraver header */
void KTraceCutter::proces_cutter_header( char *header,
                                         char *trace_in_name,
                                         char *trace_out_name )
{
  int num_comms;
  char *word;

  word = strtok( header, ":" );
  current_size += fprintf( outfile, "%s:", word );

  word = strtok( NULL, ":" );
  current_size += fprintf( outfile, "%s:", word );

  /* Obtaining the trace total time */
  word = strtok( NULL, ":" );

  if ( strstr( word, "_ns" ) )
  {
    word[ strlen( word ) - 3 ] = '\0';
    trace_time = atoll( word );

    if ( !by_time )
    {
      trace_time = atoll( word );
      time_min = ( ( double )( trace_time / 100 ) ) * min_perc;
      time_max = ( ( double )( trace_time / 100 ) ) * max_perc;
      total_time = time_max - time_min;
    }

    if ( !old_times )
      current_size += fprintf( outfile, "%lld_ns:", total_time );
    else
      current_size += fprintf( outfile, "%s_ns:", word );
  }
  else
  {
    trace_time = atoll( word );
    if ( !by_time )
    {
      trace_time = atoll( word );
      time_min = ( ( double )( trace_time / 100 ) ) * min_perc;
      time_max = ( ( double )( trace_time / 100 ) ) * max_perc;
      total_time = time_max - time_min;
    }

    if ( !old_times ) current_size += fprintf( outfile, "%lld:", total_time );
    else current_size += fprintf( outfile, "%s:", word );

  }

  word = strtok( NULL, "\n" );
  current_size += fprintf( outfile, "%s\n", word );

  word = strrchr( word, ',' );

  /* Obtaining the number of communicators */
  strcpy( header, word + 1 );
  if ( strchr( header, ')' ) != NULL ) return;
  num_comms = atoi( header );

  while ( num_comms > 0 )
  {
    if ( !is_zip ) fgets( header, MAX_TRACE_HEADER, infile );
    else gzgets( gzInfile, header, MAX_TRACE_HEADER );
    current_size += fprintf( outfile, "%s", header );
    num_comms--;
  }

  /* Writing in the header the offset of the cut regard original trace */

  /* Reading first if we have old offsets into the trace */
  if ( !is_zip ) fgets( header, MAX_TRACE_HEADER, infile );
  else gzgets( gzInfile, header, MAX_TRACE_HEADER );

  while ( header[0] == '#' )
  {
    if ( !is_zip )
    {
      if( feof( infile ) ) break;
    }
    else
    {
      if ( gzeof( gzInfile ) ) break;
    }
    current_size += fprintf( outfile, "%s", header );

    if ( !is_zip ) fgets( header, MAX_TRACE_HEADER, infile );
    else gzgets( gzInfile, header, MAX_TRACE_HEADER );
  }

  if ( !is_zip )
#ifdef WIN32
    _fseeki64( infile, 0, SEEK_SET );
#else
    fseek( infile, -( strlen( header ) ), SEEK_CUR );
#endif
  else
    gzseek( gzInfile, -( strlen( header ) ), SEEK_CUR );

  /* Writing of the current cut offset */
  if ( trace_in_name != "\0" )
    current_size += fprintf( outfile, "# %s: Offset %lld from %s\n", trace_out_name, time_min, trace_in_name );
}


void KTraceCutter::adjust_to_final_time()
{
  unsigned long long next_time = 0;
  struct thread_info *p, *q;


  while ( num_tasks > 0 )
  {

    next_time = first->last_time;

    q = first;
    for ( p = first->next; p != NULL; p = p->next )
      if ( p->last_time < next_time )
      {
        next_time = p->last_time;
        q = p;
      }

    if ( old_times )
      fprintf( outfile, "1:0:%d:%d:%d:%lld:%lld:14\n", q->appl, q->task, q->thread, next_time, trace_time );

    if ( q->first != ( q->last + 1 ) % 20 )
    {

      while ( q->first != q->last )
      {
        fprintf( outfile, "2:%d:%d:%d:%d:%lld:%lld:0\n", q->task, q->appl, q->task, q->thread, next_time, q->event_queue[q->first] );
        q->first = ( q->first + 1 ) % 20;
      }

      fprintf( outfile, "2:%d:%d:%d:%d:%lld:%lld:0\n", q->task, q->appl, q->task, q->thread, next_time, q->event_queue[q->first] );
    }

    num_tasks--;
    next_time = 0;
    if ( num_tasks > 0 )
    {
      if ( q == first )
      {
        first = q->next;
        first->previous = NULL;
      }
      else
      {
        if ( q == last )
        {
          last = q->previous;
          last->next = NULL;
        }
        else
        {
          ( q->next )->previous = q->previous;
          ( q->previous )->next = q->next;
        }
      }
    }
    free( q );
  }


}

void KTraceCutter::ini_cutter_progress_bar( char *file_name,
                                            ProgressController *progress )
{
  struct stat file_info;

  if ( stat( file_name, &file_info ) < 0 )
  {
    perror( "Error calling stat64" );
    exit( 1 );
  }
  total_trace_size = file_info.st_size;

  /* Depen mida tra�a mostrem percentatge amb un interval diferent de temps */
  if ( total_trace_size < 500000000 ) total_cutter_iters = 10000;
  else total_cutter_iters = 100000;

  current_read_size = 0;

  if( progress != NULL)
    progress->setEndLimit( total_trace_size );
}



void KTraceCutter::show_cutter_progress_bar( ProgressController *progress )
{
//  double current_showed, i, j;


#if defined(FreeBSD)
  if ( !is_zip )
    current_read_size = ( unsigned long long )ftello( infile );
  else
    current_read_size = ( unsigned long )gztell( gzInfile );
#elif defined(WIN32)
  if ( !is_zip )
    current_read_size = ( unsigned long long )_ftelli64( infile );
  else
    current_read_size = ( unsigned long )gztell( gzInfile );
#else
  if ( !is_zip )
    current_read_size = ( unsigned long long )ftello64( infile );
  else
    current_read_size = ( unsigned long )gztell( gzInfile );
#endif


/*  i = ( double )( current_read_size );
  j = ( double )( total_trace_size );

  current_showed = i / j;*/

  progress->setCurrentProgress( current_read_size );
}


void KTraceCutter::update_queue( int appl, int task, int thread,
                                 unsigned long long type,
                                 unsigned long long value )
{
  int i;
  struct thread_info *p;


  if ( tasks[appl][task][thread] == NULL )
  {
    if ( ( p = ( struct thread_info * )malloc( sizeof( struct thread_info ) ) ) == NULL )
    {
      perror( "No more memory!!!\n" );
      exit( 1 );
    }
    if ( first == NULL )
    {
      first = p;
      p->previous = NULL;
    }

    p->next = NULL;
    if ( last == NULL ) last = p;
    else
    {
      p->previous = last;
      last->next = p;
      last = p;
    }

    p->appl = appl + 1;
    p->task = task + 1;
    p->thread = thread + 1;

    tasks[appl][task][thread] = p;
    tasks[appl][task][thread]->last_time = 0;
    tasks[appl][task][thread]->finished = false;
    tasks[appl][task][thread]->first = 0;
    tasks[appl][task][thread]->last = -1;

    num_tasks++;
    useful_tasks++;
    init_task_counter = 1;
  }

  if ( value > 0 )
  {

    tasks[appl][task][thread]->last = ( tasks[appl][task][thread]->last + 1 ) % 20;
    tasks[appl][task][thread]->event_queue[tasks[appl][task][thread]->last] = type;

  }
  else
  {

    if ( tasks[appl][task][thread]->first == ( tasks[appl][task][thread]->last + 1 ) % 20 ) return;

    for ( i = tasks[appl][task][thread]->first; i != tasks[appl][task][thread]->last; i = ( i + 1 ) % 20 )
      if ( tasks[appl][task][thread]->event_queue[i] == type ) break;

    tasks[appl][task][thread]->first = ( i + 1 ) % 20;
  }
}


void KTraceCutter::load_counters_of_pcf( char *trace_name )
{
  char *pcf_name, *c;
  FILE *pcf;
  char *id;


  pcf_name = strdup( trace_name );
  c = strrchr( pcf_name, '.' );
  sprintf( c, ".pcf" );

  last_counter = 0;
  if ( ( pcf = fopen( pcf_name, "r" ) ) == NULL ) return;

  while ( fgets( line, sizeof( line ), pcf ) != NULL )
  {

    if ( strstr( line, " 42000" ) != NULL || strstr( line, " 42001" ) != NULL )
    {
      id = strtok( line, " " );
      id = strtok( NULL, " " );


      counters[last_counter] = atoll( id );
      last_counter++;

      if ( last_counter == 50 )
      {
        printf( "NO more memory for loading counters of .pcf\n" );
        return;
      }
    }
  }
}


void KTraceCutter::shift_trace_to_zero( char *nameIn, char *nameOut )
{
  unsigned long long timeOffset = 0, time_1, time_2, time_3, time_4;
  int cpu, appl, task, thread, state, cpu_2, appl_2, task_2, thread_2;
  char *trace_header;

#if defined(FreeBSD)
  if ( ( infile = fopen( nameIn, "r" ) ) == NULL )
  {
    perror( "ERROR" );
    printf( "KCutter: Error Opening File %s\n", nameIn );
    exit( 1 );
  }
#elif defined(WIN32)
  if ( fopen_s( &infile, nameIn, "r" ) != 0 )
  {
    printf( "KCutter: Error Opening File %s\n", nameIn );
    exit( 1 );
  }
#else
  if ( ( infile = fopen64( nameIn, "r" ) ) == NULL )
  {
    perror( "ERROR" );
    printf( "KCutter: Error Opening File %s\n", nameIn );
    exit( 1 );
  }
#endif



#if defined(FreeBSD)
  if ( ( outfile = fopen( nameOut, "w" ) ) == NULL )
  {
    perror( "ERROR" );
    printf( "KCutter: Error Opening File %s\n", nameOut );
    exit( 1 );
  }
#elif defined(WIN32)
  if ( fopen_s( &outfile, nameOut, "w" ) != 0 )
  {
    printf( "KCutter: Error Opening File %s\n", nameOut );
    exit( 1 );
  }
#else
  if ( ( outfile = fopen64( nameOut, "w" ) ) == NULL )
  {
    perror( "ERROR" );
    printf( "KCutter: Error Opening File %s\n", nameOut );
    exit( 1 );
  }
#endif

  /* Process header */
  total_time = last_record_time - first_record_time;
  trace_header = ( char * ) malloc( sizeof( char ) * MAX_TRACE_HEADER );
  fgets( trace_header, MAX_TRACE_HEADER, infile );


  proces_cutter_header( trace_header, (char *)string("\0").c_str(), (char *)string("\0").c_str() );

  fgets( trace_header, MAX_TRACE_HEADER, infile );
  sscanf( trace_header, "%*d:%*d:%*d:%*d:%*d:%lld:", &timeOffset );


  bool end_read = false;
  while ( !end_read )
  {
    switch ( trace_header[0] )
    {
      case '1':
        sscanf( trace_header, "%*d:%d:%d:%d:%d:%lld:%lld:%d\n", &cpu, &appl, &task, &thread, &time_1, &time_2, &state );


        time_1 = time_1 - timeOffset;
        time_2 = time_2 - timeOffset;

        fprintf( outfile, "1:%d:%d:%d:%d:%lld:%lld:%d\n", cpu, appl, task, thread, time_1, time_2, state );

        break;


      case '2':
        sscanf( trace_header, "%*d:%d:%d:%d:%d:%lld:%s", &cpu, &appl, &task, &thread, &time_1, line );

        time_1 = time_1 - timeOffset;

        fprintf( outfile, "2:%d:%d:%d:%d:%lld:%s\n", cpu, appl, task, thread, time_1, line );

        break;

      case '3':
        sscanf( trace_header, "%*d:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%s", &cpu, &appl, &task, &thread, &time_1, &time_2, &cpu_2, &appl_2, &task_2, &thread_2, &time_3, &time_4, line );

        time_1 = time_1 - timeOffset;
        time_2 = time_2 - timeOffset;
        time_3 = time_3 - timeOffset;
        time_4 = time_4 - timeOffset;

        fprintf( outfile, "3:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%s\n", cpu, appl, task, thread, time_1, time_2, cpu_2, appl_2, task_2, thread_2, time_3, time_4, line );

        break;


      default:
        break;
    }


    /* Read one more record is possible */
    if ( feof( infile ) )
      end_read = true;
    else
      fgets( trace_header, MAX_TRACE_HEADER, infile );
  }


  fclose( infile );
  fclose( outfile );
  unlink( nameIn );

}


/* Function for filtering tasks in cut */
bool KTraceCutter::is_selected_task( int task_id )
{
  int i;

  for ( i = 0; i < MAX_SELECTED_TASKS; i++ )
  {
    if ( wanted_tasks[i].min_task_id == 0 ) break;

    if ( wanted_tasks[i].range )
    {
      if ( task_id >= wanted_tasks[i].min_task_id && task_id <= wanted_tasks[i].max_task_id ) return true;
    }
    else
      if ( task_id == wanted_tasks[i].min_task_id ) return true;
  }

  return false;
}


void KTraceCutter::execute( char *trace_in,
                            char *trace_out,
                            ProgressController *progress )
{
  char *c, *tmp_dir, *word, *trace_header;
  char trace_name[1024], buffer[1024];
  bool end_parsing = false;
  bool reset_counters;
  char trace_file_out[2048];

  unsigned int id, cpu, appl, task, thread, state, cpu_2, appl_2, task_2, thread_2, size, tag;
  unsigned long long type, value, time_1, time_2, time_3, time_4;
  int i, j, k;
  bool end_line;

  unsigned long num_iters = 0;
  struct thread_info *p;

  /* Ini Data */
  for ( i = 0;i < MAX_APPL;i++ )
    for ( j = 0;j < MAX_TASK;j++ )
      for ( k = 0;k < MAX_THREAD;k++ )
        tasks[i][j][k] = NULL;


  by_time = false;
  old_times = false;
  max_size = 0;
  cut_tasks = false;
  break_states = true;
  is_zip = false;
  init_task_counter = 0;
  useful_tasks = 0;
  first_time_caught = false;
  num_tasks = 0;
  current_size = 0;

  first = NULL;
  last = NULL;

  for ( i = 0; i < MAX_SELECTED_TASKS; i++ )
    wanted_tasks[i].min_task_id = 0;

  /* Reading of the program arguments */
  read_cutter_params();

  strcpy( trace_name, trace_in );

  /* Is the trace zipped ? */
  if ( ( c = strrchr( trace_name, '.' ) ) != NULL )
  {
    /* The names finishes with .gz */
    if ( !strcmp( c, ".gz" ) )
      is_zip = true;
    else
      is_zip = false;
  }


  /* Load what counters appears in the trace */
  reset_counters = false;
  load_counters_of_pcf( trace_name );

  /* Open the files.  If NULL is returned there was an error */
  if ( !is_zip )
  {
#if defined(FreeBSD)
    if ( ( infile = fopen( trace_name, "r" ) ) == NULL )
    {
      perror( "ERROR" );
      printf( "KCutter: Error Opening File %s\n", trace_name );
      exit( 1 );
    }
#elif defined(WIN32)
    if ( fopen_s( &infile, trace_name, "r" ) != 0 )
    {
      perror( "ERROR" );
      printf( "KCutter: Error Opening File %s\n", trace_name );
      exit( 1 );
    }
#else
    if ( ( infile = fopen64( trace_name, "r" ) ) == NULL )
    {
      perror( "ERROR" );
      printf( "KCutter: Error Opening File %s\n", trace_name );
      exit( 1 );
    }
#endif
  }
  else
  {
    if ( ( gzInfile = gzopen( trace_name, "rb" ) ) == NULL )
    {
      printf( "KCutter: Error opening compressed trace\n" );
      exit( 1 );
    }
  }

  if ( !break_states )
  {
    if ( ( tmp_dir = getenv( "TMPDIR" ) ) == NULL )
      tmp_dir = getenv( "PWD" );

#ifdef WIN32
    sprintf( trace_file_out, "C:\\tmp_fileXXXXXX" );
    _mktemp_s( trace_file_out );
#else
    sprintf( trace_file_out, "%s/tmp_fileXXXXXX", tmp_dir );
    mkstemp( trace_file_out );
#endif
  }
  else
    strcpy( trace_file_out, trace_out );

#if defined(FreeBSD)
  if ( ( outfile = fopen( trace_file_out, "w" ) ) == NULL )
  {
    printf( "Error Opening KCutter Ouput File %s\n", trace_file_out );
    exit( 1 );
  }
#elif defined(WIN32)
  if ( fopen_s( &outfile, trace_file_out, "w" ) != 0 )
  {
    printf( "Error Opening KCutter Ouput File %s\n", trace_file_out );
    exit( 1 );
  }
#else
  if ( ( outfile = fopen64( trace_file_out, "w" ) ) == NULL )
  {
    printf( "Error Opening KCutter Ouput File %s\n", trace_file_out );
    exit( 1 );
  }
#endif

//cout << "trace_file_out: " << trace_file_out << endl;

  ini_cutter_progress_bar( trace_name, progress );


  /* Process header */
  trace_header = ( char * )malloc( sizeof( char ) * MAX_TRACE_HEADER );
  if ( !is_zip ) fgets( trace_header, MAX_TRACE_HEADER, infile );
  else
  {
    gzgets( gzInfile, trace_header, MAX_TRACE_HEADER );
  }

  proces_cutter_header( trace_header, trace_in, trace_out );
  free( trace_header );

  /* We process the trace like the old_times version */
  if ( !break_states ) old_times = true;

  /* Processing the trace records */
  while ( !end_parsing )
  {
    /* Read one more record is possible */
    if ( !is_zip )
    {
      if ( feof( infile ) || fgets( line, sizeof( line ), infile ) == NULL )
      {
        end_parsing = true;
        continue;
      }
    }
    else
    {
      if ( gzeof( gzInfile ) )
      {
        end_parsing = true;
        continue;
      }
      gzgets( gzInfile, line, sizeof( line ) );
    }

    if ( num_iters == total_cutter_iters )
    {
      show_cutter_progress_bar( progress );
      num_iters = 0;
    }
    else num_iters++;



    /* 1: state; 2: event; 3: comm; 4: global comm */
    switch ( line[0] )
    {

      case '1':

        sscanf( line, "%d:%d:%d:%d:%d:%lld:%lld:%d\n", &id, &cpu, &appl, &task, &thread, &time_1, &time_2, &state );

        /* If is a not traceable thread, get next record */
        if ( cut_tasks && !is_selected_task( task ) ) break;

        if ( time_2 <= time_min ) break;


        if ( time_1 < time_min && time_2 >= time_min && remFirstStates )
          break;

        if ( time_1 < time_max && time_2 > time_max && remLastStates )
          break;


        if ( old_times && time_1 > time_max )
        {
          if ( tasks[appl-1][task-1][thread-1] != NULL && tasks[appl-1][task-1][thread-1]->finished )
          {
            useful_tasks--;
            tasks[appl-1][task-1][thread-1]->finished = false;
          }
          break;
        }

        if ( !old_times && time_1 > time_max )
        {
          fclose( outfile );

          if ( !is_zip )
            fclose( infile );
          else
            gzclose( gzInfile );

//          ok_cutter_wait_window();
          return;
        }

        if ( tasks[appl-1][task-1][thread-1] == NULL )
        {
          if ( ( p = ( struct thread_info * )malloc( sizeof( struct thread_info ) ) ) == NULL )
          {
            perror( "No more memory!!!\n" );
            exit( 1 );
          }
          if ( first == NULL )
          {
            first = p;
            p->previous = NULL;
          }

          p->next = NULL;
          if ( last == NULL ) last = p;
          else
          {
            p->previous = last;
            last->next = p;
            last = p;
          }

          p->appl = appl;
          p->task = task;
          p->thread = thread;

          tasks[appl-1][task-1][thread-1] = p;
          num_tasks++;
          useful_tasks++;
          init_task_counter = 1;
          p->finished = true;
          tasks[appl-1][task-1][thread-1]->first = 0;
          tasks[appl-1][task-1][thread-1]->last = -1;


          /* Have to reset HC and the begining of cut */
          reset_counters = true;

        }


        if ( !old_times )
        {

          if ( time_2 >= time_max ) time_2 = total_time;
          else time_2 = time_2 - time_min;

          if ( time_1 <= time_min ) time_1 = 0;
          else time_1 = time_1 - time_min;
        }
        tasks[appl-1][task-1][thread-1]->last_time = time_2;


        if ( !first_time_caught )
        {
          first_record_time = time_1;
          first_time_caught = true;
        }
        else
        {
          if ( time_1 < first_record_time )
            first_record_time = time_1;
        }


        last_record_time = time_2;


        current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%lld:%d\n", id, cpu, appl, task, thread, time_1, time_2, state );


        if ( reset_counters )
        {
          reset_counters = false;
          sprintf( line, "2:%d:%d:%d:%d:%lld", cpu, appl, task, thread, time_1 );

          for ( i = 0; i < last_counter; i++ )
            sprintf( line, "%s:%lld:0", line, counters[i] );

          if ( i > 0 )
            current_size += fprintf( outfile, "%s\n", line );
        }



        break;



      case '2':
        sscanf( line, "%d:%d:%d:%d:%d:%lld:%s\n", &id, &cpu, &appl, &task, &thread, &time_1, buffer );
        strcpy( line, buffer );


        /* If isn't a traceable thread, get next record */
        if ( cut_tasks && !is_selected_task( task ) ) break;


        /* If time out of the cut, exit */
        /*                  if(time_1 > time_max && !old_times) {
                                fclose(infile); fclose(outfile);
                                if(is_zip) {
                                        sprintf(line,"rm %s/tmp.prv",tmp_dir);
                                        system(line);
                                }

                                printf("...Done\n\n");
                                exit(0);
                          }
        */
        if ( ( tasks[appl-1][task-1][thread-1] != NULL  ) &&
             ( time_1 > tasks[appl-1][task-1][thread-1]->last_time ) &&
             ( time_1 > time_max ) ) break;

        if ( tasks[appl-1][task-1][thread-1] == NULL && time_1 > time_max ) break;

        if ( tasks[appl-1][task-1][thread-1] == NULL && remFirstStates ) break;


        /* If time inside cut, adjust time */
        if ( time_1 >= time_min )
        {
          if ( !old_times ) time_1 = time_1 - time_min;


          last_record_time = time_1;
          current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%s\n", id, cpu, appl, task, thread, time_1, line );


          /* For closing all the opened calls */
          /* Event type and values */
          end_line = false;
          word = strtok( line, ":" );
          type = atoll( word );
          word = strtok( NULL, ":" );
          value = atoll( word );

          update_queue( appl - 1, task - 1, thread - 1, type, value );

          while ( !end_line )
          {

            if ( ( word = strtok( NULL, ":" ) ) != NULL )
            {
              type = atoll( word );
              word = strtok( NULL, ":" );
              value = atoll( word );
              update_queue( appl - 1, task - 1, thread - 1, type, value );
            }
            else end_line = true;
          }


        }
        break;



      case '3':
        sscanf( line, "%d:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%d:%d\n", &id, &cpu, &appl, &task, &thread, &time_1, &time_2, &cpu_2, &appl_2, &task_2, &thread_2, &time_3, &time_4, &size, &tag );


        /* If isn't a traceable thread, get next record */
        if ( cut_tasks && !is_selected_task( task ) && !is_selected_task( task_2 ) ) break;

        if ( time_1 >= time_min )
        {

          /* if time outside the cut, finish */
          if ( time_1 > time_max && !old_times )
          {
            fclose( outfile );
            if ( !is_zip )
              fclose( infile );
            else
              gzclose( gzInfile );

//            ok_cutter_wait_window();
            return;
          }


          if ( time_4 <= time_max && time_2 <= time_max )
          {

            if ( !old_times )
            {
              time_1 = time_1 - time_min;
              time_2 = time_2 - time_min;
              time_3 = time_3 - time_min;
              time_4 = time_4 - time_min;
            }

            last_record_time = time_3;

            current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%d:%d\n", id, cpu, appl, task, thread, time_1, time_2, cpu_2, appl_2, task_2, thread_2, time_3, time_4, size, tag );


          }
        }
        break;



      case '4':
        sscanf( line, "%d:%d:%d:%d:%d:%lld:%s\n", &id, &cpu, &appl, &task, &thread, &time_1, buffer );
        strcpy( line, buffer );


        /* If time out of the cut, exit */
        if ( time_1 > time_max && !old_times )
        {
          fclose( outfile );
          if ( !is_zip )
            fclose( infile );
          else
            gzclose( gzInfile );

//          ok_cutter_wait_window();
          return;
        }


        if ( old_times ) current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%s", id, cpu, appl, task, thread, time_1, line );
        else
        {
          if ( time_1 >= time_min && time_1 <= time_max )
          {
            time_1 = time_1 - time_min;
            current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%s", id, cpu, appl, task, thread, time_1, line );
          }
        }
        break;
    }

    if ( max_size > 0 )
      if ( max_size <= current_size )
        break;


    if ( init_task_counter && useful_tasks == 0 ) break;
  }


  if ( !break_states ) old_times = false;

  if ( old_times )
  {
    adjust_to_final_time();
  }


  /* Close the files */
  fclose( outfile );
  if ( !is_zip )
    fclose( infile );
  else
    gzclose( gzInfile );


  if ( !break_states )
  {
    shift_trace_to_zero( trace_file_out, trace_out );
  }

//  ok_cutter_wait_window();
//  cout << "<------------------------- KTraceCutter::execute" << endl;

}
