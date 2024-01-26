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

#include "derivedhistogrammanagement.h"

#include "functionmanagement.h"
#include "semanticderivedhistogramfunctions.h"

using namespace std;

class KHistogram;

void createDerivedHistogramFunctions()
{
  vector<string> groups;
  vector<string> names;
  vector<vector<SemanticFunction *> > functions;

  /*****************************
  ** Histogram Derived functions
  ******************************/
  groups.push_back( "Derived Histogram" );
  functions.push_back( vector<SemanticFunction *>() );

  functions[0].push_back( new DerivedHistogramAdd() );
  // functions[8].push_back( new DerivedProduct<KHistogram, DerivedHistogramFunctionInfo>() );
  // functions[8].push_back( new DerivedSubstract<KHistogram, DerivedHistogramFunctionInfo>() );
  // functions[8].push_back( new DerivedDivide<KHistogram, DerivedHistogramFunctionInfo>() );
  // functions[8].push_back( new DerivedMaximum(<KHistogram, DerivedHistogramFunctionInfo>) );
  // functions[8].push_back( new DerivedMinimum(<KHistogram, DerivedHistogramFunctionInfo>) );
  // functions[8].push_back( new DerivedDifferent<KHistogram, DerivedHistogramFunctionInfo>() );

  for ( PRV_UINT16 iGroup = 0; iGroup < functions.size(); iGroup++ )
  {
    for ( PRV_UINT16 iFunction = 0; iFunction < functions[ iGroup ].size(); iFunction++ )
      names.push_back( functions[ iGroup ][ iFunction ]->getName() );
  }

  FunctionManagement<SemanticFunction>::getInstance( groups, names, functions );
}
