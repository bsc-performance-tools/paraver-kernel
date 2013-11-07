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

/****************************************************************************
 ********                  TestAction                                ********
 ****************************************************************************/

void TestAction::execute( std::string whichTrace )
{
  std::cout << "testAction::execute with parameter: " << whichTrace << std::endl;

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
  tmpStates.push_back( TraceEditSequence::CSVWindowState );
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
  if( options->get_min_cutting_time() == 0 && options->get_max_cutting_time() >= tmpWindow->getTrace()->getEndTime() )
  {
    symlink( whichTrace.c_str(), newName.c_str() );
  }
  else
  {
#endif
    TraceCutter *myCutter = TraceCutter::create( mySequence->getKernelConnection(),
                                                (char *)whichTrace.c_str(),
                                                (char *)newName.c_str(),
                                                options,
                                                NULL );
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
    options->set_min_cutting_time( output.getMinTime() );

  tmpSequence->executeNextAction( whichTrace );
}

