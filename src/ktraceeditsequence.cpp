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

#include <stdexcept>
#include "ktraceeditsequence.h"
#include "traceeditstates.h"
#include "ktraceeditactions.h"

using std::invalid_argument;


KTraceEditSequence::KTraceEditSequence( KernelConnection *whichKernel ) :
  TraceEditSequence( whichKernel )
{
}


KTraceEditSequence::~KTraceEditSequence()
{
  for( map<TraceEditSequence::TSequenceStates, TraceEditState *>::iterator it = activeStates.begin();
       it != activeStates.end(); ++it )
    delete it->second;

  for( vector<TraceEditAction *>::iterator it = sequenceActions.begin();
       it != sequenceActions.end(); ++it )
    delete *it;
}


TraceEditState *KTraceEditSequence::createState( TraceEditSequence::TSequenceStates whichState )
{
  switch( whichState )
  {
    case testState:
      return new TestState( this );
      break;

    case traceOptionsState:
      return new TraceOptionsState( this );
      break;

    case csvWindowState:
      return new CSVWindowState( this );
      break;

    case csvFileNameState:
      return new CSVFileNameState( this );
      break;

    case csvOutputState:
      return new CSVOutputState( this );
      break;

    case outputDirSuffixState:
      return new OutputDirSuffixState( this );
      break;

    case outputTraceFileNameState:
      return new OutputTraceFileNameState( this );
      break;

    default:
      return NULL;
      break;
  }

  return NULL;
}


bool KTraceEditSequence::addState( TraceEditSequence::TSequenceStates whichState )
{
  map<TraceEditSequence::TSequenceStates, TraceEditState *>::iterator tmpIt;
  tmpIt = activeStates.find( whichState );
  if( tmpIt != activeStates.end() )
    return false;

  TraceEditState *newState = createState( whichState );
  if( newState == NULL )
    throw invalid_argument( "Invalid state for TraceEditSequence" );

  activeStates[ whichState ] = newState;
  return true;
}


bool KTraceEditSequence::addState( TraceEditSequence::TSequenceStates whichState, TraceEditState *newState )
{
  map<TraceEditSequence::TSequenceStates, TraceEditState *>::iterator tmpIt;

  tmpIt = activeStates.find( whichState );
  if( tmpIt != activeStates.end() )
    return false;

  activeStates[ whichState ] = newState;
  return true;
}


TraceEditState *KTraceEditSequence::getState( TraceEditSequence::TSequenceStates whichState )
{
  map<TraceEditSequence::TSequenceStates, TraceEditState *>::iterator tmpIt;

  tmpIt = activeStates.find( whichState );
  if( tmpIt != activeStates.end() )
    return tmpIt->second;

  return NULL;
}

bool KTraceEditSequence::pushbackAction( TraceEditSequence::TSequenceActions whichAction )
{
  TraceEditAction *newAction;

  switch( whichAction )
  {
    case testAction:
      newAction = new TestAction( this );
      break;

    case traceCutterAction:
      newAction = new TraceCutterAction( this );
      break;

    case csvOutputAction:
      newAction = new CSVOutputAction( this );
      break;

    case traceParserAction:
      newAction = new TraceParserAction( this );
      break;

    case recordTimeShifterAction:
      newAction = new RecordTimeShifterAction( this );
      break;

    case traceWriterAction:
      newAction = new TraceWriterAction( this );
      break;

    case traceSortAction:
      newAction = new TraceSortAction( this );
      break;

    default:
      return false;
      break;
  }

  if( !pushbackAction( newAction ) )
  {
    delete newAction;
    return false;
  }

  return true;
}


bool KTraceEditSequence::pushbackAction( TraceEditAction *newAction )
{
  TraceEditAction::TTraceEditActionType tmpType = newAction->getType();

  if( sequenceActions.empty() )
  {
    if( tmpType == TraceEditAction::TraceToTrace || tmpType == TraceEditAction::TraceToRecord )
    {
      sequenceActions.push_back( newAction );
      return true;
    }
    else
      return false;
  }

  switch( sequenceActions[ sequenceActions.size() - 1 ]->getType() )
  {
    case TraceEditAction::TraceToTrace:
    case TraceEditAction::RecordToTrace:
      if( tmpType != TraceEditAction::TraceToTrace && tmpType != TraceEditAction::TraceToRecord )
        return false;
      break;

    case TraceEditAction::TraceToRecord:
    case TraceEditAction::RecordToRecord:
      if( tmpType != TraceEditAction::RecordToTrace && tmpType != TraceEditAction::RecordToRecord )
        return false;
      break;

    default:
      return false;
      break;
  }

  sequenceActions.push_back( newAction );
  return true;
}


void KTraceEditSequence::execute( vector<std::string> traces )
{
  for( vector<TraceEditAction *>::iterator it = sequenceActions.begin();
       it != sequenceActions.end(); ++it )
  {
    vector<TraceEditSequence::TSequenceStates> tmpStates = (*it)->getStateDependencies();
    for( vector<TraceEditSequence::TSequenceStates>::iterator itState = tmpStates.begin();
         itState != tmpStates.end(); ++itState )
      addState( *itState );
  }

  TraceToTraceAction *firstActionTraceToTrace = ( TraceToTraceAction * )sequenceActions[ 0 ];
  TraceToRecordAction *firstActionTraceToRecord = ( TraceToRecordAction * )sequenceActions[ 0 ];

  for( vector<std::string>::iterator it = traces.begin(); it != traces.end(); ++it )
  {
    currentAction = 0;

    for( map<TraceEditSequence::TSequenceStates, TraceEditState *>::iterator itState = activeStates.begin();
         itState != activeStates.end(); ++itState )
      itState->second->init();

    switch( sequenceActions[ 0 ]->getType() )
    {
      case TraceEditAction::TraceToTrace:
        firstActionTraceToTrace->execute( *it );
        break;

      case TraceEditAction::TraceToRecord:
        firstActionTraceToRecord->execute( *it );
        break;

      default:
        break;
    }
  }
}

void KTraceEditSequence::executeNextAction( std::string whichTrace )
{
  ++currentAction;
  if( currentAction == sequenceActions.size() )
    return;

  TraceToTraceAction *nextActionToTrace = ( TraceToTraceAction * )sequenceActions[ currentAction ];
  TraceToRecordAction *nextActionToRecord = ( TraceToRecordAction * )sequenceActions[ currentAction ];

  switch( sequenceActions[ currentAction ]->getType() )
  {
    case TraceEditAction::TraceToTrace:
      nextActionToTrace->execute( whichTrace );
      break;

    case TraceEditAction::TraceToRecord:
      nextActionToRecord->execute( whichTrace );
      break;

    case TraceEditAction::RecordToTrace:

      break;

    case TraceEditAction::RecordToRecord:

      break;

    default:
      break;
  }

  --currentAction;
}


void KTraceEditSequence::executeNextAction( MemoryTrace::iterator *whichRecord )
{
  ++currentAction;
  if( currentAction == sequenceActions.size() )
    return;

  RecordToTraceAction *nextActionToTrace = ( RecordToTraceAction * )sequenceActions[ currentAction ];
  RecordToRecordAction *nextActionToRecord = ( RecordToRecordAction * )sequenceActions[ currentAction ];

  switch( sequenceActions[ currentAction ]->getType() )
  {
    case TraceEditAction::TraceToTrace:
      break;

    case TraceEditAction::TraceToRecord:
      break;

    case TraceEditAction::RecordToTrace:
      nextActionToTrace->execute( whichRecord );
      break;

    case TraceEditAction::RecordToRecord:
      nextActionToRecord->execute( whichRecord );
      break;

    default:
      break;
  }

  --currentAction;
}

