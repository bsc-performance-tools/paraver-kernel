#include "kwindow.h"
#include "semanticcomposefunctions.h"

KSingleWindow::KSingleWindow( Trace *whichTrace ): KWindow( whichTrace )
{
  topCompose1 = new ComposeAsIs();
  topCompose2 = new ComposeAsIs();
  composeThread = new ComposeAsIs();
  functionThread = NULL;

  if ( myTrace->totalThreads() > myTrace->totalCPUs() )
  {
    recordsByTime.reserve( myTrace->totalThreads() );
    for ( TThreadOrder i = 0; i < myTrace->totalThreads(); i++ )
      recordsByTime.push_back( NULL );
    // Interval compose se crean AQUI...
  }
  else
  {
    recordsByTime.reserve( myTrace->totalCPUs() );
    for ( TCPUOrder i = 0; i < myTrace->totalCPUs(); i++ )
      recordsByTime.push_back( NULL );
    // y AQUI...
  }

  intervalThread.reserve( myTrace->totalThreads() );
  for ( TThreadOrder i = 0; i < myTrace->totalThreads(); i++ )
    intervalThread.push_back( IntervalThread( this, i ) );
}


RecordList *KSingleWindow::init( TRecordTime initialTime, TCreateList create )
{
  if ( level >= SYSTEM )
  {
    if ( initialTime > 0 )
      myTrace->getRecordByTimeCPU( recordsByTime, initialTime );
    else
    {
      for ( TCPUOrder i = 0; i < myTrace->totalCPUs(); i++ )
      {
        if ( recordsByTime[ i ] != NULL )
          delete recordsByTime[ i ];
        recordsByTime[ i ] = myTrace->CPUBegin( i );
      }
    }
  }
  else
  {
    if ( initialTime > 0 )
      myTrace->getRecordByTimeThread( recordsByTime, initialTime );
    else
    {
      for ( TThreadOrder i = 0; i < myTrace->totalThreads(); i++ )
      {
        if ( recordsByTime[ i ] != NULL )
          delete recordsByTime[ i ];
        recordsByTime[ i ] = myTrace->threadBegin( i );
      }
    }
  }

  // Se debe sustituir intervalThread por el top compose
  for ( TThreadOrder i = 0; i < intervalThread.size(); i++ )
    intervalThread[ i ].init( initialTime, create );

  return NULL;
}


void KSingleWindow::setLevelFunction( TWindowLevel whichLevel,
                                      SemanticFunction *whichFunction )
{
  if ( whichLevel == THREAD )
  {
    if ( functionThread != NULL )
      delete functionThread;
    functionThread = ( SemanticThread * ) whichFunction;
    for ( TThreadOrder i = 0; i < intervalThread.size(); i++ )
      intervalThread[ i ].setSemanticFunction( ( SemanticThread * )whichFunction );
  }
}


void KSingleWindow::setFunctionParam( TWindowLevel whichLevel,
                                      TParamIndex whichParam,
                                      const TParamValue& newValue )
{
  if ( whichLevel == THREAD )
  {
    functionThread->setParam( whichParam, newValue );;
  }
}


RecordList *KSingleWindow::calcNext( TObjectOrder whichObject )
{
  // Se debe sustituir intervalThread por el top compose
  return intervalThread[ whichObject ].calcNext();
}


RecordList *KSingleWindow::calcPrev( TObjectOrder whichObject )
{
  // Se debe sustituir intervalThread por el top compose
  return intervalThread[ whichObject ].calcPrev();
}


TRecordTime KSingleWindow::getBeginTime( TObjectOrder whichObject ) const
{
  // Se debe sustituir intervalThread por el top compose
  return intervalThread[ whichObject ].getBeginTime();
}


TRecordTime KSingleWindow::getEndTime( TObjectOrder whichObject ) const
{
  // Se debe sustituir intervalThread por el top compose
  return intervalThread[ whichObject ].getEndTime();
}


TSemanticValue KSingleWindow::getValue( TObjectOrder whichObject ) const
{
  // Se debe sustituir intervalThread por el top compose
  return intervalThread[ whichObject ].getValue();
}


vector<IntervalCompose> *KSingleWindow::getLevelInterval( TWindowLevel whichLevel )
{
  if( whichLevel == TOPCOMPOSE1 )
    return &intervalTopCompose1;
  else if( whichLevel == TOPCOMPOSE2 )
    return &intervalTopCompose2;
  else if( whichLevel == THREAD )
    return &intervalComposeThread;

  return NULL;
}

