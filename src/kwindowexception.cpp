#include <string>
#include "kwindowexception.h"

string KWindowException::moduleMessage( "Kernel window: " );

const char *KWindowException::errorMessage[] =
  {
    "Undefined error: ",
    "Invalid level for kernel window: ",
    NULL
  };

