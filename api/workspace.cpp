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


// load/save XML for import/export certain workspaces

void Workspace::loadXML( std::string &wsDir )
{
  // Read user defined
  std::ifstream ifs( wsDir.c_str() );
  //ifs.open( wsDir );
  if( ifs.good() ) // due to xml_iarchive needs to be destroyed before fstream is closed
  {
    boost::archive::xml_iarchive ia( ifs );
    ia >> boost::serialization::make_nvp( "workspace", *this );
  }
  ifs.close();
}


void Workspace::saveXML( std::string &wsDir )
{
  std::ofstream ofs( wsDir.c_str() );
  if( ofs.good() ) // due to xml_oarchive needs to be destroyed before fstream is closed
  {
    boost::archive::xml_oarchive oa( ofs );
    oa << boost::serialization::make_nvp( "workspace", *this );
  }
  ofs.close();
}
