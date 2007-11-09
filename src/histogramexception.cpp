#include <string>
#include "histogramexception.h"

string HistogramException::moduleMessage( "Histogram: " );

const char *HistogramException::errorMessage[] =
  {
    "Undefined error: ",
    "No control window selected: ",
    NULL
  };
