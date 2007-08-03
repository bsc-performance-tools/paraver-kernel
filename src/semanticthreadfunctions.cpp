#include "semanticthreadfunctions.h"

TSemanticValue StateAsIs::execute( const SemanticThreadInfo& info )
{
  if( info.it->getType() & EMPTYREC )
    return 0;
  return info.it->getState();
}


