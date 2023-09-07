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

#include <errno.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <sstream>

#include "ktracesoftwarecounters.h"
#include "ktraceoptions.h"
#include "kprogresscontroller.h"
#include "paraverconfig.h"

#include "utils/traceparser/processmodel.h"
#include "utils/traceparser/resourcemodel.h"
#include "utils/traceparser/tracebodyio_v1.h"
#include "utils/traceparser/traceheader.h"

#ifdef _WIN32
#define atoll _atoi64
#endif

template <typename T, typename... Targs>
constexpr void dump_fields( std::fstream& file, T current_field )
{
  file << current_field;
}

template <typename T, typename... Targs>
constexpr void dump_fields( std::fstream& file, T current_field, Targs... Fargs )
{
  file << current_field << ":";
  dump_fields( file, Fargs... );
}

KTraceSoftwareCounters::KTraceSoftwareCounters( char *trace_in,
                                                char *trace_out,
                                                TraceOptions *options,
                                                ProgressController *progress )
{
  min_state_time = 0;
  type_of_counters = false;
  keep_events = false;
  total_iters = 0;

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
  int i, k;
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
    while ( ( words[i] = strtok( nullptr, ";" ) ) != nullptr )
      i++;

    k = 0;
    while ( k < i )
    {
      auto itCurrentType = allowed_types.insert( allowed_types.end(), type_values() );
      word_type = strtok( words[k], ":" );
      itCurrentType->type = atoll( word_type );
      if ( ( word_values = strtok( nullptr, ":" ) ) == nullptr )
      {
        itCurrentType->all_values = true;
      }
      else
      {
        word_value = strtok( word_values, "," );
        itCurrentType->all_values = false;
        itCurrentType->values.push_back( atoll( word_value ) );
        while ( ( word_value = strtok( nullptr, "," ) ) != nullptr )
        {
          itCurrentType->values.push_back( atoll( word_value ) );
        }
      }
      ++k;
    }

    free( exec_options->types );
  }

  if ( strlen( exec_options->types_kept ) > 0 )
  {
    keep_events = true;
    words[0] = strtok( exec_options->types_kept, ";" );
    keep_types.push_back( atoll( words[0] ) );

    while ( ( words[0] = strtok( nullptr, ";" ) ) != nullptr )
    {
      keep_types.push_back( atoll( words[0] ) );
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
void KTraceSoftwareCounters::parseInHeaderAndDumpOut()
{
  ProcessModel<> traceProcessModel = ProcessModel<>( );
  ResourceModel<> traceResourceModel = ResourceModel<>( );
  unsigned long long traceEndTime = 0;
  TTimeUnit traceTimeUnit = NS;
  std::vector< std::string > communicators;
  std::string tmpDate;

  try
  {
    parseTraceHeader( *infile, tmpDate, traceTimeUnit, traceEndTime, traceResourceModel, traceProcessModel, communicators );
  }
  catch( TraceHeaderException& e )
  {
    throw e;
  }

  trace_time = traceEndTime;

  // Dump header in outfile
  dumpTraceHeader( outfile, tmpDate, traceEndTime, traceTimeUnit, traceResourceModel, traceProcessModel, communicators );
}


/* For copy .pcf and add the counter types and values */
void KTraceSoftwareCounters::write_pcf( char *file_out )
{
  char *c, *trace_out;
  std::fstream pcf;

  trace_out = strdup( file_out );

  c = strrchr( trace_out, '.' );
  *c = '\0';

  line = trace_out;
  line += ".pcf";

  pcf.open( line, std::ios_base::out );
  if ( !pcf.is_open() )
    return;

  pcf << "\n\nEVENT_TYPE\n";
  pcf << "9   15001059    MPI_Waitany_counter\n";
  pcf << "9   15001004    MPI_Irecv_counter\n";
  pcf << "9   15001003    MPI_Isend_counter\n";
  pcf << "9   15001002    MPI_Recv_counter\n";
  pcf << "9   15001001    MPI_Send_counter\n";
  pcf << "9   15001041    MPI_Sendrecv_counter\n";
  pcf << "9   15001006    MPI_Waitall_counter\n";
  pcf << "9   15001005    MPI_Wait_counter\n";
  pcf << "9   15001062    MPI_Iprobe_counter\n";
  pcf << "9   15002010    MPI_Allreduce_counter\n";
  pcf << "9   15002007    MPI_Bcast_counter\n";
  pcf << "9   15002018    MPI_Allgatherv_counter\n";
  pcf << "9   15002013    MPI_Gather_counter\n";
  pcf << "9   15003019    MPI_Comm_rank_counter\n";
  pcf << "9   15003020    MPI_Comm_size_counter\n";
  pcf << "9   15003031    MPI_Init_counter\n";
  pcf << "9   15003032    MPI_Finalize_counter\n";
  pcf << "9      25001    MPI_Point-to-point_global_counter\n";
  pcf << "9      25002    MPI_Collective_comm_global_counter\n";
  pcf << "9      25003    MPI_Other_global_counter\n";

  if ( summarize_bursts )
    pcf << "9          1    Total_burst_time\n";

  pcf.close();
}


bool KTraceSoftwareCounters::allowed_type( unsigned long long type, unsigned long long value )
{
  int i;

  if ( value == 0 )
    return false;

  auto itAllowed = std::find_if( allowed_types.begin(), allowed_types.end(),
                                 [type, value]( auto el )
                                 {
                                   if( el.type == type )
                                   {
                                     if( el.all_values )
                                       return true;
                                     auto itValue = std::find( el.values.begin(), el.values.end(), value );
                                     return itValue != el.values.end();
                                   }
                                   return false;
                                 } );

  return itAllowed != allowed_types.end();
}


/* Increment of a counter for a given type and value */
void KTraceSoftwareCounters::inc_counter( int appl, int task, int thread, unsigned long long type, unsigned long long value )
{
  if( threadsInfo.find( appl, task, thread ) == threadsInfo.end() )
  {
    ThreadInfo newThreadInfo;
    newThreadInfo.appl = appl;
    newThreadInfo.task = task;
    newThreadInfo.thread = thread;
    threadsInfo( appl, task, thread ) = newThreadInfo;
  }

  if ( ( all_types && value > 0 ) || allowed_type( type, value ) )
  {
    /* Searching of the specified counter for the given thread */
    auto itCounter = threadsInfo( appl, task, thread ).counters.begin();
    for ( ; itCounter != threadsInfo( appl, task, thread ).counters.end(); ++itCounter )
    {
      if ( itCounter->type == type && ( itCounter->value == value || global_counters || acumm_values ) )
      {
        if ( !acumm_values )
          ++itCounter->num;
        else
        {
          itCounter->num += value;
        }
        break;
      }
    }

    /* The counter doesn't exist. Create it */
    if ( itCounter == threadsInfo( appl, task, thread ).counters.end() )
    {
      unsigned long long tmpNum;
      if ( !acumm_values )
        tmpNum = 1;
      else
        tmpNum = value;

      threadsInfo( appl, task, thread ).counters.emplace_back( type, value, tmpNum, false );
    }
  }
}


/* Function for putting soft counters in the trace for every specified period */
void KTraceSoftwareCounters::put_all_counters( void )
{
  unsigned long long type_mask;

  /* We pass over all the threads on the struct */
  for ( auto itThread = threadsInfo.begin(); itThread != threadsInfo.end(); ++itThread )
  {
    /* For every thread, we pass over all its counters */
    for ( auto itCounter = itThread->second.counters.begin(); itCounter != itThread->second.counters.end(); ++itCounter )
    {
      if ( acumm_values )
        type_mask = itCounter->type;
      else
      {
        if ( !global_counters )
        {
          type_mask = ( ( ( itCounter->type ) / 10000 ) + itCounter->type % 10000 + 10000 ) * 1000;
          type_mask += itCounter->value;
        }
        else
          type_mask = itCounter->type / 10000 + 20000 + itCounter->type % 10000;
      }

      dump_fields( outfile, "2:0", itThread->second.appl, itThread->second.task, itThread->second.thread, last_time, type_mask, itCounter->num );
      outfile << "\n";
    }
  }
}


void KTraceSoftwareCounters::ini_progress_bar( char *file_name, ProgressController *progress )
{
#if defined(__FreeBSD__) || (__APPLE__)
  struct stat file_info;
#elif defined(_WIN32)
  struct _stat64 file_info;
#else
  struct stat64 file_info;
#endif

#if defined(__FreeBSD__) || (__APPLE__)
  if ( stat( file_name, &file_info ) < 0 )
  {
    perror( "Error calling stat" );
    exit( 1 );
  }
#elif defined(_WIN32)
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

  /* Depen mida tra\E7a mostrem percentatge amb un interval diferent de temps */
  if ( total_trace_size < 500000000 )
    total_iters = 10000;
  else
    total_iters = 100000;

  current_read_size = 0;

  if( progress != nullptr)
    progress->setEndLimit( total_trace_size );
}


void KTraceSoftwareCounters::show_progress_bar( ProgressController *progress )
{
  current_read_size = infile->tellg();

  if( progress != nullptr)
    progress->setCurrentProgress( current_read_size );
}


void KTraceSoftwareCounters::put_counters_on_state_by_thread( int appl, int task, int thread )
{
  unsigned long long type_mask;

  auto itThread = threadsInfo.find( appl, task, thread );
  if ( itThread == threadsInfo.end() )
    return;
  auto tmpThreadInfo = itThread->second;

  /* we pass over all its counters */
  for ( auto itCounter = tmpThreadInfo.counters.begin(); itCounter != tmpThreadInfo.counters.end(); ++itCounter )
  {
    if ( acumm_values )
      type_mask = itCounter->type;
    else
    {
      if ( !global_counters )
      {
        type_mask = ( ( ( itCounter->type ) / 10000 ) + itCounter->type % 10000 + 10000 ) * 1000;
        type_mask += itCounter->value;
      }
      else
        type_mask = itCounter->type / 10000 + 20000 + itCounter->type % 10000;
    }

    dump_fields( outfile, "2:0", tmpThreadInfo.appl, tmpThreadInfo.task, tmpThreadInfo.thread, last_time, type_mask, itCounter->num );
    outfile << "\n";

    itCounter->num = 0;
  }

  tmpThreadInfo.last_time_of_sc = last_time;

  /* Put burst counters if needed */
  if ( summarize_bursts )
  {
    dump_fields( outfile, "2:0", tmpThreadInfo.appl, tmpThreadInfo.task, tmpThreadInfo.thread, last_time, tmpThreadInfo.total_burst_time );
    outfile << "\n";

    tmpThreadInfo.total_burst_time = 0;
  }
}


KTraceSoftwareCounters::SCThreadInfo::iterator KTraceSoftwareCounters::findThreadInfo( int appl, int task, int thread )
{
  auto itThread = threadsInfo.find( appl, task, thread );
  if( itThread == threadsInfo.end() )
  {
    ThreadInfo newThreadInfo;
    newThreadInfo.appl = appl;
    newThreadInfo.task = task;
    newThreadInfo.thread = thread;
    threadsInfo( appl, task, thread ) = newThreadInfo;
    itThread = threadsInfo.find( appl, task, thread );
  }
  return itThread;
};


void KTraceSoftwareCounters::sc_by_time( ProgressController *progress )
{
  int recordType, cpu, appl, task, thread, state;
  unsigned long long time_1, time_2, type, value;
  char *word;
  bool print_line = false;
  int thread_id, i, j;
  unsigned long num_iters = 0;

  bool end_parsing;

  if ( progress != nullptr )
    end_parsing = progress->getStop();
  else
    end_parsing = false;

  std::ostringstream buffer; // only for events

  while ( !infile->eof() && !end_parsing )
  {
    infile->getline( line );

    if( line[0] == '#' )
    {
      outfile << line;
      continue;
    }

    auto itBegin = line.cbegin();
    auto itEnd = line.cend();
    prv_atoll_v( itBegin, itEnd, recordType, cpu, appl, task, thread, time_1 );

    if ( progress != nullptr )
    {
      end_parsing = progress->getStop();
      if ( end_parsing )
        continue;
    }

    if ( num_iters == total_iters )
    {
      show_progress_bar( progress );
      num_iters = 0;
    }
    else
      ++num_iters;

    auto itThread = findThreadInfo( appl, task, thread );

    switch ( recordType )
    {
      case 1:
        prv_atoll_v( itBegin, itEnd, time_2, state );

        if ( state != 1 )
          break;

        itThread->second.ini_burst_time = time_1;
        itThread->second.end_burst_time = time_2;

        if ( time_2 >= ( itThread->second.last_time_of_sc + interval ) )
        {
          last_time = time_1;
          put_counters_on_state_by_thread( appl, task, thread );
        }

        /* Saving the burst time in order to summarize */
        if ( summarize_bursts )
          itThread->second.total_burst_time += ( time_2 - time_1 );

        if ( !remove_states )
        {
          dump_fields( outfile, recordType, cpu, appl, task, thread, time_1, time_2, state );
          outfile << "\n";
        }

        break;

      case 2:
        buffer.clear();
        buffer.str( "" );
        buffer << recordType << ":" << cpu << ":" << appl << ":" << task << ":" << thread << ":" << time_1;

        while ( itBegin != itEnd )
        {
          prv_atoll_v( itBegin, itEnd, type, value );

          /* For keeping some events */
          if ( keep_events )
          {
            auto itKeepType = std::find( keep_types.begin(), keep_types.end(), type );
            if( itKeepType != keep_types.end() )
            {
              buffer << ":" << type << ":" << value;
              print_line = true;
            }
          }

          /* Counting events */
          if ( only_in_bursts )
          {
            if ( time_1 > itThread->second.ini_burst_time && time_1 <= itThread->second.end_burst_time )
              inc_counter( appl, task, thread, type, value );
          }
          else
            inc_counter( appl, task, thread, type, value );
        }

        if ( print_line )
        {
          outfile << buffer.str() << "\n";
          print_line = false;
        }

        break;

      default:
        break;
    }
  }

  /* Put counters and flush events of the last interval */
  last_time = trace_time - 10;
  put_all_counters();
}


void KTraceSoftwareCounters::insert_in_queue_state( int appl, int task, int thread, unsigned long long time )
{
  auto itLastState = lastStateEndTime.find( appl, task, thread );
  if( itLastState == lastStateEndTime.end() )
  {
    KTraceSoftwareCounters::LastStateEndTime newLastState{ appl, task, thread, time };
    lastStateEndTime( appl, task, thread ) = newLastState;
    itLastState = lastStateEndTime.find( appl, task, thread );
  }
}


void KTraceSoftwareCounters::put_counters_on_state( LastStateEndTimeContainer::iterator itLastState )
{
  unsigned long long type_mask;

  auto tmpThreadInfo = threadsInfo( itLastState->second.appl, itLastState->second.task, itLastState->second.thread );
  /* For every thread, we pass over all its counters */
  for ( auto itCounter = tmpThreadInfo.counters.begin(); itCounter != tmpThreadInfo.counters.end(); ++itCounter )
  {
    if ( acumm_values )
      type_mask = itCounter->type;
    else
    {
      if ( !global_counters )
      {
        type_mask = ( ( ( itCounter->type ) / 10000 ) + itCounter->type % 10000 + 10000 ) * 1000;
        type_mask += itCounter->value;
      }
      else
        type_mask = itCounter->type / 10000 + 20000 + itCounter->type % 10000;
    }

    dump_fields( outfile, "2:0",
                          tmpThreadInfo.appl,
                          tmpThreadInfo.task,
                          tmpThreadInfo.thread,
                          itLastState->second.end_time,
                          type_mask,
                          itCounter->num );
    outfile << "\n";
    itCounter->num = 0;
  }
}

void KTraceSoftwareCounters::resumeStateCounters( unsigned long long time )
{
  auto itLastState = lastStateEndTime.begin();
  while( itLastState != lastStateEndTime.end() )
  {
    if( itLastState->second.end_time < time )
    {
      put_counters_on_state( itLastState );
      itLastState = lastStateEndTime.erase( itLastState );
    }
  }
}

void KTraceSoftwareCounters::sc_by_states( ProgressController *progress )
{
  int recordType, cpu, appl, task, thread, state;
  unsigned long long time_1, time_2, type, value;
  char *word;
  bool print_line = false;
  struct state_queue_elem *p, *q;
  int j;
  unsigned long num_iters = 0;

  p = nullptr;
  q = nullptr;

  bool end_parsing;

  if ( progress != nullptr )
    end_parsing = progress->getStop();
  else
    end_parsing = false;

  std::ostringstream buffer;

  while ( !infile->eof() && !end_parsing )
  {
    infile->getline( line );

    if( line[0] == '#' )
    {
      outfile << line;
      continue;
    }

    auto itBegin = line.cbegin();
    auto itEnd = line.cend();
    prv_atoll_v( itBegin, itEnd, recordType, cpu, appl, task, thread, time_1 );

    if ( progress != nullptr )
    {
      end_parsing = progress->getStop();
      if ( end_parsing )
        continue;
    }

    if ( num_iters == total_iters )
    {
      show_progress_bar( progress );
      num_iters = 0;
    }
    else
      num_iters++;

    auto itThread = findThreadInfo( appl, task, thread );

    switch( recordType )
    {
      case 1:
        prv_atoll_v( itBegin, itEnd, time_2, state );

        if ( ( min_state_time != 0 && ( time_2 - time_1 >= min_state_time ) && state == 1 ) || !min_state_time )
        {
          /* Insertar quan se li acaba l'estat */
          insert_in_queue_state( appl, task, thread, time_2 );

          /* Posar contadors per a aquest thread */
          last_time = time_1;
          put_counters_on_state_by_thread( appl, task, thread );

          /* Posem el record d'estat */
          dump_fields( outfile, recordType, cpu, appl, task, thread, time_1, time_2, state );
          outfile << "\n";
        }

        break;

      case 2:
        resumeStateCounters( time_1 );

        buffer.clear();
        buffer.str( "" );
        buffer << recordType << ":" << cpu << ":" << appl << ":" << task << ":" << thread << ":" << time_1;
        while ( itBegin != itEnd )
        {
          prv_atoll_v( itBegin, itEnd, type, value );

          if ( keep_events )
          {
            auto itKeepType = std::find( keep_types.begin(), keep_types.end(), type );
            if( itKeepType != keep_types.end() )
            {
                buffer << ":" << type << ":" << value;
                print_line = true;
            }
          }

          inc_counter( appl, task, thread, type, value );
        }

        if ( print_line )
        {
          outfile << buffer.str() << "\n";
          print_line = false;
        }

        break;
    
      default:
        break;
    }
  }

  auto itLastState = lastStateEndTime.begin();
  while( itLastState != lastStateEndTime.end() )
  {
    put_counters_on_state( itLastState );
    itLastState = lastStateEndTime.erase( itLastState );
  }
}


void KTraceSoftwareCounters::execute( char *trace_in, char *trace_out, ProgressController *progress )
{
  /* Reading of program args */
  read_sc_args();

  infile = TraceStream::openFile( trace_in );
  outfile = fstream( trace_out, ios_base::out );
  
  write_pcf( trace_out );

  ini_progress_bar( trace_in, progress );

  /* Read header */
  parseInHeaderAndDumpOut();

  if ( type_of_counters )
    sc_by_time( progress );
  else
    sc_by_states( progress );

  /* Close the files */
  infile->close();
  outfile.close();
}
