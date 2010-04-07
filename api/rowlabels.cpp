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

#include <fstream>
#include <sstream>
#include "rowlabels.h"
#include "paraverlabels.h"

RowLabels::RowLabels()
{}

RowLabels::RowLabels( const string& filename )
{
  ifstream rowFile( filename.c_str() );
  if ( !rowFile )
    return;

  while ( !rowFile.eof() )
  {
    string strLine;
    string strLevel;
    string strSize;
    vector<string> *tmpvector = NULL;

    getline( rowFile, strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' )
      continue;

    istringstream auxStream( strLine );
    getline( auxStream, strLevel, ' ' ); // 'LEVEL'
    getline( auxStream, strLevel, ' ' );

    if( strLevel == LEVEL_WORKLOAD )
      tmpvector = &workload;
    else if( strLevel == LEVEL_APPLICATION )
      tmpvector = &appl;
    else if( strLevel == LEVEL_TASK )
      tmpvector = &task;
    else if( strLevel == LEVEL_THREAD )
      tmpvector = &thread;
    else if( strLevel == LEVEL_SYSTEM )
      tmpvector = &system;
    else if( strLevel == LEVEL_NODE )
      tmpvector = &node;
    else if( strLevel == LEVEL_CPU )
      tmpvector = &cpu;
    else
      continue;

    getline( auxStream, strSize, 'S' ); // 'SIZE'
    getline( auxStream, strSize, ' ' ); // 'SIZE'
    getline( auxStream, strSize );

    istringstream sizeStream( strSize );
    int size;

    if ( !( sizeStream >> size ) )
      continue;

    int i = 0;
    while ( !rowFile.eof() && i < size )
    {
      getline( rowFile, strLine );
      tmpvector->push_back( strLine );
      i++;
    }

  }

  rowFile.close();
}

RowLabels::~RowLabels()
{}

string RowLabels::getRowLabel( TWindowLevel whichLevel, TObjectOrder whichRow ) const
{
  const vector<string> *tmpvector = NULL;
  switch( whichLevel )
  {
    case THREAD:
      tmpvector = &thread;
      break;
    case TASK:
      tmpvector = &task;
      break;
    case APPLICATION:
      tmpvector = &appl;
      break;
    case WORKLOAD:
      tmpvector = &workload;
      break;
    case CPU:
      tmpvector = &cpu;
      break;
    case NODE:
      tmpvector = &node;
      break;
    case SYSTEM:
      tmpvector = &system;
      break;
    default:
      tmpvector = NULL;
  }

  if( tmpvector == NULL )
    return "";
  else
  {
    if( tmpvector->begin() == tmpvector->end() ||
        whichRow > tmpvector->size() )
      return "";
    else
      return (*tmpvector)[ whichRow ];
  }

  return "";
}
