#include <stdlib.h>
#include "previousfiles.h"

#ifdef WIN32
const string PreviousFiles::previousTracesFile = "\\paraver\\paraverdb";
const string PreviousFiles::previousCFGsFile = "\\paraver\\paravercfgdb";
#else
const string PreviousFiles::previousTracesFile = "/.paraver/paraverdb";
const string PreviousFiles::previousCFGsFile = "/.paraver/paravercfgdb";
#endif

PreviousFiles::PreviousFiles( const string &filename )
{
  fstream myFile;
  string homedir;

#ifdef WIN32
  homedir = getenv( "HOMEDRIVE" );
  homedir.append( getenv( "HOMEPATH" ) );
#else
  homedir = getenv( "HOME" );
#endif
  myFileName.append( homedir );
  myFileName.append( filename );
  myFile.open( myFileName.c_str(), ios::out | ios::app );
  myFile.close();
  myFile.open( myFileName.c_str(), ios::in );

  if ( !myFile )
    throw exception();
  else
  {
    read( myFile );
    myFile.close();
  }
}


PreviousFiles::~PreviousFiles()
{
  if ( myFile.is_open() )
    myFile.close();
}


void PreviousFiles::read( fstream &myFile )
{
  UINT16 lines = 0;

  while ( !myFile.eof() && lines < SIZE )
  {
    string strLine;

    getline( myFile, strLine );
    if( strLine[ 0 ] != '#' &&  strLine != "" )
    {
      listFiles.push_back( strLine );
      lines++;
    }
  }
}


bool PreviousFiles::update( )
{
  fstream myFile;

  myFile.open( myFileName.c_str(), ios::out );
  if ( !myFile )
    return false;

  for ( vector<string>::iterator it = listFiles.begin(); it != listFiles.end(); ++it )
    myFile << *it << endl;

  myFile.close();

  return true;
}


bool PreviousFiles::add( const string &newFile )
{
  vector<string> v;

  v.push_back( newFile );
  for ( vector<string>::iterator it = listFiles.begin(); it != listFiles.end(); ++it )
  {
    if ( *it != newFile && v.size() <= SIZE )
       v.push_back( *it );
  }

  v.swap( listFiles );
/*
  for ( vector<string>::iterator it = listFiles.begin(); it != listFiles.end(); ++it )
  {
    printf("en el swqp %s\n", (*it).c_str());
  }
*/
  return update();
}

const vector<string>& PreviousFiles::getFiles() const
{
  return listFiles;
}
