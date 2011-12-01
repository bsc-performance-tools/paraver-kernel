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

#include "semanticnotthreadfunctions.h"
#include "paraverstatisticfunctions.h"
#include "kwindow.h"

string Adding::name = "Adding";
TSemanticValue Adding::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
    tmp += myInfo->values[ i ];

  return tmp;
}


string AddingSign::name = "AddingSign";
TSemanticValue AddingSign::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
    tmp += myInfo->values[ i ];

  return tmp > 0 ? 1 : 0;
}

string Average::name = "Average";
TSemanticValue Average::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
    tmp += myInfo->values[ i ];

  return tmp / myInfo->values.size();
}


string Maximum::name = "Maximum";
TSemanticValue Maximum::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
  {
    if ( myInfo->values[ i ] > tmp )
      tmp = myInfo->values[ i ];
  }

  return tmp;
}


string Minimum::name = "Minimum";
TSemanticValue Minimum::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
  {
    if ( tmp == 0 || myInfo->values[ i ] < tmp )
      tmp = myInfo->values[ i ];
  }

  return tmp;
}


string Activity::name = "Activity";
TSemanticValue Activity::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( PRV_UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->values[ 0 ] == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->values[ 0 ];
      break;
    }
  }

  if ( tmp == 0 ) return 0;

  for ( TObjectOrder i = 1; i < myInfo->values.size(); i++ )
  {
    if ( myInfo->values[ i ] != tmp )
      return 0;
  }

  return tmp;
}


string InActivity::name = "In Activity";
TSemanticValue InActivity::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for ( PRV_UINT32 i = 0; i < parameters[ VALUES ].size(); i++ )
  {
    if ( myInfo->values[ 0 ] == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->values[ 0 ];
      break;
    }
  }

  if ( tmp == 0 ) return 0;

  for ( TObjectOrder i = 1; i < myInfo->values.size(); i++ )
  {
    if ( myInfo->values[ i ] != tmp )
      return 0;
  }

  return 1;
}


string NotThreadMode::name = "Mode";
TSemanticValue NotThreadMode::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  TSemanticValue *tmpValues;
  tmpValues = new TSemanticValue[ myInfo->values.size() ];

  for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
    tmpValues[ i ] = myInfo->values[ i ];

  tmp = ParaverStatisticFunctions<TSemanticValue>::mode( tmpValues,
        myInfo->values.size() );

  delete[] tmpValues;

  return tmp;
}


string ObjectI::name = "Thread i";
TSemanticValue ObjectI::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ ( TObjectOrder ) parameters[ OBJECT ][ 0 ] ];

  return tmp;
}


string AddObjectsI::name = "Add Tasks";
TSemanticValue AddObjectsI::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;

  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  for( TObjectOrder i = 0; i < parameters[ OBJECTS ].size(); i++ )
    tmp += myInfo->values[ ( TObjectOrder ) parameters[ OBJECTS ][ i ] ];

  return tmp;
}


string ChangedValue::name = "Changed value";
void ChangedValue::init( KWindow *whichWindow )
{
  lastValues.clear();
}

TSemanticValue ChangedValue::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0.0;

  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  if( lastValues.size() != 0 )
  {
    for ( TObjectOrder i = 0; i < myInfo->values.size(); i++ )
    {
      if( myInfo->values[ i ] != lastValues[ i ] )
      {
        tmp = myInfo->values[ i ];
        break;
      }
    }
  }

  lastValues = myInfo->values;

  return tmp;
}
