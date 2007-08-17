#ifndef SEMANTICINFO_H_INCLUDED
#define SEMANTICINFO_H_INCLUDED

#include <vector>
#include "paraverkerneltypes.h"
#include "interval.h"
#include "memorytrace.h"

using namespace std;

struct SemanticInfo
{
  Interval *callingInterval;
};


struct SemanticThreadInfo: public SemanticInfo
{
  MemoryTrace::iterator *it;
};


struct SemanticHighInfo: public SemanticInfo
{
  vector<TSemanticValue> values;
};

#endif // SEMANTICINFO_H_INCLUDED
