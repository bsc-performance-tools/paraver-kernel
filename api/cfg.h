#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include <map>
#include <vector>
#include <sstream>
#include "paraverkerneltypes.h"

class KWindow;
class Trace;
class Histogram;

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
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime ) = 0;
};

class CFGLoader
{
  private:
    static map<string, TagFunction *> cfgTagFunctions;
    static void loadMap();
    static void unLoadMap();

  public:
    static bool isCFGFile( const string& filename );
    static bool loadCFG( string& filename, Trace *whichTrace,
                         vector<KWindow *>& windows,
                         Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
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
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class WindowIdentifiers: public TagFunction
{
  public:
    WindowIdentifiers()
    {}
    virtual ~WindowIdentifiers()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DCreate: public TagFunction
{
  public:
    Analyzer2DCreate()
    {}
    virtual ~Analyzer2DCreate()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DControlWindow: public TagFunction
{
  public:
    Analyzer2DControlWindow()
    {}
    virtual ~Analyzer2DControlWindow()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DDataWindow: public TagFunction
{
  public:
    Analyzer2DDataWindow()
    {}
    virtual ~Analyzer2DDataWindow()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DStatistic: public TagFunction
{
  public:
    Analyzer2DStatistic()
    {}
    virtual ~Analyzer2DStatistic()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DCalculateAll: public TagFunction
{
  public:
    Analyzer2DCalculateAll()
    {}
    virtual ~Analyzer2DCalculateAll()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DNumColumns: public TagFunction
{
  public:
    Analyzer2DNumColumns()
    {}
    virtual ~Analyzer2DNumColumns()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DScientificNotation: public TagFunction
{
  public:
    Analyzer2DScientificNotation()
    {}
    virtual ~Analyzer2DScientificNotation()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DNumDecimals: public TagFunction
{
  public:
    Analyzer2DNumDecimals()
    {}
    virtual ~Analyzer2DNumDecimals()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DThousandSeparator: public TagFunction
{
  public:
    Analyzer2DThousandSeparator()
    {}
    virtual ~Analyzer2DThousandSeparator()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DUnits: public TagFunction
{
  public:
    Analyzer2DUnits()
    {}
    virtual ~Analyzer2DUnits()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DAccumulator: public TagFunction
{
  public:
    Analyzer2DAccumulator()
    {}
    virtual ~Analyzer2DAccumulator()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DHorizontal: public TagFunction
{
  public:
    Analyzer2DHorizontal()
    {}
    virtual ~Analyzer2DHorizontal()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DAccumulateByControlWindow: public TagFunction
{
  public:
    Analyzer2DAccumulateByControlWindow()
    {}
    virtual ~Analyzer2DAccumulateByControlWindow()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DSortCols: public TagFunction
{
  public:
    Analyzer2DSortCols()
    {}
    virtual ~Analyzer2DSortCols()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DSortCriteria: public TagFunction
{
  public:
    Analyzer2DSortCriteria()
    {}
    virtual ~Analyzer2DSortCriteria()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DParameters: public TagFunction
{
  public:
    Analyzer2DParameters()
    {}
    virtual ~Analyzer2DParameters()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DAnalysisLimits: public TagFunction
{
  public:
    Analyzer2DAnalysisLimits()
    {}
    virtual ~Analyzer2DAnalysisLimits()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DRelativeTime: public TagFunction
{
  public:
    Analyzer2DRelativeTime()
    {}
    virtual ~Analyzer2DRelativeTime()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DComputeYScale: public TagFunction
{
  public:
    Analyzer2DComputeYScale()
    {}
    virtual ~Analyzer2DComputeYScale()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DMinimum: public TagFunction
{
  public:
    Analyzer2DMinimum()
    {}
    virtual ~Analyzer2DMinimum()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DMaximum: public TagFunction
{
  public:
    Analyzer2DMaximum()
    {}
    virtual ~Analyzer2DMaximum()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DDelta: public TagFunction
{
  public:
    Analyzer2DDelta()
    {}
    virtual ~Analyzer2DDelta()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DComputeGradient: public TagFunction
{
  public:
    Analyzer2DComputeGradient()
    {}
    virtual ~Analyzer2DComputeGradient()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};

class Analyzer2DMinimumGradient: public TagFunction
{
  public:
    Analyzer2DMinimumGradient()
    {}
    virtual ~Analyzer2DMinimumGradient()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer2DMaximumGradient: public TagFunction
{
  public:
    Analyzer2DMaximumGradient()
    {}
    virtual ~Analyzer2DMaximumGradient()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer3DControlWindow: public TagFunction
{
  public:
    Analyzer3DControlWindow()
    {}
    virtual ~Analyzer3DControlWindow()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer3DMinimum: public TagFunction
{
  public:
    Analyzer3DMinimum()
    {}
    virtual ~Analyzer3DMinimum()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer3DMaximum: public TagFunction
{
  public:
    Analyzer3DMaximum()
    {}
    virtual ~Analyzer3DMaximum()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer3DDelta: public TagFunction
{
  public:
    Analyzer3DDelta()
    {}
    virtual ~Analyzer3DDelta()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


class Analyzer3DFixedValue: public TagFunction
{
  public:
    Analyzer3DFixedValue()
    {}
    virtual ~Analyzer3DFixedValue()
    {}
    virtual bool parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime );
};


#endif // CFG_H_INCLUDED
