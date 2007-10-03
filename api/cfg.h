#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include <map>
#include <vector>
#include <sstream>
#include "paraverkerneltypes.h"

class KWindow;
class Trace;

using namespace std;

TWindowLevel stringToLevel( const string& strLevel );

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


class WindowBeginTime: public TagFunction
{
  public:
    WindowBeginTime()
    {}
    virtual ~WindowBeginTime()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowEndTime: public TagFunction
{
  public:
    WindowEndTime()
    {}
    virtual ~WindowEndTime()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowStopTime: public TagFunction
{
  public:
    WindowStopTime()
    {}
    virtual ~WindowStopTime()
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


class WindowComposeFunctions: public TagFunction
{
  public:
    WindowComposeFunctions()
    {}
    virtual ~WindowComposeFunctions()
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


class WindowFilterLogical: public TagFunction
{
  public:
    WindowFilterLogical()
    {}
    virtual ~WindowFilterLogical()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowFilterPhysical: public TagFunction
{
  public:
    WindowFilterPhysical()
    {}
    virtual ~WindowFilterPhysical()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowFilterBoolOpFromTo: public TagFunction
{
  public:
    WindowFilterBoolOpFromTo()
    {}
    virtual ~WindowFilterBoolOpFromTo()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowFilterBoolOpTagSize: public TagFunction
{
  public:
    WindowFilterBoolOpTagSize()
    {}
    virtual ~WindowFilterBoolOpTagSize()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowFilterBoolOpTypeVal: public TagFunction
{
  public:
    WindowFilterBoolOpTypeVal()
    {}
    virtual ~WindowFilterBoolOpTypeVal()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


#endif // CFG_H_INCLUDED
