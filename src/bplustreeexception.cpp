#include <string>
#include "bplustreeexception.h"

using namespace bplustree;

string BPlusTreeException::moduleMessage( "BPlusTree: " );

const char *BPlusTreeException::errorMessage[] =
  {
    "Undefined error: ",
    "Invalid append: ",
    "Wrong Iterator: ",
    "Invalid cast: ",
    NULL
  };

