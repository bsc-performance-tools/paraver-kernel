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

#include <string>
#include "kfilter.h"
#include "filtermanagement.h"
#include "functionmanagement.h"


void createFilter()
{
  vector<string> groups;
  vector<string> names;
  vector<vector<FilterFunction *> > functions;

  groups.push_back( "Filter functions" );
  functions.push_back( vector<FilterFunction *>() );

  functions[0].push_back( new FilterAll() );
  functions[0].push_back( new FilterNotEqual() );
  functions[0].push_back( new FilterGreater() );
  functions[0].push_back( new FilterFewer() );
  functions[0].push_back( new FilterEqual() );
  functions[0].push_back( new FilterNone() );
  functions[0].push_back( new FilterRange() );


  for ( UINT16 i = 0; i < functions[0].size(); i++ )
    names.push_back( functions[0][i]->getName() );


  FunctionManagement<FilterFunction>::getInstance( groups, names, functions );
}

