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

using namespace std;

string DerivedHistogramAdd::name = "add"; // TODO: collision with semanticderived add?
//TSemanticValue DerivedHistogramAdd::execute( const DerivedHistogramFunctionInfo *info )
TSemanticValue DerivedHistogramAdd::execute( const SemanticInfo *info )
{
  TSemanticValue tmp = 0;
  const DerivedHistogramFunctionInfo *myInfo = ( const DerivedHistogramFunctionInfo * ) info;

  tmp = myInfo->values[ 0 ] + myInfo->values[ 1 ];

  return tmp;
}


// string DerivedHistogramProduct::name = "product";
// TSemanticValue DerivedHistogramProduct::execute( const SemanticInfo *info )
// {
//   TSemanticValue tmp = 0;
//   const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

//   tmp = myInfo->values[ 0 ] * myInfo->values[ 1 ];

//   return tmp;
// }


// string DerivedHistogramSubstract::name = "substract";
// TSemanticValue DerivedHistogramSubstract::execute( const SemanticInfo *info )
// {
//   TSemanticValue tmp = 0;
//   const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

//   tmp = myInfo->values[ 0 ] - myInfo->values[ 1 ];

//   return tmp;
// }


// string DerivedHistogramDivide::name = "divide";
// TSemanticValue DerivedHistogramDivide::execute( const SemanticInfo *info )
// {
//   TSemanticValue tmp = 0.0;
//   const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

//   if( myInfo->values[ 1 ] == 0 )
//     return 0.0;

//   tmp = myInfo->values[ 0 ] / myInfo->values[ 1 ];

//   return tmp;
// }


// string DerivedHistogramMaximum::name = "maximum";
// TSemanticValue DerivedHistogramMaximum::execute( const SemanticInfo *info )
// {
//   TSemanticValue tmp = 0;
//   const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

//   tmp = myInfo->values[ 0 ] > myInfo->values[ 1 ] ?
//         myInfo->values[ 0 ] :
//         myInfo->values[ 1 ];

//   return tmp;
// }


// string DerivedHistogramMinimum::name = "minimum";
// TSemanticValue DerivedHistogramMinimum::execute( const SemanticInfo *info )
// {
//   TSemanticValue tmp = 0;
//   const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

//   tmp = myInfo->values[ 0 ] < myInfo->values[ 1 ] ?
//         myInfo->values[ 0 ] :
//         myInfo->values[ 1 ];

//   return tmp;
// }


// string DerivedHistogramDifferent::name = "different";
// TSemanticValue DerivedHistogramDifferent::execute( const SemanticInfo *info )
// {
//   TSemanticValue tmp = 0;
//   const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

//   tmp = myInfo->values[ 0 ] != myInfo->values[ 1 ] ?
//         1 :
//         0;

//   return tmp;
// }
