#include "tracestream.h"


TraceStream *TraceStream::openFile( const string& filename )
{
  string strExt = filename.substr( filename.length() - 3 );

  if ( strExt.compare( ".gz" ) == 0 )
    return new Compressed( filename );
  else
    return new NotCompressed( filename );
}


NotCompressed::NotCompressed( const string& filename )
{
  file.open( filename.c_str() );
}

void NotCompressed::open( const string& filename )
{
  file.open( filename.c_str() );
}

void NotCompressed::close()
{
  file.close();
}

void NotCompressed::getline( string& strLine )
{
  std::getline( file, strLine );
}

bool NotCompressed::eof()
{
  return file.eof();
}


Compressed::Compressed( const string& filename )
{
  file = gzopen( filename.c_str(), "r" );
}

void Compressed::open( const string& filename )
{
  file = gzopen( filename.c_str(), "r" );
}

void Compressed::close()
{
  gzclose( file );
}

void Compressed::getline( string& strLine )
{
  gzgets( file, tmpLine, LINESIZE );
  strLine = tmpLine;
}

bool Compressed::eof()
{
  return gzeof( file );
}

