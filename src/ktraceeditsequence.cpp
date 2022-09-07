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


#include <stdexcept>
#include "ktraceeditsequence.h"
#include "traceeditstates.h"
#include "ktraceeditactions.h"

using std::invalid_argument;

class TextOutput;

KTraceEditSequence::KTraceEditSequence( const KernelConnection *whichKernel ) : myKernel( whichKernel )
{
}


KTraceEditSequence::~KTraceEditSequence()
{
  for( map<TSequenceStates, TraceEditState *>::iterator it = activeStates.begin();
       it != activeStates.end(); ++it )
    delete it->second;

  for( vector<TraceEditAction *>::iterator it = sequenceActions.begin();
       it != sequenceActions.end(); ++it )
    delete *it;
}


const KernelConnection *KTraceEditSequence::getKernelConnection() const
{
  return myKernel;
}


TraceEditState *KTraceEditSequence::createState( TSequenceStates whichState )
{
  switch( whichState )
  {
    case TSequenceStates::testState:
      return new TestState( this );
      break;

    case TSequenceStates::traceOptionsState:
      return new TraceOptionsState( this );
      break;

    case TSequenceStates::sourceTimelineState:
      return new SourceTimelineState( this );
      break;

    case TSequenceStates::csvFileNameState:
      return new CSVFileNameState( this );
      break;

    case TSequenceStates::csvOutputState:
      return new CSVOutputState( this );
      break;

    case TSequenceStates::outputDirSuffixState:
      return new OutputDirSuffixState( this );
      break;

    case TSequenceStates::outputTraceFileNameState:
      return new OutputTraceFileNameState( this );
      break;

    case TSequenceStates::maxTraceTimeState:
      return new MaxTraceTimeState( this );
      break;

    case TSequenceStates::shiftTimesState:
      return new ShiftTimesState( this );
      break;

    case TSequenceStates::eofParsedState:
      return new EOFParsedState( this );
      break;

    case TSequenceStates::shiftLevelState:
      return new ShiftLevelState( this );
      break;

    case TSequenceStates::onEventCutterState:
      return new OnEventCutter( this );
      break;

    case TSequenceStates::pcfMergerReferenceState:
      return new PCFMergerReferenceState( this );
      break;

    case TSequenceStates::copyAdditionalFilesState:
      return new CopyAdditionalFilesState( this );
      break;

    case TSequenceStates::eventTranslationTableState:
      return new EventTranslationTableState( this );
      break;

    case TSequenceStates::onlyFilterState:
      return new OnlyFilterState( this );
      break;

    default:
      return nullptr;
      break;
  }

  return nullptr;
}


void KTraceEditSequence::setCurrentTrace( KTrace *whichTrace )
{
  currentTrace = whichTrace;
}


KTrace *KTraceEditSequence::getCurrentTrace()
{
  return currentTrace;
}


bool KTraceEditSequence::addState( TSequenceStates whichState )
{
  map<TSequenceStates, TraceEditState *>::iterator tmpIt;
  tmpIt = activeStates.find( whichState );
  if( tmpIt != activeStates.end() )
    return false;

  TraceEditState *newState = createState( whichState );
  if( newState == nullptr )
    throw invalid_argument( "Invalid state for TraceEditSequence" );

  activeStates[ whichState ] = newState;
  return true;
}


bool KTraceEditSequence::addState( TSequenceStates whichState, TraceEditState *newState )
{
  map<TSequenceStates, TraceEditState *>::iterator tmpIt;

  tmpIt = activeStates.find( whichState );
  if( tmpIt != activeStates.end() )
    return false;

  activeStates[ whichState ] = newState;
  return true;
}


TraceEditState *KTraceEditSequence::getState( TSequenceStates whichState )
{
  map<TSequenceStates, TraceEditState *>::iterator tmpIt;

  tmpIt = activeStates.find( whichState );
  if( tmpIt != activeStates.end() )
    return tmpIt->second;

  return nullptr;
}

bool KTraceEditSequence::pushbackAction( TSequenceActions whichAction )
{
  TraceEditAction *newAction;

  switch( whichAction )
  {
    case TSequenceActions::testAction:
      newAction = new TestAction( this );
      break;

    case TSequenceActions::traceCutterAction:
      newAction = new TraceCutterAction( this );
      break;

    case TSequenceActions::traceFilterAction:
      newAction = new TraceFilterAction( this );
      break;

    case TSequenceActions::csvOutputAction:
      newAction = new CSVOutputAction( this );
      break;

    case TSequenceActions::traceParserAction:
      newAction = new TraceParserAction( this );
      break;

    case TSequenceActions::recordTimeShifterAction:
      newAction = new RecordTimeShifterAction( this );
      break;

    case TSequenceActions::traceWriterAction:
      newAction = new TraceWriterAction( this );
      break;

    case TSequenceActions::eventDrivenCutterAction:
      newAction = new EventDrivenCutterAction( this );
      break;

    case TSequenceActions::traceSortAction:
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
  TTraceEditActionType tmpType = newAction->getType();

  if( sequenceActions.empty() )
  {
    if( tmpType == TTraceEditActionType::TraceToTrace || tmpType == TTraceEditActionType::TraceToRecord )
    {
      sequenceActions.push_back( newAction );
      return true;
    }
    else
      return false;
  }

  switch( sequenceActions[ sequenceActions.size() - 1 ]->getType() )
  {
    case TTraceEditActionType::TraceToTrace:
    case TTraceEditActionType::RecordToTrace:
      if( tmpType != TTraceEditActionType::TraceToTrace && tmpType != TTraceEditActionType::TraceToRecord )
        return false;
      break;

    case TTraceEditActionType::TraceToRecord:
    case TTraceEditActionType::RecordToRecord:
      if( tmpType != TTraceEditActionType::RecordToTrace && tmpType != TTraceEditActionType::RecordToRecord )
        return false;
      break;

    default:
      return false;
      break;
  }

  sequenceActions.push_back( newAction );
  return true;
}


bool KTraceEditSequence::execute( vector<std::string> traces )
{
  for( vector<TraceEditAction *>::iterator it = sequenceActions.begin();
       it != sequenceActions.end(); ++it )
  {
    vector<TSequenceStates> tmpStates = (*it)->getStateDependencies();
    for( vector<TSequenceStates>::iterator itState = tmpStates.begin();
         itState != tmpStates.end(); ++itState )
      addState( *itState );
  }

  TraceToTraceAction *firstActionTraceToTrace = ( TraceToTraceAction * )sequenceActions[ 0 ];
  TraceToRecordAction *firstActionTraceToRecord = ( TraceToRecordAction * )sequenceActions[ 0 ];

  bool someError = false;

  for( vector<std::string>::iterator it = traces.begin(); it != traces.end(); ++it )
  {
    currentAction = 0;
    currentTraceName = *it;

    for( map<TSequenceStates, TraceEditState *>::iterator itState = activeStates.begin();
         itState != activeStates.end(); ++itState )
      itState->second->init();

    switch( sequenceActions[ 0 ]->getType() )
    {
      case TTraceEditActionType::TraceToTrace:
        sequenceExecError[ *it ] = firstActionTraceToTrace->execute( *it );
        break;

      case TTraceEditActionType::TraceToRecord:
        sequenceExecError[ *it ] = firstActionTraceToRecord->execute( *it );
        break;

      default:
        sequenceExecError[ *it ] = false;
        break;
    }

    someError = someError || sequenceExecError[ *it ];
  }

// TODO : think, some error in some trace?
  return someError;
}

bool KTraceEditSequence::executeNextAction( std::string whichTrace )
{
  if( sequenceExecError[ whichTrace ] )
    return true; // sequenceExecError[ whichTrace ]

  ++currentAction;
  if( currentAction == sequenceActions.size() )
  {
    --currentAction;
    return false;
  }

  TraceToTraceAction *nextActionToTrace = ( TraceToTraceAction * )sequenceActions[ currentAction ];
  TraceToRecordAction *nextActionToRecord = ( TraceToRecordAction * )sequenceActions[ currentAction ];

  switch( sequenceActions[ currentAction ]->getType() )
  {
    case TTraceEditActionType::TraceToTrace:
      sequenceExecError[ whichTrace ] = nextActionToTrace->execute( whichTrace );
      break;

    case TTraceEditActionType::TraceToRecord:
      sequenceExecError[ whichTrace ] = nextActionToRecord->execute( whichTrace );
      break;

    case TTraceEditActionType::RecordToTrace:

      break;

    case TTraceEditActionType::RecordToRecord:

      break;

    default:
      break;
  }

  --currentAction;

  return sequenceExecError[ whichTrace ];
}


bool KTraceEditSequence::executeNextAction( MemoryTrace::iterator *whichRecord )
{
  if( sequenceExecError[ currentTraceName ] )
    return true; // sequenceExecError[ whichTrace ]

  ++currentAction;
  if( currentAction == sequenceActions.size() )
  {
    --currentAction;
    return false;
  }

  RecordToTraceAction *nextActionToTrace = ( RecordToTraceAction * )sequenceActions[ currentAction ];
  RecordToRecordAction *nextActionToRecord = ( RecordToRecordAction * )sequenceActions[ currentAction ];

  switch( sequenceActions[ currentAction ]->getType() )
  {
    case TTraceEditActionType::TraceToTrace:
      break;

    case TTraceEditActionType::TraceToRecord:
      break;

    case TTraceEditActionType::RecordToTrace:
      sequenceExecError[ currentTraceName ] = nextActionToTrace->execute( whichRecord );
      break;

    case TTraceEditActionType::RecordToRecord:
      sequenceExecError[ currentTraceName ] = nextActionToRecord->execute( whichRecord );
      break;

    default:
      break;
  }

  --currentAction;

  return sequenceExecError[ currentTraceName ];
}

bool KTraceEditSequence::isEndOfSequence() const
{
  return ( currentAction == sequenceActions.size() - 1 );
}
