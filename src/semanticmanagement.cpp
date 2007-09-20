#include "semanticmanagement.h"
#include "semanticcomposefunctions.h"
#include "semanticderivedfunctions.h"
#include "semanticcpufunctions.h"
#include "semanticnotthreadfunctions.h"
#include "semanticthreadfunctions.h"

SemanticFunction *createFunction( const string& functionName )
{
  SemanticFunction *tmpFunc = NULL;

  /**************************
  ** Compose functions
  ***************************/
  if ( !functionName.compare( "As Is" ) )
    tmpFunc = new ComposeAsIs();
  else if ( !functionName.compare( "Sign" ) )
    tmpFunc = new ComposeSign();
  else if ( !functionName.compare( "1-Sign" ) )
    tmpFunc = new ComposeUnsign();
  else if ( !functionName.compare( "Mod" ) )
    tmpFunc = new ComposeMod();
  else if ( !functionName.compare( "Mod+1" ) )
    tmpFunc = new ComposeModPlus1();
  else if ( !functionName.compare( "Div" ) )
    tmpFunc = new ComposeDivide();
  else if ( !functionName.compare( "Prod" ) )
    tmpFunc = new ComposeProduct();
  else if ( !functionName.compare( "Add" ) )
    tmpFunc = new ComposeAdding();
  else if ( !functionName.compare( "Subs" ) )
    tmpFunc = new ComposeSubstract();
  else if ( !functionName.compare( "Select Range" ) )
    tmpFunc = new ComposeSelectRange();
  else if ( !functionName.compare( "Is In Range" ) )
    tmpFunc = new ComposeIsInRange();
  else if ( !functionName.compare( "Is Equal" ) )
    tmpFunc = new ComposeIsEqual();
  else if ( !functionName.compare( "Is Equal (Sign)" ) )
    tmpFunc = new ComposeIsEqualSign();
  else if ( !functionName.compare( "Stacked Val" ) )
    tmpFunc = new ComposeStackedValue();
  else if ( !functionName.compare( "In Stacked Val" ) )
    tmpFunc = new ComposeInStackedValue();
  else if ( !functionName.compare( "Nesting level" ) )
    tmpFunc = new ComposeNestingLevel();
  else if ( !functionName.compare( "Delta" ) )
    tmpFunc = new ComposeDelta();
  else if ( !functionName.compare( "Burst Time" ) )
    tmpFunc = new ComposeBurstTime();
  else if ( !functionName.compare( "Join Bursts" ) )
    tmpFunc = new ComposeJoinBursts();


  /**************************
  ** Derived functions
  ***************************/
  else if ( !functionName.compare( "add" ) )
    tmpFunc = new DerivedAdd();
  else if ( !functionName.compare( "product" ) )
    tmpFunc = new DerivedProduct();
  else if ( !functionName.compare( "substract" ) )
    tmpFunc = new DerivedSubstract();
  else if ( !functionName.compare( "divide" ) )
    tmpFunc = new DerivedDivide();
  else if ( !functionName.compare( "maximum" ) )
    tmpFunc = new DerivedMaximum();
  else if ( !functionName.compare( "minimum" ) )
    tmpFunc = new DerivedMinimum();
  else if ( !functionName.compare( "controlled: clear by" ) )
    tmpFunc = new ControlDerivedClearBy();
  else if ( !functionName.compare( "controlled: maximum" ) )
    tmpFunc = new ControlDerivedMaximum();
  else if ( !functionName.compare( "controlled: add" ) )
    tmpFunc = new ControlDerivedAdd();


  /**************************
  ** CPU functions
  ***************************/
  else if ( !functionName.compare( "Active Thd" ) )
    tmpFunc = new ActiveThread();
  else if ( !functionName.compare( "Active Thd Sign" ) )
    tmpFunc = new ActiveThreadSign();
  else if ( !functionName.compare( "Active Thd Val" ) )
    tmpFunc = new ActiveThreadValues();
  else if ( !functionName.compare( "Active Thd Val Sign" ) )
    tmpFunc = new ActiveThreadValuesSign();


  /**************************
  ** Not Thread functions
  ***************************/
  else if ( !functionName.compare( "Adding" ) )
    tmpFunc = new Adding();
  else if ( !functionName.compare( "Adding Sign" ) )
    tmpFunc = new AddingSign();
  else if ( !functionName.compare( "Average" ) )
    tmpFunc = new Average();
  else if ( !functionName.compare( "Maximum" ) )
    tmpFunc = new Maximum();
  else if ( !functionName.compare( "Minimum" ) )
    tmpFunc = new Minimum();
  else if ( !functionName.compare( "Activity" ) )
    tmpFunc = new Activity();
  else if ( !functionName.compare( "In Activity" ) )
    tmpFunc = new InActivity();
  else if ( !functionName.compare( "Mode" ) )
    tmpFunc = new NotThreadMode();
  else if ( !functionName.compare( "Thread i" ) )
    tmpFunc = new ObjectI();
  else if ( !functionName.compare( "Add Tasks" ) )
    tmpFunc = new AddObjectsI();


  /**************************
  ** State functions (Thread)
  ***************************/
  else if ( !functionName.compare( "State As Is" ) )
    tmpFunc = new StateAsIs();
  else if ( !functionName.compare( "Useful" ) )
    tmpFunc = new Useful();
  else if ( !functionName.compare( "State Sign" ) )
    tmpFunc = new StateSign();
  else if ( !functionName.compare( "Given State" ) )
    tmpFunc = new GivenState();
  else if ( !functionName.compare( "In State" ) )
    tmpFunc = new InState();
  else if ( !functionName.compare( "Not In State" ) )
    tmpFunc = new NotInState();
  else if ( !functionName.compare( "State Record Dur." ) )
    tmpFunc = new StateRecordDuration();


  /**************************
  ** Event functions (Thread)
  ***************************/
  else if ( !functionName.compare( "Last Evt Type" ) )
    tmpFunc = new LastEventType();
  else if ( !functionName.compare( "Last Evt Val" ) )
    tmpFunc = new LastEventValue();
  else if ( !functionName.compare( "Last Evt Val w/o Bursts" ) )
    tmpFunc = new LastEventValueWOBursts();
  else if ( !functionName.compare( "Next Evt Type" ) )
    tmpFunc = new NextEventType();
  else if ( !functionName.compare( "Next Evt Val" ) )
    tmpFunc = new NextEventValue();
  else if ( !functionName.compare( "Avg Next Evt Val" ) )
    tmpFunc = new AverageNextEventValue();
  else if ( !functionName.compare( "Avg Last Evt Val" ) )
    tmpFunc = new AverageLastEventValue();
  else if ( !functionName.compare( "Given Evt Val" ) )
    tmpFunc = new GivenEventValue();
  else if ( !functionName.compare( "In Evt Val" ) )
    tmpFunc = new InEventValue();
  else if ( !functionName.compare( "Int. Between Evt" ) )
    tmpFunc = new IntervalBetweenEvents();
  else if ( !functionName.compare( "Not In Evt Val" ) )
    tmpFunc = new NotInEventValue();
  else if ( !functionName.compare( "In Evt Range" ) )
    tmpFunc = new InEventRange();
  else if ( !functionName.compare( "Event Bytes" ) )
    tmpFunc = new EventBytes();


  /**************************
  ** Comm functions (Thread)
  ***************************/
  else if ( !functionName.compare( "Last Tag" ) )
    tmpFunc = new LastTag();
  else if ( !functionName.compare( "Comm Size" ) )
    tmpFunc = new CommSize();
  else if ( !functionName.compare( "Comm Recv. Partner" ) )
    tmpFunc = new CommRecvPartner();
  else if ( !functionName.compare( "Comm Partner" ) )
    tmpFunc = new CommPartner();
  else if ( !functionName.compare( "Last Send Dur." ) )
    tmpFunc = new LastSendDuration();
  else if ( !functionName.compare( "Next Recv Dur." ) )
    tmpFunc = new NextRecvDuration();
  else if ( !functionName.compare( "Send Bytes in Transit" ) )
    tmpFunc = new SendBytesInTransit();
  else if ( !functionName.compare( "Send Messages in Transit" ) )
    tmpFunc = new SendMessagesInTransit();
  else if ( !functionName.compare( "Send BandWidth" ) )
    tmpFunc = new SendBandWidth();
  else if ( !functionName.compare( "Recv Bytes in Transit" ) )
    tmpFunc = new RecvBytesInTransit();
  else if ( !functionName.compare( "Recv Messages in Transit" ) )
    tmpFunc = new RecvMessagesInTransit();
  else if ( !functionName.compare( "Recv BandWidth" ) )
    tmpFunc = new RecvBandWidth();
  else if ( !functionName.compare( "Recv Negative Messages" ) )
    tmpFunc = new RecvNegativeMessages();
  else if ( !functionName.compare( "Recv Negative Bytes" ) )
    tmpFunc = new RecvNegativeBytes();
  else if ( !functionName.compare( "Number Of Receives" ) )
    tmpFunc = new NumberReceives();
  else if ( !functionName.compare( "Number Of Receive Bytes" ) )
    tmpFunc = new NumberReceiveBytes();


  /**************************
  ** Object functions (Thread)
  ***************************/
  else if ( !functionName.compare( "Application ID" ) )
    tmpFunc = new ApplicationID();
  else if ( !functionName.compare( "Task ID" ) )
    tmpFunc = new TaskID();
  else if ( !functionName.compare( "Thread ID" ) )
    tmpFunc = new ThreadID();
  else if ( !functionName.compare( "Node ID" ) )
    tmpFunc = new NodeID();
  else if ( !functionName.compare( "Cpu ID" ) )
    tmpFunc = new CPUID();
  else if ( !functionName.compare( "In Appl ID" ) )
    tmpFunc = new InApplicationID();
  else if ( !functionName.compare( "In Task ID" ) )
    tmpFunc = new InTaskID();
  else if ( !functionName.compare( "In Thread ID" ) )
    tmpFunc = new InThreadID();
  else if ( !functionName.compare( "In Node ID" ) )
    tmpFunc = new InNodeID();
  else if ( !functionName.compare( "In Cpu ID" ) )
    tmpFunc = new InCPUID();


  return tmpFunc;
}
