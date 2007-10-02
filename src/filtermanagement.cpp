#include "filtermanagement.h"


FilterFunction *FilterManagement::createFunction( const string& functionName )
{
  FilterFunction *tmpFunc = NULL;

  if ( !functionName.compare( "All" ) )
    tmpFunc = new FilterAll();
  else if ( !functionName.compare( "!=" ) )
    tmpFunc = new FilterNotEqual();
  else if ( !functionName.compare( "!= " ) )
    tmpFunc = new FilterNotEqual();
  else if ( !functionName.compare( ">" ) )
    tmpFunc = new FilterGreater();
  else if ( !functionName.compare( "<" ) )
    tmpFunc = new FilterFewer();
  else if ( !functionName.compare( "=" ) )
    tmpFunc = new FilterEqual();
  else if ( !functionName.compare( "None" ) )
    tmpFunc = new FilterNone();
  else if ( !functionName.compare( "[x,y]" ) )
    tmpFunc = new FilterRange();

  return tmpFunc;
}

