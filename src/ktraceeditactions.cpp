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

#include "ktraceeditactions.h"
#include "ktraceeditsequence.h"
#include <iostream>
#include "traceeditstates.h"
#include "tracecutter.h"
#include "kernelconnection.h"

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
  return tmpStates;
}


void TraceCutterAction::execute( std::string whichTrace )
{
  KTraceEditSequence *tmpSequence = (KTraceEditSequence *)mySequence;
  TraceOptions *options = ( (TraceOptionsState *)tmpSequence->getState( TraceEditSequence::traceOptionsState ) )->getData();
  std::string newName = mySequence->getKernelConnection()->getNewTraceName( whichTrace, TraceCutter::getID(), false );

  TraceCutter *myCutter = TraceCutter::create( mySequence->getKernelConnection(),
                                              (char *)whichTrace.c_str(),
                                              (char *)newName.c_str(),
                                              options,
                                              NULL );

  mySequence->getKernelConnection()->copyPCF( whichTrace, newName );
  mySequence->getKernelConnection()->copyROW( whichTrace, newName );

  tmpSequence->executeNextAction( newName );
}
