#include <string>
#include <fstream>
#include <sstream>

#include "cfg.h"
#include "kwindow.h"
#include "trace.h"
#include "semanticmanagement.h"


using namespace std;

bool CFGLoader::mapLoaded = false;
map<string, TagFunction *> CFGLoader::cfgTagFunctions;

bool CFGLoader::loadCFG( string& filename, Trace *whichTrace, vector<KWindow *>& windows,
                         TRecordTime& beginTime, TRecordTime& endTime )
{
  if ( !mapLoaded )
    loadMap();

  ifstream cfgFile( filename.c_str() );
  string strLine;

  windows.push_back( NULL );

  while ( !cfgFile.eof() )
  {
    string cfgTag;

    getline( cfgFile, strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' || strLine[ 0 ] == '<' )
      continue;

    istringstream auxStream( strLine );
    getline( auxStream, cfgTag, ' ' );


    cout << cfgTag << endl;

    map<string, TagFunction *>::iterator it = cfgTagFunctions.find( cfgTag );

    if ( it != cfgTagFunctions.end() )
    {
      if ( !it->second->parseLine( auxStream, whichTrace, windows,
                                   beginTime, endTime ) )
      {
        if ( windows[ windows.size() - 1 ] == NULL )
          delete windows[ windows.size() - 1 ];
        windows[ windows.size() - 1 ] = NULL;
      }
    }
  }

  cfgFile.close();

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
  cfgTagFunctions["window_begin_time_relative"] = new WindowBeginTimeRelative();
  cfgTagFunctions["window_number_of_row"]       = new WindowNumberOfRow();
  cfgTagFunctions["window_selected_functions"]  = new WindowSelectedFunctions();
  cfgTagFunctions["window_semantic_module"]     = new WindowSemanticModule();

  // Filter options
  cfgTagFunctions["window_filter_module"]       = new WindowFilterModule();
  cfgTagFunctions["window_logical_filtered"]    = new WindowFilterLogical();
  cfgTagFunctions["window_physical_filtered"]   = new WindowFilterPhysical();
  cfgTagFunctions["window_fromto"]              = new WindowFilterBoolOpFromTo();
  cfgTagFunctions["window_comm_tagsize"]        = new WindowFilterBoolOpTagSize();
  cfgTagFunctions["window_typeval"]             = new WindowFilterBoolOpTypeVal();

  mapLoaded = true;
}


bool WindowType::parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
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

bool WindowFactors::parseLine( istringstream& line, Trace *whichTrace,
                               vector<KWindow *>& windows,
                               TRecordTime& beginTime, TRecordTime& endTime )
{
  string strFactor;
  UINT8 numFactor = 0;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  while ( !line.eof() )
  {
    getline( line, strFactor, ' ' );
    istringstream tmpStream( strFactor );
    TSemanticValue factorValue;

    if ( !( tmpStream >> factorValue ) )
      return false;

    ( ( KDerivedWindow * ) windows[ windows.size() - 1 ] )->setFactor( numFactor,
        factorValue );
  }
  return true;
}


bool WindowUnits::parseLine( istringstream& line, Trace *whichTrace,
                             vector<KWindow *>& windows,
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

bool WindowOperation::parseLine( istringstream& line, Trace *whichTrace,
                                 vector<KWindow *>& windows,
                                 TRecordTime& beginTime, TRecordTime& endTime )
{
  string strFunction;
  SemanticFunction *function;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strFunction, ' ' );
  function = SemanticManagement::createFunction( strFunction );
  if ( function == NULL )
    return false;

  windows[ windows.size() - 1 ]->setLevelFunction( DERIVED, function );

  return true;
}

// For representation purposes.
bool WindowMaximumY::parseLine( istringstream& line, Trace *whichTrace,
                                vector<KWindow *>& windows,
                                TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}


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

  return level;
}

bool WindowLevel::parseLine( istringstream& line, Trace *whichTrace,
                             vector<KWindow *>& windows,
                             TRecordTime& beginTime, TRecordTime& endTime )
{
  string strLevel;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strLevel, ' ' );

  windows[ windows.size() - 1 ]->setLevel( stringToLevel( strLevel ) );

  return true;
}

bool WindowScaleRelative::parseLine( istringstream& line, Trace *whichTrace,
                                     vector<KWindow *>& windows,
                                     TRecordTime& beginTime, TRecordTime& endTime )
{
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  endTime = ( windows[ windows.size() - 1 ]->getTrace() )->getEndTime();

  return true;
}

//
bool WindowObject::parseLine( istringstream& line, Trace *whichTrace,
                              vector<KWindow *>& windows,
                              TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}

bool WindowBeginTimeRelative::parseLine( istringstream& line, Trace *whichTrace,
    vector<KWindow *>& windows,
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

bool WindowNumberOfRow::parseLine( istringstream& line, Trace *whichTrace,
                                   vector<KWindow *>& windows,
                                   TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}

bool WindowSelectedFunctions::parseLine( istringstream& line, Trace *whichTrace,
    vector<KWindow *>& windows,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string tmpString;
  string strNumFunctions;
  UINT8 numFunctions;
  string strLevel;
  TWindowLevel level;
  string strFunction;
  SemanticFunction *function;

  getline( line, tmpString, ' ' );
  getline( line, strNumFunctions, ',' );
  istringstream tmpNumFunctions( strNumFunctions );

  if( !( tmpNumFunctions >> numFunctions ) )
    return false;

  getline( line, tmpString, '{' );
  for( UINT8 i = 0; i < numFunctions; i++ )
  {
    getline( line, tmpString, '{' );
    getline( line, strLevel, ',' );
    getline( line, strFunction, '}' );
    level = stringToLevel( strLevel );

    // It's a semantic function
    if( level != NONE )
    {
      function = SemanticManagement::createFunction( strFunction );
      if( function == NULL )
        return false;
      windows[ windows.size() - 1 ]->setLevelFunction( level, function );
    }
    // It's a filter function
    else
    {

    }
  }

  return true;
}

// si
bool WindowSemanticModule::parseLine( istringstream& line, Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}


bool WindowFilterModule::parseLine( istringstream& line, Trace *whichTrace,
                                    vector<KWindow *>& windows,
                                    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strTag;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strTag, ' ' );

  if ( strTag.compare( "from_obj" ) == 0 )
  {}
  else if ( strTag.compare( "to_obj" ) == 0 )
  {}
  else if ( strTag.compare( "tag_msg" ) == 0 )
  {}
  else if ( strTag.compare( "size_msg" ) == 0 )
  {}
  else if ( strTag.compare( "bw_msg" ) == 0 )
  {}
  else if ( strTag.compare( "evt_type" ) == 0 )
  {}
  else if ( strTag.compare( "evt_value" ) == 0 )
  {}


  return true;
}


bool WindowFilterLogical::parseLine( istringstream& line, Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( "false" ) == 0 )
  {}

  return true;
}
bool WindowFilterPhysical::parseLine( istringstream& line, Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}
bool WindowFilterBoolOpFromTo::parseLine( istringstream& line, Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}
bool WindowFilterBoolOpTagSize::parseLine( istringstream& line, Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}
bool WindowFilterBoolOpTypeVal::parseLine( istringstream& line, Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}
