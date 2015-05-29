/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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

#include <stdlib.h>
#include "previousfiles.h"

using namespace std;

#ifdef WIN32
const string PreviousFiles::previousTracesFile = "\\paraver\\paraverdb";
const string PreviousFiles::previousCFGsFile = "\\paraver\\paravercfgdb";
const string PreviousFiles::previousTreatedTracesFile = "\\paraver\\paravertreatedtracesdb";
#else
const string PreviousFiles::previousTracesFile = "/.paraver/paraverdb";
const string PreviousFiles::previousCFGsFile = "/.paraver/paravercfgdb";
const string PreviousFiles::previousTreatedTracesFile = "/.paraver/paravertreatedtracesdb";
#endif


PreviousFiles *PreviousFiles::createPreviousTraces()
{
  return new PreviousFiles( previousTracesFile );
}


PreviousFiles *PreviousFiles::createPreviousCFGs()
{
  return new PreviousFiles( previousCFGsFile );
}


PreviousFiles *PreviousFiles::createPreviousTreatedTraces()
{
  return new PreviousFiles( previousTreatedTracesFile, true );
}


PreviousFiles::PreviousFiles( const string &filename, bool purge )
{
  fstream myFile;
  string homedir;

#ifdef WIN32
  homedir = getenv( "HOMEDRIVE" );
  homedir.append( getenv( "HOMEPATH" ) );
#else
  homedir = string( getenv( "HOME" ) );
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

  if ( purge )
  {
    vector< string > v;

    // Test existance
    for( vector< string >::iterator it = listFiles.begin(); it != listFiles.end(); ++it )
    {
      myFile.open( (*it).c_str(), ios::in );
      if ( !myFile.fail() )
      {
        myFile.close();
        v.push_back( *it );
      }
    }

    // The traces list is updated
    v.swap( listFiles );

    // The file is also updated
    update();
  }
}


PreviousFiles::~PreviousFiles()
{
  if ( myFile.is_open() )
    myFile.close();
}


void PreviousFiles::read( fstream &myFile )
{
  PRV_UINT16 lines = 0;

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

  return update();
}


const vector<string>& PreviousFiles::getFiles() const
{
  return listFiles;
}
