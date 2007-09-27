#include "semanticthreadfunctions.h"
#include "kwindow.h"

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


/**************************
** State functions (Thread)
***************************/

TSemanticValue StateAsIs::execute( const SemanticInfo *info )
{
  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;
  return myInfo->it->getState();
}


TSemanticValue Useful::execute( const SemanticInfo *info )
{
  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;
  return myInfo->it->getState() == RUNNING ? 1 : 0;
}


TSemanticValue StateSign::execute( const SemanticInfo *info )
{
  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;
  return myInfo->it->getState() != 0 ? 1 : 0;
}


TSemanticValue GivenState::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getState() == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->it->getState();
      break;
    }
  }

  return tmp;
}


TSemanticValue InState::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getState() == parameters[ VALUES ][ i ] )
    {
      tmp = 1;
      break;
    }
  }

  return tmp;
}


TSemanticValue NotInState::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 1;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getState() == parameters[ VALUES ][ i ] )
    {
      tmp = 0;
      break;
    }
  }

  return tmp;
}


TSemanticValue StateRecordDuration::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getState() == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->it->getStateEndTime() - myInfo->it->getTime();
      break;
    }
  }

  return tmp;
}


/**************************
** Event functions (Thread)
***************************/

TSemanticValue LastEventType::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getEventType();

  return tmp;
}


TSemanticValue LastEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getEventValue();

  return tmp;
}

//OJO!!!
//Esta funcion no esta bien. Se tiene que rehacer mirando el codigo antiguo.
TSemanticValue LastEventValueWOBursts::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getEventValue();

  return tmp;
}


TSemanticValue NextEventType::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent;

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

  tmp = nextEvent->getEventType();
  delete nextEvent;

  return tmp;
}


TSemanticValue NextEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent;

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

  tmp = nextEvent->getEventValue();
  delete nextEvent;

  return tmp;
}


TSemanticValue AverageNextEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  TSemanticValue tmpTime = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent;

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

  tmp = nextEvent->getEventValue() * parameters[ FACTOR ][ 0 ];
  tmp = tmp / tmpTime;
  delete nextEvent;

  return tmp;
}


TSemanticValue AverageLastEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  TSemanticValue tmpTime = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent;

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

  tmp = myInfo->it->getEventValue() * parameters[ FACTOR ][ 0 ];
  tmp = tmp / tmpTime;
  delete nextEvent;

  return tmp;
}


TSemanticValue GivenEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getEventValue() == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->it->getEventValue();
      break;
    }
  }

  return tmp;
}


TSemanticValue InEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getEventValue() == parameters[ VALUES ][ i ] )
    {
      tmp = 1;
      break;
    }
  }

  return tmp;
}


TSemanticValue IntervalBetweenEvents::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent;

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
  delete nextEvent;

  return tmp;
}


TSemanticValue NotInEventValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 1;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  for ( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->it->getEventValue() == parameters[ VALUES ][ i ] )
    {
      tmp = 0;
      break;
    }
  }

  return tmp;
}


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


TSemanticValue EventBytes::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;
  MemoryTrace::iterator *nextEvent;

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

  return tmp;
}


/**************************
** Comm functions (Thread)
***************************/

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


TSemanticValue CommSize::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->callingInterval->getWindow()->getTrace()->getCommSize(
          myInfo->it->getCommIndex() );
  return tmp;
}


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

  return tmp;
}


TSemanticValue NextRecvDuration::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  MemoryTrace::iterator *nextComm;

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

  delete nextComm;
  return tmp;
}


TSemanticValue SendBytesInTransit::execute( const SemanticInfo *info )
{
  TSemanticValue size = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  Trace *trace = window->getTrace();
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


TSemanticValue SendMessagesInTransit::execute( const SemanticInfo *info )
{
  TSemanticValue msgs = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  Trace *trace = window->getTrace();
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

TSemanticValue SendBandWidth::execute( const SemanticInfo *info )
{
  TRecordTime time;
  TSemanticValue bytes;
  bool changes = true;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  Trace *trace = window->getTrace();
  TCommID id = myInfo->it->getCommIndex();
  INT64 tmp;

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
  //time = time / win->unit_val;
  if ( time == 0 )
    return tmp * 1E-12;
  if ( time < 0 ) time = ( -time );

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
      tmp += ( INT64 ) ( ( bytes / time ) * 1E12 );
    else if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RRECV &&
                trace->getLogicalReceive( id ) >
                trace->getPhysicalReceive( id ) ) ||
              ( myInfo->it->getType() & PHY && myInfo->it->getType() & RRECV &&
                trace->getPhysicalReceive( id ) >=
                trace->getLogicalReceive( id ) ) )
      tmp -= ( INT64 ) ( ( bytes / time ) * 1E12 );
    else
      changes = false;
  }
  else
  {
    /* If negative communication don't use */
    if ( trace->getPhysicalReceive( id ) <= trace->getPhysicalSend( id ) )
      return tmp * 1E-12;

    if ( myInfo->it->getType() & PHY && myInfo->it->getType() & SEND )
      tmp += ( INT64 ) ( ( bytes / time ) * 1E12 );
    else if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RRECV )
      tmp -= ( INT64 ) ( ( bytes / time ) * 1E12 );
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


TSemanticValue RecvBytesInTransit::execute( const SemanticInfo *info )
{
  TSemanticValue size = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  Trace *trace = window->getTrace();
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


TSemanticValue RecvMessagesInTransit::execute( const SemanticInfo *info )
{
  TSemanticValue msgs = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  Trace *trace = window->getTrace();
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


TSemanticValue RecvBandWidth::execute( const SemanticInfo *info )
{
  TRecordTime time;
  TSemanticValue bytes;
  bool changes = true;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  Trace *trace = window->getTrace();
  TCommID id = myInfo->it->getCommIndex();
  INT64 tmp;

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
  //time = time / win->unit_val;
  if ( time == 0 )
    return tmp * 1E-12;
  if ( time < 0 ) time = ( -time );

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
      tmp += ( INT64 ) ( ( bytes / time ) * 1E12 );
    else if ( ( myInfo->it->getType() & LOG && myInfo->it->getType() & RECV &&
                trace->getLogicalReceive( id ) >
                trace->getPhysicalReceive( id ) ) ||
              ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV &&
                trace->getPhysicalReceive( id ) >=
                trace->getLogicalReceive( id ) ) )
      tmp -= ( INT64 ) ( ( bytes / time ) * 1E12 );
    else
      changes = false;
  }
  else
  {
    /* If negative communication don't use */
    if ( trace->getPhysicalReceive( id ) <= trace->getPhysicalSend( id ) )
      return tmp * 1E-12;

    if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RSEND )
      tmp += ( INT64 ) ( ( bytes / time ) * 1E12 );
    else if ( myInfo->it->getType() & PHY && myInfo->it->getType() & RECV )
      tmp -= ( INT64 ) ( ( bytes / time ) * 1E12 );
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


TSemanticValue RecvNegativeMessages::execute( const SemanticInfo *info )
{
  TSemanticValue msgs = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  Trace *trace = window->getTrace();
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


TSemanticValue RecvNegativeBytes::execute( const SemanticInfo *info )
{
  TSemanticValue size = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  Trace *trace = window->getTrace();
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


TSemanticValue NumberReceives::execute( const SemanticInfo *info )
{
  TSemanticValue msgs = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  Trace *trace = window->getTrace();
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


TSemanticValue NumberReceiveBytes::execute( const SemanticInfo *info )
{
  TSemanticValue bytes = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  KSingleWindow *window = ( KSingleWindow * ) myInfo->callingInterval->getWindow();
  Trace *trace = window->getTrace();
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

TSemanticValue ApplicationID::execute( const SemanticInfo *info )
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
  tmp = appl;

  return tmp;
}


TSemanticValue TaskID::execute( const SemanticInfo *info )
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
  tmp = myInfo->callingInterval->getWindow()->getTrace()->getGlobalTask( appl, task );

  return tmp;
}


TSemanticValue ThreadID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getThread();

  return tmp;
}


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


TSemanticValue CPUID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getCPU();

  return tmp;
}


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
  tmp = appl;

  for ( TObjectOrder i = 0; i < parameters[ OBJECTS ].size(); i++ )
  {
    if ( tmp == parameters[ OBJECTS ][ i ] )
      return tmp;
  }

  return 0;
}


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
  tmp = myInfo->callingInterval->getWindow()->getTrace()->getGlobalTask( appl, task );

  for ( TObjectOrder i = 0; i < parameters[ OBJECTS ].size(); i++ )
  {
    if ( tmp == parameters[ OBJECTS ][ i ] )
      return tmp;
  }

  return 0;
}


TSemanticValue InThreadID::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticThreadInfo *myInfo = ( const SemanticThreadInfo * ) info;

  if ( myInfo->it->getType() == EMPTYREC )
    return 0;

  tmp = myInfo->it->getThread();

  for ( TObjectOrder i = 0; i < parameters[ OBJECTS ].size(); i++ )
  {
    if ( tmp == parameters[ OBJECTS ][ i ] )
      return tmp;
  }

  return 0;
}


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
