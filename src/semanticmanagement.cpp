#include "semanticmanagement.h"
#include "semanticcomposefunctions.h"
#include "semanticderivedfunctions.h"
#include "semanticcpufunctions.h"
#include "semanticnotthreadfunctions.h"
#include "semanticthreadfunctions.h"
#include "functionmanagement.h"


void createSemantic()
{
  vector<string> names;
  vector<SemanticFunction *> functions;


  /**************************
  ** Compose functions
  ***************************/

  functions.push_back( new ComposeAsIs() );
  functions.push_back( new ComposeSign() );
  functions.push_back( new ComposeUnsign() );
  functions.push_back( new ComposeMod() );
  functions.push_back( new ComposeModPlus1() );
  functions.push_back( new ComposeDivide() );
  functions.push_back( new ComposeProduct() );
  functions.push_back( new ComposeAdding() );
  functions.push_back( new ComposeSubstract() );
  functions.push_back( new ComposeSelectRange() );
  functions.push_back( new ComposeIsInRange() );
  functions.push_back( new ComposeIsEqual() );
  functions.push_back( new ComposeIsEqualSign() );
  functions.push_back( new ComposeStackedValue() );
  functions.push_back( new ComposeInStackedValue() );
  functions.push_back( new ComposeNestingLevel() );
  functions.push_back( new ComposeDelta() );
  functions.push_back( new ComposeBurstTime() );
  functions.push_back( new ComposeJoinBursts() );


  /**************************
  ** Derived functions
  ***************************/

  functions.push_back( new DerivedAdd() );
  functions.push_back( new DerivedProduct() );
  functions.push_back( new DerivedSubstract() );
  functions.push_back( new DerivedDivide() );
  functions.push_back( new DerivedMaximum() );
  functions.push_back( new DerivedMinimum() );
  functions.push_back( new ControlDerivedClearBy() );
  functions.push_back( new ControlDerivedMaximum() );
  functions.push_back( new ControlDerivedAdd() );


  /**************************
  ** CPU functions
  ***************************/

  functions.push_back( new ActiveThread() );
  functions.push_back( new ActiveThreadSign() );
  functions.push_back( new ActiveThreadValues() );
  functions.push_back( new ActiveThreadValuesSign() );



  /**************************
  ** Not Thread functions
  ***************************/

  functions.push_back( new Adding() );
  functions.push_back( new AddingSign() );
  functions.push_back( new Average() );
  functions.push_back( new Maximum() );
  functions.push_back( new Minimum() );
  functions.push_back( new Activity() );
  functions.push_back( new InActivity() );
  functions.push_back( new NotThreadMode() );
  functions.push_back( new ObjectI() );
  functions.push_back( new AddObjectsI() );


  /**************************
  ** State functions (Thread)
  ***************************/

  functions.push_back( new StateAsIs() );
  functions.push_back( new Useful() );
  functions.push_back( new StateSign() );
  functions.push_back( new GivenState() );
  functions.push_back( new InState() );
  functions.push_back( new NotInState() );
  functions.push_back( new StateRecordDuration() );


  /**************************
  ** Event functions (Thread)
  ***************************/

  functions.push_back( new LastEventType() );
  functions.push_back( new LastEventValue() );
  functions.push_back( new LastEventValueWOBursts() );
  functions.push_back( new NextEventType() );
  functions.push_back( new NextEventValue() );
  functions.push_back( new AverageNextEventValue() );
  functions.push_back( new AverageLastEventValue() );
  functions.push_back( new GivenEventValue() );
  functions.push_back( new InEventValue() );
  functions.push_back( new IntervalBetweenEvents() );
  functions.push_back( new NotInEventValue() );
  functions.push_back( new InEventRange() );
  functions.push_back( new EventBytes() );


  /**************************
  ** Comm functions (Thread)
  ***************************/

  functions.push_back( new LastTag() );
  functions.push_back( new CommSize() );
  functions.push_back( new CommRecvPartner() );
  functions.push_back( new CommPartner() );
  functions.push_back( new LastSendDuration() );
  functions.push_back( new NextRecvDuration() );
  functions.push_back( new SendBytesInTransit() );
  functions.push_back( new SendMessagesInTransit() );
  functions.push_back( new SendBandWidth() );
  functions.push_back( new RecvBytesInTransit() );
  functions.push_back( new RecvMessagesInTransit() );
  functions.push_back( new RecvBandWidth() );
  functions.push_back( new RecvNegativeMessages() );
  functions.push_back( new RecvNegativeBytes() );
  functions.push_back( new NumberReceives() );
  functions.push_back( new NumberReceiveBytes() );


  /**************************
  ** Object functions (Thread)
  ***************************/

  functions.push_back( new ApplicationID() );
  functions.push_back( new TaskID() );
  functions.push_back( new ThreadID() );
  functions.push_back( new NodeID() );
  functions.push_back( new CPUID() );
  functions.push_back( new InApplicationID() );
  functions.push_back( new InTaskID() );
  functions.push_back( new InThreadID() );
  functions.push_back( new InNodeID() );
  functions.push_back( new InCPUID() );



  for ( UINT16 i = 0; i < functions.size(); i++ )
    names.push_back( functions[i]->getName() );


  FunctionManagement<SemanticFunction>::getInstance( names, functions );

}

