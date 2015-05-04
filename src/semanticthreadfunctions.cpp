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

#include "semanticthreadfunctions.h"
#include "kwindow.h"

using namespace std;

bool stateOnSameTime( MemoryTrace::iterator *it, KSingleWindow *window )
{
  bool finish = false;
  TRecordTime time = it->getTime();
  MemoryTrace::iterator *nextState = NULL;

  if ( window->getLevel() >= WORKLOAD && window->getLevel() <= THREAD )
    nextState = window->copyThreadIterator( it );
  else
    nextState = window->copyCPUIterator( it );

  ++( *nextState );
  while ( !finish && !nextState->isNull() && nextState->getTime() == time )
  {
    if ( nextState->getType() & STATE && nextState->getType() & BEGIN )
      finish = true;

    ++( *nextState );
  }

  delete nextState;

  return finish;
}

TRecordTime timeToNextState( MemoryTrace::iterator *it, KSingleWindow *window )
{
  bool finish = false;
  TRecordTime time = it->getTime();
  MemoryTrace::iterator *nextState = NULL;

  if ( window->getLevel() >= WORKLOAD && window->getLevel() <= THREAD )
    nextState = window->copyThreadIterator( it );
  else
    nextState = window->copyCPUIterator( it );

  ++( *nextState );
  while ( !finish && !nextState->isNull() )
  {
    if ( nextState->getType() & STATE && nextState->getType() & BEGIN )
    {
      time = nextState->getTime() - time;
      finish = true;
    }

    ++( *nextState );
  }

  delete nextState;

  return time;
}

void getNextEvent( MemoryTrace::iterator *it, KSingleWindow *window )
{
  bool finish = false;

  while ( !finish )
  {
    ++( *it );
    if ( it->isNull() )
      finish = true;
    else if ( it->getType() & EVENT )
    {
      if ( window->passFilter( it ) )
        finish = true;
    }
  }
}


TSemanticValue getTotalCommSize( MemoryTrace::iterator *itBegin,
                                 MemoryTrace::iterator *itEnd,
                                 KSingleWindow *window )
{
  TSemanticValue bytes = 0;
  MemoryTrace::iterator *nextComm;

  if ( window->getLevel() >= SYSTEM )
    nextComm = window->copyCPUIterator( itBegin );
  else
    nextComm = window->copyThreadIterator( itBegin );

  /* First we watched if there are previous records of communications
     in the same time of the "record", but only sends */
  --( *nextComm );
  while ( ( !nextComm->isNull() ) && ( nextComm->getTime() == itBegin->getTime() ) )
  {
    if ( window->getFilter()->getPhysical() && !window->getFilter()->getLogical() &&
         ( nextComm->getType() & PHY &&
           nextComm->getType() & COMM &&
           nextComm->getType() & SEND ) )
    {
      if ( window->passFilter( nextComm ) )
        bytes += window->getTrace()->getCommSize( nextComm->getCommIndex() );
    }
    else if ( window->getFilter()->getLogical() &&
              ( nextComm->getType() & LOG &&
                nextComm->getType() & COMM &&
                nextComm->getType() & SEND ) )
    {
      if ( window->passFilter( nextComm ) )
        bytes += window->getTrace()->getCommSize( nextComm->getCommIndex() );
    }
    --( *nextComm );
  }

  ( *nextComm ) = ( *itBegin );
  ++( *nextComm );

  /* Then go forward, until next event */
  while ( !nextComm->isNull() && nextComm->getTime() <= itEnd->getTime() )
  {
    if ( nextComm->getTime() == itBegin->getTime() )
    {
      if ( ( nextComm->getType() & COMM &&
             nextComm->getType() & SEND ) &&
           window->passFilter( nextComm ) )
        bytes += window->getTrace()->getCommSize( nextComm->getCommIndex() );
    }
    else if ( window->getFilter()->getPhysical() && !window->getFilter()->getLogical() &&
              ( nextComm->getType() & PHY &&
                nextComm->getType() & COMM ) )
    {
      if ( nextComm->getTime() < itEnd->getTime() ||
           ( nextComm->getTime() == itEnd->getTime() && ( nextComm->getType() & RECV ) ) )
      {
        if ( window->passFilter( nextComm ) )
          bytes += window->getTrace()->getCommSize( nextComm->getCommIndex() );
      }
    }
    else if ( ( ( nextComm->getType() & COMM ) &&
                ( window->getFilter()->getLogical() && ( nextComm->getType() & LOG ) ) ) ||
              ( nextComm->getType() & PHY &&
                nextComm->getType() & RECV ) )
    {
      if ( !( window->getFilter()->getLogical() && window->getFilter()->getPhysical() ) ||
           !( ( nextComm->getType() & LOG &&
                nextComm->getType() & RECV ) &&
              window->getTrace()->getPhysicalReceive( nextComm->getCommIndex() )
              >= window->getTrace()->getLogicalReceive( nextComm->getCommIndex() ) ) )
      {
        if ( nextComm->getTime() < itEnd->getTime() )
        {
          if ( window->passFilter( nextComm ) )
            bytes += window->getTrace()->getCommSize( nextComm->getCommIndex() );
        }
        else if ( nextComm->getTime() == itEnd->getTime() && nextComm->getType() & RECV )
        {
          if ( ( ( nextComm->getType() & LOG &&
                   nextComm->getType() & RECV ) &&
                 window->getTrace()->getLogicalReceive( nextComm->getCommIndex() ) >
                 window->getTrace()->getPhysicalReceive( nextComm->getCommIndex() ) )
               ||
               ( ( nextComm->getType() & PHY &&
                   nextComm->getType() & RECV ) &&
                 window->getTrace()->getPhysicalReceive( nextComm->getCommIndex() ) >=
                 window->getTrace()->getLogicalReceive( nextComm->getCommIndex() ) )
             )
          {
            if ( window->passFilter( nextComm ) )
              bytes += window->getTrace()->getCommSize( nextComm->getCommIndex() );
          }
        }
      }
    }

    ++( *nextComm );
  }

  delete nextComm;

  return bytes;
}

TSemanticValue getTotalSentCommSize( MemoryTrace::iterator *itBegin,
                                     MemoryTrace::iterator *itEnd,
                                     KSingleWindow *window )
{
  TSemanticValue bytes = 0;
  MemoryTrace::iterator *nextComm;

  if ( window->getLevel() >= SYSTEM )
    nextComm = window->copyCPUIterator( itBegin );
  else
    nextComm = window->copyThreadIterator( itBegin );

  /* First we watched if there are previous records of communications
     in the same time of the "record", but only sends */
  --( *nextComm );
  while ( ( !nextComm->isNull() ) && ( nextComm->getTime() == itBegin->getTime() ) )
  {
    if ( window->getFilter()->getPhysical() && !window->getFilter()->getLogical() &&
         ( nextComm->getType() & PHY &&
           nextComm->getType() & COMM &&
           nextComm->getType() & SEND ) )
    {
      if ( window->passFilter( nextComm ) )
        bytes += window->getTrace()->getCommSize( nextComm->getCommIndex() );
    }
    else if ( window->getFilter()->getLogical() &&
              ( nextComm->getType() & LOG &&
                nextComm->getType() & COMM &&
                nextComm->getType() & SEND ) )
    {
      if ( window->passFilter( nextComm ) )
        bytes += window->getTrace()->getCommSize( nextComm->getCommIndex() );
    }
    --( *nextComm );
  }

  ( *nextComm ) = ( *itBegin );
  ++( *nextComm );

  /* Then go forward, until next event */
  while ( !nextComm->isNull() && nextComm->getTime() < itEnd->getTime() )
  {
    if ( ( nextComm->getType() & COMM &&
           nextComm->getType() & SEND ) &&
         window->passFilter( nextComm ) )
      bytes += window->getTrace()->getCommSize( nextComm->getCommIndex() );

    ++( *nextComm );
  }

  delete nextComm;

  return bytes;
}

/**************************
** State functions (Thread)
***************************/

string StateAsIs::name = "State As Is";
TSemanticValue StateAsIs::execute( const SemanticInfo *info )
{
  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->it->getType() & END )
    return IDLE;
  else
    return myInfo->it->getState();
}

void StateAsIs::init( KWindow *whichWindow )
{
  myWindow = ( KSingleWindow * ) whichWindow;
  fillStateGaps = myWindow->getTrace()->getFillStateGaps();
}

bool StateAsIs::validRecord( MemoryTrace::iterator *record )
{
  if ( !SemanticThread::validRecord( record ) )
    return false;

  if ( fillStateGaps && record->getType() & STATE && record->getType() & END )
    return !stateOnSameTime( record, myWindow );

  return true;
}

string Useful::name = "Useful";
TSemanticValue Useful::execute( const SemanticInfo *info )
{
  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->it->getType() & END )
    return IDLE;
  else
    return myInfo->it->getState() == RUNNING ? 1 : 0;
}

void Useful::init( KWindow *whichWindow )
{
  myWindow = ( KSingleWindow * ) whichWindow;
  fillStateGaps = whichWindow->getTrace()->getFillStateGaps();
}

bool Useful::validRecord( MemoryTrace::iterator *record )
{
  if ( !SemanticThread::validRecord( record ) )
    return false;

  if ( fillStateGaps && record->getType() & STATE && record->getType() & END )
    return !stateOnSameTime( record, myWindow );

  return true;
}


string StateSign::name = "State Sign";
TSemanticValue StateSign::execute( const SemanticInfo *info )
{
  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->it->getType() & END )
    return IDLE;
  else
    return myInfo->it->getState() != 0 ? 1 : 0;
}

void StateSign::init( KWindow *whichWindow )
{
  myWindow = ( KSingleWindow * ) whichWindow;
  fillStateGaps = whichWindow->getTrace()->getFillStateGaps();
}

bool StateSign::validRecord( MemoryTrace::iterator *record )
{
  if ( !SemanticThread::validRecord( record ) )
    return false;

  if ( fillStateGaps && record->getType() & STATE && record->getType() & END )
    return !stateOnSameTime( record, myWindow );

  return true;
}


string GivenState::name = "Given State";
TSemanticValue GivenState::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->it->getType() & END )
    return IDLE;
  else
  {
    for ( PRV_UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
    {
      if ( myInfo->it->getState() == parameters[ VALUES ][ i ] )
      {
        tmp = myInfo->it->getState();
        break;
      }
    }
  }

  return tmp;
}

void GivenState::init( KWindow *whichWindow )
{
  myWindow = ( KSingleWindow * ) whichWindow;
  fillStateGaps = whichWindow->getTrace()->getFillStateGaps();
}

bool GivenState::validRecord( MemoryTrace::iterator *record )
{
  if ( !SemanticThread::validRecord( record ) )
    return false;

  if ( fillStateGaps && record->getType() & STATE && record->getType() & END )
    return !stateOnSameTime( record, myWindow );

  return true;
}


string InState::name = "In State";
TSemanticValue InState::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( PRV_UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getType() & END &&
         parameters[ VALUES ][ i ] == 0 )
    {
      tmp = 1;
      break;
    }
    else if ( !( myInfo->it->getType() & END ) &&
              myInfo->it->getState() == parameters[ VALUES ][ i ] )
    {
      tmp = 1;
      break;
    }
  }

  return tmp;
}

void InState::init( KWindow *whichWindow )
{
  myWindow = ( KSingleWindow * ) whichWindow;
  fillStateGaps = whichWindow->getTrace()->getFillStateGaps();
}

bool InState::validRecord( MemoryTrace::iterator *record )
{
  if ( !SemanticThread::validRecord( record ) )
    return false;

  if ( fillStateGaps && record->getType() & STATE && record->getType() & END )
    return !stateOnSameTime( record, myWindow );

  return true;
}


string NotInState::name = "Not In State";
TSemanticValue NotInState::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 1;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( PRV_UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getType() & END &&
         parameters[ VALUES ][ i ] == 0 )
    {
      tmp = 0;
      break;
    }
    if ( !( myInfo->it->getType() & END ) &&
         myInfo->it->getState() == parameters[ VALUES ][ i ] )
    {
      tmp = 0;
      break;
    }
  }

  return tmp;
}

void NotInState::init( KWindow *whichWindow )
{
  myWindow = ( KSingleWindow * ) whichWindow;
  fillStateGaps = whichWindow->getTrace()->getFillStateGaps();
}

bool NotInState::validRecord( MemoryTrace::iterator *record )
{
  if ( !SemanticThread::validRecord( record ) )
    return false;

  if ( fillStateGaps && record->getType() & STATE && record->getType() & END )
    return !stateOnSameTime( record, myWindow );

  return true;
}


string StateRecordDuration::name = "State Record Dur.";
TSemanticValue StateRecordDuration::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( PRV_UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getType() & END &&
         parameters[ VALUES ][ i ] == 0 )
    {
      tmp = timeToNextState( myInfo->it, myWindow );
      break;
    }
    if ( !( myInfo->it->getType() & END ) &&
         myInfo->it->getState() == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->it->getStateEndTime() - myInfo->it->getTime();
      break;
    }
  }

  tmp = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( tmp );

  return tmp;
}

void StateRecordDuration::init( KWindow *whichWindow )
{
  myWindow = ( KSingleWindow * ) whichWindow;
  fillStateGaps = whichWindow->getTrace()->getFillStateGaps();
}

bool StateRecordDuration::validRecord( MemoryTrace::iterator *record )
{
  if ( !SemanticThread::validRecord( record ) )
    return false;

  if ( fillStateGaps && record->getType() & STATE && record->getType() & END )
    return !stateOnSameTime( record, myWindow );

  return true;
}

/**************************
** Event functions (Thread)
***************************/

string LastEventType::name = "Last Evt Type";
TSemanticValue LastEventType::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getEventType();

  return tmp;
}


string LastEventValue::name = "Last Evt Val";
TSemanticValue LastEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getEventValue();

  return tmp;
}


string LastEventValueWOBursts::name = "Last Evt Val w/o Bursts";
TSemanticValue LastEventValueWOBursts::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getEventValue();

  if ( tmp == 0 )
    tmp = myInfo->callingInterval->getValue();

  return tmp;
}


string NextEventType::name = "Next Evt Type";
TSemanticValue NextEventType::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent = NULL;

  if ( myInfo->callingInterval->getLevel() == THREAD )
    nextEvent = myInfo->callingInterval->getWindow()->copyThreadIterator( myInfo->it );
  if ( myInfo->callingInterval->getLevel() == CPU )
    nextEvent = myInfo->callingInterval->getWindow()->copyCPUIterator( myInfo->it );

  getNextEvent( nextEvent, ( KSingleWindow * )
                myInfo->callingInterval->getWindow() );

  if ( nextEvent->isNull() )
    return 0;

  tmp = nextEvent->getEventType();
  delete nextEvent;

  return tmp;
}


string NextEventValue::name = "Next Evt Val";
TSemanticValue NextEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent = NULL;

  if ( myInfo->callingInterval->getLevel() == THREAD )
    nextEvent = myInfo->callingInterval->getWindow()->copyThreadIterator( myInfo->it );
  if ( myInfo->callingInterval->getLevel() == CPU )
    nextEvent = myInfo->callingInterval->getWindow()->copyCPUIterator( myInfo->it );

  getNextEvent( nextEvent, ( KSingleWindow * )
                myInfo->callingInterval->getWindow() );

  if ( nextEvent->isNull() )
    return 0;

  tmp = nextEvent->getEventValue();
  delete nextEvent;

  return tmp;
}


string AverageNextEventValue::name = "Avg Next Evt Val";
TSemanticValue AverageNextEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  TSemanticValue tmpTime = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent = NULL;

  if ( myInfo->callingInterval->getLevel() == THREAD )
    nextEvent = myInfo->callingInterval->getWindow()->copyThreadIterator( myInfo->it );
  if ( myInfo->callingInterval->getLevel() == CPU )
    nextEvent = myInfo->callingInterval->getWindow()->copyCPUIterator( myInfo->it );

  getNextEvent( nextEvent, ( KSingleWindow * )
                myInfo->callingInterval->getWindow() );

  if ( nextEvent->isNull() )
    return 0;

  tmpTime = nextEvent->getTime() - myInfo->it->getTime();
  if ( tmpTime == 0 )
    return 0;
  tmpTime = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( tmpTime );

  tmp = nextEvent->getEventValue() * parameters[ FACTOR ][ 0 ];
  tmp = tmp / tmpTime;
  delete nextEvent;

  return tmp;
}


string AverageLastEventValue::name = "Avg Last Evt Val";
TSemanticValue AverageLastEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  TSemanticValue tmpTime = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent = NULL;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->callingInterval->getLevel() == THREAD )
    nextEvent = myInfo->callingInterval->getWindow()->copyThreadIterator( myInfo->it );
  if ( myInfo->callingInterval->getLevel() == CPU )
    nextEvent = myInfo->callingInterval->getWindow()->copyCPUIterator( myInfo->it );

  getNextEvent( nextEvent, ( KSingleWindow * )
                myInfo->callingInterval->getWindow() );

  if ( nextEvent->isNull() )
    return 0;

  tmpTime = nextEvent->getTime() - myInfo->it->getTime();
  if ( tmpTime == 0 )
    return 0;
  tmpTime = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( tmpTime );

  tmp = myInfo->it->getEventValue() * parameters[ FACTOR ][ 0 ];
  tmp = tmp / tmpTime;
  delete nextEvent;

  return tmp;
}


string GivenEventValue::name = "Given Evt Val";
TSemanticValue GivenEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( PRV_UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getEventValue() == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->it->getEventValue();
      break;
    }
  }

  return tmp;
}


string InEventValue::name = "In Evt Val";
TSemanticValue InEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( PRV_UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getEventValue() == parameters[ VALUES ][ i ] )
    {
      tmp = 1;
      break;
    }
  }

  return tmp;
}


string IntervalBetweenEvents::name = "Int. Between Evt";
TSemanticValue IntervalBetweenEvents::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent = NULL;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->callingInterval->getLevel() == THREAD )
    nextEvent = myInfo->callingInterval->getWindow()->copyThreadIterator( myInfo->it );
  if ( myInfo->callingInterval->getLevel() == CPU )
    nextEvent = myInfo->callingInterval->getWindow()->copyCPUIterator( myInfo->it );

  getNextEvent( nextEvent, ( KSingleWindow * )
                myInfo->callingInterval->getWindow() );

  if ( nextEvent->isNull() )
    return 0;

  tmp = nextEvent->getTime() - myInfo->it->getTime();
  tmp = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( tmp );
  delete nextEvent;

  return tmp;
}


string NotInEventValue::name = "Not In Evt Val";
TSemanticValue NotInEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 1;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( PRV_UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getEventValue() == parameters[ VALUES ][ i ] )
    {
      tmp = 0;
      break;
    }
  }

  return tmp;
}


string InEventRange::name = "In Evt Range";
TSemanticValue InEventRange::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->it->getEventValue() >= parameters[ MINVALUE ][ 0 ] &&
       myInfo->it->getEventValue() <= parameters[ MAXVALUE ][ 0 ] )
  {
    tmp = 1;
  }

  return tmp;
}


string EventBytes::name = "Event Bytes";
TSemanticValue EventBytes::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent = NULL;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->callingInterval->getLevel() == THREAD )
    nextEvent = myInfo->callingInterval->getWindow()->copyThreadIterator( myInfo->it );
  if ( myInfo->callingInterval->getLevel() == CPU )
    nextEvent = myInfo->callingInterval->getWindow()->copyCPUIterator( myInfo->it );

  getNextEvent( nextEvent, ( KSingleWindow * )
                myInfo->callingInterval->getWindow() );

  if ( nextEvent->isNull() )
    return 0;

  tmp = getTotalCommSize( myInfo->it, nextEvent,
                          ( KSingleWindow * ) myInfo->callingInterval->getWindow() );

  delete nextEvent;

  return tmp;
}


string EventSentBytes::name = "Event Sent Bytes";
TSemanticValue EventSentBytes::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent = NULL;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->callingInterval->getLevel() == THREAD )
    nextEvent = myInfo->callingInterval->getWindow()->copyThreadIterator( myInfo->it );
  if ( myInfo->callingInterval->getLevel() == CPU )
    nextEvent = myInfo->callingInterval->getWindow()->copyCPUIterator( myInfo->it );

  getNextEvent( nextEvent, ( KSingleWindow * )
                myInfo->callingInterval->getWindow() );

  if ( nextEvent->isNull() )
    return 0;

  tmp = getTotalSentCommSize( myInfo->it, nextEvent,
                              ( KSingleWindow * ) myInfo->callingInterval->getWindow() );

  delete nextEvent;

  return tmp;
}

/**************************
** Comm functions (Thread)
***************************/

string LastTag::name = "Last Tag";
TSemanticValue LastTag::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->callingInterval->getWindow()->getTrace()->getCommTag(
          myInfo->it->getCommIndex() );
  return tmp;
}


string CommSize::name = "Comm Size";
TSemanticValue CommSize::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC || !( myInfo->it->getType() & COMM ) )
    return 0;

  tmp = myInfo->callingInterval->getWindow()->getTrace()->getCommSize(
          myInfo->it->getCommIndex() );
  return tmp;
}


string CommRecvPartner::name = "Comm Recv. Partner";
TSemanticValue CommRecvPartner::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->callingInterval->getLevel() == THREAD )
    tmp = myInfo->callingInterval->getWindow()->getTrace()->getSenderThread(
            myInfo->it->getCommIndex() );
  else if ( myInfo->callingInterval->getLevel() == CPU )
    tmp = myInfo->callingInterval->getWindow()->getTrace()->getSenderCPU(
            myInfo->it->getCommIndex() );

  return tmp;
}


string CommPartner::name = "Comm Partner";
TSemanticValue CommPartner::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->callingInterval->getLevel() == THREAD )
    tmp = myInfo->callingInterval->getWindow()->getTrace()->getReceiverThread(
            myInfo->it->getCommIndex() );
  else if ( myInfo->callingInterval->getLevel() == CPU )
    tmp = myInfo->callingInterval->getWindow()->getTrace()->getReceiverCPU(
            myInfo->it->getCommIndex() );

  return tmp;
}


string LastSendDuration::name = "Last Send Dur.";
TSemanticValue LastSendDuration::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->callingInterval->getWindow()->getTrace()->getLogicalReceive(
          myInfo->it->getCommIndex() )
        >
        myInfo->callingInterval->getWindow()->getTrace()->getPhysicalReceive(
          myInfo->it->getCommIndex() )
        ?
        myInfo->callingInterval->getWindow()->getTrace()->getLogicalReceive(
          myInfo->it->getCommIndex() )
        :
        myInfo->callingInterval->getWindow()->getTrace()->getPhysicalReceive(
          myInfo->it->getCommIndex() );

  tmp = tmp - myInfo->it->getTime();
  tmp = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( tmp );

  return tmp;
}


string NextRecvDuration::name = "Next Recv Dur.";
TSemanticValue NextRecvDuration::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  MemoryTrace::iterator *nextComm = NULL;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  if ( myInfo->callingInterval->getLevel() == THREAD )
    nextComm = myInfo->callingInterval->getWindow()->copyThreadIterator( myInfo->it );
  if ( myInfo->callingInterval->getLevel() == CPU )
    nextComm = myInfo->callingInterval->getWindow()->copyCPUIterator( myInfo->it );

  ++( *nextComm );
  while ( !nextComm->isNull() && ( nextComm->getType() & COMM && nextComm->getType() & RECV ) )
    ++( *nextComm );

  if ( nextComm->isNull() )
    return 0;

  tmp = myInfo->callingInterval->getWindow()->getTrace()->getLogicalReceive(
          nextComm->getCommIndex() )
        >
        myInfo->callingInterval->getWindow()->getTrace()->getPhysicalReceive(
          nextComm->getCommIndex() )
        ?
        myInfo->callingInterval->getWindow()->getTrace()->getLogicalReceive(
          nextComm->getCommIndex() )
        :
        myInfo->callingInterval->getWindow()->getTrace()->getPhysicalReceive(
          nextComm->getCommIndex() );

  tmp = myInfo->callingInterval->getWindow()->getTrace()->getLogicalSend(
          nextComm->getCommIndex() )
        - myInfo->it->getTime();
  tmp = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( tmp );

  delete nextComm;
  return tmp;
}


string SendBytesInTransit::name = "Send Bytes in Transit";
TSemanticValue SendBytesInTransit::execute( const SemanticInfo *info )
{
  TSemanticValue size = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  KTrace *trace = ( KTrace* )window->getTrace();
  TCommID id = myInfo->it->getCommIndex();

  size = myInfo->callingInterval->getValue();

  if ( window->getFilter()->getLogical() )
  {
    /* If negative communication don't use */
    if ( ( trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id ) &&
           trace->getLogicalReceive( id ) <= trace->getLogicalSend( id ) )
         ||
         ( trace->getLogicalReceive( id ) <= trace->getPhysicalReceive( id ) &&
           trace->getPhysicalReceive( id ) <= trace->getLogicalSend( id ) )
       )
      return size;

    if ( myInfo->it->getType() & LOG && myInfo->it->getType() & SEND )
      size += trace->getCommSize( id );
    else if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RRECV &&
                trace->getLogicalReceive( id ) >
                trace->getPhysicalReceive( id ) ) ||
              ( myInfo->it->getType() & PHY && myInfo->it->getType() & RRECV &&
                trace->getPhysicalReceive( id ) >=
                trace->getLogicalReceive( id ) ) )
      size -= trace->getCommSize( id );
  }
  else
  {
    /* If negative communication don't use */
    if ( trace->getPhysicalReceive( id ) <= trace->getPhysicalSend( id ) )
      return size;

    if ( myInfo->it->getType() & PHY && myInfo->it->getType() & SEND )
      size += trace->getCommSize( id );
    else if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RRECV )
      size -= trace->getCommSize( id );
  }

  return size;
}


string SendMessagesInTransit::name = "Send Messages in Transit";
TSemanticValue SendMessagesInTransit::execute( const SemanticInfo *info )
{
  TSemanticValue msgs = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  KTrace *trace = ( KTrace* )window->getTrace();
  TCommID id = myInfo->it->getCommIndex();

  msgs = myInfo->callingInterval->getValue();

  if ( window->getFilter()->getLogical() )
  {
    /* If negative communication don't use */
    if ( ( trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id ) &&
           trace->getLogicalReceive( id ) <= trace->getLogicalSend( id ) )
         ||
         ( trace->getLogicalReceive( id ) <= trace->getPhysicalReceive( id ) &&
           trace->getPhysicalReceive( id ) <= trace->getLogicalSend( id ) )
       )
      return msgs;

    if ( myInfo->it->getType() & LOG && myInfo->it->getType() & SEND )
      msgs++;
    else if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RRECV &&
                trace->getLogicalReceive( id ) >
                trace->getPhysicalReceive( id ) ) ||
              ( myInfo->it->getType() & PHY && myInfo->it->getType() & RRECV &&
                trace->getPhysicalReceive( id ) >=
                trace->getLogicalReceive( id ) ) )
      msgs--;
  }
  else
  {
    /* If negative communication don't use */
    if ( trace->getPhysicalReceive( id ) <= trace->getPhysicalSend( id ) )
      return msgs;

    if ( myInfo->it->getType() & PHY && myInfo->it->getType() & SEND )
      msgs++;
    else if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RRECV )
      msgs--;
  }

  return msgs;
}

void SendBandWidth::init( KWindow *whichWindow )
{
  TObjectOrder size;

  bandwidth.clear();

  if ( whichWindow->getLevel() >= SYSTEM )
    size = whichWindow->getTrace()->totalCPUs();
  else
    size = whichWindow->getTrace()->totalThreads();

  bandwidth.reserve( size );
  for ( TObjectOrder i = 0; i < size; i++ )
    bandwidth.push_back( 0 );
}


string SendBandWidth::name = "Send BandWidth";
TSemanticValue SendBandWidth::execute( const SemanticInfo *info )
{
  TRecordTime time;
  TSemanticValue bytes;
  bool changes = true;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  KTrace *trace = ( KTrace* )window->getTrace();
  TCommID id = myInfo->it->getCommIndex();
  PRV_INT64 tmp;

  tmp = bandwidth[ myInfo->callingInterval->getOrder() ];

  if ( window->getFilter()->getLogical() &&
       trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id ) )
    time = trace->getLogicalReceive( id ) - trace->getLogicalSend( id );
  else
  {
    if ( window->getFilter()->getPhysical() )
      time = trace->getPhysicalReceive( id ) - trace->getPhysicalSend( id );
    else
      time = trace->getPhysicalReceive( id ) - trace->getLogicalSend( id );
  }
  if ( time == 0 )
    return tmp * 1E-12;
  if ( time < 0 ) time = ( -time );
  time = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( time );

  bytes = trace->getCommSize( id ) / parameters[ FACTOR ][ 0 ];

  if ( window->getFilter()->getLogical() )
  {
    /* If negative communication don't use */
    if ( ( trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id ) &&
           trace->getLogicalReceive( id ) <= trace->getLogicalSend( id ) )
         ||
         ( trace->getLogicalReceive( id ) <= trace->getPhysicalReceive( id ) &&
           trace->getPhysicalReceive( id ) <= trace->getLogicalSend( id ) )
       )
      return tmp * 1E-12;

    if ( myInfo->it->getType() & LOG && myInfo->it->getType() & SEND )
      tmp += ( PRV_INT64 ) ( ( bytes / time ) * 1E12 );
    else if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RRECV &&
                trace->getLogicalReceive( id ) >
                trace->getPhysicalReceive( id ) ) ||
              ( myInfo->it->getType() & PHY && myInfo->it->getType() & RRECV &&
                trace->getPhysicalReceive( id ) >=
                trace->getLogicalReceive( id ) ) )
      tmp -= ( PRV_INT64 ) ( ( bytes / time ) * 1E12 );
    else
      changes = false;
  }
  else
  {
    /* If negative communication don't use */
    if ( trace->getPhysicalReceive( id ) <= trace->getPhysicalSend( id ) )
      return tmp * 1E-12;

    if ( myInfo->it->getType() & PHY && myInfo->it->getType() & SEND )
      tmp += ( PRV_INT64 ) ( ( bytes / time ) * 1E12 );
    else if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RRECV )
      tmp -= ( PRV_INT64 ) ( ( bytes / time ) * 1E12 );
    else
      changes = false;
  }

  if ( changes )
  {
    bandwidth[ myInfo->callingInterval->getOrder() ] = tmp;
    return tmp * 1E-12;
  }

  return bandwidth[ myInfo->callingInterval->getOrder() ] * 1E-12;

}


string RecvBytesInTransit::name = "Recv Bytes in Transit";
TSemanticValue RecvBytesInTransit::execute( const SemanticInfo *info )
{
  TSemanticValue size = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  KTrace *trace = ( KTrace* )window->getTrace();
  TCommID id = myInfo->it->getCommIndex();

  size = myInfo->callingInterval->getValue();

  if ( window->getFilter()->getLogical() )
  {
    /* If negative communication don't use */
    if ( ( trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id ) &&
           trace->getLogicalReceive( id ) <= trace->getLogicalSend( id ) )
         ||
         ( trace->getLogicalReceive( id ) <= trace->getPhysicalReceive( id ) &&
           trace->getPhysicalReceive( id ) <= trace->getLogicalSend( id ) )
       )
      return size;

    if ( myInfo->it->getType() & LOG && myInfo->it->getType() & RSEND )
      size += trace->getCommSize( id );
    else if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RECV &&
                trace->getLogicalReceive( id ) >
                trace->getPhysicalReceive( id ) ) ||
              ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV &&
                trace->getPhysicalReceive( id ) >=
                trace->getLogicalReceive( id ) ) )
      size -= trace->getCommSize( id );
  }
  else
  {
    /* If negative communication don't use */
    if ( trace->getPhysicalReceive( id ) <= trace->getPhysicalSend( id ) )
      return size;

    if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RSEND )
      size += trace->getCommSize( id );
    else if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV )
      size -= trace->getCommSize( id );
  }

  return size;
}


string RecvMessagesInTransit::name = "Recv Messages in Transit";
TSemanticValue RecvMessagesInTransit::execute( const SemanticInfo *info )
{
  TSemanticValue msgs = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  KTrace *trace = ( KTrace* )window->getTrace();
  TCommID id = myInfo->it->getCommIndex();

  msgs = myInfo->callingInterval->getValue();

  if ( window->getFilter()->getLogical() )
  {
    /* If negative communication don't use */
    if ( ( trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id ) &&
           trace->getLogicalReceive( id ) <= trace->getLogicalSend( id ) )
         ||
         ( trace->getLogicalReceive( id ) <= trace->getPhysicalReceive( id ) &&
           trace->getPhysicalReceive( id ) <= trace->getLogicalSend( id ) )
       )
      return msgs;

    if ( myInfo->it->getType() & LOG && myInfo->it->getType() & RSEND )
      msgs++;
    else if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RECV &&
                trace->getLogicalReceive( id ) >
                trace->getPhysicalReceive( id ) ) ||
              ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV &&
                trace->getPhysicalReceive( id ) >=
                trace->getLogicalReceive( id ) ) )
      msgs--;
  }
  else
  {
    /* If negative communication don't use */
    if ( trace->getPhysicalReceive( id ) <= trace->getPhysicalSend( id ) )
      return msgs;

    if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RSEND )
      msgs++;
    else if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV )
      msgs--;
  }

  return msgs;
}


void RecvBandWidth::init( KWindow *whichWindow )
{
  TObjectOrder size;

  bandwidth.clear();

  if ( whichWindow->getLevel() >= SYSTEM )
    size = whichWindow->getTrace()->totalCPUs();
  else
    size = whichWindow->getTrace()->totalThreads();

  bandwidth.reserve( size );
  for ( TObjectOrder i = 0; i < size; i++ )
    bandwidth.push_back( 0 );
}



string RecvBandWidth::name = "Recv BandWidth";
TSemanticValue RecvBandWidth::execute( const SemanticInfo *info )
{
  TRecordTime time;
  TSemanticValue bytes;
  bool changes = true;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  KTrace *trace = ( KTrace* )window->getTrace();
  TCommID id = myInfo->it->getCommIndex();
  PRV_INT64 tmp;

  tmp = bandwidth[ myInfo->callingInterval->getOrder() ];

  if ( window->getFilter()->getLogical() &&
       trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id ) )
    time = trace->getLogicalReceive( id ) - trace->getLogicalSend( id );
  else
  {
    if ( window->getFilter()->getPhysical() )
      time = trace->getPhysicalReceive( id ) - trace->getPhysicalSend( id );
    else
      time = trace->getPhysicalReceive( id ) - trace->getLogicalSend( id );
  }
  if ( time == 0 )
    return tmp * 1E-12;
  if ( time < 0 ) time = ( -time );
  time = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( time );

  bytes = trace->getCommSize( id ) / parameters[ FACTOR ][ 0 ];

  if ( window->getFilter()->getLogical() )
  {
    /* If negative communication don't use */
    if ( ( trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id ) &&
           trace->getLogicalReceive( id ) <= trace->getLogicalSend( id ) )
         ||
         ( trace->getLogicalReceive( id ) <= trace->getPhysicalReceive( id ) &&
           trace->getPhysicalReceive( id ) <= trace->getLogicalSend( id ) )
       )
      return tmp * 1E-12;

    if ( myInfo->it->getType() & LOG && myInfo->it->getType() & RSEND )
      tmp += ( PRV_INT64 ) ( ( bytes / time ) * 1E12 );
    else if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RECV &&
                trace->getLogicalReceive( id ) >
                trace->getPhysicalReceive( id ) ) ||
              ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV &&
                trace->getPhysicalReceive( id ) >=
                trace->getLogicalReceive( id ) ) )
      tmp -= ( PRV_INT64 ) ( ( bytes / time ) * 1E12 );
    else
      changes = false;
  }
  else
  {
    /* If negative communication don't use */
    if ( trace->getPhysicalReceive( id ) <= trace->getPhysicalSend( id ) )
      return tmp * 1E-12;

    if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RSEND )
      tmp += ( PRV_INT64 ) ( ( bytes / time ) * 1E12 );
    else if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV )
      tmp -= ( PRV_INT64 ) ( ( bytes / time ) * 1E12 );
    else
      changes = false;
  }

  if ( changes )
  {
    bandwidth[ myInfo->callingInterval->getOrder() ] = tmp;
    return tmp * 1E-12;
  }

  return bandwidth[ myInfo->callingInterval->getOrder() ] * 1E-12;
}


string RecvNegativeMessages::name = "Recv Negative Messages";
TSemanticValue RecvNegativeMessages::execute( const SemanticInfo *info )
{
  TSemanticValue msgs = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  KTrace *trace = ( KTrace* )window->getTrace();
  TCommID id = myInfo->it->getCommIndex();

  msgs = myInfo->callingInterval->getValue();

  if ( window->getFilter()->getLogical() )
  {
    /* If negative communication use it */
    if ( ( trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id ) &&
           trace->getLogicalReceive( id ) <= trace->getLogicalSend( id ) )
         ||
         ( trace->getLogicalReceive( id ) <= trace->getPhysicalReceive( id ) &&
           trace->getPhysicalReceive( id ) <= trace->getLogicalSend( id ) )
       )
    {
      if ( myInfo->it->getType() & LOG && myInfo->it->getType() & RSEND )
        msgs--;
      else if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RECV &&
                  trace->getLogicalReceive( id ) >
                  trace->getPhysicalReceive( id ) ) ||
                ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV &&
                  trace->getPhysicalReceive( id ) >=
                  trace->getLogicalReceive( id ) ) )
        msgs++;
    }
  }
  else
  {
    /* If negative communication use it */
    if ( trace->getPhysicalReceive( id ) <= trace->getPhysicalSend( id ) )
    {
      if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RSEND )
        msgs--;
      else if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV )
        msgs++;
    }
  }

  return msgs;
}


string RecvNegativeBytes::name = "Recv Negative Bytes";
TSemanticValue RecvNegativeBytes::execute( const SemanticInfo *info )
{
  TSemanticValue size = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  KTrace *trace = ( KTrace* )window->getTrace();
  TCommID id = myInfo->it->getCommIndex();

  size = myInfo->callingInterval->getValue();

  if ( window->getFilter()->getLogical() )
  {
    /* If negative communication use it */
    if ( ( trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id ) &&
           trace->getLogicalReceive( id ) <= trace->getLogicalSend( id ) )
         ||
         ( trace->getLogicalReceive( id ) <= trace->getPhysicalReceive( id ) &&
           trace->getPhysicalReceive( id ) <= trace->getLogicalSend( id ) )
       )
    {
      if ( myInfo->it->getType() & LOG && myInfo->it->getType() & RSEND )
        size -= trace->getCommSize( id );
      else if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RECV &&
                  trace->getLogicalReceive( id ) >
                  trace->getPhysicalReceive( id ) ) ||
                ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV &&
                  trace->getPhysicalReceive( id ) >=
                  trace->getLogicalReceive( id ) ) )
        size += trace->getCommSize( id );
    }
  }
  else
  {
    /* If negative communication use it */
    if ( trace->getPhysicalReceive( id ) <= trace->getPhysicalSend( id ) )
    {
      if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RSEND )
        size -= trace->getCommSize( id );
      else if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV )
        size += trace->getCommSize( id );
    }
  }

  return size;
}


string NumberReceives::name = "Number Of Receives";
TSemanticValue NumberReceives::execute( const SemanticInfo *info )
{
  TSemanticValue msgs = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  KTrace *trace = ( KTrace* )window->getTrace();
  TCommID id = myInfo->it->getCommIndex();

  msgs = myInfo->callingInterval->getValue();

  if ( window->getFilter()->getPhysical() &&
       myInfo->it->getType() & PHY && myInfo->it->getType() & RECV )
    msgs++;

  else if ( window->getFilter()->getLogical() )
  {
    if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RECV &&
           trace->getLogicalReceive( id ) >
           trace->getPhysicalReceive( id ) ) ||
         ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV &&
           trace->getPhysicalReceive( id ) >=
           trace->getLogicalReceive( id ) )
       )
      msgs++;
  }

  return msgs;
}


string NumberReceiveBytes::name = "Number Of Receive Bytes";
TSemanticValue NumberReceiveBytes::execute( const SemanticInfo *info )
{
  TSemanticValue bytes = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  KTrace *trace = ( KTrace* )window->getTrace();
  TCommID id = myInfo->it->getCommIndex();

  bytes = myInfo->callingInterval->getValue();

  if ( window->getFilter()->getPhysical() &&
       myInfo->it->getType() & PHY && myInfo->it->getType() & RECV )
    bytes += trace->getCommSize( id );

  else if ( window->getFilter()->getLogical() )
  {
    if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RECV &&
           trace->getLogicalReceive( id ) >
           trace->getPhysicalReceive( id ) ) ||
         ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV &&
           trace->getPhysicalReceive( id ) >=
           trace->getLogicalReceive( id ) )
       )
      bytes += trace->getCommSize( id );
  }

  return bytes;
}


/**************************
** Object functions (Thread)
***************************/

string ApplicationID::name = "Application ID";
TSemanticValue ApplicationID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  /*if ( myInfo->it->getType() == EMPTYREC )
    return 0;*/

  myInfo->callingInterval->getWindow()->getTrace()->getThreadLocation(
    myInfo->it->getThread(), appl, task, thread );
  tmp = appl;

  return tmp + 1;
}


string TaskID::name = "TaskID";
TSemanticValue TaskID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  /*if ( myInfo->it->getType() == EMPTYREC )
    return 0;*/

  myInfo->callingInterval->getWindow()->getTrace()->getThreadLocation(
    myInfo->it->getThread(), appl, task, thread );
  tmp = myInfo->callingInterval->getWindow()->getTrace()->getGlobalTask( appl, task );

  return tmp + 1;
}


string ThreadID::name = "Thread ID";
TSemanticValue ThreadID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  /*if ( myInfo->it->getType() == EMPTYREC )
    return 0;*/

  tmp = myInfo->it->getThread();

  return tmp + 1;
}


string NodeID::name = "Node ID";
TSemanticValue NodeID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  TNodeOrder node;
  TCPUOrder CPU;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  myInfo->callingInterval->getWindow()->getTrace()->getCPULocation(
    myInfo->it->getCPU(), node, CPU );
  tmp = node;

  return tmp;
}


string CPUID::name = "Cpu ID";
TSemanticValue CPUID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getCPU();

  return tmp;
}


string InApplicationID::name = "In Appl ID";
TSemanticValue InApplicationID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  myInfo->callingInterval->getWindow()->getTrace()->getThreadLocation(
    myInfo->it->getThread(), appl, task, thread );
  tmp = appl + 1;

  for ( TObjectOrder i = 0; i < parameters[ OBJECTS ].size(); i++ )
  {
    if ( tmp == parameters[ OBJECTS ][ i ] )
      return tmp;
  }

  return 0;
}


string InTaskID::name = "In Task ID";
TSemanticValue InTaskID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  TApplOrder appl;
  TTaskOrder task;
  TThreadOrder thread;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  myInfo->callingInterval->getWindow()->getTrace()->getThreadLocation(
    myInfo->it->getThread(), appl, task, thread );
  tmp = myInfo->callingInterval->getWindow()->getTrace()->getGlobalTask( appl, task ) + 1;

  for ( TObjectOrder i = 0; i < parameters[ OBJECTS ].size(); i++ )
  {
    if ( tmp == parameters[ OBJECTS ][ i ] )
      return tmp;
  }

  return 0;
}


string InThreadID::name = "In Thread ID";
TSemanticValue InThreadID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getThread() + 1;

  for ( TObjectOrder i = 0; i < parameters[ OBJECTS ].size(); i++ )
  {
    if ( tmp == parameters[ OBJECTS ][ i ] )
      return tmp;
  }

  return 0;
}


string InNodeID::name = "In Node ID";
TSemanticValue InNodeID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  TNodeOrder node;
  TCPUOrder CPU;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  myInfo->callingInterval->getWindow()->getTrace()->getCPULocation(
    myInfo->it->getCPU(), node, CPU );
  tmp = node;

  for ( TObjectOrder i = 0; i < parameters[ OBJECTS ].size(); i++ )
  {
    if ( tmp == parameters[ OBJECTS ][ i ] )
      return tmp;
  }

  return 0;
}


string InCPUID::name = "In Cpu ID";
TSemanticValue InCPUID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getCPU();

  for ( TObjectOrder i = 0; i < parameters[ OBJECTS ].size(); i++ )
  {
    if ( tmp == parameters[ OBJECTS ][ i ] )
      return tmp;
  }

  return 0;
}
