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

#include "semanticderivedhistogramfunctions.h"

#include <algorithm>
#include <numeric>

using namespace std;

string DerivedHistogramAdd::name = "add";
string DerivedHistogramAdd::symbol = "+";
TSemanticValue DerivedHistogramAdd::execute( const SemanticInfo *info )
{
  const DerivedHistogramFunctionInfo *myInfo = ( const DerivedHistogramFunctionInfo * ) info;

  TSemanticValue tmp = std::accumulate( myInfo->values.cbegin(), myInfo->values.cend(), tmp );

  return tmp;
}


string DerivedHistogramProduct::name = "product";
string DerivedHistogramProduct::symbol = "x";
TSemanticValue DerivedHistogramProduct::execute( const SemanticInfo *info )
{
  const DerivedHistogramFunctionInfo *myInfo = ( const DerivedHistogramFunctionInfo * ) info;

  TSemanticValue tmp = std::accumulate( myInfo->values.cbegin(), myInfo->values.cend(), 1.0, std::multiplies<TSemanticValue>() );

  return tmp;
}


string DerivedHistogramSubstract::name = "substract";
string DerivedHistogramSubstract::symbol = "-";
TSemanticValue DerivedHistogramSubstract::execute( const SemanticInfo *info )
{
  const DerivedHistogramFunctionInfo *myInfo = ( const DerivedHistogramFunctionInfo * ) info;
  TSemanticValue tmp = myInfo->values[ 0 ];

  if ( myInfo->values.size() > 1 )
    tmp = std::accumulate( ++( myInfo->values.cbegin() ), myInfo->values.cend(), tmp, std::minus<TSemanticValue>() );

  return tmp;
}


string DerivedHistogramDivide::name = "divide";
string DerivedHistogramDivide::symbol = "/";
TSemanticValue DerivedHistogramDivide::execute( const SemanticInfo *info )
{
  const DerivedHistogramFunctionInfo *myInfo = ( const DerivedHistogramFunctionInfo * ) info;
  TSemanticValue tmp = myInfo->values[ 0 ];

  if ( myInfo->values.size() > 1 )
  {
    if( std::any_of( ++( myInfo->values.cbegin() ), myInfo->values.cend(), []( auto &value ){ return value == 0.0; } ) )
      return 0.0;

    tmp = std::accumulate( ++( myInfo->values.cbegin() ), myInfo->values.cend(), tmp, std::divides<TSemanticValue>() );
  }

  return tmp;
}


string DerivedHistogramMaximum::name = "maximum";
string DerivedHistogramMaximum::symbol = "_MAX_";
TSemanticValue DerivedHistogramMaximum::execute( const SemanticInfo *info )
{
  const DerivedHistogramFunctionInfo *myInfo = ( const DerivedHistogramFunctionInfo * ) info;

  TSemanticValue tmp = *( std::max_element( myInfo->values.cbegin(), myInfo->values.cend() ) );

  return tmp;
}


string DerivedHistogramMinimum::name = "minimum";
string DerivedHistogramMinimum::symbol = "_min_";
TSemanticValue DerivedHistogramMinimum::execute( const SemanticInfo *info )
{
  const DerivedHistogramFunctionInfo *myInfo = ( const DerivedHistogramFunctionInfo * ) info;

  TSemanticValue tmp = *( std::min_element( myInfo->values.cbegin(), myInfo->values.cend() ) );

  return tmp;
}


string DerivedHistogramDifferent::name = "different";
string DerivedHistogramDifferent::symbol = "!=";
TSemanticValue DerivedHistogramDifferent::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 1.0;
  const DerivedHistogramFunctionInfo *myInfo = ( const DerivedHistogramFunctionInfo * ) info;

  // tmp = myInfo->values[ 0 ] != myInfo->values[ 1 ] ?
  //       1 :
  //       0;
  if ( myInfo->values.size() > 1 )
  {
    if ( std::any_of( ++( myInfo->values.cbegin() ), myInfo->values.cend(), [&myInfo]( auto &value ){ return value == myInfo->values[ 0 ]; } ) )
      tmp = 0.0;
  }

  return tmp;
}
