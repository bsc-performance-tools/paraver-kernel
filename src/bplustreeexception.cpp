#include <string>
#include "bplustreeexception.h"

string BPlusTreeException::moduleMessage( "BPlusTree: " );

const char *BPlusTreeException::errorMessage[] =
  {
    "Undefined error: ",
    "Invalid append: ",
    NULL
  };

