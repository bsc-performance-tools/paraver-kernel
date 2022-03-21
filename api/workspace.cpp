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

#include "workspace.h"

#include <sys/stat.h>
#ifdef _WIN32
  #include <shlobj.h>
  #include <Shlwapi.h>
#else
  #include <sys/types.h>
#endif

using std::string;
using std::vector;
using std::pair;

Workspace::Workspace()
{
}

Workspace::~Workspace()
{
}

string Workspace::getName() const
{
  return name;
}

WorkspaceType Workspace::getType() const
{
  return myType;
}

vector<WorkspaceValue> Workspace::getAutoTypes() const
{
  return autoTypes;
}

vector<pair<string,string> > Workspace::getHintCFGs() const
{
  return hintCFGs;
}

pair<string,string> Workspace::getHintCFG( size_t whichHint ) const
{
  return hintCFGs[ whichHint ];
}

void Workspace::setName( string& whichName )
{
  name = whichName;
}

void Workspace::setType( WorkspaceType whichType )
{
  myType = whichType;
  for( vector<WorkspaceValue>::iterator it = autoTypes.begin(); it != autoTypes.end(); ++it )
    it->myType = whichType;
}

void Workspace::setAutoTypes( vector<WorkspaceValue>& whichAutoTypes )
{
  autoTypes = whichAutoTypes;
}

void Workspace::addHintCFG( pair<string,string>& whichCFG )
{
  hintCFGs.push_back( whichCFG );
}

void Workspace::addHintCFG( size_t position, pair<string,string>& whichCFG )
{
  hintCFGs.insert( hintCFGs.begin() + position, whichCFG );
}

void Workspace::removeHintCFG( size_t whichHint )
{
  hintCFGs.erase( hintCFGs.begin() + whichHint );
}


void Workspace::modifyHintCFG( size_t position, pair<string,string>& whichCFG )
{
  hintCFGs[ position ] = whichCFG;
}


void Workspace::clearHintCFGs()
{
  hintCFGs.clear();
}


void Workspace::loadXML( const std::string &wsFile )
{
  std::ifstream ifs( wsFile.c_str() );
  if( ifs.good() )
  {
    boost::archive::xml_iarchive ia( ifs );
    ia >> boost::serialization::make_nvp( "workspace", *this );
  }
  
  ifs.close();
}


void Workspace::saveXML( std::string &wsFile )
{
  std::ofstream ofs( wsFile.c_str() );
  if( ofs.good() )
  {
    boost::archive::xml_oarchive oa( ofs );
    oa << boost::serialization::make_nvp( "workspace", *this );
  }

  ofs.close();
}


std::string readToCFGSeparator( std::ifstream& ifs, std::ofstream& ofs )
{
  std::string line;
  static const size_t cfgSeparatorSize = std::string( cfgSeparator ).length();
  while( !ifs.eof() )
  {
    getline( ifs, line );
    if( line.substr( 0, cfgSeparatorSize ) == cfgSeparator )
    {
      return line.substr( cfgSeparatorSize, line.npos );
    }

    ofs << line << std::endl;
  }

  return "";
}


void Workspace::importWSXML( std::string& wsFile, const std::string& paraverUserDir )
{
  std::ifstream ifs( wsFile.c_str() );

  if( ifs.good() )
  {
    std::ofstream ofs( paraverUserDir + "/temp.ws" );
    if( ofs.good() )
    {
      firstCFGName = readToCFGSeparator( ifs, ofs );
    }
    ofs.close();
    
    wsFileXMLPos = ifs.tellg();
    
    loadXML( paraverUserDir + "/temp.ws" );
  }

  ifs.close();
}


void Workspace::importWSCFGs( std::string& wsFile, const std::string& paraverUserDir )
{
  std::ifstream ifs( wsFile.c_str() );
  std::string nextCfgName = firstCFGName;

  if( ifs.good() )
  {
    ifs.seekg( wsFileXMLPos );

    std::string importedCFGsPath( paraverUserDir );

#ifdef _WIN32
    importedCFGsPath.append( "\\importedCFGS\\" );
    SHCreateDirectoryEx( nullptr, importedCFGsPath.c_str(), nullptr );
    importedCFGsPath.append( name ).append( "\\" );
    SHCreateDirectoryEx( nullptr, importedCFGsPath.c_str(), nullptr );
#else
    importedCFGsPath.append( "/importedCFGS/" );
    mkdir( importedCFGsPath.c_str(), (mode_t)0700 );
    importedCFGsPath.append( name ).append( "/" );
    mkdir( importedCFGsPath.c_str(), (mode_t)0700 );
#endif

    for_each( hintCFGs.begin(), hintCFGs.end(), 
              [&]( std::pair< std::string, std::string >& elem )
              {
#ifdef _WIN32
                elem.first = importedCFGsPath + elem.first.substr( elem.first.find_last_of( '\\' ) + 1 );
#else
                elem.first = importedCFGsPath + elem.first.substr( elem.first.find_last_of( '/' ) + 1 );
#endif
              } );

    std::ofstream ofs;
    while( nextCfgName != "" )
    {
      ofs.open( importedCFGsPath + nextCfgName );
      nextCfgName = readToCFGSeparator( ifs, ofs );
      ofs.close();
    }
  }

  ifs.close();
}
