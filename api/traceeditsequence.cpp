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

#include <stdexcept>
#include "traceeditsequence.h"
#include "traceeditactions.h"
#include "traceeditstates.h"

using std::invalid_argument;

TraceEditSequence::TraceEditSequence()
{

}


TraceEditSequence::~TraceEditSequence()
{
  for( map<TraceEditSequence::TSequenceStates, TraceEditState *>::iterator it = activeStates.begin();
       it != activeStates.end(); ++it )
    delete it->second;

  for( vector<TraceEditAction *>::iterator it = sequenceActions.begin();
       it != sequenceActions.end(); ++it )
    delete *it;
}


TraceEditState *TraceEditSequence::createState( TraceEditSequence::TSequenceStates whichState )
{
  switch( whichState )
  {
    case testState:
      return new TestState( this );
      break;

    default:
      return NULL;
      break;
  }

  return NULL;
}


bool TraceEditSequence::addState( TraceEditSequence::TSequenceStates whichState )
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


bool TraceEditSequence::addState( TraceEditSequence::TSequenceStates whichState, TraceEditState *newState )
{
  map<TraceEditSequence::TSequenceStates, TraceEditState *>::iterator tmpIt;

  tmpIt = activeStates.find( whichState );
  if( tmpIt != activeStates.end() )
    return false;

  activeStates[ whichState ] = newState;
  return true;
}


TraceEditState *TraceEditSequence::getState( TraceEditSequence::TSequenceStates whichState )
{
  map<TraceEditSequence::TSequenceStates, TraceEditState *>::iterator tmpIt;

  tmpIt = activeStates.find( whichState );
  if( tmpIt != activeStates.end() )
    return tmpIt->second;

  return NULL;
}


bool TraceEditSequence::pushbackAction( TraceEditAction *newAction )
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


void TraceEditSequence::execute( vector<std::string> traces )
{
  for( vector<TraceEditAction *>::iterator it = sequenceActions.begin();
       it != sequenceActions.end(); ++it )
  {
    vector<TraceEditSequence::TSequenceStates> tmpStates = (*it)->getStateDependencies();
    for( vector<TraceEditSequence::TSequenceStates>::iterator itState = tmpStates.begin();
         itState != tmpStates.end(); ++itState )
      addState( *itState );
  }

  for( map<TraceEditSequence::TSequenceStates, TraceEditState *>::iterator it = activeStates.begin();
       it != activeStates.end(); ++it )
    it->second->init();

  currentAction = 0;

  TraceToTraceAction *firstAction = ( TraceToTraceAction * )sequenceActions[ 0 ];
  for( vector<std::string>::iterator it = traces.begin(); it != traces.end(); ++it )
    firstAction->execute( *it );
}


void TraceEditSequence::executeNextAction( std::string whichTrace )
{

}


void TraceEditSequence::executeNextAction( MemoryTrace::iterator * )
{

}

