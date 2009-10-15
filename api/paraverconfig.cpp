#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#ifdef WIN32
  #include <shlobj.h>
#else
  #include <sys/stat.h>
  #include <sys/types.h>
#endif
#include "paraverconfig.h"


ParaverConfig *ParaverConfig::instance = NULL;

ParaverConfig *ParaverConfig::getInstance()
{
  if ( ParaverConfig::instance == NULL )
    ParaverConfig::instance = new ParaverConfig();
  return ParaverConfig::instance;
}

ParaverConfig::ParaverConfig() :
    precision( 2 ),
    histoNumColumns( 20 ),
    showUnits( true ),
    thousandSep( true )
{
  loadMap();
}

ParaverConfig::~ParaverConfig()
{
  unLoadMap();
}

UINT32 ParaverConfig::getPrecision() const
{
  return precision;
}

TObjectOrder ParaverConfig::getHistoNumColumns() const
{
  return histoNumColumns;
}

bool ParaverConfig::getShowUnits() const
{
  return showUnits;
}

bool ParaverConfig::getThousandSep() const
{
  return thousandSep;
}

void ParaverConfig::setPrecision( UINT32 prec )
{
  precision = prec;
}

void ParaverConfig::setHistoNumColumns( TObjectOrder columns )
{
  histoNumColumns = columns;
}

void ParaverConfig::setShowUnits( bool units )
{
  showUnits = units;
}

void ParaverConfig::setThousandSep( bool sep )
{
  thousandSep = sep;
}

void ParaverConfig::readParaverConfigFile()
{
  ParaverConfig& config = *ParaverConfig::instance;

  ifstream file;
  ifstream fileXML;
  string strLine;
  string strTag;
  string strFile;
  string strFileXML;
  string homedir;

#ifdef WIN32
  homedir = getenv( "HOMEDRIVE" );
  homedir.append( getenv( "HOMEPATH" ) );
#else
  homedir = getenv( "HOME" );
#endif
  strFile.append( homedir );
#ifdef WIN32
  strFile.append( "\\paraver\\paraver" );
#else
  strFile.append( "/.paraver/paraver" );
#endif
  strFileXML.append( ".xml" );

  fileXML.open( strFileXML.c_str() );

  if ( !fileXML )
  {
    // XML file can't be opened
    fileXML.close();

    // Does file in old format exists?
    file.open( strFile.c_str() );
    if ( file )
    {
      // Read it!
      while ( !file.eof() )
      {
        getline( file, strLine );
        if ( strLine.length() == 0 )
          continue;
        else if ( strLine[ 0 ] == '#' || strLine[ 0 ] == '<' )
          continue;

        istringstream auxStream( strLine );
        getline( auxStream, strTag, ' ' );

        map<string, PropertyFunction*>::iterator it =
          config.propertyFunctions.find( strTag );
        if ( it != config.propertyFunctions.end() )
        {
          it->second->parseLine( auxStream, config );
        }
      }
      file.close();
    }

    // Try to create/rewrite new XML format
    if ( !ParaverConfig::writeDefaultConfig())
      return;
    else
    { // Right! And also write old info.
      ParaverConfig::writeParaverConfigFile();
    }
  }
  else
  {
    // XML file found ! load it!
    fileXML.close();
    instance->loadXML( strFileXML );
  }
}


void ParaverConfig::writeParaverConfigFile()
{
  ofstream file;
  string strFile;
  string homedir;

#ifdef WIN32
  homedir = getenv( "HOMEDRIVE" );
  homedir.append( getenv( "HOMEPATH" ) );
#else
  homedir = getenv( "HOME" );
#endif
  strFile.append( homedir );

#ifdef WIN32
  strFile.append( "\\paraver\\paraver" );
  string tmpPath( homedir + "\\paraver" );

  int len = tmpPath.length() + 1;
  wchar_t *wText = new wchar_t[len];
  memset(wText,0,len);
  ::MultiByteToWideChar( CP_ACP, NULL, tmpPath.c_str(), -1, wText, len );

  SHCreateDirectoryEx( NULL, wText, NULL );
  delete []wText;
#else
  strFile.append( "/.paraver/paraver" );
  mkdir( ( homedir + "/.paraver" ).c_str(), (mode_t)0700 );
#endif
  strFile.append( ".xml" );

  instance->saveXML( strFile.c_str() );
}


bool ParaverConfig::writeDefaultConfig()
{
  ofstream file;
  string strFile;
  string homedir;

#ifdef WIN32
  homedir = getenv( "HOMEDRIVE" );
  homedir.append( getenv( "HOMEPATH" ) );
#else
  homedir = getenv( "HOME" );
#endif
  strFile.append( homedir );

#ifdef WIN32
  strFile.append( "\\paraver\\paraver.xml" );
  string tmpPath( homedir + "\\paraver" );

  int len = tmpPath.length() + 1;
  wchar_t *wText = new wchar_t[len];
  memset(wText,0,len);
  ::MultiByteToWideChar( CP_ACP, NULL, tmpPath.c_str(), -1, wText, len );

  SHCreateDirectoryEx( NULL, wText, NULL );
  delete []wText;
#else
  strFile.append( "/.paraver/paraver.xml" );
  mkdir( ( homedir + "/.paraver" ).c_str(), (mode_t)0700 );
#endif

  file.open( strFile.c_str() );

  if ( !file )
    return false;

  file.close();

  return true;
}

void ParaverConfig::saveXML( const string &filename )
{
  std::ofstream ofs( filename.c_str() );
  boost::archive::xml_oarchive oa( ofs );
  oa << boost::serialization::make_nvp( "ParaverConfiguration", this );
}

void ParaverConfig::loadXML( const string &filename )
{
  std::ifstream ifs( filename.c_str() );
  boost::archive::xml_iarchive ia( ifs );
  ia >> boost::serialization::make_nvp( "ParaverConfiguration", *this );
}

void ParaverConfig::loadMap()
{
  propertyFunctions[ "WhatWhere.num_decimals:" ] = new WWNumDecimals();
  propertyFunctions[ "Analyzer2D.num_columns:" ] = new HistoNumColumns();
  propertyFunctions[ "Analyzer2D.units:" ] = new HistoUnits();
  propertyFunctions[ "Analyzer2D.thousandsep:" ] = new HistoThousanSep();
}

void ParaverConfig::unLoadMap()
{
  for ( map<string, PropertyFunction*>::iterator it = propertyFunctions.begin();
        it != propertyFunctions.end();
        ++it )
    delete ( *it ).second;
}

void WWNumDecimals::parseLine( istringstream& line, ParaverConfig& config )
{
  string strNumDecimals;
  UINT32 precision;

  getline( line, strNumDecimals );
  istringstream streamNumDecimals( strNumDecimals );

  if ( streamNumDecimals >> precision )
    config.setPrecision( precision );
}

void HistoNumColumns::parseLine( istringstream& line, ParaverConfig& config )
{
  string strNumColumns;
  TObjectOrder numColumns;

  getline( line, strNumColumns );
  istringstream streamNumColumns( strNumColumns );

  if ( streamNumColumns >> numColumns )
    config.setHistoNumColumns( numColumns );
}

void HistoUnits::parseLine( istringstream& line, ParaverConfig& config )
{
  string strUnits;

  getline( line, strUnits );

  if ( strUnits.compare( "True" ) == 0 )
    config.setShowUnits( true );
  else
    config.setShowUnits( false );
}

void HistoThousanSep::parseLine( istringstream& line, ParaverConfig& config )
{
  string strSep;

  getline( line, strSep );

  if ( strSep.compare( "True" ) == 0 )
    config.setThousandSep( true );
  else
    config.setThousandSep( false );
}
