#include "semanticcpufunctions.h"

string ActiveThread::name = "Active Thd";
TSemanticValue ActiveThread::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return myInfo->values[ 0 ];
}


string ActiveThreadSign::name = "Active Thd Sign";
TSemanticValue ActiveThreadSign::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  if ( myInfo->values[ 0 ] )
    return 1;
  return 0;
}


string ActiveThreadValues::name = "Active Thd Val";
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


string ActiveThreadValuesSign::name = "Active Thd Val Sign";
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
