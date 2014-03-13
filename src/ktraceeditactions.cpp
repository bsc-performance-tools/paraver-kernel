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

#ifndef WIN32
#include <unistd.h>
#include <errno.h>
#else
#include <io.h>
#endif
#include <iostream>
#include "ktraceeditactions.h"
#include "ktraceeditsequence.h"
#include "traceeditstates.h"
#include "tracecutter.h"
#include "kernelconnection.h"
#include "textoutput.h"
#include "traceoptions.h"
#include "ParaverMetadataManager.h"
#include "ktrace.h"

/****************************************************************************
 ********                  TestAction                                ********
 ****************************************************************************/

void TestAction::execute( std::string whichTrace )
{
  ((KTraceEditSequence *)mySequence)->executeNextAction( whichTrace );
}

vector<TraceEditSequence::TSequenceStates> TestAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  tmpStates.push_back( TraceEditSequence::testState );
  return tmpStates;
}


/****************************************************************************
 ********                  TraceCutterAction                         ********
 ****************************************************************************/

vector<TraceEditSequence::TSequenceStates> TraceCutterAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  tmpStates.push_back( TraceEditSequence::traceOptionsState );
  tmpStates.push_back( TraceEditSequence::csvWindowState );
  return tmpStates;
}


void TraceCutterAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;
  Window *tmpWindow = ( (CSVWindowState *)tmpSequence->getState( TraceEditSequence::csvWindowState ) )->getData();
  TraceOptions *options = ( (TraceOptionsState *)tmpSequence->getState( TraceEditSequence::traceOptionsState ) )->getData();
  std::string tmpSuffix = ( (OutputDirSuffixState *)tmpSequence->getState( TraceEditSequence::outputDirSuffixState ) )->getData();
  std::string outputPath = whichTrace.substr( 0, whichTrace.find_last_of( mySequence->getKernelConnection()->getPathSeparator() ) ) +
                           mySequence->getKernelConnection()->getPathSeparator() + tmpSuffix;

  vector< std::string > tmpID;
  tmpID.push_back( TraceCutter::getID() );
  std::string newName = mySequence->getKernelConnection()->getNewTraceName( whichTrace, outputPath, tmpID, false );

#ifndef WIN32
  if( tmpWindow != NULL && options->get_min_cutting_time() == 0 && options->get_max_cutting_time() >= tmpWindow->getTrace()->getEndTime() )
  {
    newName = outputPath + mySequence->getKernelConnection()->getPathSeparator() +
              whichTrace.substr( whichTrace.find_last_of( mySequence->getKernelConnection()->getPathSeparator() ) );
    if( symlink( whichTrace.c_str(), newName.c_str() ) != 0 )
    {
      if( errno != EEXIST )
      {
        TraceCutter *myCutter = TraceCutter::create( mySequence->getKernelConnection(),
                                                     (char *)whichTrace.c_str(),
                                                     (char *)newName.c_str(),
                                                     options,
                                                     NULL );
        myCutter->setCutterApplicationCaller( CutterMetadata::RUNAPP_APPLICATION_ID );
        myCutter->execute( (char *)whichTrace.c_str(), (char *)newName.c_str(), NULL );
      }
    }
  }
  else
  {
#endif
    TraceCutter *myCutter = TraceCutter::create( mySequence->getKernelConnection(),
                                                (char *)whichTrace.c_str(),
                                                (char *)newName.c_str(),
                                                options,
                                                NULL );
    myCutter->setCutterApplicationCaller( CutterMetadata::RUNAPP_APPLICATION_ID );
    myCutter->execute( (char *)whichTrace.c_str(), (char *)newName.c_str(), NULL );
#ifndef WIN32
  }
#endif

  mySequence->getKernelConnection()->copyPCF( whichTrace, newName );
  mySequence->getKernelConnection()->copyROW( whichTrace, newName );

  tmpSequence->executeNextAction( newName );
}


/****************************************************************************
 ********                  CSVOutputAction                           ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> CSVOutputAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  tmpStates.push_back(  TraceEditSequence::csvWindowState );
  tmpStates.push_back(  TraceEditSequence::csvFileNameState );
  return tmpStates;
}


void CSVOutputAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;
  Window *tmpWindow = ( (CSVWindowState *)tmpSequence->getState( TraceEditSequence::csvWindowState ) )->getData();
  std::string tmpFileName = ( (CSVFileNameState *)tmpSequence->getState( TraceEditSequence::csvFileNameState ) )->getData();
  TextOutput output = ( (CSVOutputState *)tmpSequence->getState( TraceEditSequence::csvOutputState ) )->getData();

  output.dumpWindow( tmpWindow, tmpFileName );

  TraceOptions *options = ( (TraceOptionsState *)tmpSequence->getState( TraceEditSequence::traceOptionsState ) )->getData();
  if( options != NULL )
  {
    options->set_min_cutting_time( output.getMinTime() );
    options->set_max_cutting_time( output.getMaxTime() );
  }

  tmpSequence->executeNextAction( whichTrace );
}


/****************************************************************************
 ********               TraceShifterTimesLoaderAction                ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> TraceShifterTimesLoaderAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;

  tmpStates.push_back( TraceEditSequence::shiftTimesState );

  return tmpStates;
}


void TraceShifterTimesLoaderAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;
  MemoryTrace::iterator *it = NULL;

  tmpSequence->executeNextAction( it );
}


/****************************************************************************
 ********                  TraceParserAction                         ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> TraceParserAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;

  tmpStates.push_back( TraceEditSequence::maxTraceTimeState );
  tmpStates.push_back( TraceEditSequence::eofParsedState );

  return tmpStates;
}


void TraceParserAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;

  KTrace myTrace( whichTrace, NULL, true );
  tmpSequence->setCurrentTrace( &myTrace );

  MemoryTrace::iterator *it = myTrace.begin();
  while( !it->isNull() )
  {
    tmpSequence->executeNextAction( it );
    ++(*it);
  }

  delete it;

  // Final dummy record
  it = myTrace.empty();
  EOFParsedState *tmpEOFParseState = (EOFParsedState *)tmpSequence->getState( TraceEditSequence::eofParsedState );
  tmpEOFParseState->setData( true );
  tmpSequence->executeNextAction( it );

  delete it;
}


/****************************************************************************
 ********                 RecordTimeShifterAction                    ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> RecordTimeShifterAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;

  tmpStates.push_back( TraceEditSequence::maxTraceTimeState );
  tmpStates.push_back( TraceEditSequence::shiftTimesState );
  tmpStates.push_back( TraceEditSequence::shiftLevelState );

  return tmpStates;
}


void RecordTimeShifterAction::execute( MemoryTrace::iterator *whichRecord )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;

  std::vector< TTime > *shiftTimes =
          ( (ShiftTimesState *)tmpSequence->getState( TraceEditSequence::shiftTimesState ) )->getData();
  TWindowLevel shiftLevel =
          ( (ShiftLevelState *)tmpSequence->getState( TraceEditSequence::shiftLevelState ) )->getData();

  TTime delta = (TTime)0;
  TObjectOrder objects;
  switch ( shiftLevel )
  {

    case TASK:
      objects = tmpSequence->getCurrentTrace()->totalTasks();
      if ( shiftTimes->size() == objects )
        delta = (*shiftTimes)[ whichRecord->getOrder() ];
      break;

    case APPLICATION:
      objects = tmpSequence->getCurrentTrace()->totalApplications();
      if ( shiftTimes->size() == objects )
        delta = (*shiftTimes)[ whichRecord->getOrder() ];
      break;

//    case WORKLOAD:
//    case THREAD:
//      break;
    default:
      objects = tmpSequence->getCurrentTrace()->totalThreads();
      if ( shiftTimes->size() == objects )
        delta = (*shiftTimes)[ whichRecord->getOrder() ];
      break;
  }

  TTime shiftedTime = whichRecord->getTime() + delta;
  whichRecord->setTime( shiftedTime );

  tmpSequence->executeNextAction( whichRecord );
}


/****************************************************************************
 ********                  TraceWriterAction                         ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> TraceWriterAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;

  tmpStates.push_back( TraceEditSequence::outputTraceFileNameState );
  tmpStates.push_back( TraceEditSequence::maxTraceTimeState );

  return tmpStates;
}


void TraceWriterAction::execute( MemoryTrace::iterator *it  )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;

  if( !outputTrace.is_open() )
  {
    std::string tmpFileName = ( (OutputTraceFileNameState *)tmpSequence->getState( TraceEditSequence::outputTraceFileNameState ) )->getData();
    outputTrace.open( tmpFileName.c_str(), std::ios::out );
    tmpSequence->getCurrentTrace()->dumpFileHeader( outputTrace );
  }

  bool eofParsed = ( (EOFParsedState *)tmpSequence->getState( TraceEditSequence::eofParsedState ) )->getData();

  if ( ( it->getType() == STATE + BEGIN ) ||
       ( it->getType() == EVENT ) ||
       ( it->getType() == COMM + LOG + SEND ) ||
       ( eofParsed )
     )
  {
    body.write( outputTrace, *tmpSequence->getCurrentTrace(), it );
  }

  if ( eofParsed && outputTrace.is_open() )
    outputTrace.close();

  tmpSequence->executeNextAction( it );
}


/****************************************************************************
 ********               EventDrivenCutterAction                      ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> EventDrivenCutterAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;

  tmpStates.push_back( TraceEditSequence::outputTraceFileNameState );

  return tmpStates;
}


void EventDrivenCutterAction::execute( MemoryTrace::iterator *it  )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;
  PRV_UINT32 currentFile;

  if( outputTraces.empty() )
  {
    std::fstream *tmpStream = new std::fstream;
    outputTraces.push_back( tmpStream );
    currentThreadFile.insert( currentThreadFile.begin(), ( size_t )tmpSequence->getCurrentTrace()->totalThreads(), 0 );
    countThreadsPerFile[ 0 ] = tmpSequence->getCurrentTrace()->totalThreads();
  }

  if( ( it->getType() == EVENT ) &&
      ( it->getEventType() == 1 /*TODO get event type from state*/ ) )
  {
    if( countThreadsPerFile.count( currentThreadFile[ it->getThread() ] ) > 0 )
      --countThreadsPerFile[ currentThreadFile[ it->getThread() ] ];

    ++currentThreadFile[ it->getThread() ];

    if( countThreadsPerFile.count( currentThreadFile[ it->getThread() ] ) > 0 )
      ++countThreadsPerFile[ currentThreadFile[ it->getThread() ] ];
    else
      countThreadsPerFile[ currentThreadFile[ it->getThread() ] ] = 1;
  }

  currentFile = currentThreadFile[ it->getThread() ];

  if( !outputTraces[ currentFile ]->is_open() )
  {
    std::string tmpFileName = ( (OutputTraceFileNameState *)tmpSequence->getState( TraceEditSequence::outputTraceFileNameState ) )->getData();
// TODO filename suffix
    //tmpNameOut = myKernel->getNewTraceName( fullTmpNameIn, filterToolOrder, true );

    outputTraces[ currentFile ]->open( tmpFileName.c_str(), std::ios::out );
    tmpSequence->getCurrentTrace()->dumpFileHeader( *outputTraces[ currentFile ] );
  }

  bool eofParsed = ( (EOFParsedState *)tmpSequence->getState( TraceEditSequence::eofParsedState ) )->getData();

  if ( ( it->getType() == STATE + BEGIN ) ||
       ( it->getType() == EVENT ) ||
       ( it->getType() == COMM + LOG + SEND ) ||
       ( eofParsed )
     )
  {
    body.write( *outputTraces[ currentFile ], *tmpSequence->getCurrentTrace(), it );
  }

  if ( eofParsed )
  {
    for( vector<std::fstream *>::iterator it; it != outputTraces.end(); ++it )
    {
      if( *it != NULL && (*it)->is_open() )
      {
        (*it)->close();
        delete *it;
        *it = NULL;
      }
    }
  }

  tmpSequence->executeNextAction( it );
}


/****************************************************************************
 ********                  TraceSortAction                           ********
 ****************************************************************************/
vector<TraceEditSequence::TSequenceStates> TraceSortAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;

  return tmpStates;
}


void TraceSortAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;

  tmpSequence->executeNextAction( whichTrace );
}
