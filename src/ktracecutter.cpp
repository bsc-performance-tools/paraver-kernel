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

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <math.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

#ifndef _WIN32
#  include <unistd.h>
#else
#  include <io.h>
#endif

#include "ParaverMetadataManager.h"
#include "kprogresscontroller.h"
#include "ktracecutter.h"
#include "paraverconfig.h"
#include "utils/traceparser/tracebodyio_v1.h"
#include "utils/traceparser/traceheader.h"

#ifdef _WIN32
#  define atoll _atoi64
#endif

#include <iostream>
using namespace std;

// Helper function to check stream state and throw with file path on error
static void checkStreamWrite( std::fstream &stream, const std::string &filePath )
{
  if( stream.fail() || stream.bad() )
  {
    int savedErrno     = errno;
    std::string errMsg = "Write failed to file: " + filePath;
    if( savedErrno != 0 )
      errMsg += " (" + std::string( std::strerror( savedErrno ) ) + ")";
    throw std::runtime_error( errMsg );
  }
}


KTraceCutter::KTraceCutter( TraceOptions *options, const vector< TEventType > &whichHWCTypes, const vector< TEventType > &whichNotHWCTypes )
{
  total_cutter_iters = 0;
  exec_options       = new KTraceOptions( (KTraceOptions *)options );
  HWCTypesInPCF.insert( whichHWCTypes.begin(), whichHWCTypes.end() );
  notHWCTypesInPCF.insert( whichNotHWCTypes.begin(), whichNotHWCTypes.end() );
  cutterOriginCaller = MetadataAction::MANUAL_ORIGIN_ID;

  // MESS
  firstApplicationFinished      = false;
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

void KTraceCutter::setCutterOriginCaller( std::string caller )
{
  cutterOriginCaller = caller;
}


/* Function for parsing program parameters */
void KTraceCutter::read_cutter_params()
{
  char *word, *buffer;

  by_time       = exec_options->by_time;
  time_min      = exec_options->min_cutting_time;
  time_max      = exec_options->max_cutting_time;
  total_time    = time_max - time_min;
  min_perc      = exec_options->min_percentage;
  max_perc      = exec_options->max_percentage;
  originalTime  = exec_options->original_time;
  modify_header = exec_options->modify_trace_header;

  cut_tasks     = 0;
  cut_apps      = 0;
  cut_threads   = 0;
  int j_tasks   = 0;
  int j_apps    = 0;
  int j_threads = 0;

  if( exec_options->tasks_list[ 0 ] != '\0' )
  {
    // Clear vectors
    wanted_apps.clear();
    wanted_tasks.clear();
    wanted_threads.clear();

    CutterObjectLevel mode = exec_options->get_cutter_object_level();

    // Setup mode flags
    if( mode == CutterObjectLevel::CUT_OBJECT_APPLICATION )
      cut_apps = 1;
    else if( mode == CutterObjectLevel::CUT_OBJECT_THREAD )
      cut_threads = 1;
    else
      cut_tasks = 1;

    // Parse
    std::string taskListStr( exec_options->tasks_list );
    std::stringstream ss( taskListStr );
    std::string token;

    while( std::getline( ss, token, ',' ) )
    {
      // Trim whitespace
      size_t first = token.find_first_not_of( ' ' );
      if( std::string::npos == first )
        continue;
      size_t last = token.find_last_not_of( ' ' );
      token       = token.substr( first, ( last - first + 1 ) );

      int a = 0, t = 0, th = 0;

      if( mode == CutterObjectLevel::CUT_OBJECT_APPLICATION )
      {
        if( sscanf( token.c_str(), "%d", &a ) == 1 )
        {
          SelectedObject obj = { a, 0, 0 };
          wanted_apps.push_back( obj );
        }
      }
      else if( mode == CutterObjectLevel::CUT_OBJECT_TASK )
      {
        if( sscanf( token.c_str(), "%d.%d", &a, &t ) == 2 )
        {
          SelectedObject obj = { a, t, 0 };
          wanted_tasks.push_back( obj );
        }
      }
      else if( mode == CutterObjectLevel::CUT_OBJECT_THREAD )
      {
        // Try 1.1.1 format
        if( sscanf( token.c_str(), "%d.%d.%d", &a, &t, &th ) == 3 )
        {
          SelectedObject obj = { a, t, th };
          wanted_threads.push_back( obj );
        }
      }
    }
  }


  if( exec_options->max_trace_size != 0 )
    max_size = exec_options->max_trace_size * 1000000;

  break_states         = exec_options->break_states;
  remFirstStates       = exec_options->remFirstStates;
  remLastStates        = exec_options->remLastStates;
  keep_boundary_events = exec_options->keep_boundary_events;
  keep_all_events      = exec_options->keep_all_events;

  if( originalTime )
    break_states = false; // Assigned ONLY if originalTime == true, else KEEP value.

  if( break_states )
    keep_boundary_events = false;
}

void writeOffsetLineToFile( FILE *file, const std::string &line )
{
  fprintf( file, "%s\n", line.c_str() );
}

void writeOffsetLineToFile( std::fstream &file, const std::string &line )
{
  file << line << std::endl;
}

template< typename T >
void KTraceCutter::writeOffsetLine( T &whichFile,
                                    const char *trace_in_name,
                                    unsigned long long timeOffset,
                                    unsigned long long timeCutBegin,
                                    unsigned long long timeCutEnd )
{
  if( originalTime )
    timeOffset = 0;

  CutterMetadataAction tmpData( MetadataManager::getCurrentDate(),
                                cutterOriginCaller,
                                string( '"' + string( trace_in_name ) + '"' ),
                                (PRV_UINT64)timeOffset,   // first record of the trace
                                (PRV_UINT64)timeCutBegin, // first time of the cut
                                (PRV_UINT64)timeCutEnd ); // last time of the cut
  ostringstream tmpStream;
  tmpData.write( tmpStream );

  writeOffsetLineToFile( whichFile, tmpStream.str() );
}


void KTraceCutter::parseInHeaderAndDumpOut( TraceStream *whichFile, std::fstream &outfile )
{
  unsigned long long traceEndTime = 0;
  TTimeUnit traceTimeUnit         = NS;
  std::vector< std::string > communicators;

  string tmpDate;
  try
  {
    parseTraceHeader( *whichFile, tmpDate, traceTimeUnit, traceEndTime, traceResourceModel, traceProcessModel, communicators );
  }
  catch( TraceHeaderException &e )
  {
    throw e;
  }

  // Store original header metadata for use during header rebuild
  originalDate          = tmpDate;
  originalTimeUnit      = traceTimeUnit;
  originalCommunicators = communicators;

  // Build threadNodeMap from original ProcessModel
  // This maps (appl, task, thread) -> node (0-based) for all threads in original trace
  for( TApplOrder iAppl = 0; iAppl < traceProcessModel.totalApplications(); ++iAppl )
  {
    for( TTaskOrder iTask = traceProcessModel.getFirstTask( iAppl ); iTask <= traceProcessModel.getLastTask( iAppl ); ++iTask )
    {
      TApplOrder tmpAppl;
      TTaskOrder tmpTask;
      traceProcessModel.getTaskLocation( iTask, tmpAppl, tmpTask );

      for( TThreadOrder iThread = traceProcessModel.getFirstThread( iAppl, tmpTask ); iThread <= traceProcessModel.getLastThread( iAppl, tmpTask );
           ++iThread )
      {
        TApplOrder threadAppl;
        TTaskOrder threadTask;
        TThreadOrder threadThread;
        traceProcessModel.getThreadLocation( iThread, threadAppl, threadTask, threadThread );

        if( modify_header )
        {
          if( ( cut_apps && is_selected_appl( iAppl + 1 ) ) || ( cut_tasks && is_selected_task( iAppl + 1, threadTask + 1 ) ) ||
              ( cut_threads && is_selected_thread( iAppl + 1, threadTask + 1, threadThread + 1 ) ) )
          {
            writtenThreads.insert( { iAppl + 1, threadTask + 1, threadThread + 1 } );
          }
        }

        TNodeOrder node = traceProcessModel.getNode( iThread );
        // Store with 1-based IDs to match record format
        threadNodeMap[ { threadAppl + 1, threadTask + 1, threadThread + 1 } ] = node;
      }
    }
  }

  trace_time = traceEndTime;
  if( !by_time )
  {
    time_min   = ( (double)( trace_time / 100 ) ) * min_perc;
    time_max   = ( (double)( trace_time / 100 ) ) * max_perc;
    total_time = time_max - time_min;
  }

  if( !originalTime )
    traceEndTime = total_time;

  for( auto i = 0; i < traceProcessModel.totalApplications(); ++i )
    appsInfo.emplace_back( traceProcessModel.totalThreads( i ) );

  // Populate global thread offsets
  global_thread_offsets.resize( traceProcessModel.totalApplications() );
  unsigned int global_thread_count = 0;
  int app_idx                      = 0;
  for( auto itApp = traceProcessModel.cbegin(); itApp != traceProcessModel.cend(); ++itApp )
  {
    global_thread_offsets[ app_idx ].resize( itApp->size() );
    int task_idx = 0;
    for( auto itTask = itApp->cbegin(); itTask != itApp->cend(); ++itTask )
    {
      global_thread_offsets[ app_idx ][ task_idx ] = global_thread_count;
      global_thread_count += itTask->size();
      task_idx++;
    }
    app_idx++;
  }

  if( modify_header )
  {
    do_modify_header();
    dumpTraceHeader( outfile, tmpDate, traceEndTime, traceTimeUnit, traceResourceModel, choppedProcessModel, communicators );
  }
  else
    dumpTraceHeader( outfile, tmpDate, traceEndTime, traceTimeUnit, traceResourceModel, traceProcessModel, communicators );
}


template< typename IteratorType >
void KTraceCutter::dumpEventsSet( std::fstream &outfile,
                                  const IteratorType &begin,
                                  const IteratorType &end,
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

  if( !writtenComment )
  {
    outfile << "# Appending events with value 0\n";
    if( writeToTmpFile )
      ++total_tmp_lines;
    writtenComment = true;
  }

  for( IteratorType it = begin; it != end; ++it )
  {
    if( numWrittenChars == 0 )
    {
      // Write new line
      numWrittenChars += sprintf( buffer, "2:%d:%d:%d:%d:%lld:%lld:0", cpu, appl + 1, task + 1, thread + 1, final_time, (unsigned long long)*it );
      outfile << buffer;
      if( writeToTmpFile )
        ++total_tmp_lines;

      needEOL = true;
    }
    else if( numWrittenChars + 32 > MAX_LINE_SIZE )
    {
      // Too many events: close current line
      outfile << std::endl;
      numWrittenChars = 0;
      needEOL         = false;
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
    outfile << std::endl; // because we know theres's one at least.
    numWrittenChars = 0;
    needEOL         = false;
  }

  free( buffer );
}


void KTraceCutter::appendLastZerosToUnclosedEvents( const unsigned long long final_time, std::fstream &outfile )
{
  TCPUOrder cpu;
  int numWrittenChars = 0;
  bool needEOL        = false;
  bool writtenComment = false;

  for( CutterThreadInfo::iterator it = threadsInfo.begin(); it != threadsInfo.end(); ++it )
  {
    ThreadInfo tmpInfo  = ( *it ).second;
    unsigned int appl   = ( *it ).first.dim1;
    unsigned int task   = ( *it ).first.dim2;
    unsigned int thread = ( *it ).first.dim3;

    cpu = tmpInfo.lastCPU;

    numWrittenChars = 0;
    needEOL         = false;

    if( tmpInfo.openedEventTypes.size() > 0 )
    {
      std::set< TEventType > insertedUniqueOpenedTypes;
      std::vector< TEventType > uniqueOpenedTypes;

      std::copy_if( tmpInfo.openedEventTypes.begin(),
                    tmpInfo.openedEventTypes.end(),
                    std::back_inserter( uniqueOpenedTypes ),
                    [ &insertedUniqueOpenedTypes ]( auto elem )
                    {
                      if( insertedUniqueOpenedTypes.find( elem ) != insertedUniqueOpenedTypes.end() )
                        return false;
                      insertedUniqueOpenedTypes.insert( elem );
                      return true;
                    } );

      dumpEventsSet( outfile,
                     uniqueOpenedTypes.rbegin(),
                     uniqueOpenedTypes.rend(),
                     cpu,
                     appl,
                     task,
                     thread,
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
                     cpu,
                     appl,
                     task,
                     thread,
                     final_time,
                     numWrittenChars,
                     needEOL,
                     writtenComment );
    }
  }
}


void KTraceCutter::ini_cutter_progress_bar( const std::string &fileName, ProgressController *progress )
{
  total_trace_size = TraceStream::getTraceFileSize( fileName );

  /* Depen mida traça mostrem percentatge amb un interval diferent de temps */
  if( total_trace_size < 500000000 )
    total_cutter_iters = 10000;
  else
    total_cutter_iters = 100000;

  current_read_size = 0;

  if( progress != nullptr )
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

  if( progress != nullptr )
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
          if( total_tmp_lines == 0 )
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


void KTraceCutter::update_queue( unsigned int appl, unsigned int task, unsigned int thread, unsigned long long type, unsigned long long value )
{
  if( threadsInfo.find( appl, task, thread ) == threadsInfo.end() )
  {
    init_useful_tasks = true;
    ++useful_tasks;
    ThreadInfo newThreadInfo( HWCTypesInPCF );
    newThreadInfo.without_states      = true;
    threadsInfo( appl, task, thread ) = newThreadInfo;
  }

  if( notHWCTypesInPCF.find( type ) != notHWCTypesInPCF.end() )
    return;

  ThreadInfo &tmpInfo = threadsInfo( appl, task, thread );
  bool isHWC          = HWCTypesInPCF.find( type ) != HWCTypesInPCF.end();
  if( value > 0 )
  {
    if( isHWC )
      tmpInfo.HWCTypesInPRV.insert( (TEventType)type );
    else
      tmpInfo.openedEventTypes.push_back( (TEventType)type );
  }
  else
  {
    if( !isHWC )
    {
      auto it = std::find( tmpInfo.openedEventTypes.rbegin(), tmpInfo.openedEventTypes.rend(), (TEventType)type );
      if( it != tmpInfo.openedEventTypes.rend() )
        tmpInfo.openedEventTypes.erase( std::next( it ).base() ); // calling base() points resulting iterator to previous position
    }
  }
}

template< typename T, typename... Targs >
constexpr void dump_fields( fstream &file, T current_field )
{
  file << current_field;
}

template< typename T, typename... Targs >
constexpr void dump_fields( fstream &file, T current_field, Targs... Fargs )
{
  file << current_field << ":";
  dump_fields( file, Fargs... );
}

void KTraceCutter::do_modify_header()
{
  TApplOrder newAppl     = 0;
  TTaskOrder newTask     = 0;
  TThreadOrder newThread = 0;

  choppedProcessModel.setReady( true );

  // Sort threads to ensure consistent ordering
  std::vector< ObjectLocation > sortedThreads( writtenThreads.begin(), writtenThreads.end() );
  std::sort( sortedThreads.begin(), sortedThreads.end() );

  TApplOrder lastAppl     = 0;
  TTaskOrder lastTask     = 0;
  TThreadOrder lastThread = 0;

  std::string tmpRowLabel;

  auto copyRowLabelToNewRowFile = [ & ]( TTraceLevel whichLevel, auto whichObject )
  {
    tmpRowLabel = originalRowFile.getRowLabel( whichLevel, whichObject );
    if( tmpRowLabel != "" )
      newRowFile.pushBack( whichLevel, tmpRowLabel );
  };

  for( const auto &t : sortedThreads )
  {
    TApplOrder appl     = std::get< 0 >( t );
    TTaskOrder task     = std::get< 1 >( t );
    TThreadOrder thread = std::get< 2 >( t );

    // Ensure application exists
    if( choppedProcessModel.totalApplications() == 0 || appl != lastAppl )
    {
      choppedProcessModel.addApplication();
      ++newAppl;
      newTask = 0;
      copyRowLabelToNewRowFile( TTraceLevel::APPLICATION, appl );

      lastAppl   = appl;
      lastTask   = 0;
      lastThread = 0;
    }

    // Ensure task exists
    if( !choppedProcessModel.isValidTask( 0 ) || task != lastTask )
    {
      choppedProcessModel.addTask( newAppl - 1 );
      ++newTask;
      copyRowLabelToNewRowFile( TTraceLevel::TASK, task );

      lastTask   = task;
      lastThread = 0;
    }

    // Add thread with node from original model
    TNodeOrder node = 0;
    auto nodeIt     = threadNodeMap.find( { appl, task, thread } );
    if( nodeIt != threadNodeMap.end() )
      node = nodeIt->second;

    if( !choppedProcessModel.isValidThread( 0, 0, 0 ) || thread != lastThread )
    {
      newThread = choppedProcessModel.addThread( newAppl - 1, newTask - 1, node );

      originalToNewObjectLocation[ { appl, task, thread } ] = { newAppl, newTask, newThread + 1 };

      TThreadOrder tmpGlobalThread = traceProcessModel.getGlobalThread( appl - 1, task - 1, thread - 1 );
      copyRowLabelToNewRowFile( TTraceLevel::THREAD, tmpGlobalThread );

      lastThread = thread;
    }
  }

  for( TNodeOrder iNode = 0; iNode < traceResourceModel.totalNodes(); ++iNode )
    copyRowLabelToNewRowFile( TTraceLevel::NODE, iNode );

  for( TNodeOrder iCPU = 0; iCPU < traceResourceModel.totalCPUs(); ++iCPU )
    copyRowLabelToNewRowFile( TTraceLevel::CPU, iCPU );
}


// Substract to all the times in the trace the first time of the first record
// Doesn't change header
void KTraceCutter::shiftLeft_TraceTimes_ToStartFromZero( const char *originalTraceName,
                                                         const char *nameIn,
                                                         const char *nameOut,
                                                         ProgressController *progress )
{
  unsigned long long timeOffset = 0, time_1, time_2, time_3, time_4;
  int recordType, cpu, appl, task, thread, state, cpu_2, appl_2, task_2, thread_2;
  std::string line;
  TraceStream *infile = TraceStream::openFile( nameIn );

  std::string outfilePath( nameOut );
  fstream outfile( nameOut, ios_base::out );
  if( !outfile.is_open() )
  {
    int savedErrno     = errno;
    std::string errMsg = "Cannot open output file: " + outfilePath;
    if( savedErrno != 0 )
      errMsg += " (" + std::string( std::strerror( savedErrno ) ) + ")";
    throw std::runtime_error( errMsg );
  }

  /* Process header */
  total_time = last_record_time - first_record_time;

  // Skip the original header in the temp file (it used original topology)
  // We will write a new header based on actually-written objects
  std::string tmpLine;
  while( !infile->eof() && infile->good() )
  {
    std::streampos currentPos = infile->tellg();
    infile->getline( tmpLine );
    if( tmpLine.empty() )
      continue;
    // Stop when we hit the first non-comment, non-header line (record lines start with 1,2,3)
    if( tmpLine[ 0 ] >= '1' && tmpLine[ 0 ] <= '3' )
    {
      infile->seekg( currentPos );
      break;
    }
    // Also stop at comment lines that are not part of header (preserves provenance)
    // The header ends after communicator lines
  }

  if( modify_header )
  {
    dumpTraceHeader( outfile, originalDate, total_time, originalTimeUnit, traceResourceModel, choppedProcessModel, originalCommunicators );
  }
  else
  {
    dumpTraceHeader( outfile, originalDate, total_time, originalTimeUnit, traceResourceModel, traceProcessModel, originalCommunicators );
  }

  bool end_read = false;

  /* Read one more record if possible */
  if( infile->eof() )
    end_read = true;
  else
    infile->getline( line );

  // Get time of the first record ignoring any other field.
  if( !end_read )
    sscanf( line.c_str(), "%*d:%*d:%*d:%*d:%*d:%lld:", &timeOffset );

  // Override it: we have the minimum time of the written records.
  timeOffset        = first_record_time;
  current_tmp_lines = 0;

  writeOffsetLine( outfile, originalTraceName, timeOffset, time_min, time_max );

  while( !end_read )
  {
    show_cutter_progress_bar( progress, infile );

    auto itBegin = line.cbegin();
    auto itEnd   = line.cend();
    switch( line[ 0 ] )
    {
      case '1':
        if( !prv_atoll_v( itBegin, itEnd, recordType, cpu, appl, task, thread, time_1, time_2, state ) )
        {
          std::cerr << "Invalid record: " << line << std::endl;
          break;
        }

        time_1 = time_1 - timeOffset;
        time_2 = time_2 - timeOffset;

        dump_fields( outfile, recordType, cpu, appl, task, thread, time_1, time_2, state );
        outfile << "\n";

        ++current_tmp_lines;
        break;

      case '2':
        if( !prv_atoll_v( itBegin, itEnd, recordType, cpu, appl, task, thread, time_1 ) )
        {
          std::cerr << "Invalid record: " << line << std::endl;
          break;
        }

        time_1 = time_1 - timeOffset;

        dump_fields( outfile, recordType, cpu, appl, task, thread, time_1 );
        outfile << ":";
        std::for_each( itBegin,
                       itEnd,
                       [ &outfile ]( auto c )
                       {
                         outfile.put( c );
                       } );
        outfile << "\n";

        ++current_tmp_lines;
        break;

      case '3':
        if( !prv_atoll_v( itBegin, itEnd, recordType, cpu, appl, task, thread, time_1, time_2, cpu_2, appl_2, task_2, thread_2, time_3, time_4 ) )
        {
          std::cerr << "Invalid record: " << line << std::endl;
          break;
        }

        time_1 = time_1 - timeOffset;
        time_2 = time_2 - timeOffset;
        time_3 = time_3 - timeOffset;
        time_4 = time_4 - timeOffset;

        dump_fields( outfile, recordType, cpu, appl, task, thread, time_1, time_2, cpu_2, appl_2, task_2, thread_2, time_3, time_4 );
        outfile << ":";
        std::for_each( itBegin,
                       itEnd,
                       [ &outfile ]( auto c )
                       {
                         outfile.put( c );
                       } );
        outfile << "\n";

        ++current_tmp_lines;
        break;

      case '#':
        outfile << line << "\n";
        ++current_tmp_lines;
        break;

      default:
        break;
    }

    /* Read one more record if possible */
    if( infile->eof() )
      end_read = true;
    else
      infile->getline( line );
  }

  infile->close();
  checkStreamWrite( outfile, outfilePath );
  outfile.close();

  std::string tmpFilePath( nameIn );
  if( unlink( nameIn ) != 0 )
  {
    int savedErrno     = errno;
    std::string errMsg = "Failed to delete temporary file: " + tmpFilePath;
    if( savedErrno != 0 )
      errMsg += " (" + std::string( std::strerror( savedErrno ) ) + ")";
    throw std::runtime_error( errMsg );
  }
}


KTraceCutter::ThreadInfo &KTraceCutter::initThreadInfo( unsigned int appl, unsigned int task, unsigned int thread, unsigned int cpu )
{
  ++useful_tasks;
  init_useful_tasks = true;
  ThreadInfo newThreadInfo( HWCTypesInPCF );
  ThreadInfo &tmpInfo = threadsInfo( appl - 1, task - 1, thread - 1 );
  tmpInfo             = newThreadInfo;
  tmpInfo.lastCPU     = cpu;
  tmpInfo.last_time   = 0;

  return tmpInfo;
}

void KTraceCutter::execute( std::string trace_in, std::string trace_out, ProgressController *progress )
{
  TraceStream *inFile;
  string trace_header;
  string line;
  string tmp_dir;
  char *trace_file_out;
  bool end_parsing = false;

  unsigned int id, cpu, appl, task, thread, state, cpu_2, appl_2, task_2, thread_2, size, tag;
  unsigned long long type, time_1, time_2, time_3, time_4;
  TEventValue value;
  int i;

  unsigned long num_iters = 0;

  KProgressController *tmpKProgressControler = nullptr;

  std::string rowFileName    = LocalKernel::composeName( trace_in, "row" );
  std::string newRowFileName = LocalKernel::composeName( trace_out, "row" );

  originalRowFile = RowFileParser<>( rowFileName );

  if( progress != nullptr )
    tmpKProgressControler = (KProgressController *)progress->getConcrete();

  trace_file_out = (char *)malloc( sizeof( char ) * MAX_FILENAME_SIZE );

  by_time           = false;
  originalTime      = false;
  max_size          = 0;
  cut_tasks         = false;
  break_states      = true;
  init_useful_tasks = false;
  useful_tasks      = 0;
  first_time_caught = false;
  current_size      = 0;

  // Clear tracking sets for object cutting header rebuild
  writtenThreads.clear();
  threadNodeMap.clear();

  /* Reading of the program arguments */
  read_cutter_params();

  inFile = TraceStream::openFile( trace_in );

  writeToTmpFile = break_states || !originalTime;
  if( writeToTmpFile )
  {
    tmp_dir = ParaverConfig::getInstance()->getGlobalTmpPath();

#ifdef _WIN32
    sprintf( trace_file_out, "%s\\tmp_fileXXXXXX", tmp_dir.c_str() );
    if( _mktemp_s( trace_file_out, strlen( trace_file_out ) + 1 ) != 0 )
    {
      int savedErrno     = errno;
      std::string errMsg = "Failed to create temporary file in: " + tmp_dir;
      if( savedErrno != 0 )
        errMsg += " (" + std::string( std::strerror( savedErrno ) ) + ")";
      throw std::runtime_error( errMsg );
    }
#else
    sprintf( trace_file_out, "%s/tmp_fileXXXXXX", tmp_dir.c_str() );
    int tmpFd = mkstemp( trace_file_out );
    if( tmpFd == -1 )
    {
      int savedErrno     = errno;
      std::string errMsg = "Failed to create temporary file in: " + tmp_dir;
      if( savedErrno != 0 )
        errMsg += " (" + std::string( std::strerror( savedErrno ) ) + ")";
      throw std::runtime_error( errMsg );
    }
    close( tmpFd ); // Close fd, will reopen via fstream
#endif
  }
  else
    strcpy( trace_file_out, trace_out.c_str() );

  std::string outfilePath( trace_file_out );
  fstream outfile( trace_file_out, ios_base::out );
  if( !outfile.is_open() )
  {
    int savedErrno     = errno;
    std::string errMsg = "Cannot open output file: " + outfilePath;
    if( savedErrno != 0 )
      errMsg += " (" + std::string( std::strerror( savedErrno ) ) + ")";
    throw std::runtime_error( errMsg );
  }

  ini_cutter_progress_bar( trace_in, tmpKProgressControler );

  /* Process header */
  parseInHeaderAndDumpOut( inFile, outfile );

  if( !modify_header )
  {
    newRowFile = originalRowFile;
  }

  if( !newRowFile.empty() )
    newRowFile.dumpToFile( newRowFileName );

  if( !writeToTmpFile )
  {
    writeOffsetLine( outfile, trace_in.c_str(), time_min, time_min, time_max );
  }

  /* We process the trace like the originalTime version */

  bool maxTimeReached = false;
  last_record_time    = 0;
  total_tmp_lines     = 0;
  secondPhase         = false;

  if( tmpKProgressControler != nullptr )
    end_parsing = tmpKProgressControler->getStop();

  TApplOrder newAppl     = 0;
  TTaskOrder newTask     = 0;
  TThreadOrder newThread = 0;

  TApplOrder newAppl_2     = 0;
  TTaskOrder newTask_2     = 0;
  TThreadOrder newThread_2 = 0;

  /* Processing the trace records */
  while( !end_parsing && !maxTimeReached )
  {
    if( tmpKProgressControler != nullptr )
    {
      end_parsing = tmpKProgressControler->getStop();
      if( end_parsing )
        continue;
    }

    /* Read one more record is possible */
    if( inFile->eof() || !inFile->good() )
    {
      end_parsing = true;

      continue;
    }

    inFile->getline( line );

    if( num_iters == total_cutter_iters )
    {
      show_cutter_progress_bar( tmpKProgressControler, inFile );
      num_iters = 0;
    }
    else
      ++num_iters;

    CutterThreadInfo::iterator threadInfoIt;

    auto itBegin = line.cbegin();
    auto itEnd   = line.cend();
    switch( line[ 0 ] )
    {
      case '1':
        if( !prv_atoll_v( itBegin, itEnd, id, cpu, appl, task, thread, time_1, time_2, state ) )
        {
          std::cerr << "Invalid record: " << line << std::endl;
          break;
        }

        // MESS
        if( exec_options->get_max_cut_time_to_finish_of_first_appl() && firstApplicationFinished &&
            ( time_1 >= timeOfFirsApplicationFinished || ( time_2 >= timeOfFirsApplicationFinished && !break_states ) ) )
          break;

        if( cut_apps && !is_selected_appl( appl ) )
          break;
        if( cut_tasks && !is_selected_task( appl, task ) )
          break;
        if( cut_threads && !is_selected_thread( appl, task, thread ) )
          break;

        if( time_2 <= time_min )
          break;

        threadInfoIt = threadsInfo.find( appl - 1, task - 1, thread - 1 );

        if( time_1 < time_min && time_2 > time_max && ( remFirstStates || remLastStates ) )
        {
          ThreadInfo *tmpInfo = nullptr;
          if( threadInfoIt == threadsInfo.end() )
            tmpInfo = &initThreadInfo( appl, task, thread, cpu );
          else
            tmpInfo = &threadInfoIt->second;

          tmpInfo->without_states = true;

          break;
        }

        if( time_1 < time_min && time_2 >= time_min && remFirstStates )
          break;

        if( time_1 < time_max && time_2 > time_max && remLastStates || originalTime && time_1 > time_max )
        {
          if( threadInfoIt != threadsInfo.end() && !threadsInfo( appl - 1, task - 1, thread - 1 ).finished )
          {
            --useful_tasks;
            threadsInfo( appl - 1, task - 1, thread - 1 ).finished = true;

            // MESS
            if( exec_options->get_max_cut_time_to_finish_of_first_appl() )
            {
              if( appl == 1 && appsInfo[ appl - 1 ].addFinishedThread() )
              {
                firstApplicationFinished      = true;
                timeOfFirsApplicationFinished = threadsInfo( appl - 1, task - 1, thread - 1 ).lastStateEndTime;
              }
            }
          }

          break;
        }


        if( !originalTime && time_1 > time_max )
        {
          maxTimeReached = true;
        }
        else // originalTime || time_1 <= time_max
        {
          if( threadInfoIt == threadsInfo.end() )
            initThreadInfo( appl, task, thread, cpu );

          threadsInfo( appl - 1, task - 1, thread - 1 ).without_states = false;

          threadsInfo( appl - 1, task - 1, thread - 1 ).lastStateEndTime = time_2;
          if( time_1 < time_max && time_2 > time_max && !remLastStates && !break_states && keep_boundary_events )
          {
            threadsInfo( appl - 1, task - 1, thread - 1 ).finished = true;

            // MESS
            if( exec_options->get_max_cut_time_to_finish_of_first_appl() )
            {
              if( appl == 1 && appsInfo[ appl - 1 ].addFinishedThread() )
              {
                firstApplicationFinished      = true;
                timeOfFirsApplicationFinished = threadsInfo( appl - 1, task - 1, thread - 1 ).lastStateEndTime;
              }
            }
          }

          if( !originalTime && break_states )
          {
            if( time_1 < time_min )
            {
              time_1 = time_min;
            }

            if( time_2 > time_max )
            {
              time_2 = time_max;
            }
          }

          // if ( threadsInfo[appl-1][task-1][thread-1]->last_time < time_2 )
          threadsInfo( appl - 1, task - 1, thread - 1 ).last_time = time_2;

          if( !first_time_caught )
          {
            first_record_time = time_1;
            first_time_caught = true;
          }
          else
          {
            if( time_1 < first_record_time )
              first_record_time = time_1;
          }

          if( time_2 > last_record_time )
            last_record_time = time_2;

          if( modify_header )
            std::tie( newAppl, newTask, newThread ) = originalToNewObjectLocation[ { appl, task, thread } ];
          else
          {
            newAppl   = appl;
            newTask   = task;
            newThread = thread;
          }

          dump_fields( outfile, id, cpu, newAppl, newTask, newThread, time_1, time_2, state );
          outfile << "\n";
          if( writeToTmpFile )
            ++total_tmp_lines;
        }

        break;

      case '2':
        if( !prv_atoll_v( itBegin, itEnd, id, cpu, appl, task, thread, time_1 ) )
        {
          std::cerr << "Invalid record: " << line << std::endl;
          break;
        }

        // MESS
        if( exec_options->get_max_cut_time_to_finish_of_first_appl() && firstApplicationFinished && time_1 >= timeOfFirsApplicationFinished )
          break;

        if( cut_apps && !is_selected_appl( appl ) )
          break;
        if( cut_tasks && !is_selected_task( appl, task ) )
          break;
        if( cut_threads && !is_selected_thread( appl, task, thread ) )
          break;

        threadInfoIt = threadsInfo.find( appl - 1, task - 1, thread - 1 );

        if( threadInfoIt != threadsInfo.end() && threadInfoIt->second.finished && time_1 != threadInfoIt->second.lastStateEndTime )
        {
          if( !( !remLastStates && !break_states && keep_boundary_events && time_1 <= threadInfoIt->second.lastStateEndTime ) )
            break;
        }
        else if( time_1 > time_max )
          break;

        if( ( threadInfoIt != threadsInfo.end() ) && ( time_1 > threadInfoIt->second.last_time ) && ( time_1 > time_max ) && !keep_boundary_events )
          break;

        if( threadInfoIt == threadsInfo.end() && time_1 > time_max )
          break;

        // TODO: keep_all_events
        if( threadInfoIt == threadsInfo.end() && remFirstStates )
          break; // ?


        /* If time inside cut, adjust time */
        if( time_1 >= time_min ||
            ( time_1 < time_min && threadInfoIt != threadsInfo.end() && threadInfoIt->second.last_time >= time_min && keep_boundary_events ) ||
            ( first_time_caught && time_1 >= first_record_time && threadInfoIt != threadsInfo.end() && threadInfoIt->second.without_states &&
              keep_all_events ) )
        {
          if( !first_time_caught )
          {
            first_record_time = time_1;
            first_time_caught = true;
          }
          else
          {
            if( time_1 < first_record_time )
              first_record_time = time_1;
          }

          if( time_1 > last_record_time )
            last_record_time = time_1;

          if( modify_header )
            std::tie( newAppl, newTask, newThread ) = originalToNewObjectLocation[ { appl, task, thread } ];
          else
          {
            newAppl   = appl;
            newTask   = task;
            newThread = thread;
          }

          dump_fields( outfile, id, cpu, newAppl, newTask, newThread, time_1 );

          while( itBegin != itEnd )
          {
            if( !prv_atoll_v( itBegin, itEnd, type, value ) )
            {
              std::cerr << "Invalid event: " << line << std::endl;
              continue;
            }

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

          if( writeToTmpFile )
            ++total_tmp_lines;

          /* For closing all the opened calls */
          threadsInfo( appl - 1, task - 1, thread - 1 ).last_time = time_1;
          threadsInfo( appl - 1, task - 1, thread - 1 ).lastCPU   = cpu;
        }

        break;

      case '3':
        if( !prv_atoll_v( itBegin, itEnd, id, cpu, appl, task, thread, time_1, time_2, cpu_2, appl_2, task_2, thread_2, time_3, time_4, size, tag ) )
        {
          std::cerr << "Invalid record: " << line << std::endl;
          break;
        }

        // MESS
        if( exec_options->get_max_cut_time_to_finish_of_first_appl() && firstApplicationFinished &&
            ( time_1 >= timeOfFirsApplicationFinished || time_2 >= timeOfFirsApplicationFinished || time_3 >= timeOfFirsApplicationFinished ||
              time_4 >= timeOfFirsApplicationFinished ) )
          break;

        if( cut_apps && !is_selected_appl( appl ) )
          break;
        if( cut_tasks && !is_selected_task( appl, task ) )
          break;
        if( cut_threads && !is_selected_thread( appl, task, thread ) )
          break;

        if( cut_apps && !is_selected_appl( appl_2 ) )
          break;
        if( cut_tasks && !is_selected_task( appl_2, task_2 ) )
          break;
        if( cut_threads && !is_selected_thread( appl_2, task_2, thread_2 ) )
          break;

        if( time_1 >= time_min && time_3 >= time_min )
        {
          /* if time outside the cut, finish */
          if( time_1 > time_max && !originalTime ) // PROBLEM: DISORDERED TRACES
          {
            maxTimeReached = true;
          }
          else if( time_2 <= time_max && time_4 <= time_max )
          {
            vector< unsigned long long > times;

            times.push_back( time_1 );
            times.push_back( time_2 );
            times.push_back( time_3 );
            times.push_back( time_4 );
            sort( times.begin(), times.end() );

            if( !first_time_caught )
            {
              first_record_time = times[ 0 ];
              first_time_caught = true;
            }
            else
            {
              if( times[ 0 ] < first_record_time )
                first_record_time = times[ 0 ];
            }

            if( times[ 3 ] > last_record_time )
              last_record_time = times[ 3 ];

            if( modify_header )
            {
              std::tie( newAppl, newTask, newThread )       = originalToNewObjectLocation[ { appl, task, thread } ];
              std::tie( newAppl_2, newTask_2, newThread_2 ) = originalToNewObjectLocation[ { appl_2, task_2, thread_2 } ];
            }
            else
            {
              newAppl   = appl;
              newTask   = task;
              newThread = thread;

              newAppl_2   = appl_2;
              newTask_2   = task_2;
              newThread_2 = thread_2;
            }

            dump_fields( outfile,
                         id,
                         cpu,
                         newAppl,
                         newTask,
                         newThread,
                         time_1,
                         time_2,
                         cpu_2,
                         newAppl_2,
                         newTask_2,
                         newThread_2,
                         time_3,
                         time_4,
                         size,
                         tag );
            outfile << "\n";
            if( writeToTmpFile )
              ++total_tmp_lines;
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

    if( max_size > 0 )
      if( max_size <= outfile.tellg() )
        break;

    if( init_useful_tasks && useful_tasks == 0 )
      break;
  }

  if( tmpKProgressControler != nullptr && tmpKProgressControler->getStop() )
    tmpKProgressControler->setMessage( "Finishing cut..." );

  if( last_record_time > time_max || remLastStates )
    // if ( !originalTime )
    appendLastZerosToUnclosedEvents( last_record_time, outfile );
  else
    appendLastZerosToUnclosedEvents( time_max, outfile );

  /* Close the files */
  inFile->close();
  checkStreamWrite( outfile, outfilePath );
  outfile.close();

  if( writeToTmpFile ) // trace_file_out is a tmpfile!!
  {
    secondPhase = true;
    shiftLeft_TraceTimes_ToStartFromZero( trace_in.c_str(), trace_file_out, trace_out.c_str(), tmpKProgressControler );
  }

  free( trace_file_out );
}

bool KTraceCutter::is_selected_appl( int appl_id )
{
  for( const auto &obj : wanted_apps )
  {
    if( obj.appl == appl_id )
      return true;
  }
  return false;
}

bool KTraceCutter::is_selected_task( int appl_id, int task_id )
{
  for( const auto &obj : wanted_tasks )
  {
    if( obj.appl == appl_id && obj.task == task_id )
      return true;
  }
  return false;
}

bool KTraceCutter::is_selected_thread( int appl_id, int task_id, int thread_id )
{
  for( const auto &obj : wanted_threads )
  {
    if( obj.appl == appl_id && obj.task == task_id && obj.thread == thread_id )
      return true;
  }
  return false;
}
