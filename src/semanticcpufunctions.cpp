#include "semanticcpufunctions.h"

TSemanticValue ActiveThread::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return myInfo->values[ 0 ];
}


TSemanticValue ActiveThreadSign::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  if ( myInfo->values[ 0 ] )
    return 1;
  return 0;
}


TSemanticValue ActiveThreadValues::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if( parameters[ VALUES ][ i ] == myInfo->values[ 0 ] )
      return myInfo->values[ 0 ];
  }
  return 0;
}


TSemanticValue ActiveThreadValuesSign::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for( UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if( parameters[ VALUES ][ i ] == myInfo->values[ 0 ] )
      return 1;
  }
  return 0;
}
