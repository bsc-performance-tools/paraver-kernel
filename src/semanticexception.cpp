#include "semanticexception.h"

string SemanticException::moduleMessage( "Trace header: " );

const char *SemanticException::errorMessage[] =
  {
    "Undefined error: ",
    "Maximum parameter exceeded: ",
    NULL
  };
