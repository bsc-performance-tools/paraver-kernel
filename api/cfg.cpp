#include <string>
#include <fstream>
#include <sstream>

#include "kernelconnection.h"
#include "cfg.h"
#include "kwindow.h"
#include "trace.h"

#include "histogram.h"
#include "histogramstatistic.h"
#include "functionmanagement.h"
#include "semanticfunction.h"
#include "filter.h"


using namespace std;

map<string, TagFunction *> CFGLoader::cfgTagFunctions;


TWindowLevel stringToLevel( const string& strLevel )
{
  TWindowLevel level = NONE;

  if ( strLevel.compare( "appl" ) == 0 )
    level = APPLICATION;
  else if ( strLevel.compare( "ptask" ) == 0 )
    level = APPLICATION;
  else if ( strLevel.compare( "workload" ) == 0 )
    level = WORKLOAD;
  else if ( strLevel.compare( "task" ) == 0 )
    level = TASK;
  else if ( strLevel.compare( "thread" ) == 0 )
    level = THREAD;
  else if ( strLevel.compare( "cpu" ) == 0 )
    level = CPU;
  else if ( strLevel.compare( "node" ) == 0 )
    level = NODE;
  else if ( strLevel.compare( "system" ) == 0 )
    level = SYSTEM;
  else if ( strLevel.compare( "topcompose1" ) == 0 )
    level = TOPCOMPOSE1;
  else if ( strLevel.compare( "topcompose2" ) == 0 )
    level = TOPCOMPOSE2;
  else if ( strLevel.compare( "compose_workload" ) == 0 )
    level = COMPOSEWORKLOAD;
  else if ( strLevel.compare( "compose_appl" ) == 0 )
    level = COMPOSEAPPLICATION;
  else if ( strLevel.compare( "compose_task" ) == 0 )
    level = COMPOSETASK;
  else if ( strLevel.compare( "compose_thread" ) == 0 )
    level = COMPOSETHREAD;
  else if ( strLevel.compare( "compose_cpu" ) == 0 )
    level = COMPOSECPU;
  else if ( strLevel.compare( "compose_node" ) == 0 )
    level = COMPOSENODE;
  else if ( strLevel.compare( "compose_system" ) == 0 )
    level = COMPOSESYSTEM;
  // Old semantic composes
  else if ( strLevel.compare( "compose1" ) == 0 )
    level = TOPCOMPOSE1;
  else if ( strLevel.compare( "compose2" ) == 0 )
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
                         vector<KWindow *>& windows,
                         Histogram *histogram,
                         TRecordTime& beginTime, TRecordTime& endTime )
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
                                   histogram, beginTime, endTime ) )
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
  cfgTagFunctions["window_type"]                = new WindowType();
  cfgTagFunctions["window_factors"]             = new WindowFactors();
  cfgTagFunctions["window_units"]               = new WindowUnits();
  cfgTagFunctions["window_operation"]           = new WindowOperation();
  cfgTagFunctions["window_maximum_y"]           = new WindowMaximumY();
  cfgTagFunctions["window_level"]               = new WindowLevel();
  cfgTagFunctions["window_scale_relative"]      = new WindowScaleRelative();
  cfgTagFunctions["window_object"]              = new WindowObject();
  cfgTagFunctions["window_identifiers"]         = new WindowIdentifiers();

  cfgTagFunctions["window_begin_time"]          = new WindowBeginTime();
  cfgTagFunctions["window_stop_time"]           = new WindowStopTime();
  cfgTagFunctions["window_end_time"]            = new WindowEndTime();
  cfgTagFunctions["window_begin_time_relative"] = new WindowBeginTimeRelative();

  cfgTagFunctions["window_number_of_row"]       = new WindowNumberOfRow();
  cfgTagFunctions["window_selected_functions"]  = new WindowSelectedFunctions();
  cfgTagFunctions["window_semantic_module"]     = new WindowSemanticModule();
  cfgTagFunctions["window_compose_functions"]   = new WindowComposeFunctions();

  // Filter options
  cfgTagFunctions["window_filter_module"]       = new WindowFilterModule();
  cfgTagFunctions["window_logical_filtered"]    = new WindowFilterLogical();
  cfgTagFunctions["window_physical_filtered"]   = new WindowFilterPhysical();
  cfgTagFunctions["window_fromto"]              = new WindowFilterBoolOpFromTo();
  cfgTagFunctions["window_comm_tagsize"]        = new WindowFilterBoolOpTagSize();
  cfgTagFunctions["window_typeval"]             = new WindowFilterBoolOpTypeVal();

  // Histogram options

  cfgTagFunctions["< NEW ANALYZER2D >"] = new Analyzer2DCreate();
  // --> Analyzer2D.Name:
  // --> Analyzer2D.X:
  // --> Analyzer2D.Y:
  // --> Analyzer2D.Width:
  // --> Analyzer2D.Height:
  cfgTagFunctions["Analyzer2D.ControlWindow:"] = new Analyzer2DControlWindow();
  cfgTagFunctions["Analyzer2D.DataWindow:"] = new Analyzer2DDataWindow();
  cfgTagFunctions["Analyzer2D.Statistic:"] = new Analyzer2DStatistic();
  cfgTagFunctions["Analyzer2D.calculate_all:"] = new Analyzer2DCalculateAll();
  cfgTagFunctions["Analyzer2D.num_columns:"] = new Analyzer2DNumColumns();
  // --> Analyzer2D.hide_empty_cols:
  cfgTagFunctions["Analyzer2D.HideCols:"] = new Analyzer2DHideColumns();
  cfgTagFunctions["Analyzer2D.scientific_notation:"] = new Analyzer2DScientificNotation();
  cfgTagFunctions["Analyzer2D.num_decimals:"] = new Analyzer2DNumDecimals();
  cfgTagFunctions["Analyzer2D.thousandsep:"] = new Analyzer2DThousandSeparator();
  cfgTagFunctions["Analyzer2D.units:"] = new Analyzer2DUnits();
  cfgTagFunctions["Analyzer2D.HorizVert"] = new Analyzer2DHorizontal();
  // --> Analyzer2D.Color:
  // --> Analyzer2D.SemanticColor:
  // --> Analyzer2D.TextMode:
  // --> Analyzer2D.Zoom:
  // --> Analyzer2D.Expanded:
  // --> Analyzer2D.expanded:
  cfgTagFunctions["Analyzer2D.Accumulator:"] = new Analyzer2DAccumulator();
  cfgTagFunctions["Analyzer2D.AccumulateByControlWindow:"] = new Analyzer2DAccumulateByControlWindow();
  cfgTagFunctions["Analyzer2D.SortCols:"] = new Analyzer2DSortCols();
  cfgTagFunctions["Analyzer2D.SortCriteria:"] = new Analyzer2DSortCriteria();

  cfgTagFunctions["Analyzer2D.Parameters:"] = new Analyzer2DParameters();
  cfgTagFunctions["Analyzer2D.AnalysisLimits:"] = new Analyzer2DAnalysisLimits();
  cfgTagFunctions["Analyzer2D.RelativeTime:"] = new Analyzer2DRelativeTime();
  // --> cfgTagFunctions["Analyzer2D.RelativeXScale:"] = new Analyzer2DRelativeXScale();
  // --> Analyzer2D.ShowWindows:
  cfgTagFunctions["Analyzer2D.ComputeYScale:"] = new Analyzer2DComputeYScale();
  cfgTagFunctions["Analyzer2D.Minimum:"] = new Analyzer2DMinimum();
  cfgTagFunctions["Analyzer2D.Maximum:"] = new Analyzer2DMaximum();
  cfgTagFunctions["Analyzer2D.Delta:"] = new Analyzer2DDelta();
  cfgTagFunctions["Analyzer2D.ComputeGradient:"] = new Analyzer2DComputeGradient();
  cfgTagFunctions["Analyzer2D.MinimumGradient:"] = new Analyzer2DMinimumGradient();
  cfgTagFunctions["Analyzer2D.MaximumGradient:"] = new Analyzer2DMaximumGradient();

  // 3D Histogram
  cfgTagFunctions["Analyzer2D.3D_ControlWindow:"] = new Analyzer3DControlWindow();
  cfgTagFunctions["Analyzer2D.3D_Minimum:"] = new Analyzer3DMinimum();
  cfgTagFunctions["Analyzer2D.3D_Maximum:"] = new Analyzer3DMaximum();
  cfgTagFunctions["Analyzer2D.3D_Delta:"] = new Analyzer3DDelta();
  cfgTagFunctions["Analyzer2D.3D_FixedValue:"] = new Analyzer3DFixedValue();
}

void CFGLoader::unLoadMap()
{
  delete cfgTagFunctions["window_type"];
  delete cfgTagFunctions["window_factors"];
  delete cfgTagFunctions["window_units"];
  delete cfgTagFunctions["window_operation"];
  delete cfgTagFunctions["window_maximum_y"];
  delete cfgTagFunctions["window_level"];
  delete cfgTagFunctions["window_scale_relative"];
  delete cfgTagFunctions["window_object"];
  delete cfgTagFunctions["window_identifiers"];

  delete cfgTagFunctions["window_begin_time"];
  delete cfgTagFunctions["window_stop_time"];
  delete cfgTagFunctions["window_end_time"];
  delete cfgTagFunctions["window_begin_time_relative"];

  delete cfgTagFunctions["window_number_of_row"];
  delete cfgTagFunctions["window_selected_functions"];
  delete cfgTagFunctions["window_semantic_module"];
  delete cfgTagFunctions["window_compose_functions"];

  // Filter options
  delete cfgTagFunctions["window_filter_module"];
  delete cfgTagFunctions["window_logical_filtered"];
  delete cfgTagFunctions["window_physical_filtered"];
  delete cfgTagFunctions["window_fromto"];
  delete cfgTagFunctions["window_comm_tagsize"];
  delete cfgTagFunctions["window_typeval"];

  // Histogram
  delete cfgTagFunctions["< NEW ANALYZER2D >"];
  delete cfgTagFunctions["Analyzer2D.ControlWindow:"];
  delete cfgTagFunctions["Analyzer2D.DataWindow:"];
  delete cfgTagFunctions["Analyzer2D.Statistic:"];
  delete cfgTagFunctions["Analyzer2D.calculate_all:"];
  delete cfgTagFunctions["Analyzer2D.num_columns:"];
  delete cfgTagFunctions["Analyzer2D.HideCols:"];
  delete cfgTagFunctions["Analyzer2D.scientific_notation:"];
  delete cfgTagFunctions["Analyzer2D.num_decimals:"];
  delete cfgTagFunctions["Analyzer2D.thousandsep:"];
  delete cfgTagFunctions["Analyzer2D.units:"];
  delete cfgTagFunctions["Analyzer2D.HorizVert:"];
  delete cfgTagFunctions["Analyzer2D.Accumulator:"];
  delete cfgTagFunctions["Analyzer2D.AccumulateByControlWindow:"];
  delete cfgTagFunctions["Analyzer2D.SortCols:"];
  delete cfgTagFunctions["Analyzer2D.SortCriteria:"];
  delete cfgTagFunctions["Analyzer2D.Parameters:"];
  delete cfgTagFunctions["Analyzer2D.AnalysisLimits:"];
  delete cfgTagFunctions["Analyzer2D.RelativeTime:"];
  delete cfgTagFunctions["Analyzer2D.ComputeYScale:"];
  delete cfgTagFunctions["Analyzer2D.Minimum:"];
  delete cfgTagFunctions["Analyzer2D.Maximum:"];
  delete cfgTagFunctions["Analyzer2D.Delta:"];
  delete cfgTagFunctions["Analyzer2D.ComputeGradient:"];
  delete cfgTagFunctions["Analyzer2D.MinimumGradient:"];
  delete cfgTagFunctions["Analyzer2D.MaximumGradient:"];

  // 3D Histogram
  delete cfgTagFunctions["Analyzer2D.3D_ControlWindow:"];
  delete cfgTagFunctions["Analyzer2D.3D_Minimum:"];
  delete cfgTagFunctions["Analyzer2D.3D_Maximum:"];
  delete cfgTagFunctions["Analyzer2D.3D_Delta:"];
  delete cfgTagFunctions["Analyzer2D.3D_FixedValue:"];
}


bool WindowType::parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<KWindow *>& windows,
                            Histogram *histogram,
                            TRecordTime& beginTime, TRecordTime& endTime )
{
  string type;
  KWindow *tmpWin;

  getline( line, type, ' ' );
  if ( type.compare( "single" ) == 0 )
  {
    tmpWin = new KSingleWindow( whichTrace );
  }
  else if ( type.compare( "composed" ) == 0 )
  {
    tmpWin = new KDerivedWindow( );
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
                               vector<KWindow *>& windows,
                               Histogram *histogram,
                               TRecordTime& beginTime, TRecordTime& endTime )
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

    ( ( KDerivedWindow * ) windows[ windows.size() - 1 ] )->setFactor( numFactor,
        factorValue );

    numFactor++;
  }
  return true;
}


bool WindowUnits::parseLine( KernelConnection *whichKernel, istringstream& line,
                             Trace *whichTrace,
                             vector<KWindow *>& windows,
                             Histogram *histogram,
                             TRecordTime& beginTime, TRecordTime& endTime )
{
  string strUnits;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strUnits, ' ' );

  if ( strUnits.compare( "Nanoseconds" ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( NS );
  else if ( strUnits.compare( "Microseconds" ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( US );
  else if ( strUnits.compare( "Milliseconds" ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( MS );
  else if ( strUnits.compare( "Seconds" ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( SEC );
  else if ( strUnits.compare( "Minutes" ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( MIN );
  else if ( strUnits.compare( "Hours" ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( HOUR );
  else if ( strUnits.compare( "Days" ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( DAY );

  return true;
}

bool WindowOperation::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<KWindow *>& windows,
                                 Histogram *histogram,
                                 TRecordTime& beginTime, TRecordTime& endTime )
{
  string strFunction;
  SemanticFunction *function;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( !windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strFunction, ' ' );
  function = ( FunctionManagement<SemanticFunction>::getInstance() )->getFunction( strFunction );
//  function = SemanticManagement::createFunction( strFunction );
  if ( function == NULL )
    return false;

  ( ( KDerivedWindow * ) windows[ windows.size() - 1 ] )->setLevelFunction( DERIVED, function );

  return true;
}

// For representation purposes.
bool WindowMaximumY::parseLine( KernelConnection *whichKernel, istringstream& line,
                                Trace *whichTrace,
                                vector<KWindow *>& windows,
                                Histogram *histogram,
                                TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}


bool WindowLevel::parseLine( KernelConnection *whichKernel, istringstream& line,
                             Trace *whichTrace,
                             vector<KWindow *>& windows,
                             Histogram *histogram,
                             TRecordTime& beginTime, TRecordTime& endTime )
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
                                   vector<KWindow *>& windows,
                                   Histogram *histogram,
                                   TRecordTime& beginTime, TRecordTime& endTime )
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

    ( ( KDerivedWindow * ) windows[ windows.size() - 1 ] )->setParent( numID,
        windows[ id - 1 ] );
    numID++;
  }

  return true;
}

bool WindowScaleRelative::parseLine( KernelConnection *whichKernel, istringstream& line,
                                     Trace *whichTrace,
                                     vector<KWindow *>& windows,
                                     Histogram *histogram,
                                     TRecordTime& beginTime, TRecordTime& endTime )
{
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  endTime = ( windows[ windows.size() - 1 ]->getTrace() )->getEndTime();

  return true;
}

bool WindowObject::parseLine( KernelConnection *whichKernel, istringstream& line, Trace *whichTrace,
                              vector<KWindow *>& windows,
                              Histogram *histogram,
                              TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}

bool WindowBeginTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<KWindow *>& windows,
                                 Histogram *histogram,
                                 TRecordTime& beginTime, TRecordTime& endTime )
{
  string strTime;
  TRecordTime auxTime;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strTime, ' ' );
  istringstream tmpTime( strTime );

  if ( !( tmpTime >> auxTime ) )
    return false;

  beginTime = auxTime;

  return true;
}

bool WindowEndTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                               Trace *whichTrace,
                               vector<KWindow *>& windows,
                               Histogram *histogram,
                               TRecordTime& beginTime, TRecordTime& endTime )
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
                                vector<KWindow *>& windows,
                                Histogram *histogram,
                                TRecordTime& beginTime, TRecordTime& endTime )
{
  string strTime;
  TRecordTime auxTime;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strTime, ' ' );
  istringstream tmpTime( strTime );

  if ( !( tmpTime >> auxTime ) )
    return false;

  endTime = auxTime;

  return true;
}

bool WindowBeginTimeRelative::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strPercentage;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strPercentage, ' ' );
  istringstream tmpStream( strPercentage );
  TSemanticValue percentage;

  if ( !( tmpStream >> percentage ) )
    return false;

  beginTime = ( windows[ windows.size() - 1 ]->getTrace() )->getEndTime() *
              percentage;

  return true;
}

bool WindowNumberOfRow::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<KWindow *>& windows,
                                   Histogram *histogram,
                                   TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}

bool WindowSelectedFunctions::parseLine( KernelConnection *whichKernel, istringstream& line, Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
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
//      function = SemanticManagement::createFunction( strFunction );
      if ( function == NULL )
        return false;
      windows[ windows.size() - 1 ]->setLevelFunction( level, function );
    }
    // It's a filter function
    else
    {
      FilterFunction *function;
      Filter *filter = ( ( KSingleWindow * ) windows[ windows.size() - 1 ] )->getFilter();


      function = ( FunctionManagement<FilterFunction>::getInstance() )->getFunction( strFunction );
//      function = FilterManagement::createFunction( strFunction );
      if ( function == NULL )
        return false;

      if ( strLevel.compare( "from_obj" ) == 0 )
        filter->setCommFromFunction( function );
      else if ( strLevel.compare( "to_obj" ) == 0 )
        filter->setCommToFunction( function );
      else if ( strLevel.compare( "tag_msg" ) == 0 )
        filter->setCommTagFunction( function );
      else if ( strLevel.compare( "size_msg" ) == 0 )
        filter->setCommSizeFunction( function );
      else if ( strLevel.compare( "bw_msg" ) == 0 )
        filter->setBandWidthFunction( function );
      else if ( strLevel.compare( "evt_type" ) == 0 )
        filter->setEventTypeFunction( function );
      else if ( strLevel.compare( "evt_value" ) == 0 )
        filter->setEventValueFunction( function );
    }
  }

  return true;
}


bool WindowComposeFunctions::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<KWindow *>& windows,
                                        Histogram *histogram,
                                        TRecordTime& beginTime, TRecordTime& endTime )
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
//      function = SemanticManagement::createFunction( strFunction );
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
                                      vector<KWindow *>& windows,
                                      Histogram *histogram,
                                      TRecordTime& beginTime, TRecordTime& endTime )
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
//  function = SemanticManagement::createFunction( strFunction );
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
                                    vector<KWindow *>& windows,
                                    Histogram *histogram,
                                    TRecordTime& beginTime, TRecordTime& endTime )
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

  filter = ( ( KSingleWindow * )windows[ windows.size() - 1 ] )->getFilter();

  for ( UINT16 ii = 0; ii < numParams; ii++ )
  {
    if ( strTag.compare( "from_obj" ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> fromObject ) )
        return false;

      filter->insertCommFrom( fromObject );
    }
    else if ( strTag.compare( "to_obj" ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> toObject ) )
        return false;

      filter->insertCommTo( toObject );
    }
    else if ( strTag.compare( "tag_msg" ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> commTag ) )
        return false;

      filter->insertCommTag( commTag );
    }
    else if ( strTag.compare( "size_msg" ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> commSize ) )
        return false;

      filter->insertCommSize( commSize );
    }
    else if ( strTag.compare( "bw_msg" ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> bandWidth ) )
        return false;

      filter->insertBandWitdh( bandWidth );
    }
    else if ( strTag.compare( "evt_type" ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> eventType ) )
        return false;

      filter->insertEventType( eventType );
    }
    else if ( strTag.compare( "evt_value" ) == 0 )
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
                                     vector<KWindow *>& windows,
                                     Histogram *histogram,
                                     TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = ( ( KSingleWindow * )windows[ windows.size() - 1 ] )->getFilter();

  if ( strBool.compare( "false" ) == 0 )
    filter->setLogical( false );
  else if ( strBool.compare( "true" ) == 0 )
    filter->setLogical( true );
  else
    return false;

  return true;
}


bool WindowFilterPhysical::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      Histogram *histogram,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = ( ( KSingleWindow * )windows[ windows.size() - 1 ] )->getFilter();

  if ( strBool.compare( "false" ) == 0 )
    filter->setPhysical( false );
  else if ( strBool.compare( "true" ) == 0 )
    filter->setPhysical( true );
  else
    return false;

  return true;
}


bool WindowFilterBoolOpFromTo::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  filter = ( ( KSingleWindow * )windows[ windows.size() - 1 ] )->getFilter();

  getline( line, strBool, ' ' );

  if ( strBool.compare( "false" ) == 0 )
    filter->setOpFromToOr();
  else if ( strBool.compare( "true" ) == 0 )
    filter->setOpFromToAnd();
  else
    return false;

  return true;
}


bool WindowFilterBoolOpTagSize::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = ( ( KSingleWindow * )windows[ windows.size() - 1 ] )->getFilter();

  if ( strBool.compare( "false" ) == 0 )
    filter->setOpTagSizeOr();
  else if ( strBool.compare( "true" ) == 0 )
    filter->setOpTagSizeAnd();
  else
    return false;

  return true;
}


bool WindowFilterBoolOpTypeVal::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = ( ( KSingleWindow * )windows[ windows.size() - 1 ] )->getFilter();

  if ( strBool.compare( "false" ) == 0 )
    filter->setOpTypeValueOr();
  else if ( strBool.compare( "true" ) == 0 )
    filter->setOpTypeValueAnd();
  else
    return false;

  return true;
}

bool Analyzer2DCreate::parseLine( KernelConnection *whichKernel, istringstream& line,
                                  Trace *whichTrace,
                                  vector<KWindow *>& windows,
                                  Histogram *histogram,
                                  TRecordTime& beginTime, TRecordTime& endTime )
{
  if ( histogram != NULL )
    histogram = Histogram::create( whichKernel );

  return true;
}

bool Analyzer2DControlWindow::parseLine( KernelConnection *whichKernel, istringstream& line, Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strIndexControlWindow;
  UINT32 indexControlWindow;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strIndexControlWindow );
  istringstream tmpWindow( strIndexControlWindow );
  if ( !( tmpWindow >> indexControlWindow ) )
    return false;

  if ( indexControlWindow >= windows.size() )
    return false;

  histogram->setControlWindow( windows[ indexControlWindow ] );

  return true;
}


bool Analyzer2DDataWindow::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      Histogram *histogram,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  string strIndexDataWindow;
  UINT32 indexDataWindow;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strIndexDataWindow );
  istringstream tmpWindow( strIndexDataWindow );
  if ( !( tmpWindow >> indexDataWindow ) )
    return false;

  if ( indexDataWindow >= windows.size() )
    return false;

  // get histogram

  histogram->setDataWindow( windows[ indexDataWindow ] );

  return true;
}


bool Analyzer2DStatistic::parseLine( KernelConnection *whichKernel, istringstream& line,
                                     Trace *whichTrace,
                                     vector<KWindow *>& windows,
                                     Histogram *histogram,
                                     TRecordTime& beginTime, TRecordTime& endTime )
{
  string strStatistic;
  HistogramStatistic *statistic;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strStatistic );
  statistic = ( FunctionManagement<HistogramStatistic>::getInstance() )->getFunction( strStatistic );
  if ( statistic == NULL )
    return false;

  // get histogram

  histogram->pushbackStatistic( statistic );

  return true;
}


bool Analyzer2DCalculateAll::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<KWindow *>& windows,
                                        Histogram *histogram,
                                        TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBoolAll;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strBoolAll, ' ' );

  if ( strBoolAll.compare( "True" ) == 0 )
  {
    // get histogram
    histogram->clearStatistics();

  }
  else if ( strBoolAll.compare( "False" ) == 0 )
    return true;
  else
    return false;


  return true;
}


bool Analyzer2DNumColumns::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      Histogram *histogram,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  return true;
}


bool Analyzer2DHideColumns::parseLine( KernelConnection *whichKernel, istringstream& line,
                                       Trace *whichTrace,
                                       vector<KWindow *>& windows,
                                       Histogram *histogram,
                                       TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( "True" ) == 0 )
    histogram->setHideColumns( true );
  else if ( strBool.compare( "False" ) == 0 )
    histogram->setHideColumns( false );
  else
    return false;

  return true;
}



bool Analyzer2DScientificNotation::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  return true;
}


bool Analyzer2DNumDecimals::parseLine( KernelConnection *whichKernel, istringstream& line,
                                       Trace *whichTrace,
                                       vector<KWindow *>& windows,
                                       Histogram *histogram,
                                       TRecordTime& beginTime, TRecordTime& endTime )
{
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  return true;
}


bool Analyzer2DThousandSeparator::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  return true;
}


bool Analyzer2DUnits::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<KWindow *>& windows,
                                 Histogram *histogram,
                                 TRecordTime& beginTime, TRecordTime& endTime )
{
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  return true;
}


bool Analyzer2DHorizontal::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      Histogram *histogram,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( "True" ) == 0 )
    histogram->setHorizontal( true );
  else if ( strBool.compare( "False" ) == 0 )
    histogram->setHorizontal( false );
  else
    return false;

  return true;
}


bool Analyzer2DAccumulator:: parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<KWindow *>& windows,
                                        Histogram *histogram,
                                        TRecordTime& beginTime, TRecordTime& endTime )
{
  string strAccumulator;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strAccumulator );

  return false;
}


bool Analyzer2DAccumulateByControlWindow::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( "False" ) == 0 )
    {}
  else if ( strBool.compare( "True" ) == 0 )
    {}
  else
    return false;

  return true;
}


bool Analyzer2DSortCols::parseLine( KernelConnection *whichKernel, istringstream& line,
                                    Trace *whichTrace,
                                    vector<KWindow *>& windows,
                                    Histogram *histogram,
                                    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( "False" ) == 0 )
    {}
  else if ( strBool.compare( "True" ) == 0 )
    {}
  else
    return false;

  return true;
}


bool Analyzer2DSortCriteria::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<KWindow *>& windows,
                                        Histogram *histogram,
                                        TRecordTime& beginTime, TRecordTime& endTime )
{
  string strSortCriteria;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strSortCriteria );

  if ( strSortCriteria.compare( "Average" ) == 0 )
    {}
  else if ( strSortCriteria.compare( "Total" ) == 0 )
    {}
  else if ( strSortCriteria.compare( "Maximum" ) == 0 )
    {}
  else if ( strSortCriteria.compare( "Minimum" ) == 0 )
    {}
  else if ( strSortCriteria.compare( "Stdev" ) == 0 )
    {}
  else
    return false;

  return true;
}

/*
 Number_of_parameters Parameter1 ... ParameterN
*/
bool Analyzer2DParameters::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      Histogram *histogram,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  string strNumParams, strValue;
  UINT16 numParams;
  float dataValue;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
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

  }

  return false;
}


bool Analyzer2DAnalysisLimits::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strLimit;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strLimit, ' ' );

  if ( strLimit.compare( "Alltrace" ) == 0 )
    {}
  else if ( strLimit.compare( "Allwindow" ) == 0 )
    {}
  else if ( strLimit.compare( "Region" ) == 0 )
    {}
  else
    return false;

  return true;
}


bool Analyzer2DRelativeTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<KWindow *>& windows,
                                        Histogram *histogram,
                                        TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( "False" ) == 0 )
    {}
  else if ( strBool.compare( "True" ) == 0 )
    {}
  else
    return false;

  return true;
}


bool Analyzer2DComputeYScale::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( "False" ) == 0 )
    {}
  else if ( strBool.compare( "True" ) == 0 )
    {}
  else
    return false;

  return true;
}

bool Analyzer2DMinimum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<KWindow *>& windows,
                                   Histogram *histogram,
                                   TRecordTime& beginTime, TRecordTime& endTime )
{
  string strMinimum;
  THistogramLimit dataMinimum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strMinimum );
  istringstream tmpValue( strMinimum );
  if ( !( tmpValue >> dataMinimum ) )
    return false;

  return true;
}


bool Analyzer2DMaximum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<KWindow *>& windows,
                                   Histogram *histogram,
                                   TRecordTime& beginTime, TRecordTime& endTime )
{
  string strMaximum;
  THistogramLimit dataMaximum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strMaximum );
  istringstream tmpValue( strMaximum );
  if ( !( tmpValue >> dataMaximum ) )
    return false;

  return true;
}


bool Analyzer2DDelta::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<KWindow *>& windows,
                                 Histogram *histogram,
                                 TRecordTime& beginTime, TRecordTime& endTime )
{
  string strDelta;
  THistogramLimit dataDelta;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strDelta );
  istringstream tmpValue( strDelta );
  if ( !( tmpValue >> dataDelta ) )
    return false;

  return true;
}


bool Analyzer2DComputeGradient::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( "False" ) == 0 )
    {}
  else if ( strBool.compare( "True" ) == 0 )
    {}
  else
    return false;

  return true;
}


bool Analyzer2DMinimumGradient::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strMinimumGradient;
  THistogramLimit dataMinimumGradient;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strMinimumGradient );
  istringstream tmpValue( strMinimumGradient );
  if ( !( tmpValue >> dataMinimumGradient ) )
    return false;

  return true;
}


bool Analyzer2DMaximumGradient::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strMaximumGradient;
  THistogramLimit dataMaximumGradient;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, strMaximumGradient );
  istringstream tmpValue( strMaximumGradient );
  if ( !( tmpValue >> dataMaximumGradient ) )
    return false;

  return true;
}


bool Analyzer3DControlWindow::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<KWindow *>& windows,
    Histogram *histogram,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string str3DControlWindow;
  UINT32 controlWindow;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, str3DControlWindow );
  istringstream tmpValue( str3DControlWindow );
  if ( !( tmpValue >> controlWindow ) )
    return false;

  return true;
}


bool Analyzer3DMinimum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<KWindow *>& windows,
                                   Histogram *histogram,
                                   TRecordTime& beginTime, TRecordTime& endTime )
{
  string str3DMinimum;
  THistogramLimit data3DMinimum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, str3DMinimum );
  istringstream tmpValue( str3DMinimum );
  if ( !( tmpValue >> data3DMinimum ) )
    return false;

  return true;
}


bool Analyzer3DMaximum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<KWindow *>& windows,
                                   Histogram *histogram,
                                   TRecordTime& beginTime, TRecordTime& endTime )
{
  string str3DMaximum;
  THistogramLimit data3DMaximum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, str3DMaximum );
  istringstream tmpValue( str3DMaximum );
  if ( !( tmpValue >> data3DMaximum ) )
    return false;

  return true;
}


bool Analyzer3DDelta::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<KWindow *>& windows,
                                 Histogram *histogram,
                                 TRecordTime& beginTime, TRecordTime& endTime )
{
  string str3DDelta;
  THistogramLimit data3DDelta;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, str3DDelta );
  istringstream tmpValue( str3DDelta );
  if ( !( tmpValue >> data3DDelta ) )
    return false;

  return true;
}


bool Analyzer3DFixedValue::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      Histogram *histogram,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  string str3DFixedValue;
  UINT32 data3DFixedValue;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if( histogram == NULL )
    return false;

  getline( line, str3DFixedValue );
  istringstream tmpValue( str3DFixedValue );
  if ( !( tmpValue >> data3DFixedValue ) )
    return false;

  return true;
}
