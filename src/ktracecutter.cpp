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
#include <algorithm>
#include <vector>
#include <string>

#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include <zlib.h>

#include "ktracecutter.h"
#include "kprogresscontroller.h"
#include "tracestream.h" // for GZIP_COMPRESSION_RATIO
#include "paraverconfig.h"

#ifdef WIN32
#define atoll _atoi64
#endif

#include <iostream>
using namespace std;

KTraceCutter::KTraceCutter( char *&trace_in,
                            char *&trace_out,
                            TraceOptions *options,
                            const vector< TEventType > &whichTypesWithValuesZero,
                            ProgressController *progress )
{
  line = (char *) malloc( sizeof( char ) * MAX_TRACE_HEADER );
  total_cutter_iters = 0;
  exec_options = new KTraceOptions( (KTraceOptions *) options );
  PCFEventTypesWithValuesZero.insert( whichTypesWithValuesZero.begin(), whichTypesWithValuesZero.end() );
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

void KTraceCutter::set_keep_events( bool keepEvents )
{
}


/* Function for parsing program parameters */
void KTraceCutter::read_cutter_params()
{
  char *word, *buffer;

  by_time = exec_options->by_time;
  time_min = exec_options->min_cutting_time;
  time_max = exec_options->max_cutting_time;
  total_time = time_max - time_min;
  min_perc = exec_options->min_percentage;
  max_perc = exec_options->max_percentage;
  originalTime = exec_options->original_time;
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
      }
      else
      {
        wanted_tasks[j].min_task_id = atoll( word );
        wanted_tasks[j].range = 0;
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
  keep_events = exec_options->keep_events;

  if ( originalTime )
    break_states = false; // Assigned ONLY if originalTime == true, else KEEP value.
}


/* For processing the Paraver header */
void KTraceCutter::proces_cutter_header( char *header,
                                         char *trace_in_name,
                                         char *trace_out_name,
                                         bool is_zip )
{
  int num_comms;
  char *word;
  // char auxLine[ MAX_TRACE_HEADER + 1 ];
  char *auxLine = (char *) malloc( sizeof( char ) * MAX_TRACE_HEADER );

  // PARSE variable header
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

    if ( !originalTime )
    {
      // because time_min -> 0
      current_size += fprintf( outfile, "%lld_ns:", total_time );
    }
    else
    {
      current_size += fprintf( outfile, "%s_ns:", word );
    }
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

    if ( !originalTime )
      current_size += fprintf( outfile, "%lld:", total_time );
    else
      current_size += fprintf( outfile, "%s:", word );
  }

  word = strtok( NULL, "\n" );
  current_size += fprintf( outfile, "%s\n", word );

  word = strrchr( word, ',' );

  /* Obtaining the number of communicators */
  strcpy( header, word + 1 );
  if ( strchr( header, ')' ) != NULL )
    return;

  // NOW READ FROM GLOBAL VARIABLE infile
  num_comms = atoi( header );
  while ( num_comms > 0 )
  {
    if ( !is_zip )
    {
      fgets( auxLine, MAX_TRACE_HEADER, infile );
    }
    else
    {
      gzgets( gzInfile, auxLine, MAX_TRACE_HEADER );
    }

    current_size += fprintf( outfile, "%s", auxLine );
    num_comms--;
  }

  /* Writing in the header the offset of the cut regard original trace */

  /* Reading first if we have old offsets into the trace */
  if ( !is_zip )
    fgets( auxLine, MAX_TRACE_HEADER, infile );
  else
    gzgets( gzInfile, auxLine, MAX_TRACE_HEADER );

  while ( auxLine[0] == '#' )
  {
    if ( !is_zip )
    {
      if( feof( infile ) )
        break;
    }
    else
    {
      if ( gzeof( gzInfile ) )
        break;
    }

    current_size += fprintf( outfile, "%s", auxLine );

    if ( !is_zip )
      fgets( auxLine, MAX_TRACE_HEADER, infile );
    else
      gzgets( gzInfile, auxLine, MAX_TRACE_HEADER );
  }

  if ( !is_zip )
#ifdef WIN32
    _fseeki64( infile, 0, SEEK_SET );
#else
    fseek( infile, -( strlen( auxLine ) ), SEEK_CUR );
#endif
  else
    gzseek( gzInfile, -( strlen( auxLine ) ), SEEK_CUR );

  /* Writing of the current cut offset */
  if ( trace_in_name != NULL )
    current_size += fprintf( outfile, "# %s: Offset %lld from %s\n", trace_out_name, time_min, trace_in_name );

  free( auxLine );
}


const set< TEventType > KTraceCutter::mergeDuplicates( const multiset< TEventType>& eventTypesWithPCFZeros )
{
  set< TEventType > uniqueEventTypes;

  for ( multiset< TEventType >::const_iterator it = eventTypesWithPCFZeros.begin();
        it != eventTypesWithPCFZeros.end(); ++it )
  {
    uniqueEventTypes.insert( *it );
  }

  return uniqueEventTypes;
}


void KTraceCutter::dumpEventsSet( const set< TEventType >& closingEventTypes,
                                  unsigned int cpu,
                                  unsigned int appl,
                                  unsigned int task,
                                  unsigned int thread,
                                  const unsigned long long final_time,
                                  int &numWrittenChars,
                                  bool &needEOL,
                                  bool &writtenComment )
{
  if ( !writtenComment )
  {
    fprintf( outfile, "# Appending events with value 0\n");
    if( writeToTmpFile ) ++total_tmp_lines;
    writtenComment = true;
  }

  for ( set< TEventType >::const_iterator it = closingEventTypes.begin(); it != closingEventTypes.end(); ++it )
  {
    if ( numWrittenChars == 0 )
    {
      // Write new line
      numWrittenChars += fprintf( outfile, "2:%d:%d:%d:%d:%lld:%lld:0",
                               cpu, appl + 1, task + 1, thread + 1,
                               final_time, (unsigned long long)*it );
      if( writeToTmpFile ) ++total_tmp_lines;

      needEOL = true;
    }
    else if ( numWrittenChars + 32 > MAX_LINE_SIZE )
    {
      // Too many events: close current line
      fprintf( outfile, "\n" );
      numWrittenChars = 0;
      needEOL = false;
    }
    else
    {
      // Append to current line
      numWrittenChars += fprintf( outfile, ":%lld:0", (unsigned long long)*it );
      needEOL = true;
    }
  }

  if( needEOL )
  {
    // Close current line
    fprintf( outfile, "\n" );  // because we know theres's one at least.
    numWrittenChars = 0;
    needEOL = false;
  }
}


void KTraceCutter::appendLastZerosToUnclosedEvents( const unsigned long long final_time )
{
  TCPUOrder cpu;
  int numWrittenChars = 0;
  bool needEOL = false;
  bool writtenComment = false;
  set< TEventType > currentEventTypesWithPCFZeros;

  for( unsigned int appl = 0; appl < MAX_APPL; ++appl )
  {
    for( unsigned int task = 0; task < MAX_TASK; ++task )
    {
      for( unsigned int thread = 0; thread < MAX_THREAD; ++thread )
      {
        if ( tasks[appl][task][thread] != NULL )
        {
          cpu = tasks[appl][task][thread]->lastCPU;

          numWrittenChars = 0;
          needEOL = false;

          if( tasks[appl][task][thread]->eventTypesWithoutPCFZeros.size() > 0 )
          {
            dumpEventsSet( tasks[appl][task][thread]->eventTypesWithoutPCFZeros,
                           cpu, appl, task, thread,
                           final_time,
                           numWrittenChars,
                           needEOL,
                           writtenComment );
          }

          if( tasks[appl][task][thread]->eventTypesWithPCFZeros.size() > 0 )
          {

            // Avoid long queues of zeros with same event type
            currentEventTypesWithPCFZeros = mergeDuplicates( tasks[appl][task][thread]->eventTypesWithPCFZeros );

            dumpEventsSet( currentEventTypesWithPCFZeros,
                           cpu, appl, task, thread,
                           final_time,
                           numWrittenChars,
                           needEOL,
                           writtenComment );
          }
        }
      }
    }
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

  /* Depen mida traça mostrem percentatge amb un interval diferent de temps */
  if ( total_trace_size < 500000000 )
    total_cutter_iters = 10000;
  else
    total_cutter_iters = 100000;

  current_read_size = 0;

  if( progress != NULL)
  {
    if( writeToTmpFile )
    {
      progress->setEndLimit( total_trace_size * 2 );
    }
    else
      progress->setEndLimit( total_trace_size );
  }
}


void KTraceCutter::show_cutter_progress_bar( ProgressController *progress )
{
//  double current_showed, i, j;

  if( !secondPhase )
  {
#if defined(__FreeBSD__) || defined(__APPLE__)
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
  }

/*  i = ( double )( current_read_size );
  j = ( double )( total_trace_size );

  current_showed = i / j;*/

  if ( is_zip )
    current_read_size = current_read_size / TraceStream::GZIP_COMPRESSION_RATIO;

  if ( progress != NULL )
  {
    if( writeToTmpFile )
    {
      if( !secondPhase )
      {
        progress->setCurrentProgress( current_read_size );
      }
      else
      {
        if( current_tmp_lines % 10000 == 0 )
        {
          double tmpPerc;
          if ( total_tmp_lines == 0 )
          {
            tmpPerc = 0;
          }
          else
          {
            tmpPerc = (double)current_tmp_lines / (double)total_tmp_lines;
          }

          double tmpSize = progress->getEndLimit() / 2.0;
          progress->setCurrentProgress( tmpSize + tmpSize * tmpPerc );
        }
      }
    }
    else
      progress->setCurrentProgress( current_read_size );
  }
}


void KTraceCutter::update_queue( int appl, int task, int thread,
                                 unsigned long long type,
                                 unsigned long long value )
{
  thread_info *p;


  if ( tasks[appl][task][thread] == NULL )
  {
    if (( p = new thread_info() ) == NULL )
    {
      perror( "No more memory!!!\n" );
      exit( 1 );
    }

    tasks[appl][task][thread] = p;
    tasks[appl][task][thread]->last_time = 0;
    tasks[appl][task][thread]->lastCPU = 0;
    tasks[appl][task][thread]->finished = false;

    tasks[appl][task][thread]->eventTypesWithoutPCFZeros.clear();
    tasks[appl][task][thread]->eventTypesWithPCFZeros.clear();

    num_tasks++;
    useful_tasks++;
    init_task_counter = 1;
  }

  if ( value > 0 )
  {
    if ( PCFEventTypesWithValuesZero.find( type ) != PCFEventTypesWithValuesZero.end() )
    {
      tasks[appl][task][thread]->eventTypesWithPCFZeros.insert( (TEventType)type );
    }
    else
    {
      tasks[appl][task][thread]->eventTypesWithoutPCFZeros.insert( (TEventType)type );
    }
  }
  else
  {
    if( tasks[appl][task][thread]->eventTypesWithPCFZeros.size() > 0)
    {
      multiset< TEventType >::const_iterator it = tasks[appl][task][thread]->eventTypesWithPCFZeros.find( (TEventType)type );
      if ( it != tasks[appl][task][thread]->eventTypesWithPCFZeros.end() )
      {
        tasks[appl][task][thread]->eventTypesWithPCFZeros.erase( it );
      }
    }
  }
}


void KTraceCutter::load_counters_of_pcf( char *trace_name )
{
  string pcf_name;
  FILE *pcf;

  pcf_name = LocalKernel::composeName( string( trace_name ), string( "pcf" ) );

  last_counter = 0;
  if ( ( pcf = fopen( pcf_name.c_str(), "r" ) ) == NULL )
    return;

  fclose( pcf );

  // Assumed counters
  for( set< TEventType >::iterator it = PCFEventTypesWithValuesZero.begin(); it != PCFEventTypesWithValuesZero.end(); ++it )
  {
    counters[last_counter] = (unsigned long long)(*it);
    last_counter++;

    if ( last_counter == 500 )
    {
      printf( "NO more memory for loading counters of .pcf\n" );
      break;
    }
  }
}

// Substract to all the times in the trace the first time of the first record
// Doesn't change header
void KTraceCutter::shiftLeft_TraceTimes_ToStartFromZero( char *nameIn, char *nameOut, bool is_zip, ProgressController *progress )
{
  unsigned long long timeOffset = 0, time_1, time_2, time_3, time_4;
  int cpu, appl, task, thread, state, cpu_2, appl_2, task_2, thread_2;
  char *trace_header;

  if ( !is_zip )
  {
#if defined(__FreeBSD__) || defined(__APPLE__)
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
  }
  else
  {
    if ( ( gzInfile = gzopen( nameIn, "rb" ) ) == NULL )
    {
      printf( "KCutter: Error opening compressed trace\n" );
      exit( 1 );
    }
  }

#if defined(__FreeBSD__) || defined(__APPLE__)
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
  if ( !is_zip )
  {
    fgets( trace_header, MAX_TRACE_HEADER, infile );
  }
  else
  {
    gzgets( gzInfile, trace_header, MAX_TRACE_HEADER );
  }

  // Consume header
  proces_cutter_header( trace_header, NULL, NULL, is_zip );

  bool end_read = false;

#ifdef WIN32
  if ( !is_zip )
  {
    fgets( trace_header, MAX_TRACE_HEADER, infile );
  }
  else
  {
    gzgets( gzInfile, trace_header, MAX_TRACE_HEADER );
  }

  while( !( trace_header[0] == '1' || trace_header[0] == '2' || trace_header[0] == '3' ) )
  {
    if ( !is_zip )
    {
      fgets( trace_header, MAX_TRACE_HEADER, infile );
    }
    else
    {
      gzgets( gzInfile, trace_header, MAX_TRACE_HEADER );
    }
  }
#else
  /* Read one more record if possible */
  if ( !is_zip )
  {
    if ( feof( infile ) )
      end_read = true;
    else
    {
      char *auxLine = fgets( trace_header, MAX_TRACE_HEADER, infile );
      if ( auxLine == NULL )
        end_read = true;
    }
  }
  else
  {
    if ( gzeof( gzInfile ) )
      end_read = true;
    else
    {
      char *auxLine = gzgets( gzInfile, trace_header, MAX_TRACE_HEADER );
      if ( auxLine == Z_NULL )
        end_read = true;
    }
  }
#endif

  // Get time of the first record ignoring any other field.
  if (!end_read)
    sscanf( trace_header, "%*d:%*d:%*d:%*d:%*d:%lld:", &timeOffset );

  // Override it: we have the minimum time of the written records.
  timeOffset = first_record_time;
  current_tmp_lines = 0;

  while ( !end_read )
  {
  //cout << trace_header << endl;
    show_cutter_progress_bar( progress );

    switch ( trace_header[0] )
    {
      case '1':
        sscanf( trace_header, "%*d:%d:%d:%d:%d:%lld:%lld:%d\n", &cpu, &appl, &task, &thread, &time_1, &time_2, &state );


        time_1 = time_1 - timeOffset;
        time_2 = time_2 - timeOffset;

        fprintf( outfile, "1:%d:%d:%d:%d:%lld:%lld:%d\n", cpu, appl, task, thread, time_1, time_2, state );
        ++current_tmp_lines;
        break;


      case '2':
        sscanf( trace_header, "%*d:%d:%d:%d:%d:%lld:%s\n", &cpu, &appl, &task, &thread, &time_1, line );

        time_1 = time_1 - timeOffset;

        fprintf( outfile, "2:%d:%d:%d:%d:%lld:%s\n", cpu, appl, task, thread, time_1, line );
        ++current_tmp_lines;
        break;

      case '3':
        sscanf( trace_header, "%*d:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%s\n",
                &cpu,   &appl,   &task,   &thread,   &time_1, &time_2,
                &cpu_2, &appl_2, &task_2, &thread_2, &time_3, &time_4, line );

        time_1 = time_1 - timeOffset;
        time_2 = time_2 - timeOffset;
        time_3 = time_3 - timeOffset;
        time_4 = time_4 - timeOffset;

        fprintf( outfile, "3:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%s\n",
                 cpu,   appl,   task,   thread,   time_1, time_2,
                 cpu_2, appl_2, task_2, thread_2, time_3, time_4, line );
        ++current_tmp_lines;
        break;

      case '4':
        sscanf( trace_header, "%s\n", line );
        fprintf( outfile, "%s\n", line );
        ++current_tmp_lines;
        break;

      case '#':
        sscanf( trace_header, "%s\n", line );
        if ( string( line ).compare( string( " Appending events with value 0" ) ) == 0 )
        {
          fprintf( outfile, "%s\n", line );
          ++current_tmp_lines;
        }
        break;

      default:
        break;
    }

    /* Read one more record if possible */
    if ( !is_zip )
    {
      if ( feof( infile ) )
        end_read = true;
      else
      {
        char *auxLine = fgets( trace_header, MAX_TRACE_HEADER, infile );
        if ( auxLine == NULL )
          end_read = true;
      }
    }
    else
    {
      if ( gzeof( gzInfile ) )
        end_read = true;
      else
      {
        char *auxLine = gzgets( gzInfile, trace_header, MAX_TRACE_HEADER );
        if ( auxLine == Z_NULL )
          end_read = true;
      }
    }
  }

  if ( !is_zip )
  {
    fclose( infile );
  }
  else
  {
    gzclose( gzInfile );
  }

  fclose( outfile );
  unlink( nameIn );
}

/* Function for filtering tasks in cut */
bool KTraceCutter::is_selected_task( int task_id )
{
  int i;

  for ( i = 0; i < MAX_SELECTED_TASKS; i++ )
  {
    if ( wanted_tasks[i].min_task_id == 0 )
      break;

    if ( wanted_tasks[i].range )
    {
      if ( task_id >= wanted_tasks[i].min_task_id && task_id <= wanted_tasks[i].max_task_id )
        return true;
    }
    else
      if ( task_id == wanted_tasks[i].min_task_id )
        return true;
  }

  return false;
}


void KTraceCutter::execute( char *trace_in,
                            char *trace_out,
                            ProgressController *progress )
{
  char *c, *word, *trace_header;
  string tmp_dir;
  //char trace_name[1024], buffer[MAX_LINE_SIZE];
  char *trace_name;
  char *buffer;
  bool end_parsing = false;
  bool reset_counters;
  //char trace_file_out[2048];
  char *trace_file_out;

  unsigned int id, cpu, appl, task, thread, state, cpu_2, appl_2, task_2, thread_2, size, tag;
  unsigned long long type, value, time_1, time_2, time_3, time_4;
  int i, j, k;
  bool end_line;

  unsigned long num_iters = 0;
  thread_info *p;

  trace_name     = (char *) malloc( sizeof(char) * MAX_FILENAME_SIZE );
  trace_file_out = (char *) malloc( sizeof(char) * MAX_FILENAME_SIZE );
  buffer         = (char *) malloc( sizeof(char) * MAX_LINE_SIZE );

  /* Ini Data */
  for ( i = 0;i < MAX_APPL;i++ )
    for ( j = 0;j < MAX_TASK;j++ )
      for ( k = 0;k < MAX_THREAD;k++ )
        tasks[i][j][k] = NULL;

  by_time = false;
  originalTime = false;
  max_size = 0;
  cut_tasks = false;
  break_states = true;
  is_zip = false;
  init_task_counter = 0;
  useful_tasks = 0;
  first_time_caught = false;
  num_tasks = 0;
  current_size = 0;

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
#if defined(__FreeBSD__) || defined(__APPLE__)
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

  //bool writeToTmpFile = !originalTime;
  writeToTmpFile = break_states || !originalTime;

  if ( writeToTmpFile )
  {
    tmp_dir = ParaverConfig::getInstance()->getGlobalTmpPath();

#ifdef WIN32
    sprintf( trace_file_out, "%s\\tmp_fileXXXXXX", tmp_dir.c_str() );
    _mktemp_s( trace_file_out, strlen( trace_file_out ) + 1 );
#else
    sprintf( trace_file_out, "%s/tmp_fileXXXXXX", tmp_dir.c_str() );
    mkstemp( trace_file_out );
#endif
  }
  else
    strcpy( trace_file_out, trace_out );

#if defined(__FreeBSD__) || defined(__APPLE__)
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

  ini_cutter_progress_bar( trace_name, progress );

  /* Process header */
  trace_header = ( char * )malloc( sizeof( char ) * MAX_TRACE_HEADER );
  if ( !is_zip )
    fgets( trace_header, MAX_TRACE_HEADER, infile );
  else
  {
    gzgets( gzInfile, trace_header, MAX_TRACE_HEADER );
  }

  proces_cutter_header( trace_header, trace_in, trace_out, is_zip );
  free( trace_header );

  /* We process the trace like the originalTime version */

  bool maxTimeReached = false;
  last_record_time = 0;
  total_tmp_lines = 0;
  secondPhase = false;

  if( progress != NULL )
    end_parsing = progress->getStop();

  /* Processing the trace records */
  while ( !end_parsing && !maxTimeReached )
  {
    if( progress != NULL )
      end_parsing = progress->getStop();

    /* Read one more record is possible */
    if ( !is_zip )
    {
      if ( feof( infile ) || fgets( line, MAX_TRACE_HEADER, infile ) == NULL )
      {
        end_parsing = true;

        continue;
      }
    }
    else
    {
      if ( gzeof( gzInfile ) || gzgets( gzInfile, line, MAX_TRACE_HEADER ) == Z_NULL )
      {
        end_parsing = true;
        continue;
      }
    }

    if ( num_iters == total_cutter_iters )
    {
      show_cutter_progress_bar( progress );
      num_iters = 0;
    }
    else
      num_iters++;

    /* 1: state; 2: event; 3: comm; #: comment in trace */
    /* DEPRECATED 4: global comm */
    switch ( line[0] )
    {
      case '1':
        sscanf( line, "%d:%d:%d:%d:%d:%lld:%lld:%d\n",
                &id, &cpu, &appl, &task, &thread, &time_1, &time_2, &state );

        /* If is a not traceable thread, get next record */
        if ( cut_tasks && !is_selected_task( task ) )
          break;

        if ( time_2 <= time_min )
          break;

        if ( time_1 < time_min && time_2 >= time_min && remFirstStates )
          break;

        if ( time_1 < time_max && time_2 > time_max && remLastStates )
          break;

        if ( time_1 < time_min && time_2 > time_max && ( remFirstStates || remLastStates ))
          break;

        if ( originalTime && time_1 > time_max )
        {
          if ( tasks[appl-1][task-1][thread-1] != NULL &&
               tasks[appl-1][task-1][thread-1]->finished )
          {
            useful_tasks--;
            tasks[appl-1][task-1][thread-1]->finished = false;
          }

          break;
        }

        if ( !originalTime && time_1 > time_max )
        {
          maxTimeReached = true;
        }
        else // originalTime || time_1 <= time_max
        {
          if ( tasks[appl-1][task-1][thread-1] == NULL )
          {
            if (( p = new thread_info() ) == NULL )
            {
              perror( "No more memory!!!\n" );
              exit( 1 );
            }

            tasks[appl-1][task-1][thread-1] = p;
            num_tasks++;
            useful_tasks++;
            init_task_counter = 1;
            p->finished = true;
            p->lastCPU = cpu;
            p->last_time = 0;

            p->eventTypesWithoutPCFZeros.clear();
            p->eventTypesWithPCFZeros.clear();

            /* Have to reset HC and the begining of cut */
            reset_counters = true;
          }

          if ( !originalTime )
          {
            if ( time_1 < time_min )
            {
              if ( break_states )
              {
                time_1 = time_min;
              }
            }

            if ( time_2 > time_max )
            {
              if ( break_states )
              {
                time_2 = time_max;
              }
            }
          }
/*
          if ( !originalTime )
          {
            if ( time_1 < time_min )
            {
              if ( break_states )
              {
                time_1 = 0;
              }
              else
              {
                // NEW MIN TIME!
                time_min = time_1;
                total_time = time_max - time_min;
                time_1 = 0;
              }
            }
            else
            {
              time_1 = time_1 - time_min;
            }

            if ( time_2 >= time_max )
            {
              if ( break_states )
              {
                time_2 = total_time;
              }
              else
              {
                time_2 = time_2 - time_min;
              }
            }
            else
            {
              time_2 = time_2 - time_min;
            }
          }
*/
          //if ( tasks[appl-1][task-1][thread-1]->last_time < time_2 )
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

          if ( time_2 > last_record_time )
             last_record_time = time_2;

          current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%lld:%d\n",
                                   id, cpu, appl, task, thread, time_1, time_2, state );
          if( writeToTmpFile ) ++total_tmp_lines;

          if ( reset_counters )
          {
            reset_counters = false;
            sprintf( line, "2:%d:%d:%d:%d:%lld", cpu, appl, task, thread, time_1 );

            for ( i = 0; i < last_counter; i++ )
              sprintf( line, "%s:%lld:0", line, counters[i] );

            if ( i > 0 )
            {
              current_size += fprintf( outfile, "%s\n", line );
              if( writeToTmpFile ) ++total_tmp_lines;
            }
          }
        }

        break;

      case '2':
        sscanf( line, "%d:%d:%d:%d:%d:%lld:%s\n", &id, &cpu, &appl, &task, &thread, &time_1, buffer );
        strcpy( line, buffer );

        /* If isn't a traceable thread, get next record */
        if( cut_tasks && !is_selected_task( task ) )
          break;

        if ( ( tasks[appl-1][task-1][thread-1] != NULL ) &&
             ( time_1 > tasks[appl-1][task-1][thread-1]->last_time ) &&
             ( time_1 > time_max ) &&
             !keep_events )
          break;

        if ( tasks[appl-1][task-1][thread-1] == NULL && time_1 > time_max )
          break;

        if ( tasks[appl-1][task-1][thread-1] == NULL && remFirstStates )
          break; // ?

        if ( time_1 > time_max )
          break;

        /* If time inside cut, adjust time */
        if ( time_1 >= time_min ||
             ( time_1 < time_min &&
               tasks[appl-1][task-1][thread-1] != NULL &&
               tasks[appl-1][task-1][thread-1]->last_time >= time_min &&
               keep_events )
           )
        {
      //    if ( !originalTime )
      //      time_1 = time_1 - time_min;

          //last_record_time = time_1;

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

          if ( time_1 > last_record_time )
             last_record_time = time_1;

          current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%s\n",
                                   id, cpu, appl, task, thread, time_1, line );
          if( writeToTmpFile ) ++total_tmp_lines;

          /* For closing all the opened calls */
          end_line = false;
          word = strtok( line, ":" );
          type = atoll( word );
          word = strtok( NULL, ":" );
          value = atoll( word );

          update_queue( appl - 1, task - 1, thread - 1, type, value );

          tasks[appl - 1 ][task - 1 ][thread - 1 ]->last_time = time_1;
          tasks[appl - 1 ][task - 1 ][thread - 1 ]->lastCPU = cpu;

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
        sscanf( line, "%d:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%d:%d\n",
                &id,
                &cpu,   &appl,   &task,   &thread,   &time_1, &time_2,
                &cpu_2, &appl_2, &task_2, &thread_2, &time_3, &time_4, &size, &tag );

        /* If isn't a traceable thread, get next record */
        if ( cut_tasks && !is_selected_task( task ) && !is_selected_task( task_2 ) )
          break;

        if ( time_1 >= time_min )
        {
          /* if time outside the cut, finish */
          if ( time_1 > time_max && !originalTime ) // PROBLEM: DISORDERED TRACES
          {
            maxTimeReached = true;
          }
          else if ( time_4 <= time_max && time_2 <= time_max )
          {
        /*    if ( !originalTime )
            {
              time_1 = time_1 - time_min;
              time_2 = time_2 - time_min;
              time_3 = time_3 - time_min;
              time_4 = time_4 - time_min;
            }
*/
            // last_record_time = time_3;
            vector< unsigned long long > times;

            times.push_back( time_1 );
            times.push_back( time_2 );
            times.push_back( time_3 );
            times.push_back( time_4 );
            sort( times.begin(), times.end() );

            if ( !first_time_caught )
            {
              first_record_time = times[0];
              first_time_caught = true;
            }
            else
            {
              if ( times[0] < first_record_time )
                first_record_time = times[0];
            }

            if ( times[3] > last_record_time )
              last_record_time = times[3];

            current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%d:%d\n",
                                     id,
                                     cpu,   appl,   task,   thread,   time_1, time_2,
                                     cpu_2, appl_2, task_2, thread_2, time_3, time_4, size, tag );
            if( writeToTmpFile ) ++total_tmp_lines;
          }
        }

        break;

      case '4': // DEPRECATED
        sscanf( line, "%d:%d:%d:%d:%d:%lld:%s\n", &id, &cpu, &appl, &task, &thread, &time_1, buffer );
        strcpy( line, buffer );

        /* If time out of the cut, exit */
        if ( time_1 > time_max && !originalTime )
        {
          maxTimeReached = true;
        }
        else
        {
  /*
          if ( originalTime )
            current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%s\n", id, cpu, appl, task, thread, time_1, line );
          else
          {
            if ( time_1 >= time_min && time_1 <= time_max )
            {
              time_1 = time_1 - time_min;
              current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%s\n", id, cpu, appl, task, thread, time_1, line );
            }
          }
  */
          if ( time_1 >= time_min && time_1 <= time_max )
          {
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

            if ( time_1 > last_record_time )
              last_record_time = time_1;

            current_size += fprintf( outfile, "%d:%d:%d:%d:%d:%lld:%s\n", id, cpu, appl, task, thread, time_1, line );
            if( writeToTmpFile ) ++total_tmp_lines;
          }
        }

        break;

      case '#':
        sscanf( line, "%s\n", buffer );
#ifdef WIN32
        if( strcmp( buffer, "#Paraver" ) == 0)
          break;
#endif
        current_size += fprintf( outfile, "%s\n", buffer );
        if( writeToTmpFile ) ++total_tmp_lines;

        break;

      default:
        break;
    }

    if ( max_size > 0 )
      if ( max_size <= current_size )
        break;

    if ( init_task_counter && useful_tasks == 0 )
      break;
  }

  if( progress != NULL && progress->getStop() )
    progress->setMessage( "Finishing cut..." );

  if ( last_record_time > time_max )
  //if ( !originalTime )
    appendLastZerosToUnclosedEvents( last_record_time );
  else
    appendLastZerosToUnclosedEvents( time_max );

  /* Close the files */
  fclose( outfile );
  if ( !is_zip )
    fclose( infile );
  else
    gzclose( gzInfile );

  if ( writeToTmpFile )   // trace_file_out is a tmpfile!!
  {
    secondPhase = true;
    shiftLeft_TraceTimes_ToStartFromZero( trace_file_out, trace_out, false, progress );
  }

  free( trace_name );
  free( trace_file_out );
  free( buffer );
}
