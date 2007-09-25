#include <iostream>
#include <string>
#include "paramedir.h"
#include "cfg.h"
#include "trace.h"
#include "kwindow.h"

using namespace std;

bool multipleFiles = false;
Trace *trace;

int main( int argc, char *argv[] )
{
  // argv[0] nombre del ejecutable
  if ( argc == 1 )
    printHelp();
  else
  {
    INT32 currentArg = 1;
    // Read options ( Deberia ser un while si hay mas opciones. De momento solo hay esta)
    if( argv[ currentArg ][ 0 ] == '-' )
    {
      if( argv[ currentArg ][ 1 ] == 'm' )
      {
        multipleFiles = true;
      }
      currentArg++;
    }

    // Read the trace
    string strTrace( argv[ currentArg ] );
    trace = new Trace( strTrace );
    currentArg++;

    //Read the cfgs list and create the output files for each one
    while( currentArg < argc )
    {
      string strCfg( argv[ currentArg ] );
      vector<KWindow *> windows;
      TRecordTime beginTime;
      TRecordTime endTime;


      if( loadCFG( strCfg, trace, windows, beginTime, endTime ) )
      {
        windows[ windows.size() - 1 ]->init( beginTime, NOCREATE );

        // Make the output file
      }
      else
        cout << "Cannot load '" << strCfg << "' file." << endl;

      for( UINT32 i = 0; i < windows.size(); i++ )
      {
        if( windows[ i ] != NULL )
          delete windows[ i ];
      }
      windows.clear();

      currentArg++;
    }
  }

  return 1;
}

void printHelp()
{
  cout << "USAGE" << endl;
  cout << "  paramedir [OPTION] trc cfgs" << endl;
  cout << endl;
  cout << "  Options:" << endl;
  cout << "  -m: Prints on multiple files." << endl;
  cout << endl;
  cout << "  Parameters:" << endl;
  cout << "  trc: Paraver trace filename (ASCII: *.prv)." << endl;
  cout << "  cfgs: Paraver configuration filename list (*.cfg)." << endl;
}
