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

#include "ktraceshifter.h"

#include <sstream>
#include <fstream>

//using namespace std;
//#include <iostream>

KTraceShifter::KTraceShifter( const KernelConnection *myKernel,
                              std::string traceIn,
                              std::string traceOut,
                              std::string whichShiftTimes,
                              TWindowLevel shiftLevel,
                              ProgressController *progress )
{
  maxShiftTime = -std::numeric_limits<TTime>::max();
  shiftTimes = readShiftTimes( whichShiftTimes );

  // Check level
  if ( shiftLevel < WORKLOAD || shiftLevel > THREAD )
    shiftLevel = THREAD;

  // Build sequence
  mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TraceEditSequence::traceParserAction );
  mySequence->pushbackAction( TraceEditSequence::recordTimeShifterAction );
  mySequence->pushbackAction( TraceEditSequence::traceWriterAction );
  //mySequence->pushbackAction( TraceEditSequence::traceSortAction ); //?

  // State: max trace time
  //   Initialized by TraceParserAction
  //   Modified    by RecordTimeShifterAction
  //   Read        by TraceWriterAction
  MaxTraceTimeState *tmpMaxTraceTimeState = new MaxTraceTimeState( mySequence );
  tmpMaxTraceTimeState->setData( maxShiftTime );
  mySequence->addState( TraceEditSequence::maxTraceTimeState, tmpMaxTraceTimeState );

  // State: shift times
  ShiftTimesState *tmpShiftTimesState = new ShiftTimesState( mySequence );
  tmpShiftTimesState->setData( &shiftTimes );
  mySequence->addState( TraceEditSequence::shiftTimesState, tmpShiftTimesState );

  ShiftLevelState *tmpShiftLevelState = new ShiftLevelState( mySequence );
  tmpShiftLevelState->setData( shiftLevel );
  mySequence->addState( TraceEditSequence::shiftLevelState, tmpShiftLevelState );

  OutputTraceFileNameState *tmpOutputTraceFileNameState = new OutputTraceFileNameState( mySequence );
  tmpOutputTraceFileNameState->setData( traceOut );
  mySequence->addState( TraceEditSequence::outputTraceFileNameState, tmpOutputTraceFileNameState );

  EOFParsedState *tmpEOFParseState = new EOFParsedState( mySequence );
  tmpEOFParseState->setData( false );
  mySequence->addState( TraceEditSequence::eofParsedState, tmpEOFParseState );

  traces.push_back( traceIn );
}


KTraceShifter::~KTraceShifter()
{
  delete mySequence;
}


void KTraceShifter::execute( std::string traceIn,
                             std::string traceOut,
                             ProgressController *progress )
{
  mySequence->execute( traces );
  mySequence->getKernelConnection()->copyPCF( traceIn, traceOut );
  mySequence->getKernelConnection()->copyROW( traceIn, traceOut );
}


std::vector< TTime > KTraceShifter::readShiftTimes( std::string shiftTimesFile )
{
  std::vector< TTime > shiftTimes;

  std::ifstream timesFile( shiftTimesFile.c_str() );
  if ( timesFile != NULL )
  {
    std::string currentLine;
    TTime currentTime;

    while ( !timesFile.eof() )
    {
      getline( timesFile, currentLine );

      if ( ( currentLine.length() > 0 ) && ( currentLine[ 0 ] != '#' ) )
      {
        std::stringstream auxVal( currentLine );
        if ( !( auxVal >> currentTime ) )
        {
          // TODO: Be gentle and warn about wrong time/conversion
          continue;

        }
        else
        {
          shiftTimes.push_back( TTime( currentTime ) );

          if ( currentTime > maxShiftTime )
            maxShiftTime = currentTime;
        }
      }
    }

    timesFile.close();
  }

  return shiftTimes;
}


