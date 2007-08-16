#include "semanticthreadfunctions.h"

TSemanticValue StateAsIs::execute( const SemanticInfo *info )
{
  const SemanticThreadInfo *myInfo = (const SemanticThreadInfo *) info;
  if( myInfo->it->getType() == EMPTYREC )
    return 0;
  return myInfo->it->getState();
}


