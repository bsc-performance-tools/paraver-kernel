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

vector<pair<string,string> > Workspace::getHintCFGs() const
{
  return hintCFGs;
}

std::pair<std::string,std::string> Workspace::getHintCFG( size_t whichHint ) const
{
  return hintCFGs[ whichHint ];
}

void Workspace::setName( string& whichName )
{
  name = whichName;
}

void Workspace::addHintCFG( pair<string,string>& whichCFG )
{
  hintCFGs.push_back( whichCFG );
}

void Workspace::addHintCFG( size_t position, std::pair<std::string,std::string>& whichCFG )
{
  hintCFGs.insert( hintCFGs.begin() + position, whichCFG );
}

void Workspace::removeHintCFG( size_t whichHint )
{
  hintCFGs.erase( hintCFGs.begin() + whichHint );
}

void Workspace::modifyHintCFG( size_t position, std::pair<std::string,std::string>& whichCFG )
{
  hintCFGs[ position ] = whichCFG;
}

void Workspace::clearHintCFGs()
{
  hintCFGs.clear();
}
