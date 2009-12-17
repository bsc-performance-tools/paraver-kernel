/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

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

void NotCompressed::seekbegin()
{
  file.seekg( 0, ios_base::beg );
}

void NotCompressed::seekend()
{
  file.seekg( 0, ios_base::end );
}

void NotCompressed::seekg( streampos pos )
{
  file.seekg( pos );
}

streampos NotCompressed::tellg()
{
  return file.tellg();
}

bool NotCompressed::canseekend()
{
  return true;
}

bool NotCompressed::good() const
{
  return file.good();
}

void NotCompressed::clear()
{
  file.clear();
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

void Compressed::seekbegin()
{
  gzseek( file, 0, SEEK_SET );
}

void Compressed::seekend()
{
  gzseek( file, 0, SEEK_END );
}

void Compressed::seekg( streampos pos )
{
  gzseek( file, pos, SEEK_SET );
}

streampos Compressed::tellg()
{
  return gztell( file );
}

bool Compressed::canseekend()
{
  return false;
}

bool Compressed::good() const
{
  return file != NULL;
}

void Compressed::clear()
{}
