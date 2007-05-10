// Application for testing paraver-kernel library code
#include <string>
#include <iostream>
#include "processmodel.h"
#include "paraverkerneltypes.h"
#include "traceheaderexception.h"
#include "paraverstatisticfuncions.h"

using namespace std;

int main( int argc, char *argv[] )
{
  // GOOD STRING
  string testStringProcessModel( "2:2(1:0,1:0):2(1:0,1:0)" );

  cout << "For process model " << testStringProcessModel << endl;

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

  // BAD STRING
  testStringProcessModel = "-1:2(1:0,1:0)";

  cout << endl << "For process model " << testStringProcessModel << endl;

  ProcessModel testBadProcessModel;

  try
  {
    testBadProcessModel = ProcessModel( testStringProcessModel );

    if ( testBadProcessModel.isReady() )
      cout << testBadProcessModel.totalApplications() << endl;
  }
  catch ( TraceHeaderException& ex )
  {
    ex.printMessage();
  }

}

