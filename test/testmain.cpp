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

using namespace std;
using namespace BPlusTree;

int main( int argc, char *argv[] )
{
  MemoryBlocks *b = new BPlusTreeBlocks();
  b->resetCountInserted();
  delete b;
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
  Trace testTrace;
  try
  {
    testTrace = Trace( "/home/eloy/traces/mpi_ping.prv" );
  }
  catch ( TraceHeaderException& ex )
  {
    ex.printMessage();
  }
  catch ( ParaverKernelException& ex )
  {
    ex.printMessage();
  }


  //--------------------------------------------------------------------------
  // END TESTING Trace
  //--------------------------------------------------------------------------

}

