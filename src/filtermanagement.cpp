#include "filter.h"
#include "filtermanagement.h"
#include "functionmanagement.h"


void createFilter()
{
  vector<string> groups;
  vector<string> names;
  vector<vector<FilterFunction *> > functions;

  groups.push_back( "Filter functions" );
  functions.push_back( vector<FilterFunction *>() );

  functions[0].push_back( new FilterAll() );
  functions[0].push_back( new FilterNotEqual() );
  functions[0].push_back( new FilterGreater() );
  functions[0].push_back( new FilterFewer() );
  functions[0].push_back( new FilterEqual() );
  functions[0].push_back( new FilterNone() );
  functions[0].push_back( new FilterRange() );


  for ( UINT16 i = 0; i < functions[0].size(); i++ )
    names.push_back( functions[0][i]->getName() );


  FunctionManagement<FilterFunction>::getInstance( groups, names, functions );
}

