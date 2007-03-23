#include "processmodel.h"
#include <sstream>
#include <iostream>

ProcessModel::ProcessModel( string headerInfo )
{
  istringstream headerBuffer( headerInfo );
  string strNumberApplications;
  TApplOrder numberApplications;

  ready = false;
  std::getline( headerBuffer, strNumberApplications, ':' );

  istringstream sstrNumberAppl( strNumberApplications );

  if ( !( sstrNumberAppl >> numberApplications ) ||
       numberApplications == 0 )
  {
    throw std::string( "Incorrect number of applications in header" );
  }

  for ( TApplOrder countAppl = 0; countAppl < numberApplications; countAppl++ )
  {
    applications.push_back( ProcessModelAppl( countAppl ) );

  }

  ready = true;
}

