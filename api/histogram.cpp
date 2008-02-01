#include "histogram.h"

Histogram *Histogram::create()
{
  return new HistogramProxy();
}
