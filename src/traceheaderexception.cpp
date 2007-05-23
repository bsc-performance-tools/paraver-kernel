#include <string>
#include "traceheaderexception.h"

string TraceHeaderException::moduleMessage( "Trace header: " );

const char *TraceHeaderException::errorMessage[] =
  {
    "Undefined error: ",
    "Invalid number of applications: ",
    "Invalid number of tasks: ",
    "Invalid number of threads: ",
    "Invalid number of node: ",
    "Invalid number of CPU: ",
    "Invalid time: ",
    "Invalid number of communicators: ",
    "Unknown communicator line format: ",
    NULL
  };

