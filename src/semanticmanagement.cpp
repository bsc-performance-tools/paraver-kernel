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

#include "semanticmanagement.h"
#include "semanticcomposefunctions.h"
#include "semanticderivedfunctions.h"
#include "semanticcpufunctions.h"
#include "semanticnotthreadfunctions.h"
#include "semanticthreadfunctions.h"
#include "functionmanagement.h"

using namespace std;

void createSemantic()
{
  vector<string> groups;
  vector<string> names;
  vector<vector<SemanticFunction *> > functions;

  /**************************
  ** Compose functions
  ***************************/
  groups.push_back( "Compose" );
  functions.push_back( vector<SemanticFunction *>() );

  functions[0].push_back( new ComposeAsIs() );
  functions[0].push_back( new ComposeSign() );
  functions[0].push_back( new ComposeUnsign() );
  functions[0].push_back( new ComposeMod() );
  functions[0].push_back( new ComposeModPlus1() );
  functions[0].push_back( new ComposeDivide() );
  functions[0].push_back( new ComposeProduct() );
  functions[0].push_back( new ComposeAdding() );
  functions[0].push_back( new ComposeSubstract() );
  functions[0].push_back( new ComposeSelectRange() );
  functions[0].push_back( new ComposeSelectRangeOpen() );
  functions[0].push_back( new ComposeIsInRange() );
  functions[0].push_back( new ComposeIsInRangeOpen() );
  functions[0].push_back( new ComposeIsEqual() );
  functions[0].push_back( new ComposeIsEqualSign() );
  functions[0].push_back( new ComposeFloor() );
  functions[0].push_back( new ComposeCeil() );
  functions[0].push_back( new ComposeRound() );
  functions[0].push_back( new ComposeAbs() );
  functions[0].push_back( new ComposeStackedValue() );
  functions[0].push_back( new ComposeInStackedValue() );
  functions[0].push_back( new ComposeNestingLevel() );
  functions[0].push_back( new ComposeEnumerate() );
  functions[0].push_back( new ComposeDelta() );
  functions[0].push_back( new ComposeBurstTime() );
  functions[0].push_back( new ComposeBeginTime() );
  functions[0].push_back( new ComposeEndTime() );
  functions[0].push_back( new ComposeJoinBursts() );
  functions[0].push_back( new ComposeTranslate() );
  //functions[0].push_back( new ComposeCosine() );


  /**************************
  ** Derived functions
  ***************************/
  groups.push_back( "Derived" );
  functions.push_back( vector<SemanticFunction *>() );

  functions[1].push_back( new DerivedAdd() );
  functions[1].push_back( new DerivedProduct() );
  functions[1].push_back( new DerivedSubstract() );
  functions[1].push_back( new DerivedDivide() );
  functions[1].push_back( new DerivedMaximum() );
  functions[1].push_back( new DerivedMinimum() );
  functions[1].push_back( new DerivedDifferent() );
  functions[1].push_back( new ControlDerivedClearBy() );
  functions[1].push_back( new ControlDerivedMaximum() );
  functions[1].push_back( new ControlDerivedAdd() );


  /**************************
  ** CPU functions
  ***************************/
  groups.push_back( "CPU" );
  functions.push_back( vector<SemanticFunction *>() );

  functions[2].push_back( new ActiveThread() );
  functions[2].push_back( new ActiveThreadSign() );
  functions[2].push_back( new ActiveThreadValues() );
  functions[2].push_back( new ActiveThreadValuesSign() );



  /**************************
  ** Not Thread functions
  ***************************/
  groups.push_back( "Not thread" );
  functions.push_back( vector<SemanticFunction *>() );

  functions[3].push_back( new Adding() );
  functions[3].push_back( new AddingSign() );
  functions[3].push_back( new Average() );
  functions[3].push_back( new Maximum() );
  functions[3].push_back( new Minimum() );
  functions[3].push_back( new Activity() );
  functions[3].push_back( new InActivity() );
  functions[3].push_back( new NotThreadMode() );
  functions[3].push_back( new ObjectI() );
  functions[3].push_back( new AddObjectsI() );
  functions[3].push_back( new ChangedValue() );


  /**************************
  ** State functions (Thread)
  ***************************/
  groups.push_back( "State" );
  functions.push_back( vector<SemanticFunction *>() );

  functions[4].push_back( new StateAsIs() );
  functions[4].push_back( new Useful() );
  functions[4].push_back( new StateSign() );
  functions[4].push_back( new GivenState() );
  functions[4].push_back( new InState() );
  functions[4].push_back( new NotInState() );
  functions[4].push_back( new StateRecordDuration() );


  /**************************
  ** Event functions (Thread)
  ***************************/
  groups.push_back( "Event" );
  functions.push_back( vector<SemanticFunction *>() );

  functions[5].push_back( new LastEventType() );
  functions[5].push_back( new LastEventValue() );
  functions[5].push_back( new LastEventValueWOBursts() );
  functions[5].push_back( new NextEventType() );
  functions[5].push_back( new NextEventValue() );
  functions[5].push_back( new AverageNextEventValue() );
  functions[5].push_back( new AverageLastEventValue() );
  functions[5].push_back( new GivenEventValue() );
  functions[5].push_back( new InEventValue() );
  functions[5].push_back( new IntervalBetweenEvents() );
  functions[5].push_back( new NotInEventValue() );
  functions[5].push_back( new InEventRange() );
  functions[5].push_back( new EventBytes() );
  functions[5].push_back( new EventSentBytes() );


  /**************************
  ** Comm functions (Thread)
  ***************************/
  groups.push_back( "Communication" );
  functions.push_back( vector<SemanticFunction *>() );

  functions[6].push_back( new LastTag() );
  functions[6].push_back( new CommSize() );
  functions[6].push_back( new CommRecvPartner() );
  functions[6].push_back( new CommPartner() );
  functions[6].push_back( new LastSendDuration() );
  functions[6].push_back( new NextRecvDuration() );
  functions[6].push_back( new LastSendSize() );
  functions[6].push_back( new SendBytesInTransit() );
  functions[6].push_back( new SendMessagesInTransit() );
  functions[6].push_back( new SendBandWidth() );
  functions[6].push_back( new RecvBytesInTransit() );
  functions[6].push_back( new RecvMessagesInTransit() );
  functions[6].push_back( new RecvBandWidth() );
  functions[6].push_back( new RecvNegativeMessages() );
  functions[6].push_back( new RecvNegativeBytes() );
  functions[6].push_back( new NumberReceives() );
  functions[6].push_back( new NumberReceiveBytes() );


  /**************************
  ** Object functions (Thread)
  ***************************/
  groups.push_back( "Object" );
  functions.push_back( vector<SemanticFunction *>() );

  functions[7].push_back( new ApplicationID() );
  functions[7].push_back( new TaskID() );
  functions[7].push_back( new ThreadID() );
  functions[7].push_back( new NodeID() );
  functions[7].push_back( new CPUID() );
  functions[7].push_back( new InApplicationID() );
  functions[7].push_back( new InTaskID() );
  functions[7].push_back( new InThreadID() );
  functions[7].push_back( new InNodeID() );
  functions[7].push_back( new InCPUID() );


  for ( PRV_UINT16 iGroup = 0; iGroup < functions.size(); iGroup++ )
  {
    for ( PRV_UINT16 iFunction = 0; iFunction < functions[ iGroup ].size(); iFunction++ )
      names.push_back( functions[ iGroup ][ iFunction ]->getName() );
  }

  FunctionManagement<SemanticFunction>::getInstance( groups, names, functions );

}

