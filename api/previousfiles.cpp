#include "previousfiles.h"

PreviousFiles::PreviousFiles( const string &filename ): myFileName( filename )
{
  fstream myFile;

  myFile.open( myFileName.c_str(), ios::in );
  if ( !myFile.fail() )
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
    listFiles.insert( strLine );

    lines ++;
  }
}


bool PreviousFiles::update( )
{
  fstream myFile;

  myFile.open(  myFileName.c_str(), ios::out );
  if ( !myFile.fail() )
    return false;

  for ( set<string>::iterator it = listFiles.begin(); it != listFiles.end(); it++ )
    myFile << *it << endl;

  myFile.close();

  return true;
}


bool PreviousFiles::add( string newFile )
{
  set<string>::iterator it = listFiles.find( newFile );

  if ( it != listFiles.end() )
      listFiles.erase( it );

  listFiles.insert( listFiles.begin(), newFile );

  if ( listFiles.size() > SIZE )
    listFiles.erase( --listFiles.end() );

  return update();
}

const set<string>& PreviousFiles::getFiles() const
{
  return listFiles;
}
