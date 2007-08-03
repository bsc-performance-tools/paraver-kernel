// Application for testing paraver-kernel library code
#include <string>
#include <iostream>
#include <sstream>
#include "paraverkerneltypes.h"
#include "traceheaderexception.h"
#include "paraverstatisticfuncions.h"
#include "processmodel.h"
#include "resourcemodel.h"
#include "trace.h"
#include "memorytrace.h"
#include "bplustreetypes.h"
#include "memoryblocks.h"
#include "tracebodyio.h"
#include "tracebodyio_v1.h"
#include "bplustreeblocks.h"
#include "interval.h"

using namespace std;
using namespace bplustree;

int main( int argc, char *argv[] )
{

  //--------------------------------------------------------------------------
  // TESTING ProcessModel
  //--------------------------------------------------------------------------
  // GOOD STRING
  istringstream testStringProcessModel( "2:2(1:0,1:0):2(1:0,1:0)" );

  cout << "For process model " << testStringProcessModel.str() << endl;

  ProcessModel testProcessModel;

  try
  {
    testProcessModel = ProcessModel( testStringProcessModel );

    if ( testProcessModel.isReady() )
    {
      cout << "Appls: " << testProcessModel.totalApplications() << endl;
      cout << "Tasks: " << testProcessModel.totalTasks() << endl;
      cout << "Threads: " << testProcessModel.totalThreads() << endl;
    }
  }
  catch ( TraceHeaderException& ex )
  {
    ex.printMessage();
  }
  MemoryBlocks *b = new BPlusTreeBlocks( testProcessModel );
  b->resetCountInserted();
  delete b;

  cout << endl;

  // BAD STRING
  istringstream testStringProcessModel2( "-1:2(1:0,1:0)" );

  cout << "For process model " << testStringProcessModel2.str() << endl;

  ProcessModel testBadProcessModel;

  try
  {
    testBadProcessModel = ProcessModel( testStringProcessModel2 );

    if ( testBadProcessModel.isReady() )
      cout << testBadProcessModel.totalApplications() << endl;
  }
  catch ( TraceHeaderException& ex )
  {
    ex.printMessage();
  }
  //--------------------------------------------------------------------------
  // END TESTING ProcessModel
  //--------------------------------------------------------------------------

  cout << endl;
  //--------------------------------------------------------------------------
  // TESTING ResourceModel
  //--------------------------------------------------------------------------
  // GOOD STRING
  istringstream testStringResourceModel( "2(4,4)" );

  cout << "For resource model " << testStringResourceModel.str() << endl;

  ResourceModel testResourceModel;

  try
  {
    testResourceModel = ResourceModel( testStringResourceModel );

    if ( testResourceModel.isReady() )
    {
      cout << "Nodes: " << testResourceModel.totalNodes() << endl;
      cout << "CPUs: " << testResourceModel.totalCPUs() << endl;
    }
  }
  catch ( TraceHeaderException& ex )
  {
    ex.printMessage();
  }

  //--------------------------------------------------------------------------
  // END TESTING ResourceModel
  //--------------------------------------------------------------------------

  cout << endl;

  //--------------------------------------------------------------------------
  // TESTING Trace
  //--------------------------------------------------------------------------
  Trace *testTrace;
  try
  {
//    testTrace = Trace( "/home/eloy/traces/sweep3d.150.100.chop1.prv" );
    testTrace = new Trace( "/home/eloy/traces/mpi_ping.prv" );
//    testTrace = new Trace( "/home/pedro/tools/trazas/ping-pong/ping_pong.prv" );
  }
  catch ( TraceHeaderException& ex )
  {
    ex.printMessage();
  }
  catch ( ParaverKernelException& ex )
  {
    ex.printMessage();
  }

  TApplOrder ap;
  TTaskOrder tk;
  TThreadOrder th;

  testTrace->getThreadLocation( 9000, ap, tk, th );
  cout << ap << " " << tk << " " << th << endl;

  testTrace->dumpFile("/home/eloy/ping_pong_dump.prv");

  //--------------------------------------------------------------------------
  // END TESTING Trace
  //--------------------------------------------------------------------------
  cin.get();
}

