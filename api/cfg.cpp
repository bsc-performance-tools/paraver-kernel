#include <string>
#include <fstream>
#include <sstream>

#include "kernelconnection.h"
#include "cfg.h"
#include "window.h"
#include "trace.h"

#include "histogram.h"
#include "histogramstatistic.h"
#include "functionmanagement.h"
#include "semanticfunction.h"
#include "filter.h"
#include "paraverlabels.h"

using namespace std;

map<string, TagFunction *> CFGLoader::cfgTagFunctions;


TWindowLevel stringToLevel( const string& strLevel )
{
  TWindowLevel level = NONE;

  if ( strLevel.compare( OLDCFG_LVL_APPL ) == 0 )
    level = APPLICATION;
  else if ( strLevel.compare( OLDCFG_LVL_PTASK ) == 0 )
    level = APPLICATION;
  else if ( strLevel.compare( OLDCFG_LVL_WORKLOAD ) == 0 )
    level = WORKLOAD;
  else if ( strLevel.compare( OLDCFG_LVL_TASK ) == 0 )
    level = TASK;
  else if ( strLevel.compare( OLDCFG_LVL_THREAD ) == 0 )
    level = THREAD;
  else if ( strLevel.compare( OLDCFG_LVL_CPU ) == 0 )
    level = CPU;
  else if ( strLevel.compare( OLDCFG_LVL_NODE ) == 0 )
    level = NODE;
  else if ( strLevel.compare( OLDCFG_LVL_SYSTEM ) == 0 )
    level = SYSTEM;
  else if ( strLevel.compare( OLDCFG_LVL_TOPCOMPOSE1 ) == 0 )
    level = TOPCOMPOSE1;
  else if ( strLevel.compare( OLDCFG_LVL_TOPCOMPOSE2 ) == 0 )
    level = TOPCOMPOSE2;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_WORKLOAD ) == 0 )
    level = COMPOSEWORKLOAD;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_APPL ) == 0 )
    level = COMPOSEAPPLICATION;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_TASK ) == 0 )
    level = COMPOSETASK;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_THREAD ) == 0 )
    level = COMPOSETHREAD;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_CPU ) == 0 )
    level = COMPOSECPU;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_NODE ) == 0 )
    level = COMPOSENODE;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_SYSTEM ) == 0 )
    level = COMPOSESYSTEM;
  // Old semantic composes
  else if ( strLevel.compare( OLDCFG_LVL_TOPCOMPOSE1 ) == 0 )
    level = TOPCOMPOSE1;
  else if ( strLevel.compare( OLDCFG_LVL_TOPCOMPOSE2 ) == 0 )
    level = TOPCOMPOSE2;

  return level;
}

bool CFGLoader::isCFGFile( const string& filename )
{
  string cfgExt;

  cfgExt = filename.substr( filename.length() - 4 );

  return ( cfgExt.compare( ".cfg" ) == 0 );
}

bool CFGLoader::loadCFG( KernelConnection *whichKernel,
                         string& filename,
                         Trace *whichTrace,
                         vector<Window *>& windows,
                         Histogram *histogram )
{
  ifstream cfgFile( filename.c_str() );
  if ( !cfgFile )
    return false;

  histogram = NULL;

  loadMap();

  windows.push_back( NULL );
  histogram = NULL;

  while ( !cfgFile.eof() )
  {
    string strLine;
    string cfgTag;

    getline( cfgFile, strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' || strLine[ 0 ] == '<' )
      continue;

    istringstream auxStream( strLine );
    getline( auxStream, cfgTag, ' ' );

    map<string, TagFunction *>::iterator it = cfgTagFunctions.find( cfgTag );

    if ( it != cfgTagFunctions.end() )
    {
      if ( !it->second->parseLine( whichKernel, auxStream, whichTrace, windows,
                                   histogram ) )
      {
        if ( windows[ windows.size() - 1 ] != NULL )
          delete windows[ windows.size() - 1 ];
        windows[ windows.size() - 1 ] = NULL;
      }
    }
  }

  cfgFile.close();

  unLoadMap();

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  return true;
}


void CFGLoader::loadMap()
{
  cfgTagFunctions[OLDCFG_TAG_WNDW_TYPE]                = new WindowType();
  cfgTagFunctions[OLDCFG_TAG_WNDW_FACTORS]             = new WindowFactors();
  cfgTagFunctions[OLDCFG_TAG_WNDW_UNITS]               = new WindowUnits();
  cfgTagFunctions[OLDCFG_TAG_WNDW_OPERATION]           = new WindowOperation();
  cfgTagFunctions[OLDCFG_TAG_WNDW_MAXIMUM_Y]           = new WindowMaximumY();
  cfgTagFunctions[OLDCFG_TAG_WNDW_LEVEL]               = new WindowLevel();
  cfgTagFunctions[OLDCFG_TAG_WNDW_SCALE_RELATIVE]      = new WindowScaleRelative();
  cfgTagFunctions[OLDCFG_TAG_WNDW_OBJECT]              = new WindowObject();
  cfgTagFunctions[OLDCFG_TAG_WNDW_IDENTIFIERS]         = new WindowIdentifiers();

  cfgTagFunctions[OLDCFG_TAG_WNDW_BEGIN_TIME]          = new WindowBeginTime();
  cfgTagFunctions[OLDCFG_TAG_WNDW_STOP_TIME]           = new WindowStopTime();
  cfgTagFunctions[OLDCFG_TAG_WNDW_END_TIME]            = new WindowEndTime();
  cfgTagFunctions[OLDCFG_TAG_WNDW_BEGIN_TIME_RELATIVE] = new WindowBeginTimeRelative();

  cfgTagFunctions[OLDCFG_TAG_WNDW_NUMBER_OF_ROWS]      = new WindowNumberOfRow();
  cfgTagFunctions[OLDCFG_TAG_WNDW_SELECTED_FUNCTIONS]  = new WindowSelectedFunctions();
  cfgTagFunctions[OLDCFG_TAG_WNDW_SEMANTIC_MODULE]     = new WindowSemanticModule();
  cfgTagFunctions[OLDCFG_TAG_WNDW_COMPOSE_FUNCTIONS]   = new WindowComposeFunctions();

  // Filter options
  cfgTagFunctions[OLDCFG_TAG_WNDW_FILTER_MODULE]       = new WindowFilterModule();
  cfgTagFunctions[OLDCFG_TAG_WNDW_LOGICAL_FILTERED]    = new WindowFilterLogical();
  cfgTagFunctions[OLDCFG_TAG_WNDW_PHYSICAL_FILTERED]   = new WindowFilterPhysical();
  cfgTagFunctions[OLDCFG_TAG_WNDW_FROMTO]              = new WindowFilterBoolOpFromTo();
  cfgTagFunctions[OLDCFG_TAG_WNDW_COMM_TAGSIZE]        = new WindowFilterBoolOpTagSize();
  cfgTagFunctions[OLDCFG_TAG_WNDW_TYPEVAL]             = new WindowFilterBoolOpTypeVal();

  // Histogram options

  cfgTagFunctions[OLDCFG_TAG_AN2D_NEW]                 = new Analyzer2DCreate();
  // --> Analyzer2D.Name:
  // --> Analyzer2D.X:
  // --> Analyzer2D.Y:
  // --> Analyzer2D.Width:
  // --> Analyzer2D.Height:
  cfgTagFunctions[OLDCFG_TAG_AN2D_CONTROL_WINDOW]      = new Analyzer2DControlWindow();
  cfgTagFunctions[OLDCFG_TAG_AN2D_DATA_WINDOW]         = new Analyzer2DDataWindow();
  cfgTagFunctions[OLDCFG_TAG_AN2D_STATISTIC]           = new Analyzer2DStatistic();
  cfgTagFunctions[OLDCFG_TAG_AN2D_CALCULATE_ALL]       = new Analyzer2DCalculateAll();
  cfgTagFunctions[OLDCFG_TAG_AN2D_NUM_COLUMNS]         = new Analyzer2DNumColumns();
  // --> Analyzer2D.hide_empty_cols:
  cfgTagFunctions[OLDCFG_TAG_AN2D_HIDE_COLS]           = new Analyzer2DHideColumns();
  cfgTagFunctions[OLDCFG_TAG_AN2D_SCIENTIFIC_NOTATION] = new Analyzer2DScientificNotation();
  cfgTagFunctions[OLDCFG_TAG_AN2D_NUM_DECIMALS]        = new Analyzer2DNumDecimals();
  cfgTagFunctions[OLDCFG_TAG_AN2D_THOUSANDSEP]         = new Analyzer2DThousandSeparator();
  cfgTagFunctions[OLDCFG_TAG_AN2D_UNITS]               = new Analyzer2DUnits();
  cfgTagFunctions[OLDCFG_TAG_AN2D_HORIZONTAL]          = new Analyzer2DHorizontal();
  // --> Analyzer2D.Color:
  // --> Analyzer2D.SemanticColor:
  // --> Analyzer2D.TextMode:
  // --> Analyzer2D.Zoom:
  // --> Analyzer2D.Expanded:
  // --> Analyzer2D.expanded:
  cfgTagFunctions[OLDCFG_TAG_AN2D_ACCUMULATOR]          = new Analyzer2DAccumulator();
  cfgTagFunctions[OLDCFG_TAG_AN2D_ACCUM_BY_CTRL_WINDOW] = new Analyzer2DAccumulateByControlWindow();
  cfgTagFunctions[OLDCFG_TAG_AN2D_SORTCOLS]             = new Analyzer2DSortCols();
  cfgTagFunctions[OLDCFG_TAG_AN2D_SORTCRITERIA]         = new Analyzer2DSortCriteria();

  cfgTagFunctions[OLDCFG_TAG_AN2D_PARAMETERS]           = new Analyzer2DParameters();
  cfgTagFunctions[OLDCFG_TAG_AN2D_ANALYSISLIMITS]       = new Analyzer2DAnalysisLimits();
  cfgTagFunctions[OLDCFG_TAG_AN2D_RELATIVETIME]         = new Analyzer2DRelativeTime();
  // --> cfgTagFunctions["Analyzer2D.RelativeXScale:"] = new Analyzer2DRelativeXScale();
  // --> Analyzer2D.ShowWindows:
  cfgTagFunctions[OLDCFG_TAG_AN2D_COMPUTEYSCALE]        = new Analyzer2DComputeYScale();
  cfgTagFunctions[OLDCFG_TAG_AN2D_MINIMUM]              = new Analyzer2DMinimum();
  cfgTagFunctions[OLDCFG_TAG_AN2D_MAXIMUM]              = new Analyzer2DMaximum();
  cfgTagFunctions[OLDCFG_TAG_AN2D_DELTA]                = new Analyzer2DDelta();
  cfgTagFunctions[OLDCFG_TAG_AN2D_COMPUTEGRADIENT]      = new Analyzer2DComputeGradient();
  cfgTagFunctions[OLDCFG_TAG_AN2D_MINIMUMGRADIENT]      = new Analyzer2DMinimumGradient();
  cfgTagFunctions[OLDCFG_TAG_AN2D_MAXIMUMGRADIENT]      = new Analyzer2DMaximumGradient();

  // 3D Histogram
  cfgTagFunctions[OLDCFG_TAG_AN3D_CONTROLWINDOW]        = new Analyzer3DControlWindow();
  cfgTagFunctions[OLDCFG_TAG_AN3D_MINIMUM]              = new Analyzer3DMinimum();
  cfgTagFunctions[OLDCFG_TAG_AN3D_MAXIMUM]              = new Analyzer3DMaximum();
  cfgTagFunctions[OLDCFG_TAG_AN3D_DELTA]                = new Analyzer3DDelta();
  cfgTagFunctions[OLDCFG_TAG_AN3D_FIXEDVALUE]           = new Analyzer3DFixedValue();
}

void CFGLoader::unLoadMap()
{
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_TYPE];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_FACTORS];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_UNITS];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_OPERATION];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_MAXIMUM_Y];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_LEVEL];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_SCALE_RELATIVE];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_OBJECT];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_IDENTIFIERS];

  delete cfgTagFunctions[OLDCFG_TAG_WNDW_BEGIN_TIME];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_STOP_TIME];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_END_TIME];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_BEGIN_TIME_RELATIVE];

  delete cfgTagFunctions[OLDCFG_TAG_WNDW_NUMBER_OF_ROWS];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_SELECTED_FUNCTIONS];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_SEMANTIC_MODULE];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_COMPOSE_FUNCTIONS];

  // Filter options
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_FILTER_MODULE];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_LOGICAL_FILTERED];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_PHYSICAL_FILTERED];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_FROMTO];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_COMM_TAGSIZE];
  delete cfgTagFunctions[OLDCFG_TAG_WNDW_TYPEVAL];

  // Histogram
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_NEW];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_CONTROL_WINDOW];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_DATA_WINDOW];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_STATISTIC];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_CALCULATE_ALL];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_NUM_COLUMNS];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_HIDE_COLS];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_SCIENTIFIC_NOTATION];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_NUM_DECIMALS];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_THOUSANDSEP];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_UNITS];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_HORIZONTAL];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_ACCUMULATOR];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_ACCUM_BY_CTRL_WINDOW];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_SORTCOLS];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_SORTCRITERIA];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_PARAMETERS];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_ANALYSISLIMITS];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_RELATIVETIME];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_COMPUTEYSCALE];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_MINIMUM];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_MAXIMUM];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_DELTA];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_COMPUTEGRADIENT];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_MINIMUMGRADIENT];
  delete cfgTagFunctions[OLDCFG_TAG_AN2D_MAXIMUMGRADIENT];

  // 3D Histogram
  delete cfgTagFunctions[OLDCFG_TAG_AN3D_CONTROLWINDOW];
  delete cfgTagFunctions[OLDCFG_TAG_AN3D_MINIMUM];
  delete cfgTagFunctions[OLDCFG_TAG_AN3D_MAXIMUM];
  delete cfgTagFunctions[OLDCFG_TAG_AN3D_DELTA];
  delete cfgTagFunctions[OLDCFG_TAG_AN3D_FIXEDVALUE];
}


bool WindowType::parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            Histogram *histogram )
{
  string type;
  Window *tmpWin;

  getline( line, type, ' ' );
  if ( type.compare( OLDCFG_VAL_WNDW_TYPE_SINGLE ) == 0 )
  {
    tmpWin = Window::create( whichKernel, whichTrace );
  }
  else if ( type.compare( OLDCFG_VAL_WNDW_TYPE_COMPOSED ) == 0 )
  {
    tmpWin = Window::create( whichKernel );
  }
  else
    return false;

  if ( windows.size() == 1 && windows[ 0 ] == NULL )
    windows[ 0 ] = tmpWin;
  else
    windows.push_back( tmpWin );

  return true;
}

bool WindowFactors::parseLine( KernelConnection *whichKernel, istringstream& line,
                               Trace *whichTrace,
                               vector<Window *>& windows,
                               Histogram *histogram )
{
  string strFactor;
  UINT16 numFactor = 0;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( !windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  while ( !line.eof() )
  {
    getline( line, strFactor, ' ' );
    istringstream tmpStream( strFactor );
    TSemanticValue factorValue;

    if ( !( tmpStream >> factorValue ) )
      return false;

    windows[ windows.size() - 1 ]->setFactor( numFactor, factorValue );

    numFactor++;
  }
  return true;
}


bool WindowUnits::parseLine( KernelConnection *whichKernel, istringstream& line,
                             Trace *whichTrace,
                             vector<Window *>& windows,
                             Histogram *histogram )
{
  string strUnits;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strUnits, ' ' );

  if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_NS ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( NS );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_US ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( US );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_MS ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( MS );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_S ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( SEC );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_M ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( MIN );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_H ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( HOUR );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_D ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( DAY );

  return true;
}

bool WindowOperation::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 Histogram *histogram )
{
  string strFunction;
  SemanticFunction *function;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( !windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strFunction, ' ' );
  function = ( FunctionManagement<SemanticFunction>::getInstance() )->getFunction( strFunction );

  if ( function == NULL )
    return false;

  windows[ windows.size() - 1 ]->setLevelFunction( DERIVED, function );

  return true;
}

// For representation purposes.
bool WindowMaximumY::parseLine( KernelConnection *whichKernel, istringstream& line,
                                Trace *whichTrace,
                                vector<Window *>& windows,
                                Histogram *histogram )
{
  return true;
}


bool WindowLevel::parseLine( KernelConnection *whichKernel, istringstream& line,
                             Trace *whichTrace,
                             vector<Window *>& windows,
                             Histogram *histogram )
{
  string strLevel;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strLevel, ' ' );

  windows[ windows.size() - 1 ]->setLevel( stringToLevel( strLevel ) );

  return true;
}


bool WindowIdentifiers::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   Histogram *histogram )
{
  string strID;
  UINT16 id;
  UINT16 numID = 0;
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( !windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  while ( !line.eof() )
  {
    getline( line, strID, ' ' );
    istringstream tmpStream( strID );
    if ( !( tmpStream >> id ) )
      return false;

    if ( windows[ id - 1 ] == NULL )
      return false;

    windows[ windows.size() - 1 ]->setParent( numID, windows[ id - 1 ] );
    numID++;
  }

  return true;
}

bool WindowScaleRelative::parseLine( KernelConnection *whichKernel, istringstream& line,
                                     Trace *whichTrace,
                                     vector<Window *>& windows,
                                     Histogram *histogram )
{
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  windows[ windows.size() - 1 ]->setWindowEndTime(
    ( windows[ windows.size() - 1 ]->getTrace() )->getEndTime() );

  return true;
}

bool WindowObject::parseLine( KernelConnection *whichKernel, istringstream& line, Trace *whichTrace,
                              vector<Window *>& windows,
                              Histogram *histogram )
{
  return true;
}

bool WindowBeginTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 Histogram *histogram )
{
  string strTime;
  TRecordTime auxTime;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strTime, ' ' );
  istringstream tmpTime( strTime );

  if ( !( tmpTime >> auxTime ) )
    return false;

  windows[ windows.size() - 1 ]->setWindowBeginTime( auxTime );

  return true;
}

bool WindowEndTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                               Trace *whichTrace,
                               vector<Window *>& windows,
                               Histogram *histogram )
{
  string strTime;
  TRecordTime auxTime;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strTime, ' ' );
  istringstream tmpTime( strTime );

  if ( !( tmpTime >> auxTime ) )
    return false;

  // endTime = auxTime;

  return true;
}

bool WindowStopTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                                Trace *whichTrace,
                                vector<Window *>& windows,
                                Histogram *histogram )
{
  string strTime;
  TRecordTime auxTime;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strTime, ' ' );
  istringstream tmpTime( strTime );

  if ( !( tmpTime >> auxTime ) )
    return false;

  windows[ windows.size() - 1 ]->setWindowEndTime( auxTime );

  return true;
}

bool WindowBeginTimeRelative::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strPercentage;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strPercentage, ' ' );
  istringstream tmpStream( strPercentage );
  TSemanticValue percentage;

  if ( !( tmpStream >> percentage ) )
    return false;

  windows[ windows.size() - 1 ]->setWindowBeginTime(
    ( windows[ windows.size() - 1 ]->getTrace() )->getEndTime() * percentage );

  return true;
}

bool WindowNumberOfRow::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   Histogram *histogram )
{
  return true;
}

bool WindowSelectedFunctions::parseLine( KernelConnection *whichKernel, istringstream& line, Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string tmpString;
  string strNumFunctions;
  UINT16 numFunctions;
  string strLevel;
  TWindowLevel level;
  string strFunction;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, tmpString, ' ' );
  getline( line, strNumFunctions, ',' );
  istringstream tmpNumFunctions( strNumFunctions );

  if ( !( tmpNumFunctions >> numFunctions ) )
    return false;

  getline( line, tmpString, '{' );
  for ( UINT16 i = 0; i < numFunctions; i++ )
  {
    getline( line, tmpString, '{' );
    getline( line, strLevel, ',' );
    getline( line, tmpString, ' ' );
    getline( line, strFunction, '}' );
    level = stringToLevel( strLevel );

    // It's a semantic function
    if ( level != NONE )
    {
      SemanticFunction *function;

      function = ( FunctionManagement<SemanticFunction>::getInstance() )->getFunction( strFunction );
      if ( function == NULL )
        return false;
      windows[ windows.size() - 1 ]->setLevelFunction( level, function );
    }
    // It's a filter function
    else
    {
      FilterFunction *function;
      Filter *filter = windows[ windows.size() - 1 ]->getFilter();


      function = ( FunctionManagement<FilterFunction>::getInstance() )->getFunction( strFunction );
      if ( function == NULL )
        return false;

      if ( strLevel.compare( OLDCFG_VAL_FILTER_OBJ_FROM ) == 0 )
        filter->setCommFromFunction( function );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_OBJ_TO ) == 0 )
        filter->setCommToFunction( function );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_COM_TAG ) == 0 )
        filter->setCommTagFunction( function );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_COM_SIZE ) == 0 )
        filter->setCommSizeFunction( function );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_COM_BW ) == 0 )
        filter->setBandWidthFunction( function );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_EVT_TYPE ) == 0 )
        filter->setEventTypeFunction( function );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_EVT_VALUE ) == 0 )
        filter->setEventValueFunction( function );
    }
  }

  return true;
}


bool WindowComposeFunctions::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<Window *>& windows,
                                        Histogram *histogram )
{
  string tmpString;
  string strNumFunctions;
  UINT16 numFunctions;
  string strLevel;
  TWindowLevel level;
  string strFunction;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, tmpString, ' ' );
  getline( line, strNumFunctions, ',' );
  istringstream tmpNumFunctions( strNumFunctions );

  if ( !( tmpNumFunctions >> numFunctions ) )
    return false;

  getline( line, tmpString, '{' );
  for ( UINT16 i = 0; i < numFunctions; i++ )
  {
    getline( line, tmpString, '{' );
    getline( line, strLevel, ',' );
    getline( line, tmpString, ' ' );
    getline( line, strFunction, '}' );
    level = stringToLevel( strLevel );

    // It's a semantic function
    if ( level != NONE )
    {
      SemanticFunction *function;

      function = ( FunctionManagement<SemanticFunction>::getInstance() )->getFunction( strFunction );
      if ( function == NULL )
        return false;
      if ( !windows[ windows.size() - 1 ]->setLevelFunction( level, function ) )
        delete function;
    }
    else
      return false;
  }

  return true;
}


bool WindowSemanticModule::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      Histogram *histogram )
{
  string strLevel;
  TWindowLevel level;
  string strFunction;
  SemanticFunction *function;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strLevel, ' ' );
  level = stringToLevel( strLevel );
  if ( level == NONE )
    return false;

  getline( line, strFunction, '{' );
  strFunction.erase( strFunction.length() - 1 ); // Final space.

  function = ( FunctionManagement<SemanticFunction>::getInstance() )->getFunction( strFunction );
  if ( function == NULL )
    return false;

  if ( typeid( *( windows[ windows.size() - 1 ]->getLevelFunction( level ) ) ) ==
       typeid( *function ) )
  {
    string tmpString;
    string strNumParam;
    TParamIndex numParam;

    delete function;

    getline( line, tmpString, ' ' );
    getline( line, strNumParam, ',' );
    istringstream tmpNumParam( strNumParam );
    if ( !( tmpNumParam >> numParam ) )
      return false;

    getline( line, tmpString, '{' );

    for ( TParamIndex i = 0; i < numParam; i++ )
    {
      string strNumValues;
      UINT32 numValues;
      TParamValue values;

      getline( line, tmpString, ' ' );

      getline( line, strNumValues, ' ' );
      istringstream tmpNumValues( strNumValues );
      if ( !( tmpNumValues >> numValues ) )
        return false;

      for ( UINT32 j = 0; j < numValues; j++ )
      {
        string strParamValue;
        double paramValue;

        if ( j < numValues - 1 )
          getline( line, strParamValue, ' ' );
        else if ( j == numValues - 1 )
        {
          if ( i < numParam - 1 )
            getline( line, strParamValue, ',' );
          else if ( i == numParam - 1 )
            getline( line, strParamValue, ' ' );
        }
        istringstream tmpParamValue( strParamValue );
        if ( !( tmpParamValue >> paramValue ) )
          return false;
        values.push_back( paramValue );
      }

      windows[ windows.size() - 1 ]->setFunctionParam( level, i, values );
    }

  }
  else
    delete function;

  return true;
}


bool WindowFilterModule::parseLine( KernelConnection *whichKernel, istringstream& line,
                                    Trace *whichTrace,
                                    vector<Window *>& windows,
                                    Histogram *histogram )
{
  string strTag, strNumberParams, strValue;
  UINT16 numParams;
  Filter *filter;
  TObjectOrder fromObject;
  TObjectOrder toObject;
  TCommTag commTag;
  TCommSize commSize;
  TSemanticValue bandWidth;
  TEventType eventType;
  TEventValue eventValue;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strTag, ' ' );          // Parameter type.
  getline( line, strNumberParams, ' ' ); // Number of following parameters.
  istringstream tmpNumberParams( strNumberParams );

  if ( !( tmpNumberParams >> numParams ) )
    return false;

  filter = windows[ windows.size() - 1 ]->getFilter();

  for ( UINT16 ii = 0; ii < numParams; ii++ )
  {
    if ( strTag.compare( OLDCFG_VAL_FILTER_OBJ_FROM ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> fromObject ) )
        return false;

      filter->insertCommFrom( fromObject );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_OBJ_TO ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> toObject ) )
        return false;

      filter->insertCommTo( toObject );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_COM_TAG ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> commTag ) )
        return false;

      filter->insertCommTag( commTag );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_COM_SIZE ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> commSize ) )
        return false;

      filter->insertCommSize( commSize );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_COM_BW ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> bandWidth ) )
        return false;

      filter->insertBandWitdh( bandWidth );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_EVT_TYPE ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> eventType ) )
        return false;

      filter->insertEventType( eventType );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_EVT_VALUE ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> eventValue ) )
        return false;

      filter->insertEventValue( eventValue );
    }
  }

  return true;
}


bool WindowFilterLogical::parseLine( KernelConnection *whichKernel, istringstream& line,
                                     Trace *whichTrace,
                                     vector<Window *>& windows,
                                     Histogram *histogram )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = windows[ windows.size() - 1 ]->getFilter();

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    filter->setLogical( false );
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    filter->setLogical( true );
  else
    return false;

  return true;
}


bool WindowFilterPhysical::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      Histogram *histogram )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = windows[ windows.size() - 1 ]->getFilter();

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    filter->setPhysical( false );
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    filter->setPhysical( true );
  else
    return false;

  return true;
}


bool WindowFilterBoolOpFromTo::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  filter = windows[ windows.size() - 1 ]->getFilter();

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    filter->setOpFromToOr();
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    filter->setOpFromToAnd();
  else
    return false;

  return true;
}


bool WindowFilterBoolOpTagSize::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = windows[ windows.size() - 1 ]->getFilter();

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    filter->setOpTagSizeOr();
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    filter->setOpTagSizeAnd();
  else
    return false;

  return true;
}


bool WindowFilterBoolOpTypeVal::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = windows[ windows.size() - 1 ]->getFilter();

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    filter->setOpTypeValueOr();
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    filter->setOpTypeValueAnd();
  else
    return false;

  return true;
}

bool Analyzer2DCreate::parseLine( KernelConnection *whichKernel, istringstream& line,
                                  Trace *whichTrace,
                                  vector<Window *>& windows,
                                  Histogram *histogram )
{
  if ( histogram == NULL )
    histogram = Histogram::create( whichKernel );

  return true;
}

bool Analyzer2DControlWindow::parseLine( KernelConnection *whichKernel, istringstream& line, Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strIndexControlWindow;
  UINT32 indexControlWindow;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strIndexControlWindow );
  istringstream tmpWindow( strIndexControlWindow );
  if ( !( tmpWindow >> indexControlWindow ) )
    return false;

  if ( indexControlWindow > windows.size() )
    return false;

  histogram->setControlWindow( windows[ indexControlWindow - 1 ] );

  return true;
}


bool Analyzer2DDataWindow::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      Histogram *histogram )
{
  string strIndexDataWindow;
  UINT32 indexDataWindow;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strIndexDataWindow );
  istringstream tmpWindow( strIndexDataWindow );
  if ( !( tmpWindow >> indexDataWindow ) )
    return false;

  if ( indexDataWindow > windows.size() )
    return false;

  histogram->setDataWindow( windows[ indexDataWindow - 1 ] );

  return true;
}


bool Analyzer2DStatistic::parseLine( KernelConnection *whichKernel, istringstream& line,
                                     Trace *whichTrace,
                                     vector<Window *>& windows,
                                     Histogram *histogram )
{
  string strStatistic;
  HistogramStatistic *statistic;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strStatistic );
  statistic = ( FunctionManagement<HistogramStatistic>::getInstance() )->getFunction( strStatistic );
  if ( statistic == NULL )
    return false;

  histogram->pushbackStatistic( statistic );

  return true;
}


bool Analyzer2DCalculateAll::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<Window *>& windows,
                                        Histogram *histogram )
{
  string strBoolAll;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBoolAll, ' ' );

  if ( strBoolAll.compare( OLDCFG_VAL_TRUE2 ) == 0 )
  {
    vector<HistogramStatistic *> v;
    ( FunctionManagement<HistogramStatistic>::getInstance() )->getAll( v );
    for ( UINT32 i = 0; i < v.size(); i++ )
      histogram->pushbackStatistic( v[ i ] );
  }
  else if ( strBoolAll.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    return true;
  else
    return false;

  return true;
}


bool Analyzer2DNumColumns::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      Histogram *histogram )
{
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  return true;
}


bool Analyzer2DHideColumns::parseLine( KernelConnection *whichKernel, istringstream& line,
                                       Trace *whichTrace,
                                       vector<Window *>& windows,
                                       Histogram *histogram )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histogram->setHideColumns( true );
  else if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histogram->setHideColumns( false );
  else
    return false;

  return true;
}



bool Analyzer2DScientificNotation::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histogram->setScientificNotation( true );
  else if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histogram->setScientificNotation( false );
  else
    return false;

  return true;
}


bool Analyzer2DNumDecimals::parseLine( KernelConnection *whichKernel, istringstream& line,
                                       Trace *whichTrace,
                                       vector<Window *>& windows,
                                       Histogram *histogram )
{
  string strDec;
  UINT16 numDecimals;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strDec, ' ' );
  istringstream tmpValue( strDec );
  if ( !( tmpValue >> numDecimals ) )
    return false;

  histogram->setNumDecimals( numDecimals );

  return true;
}


bool Analyzer2DThousandSeparator::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histogram->setThousandSeparator( true );
  else if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histogram->setThousandSeparator( false );
  else
    return false;

  return true;
}


bool Analyzer2DUnits::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 Histogram *histogram )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histogram->setShowUnits( true );
  else if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histogram->setShowUnits( false );
  else
    return false;

  return true;
}


bool Analyzer2DHorizontal::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      Histogram *histogram )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histogram->setHorizontal( true );
  else if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histogram->setHorizontal( false );
  else
    return false;

  return true;
}


bool Analyzer2DAccumulator:: parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<Window *>& windows,
                                        Histogram *histogram )
{
  string strAccumulator;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  return true;
}


bool Analyzer2DAccumulateByControlWindow::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    {}
  else if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    {}
  else
    return false;

  return true;
}


bool Analyzer2DSortCols::parseLine( KernelConnection *whichKernel, istringstream& line,
                                    Trace *whichTrace,
                                    vector<Window *>& windows,
                                    Histogram *histogram )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
  {
    histogram->setSortColumns( true );
  }
  else if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
  {
    histogram->setSortColumns( false );
  }
  else
    return false;

  return true;
}


bool Analyzer2DSortCriteria::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<Window *>& windows,
                                        Histogram *histogram )
{
  string strSortCriteria;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strSortCriteria );

  if ( strSortCriteria.compare( OLDCFG_VAL_SORT_AVERAGE ) == 0 )
    histogram->setSortCriteria( AVERAGE );
  else if ( strSortCriteria.compare( OLDCFG_VAL_SORT_TOTAL ) == 0 )
    histogram->setSortCriteria( TOTAL );
  else if ( strSortCriteria.compare( OLDCFG_VAL_SORT_MAXIMUM ) == 0 )
    histogram->setSortCriteria( MAXIMUM );
  else if ( strSortCriteria.compare( OLDCFG_VAL_SORT_MINIMUM ) == 0 )
    histogram->setSortCriteria( MINIMUM );
  else if ( strSortCriteria.compare( OLDCFG_VAL_SORT_STDEV ) == 0 )
    histogram->setSortCriteria( STDEV );
  else if ( strSortCriteria.compare( OLDCFG_VAL_SORT_AVGDIVMAX ) == 0 )
    histogram->setSortCriteria( AVGDIVMAX );
  else
    return false;

  return true;
}

/*
 Number_of_parameters Parameter1 ... ParameterN
*/
bool Analyzer2DParameters::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      Histogram *histogram )
{
  string strNumParams, strValue;
  UINT16 numParams;
  double dataValue;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strNumParams, ' ' ); // Number of following parameters.
  istringstream tmpNumParams( strNumParams );

  if ( !( tmpNumParams >> numParams ) )
    return false;

  for ( UINT16 ii = 0; ii < numParams; ii++ )
  {
    getline( line, strValue, ' ' );
    istringstream tmpValue( strValue );
    if ( !( tmpValue >> dataValue ) )
      return false;
    if ( ii == 0 )
      histogram->setDataMin( dataValue );
    else if ( ii == 1 )
      histogram->setDataMax( dataValue );
  }

  return false;
}


bool Analyzer2DAnalysisLimits::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strLimit;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strLimit, ' ' );

  if ( strLimit.compare( OLDCFG_VAL_LIMIT_ALLTRACE ) == 0 )
  {
    histogram->setWindowBeginTime( 0.0 );
    histogram->setWindowEndTime( whichTrace->getEndTime() );
  }
  else if ( strLimit.compare( OLDCFG_VAL_LIMIT_ALLWINDOW ) == 0 )
  {
    histogram->setWindowBeginTime( histogram->getControlWindow()->getWindowBeginTime() );
    histogram->setWindowEndTime( histogram->getControlWindow()->getWindowEndTime() );
  }
  else if ( strLimit.compare( OLDCFG_VAL_LIMIT_REGION ) == 0 )
  {
    // Not implemented yet
    histogram->setWindowBeginTime( 0.0 );
    histogram->setWindowEndTime( whichTrace->getEndTime() );
  }
  else
    return false;

  return true;
}


bool Analyzer2DRelativeTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<Window *>& windows,
                                        Histogram *histogram )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    {}
  else if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    {}
  else
    return false;

  return true;
}


bool Analyzer2DComputeYScale::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histogram->setComputeScale( false );
  else if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histogram->setComputeScale( true );
  else
    return false;

  return true;
}

bool Analyzer2DMinimum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   Histogram *histogram )
{
  string strMinimum;
  THistogramLimit dataMinimum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strMinimum );
  istringstream tmpValue( strMinimum );
  if ( !( tmpValue >> dataMinimum ) )
    return false;
  histogram->setControlMin( dataMinimum );

  return true;
}


bool Analyzer2DMaximum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   Histogram *histogram )
{
  string strMaximum;
  THistogramLimit dataMaximum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strMaximum );
  istringstream tmpValue( strMaximum );
  if ( !( tmpValue >> dataMaximum ) )
    return false;
  histogram->setControlMax( dataMaximum );

  return true;
}


bool Analyzer2DDelta::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 Histogram *histogram )
{
  string strDelta;
  THistogramLimit dataDelta;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strDelta );
  istringstream tmpValue( strDelta );
  if ( !( tmpValue >> dataDelta ) )
    return false;
  histogram->setControlDelta( dataDelta );

  return true;
}


bool Analyzer2DComputeGradient::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histogram->setComputeGradient( false );
  else if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histogram->setComputeGradient( true );
  else
    return false;

  return true;
}


bool Analyzer2DMinimumGradient::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strMinimumGradient;
  THistogramLimit dataMinimumGradient;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strMinimumGradient );
  istringstream tmpValue( strMinimumGradient );
  if ( !( tmpValue >> dataMinimumGradient ) )
    return false;
  histogram->setMinGradient( dataMinimumGradient );

  return true;
}


bool Analyzer2DMaximumGradient::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string strMaximumGradient;
  THistogramLimit dataMaximumGradient;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, strMaximumGradient );
  istringstream tmpValue( strMaximumGradient );
  if ( !( tmpValue >> dataMaximumGradient ) )
    return false;
  histogram->setMaxGradient( dataMaximumGradient );

  return true;
}


bool Analyzer3DControlWindow::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    Histogram *histogram )
{
  string str3DControlWindow;
  UINT32 controlWindow;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, str3DControlWindow );
  istringstream tmpValue( str3DControlWindow );
  if ( !( tmpValue >> controlWindow ) )
    return false;
  histogram->setExtraControlWindow( windows[ controlWindow ] );

  return true;
}


bool Analyzer3DMinimum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   Histogram *histogram )
{
  string str3DMinimum;
  THistogramLimit data3DMinimum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, str3DMinimum );
  istringstream tmpValue( str3DMinimum );
  if ( !( tmpValue >> data3DMinimum ) )
    return false;
  histogram->setExtraControlMin( data3DMinimum );

  return true;
}


bool Analyzer3DMaximum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   Histogram *histogram )
{
  string str3DMaximum;
  THistogramLimit data3DMaximum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, str3DMaximum );
  istringstream tmpValue( str3DMaximum );
  if ( !( tmpValue >> data3DMaximum ) )
    return false;
  histogram->setExtraControlMax( data3DMaximum );

  return true;
}


bool Analyzer3DDelta::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 Histogram *histogram )
{
  string str3DDelta;
  THistogramLimit data3DDelta;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, str3DDelta );
  istringstream tmpValue( str3DDelta );
  if ( !( tmpValue >> data3DDelta ) )
    return false;
  histogram->setExtraControlDelta( data3DDelta );

  return true;
}


bool Analyzer3DFixedValue::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      Histogram *histogram )
{
  string str3DFixedValue;
  double data3DFixedValue;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histogram == NULL )
    return false;

  getline( line, str3DFixedValue );
  istringstream tmpValue( str3DFixedValue );
  if ( !( tmpValue >> data3DFixedValue ) )
    return false;
  histogram->setPlaneMinValue( data3DFixedValue );

  return true;
}
