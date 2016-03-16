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

#include "histogramstatistic.h"
#include "statisticmanagement.h"
#include "functionmanagement.h"

using namespace std;

void createStatistic()
{
  vector<string> groups;
  vector<string> names;
  vector<vector<HistogramStatistic *> > functions;

  // Communication statistics
  groups.push_back( "Communication" );
  functions.push_back( vector<HistogramStatistic *>() );

  functions[0].push_back( new StatNumSends() );
  functions[0].push_back( new StatNumReceives() );
  functions[0].push_back( new StatBytesSent() );
  functions[0].push_back( new StatBytesReceived() );
  functions[0].push_back( new StatAvgBytesSent() );
  functions[0].push_back( new StatAvgBytesReceived() );
  functions[0].push_back( new StatMinBytesSent() );
  functions[0].push_back( new StatMinBytesReceived() );
  functions[0].push_back( new StatMaxBytesSent() );
  functions[0].push_back( new StatMaxBytesReceived() );

  // Semantic statistics
  groups.push_back( "Semantic" );
  functions.push_back( vector<HistogramStatistic *>() );

  functions[1].push_back( new StatTime() );
  functions[1].push_back( new StatPercTime() );
  functions[1].push_back( new StatPercTimeNotZero() );
  functions[1].push_back( new StatPercTimeWindow() );
  functions[1].push_back( new StatNumBursts() );
  functions[1].push_back( new StatPercNumBursts() );
  functions[1].push_back( new StatIntegral() );
  functions[1].push_back( new StatAvgValue() );
  functions[1].push_back( new StatMaximum() );
  functions[1].push_back( new StatAvgBurstTime() );
  functions[1].push_back( new StatStdevBurstTime() );
  functions[1].push_back( new StatAvgPerBurst() );
  functions[1].push_back( new StatAvgValueNotZero() );
  functions[1].push_back( new StatAvgPerBurstNotZero() );
  functions[1].push_back( new StatNumBurstsNotZero() );
  functions[1].push_back( new StatSumBursts() );

  for ( PRV_UINT16 iGroup = 0; iGroup < functions.size(); iGroup++ )
  {
    for ( PRV_UINT16 iFunction = 0; iFunction < functions[ iGroup ].size(); iFunction++ )
      names.push_back( functions[ iGroup ][ iFunction ]->getName() );
  }

  FunctionManagement<HistogramStatistic>::getInstance( groups, names, functions );
}
