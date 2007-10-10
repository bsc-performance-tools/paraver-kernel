#include "semanticderivedfunctions.h"
#include "kwindow.h"

TSemanticValue DerivedAdd::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] + myInfo->values[ 1 ];

  return tmp;
}

TSemanticValue DerivedProduct::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] * myInfo->values[ 1 ];

  return tmp;
}

TSemanticValue DerivedSubstract::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] - myInfo->values[ 1 ];

  return tmp;
}

TSemanticValue DerivedDivide::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0.0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  if( myInfo->values[ 1 ] == 0 )
    return 0.0;

  tmp = myInfo->values[ 0 ] / myInfo->values[ 1 ];

  return tmp;
}

TSemanticValue DerivedMaximum::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] > myInfo->values[ 1 ] ?
        myInfo->values[ 0 ] :
        myInfo->values[ 1 ];

  return tmp;
}

TSemanticValue DerivedMinimum::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] < myInfo->values[ 1 ] ?
        myInfo->values[ 0 ] :
        myInfo->values[ 1 ];

  return tmp;
}

TSemanticValue ControlDerivedClearBy::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  TObjectOrder tmpOrder = myInfo->callingInterval->getOrder();

  if( myInfo->values[ 1 ] < prevValue[ tmpOrder ] )
  {
    tmp = 0;
    state[ tmpOrder ] = myInfo->values[ 0 ];
    prevValue[ tmpOrder ] = myInfo->values[ 1 ];
    prevResult[ tmpOrder ] = tmp;
  }
  else
  {
    if( state[ tmpOrder ] != myInfo->values[ 0 ] )
    {
      tmp = myInfo->values[ 0 ];
      state[ tmpOrder ] = myInfo->values[ 0 ];
      prevValue[ tmpOrder ] = myInfo->values[ 1 ];
      prevResult[ tmpOrder ] = tmp;
    }
    else
      tmp = prevResult[ tmpOrder ];
  }

  return tmp;
}

void ControlDerivedClearBy::init( KWindow *whichWindow )
{
  TObjectOrder size = 0;

  prevValue.clear();
  state.clear();
  prevResult.clear();

  if( whichWindow->getLevel() >= SYSTEM )
    size = whichWindow->getTrace()->totalNodes();
  else
    size = whichWindow->getTrace()->totalApplications();

  prevValue.reserve( size );
  state.reserve( size );
  prevResult.reserve( size );
  for( TObjectOrder i = 0; i < size; i++ )
  {
    prevValue.push_back( 0 );
    state.push_back( 0 );
    prevResult.push_back( 0 );
  }
}

TSemanticValue ControlDerivedMaximum::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] > myInfo->values[ 1 ] ?
        myInfo->values[ 0 ] :
        myInfo->values[ 1 ];

  return tmp;
}


TSemanticValue ControlDerivedAdd::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] + myInfo->values[ 1 ];

  return tmp;
}
