#include <sstream>
#include "traceheaderexception.h"

string TraceHeaderException::moduleMessage( "Trace header: " );

const char *TraceHeaderException::errorMessage[] =
  {
    "Undefined error: ",
    "Invalid number of applications: ",
    "Invalid number of tasks: ",
    "Invalid number of threads: ",
    "Invalid number of node: ",
    NULL
  };

