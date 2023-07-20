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


#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifndef _WIN32
#include <unistd.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <zlib.h>

#include "kprogresscontroller.h"
#include "ktracefilter.h"


#include "utils/traceparser/tracebodyio_v1.h"
#include "utils/traceparser/processmodel.h"
#include "utils/traceparser/resourcemodel.h"
#include "utils/traceparser/traceheader.h"


#ifdef _WIN32
#define atoll _atoi64
#endif


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


void KTraceFilter::parseInHeaderAndDumpOut( TraceStream *whichFile, std::fstream& outfile )
{
  ProcessModel<> traceProcessModel = ProcessModel<>( );
  ResourceModel<> traceResourceModel = ResourceModel<>( );
  unsigned long long traceEndTime = 0;
  TTimeUnit traceTimeUnit = NS;
  std::vector< std::string > communicators;

  string tmpDate;
  try
  {
    parseTraceHeader( *whichFile, tmpDate, traceTimeUnit, traceEndTime, traceResourceModel, traceProcessModel, communicators );
  }
  catch( TraceHeaderException& e )
  {
    throw e;
  }

  dumpTraceHeader( outfile, tmpDate, traceEndTime, traceTimeUnit, traceResourceModel, traceProcessModel, communicators );
}


/* 0:  Type not allowed */
/* 1:  Type allowed, whitout min_time */
/* 2:  Type allowed, with min_time */
int KTraceFilter::filter_allowed_type(  int appl, int task, int thread,
                                        unsigned long long time,
                                        unsigned long long type,
                                        unsigned long long value )
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
      if ( thread_call_info[appl][task][thread] == nullptr )
        type_allowed = 0;
      else
      {
        if ( time - thread_call_info[appl][task][thread]->event_time >= exec_options->filter_types[i].min_call_time )
          type_allowed = 2;
        else
        {
          type_allowed = 0;
          thread_call_info[appl][task][thread] = nullptr;
        }
      }
    }
  }
#endif

  return type_allowed;
}


void KTraceFilter::initFilterProgressBar( const std::string& fileName, ProgressController *progress )
{
  total_trace_size = TraceStream::getTraceFileSize( fileName );
  if ( total_trace_size < 500000000 )
    total_iters = 10000;
  else
    total_iters = 100000;

  current_read_size = 0;

  if( progress != nullptr)
    progress->setEndLimit( total_trace_size );
}


void KTraceFilter::show_progress_bar( ProgressController *progress )
{
  current_read_size = inFile->tellg();

  if ( is_zip_filter )
    current_read_size = current_read_size / TraceStream::GZIP_COMPRESSION_RATIO;

  if( progress != nullptr)
    progress->setCurrentProgress( current_read_size );
}


void KTraceFilter::load_pcf( char *pcf_name )
{
  FILE *infile;
  unsigned int state_id;
  char *state_name;

  state_name = (char *) malloc( sizeof(char) * MAX_STATE_NAME_SIZE );

  /* Open the files.  If nullptr is returned there was an error */
  if ( ( infile = fopen( pcf_name, "r" ) ) == nullptr )
  {
    printf( "Can't open file %s. Keeping all the states of the trace\n", pcf_name );
    all_states = 1;
    return;
  }

  /* Loading of state ids */
  while ( fgets( line, sizeof( line ), infile ) != nullptr )
  {
    if ( !strcmp( line, "STATES\n" ) )
    {
      int i;
      while ( fgets( line, sizeof( line ), infile ) != nullptr )
      {
        if ( !strcmp( line, "\n" ) ) return;

        sscanf( line, "%d %[^\n]", &state_id, state_name );

        i = 0;
        while ( i < 20 && exec_options->state_names[i] != nullptr )
        {
          if ( strstr( exec_options->state_names[i], state_name ) != nullptr )
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
  while ( elem != nullptr && elem->dump )
  {
    if ( elem->dump )
      outfile << elem->record;

    free( elem->record );
    elem_aux = elem;
    elem = elem->next;
    free( elem_aux );
  }

  buffer_first = elem;
  if ( buffer_first == nullptr )
    buffer_last = nullptr;
}


void KTraceFilter::translateEvent( unsigned long long &type, unsigned long long &value )
{
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
}


void KTraceFilter::execute( char *trace_in, char *trace_out, ProgressController *progress )
{
  bool print_record;
  int i, j, k, state, size;
  int recordType, appl, task, thread, cpu;
  int  appl_2, task_2, thread_2, cpu_2;
  unsigned long long time_1, time_2, time_3, time_4, type, value;
  char *c;
  char *pcf_file;
  unsigned long num_iters = 0;
  bool end_parsing = false;
  bool dump_event_buffer, call_in;
  struct buffer_elem *new_elem, *elem_aux;
  std::string line;

  pcf_file     = (char *) malloc( sizeof(char) * MAX_FILENAME_SIZE );

  /* ini vars. */
  show_states = false;
  show_events = false;
  show_comms = false;
  filter_all_types = false;
  min_state_time = 0;
  min_comm_size = 0;
  states_info.last_id = 0;
  buffer_first = nullptr;
  buffer_last = nullptr;

  for ( i = 0; i < MAX_APPL; i++ )
    for ( j = 0; j < MAX_TASK; j++ )
      for ( k = 0; k < MAX_THREAD; k++ )
        thread_call_info[i][j][k] = nullptr;

  // TODO: change in execute call
  std::string traceIn( trace_in );
  std::string traceOut( trace_out );

  read_params();

  inFile = TraceStream::openFile( trace_in );
  outfile.open( trace_out, ios_base::out );

  initFilterProgressBar( traceIn, progress);

  /* Symbol loading of the .pcf file */
  if ( show_states && !all_states )
  {
    strcpy( pcf_file, traceIn.c_str() );
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

  parseInHeaderAndDumpOut( inFile, outfile );

  if ( progress != nullptr )
    end_parsing = progress->getStop();

  std::ostringstream event_record;

  /* Processing the trace records */
  while ( !end_parsing )
  {
    if ( progress != nullptr )
    {
      end_parsing = progress->getStop();
      if ( end_parsing )
        continue;
    }

    if ( inFile->eof() || !inFile->good() )
    {
      end_parsing = true;
      continue;
    }
    
    inFile->getline( line );

    if ( num_iters == total_iters )
    {
      show_progress_bar( progress );
      num_iters = 0;
    }
    else
      num_iters++;

    auto itBegin = line.cbegin();
    auto itEnd = line.cend();
    switch ( line[0] )
    {
      case '1':
        if ( !show_states )
          break;

        prv_atoll_v( itBegin, itEnd, recordType, cpu, appl, task, thread, time_1, time_2, state );

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
            outfile << line << '\n';
          else
          {
            /* Insert on event buffer */
            if ( ( new_elem = ( struct buffer_elem * )malloc( sizeof( struct buffer_elem ) ) ) == nullptr )
            {
              printf( "NO MORE MEMORY!\n" );
              exit( 1 );
            }

            new_elem->record = strdup( line.c_str() );
            new_elem->dump = true;
            new_elem->appl = appl;
            new_elem->task = task;
            new_elem->thread = thread;
            new_elem->next = nullptr;

            if ( buffer_first == nullptr )
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
          outfile << line << '\n';
          break;
        }

        prv_atoll_v( itBegin, itEnd, recordType, cpu, appl, task, thread, time_1 );

        event_record.clear();
        event_record.str( "" );
        event_record << recordType << ":" << cpu << ":" << appl << ":" << task << ":" << thread << ":" << time_1;

        call_in = false;
        dump_event_buffer = false;
        print_record = false;
        
        prv_atoll_v( itBegin, itEnd, type, value );        
        translateEvent( type, value );

        if ( ( i = filter_allowed_type( appl, task, thread, time_1, type, value ) ) > 0 )
        {
          print_record = true;

          event_record << ":" << type << ":" << value;

          if ( i == 2 )
          {
            if ( value > 0 ) call_in = true;
            else dump_event_buffer = true;
          }
        }

        while ( prv_atoll_v( itBegin, itEnd, type, value ) )
        {
          translateEvent( type, value );

          if ( ( i = filter_allowed_type( appl, task, thread, time_1, type, value ) ) > 0 )
          {
            print_record = true;
            
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

        event_record << std::endl;

        if ( print_record )
        {
          if ( !filter_by_call_time )
          {
            outfile << event_record.str();
          }
          else
          {
            /* Insert on buffer */
            if ( ( new_elem = ( struct buffer_elem * )malloc( sizeof( struct buffer_elem ) ) ) == nullptr )
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
            new_elem->next = nullptr;

            if ( buffer_first == nullptr )
              buffer_first = new_elem;

            if ( buffer_last == nullptr )
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
              if ( thread_call_info[appl][task][thread] != nullptr )
                thread_call_info[appl][task][thread]->dump = true;

              dump_buffer();
              thread_call_info[appl][task][thread] = nullptr;
            }
          }
        }

        break;

      case '3':
        if ( !show_comms )
          break;

        if ( exec_options->min_comm_size > 0 )
        {
          prv_atoll_v( itBegin, itEnd, recordType, cpu,   appl,   task,   thread,   time_1, time_2,
                                                   cpu_2, appl_2, task_2, thread_2, time_3, time_4, size );
          if ( size < exec_options->min_comm_size )
            break;
        }

        if ( !filter_by_call_time )
          outfile << line << '\n';
        else
        {
          /* Insert on event buffer */
          if ( ( new_elem = ( struct buffer_elem * )malloc( sizeof( struct buffer_elem ) ) ) == nullptr )
          {
            printf( "NO MORE MEMORY!!!\n" );
            exit( 1 );
          }

          new_elem->record = strdup( line.c_str() );
          new_elem->dump = true;
          new_elem->appl = appl;
          new_elem->task = task;
          new_elem->thread = thread;
          new_elem->next = nullptr;

          if ( buffer_first == nullptr )
          {
            buffer_first = new_elem;
            buffer_last = new_elem;
          }

          buffer_last->next = new_elem;
          buffer_last = new_elem;
        }
        break;


      case '#':
        outfile << line << '\n';
        break;


      default:
        break;
    }
  }

  /* Dumping the elems left in the buffer */
  if ( filter_by_call_time )
  {
    new_elem = buffer_first;
    while ( new_elem != nullptr )
    {
      if ( new_elem->dump )
        outfile << new_elem->record;

      free( new_elem->record );
      elem_aux = new_elem;
      new_elem = new_elem->next;
      free( elem_aux );
    }
  }

  outfile.close();
  inFile->close();
  delete inFile;

  free( pcf_file );
}
