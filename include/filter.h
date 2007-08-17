#ifndef _FILTER_H
#define _FILTER_H

#include "memorytrace.h"


class Filter
{
  public:
    bool passFilter( MemoryTrace::iterator *it );

  private:
    vector<FilterEvents> eventsFilter;
    vector<FilterComms>  commsFilter;
}


#endif // _FILTER_H_INCLUDED
