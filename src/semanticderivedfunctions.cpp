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

#include "semanticderivedfunctions.h"
#include "kwindow.h"

using namespace std;

string DerivedAdd::name = "add";
TSemanticValue DerivedAdd::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] + myInfo->values[ 1 ];

  return tmp;
}


string DerivedProduct::name = "product";
TSemanticValue DerivedProduct::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] * myInfo->values[ 1 ];

  return tmp;
}


string DerivedSubstract::name = "substract";
TSemanticValue DerivedSubstract::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] - myInfo->values[ 1 ];

  return tmp;
}


string DerivedDivide::name = "divide";
TSemanticValue DerivedDivide::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0.0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  if( myInfo->values[ 1 ] == 0 )
    return 0.0;

  tmp = myInfo->values[ 0 ] / myInfo->values[ 1 ];

  return tmp;
}


string DerivedMaximum::name = "maximum";
TSemanticValue DerivedMaximum::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] > myInfo->values[ 1 ] ?
        myInfo->values[ 0 ] :
        myInfo->values[ 1 ];

  return tmp;
}


string DerivedMinimum::name = "minimum";
TSemanticValue DerivedMinimum::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] < myInfo->values[ 1 ] ?
        myInfo->values[ 0 ] :
        myInfo->values[ 1 ];

  return tmp;
}


string DerivedDifferent::name = "different";
TSemanticValue DerivedDifferent::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] != myInfo->values[ 1 ] ?
        1 :
        0;

  return tmp;
}


string ControlDerivedClearBy::name = "controlled: clear by";
TSemanticValue ControlDerivedClearBy::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  TObjectOrder tmpOrder = myInfo->callingInterval->getOrder();

  if( myInfo->values[ 1 ] < prevValue[ tmpOrder ] )
  {
    tmp = 0;
    state[ tmpOrder ] = myInfo->values[ 0 ];
    prevValue[ tmpOrder ] = myInfo->values[ 1 ];
    prevResult[ tmpOrder ] = tmp;
  }
  else
  {
    if( state[ tmpOrder ] != myInfo->values[ 0 ] )
    {
      tmp = myInfo->values[ 0 ];
      state[ tmpOrder ] = myInfo->values[ 0 ];
      prevValue[ tmpOrder ] = myInfo->values[ 1 ];
      prevResult[ tmpOrder ] = tmp;
    }
    else
      tmp = prevResult[ tmpOrder ];

    prevValue[ tmpOrder ] = myInfo->values[ 1 ];
  }

  return tmp;
}

void ControlDerivedClearBy::init( KWindow *whichWindow )
{
  TObjectOrder size = 0;

  prevValue.clear();
  state.clear();
  prevResult.clear();

  if( whichWindow->getLevel() >= SYSTEM )
    size = whichWindow->getTrace()->totalCPUs();
  else
    size = whichWindow->getTrace()->totalThreads();

  prevValue.reserve( size );
  state.reserve( size );
  prevResult.reserve( size );
  for( TObjectOrder i = 0; i < size; i++ )
  {
    prevValue.push_back( 0 );
    state.push_back( 0 );
    prevResult.push_back( 0 );
  }
}


string ControlDerivedMaximum::name = "controlled: maximum";
TSemanticValue ControlDerivedMaximum::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] > myInfo->values[ 1 ] ?
        myInfo->values[ 0 ] :
        myInfo->values[ 1 ];

  return tmp;
}


string ControlDerivedAdd::name = "controlled: add";
TSemanticValue ControlDerivedAdd::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  tmp = myInfo->values[ 0 ] + myInfo->values[ 1 ];

  return tmp;
}
