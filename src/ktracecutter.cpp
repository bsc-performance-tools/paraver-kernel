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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif

#include "ktracecutter.h"

#include "utils/traceparser/tracebodyio_v1.h"
#include "utils/traceparser/processmodel.h"
#include "utils/traceparser/resourcemodel.h"
#include "utils/traceparser/traceheader.h"

#include "kprogresscontroller.h"
#include "paraverconfig.h"
#include "ParaverMetadataManager.h"

#ifdef _WIN32
#define atoll _atoi64
#endif

#include <iostream>
using namespace std;

KTraceCutter::KTraceCutter( TraceOptions *options,
                            const vector< TEventType > &whichTypesWithValuesZero )
{
  total_cutter_iters = 0;
  exec_options = new KTraceOptions( (KTraceOptions *) options );
  HWCTypesInPCF.insert( whichTypesWithValuesZero.begin(), whichTypesWithValuesZero.end() );
  cutterApplicationCaller = CutterMetadata::ORIGINAL_APPLICATION_ID;
  
  // PROFET
  firstApplicationFinished = false;
  timeOfFirsApplicationFinished = 0;
}


KTraceCutter::~KTraceCutter()
{
  delete( exec_options );
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

void KTraceCutter::set_keep_boundary_events( bool keepEvents )
{
}

void KTraceCutter::setCutterApplicationCaller( std::string caller )
{
  cutterApplicationCaller = caller;
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
      if ( ( buffer = strchr( word, '-' ) ) != nullptr )
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
    while ( ( word = strtok( nullptr, "," ) ) != nullptr );
  }

  if ( exec_options->max_trace_size != 0 )
    max_size = exec_options->max_trace_size * 1000000;

  break_states = exec_options->break_states;
  remFirstStates = exec_options->remFirstStates;
  remLastStates = exec_options->remLastStates;
  keep_boundary_events = exec_options->keep_boundary_events;
  keep_all_events = exec_options->keep_all_events;

  if ( originalTime )
    break_states = false; // Assigned ONLY if originalTime == true, else KEEP value.

  if( break_states )
    keep_boundary_events = false;
}

void writeOffsetLineToFile( FILE *file, const std::string& line )
{
  fprintf( file, "%s\n", line.c_str() );
}

void writeOffsetLineToFile( std::fstream& file, const std::string& line )
{
  file << line << std::endl;
}

template< typename T >
void KTraceCutter::writeOffsetLine( T& whichFile,
                                    const char *trace_in_name,
                                    unsigned long long timeOffset,
                                    unsigned long long timeCutBegin,
                                    unsigned long long timeCutEnd )
{
  if ( originalTime )
    timeOffset = 0;

  CutterMetadata tmpData( MetadataManager::GetCurrentDate(),
                          cutterApplicationCaller,
                          string( '"' + string( trace_in_name ) + '"'),
                          (PRV_UINT64)timeOffset,   // first record of the trace
                          (PRV_UINT64)timeCutBegin, // first time of the cut
                          (PRV_UINT64)timeCutEnd ); // last time of the cut
  ostringstream tmpStream;
  tmpData.Write( tmpStream );
  
  writeOffsetLineToFile( whichFile, tmpStream.str() );
}


void KTraceCutter::parseInHeaderAndDumpOut( TraceStream *whichFile, std::fstream& outfile )
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

  trace_time = traceEndTime;
  if ( !by_time )
  {
    time_min = ( ( double )( trace_time / 100 ) ) * min_perc;
    time_max = ( ( double )( trace_time / 100 ) ) * max_perc;
    total_time = time_max - time_min;
  }

  if ( !originalTime )
    traceEndTime = total_time;

  for ( auto i = 0; i < traceProcessModel.totalApplications(); ++i )
    appsInfo.emplace_back( traceProcessModel.totalThreads( i ) );

  // Dump header in outfile
  dumpTraceHeader( outfile, tmpDate, traceEndTime, traceTimeUnit, traceResourceModel, traceProcessModel, communicators );
}


// #Paraver (12/03/2018 at 16:11:35.687574899):100:1:1:1(1:1)
void KTraceCutter::proces_cutter_header( const std::string& header, TraceStream *whichFile, FILE *outfile )
{
  int num_comms;
  char *word;
  string auxLine;

  char *tmpHeader = (char *)malloc( header.length() + 1 );
  strcpy( tmpHeader, header.c_str() );

  // PARSE variable header
  // #Paraver (12/03/2018 at 16:11:35.687574899):
  word = strtok( tmpHeader, ")" );
  current_size += fprintf( outfile, "%s):", word );

  /* Obtaining the trace total time */
  word = strtok( nullptr, ":" );
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

  /* Obtaining the number of communicators */
  word = strtok( nullptr, "\n" ); // put in word the rest of the line
  current_size += fprintf( outfile, "%s\n", word );

  // Do I have some "," looking back?
  word = strrchr( word, ',' );
  if ( word != nullptr )
  {
    // Is it because some "1:1,1:1)\n" or the expected "1:1,1:1),16\n" ?
    //                       -^-                               -^-
    strcpy( tmpHeader, word + 1 ); // Copy "1:1)" or "16" in header
    if ( strchr( tmpHeader, ')' ) == nullptr ) // Do I have some ")"?
    {
      // Hope it's a number and it fits the number of communicator lines...
      num_comms = atoi( tmpHeader );
      while ( num_comms > 0 )
      {
        whichFile->getline( auxLine );
      
        if( auxLine.back() == '\n' )
          current_size += fprintf( outfile, "%s", auxLine.c_str() );
        else
          current_size += fprintf( outfile, "%s\n", auxLine.c_str() );
        --num_comms;
      }
    }
  }


  /* Writing in the header the offset of the cut regard original trace */

  /* Reading first if we have old offsets into the trace */
  std::streampos lastPos = whichFile->tellg();
  whichFile->getline( auxLine );

  while ( auxLine[0] == '#' )
  {
    if( whichFile->eof() )
      break;

    current_size += fprintf( outfile, "%s", auxLine.c_str() );

    lastPos = whichFile->tellg();
    whichFile->getline( auxLine );
  }

  whichFile->seekg( lastPos );
}


template< typename IteratorType >
void KTraceCutter::dumpEventsSet( std::fstream& outfile,
                                  const IteratorType& begin,
                                  const IteratorType& end,
                                  unsigned int cpu,
                                  unsigned int appl,
                                  unsigned int task,
                                  unsigned int thread,
                                  const unsigned long long final_time,
                                  int &numWrittenChars,
                                  bool &needEOL,
                                  bool &writtenComment )
{
  char *buffer = (char *)malloc( MAX_LINE_SIZE );

  if ( !writtenComment )
  {
    outfile << "# Appending events with value 0\n";
    if( writeToTmpFile ) ++total_tmp_lines;
    writtenComment = true;
  }

  for ( IteratorType it = begin; it != end; ++it )
  {
    if ( numWrittenChars == 0 )
    {
      // Write new line
      numWrittenChars += sprintf( buffer, "2:%d:%d:%d:%d:%lld:%lld:0",
                                  cpu, appl + 1, task + 1, thread + 1,
                                  final_time, (unsigned long long)*it );
      outfile << buffer;
      if( writeToTmpFile )
        ++total_tmp_lines;

      needEOL = true;
    }
    else if ( numWrittenChars + 32 > MAX_LINE_SIZE )
    {
      // Too many events: close current line
      outfile << std::endl;
      numWrittenChars = 0;
      needEOL = false;
    }
    else
    {
      // Append to current line
      numWrittenChars += sprintf( buffer, ":%lld:0", (unsigned long long)*it );
      outfile << buffer;
      needEOL = true;
    }
  }

  if( needEOL )
  {
    // Close current line
    outfile << std::endl;  // because we know theres's one at least.
    numWrittenChars = 0;
    needEOL = false;
  }

  free( buffer );
}


void KTraceCutter::appendLastZerosToUnclosedEvents( const unsigned long long final_time, std::fstream& outfile )
{
  TCPUOrder cpu;
  int numWrittenChars = 0;
  bool needEOL = false;
  bool writtenComment = false;

  for( CutterThreadInfo::iterator it = threadsInfo.begin(); it != threadsInfo.end(); ++it )
  {
    ThreadInfo tmpInfo = (*it).second;
    unsigned int appl = (*it).first.dim1;
    unsigned int task = (*it).first.dim2;
    unsigned int thread = (*it).first.dim3;

    cpu = tmpInfo.lastCPU;

    numWrittenChars = 0;
    needEOL = false;

    if( tmpInfo.openedEventTypes.size() > 0 )
    {
      std::set<TEventType> insertedUniqueOpenedTypes;
      std::vector<TEventType> uniqueOpenedTypes;

      std::copy_if( tmpInfo.openedEventTypes.begin(),
                    tmpInfo.openedEventTypes.end(),
                    std::back_inserter( uniqueOpenedTypes ),
                    [&insertedUniqueOpenedTypes]( auto elem )
                    {
                      if( insertedUniqueOpenedTypes.find( elem ) != insertedUniqueOpenedTypes.end() )
                        return false;
                      insertedUniqueOpenedTypes.insert( elem );
                      return true;
                    } );

      dumpEventsSet( outfile,
                     uniqueOpenedTypes.rbegin(),
                     uniqueOpenedTypes.rend(),
                     cpu, appl, task, thread,
                     final_time,
                     numWrittenChars,
                     needEOL,
                     writtenComment );
    }

    if( tmpInfo.HWCTypesInPRV.size() > 0 )
    {
      dumpEventsSet( outfile,
                     tmpInfo.HWCTypesInPRV.begin(),
                     tmpInfo.HWCTypesInPRV.end(),
                     cpu, appl, task, thread,
                     final_time,
                     numWrittenChars,
                     needEOL,
                     writtenComment );
    }
  }
}


void KTraceCutter::ini_cutter_progress_bar( const std::string& fileName,
                                            ProgressController *progress )
{
  total_trace_size = TraceStream::getTraceFileSize( fileName );

  /* Depen mida traça mostrem percentatge amb un interval diferent de temps */
  if ( total_trace_size < 500000000 )
    total_cutter_iters = 10000;
  else
    total_cutter_iters = 100000;

  current_read_size = 0;

  if( progress != nullptr)
  {
    if( writeToTmpFile )
    {
      progress->setEndLimit( total_trace_size * 2 );
    }
    else
      progress->setEndLimit( total_trace_size );
  }
}


void KTraceCutter::show_cutter_progress_bar( ProgressController *progress, TraceStream *whichFile )
{
  if( !secondPhase )
    current_read_size = whichFile->tellg();

  if ( progress != nullptr )
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


void KTraceCutter::update_queue( unsigned int appl, unsigned int task, unsigned int thread,
                                 unsigned long long type,
                                 unsigned long long value )
{
  if ( threadsInfo.find( appl, task, thread ) == threadsInfo.end() )
  {
    init_useful_tasks = true;
    ++useful_tasks;
    ThreadInfo newThreadInfo( HWCTypesInPCF );
    threadsInfo( appl, task, thread ) = newThreadInfo;
  }

  ThreadInfo& tmpInfo = threadsInfo( appl, task, thread );
  if ( value > 0 )
  {
    if ( HWCTypesInPCF.find( type ) != HWCTypesInPCF.end() )
    {
      tmpInfo.HWCTypesInPRV.insert( (TEventType)type );
    }
    else
    {
      tmpInfo.openedEventTypes.push_back( (TEventType)type );
    }
  }
  else
  {
    auto it = std::find( tmpInfo.openedEventTypes.rbegin(), tmpInfo.openedEventTypes.rend(), (TEventType)type );
    if( it != tmpInfo.openedEventTypes.rend() )
      tmpInfo.openedEventTypes.erase( std::next( it ).base() ); // calling base() points resulting iterator to previous position
  }
}


// Substract to all the times in the trace the first time of the first record
// Doesn't change header
void KTraceCutter::shiftLeft_TraceTimes_ToStartFromZero( const char *originalTraceName,
                                                         const char *nameIn, const char *nameOut, ProgressController *progress )
{
  unsigned long long timeOffset = 0, time_1, time_2, time_3, time_4;
  int cpu, appl, task, thread, state, cpu_2, appl_2, task_2, thread_2;
  std::string trace_header;
  char *line = (char *) malloc( sizeof( char ) * MAX_TRACE_HEADER );
  char *outBuffer = (char *) malloc( sizeof( char ) * MAX_TRACE_HEADER );
  TraceStream *infile = TraceStream::openFile( nameIn );

  fstream outfile( nameOut, ios_base::out );

  /* Process header */
  total_time = last_record_time - first_record_time;

  // Consume header
  parseInHeaderAndDumpOut( infile, outfile );

  bool end_read = false;

  /* Read one more record if possible */
  if ( infile->eof() )
    end_read = true;
  else
    infile->getline( trace_header );

  // Get time of the first record ignoring any other field.
  if (!end_read)
    sscanf( trace_header.c_str(), "%*d:%*d:%*d:%*d:%*d:%lld:", &timeOffset );

  // Override it: we have the minimum time of the written records.
  timeOffset = first_record_time;
  current_tmp_lines = 0;

  writeOffsetLine( outfile, originalTraceName, timeOffset, time_min, time_max );

  while ( !end_read )
  {
    show_cutter_progress_bar( progress, infile );

    switch ( trace_header[0] )
    {
      case '1':
        sscanf( trace_header.c_str(), "%*d:%d:%d:%d:%d:%lld:%lld:%d\n", &cpu, &appl, &task, &thread, &time_1, &time_2, &state );


        time_1 = time_1 - timeOffset;
        time_2 = time_2 - timeOffset;

        sprintf( outBuffer, "1:%d:%d:%d:%d:%lld:%lld:%d\n", cpu, appl, task, thread, time_1, time_2, state );
        outfile << outBuffer;

        ++current_tmp_lines;
        break;


      case '2':
        sscanf( trace_header.c_str(), "%*d:%d:%d:%d:%d:%lld:%s\n", &cpu, &appl, &task, &thread, &time_1, line );

        time_1 = time_1 - timeOffset;

        sprintf( outBuffer, "2:%d:%d:%d:%d:%lld:%s\n", cpu, appl, task, thread, time_1, line );
        outfile << outBuffer;

        ++current_tmp_lines;
        break;

      case '3':
        sscanf( trace_header.c_str(), "%*d:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%s\n",
                &cpu,   &appl,   &task,   &thread,   &time_1, &time_2,
                &cpu_2, &appl_2, &task_2, &thread_2, &time_3, &time_4, line );

        time_1 = time_1 - timeOffset;
        time_2 = time_2 - timeOffset;
        time_3 = time_3 - timeOffset;
        time_4 = time_4 - timeOffset;

        sprintf( outBuffer, "3:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%s\n",
                 cpu,   appl,   task,   thread,   time_1, time_2,
                 cpu_2, appl_2, task_2, thread_2, time_3, time_4, line );
        outfile << outBuffer;

        ++current_tmp_lines;
        break;

      case '#':
        outfile << trace_header << "\n";
        ++current_tmp_lines;
        break;

      default:
        break;
    }

    /* Read one more record if possible */
    if ( infile->eof() )
      end_read = true;
    else
      infile->getline( trace_header );
  }

  free( line );
  free( outBuffer );

  infile->close();
  outfile.close();
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

KTraceCutter::ThreadInfo& KTraceCutter::initThreadInfo( unsigned int appl, unsigned int task, unsigned int thread, unsigned int cpu )
{
  ++useful_tasks;
  init_useful_tasks = true;
  ThreadInfo newThreadInfo( HWCTypesInPCF );
  ThreadInfo& tmpInfo = threadsInfo( appl - 1, task - 1, thread - 1 );
  tmpInfo = newThreadInfo;
  tmpInfo.lastCPU = cpu;
  tmpInfo.last_time = 0;

  return tmpInfo;
}

void KTraceCutter::execute( std::string trace_in,
                            std::string trace_out,
                            ProgressController *progress )
{
  TraceStream *inFile;
  string trace_header;
  string line;
  string tmp_dir;
  char *trace_file_out;
  char *buffer;
  bool end_parsing = false;

  unsigned int id, cpu, appl, task, thread, state, cpu_2, appl_2, task_2, thread_2, size, tag;
  unsigned long long type, value, time_1, time_2, time_3, time_4;
  int i;

  unsigned long num_iters = 0;

  KProgressController *tmpKProgressControler = nullptr;

  if ( progress != nullptr )
    tmpKProgressControler = (KProgressController *)progress->getConcrete();

  trace_file_out = (char *) malloc( sizeof(char) * MAX_FILENAME_SIZE );
  buffer         = (char *) malloc( sizeof(char) * MAX_LINE_SIZE );

  by_time = false;
  originalTime = false;
  max_size = 0;
  cut_tasks = false;
  break_states = true;
  init_useful_tasks = false;
  useful_tasks = 0;
  first_time_caught = false;
  current_size = 0;

  for ( i = 0; i < MAX_SELECTED_TASKS; i++ )
    wanted_tasks[i].min_task_id = 0;

  /* Reading of the program arguments */
  read_cutter_params();

  inFile = TraceStream::openFile( trace_in );
  
  writeToTmpFile = break_states || !originalTime;
  if ( writeToTmpFile )
  {
    tmp_dir = ParaverConfig::getInstance()->getGlobalTmpPath();

#ifdef _WIN32
    sprintf( trace_file_out, "%s\\tmp_fileXXXXXX", tmp_dir.c_str() );
    _mktemp_s( trace_file_out, strlen( trace_file_out ) + 1 );
#else
    sprintf( trace_file_out, "%s/tmp_fileXXXXXX", tmp_dir.c_str() );
    mkstemp( trace_file_out );
#endif
  }
  else
    strcpy( trace_file_out, trace_out.c_str() );

  fstream outfile( trace_file_out, ios_base::out );

  ini_cutter_progress_bar( trace_in, tmpKProgressControler );

  /* Process header */
  parseInHeaderAndDumpOut( inFile, outfile );

  if ( !writeToTmpFile )
  {
    writeOffsetLine( outfile, trace_in.c_str(), time_min, time_min, time_max );
  }

  /* We process the trace like the originalTime version */

  bool maxTimeReached = false;
  last_record_time = 0;
  total_tmp_lines = 0;
  secondPhase = false;

  if( tmpKProgressControler != nullptr )
    end_parsing = tmpKProgressControler->getStop();

  /* Processing the trace records */
  while ( !end_parsing && !maxTimeReached )
  {
    if( tmpKProgressControler != nullptr )
    {
      end_parsing = tmpKProgressControler->getStop();
      if ( end_parsing )
        continue;
    }

    /* Read one more record is possible */
    if ( inFile->eof() || !inFile->good() )
    {
      end_parsing = true;

      continue;
    }
    
    inFile->getline( line );
    
    if ( num_iters == total_cutter_iters )
    {
      show_cutter_progress_bar( tmpKProgressControler, inFile );
      num_iters = 0;
    }
    else
      ++num_iters;

    std::ostringstream aux_buffer;

    CutterThreadInfo::iterator threadInfoIt;

    switch ( line[0] )
    {
      case '1':
        sscanf( line.c_str(), "%d:%d:%d:%d:%d:%lld:%lld:%d\n",
                &id, &cpu, &appl, &task, &thread, &time_1, &time_2, &state );
        
        // PROFET
        if ( exec_options->get_max_cut_time_to_finish_of_first_appl() &&
             firstApplicationFinished &&
             ( time_1 >= timeOfFirsApplicationFinished || 
               ( time_2 >= timeOfFirsApplicationFinished && !break_states ) ) )
          break;

        if ( cut_tasks && !is_selected_task( task ) )
          break;

        if ( time_2 <= time_min )
          break;

        threadInfoIt = threadsInfo.find( appl - 1, task - 1, thread - 1 );

        if ( time_1 < time_min && time_2 > time_max && ( remFirstStates || remLastStates ))
        {
          ThreadInfo *tmpInfo = nullptr;
          if ( threadInfoIt == threadsInfo.end() )
            tmpInfo = &initThreadInfo( appl, task, thread, cpu );
          else
            tmpInfo = &threadInfoIt->second;

          tmpInfo->without_states = true;

          break;
        }

        if ( time_1 < time_min && time_2 >= time_min && remFirstStates )
          break;

        if ( time_1 < time_max && time_2 > time_max && remLastStates ||
             originalTime && time_1 > time_max )
        {
          if ( threadInfoIt != threadsInfo.end() &&
               !threadsInfo( appl - 1, task - 1, thread - 1 ).finished )
          {
            --useful_tasks;
            threadsInfo( appl - 1, task - 1, thread - 1 ).finished = true;

            // PROFET
            if ( exec_options->get_max_cut_time_to_finish_of_first_appl() )
            {
              if ( appl == 1 && appsInfo[ appl - 1 ].addFinishedThread() )
              {
                firstApplicationFinished = true;
                timeOfFirsApplicationFinished = threadsInfo( appl - 1, task - 1, thread - 1 ).lastStateEndTime;
              }
            }
          }

          break;
        }


        if ( !originalTime && time_1 > time_max )
        {
          maxTimeReached = true;
        }
        else // originalTime || time_1 <= time_max
        {
          if ( threadInfoIt == threadsInfo.end() )
            initThreadInfo( appl, task, thread, cpu );

          threadsInfo( appl - 1, task - 1, thread - 1 ).lastStateEndTime = time_2;
          if( time_1 < time_max && time_2 > time_max && !remLastStates && !break_states && keep_boundary_events )
          {
            threadsInfo( appl - 1, task - 1, thread - 1 ).finished = true;

            // PROFET
            if ( exec_options->get_max_cut_time_to_finish_of_first_appl() )
            {
              if ( appl == 1 && appsInfo[ appl - 1 ].addFinishedThread() )
              {
                firstApplicationFinished = true;
                timeOfFirsApplicationFinished = threadsInfo( appl - 1, task - 1, thread - 1 ).lastStateEndTime;
              }
            }
          }

          if ( !originalTime && break_states )
          {
            if ( time_1 < time_min )
            {
              time_1 = time_min;
            }
            
            if ( time_2 > time_max)
            {
              time_2 = time_max;
            }
          }

          //if ( threadsInfo[appl-1][task-1][thread-1]->last_time < time_2 )
          threadsInfo( appl - 1, task - 1, thread - 1 ).last_time = time_2;

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

          sprintf( buffer, "%d:%d:%d:%d:%d:%lld:%lld:%d\n",
                   id, cpu, appl, task, thread, time_1, time_2, state );
          outfile << buffer;
          if( writeToTmpFile ) ++total_tmp_lines;

        }

        break;

      case '2':
        sscanf( line.c_str(), "%d:%d:%d:%d:%d:%lld:%s\n", &id, &cpu, &appl, &task, &thread, &time_1, buffer );
        line = buffer;

        // PROFET
        if ( exec_options->get_max_cut_time_to_finish_of_first_appl() &&
             firstApplicationFinished && time_1 >= timeOfFirsApplicationFinished )
          break;

        if( cut_tasks && !is_selected_task( task ) )
          break;

        threadInfoIt = threadsInfo.find( appl - 1, task - 1, thread - 1 );

        if( threadInfoIt != threadsInfo.end() &&
            threadInfoIt->second.finished &&
            time_1 != threadInfoIt->second.lastStateEndTime )
        {
          if( !( !remLastStates && !break_states && keep_boundary_events && time_1 <= threadInfoIt->second.lastStateEndTime ) )
            break;
        }
        else if ( time_1 > time_max )
          break;

        if ( ( threadInfoIt != threadsInfo.end() ) &&
             ( time_1 > threadInfoIt->second.last_time ) &&
             ( time_1 > time_max ) &&
             !keep_boundary_events )
          break;

        if ( threadInfoIt == threadsInfo.end() && time_1 > time_max )
          break;

        // TODO: keep_all_events
        if ( threadInfoIt == threadsInfo.end() && remFirstStates )
          break; // ?


        /* If time inside cut, adjust time */
        if ( time_1 >= time_min ||
             ( time_1 < time_min &&
               threadInfoIt != threadsInfo.end() &&
               threadInfoIt->second.last_time >= time_min &&
               keep_boundary_events ) ||
             ( first_time_caught &&
               time_1 >= first_record_time &&
               threadInfoIt != threadsInfo.end() &&
               threadInfoIt->second.without_states &&
               keep_all_events )
           )
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

          sprintf( buffer, "%d:%d:%d:%d:%d:%lld",
                   id, cpu, appl, task, thread, time_1 );
          outfile << buffer;

          std::string::const_iterator itBegin = line.begin();
          const std::string::const_iterator itEnd = line.end();

          while ( itBegin != itEnd )
          {
            prv_atoll_v( itBegin, itEnd, type, value );
            update_queue( appl - 1, task - 1, thread - 1, type, value );

            if( threadInfoIt == threadsInfo.end() )
              threadInfoIt = threadsInfo.find( appl - 1, task - 1, thread - 1 );

            if( threadInfoIt->second.HWCTypesToReset.find( type ) != threadInfoIt->second.HWCTypesToReset.end() )
            {
              value = 0;
              threadInfoIt->second.HWCTypesToReset.erase( type );
            }

            outfile << ":" << type << ":" << value;
          }

          outfile << "\n";

          if( writeToTmpFile ) ++total_tmp_lines;

          /* For closing all the opened calls */
          threadsInfo( appl - 1, task - 1, thread - 1 ).last_time = time_1;
          threadsInfo( appl - 1, task - 1, thread - 1 ).lastCPU = cpu;


        }

        break;

      case '3':
        sscanf( line.c_str(), "%d:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%d:%d\n",
                &id,
                &cpu,   &appl,   &task,   &thread,   &time_1, &time_2,
                &cpu_2, &appl_2, &task_2, &thread_2, &time_3, &time_4, &size, &tag );

        // PROFET
        if ( exec_options->get_max_cut_time_to_finish_of_first_appl() &&
             firstApplicationFinished &&
             ( time_1 >= timeOfFirsApplicationFinished ||
               time_2 >= timeOfFirsApplicationFinished ||
               time_3 >= timeOfFirsApplicationFinished ||
               time_4 >= timeOfFirsApplicationFinished ) )
          break;

        if ( cut_tasks && !is_selected_task( task ) && !is_selected_task( task_2 ) )
          break;

        if ( time_1 >= time_min && time_3 >= time_min )
        {
          /* if time outside the cut, finish */
          if ( time_1 > time_max && !originalTime ) // PROBLEM: DISORDERED TRACES
          {
            maxTimeReached = true;
          }
          else if ( time_2 <= time_max && time_4 <= time_max )
          {
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

            sprintf( buffer, "%d:%d:%d:%d:%d:%lld:%lld:%d:%d:%d:%d:%lld:%lld:%d:%d\n",
                     id,
                     cpu,   appl,   task,   thread,   time_1, time_2,
                     cpu_2, appl_2, task_2, thread_2, time_3, time_4, size, tag );
            outfile << buffer;

            if( writeToTmpFile ) ++total_tmp_lines;
          }
        }

        break;

      case '#':
        outfile << line << std::endl;
        if( writeToTmpFile )
          ++total_tmp_lines;

        break;

      default:
        break;
    }

    if ( max_size > 0 )
      if ( max_size <= outfile.tellg() )
        break;

    if ( init_useful_tasks && useful_tasks == 0 )
      break;
  }

  if( tmpKProgressControler != nullptr && tmpKProgressControler->getStop() )
    tmpKProgressControler->setMessage( "Finishing cut..." );

  if ( last_record_time > time_max || remLastStates )
  //if ( !originalTime )
    appendLastZerosToUnclosedEvents( last_record_time, outfile );
  else
    appendLastZerosToUnclosedEvents( time_max, outfile );

  /* Close the files */
  inFile->close();
  outfile.close();

  if ( writeToTmpFile )   // trace_file_out is a tmpfile!!
  {
    secondPhase = true;
    shiftLeft_TraceTimes_ToStartFromZero( trace_in.c_str(), trace_file_out, trace_out.c_str(), tmpKProgressControler );
  }

  free( trace_file_out );
  free( buffer );
}
