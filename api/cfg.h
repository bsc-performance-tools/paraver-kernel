#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include <map>
#include <vector>
#include <sstream>
#include "paraverkerneltypes.h"

class KWindow;
class Trace;

using namespace std;

class TagFunction
{
  public:
    virtual ~TagFunction()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime ) = 0;
};

class CFGLoader
{
  private:
    static bool mapLoaded;
    static map<string, TagFunction *> cfgTagFunctions;
    static void loadMap();
  public:
    static bool loadCFG( string& filename, Trace *whichTrace, vector<KWindow *>& windows,
                         TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowType: public TagFunction
{
  public:
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


#endif // CFG_H_INCLUDED
