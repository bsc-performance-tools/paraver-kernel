#include <string>
#include <fstream>
#include <sstream>
#include "paraverconfig.h"

ParaverConfig::ParaverConfig() :
    precision( 2 )
{}

UINT32 ParaverConfig::getPrecision() const
{
  return precision;
}

void ParaverConfig::readParaverConfigFile( ParaverConfig& config )
{
  ifstream file;
  string strLine;
  string strTag;
  string strFile;

  strFile.append( getenv( "HOME" ) );
  strFile.append( "/.paraver/paraver" );

  file.open( strFile.c_str() );

  if ( !file )
    return;

  while ( !file.eof() )
  {
    getline( file, strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' || strLine[ 0 ] == '<' )
      continue;

    istringstream auxStream( strLine );
    getline( auxStream, strTag, ' ' );

    if ( strTag.compare( "WhatWhere.num_decimals:" ) == 0 )
    {
      string strNumDecimals;

      getline( auxStream, strNumDecimals );
      istringstream streamNumDecimals( strNumDecimals );

      streamNumDecimals >> config.precision;
    }
  }
  file.close();
}
