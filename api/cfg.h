#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include <map>
#include "kwindow.h"
#include "trace.h"

class TagFunction
{
  public:
    virtual ~TagFunction()
    {}
    virtual bool read( istringstream& line, Trace *whichTrace, KWindow **window,
                       TRecordTime& beginTime, TRecordTime& endTime ) = 0;
};

bool mapLoaded = false;
map<string, TagFunction *> cfgTagFunctions;

bool loadCFG( string& filename, Trace *whichTrace, vector<KWindow *>& windows,
              TRecordTime& beginTime, TRecordTime& endTime );

void loadMap();

#endif // CFG_H_INCLUDED
