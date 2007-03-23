// Application for testing paraver-kernel library code
#include "processmodel.h"
#include "paraverkerneltypes.h"
#include "paraverkernelexception.h"
#include <string>
#include <iostream>

using namespace std;

int main( int argc, char *argv[] )
{
  // String representing an application with two task, and one thread per task
  //---------------------------------------------------------------------------

  // GOOD STRING
  string testStringProcessModel( "1:2(1:0,1:0)" );

  ProcessModel testProcessModel( testStringProcessModel );

  if ( testProcessModel.isReady() )
    cout << testProcessModel.totalApplications() << endl;
  else
    cerr << "Error reading header" << endl;

  // BAD STRING
  testStringProcessModel = "-1:2(1:0,1:0)";

  ProcessModel testBadProcessModel;

  try
  {
    testBadProcessModel = ProcessModel( testStringProcessModel );
  }
  catch ( std::string& ex )
  {
    cout << ex << endl;
  }

  if ( testBadProcessModel.isReady() )
    cout << testBadProcessModel.totalApplications() << endl;
  else
    cerr << "Error reading header" << endl;

}

