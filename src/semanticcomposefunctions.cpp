#include "semanticcomposefunctions.h"

TSemanticValue ComposeAsIs::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = (const SemanticHighInfo *) info;
  return myInfo->values[ 0 ];
}
