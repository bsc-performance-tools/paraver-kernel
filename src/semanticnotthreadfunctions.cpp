#include "semanticnotthreadfunctions.h"
#include "paraverstatisticfuncions.h"

TSemanticValue Adding::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
    tmp += myInfo->values[ i ];

  return tmp;
}

TSemanticValue AddingSign::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
    tmp += myInfo->values[ i ];

  return tmp > 0 ? 1 : 0;
}

TSemanticValue Average::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
    tmp += myInfo->values[ i ];

  return tmp / myInfo->values.size();
}

TSemanticValue Maximum::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
  {
    if ( myInfo->values[ i ] > tmp )
      tmp = myInfo->values[ i ];
  }

  return tmp;
}

TSemanticValue Minimum::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
  {
    if ( tmp == 0 || myInfo->values[ i ] < tmp )
      tmp = myInfo->values[ i ];
  }

  return tmp;
}


TSemanticValue Activity::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->values[ 0 ] == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->values[ 0 ];
      break;
    }
  }

  if ( tmp == 0 ) return 0;

  for ( TObjectOrder i = 1; i < myInfo->values.size(); i++ )
  {
    if ( myInfo->values[ i ] != tmp )
      return 0;
  }

  return tmp;
}


TSemanticValue InActivity::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->values[ 0 ] == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->values[ 0 ];
      break;
    }
  }

  if ( tmp == 0 ) return 0;

  for ( TObjectOrder i = 1; i < myInfo->values.size(); i++ )
  {
    if ( myInfo->values[ i ] != tmp )
      return 0;
  }

  return 1;
}


TSemanticValue NotThreadMode::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  TSemanticValue tmpValues[ myInfo->values.size() ];

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
    tmpValues[ i ] = myInfo->values[ i ];

  tmp = ParaverStatisticFunctions<TSemanticValue>::mode( tmpValues,
        myInfo->values.size() );

  return tmp;
}


TSemanticValue ObjectI::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ ( TObjectOrder ) parameters[ OBJECT ][ 0 ] ];

  return tmp;
}


TSemanticValue AddObjectsI::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for( TObjectOrder i = 0; i < parameters[ OBJECTS ].size(); i++ )
    tmp += myInfo->values[ ( TObjectOrder ) parameters[ OBJECTS ][ i ] ];

  return tmp;
}
