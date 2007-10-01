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
    TagFunction()
    {}
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
    WindowType()
    {}
    virtual ~WindowType()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowFactors: public TagFunction
{
  public:
    WindowFactors()
    {}
    virtual ~WindowFactors()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowUnits: public TagFunction
{
  public:
    WindowUnits()
    {}
    virtual ~WindowUnits()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowOperation: public TagFunction
{
  public:
    WindowOperation()
    {}
    virtual ~WindowOperation()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowMaximumY: public TagFunction
{
  public:
    WindowMaximumY()
    {}
    virtual ~WindowMaximumY()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowLevel: public TagFunction
{
  public:
    WindowLevel()
    {}
    virtual ~WindowLevel()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowScaleRelative: public TagFunction
{
  public:
    WindowScaleRelative()
    {}
    virtual ~WindowScaleRelative()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowObject: public TagFunction
{
  public:
    WindowObject()
    {}
    virtual ~WindowObject()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowBeginTimeRelative: public TagFunction
{
  public:
    WindowBeginTimeRelative()
    {}
    virtual ~WindowBeginTimeRelative()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowNumberOfRow: public TagFunction
{
  public:
    WindowNumberOfRow()
    {}
    virtual ~WindowNumberOfRow()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};

class WindowSelectedFunctions: public TagFunction
{
  public:
    WindowSelectedFunctions()
    {}
    virtual ~WindowSelectedFunctions()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowSemanticModule: public TagFunction
{
  public:
    WindowSemanticModule()
    {}
    virtual ~WindowSemanticModule()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowFilterModule: public TagFunction
{
  public:
    WindowFilterModule()
    {}
    virtual ~WindowFilterModule()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};

#endif // CFG_H_INCLUDED
