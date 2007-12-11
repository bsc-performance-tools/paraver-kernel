#include "filter.h"
#include "filtermanagement.h"
#include "functionmanagement.h"


void createFilter()
{
  vector<string> names;
  vector<FilterFunction *> functions;

  functions.push_back(new FilterAll());
  functions.push_back(new FilterNotEqual());
  functions.push_back(new FilterGreater());
  functions.push_back(new FilterFewer());
  functions.push_back(new FilterEqual());
  functions.push_back(new FilterNone());
  functions.push_back(new FilterRange());


  for(UINT16 i = 0; i < functions.size(); i++)
        names.push_back(functions[i]->getName());
                                                                                
                                                                                
  FunctionManagement<FilterFunction>::getInstance(names, functions);
                                                                                

}


/*
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
*/

