#include <string>
#include <fstream>
#include <sstream>
#include "paraverconfig.h"

ParaverConfig *ParaverConfig::instance = NULL;

ParaverConfig *ParaverConfig::getInstance()
{
  if( ParaverConfig::instance == NULL )
    ParaverConfig::instance = new ParaverConfig();
  return ParaverConfig::instance;
}

ParaverConfig::ParaverConfig() :
    precision( 2 ),
    histoNumColumns( 20 )
{}

UINT32 ParaverConfig::getPrecision() const
{
  return precision;
}

TObjectOrder ParaverConfig::getHistoNumColumns() const
{
  return histoNumColumns;
}

void ParaverConfig::readParaverConfigFile()
{
  ParaverConfig& config = *ParaverConfig::instance;

  ifstream file;
  string strLine;
  string strTag;
  string strFile;

  strFile.append( getenv( "HOME" ) );
  strFile.append( "/.paraver/paraver" );

  file.open( strFile.c_str() );

  if ( !file )
  {
    if( !ParaverConfig::writeDefaultConfig())
      return;
    file.open( strFile.c_str() );
  }

  while ( !file.eof() )
  {
    getline( file, strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' || strLine[ 0 ] == '<' )
      continue;

    istringstream auxStream( strLine );
    getline( auxStream, strTag, ' ' );

// Hay que hacer algo parecido a lo del cfg para cuando se
// quiera leer el fichero entero
    if ( strTag.compare( "WhatWhere.num_decimals:" ) == 0 )
    {
      string strNumDecimals;

      getline( auxStream, strNumDecimals );
      istringstream streamNumDecimals( strNumDecimals );

      streamNumDecimals >> config.precision;
    }
    else if ( strTag.compare( "Analyzer2D.num_columns:" ) == 0 )
    {
      string strNumColumns;

      getline( auxStream, strNumColumns );
      istringstream streamNumColumns( strNumColumns );

      streamNumColumns >> config.histoNumColumns;
    }

  }
  file.close();
}

bool ParaverConfig::writeDefaultConfig()
{
  ofstream file;
  string strFile;

  strFile.append( getenv( "HOME" ) );
  strFile.append( "/.paraver/paraver" );

  file.open( strFile.c_str() );

  if( !file )
    return false;

  return true;
}
