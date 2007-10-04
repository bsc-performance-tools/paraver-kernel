#include <math.h>
#include "semanticcomposefunctions.h"
#include "kwindow.h"

TSemanticValue ComposeAsIs::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return myInfo->values[ 0 ];
}


TSemanticValue ComposeSign::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  if ( myInfo->values[ 0 ] != 0 )
    return 1;
  return 0;
}


TSemanticValue ComposeUnsign::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp;

  if ( myInfo->values[ 0 ] != 0 )
    tmp = 1;
  else
    tmp = 0;

  return 1 - tmp;
}


TSemanticValue ComposeMod::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue remainder = 0, divider;

  if ( parameters[ DIVIDER ][ 0 ] != 0 )
    divider = parameters[ DIVIDER ][ 0 ];
  else
    divider = 1;

  remainder = fmod( myInfo->values[ 0 ], divider );

  return remainder;
}


TSemanticValue ComposeModPlus1::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue remainder = 0, divider;

  if ( parameters[ DIVIDER ][ 0 ] != 0 )
    divider = parameters[ DIVIDER ][ 0 ];
  else
    divider = 1;

  remainder = fmod( myInfo->values[ 0 ], divider );

  return remainder + 1;
}


TSemanticValue ComposeDivide::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue quotient = 0, divider;

  if ( parameters[ DIVIDER ][ 0 ] != 0 )
    quotient = myInfo->values[ 0 ] / parameters[ DIVIDER ][ 0 ];
  else
  {
    divider = 1;
    quotient = fmod( myInfo->values[ 0 ], divider );
  }
  return quotient;
}


TSemanticValue ComposeProduct::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  return myInfo->values[ 0 ] * parameters[ FACTOR ][ 0 ];
}


TSemanticValue ComposeAdding::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  return myInfo->values[ 0 ] + parameters[ FACTOR ][ 0 ];
}


TSemanticValue ComposeSubstract::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  return myInfo->values[ 0 ] - parameters[ FACTOR ][ 0 ];
}


TSemanticValue ComposeSelectRange::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp = 0;

  if ( myInfo->values[ 0 ] <= parameters[ MAXVALUE ][ 0 ] &&
       myInfo->values[ 0 ] >= parameters[ MINVALUE ][ 0 ] )
    tmp = myInfo->values[ 0 ];
  else
    tmp = 0;

  return tmp;
}


TSemanticValue ComposeIsInRange::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp = 0;

  if ( myInfo->values[ 0 ] <= parameters[ MAXVALUE ][ 0 ] &&
       myInfo->values[ 0 ] >= parameters[ MINVALUE ][ 0 ] )
    tmp = 1;
  else
    tmp = 0;

  return tmp;
}


TSemanticValue ComposeIsEqual::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp = 0;

  for ( TParamIndex i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->values[ 0 ] == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->values[ 0 ];
      break;
    }
  }

  return tmp;
}


TSemanticValue ComposeIsEqualSign::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp = 0;

  for ( TParamIndex i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->values[ 0 ] == parameters[ VALUES ][ i ] )
    {
      tmp = 1;
      break;
    }
  }

  return tmp;
}


void ComposeStackedValue::init( KWindow *whichWindow )
{
  myStack.clear();

  if ( whichWindow->getTrace()->totalThreads() >
       whichWindow->getTrace()->totalCPUs() )
  {
    myStack.reserve( whichWindow->getTrace()->totalThreads() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalThreads(); i++ )
      myStack.push_back( stack<TSemanticValue>() );
  }
  else
  {
    myStack.reserve( whichWindow->getTrace()->totalCPUs() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalCPUs(); i++ )
      myStack.push_back( stack<TSemanticValue>() );
  }
}


TSemanticValue ComposeStackedValue::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TObjectOrder tmpOrder = myInfo->callingInterval->getOrder();

  if ( myInfo->values[ 0 ] != 0 )
    myStack[ tmpOrder ].push( myInfo->values[ 0 ] );
  else
  {
    if ( !myStack[ tmpOrder ].empty() )
      myStack[ tmpOrder ].pop();
  }

  if ( myStack[ tmpOrder ].empty() )
    return 0;

  return myStack[ tmpOrder ].top();
}


void ComposeInStackedValue::init( KWindow *whichWindow )
{
  myStack.clear();

  if ( whichWindow->getTrace()->totalThreads() >
       whichWindow->getTrace()->totalCPUs() )
  {
    myStack.reserve( whichWindow->getTrace()->totalThreads() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalThreads(); i++ )
      myStack.push_back( stack<TSemanticValue>() );
  }
  else
  {
    myStack.reserve( whichWindow->getTrace()->totalCPUs() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalCPUs(); i++ )
      myStack.push_back( stack<TSemanticValue>() );
  }
}


TSemanticValue ComposeInStackedValue::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TObjectOrder tmpOrder = myInfo->callingInterval->getOrder();

  if ( myInfo->values[ 0 ] != 0 )
    myStack[ tmpOrder ].push( myInfo->values[ 0 ] );
  else
  {
    if ( !myStack[ tmpOrder ].empty() )
      myStack[ tmpOrder ].pop();
  }

  if ( myStack[ tmpOrder ].empty() )
    return 0;

  return myStack[ tmpOrder ].top() == parameters[ VALUE ][ 0 ] ?
         myStack[ tmpOrder ].top() : 0;
}


void ComposeNestingLevel::init( KWindow *whichWindow )
{
  myStack.clear();

  if ( whichWindow->getTrace()->totalThreads() >
       whichWindow->getTrace()->totalCPUs() )
  {
    myStack.reserve( whichWindow->getTrace()->totalThreads() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalThreads(); i++ )
      myStack.push_back( 0 );
  }
  else
  {
    myStack.reserve( whichWindow->getTrace()->totalCPUs() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalCPUs(); i++ )
      myStack.push_back( 0 );
  }
}

TSemanticValue ComposeNestingLevel::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TObjectOrder tmpOrder = myInfo->callingInterval->getOrder();

  if ( myInfo->values[ 0 ] != 0 )
    myStack[ tmpOrder ]++;
  else
    myStack[ tmpOrder ]--;

  return myStack[ tmpOrder ];
}


void ComposeDelta::init( KWindow *whichWindow )
{
  prevValue.clear();
  semPrevValue.clear();

  if ( whichWindow->getTrace()->totalThreads() >
       whichWindow->getTrace()->totalCPUs() )
  {
    prevValue.reserve( whichWindow->getTrace()->totalThreads() );
    semPrevValue.reserve( whichWindow->getTrace()->totalThreads() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalThreads(); i++ )
    {
      prevValue.push_back( 0 );
      semPrevValue.push_back( 0 );
    }
  }
  else
  {
    prevValue.reserve( whichWindow->getTrace()->totalCPUs() );
    semPrevValue.reserve( whichWindow->getTrace()->totalCPUs() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalCPUs(); i++ )
    {
      prevValue.push_back( 0 );
      semPrevValue.push_back( 0 );
    }
  }
}

TSemanticValue ComposeDelta::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TObjectOrder tmpOrder = myInfo->callingInterval->getOrder();
  TSemanticValue result = 0;

  if ( myInfo->values[ 0 ] > semPrevValue[ tmpOrder ] )
    result = myInfo->values[ 0 ] - semPrevValue[ tmpOrder ];
  else if ( myInfo->values[ 0 ] == semPrevValue[ tmpOrder ] )
    result = prevValue[ tmpOrder ];

  semPrevValue[ tmpOrder ] = myInfo->values[ 0 ];
  prevValue[ tmpOrder ] = result;

  return result;
}


TSemanticValue ComposeBurstTime::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  return myInfo->callingInterval->getEnd()->getTime() -
         myInfo->callingInterval->getBegin()->getTime();
}


TSemanticValue ComposeJoinBursts::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  return myInfo->values[ 0 ];
}
