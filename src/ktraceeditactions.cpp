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


#ifndef _WIN32
#include <unistd.h>
#include <errno.h>
#else
#include <io.h>
#endif
#include <iostream>
#include <sstream>
#include <set>
#include "ktraceeditactions.h"
#include "ktraceeditsequence.h"
#include "traceeditstates.h"
#include "tracecutter.h"
#include "tracefilter.h"
#include "eventtranslator.h"
#include "kernelconnection.h"
#include "textoutput.h"
#include "traceoptions.h"
#include "ParaverMetadataManager.h"
#include "ktrace.h"

using std::set;

/****************************************************************************
 ********                  TestAction                                ********
 ****************************************************************************/

vector<TSequenceStates> TestAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  tmpStates.push_back( TSequenceStates::testState );
  return tmpStates;
}


bool  TestAction::execute( std::string whichTrace )
{
  ((KTraceEditSequence *)mySequence)->executeNextAction( whichTrace );

  return true;
}


/****************************************************************************
 ********                  TraceCutterAction                         ********
 ****************************************************************************/

vector<TSequenceStates> TraceCutterAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  tmpStates.push_back( TSequenceStates::traceOptionsState );
  tmpStates.push_back( TSequenceStates::sourceTimelineState );
  return tmpStates;
}


bool TraceCutterAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;
  Timeline *tmpWindow = ( (SourceTimelineState *)tmpSequence->getState( TSequenceStates::sourceTimelineState ) )->getData();
  TraceOptions *options = ( (TraceOptionsState *)tmpSequence->getState( TSequenceStates::traceOptionsState ) )->getData();

  std::string tmpSuffix = ( (OutputDirSuffixState *)tmpSequence->getState( TSequenceStates::outputDirSuffixState ) )->getData();
  std::string outputPath = whichTrace.substr( 0, whichTrace.find_last_of( mySequence->getKernelConnection()->getPathSeparator() ) ) +
                           mySequence->getKernelConnection()->getPathSeparator() + tmpSuffix;
  vector< std::string > tmpID;
  tmpID.push_back( TraceCutter::getID() );
  std::string newName = mySequence->getKernelConnection()->getNewTraceName( whichTrace, outputPath, tmpID, false );

#ifndef _WIN32
  if( tmpWindow != nullptr && options->get_min_cutting_time() == 0 && options->get_max_cutting_time() >= tmpWindow->getTrace()->getEndTime() )
  {
    newName = outputPath + mySequence->getKernelConnection()->getPathSeparator() +
              whichTrace.substr( whichTrace.find_last_of( mySequence->getKernelConnection()->getPathSeparator() ) );
    std::string relativeTrace = "../" + whichTrace.substr( whichTrace.find_last_of( mySequence->getKernelConnection()->getPathSeparator() ) );
    if( symlink( relativeTrace.c_str(), newName.c_str() ) != 0 )
    {
      if( errno != EEXIST )
      {
        TraceCutter *myCutter = TraceCutter::create( mySequence->getKernelConnection(),
                                                     whichTrace,
                                                     newName,
                                                     options,
                                                     nullptr );
        myCutter->setCutterApplicationCaller( CutterMetadata::RUNAPP_APPLICATION_ID );
        myCutter->execute( whichTrace, newName, nullptr );
      }
    }
  }
  else
  {
#endif
    TraceCutter *myCutter = TraceCutter::create( mySequence->getKernelConnection(),
                                                whichTrace,
                                                newName,
                                                options,
                                                nullptr );
    myCutter->setCutterApplicationCaller( CutterMetadata::RUNAPP_APPLICATION_ID );
    myCutter->execute( whichTrace, newName, nullptr );
#ifndef _WIN32
  }
#endif

  mySequence->getKernelConnection()->copyPCF( whichTrace, newName );
  mySequence->getKernelConnection()->copyROW( whichTrace, newName );

  tmpSequence->executeNextAction( newName );

  return true;
}


/****************************************************************************
 ********                  TraceFilterAction                         ********
 ****************************************************************************/

vector<TSequenceStates> TraceFilterAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  tmpStates.push_back( TSequenceStates::onlyFilterState );
  tmpStates.push_back( TSequenceStates::traceOptionsState );
  tmpStates.push_back( TSequenceStates::eventTranslationTableState );
  tmpStates.push_back( TSequenceStates::copyAdditionalFilesState );
  return tmpStates;
}


bool TraceFilterAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;

  std::map< TTypeValuePair, TTypeValuePair > translationTable =
          ( (EventTranslationTableState *)tmpSequence->getState( TSequenceStates::eventTranslationTableState ) )->getData();

  bool onlyFilter = ( (OnlyFilterState *)tmpSequence->getState( TSequenceStates::onlyFilterState ) )->getData();
  std::string newName;

  if ( onlyFilter || !translationTable.empty() )
  {
    TraceOptions *options = ( (TraceOptionsState *)tmpSequence->getState( TSequenceStates::traceOptionsState ) )->getData();

    newName = ( (OutputTraceFileNameState *)tmpSequence->getState( TSequenceStates::outputTraceFileNameState ) )->getData();
    if ( !tmpSequence->isEndOfSequence() || newName.empty() )
    {
      std::string tmpSuffix = ( (OutputDirSuffixState *)tmpSequence->getState( TSequenceStates::outputDirSuffixState ) )->getData();
      std::string outputPath = whichTrace.substr( 0, whichTrace.find_last_of( mySequence->getKernelConnection()->getPathSeparator() ) ) +
                             mySequence->getKernelConnection()->getPathSeparator() + tmpSuffix;

      vector< std::string > tmpID;
      if( onlyFilter )
        tmpID.push_back( TraceFilter::getID() );
      else
        tmpID.push_back( EventTranslator::getID() );

      newName = mySequence->getKernelConnection()->getNewTraceName( whichTrace, outputPath, tmpID, false );
    }

    TraceFilter *myFilter = TraceFilter::create( mySequence->getKernelConnection(),
                                                 (char *)whichTrace.c_str(),
                                                 (char *)newName.c_str(),
                                                 options,
                                                 nullptr,
                                                 translationTable );

    bool copyFiles = ( (CopyAdditionalFilesState *)tmpSequence->getState( TSequenceStates::copyAdditionalFilesState ) )->getData();
    if ( copyFiles )
    {
      mySequence->getKernelConnection()->copyPCF( whichTrace, newName );
      mySequence->getKernelConnection()->copyROW( whichTrace, newName );
    }
  }
  else
  {
    newName = whichTrace;
  }

  tmpSequence->executeNextAction( newName );

  return true;
}


/****************************************************************************
 ********                  CSVOutputAction                           ********
 ****************************************************************************/
vector<TSequenceStates> CSVOutputAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  tmpStates.push_back(  TSequenceStates::sourceTimelineState );
  tmpStates.push_back(  TSequenceStates::csvFileNameState );
  return tmpStates;
}


bool CSVOutputAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;
  Timeline *tmpWindow = ( (SourceTimelineState *)tmpSequence->getState( TSequenceStates::sourceTimelineState ) )->getData();
  std::string tmpFileName = ( (CSVFileNameState *)tmpSequence->getState( TSequenceStates::csvFileNameState ) )->getData();
  TextOutput output = ( (CSVOutputState *)tmpSequence->getState( TSequenceStates::csvOutputState ) )->getData();

  output.dumpWindow( tmpWindow, tmpFileName );

  TraceOptions *options = ( (TraceOptionsState *)tmpSequence->getState( TSequenceStates::traceOptionsState ) )->getData();
  if( options != nullptr )
  {
    options->set_min_cutting_time( output.getMinTime() );
    options->set_max_cutting_time( output.getMaxTime() );
  }

  tmpSequence->executeNextAction( whichTrace );

  return true;
}

/****************************************************************************
 ********               TraceShifterTimesLoaderAction                ********
 ****************************************************************************/
/*vector<TSequenceStates> TraceShifterTimesLoaderAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;

  //tmpStates.push_back( TSequenceStates::shiftTimesState );

  return tmpStates;
}


bool TraceShifterTimesLoaderAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;
  MemoryTrace::iterator *it = nullptr;

  tmpSequence->executeNextAction( it );

  return true;
}
*/

/****************************************************************************
 ********                  TraceParserAction                         ********
 ****************************************************************************/
vector<TSequenceStates> TraceParserAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;

  //tmpStates.push_back( TSequenceStates::maxTraceTimeState );
  tmpStates.push_back( TSequenceStates::eofParsedState );

  return tmpStates;
}


bool TraceParserAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;

  bool noLoad = true;
  KTrace myTrace( whichTrace, nullptr, noLoad );
  tmpSequence->setCurrentTrace( &myTrace );

  bool executionError = false;
  MemoryTrace::iterator *it = myTrace.begin();
  while( !it->isNull() && !executionError )
  {
    executionError = tmpSequence->executeNextAction( it );
    ++(*it);
  }

  delete it;

  if ( !executionError )
  {
    // Final dummy record
    it = myTrace.empty();
    EOFParsedState *tmpEOFParseState = (EOFParsedState *)tmpSequence->getState( TSequenceStates::eofParsedState );
    tmpEOFParseState->setData( true );
    executionError = tmpSequence->executeNextAction( it );

    delete it;
  }

  return executionError;
}


/****************************************************************************
 ********                 RecordTimeShifterAction                    ********
 ****************************************************************************/
vector<TSequenceStates> RecordTimeShifterAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;

  //tmpStates.push_back( TSequenceStates::maxTraceTimeState );
  tmpStates.push_back( TSequenceStates::shiftTimesState );
  tmpStates.push_back( TSequenceStates::shiftLevelState );

  return tmpStates;
}


bool RecordTimeShifterAction::execute( MemoryTrace::iterator *whichRecord )
{
  //std::cout << ++count << std::endl;
  bool executionError = false;

  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;

  bool eofParsed = ( (EOFParsedState *)tmpSequence->getState( TSequenceStates::eofParsedState ) )->getData();

  if ( ( whichRecord->getType() == STATE + BEGIN ) ||
       ( whichRecord->getType() == EVENT ) ||
       ( whichRecord->getType() == COMM + LOG + SEND ) ||
       ( eofParsed ) )
  {
    TTime delta = (TTime)0;

    TApplOrder app;
    TTaskOrder task;
    TThreadOrder thread;

    if ( !eofParsed )
    {
      std::vector< TTime > *shiftTimes =
              ( (ShiftTimesState *)tmpSequence->getState( TSequenceStates::shiftTimesState ) )->getData();

      TWindowLevel shiftLevel =
              ( (ShiftLevelState *)tmpSequence->getState( TSequenceStates::shiftLevelState ) )->getData();

      tmpSequence->getCurrentTrace()->getThreadLocation( whichRecord->getThread(), app, task, thread );

      switch ( shiftLevel )
      {
        case WORKLOAD:
          if ( !checkedAvailableShiftTime )
          {
            checkedAvailableShiftTime = true;
            if ( shiftTimes->size() >= 1 )
              availableShiftTime = true;
          }

          if ( availableShiftTime )
            delta = (*shiftTimes)[ 0 ];

          break;

        case APPLICATION:
          if ( !checkedAvailableShiftTime )
          {
            checkedAvailableShiftTime = true;
            if ( shiftTimes->size() >= tmpSequence->getCurrentTrace()->totalApplications() )
              availableShiftTime = true;
          }

          if ( availableShiftTime )
            delta = (*shiftTimes)[ app ];

          break;

        case TASK:
          if ( !checkedAvailableShiftTime )
          {
            checkedAvailableShiftTime = true;
            if ( shiftTimes->size() >= tmpSequence->getCurrentTrace()->totalTasks() )
              availableShiftTime = true;
          }

          if ( availableShiftTime )
          {
            delta = (*shiftTimes)[ tmpSequence->getCurrentTrace()->getGlobalTask( app, task ) ];
          }

          break;

        case THREAD:
          if ( !checkedAvailableShiftTime )
          {
            checkedAvailableShiftTime = true;
            if ( shiftTimes->size() >= tmpSequence->getCurrentTrace()->totalThreads() )
              availableShiftTime = true;
          }

          if ( availableShiftTime )
          {
            delta = (*shiftTimes)[ tmpSequence->getCurrentTrace()->getGlobalThread( app, task, thread ) ];
            //delta = (*shiftTimes)[ whichRecord->getThread() ]; //??
          }

          break;

        default:
          break;
      }
    }

    executionError = !availableShiftTime;
    if ( availableShiftTime || eofParsed )
    {
      // Common for events time, states begin time, communications logical send time
      whichRecord->setTime( whichRecord->getTime() + delta );

      if ( whichRecord->getType() == STATE + BEGIN )
      {
        whichRecord->setStateEndTime( whichRecord->getStateEndTime() + delta );
      }
      else if ( whichRecord->getType() == COMM + LOG + SEND )
      {
        TCommID commID = whichRecord->getCommIndex();
        tmpSequence->getCurrentTrace()->setLogicalSend( commID,
                tmpSequence->getCurrentTrace()->getLogicalSend( commID ) + delta );
        tmpSequence->getCurrentTrace()->setLogicalReceive( commID,
                tmpSequence->getCurrentTrace()->getLogicalReceive( commID ) + delta );
        tmpSequence->getCurrentTrace()->setPhysicalSend( commID,
                tmpSequence->getCurrentTrace()->getPhysicalSend( commID ) + delta );
        tmpSequence->getCurrentTrace()->setPhysicalReceive( commID,
                tmpSequence->getCurrentTrace()->getPhysicalReceive( commID ) + delta );
      }

      executionError = tmpSequence->executeNextAction( whichRecord );
    }
  }

  return executionError;
}



/****************************************************************************
 ********                  TraceWriterAction                         ********
 ****************************************************************************/
vector<TSequenceStates> TraceWriterAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;

  //tmpStates.push_back( TSequenceStates::maxTraceTimeState );
  tmpStates.push_back( TSequenceStates::outputTraceFileNameState );

  return tmpStates;
}


bool TraceWriterAction::execute( MemoryTrace::iterator *it  )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;

  if( !outputTrace.is_open() )
  {
    std::string tmpFileName =
            ( (OutputTraceFileNameState *)tmpSequence->getState( TSequenceStates::outputTraceFileNameState ) )->getData();
    outputTrace.open( tmpFileName.c_str(), std::ios::out );
    TTime tmpMaxDelta =
            ( (MaxTraceTimeState *)tmpSequence->getState( TSequenceStates::maxTraceTimeState ) )->getData();

    tmpSequence->getCurrentTrace()->setEndTime( tmpSequence->getCurrentTrace()->getEndTime() + tmpMaxDelta );
    tmpSequence->getCurrentTrace()->dumpFileHeader( outputTrace );
  }

  bool eofParsed = ( (EOFParsedState *)tmpSequence->getState( TSequenceStates::eofParsedState ) )->getData();

  if ( ( it->getType() == STATE + BEGIN ) ||
       ( it->getType() == EVENT ) ||
       ( it->getType() == COMM + LOG + SEND ) ||
       ( eofParsed )
     )
  {
    body.write( outputTrace,
                tmpSequence->getCurrentTrace()->getProcessModel(),
                tmpSequence->getCurrentTrace()->getResourceModel(),
                it );
  }

  if ( eofParsed && outputTrace.is_open() )
    outputTrace.close();

  return tmpSequence->executeNextAction( it );
}


/****************************************************************************
 ********               EventDrivenCutterAction                      ********
 ****************************************************************************/
vector<TSequenceStates> EventDrivenCutterAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;

  tmpStates.push_back( TSequenceStates::outputTraceFileNameState );
  tmpStates.push_back( TSequenceStates::onEventCutterState );
  tmpStates.push_back( TSequenceStates::eofParsedState );

  return tmpStates;
}


bool EventDrivenCutterAction::execute( MemoryTrace::iterator *it  )
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
      ( it->getEventType() == ( (OnEventCutter *)tmpSequence->getState( TSequenceStates::onEventCutterState ) )->getData() ) )
  {
    TThreadOrder firstThread, lastThread;
    TApplOrder applLoc;
    TTaskOrder taskLoc;

    tmpSequence->getCurrentTrace()->getThreadLocation( it->getThread(), applLoc, taskLoc, firstThread );
    firstThread = tmpSequence->getCurrentTrace()->getFirstThread( applLoc, taskLoc );
    lastThread = tmpSequence->getCurrentTrace()->getLastThread( applLoc, taskLoc );

    if( countThreadsPerFile.count( currentThreadFile[ it->getThread() ] ) > 0 )
    {
      for( TThreadOrder iThread = firstThread; iThread <= lastThread; ++iThread )
        --countThreadsPerFile[ currentThreadFile[ iThread ] ];

      // TODO: close file
    }

    for( TThreadOrder iThread = firstThread; iThread <= lastThread; ++iThread )
      ++currentThreadFile[ iThread ];

    if( countThreadsPerFile.count( currentThreadFile[ it->getThread() ] ) > 0 )
    {
      for( TThreadOrder iThread = firstThread; iThread <= lastThread; ++iThread )
        ++countThreadsPerFile[ currentThreadFile[ iThread ] ];
    }
    else
    {
      std::fstream *tmpStream = new std::fstream;
      outputTraces.push_back( tmpStream );
      countThreadsPerFile[ currentThreadFile[ it->getThread() ] ] = 0;
      for( TThreadOrder iThread = firstThread; iThread <= lastThread; ++iThread )
        ++countThreadsPerFile[ currentThreadFile[ iThread ] ];
    }
  }

  if( it->getType() == EMPTYREC )
    currentFile = outputTraces.size() - 1;
  else
    currentFile = currentThreadFile[ it->getThread() ];

  if( !outputTraces[ currentFile ]->is_open() )
  {
    std::string tmpFileName = ( (OutputTraceFileNameState *)tmpSequence->getState( TSequenceStates::outputTraceFileNameState ) )->getData();
    if( currentFile > 0 )
    {
      std::string::size_type partPos = tmpFileName.rfind( "-part" );
      std::string::size_type nextDotPos = tmpFileName.find( ".", partPos );
      std::string currentSuffix = tmpFileName.substr( nextDotPos );
      tmpFileName = tmpFileName.substr( 0, partPos );

      std::stringstream sstrFileName( "" );
      sstrFileName << tmpFileName << "-part" << currentFile + 1 << currentSuffix;
      tmpFileName = sstrFileName.str();
    }

    tmpSequence->getKernelConnection()->copyPCF( tmpSequence->getCurrentTrace()->getFileName(), tmpFileName );
    tmpSequence->getKernelConnection()->copyROW( tmpSequence->getCurrentTrace()->getFileName(), tmpFileName );

    outputTraces[ currentFile ]->open( tmpFileName.c_str(), std::ios::out );
    tmpSequence->getCurrentTrace()->dumpFileHeader( *outputTraces[ currentFile ] );
  }

  bool eofParsed = ( (EOFParsedState *)tmpSequence->getState( TSequenceStates::eofParsedState ) )->getData();

  if ( ( it->getType() == STATE + BEGIN ) ||
       ( it->getType() == EVENT ) ||
       ( it->getType() == COMM + LOG + SEND ) ||
       ( eofParsed )
     )
  {
    body.write( *outputTraces[ currentFile ],
                tmpSequence->getCurrentTrace()->getProcessModel(),
                tmpSequence->getCurrentTrace()->getResourceModel(),
                it );
  }

  if ( eofParsed )
  {
    for( vector<std::fstream *>::iterator it = outputTraces.begin(); it != outputTraces.end(); ++it )
    {
      if( *it != nullptr && (*it)->is_open() )
      {
        (*it)->close();
        delete *it;
        *it = nullptr;
      }
    }
  }

  //tmpSequence->executeNextAction( it );

  return tmpSequence->executeNextAction( it );;
}


/****************************************************************************
 ********                  TraceSortAction                           ********
 ****************************************************************************/
vector<TSequenceStates> TraceSortAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;

  return tmpStates;
}


bool TraceSortAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;

  //tmpSequence->executeNextAction( whichTrace );

  return tmpSequence->executeNextAction( whichTrace );
}

/****************************************************************************
 ********                  PCFEventMergerAction                      ********
 ****************************************************************************/

vector<TSequenceStates> PCFEventMergerAction::getStateDependencies() const
{
  vector<TSequenceStates> tmpStates;
  tmpStates.push_back( TSequenceStates::eventTranslationTableState );
  tmpStates.push_back( TSequenceStates::outputTraceFileNameState );
  tmpStates.push_back( TSequenceStates::outputDirSuffixState );
  tmpStates.push_back( TSequenceStates::pcfMergerReferenceState );

  return tmpStates;
}


bool PCFEventMergerAction::execute( std::string whichTrace )
{
  TraceEditSequence *tmpSequence = mySequence;
  bool translationOk = false;

  // Get new tracename
  std::string newName = ( (OutputTraceFileNameState *)tmpSequence->getState( TSequenceStates::outputTraceFileNameState ) )->getData();
  if ( newName.empty() )
  {
    std::string tmpSuffix = ( (OutputDirSuffixState *)tmpSequence->getState( TSequenceStates::outputDirSuffixState ) )->getData();
    std::string outputPath = whichTrace.substr( 0, whichTrace.find_last_of( mySequence->getKernelConnection()->getPathSeparator() ) ) +
                           mySequence->getKernelConnection()->getPathSeparator() + tmpSuffix;

    vector< std::string > tmpID;
    tmpID.push_back( EventTranslator::getID() );

    newName = mySequence->getKernelConnection()->getNewTraceName( whichTrace, outputPath, tmpID, false );
  }

  std::string referenceTrace = ( (PCFMergerReferenceState *)tmpSequence->getState( TSequenceStates::pcfMergerReferenceState ) )->getData();
  std::string referencePCFFile = LocalKernel::composeName( referenceTrace, "pcf" );
  std::string sourceTrace = LocalKernel::composeName( whichTrace, "pcf" );

  // Do pcf file exist? If not, exit sequence
  bool verbose = true;
  bool keepOpen = false;
  bool exitProgram = false;
  if ( !mySequence->getKernelConnection()->isFileReadable( referencePCFFile,
                                                           "PCFEventMergerAction::execute",
                                                           verbose, keepOpen, exitProgram ) ||
       !mySequence->getKernelConnection()->isFileReadable( sourceTrace,
                                                           "PCFEventMergerAction::execute",
                                                           verbose, keepOpen, exitProgram ) )
    return false;

  PCFFileParser<> referenceTraceConfig( referencePCFFile );
  PCFFileParser<> sourceTraceConfig( sourceTrace );

  // Translation algorithm
  map< TTypeValuePair, TTypeValuePair > translation;

  vector< TEventType > sourceTypes;
  sourceTraceConfig.getEventTypes( sourceTypes );
  vector< TEventType > tmpCodes;
  referenceTraceConfig.getEventTypes( tmpCodes );
  set< unsigned int > referenceTypes;
  referenceTypes.insert( tmpCodes.begin(), tmpCodes.end() );

  for ( auto itSourceType : sourceTypes )
  {
    if ( referenceTypes.find( itSourceType ) != referenceTypes.end() )
    {
      auto sourceValues = sourceTraceConfig.getEventValues( itSourceType );
      if ( sourceValues.empty() )
        continue;

      map< std::string, TEventValue > referenceValues;
      auto tmpReferenceValues = referenceTraceConfig.getEventValues( itSourceType );
      if ( tmpReferenceValues.empty() )
        continue;

      for ( auto itTmpReferenceValue : tmpReferenceValues )
        referenceValues[ itTmpReferenceValue.second ] = itTmpReferenceValue.first;

      map< TEventValue, std::string > valuesColliding;
      map< TEventValue, std::string > valuesFinal;

      for ( auto itSourceValue : sourceValues )
      {
        auto itRefValue = referenceValues.find( itSourceValue.second );
        if ( itRefValue != referenceValues.end() )
        {
          if ( valuesFinal.find( (*itRefValue).second ) != valuesFinal.end() )
          {
            valuesColliding[ (*itRefValue).second ] = valuesFinal[ (*itRefValue).second ];
          }
          else
          {
            if ( itSourceValue.first != (*itRefValue).second )
              translation[ TTypeValuePair( itSourceType, itSourceValue.first ) ] = TTypeValuePair( itSourceType, (*itRefValue).second );
          }

          valuesFinal[ (*itRefValue).second ] = itSourceValue.second;
        }
        else
        {
          if ( valuesFinal.find( itSourceValue.first ) != valuesFinal.end() )
            valuesColliding[ itSourceValue.first ] = itSourceValue.second;
          else
            valuesFinal[ itSourceValue.first ] = itSourceValue.second;
        }
      }

      TEventValue maxValue = (--valuesFinal.end())->first;
      for ( auto itCollision : valuesColliding )
      {
        valuesFinal[ ++maxValue ] = itCollision.second;
        if ( itCollision.first != maxValue )
          translation[ TTypeValuePair( itSourceType, itCollision.first ) ] = TTypeValuePair( itSourceType, maxValue );
      }

      sourceTraceConfig.setEventValues( itSourceType, valuesFinal );
    }
  }

  // Write files
  ( (EventTranslationTableState *)tmpSequence->getState( TSequenceStates::eventTranslationTableState ) )->setData( translation );

  if ( !translation.empty() )
  {
    mySequence->getKernelConnection()->copyROW( whichTrace, newName );
    sourceTraceConfig.dumpToFile( LocalKernel::composeName( newName, std::string( "pcf" ) ) );
  }

  tmpSequence->executeNextAction( whichTrace );

  return true;
}