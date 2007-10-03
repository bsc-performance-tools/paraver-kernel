#include <string>
#include <fstream>
#include <sstream>

#include "cfg.h"
#include "kwindow.h"
#include "trace.h"
#include "semanticmanagement.h"
#include "filtermanagement.h"


using namespace std;

bool CFGLoader::mapLoaded = false;
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

bool WindowObject::parseLine( istringstream& line, Trace *whichTrace,
                              vector<KWindow *>& windows,
                              TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}

bool WindowBeginTime::parseLine( istringstream& line, Trace *whichTrace,
                                 vector<KWindow *>& windows,
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

bool WindowEndTime::parseLine( istringstream& line, Trace *whichTrace,
                               vector<KWindow *>& windows,
                               TRecordTime& beginTime, TRecordTime& endTime )
{
  return true;
}

bool WindowStopTime::parseLine( istringstream& line, Trace *whichTrace,
                                vector<KWindow *>& windows,
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

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, tmpString, ' ' );
  getline( line, strNumFunctions, ',' );
  istringstream tmpNumFunctions( strNumFunctions );

  if ( !( tmpNumFunctions >> numFunctions ) )
    return false;

  getline( line, tmpString, '{' );
  for ( UINT8 i = 0; i < numFunctions; i++ )
  {
    getline( line, tmpString, '{' );
    getline( line, strLevel, ',' );
    getline( line, strFunction, '}' );
    level = stringToLevel( strLevel );

    // It's a semantic function
    if ( level != NONE )
    {
      SemanticFunction *function;

      function = SemanticManagement::createFunction( strFunction );
      if ( function == NULL )
        return false;
      windows[ windows.size() - 1 ]->setLevelFunction( level, function );
    }
    // It's a filter function
    else
    {
      FilterFunction *function;
      Filter *filter = ( ( KSingleWindow * ) windows[ windows.size() - 1 ] )->getFilter();

      function = FilterManagement::createFunction( strFunction );
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


bool WindowComposeFunctions::parseLine( istringstream& line, Trace *whichTrace,
                                        vector<KWindow *>& windows,
                                        TRecordTime& beginTime, TRecordTime& endTime )
{
  string tmpString;
  string strNumFunctions;
  UINT8 numFunctions;
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
  for ( UINT8 i = 0; i < numFunctions; i++ )
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

      function = SemanticManagement::createFunction( strFunction );
      if ( function == NULL )
        return false;
      windows[ windows.size() - 1 ]->setLevelFunction( level, function );
    }
    else
      return false;
  }

  return true;
}


bool WindowSemanticModule::parseLine( istringstream& line, Trace *whichTrace,
                                      vector<KWindow *>& windows,
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

  getline( line, strFunction, ' ' );
  function = SemanticManagement::createFunction( strFunction );
  if ( function == NULL )
    return false;

  if ( typeid( windows[ windows.size() - 1 ]->getLevelFunction( level ) ) ==
       typeid( function ) )
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

  return true;
}


bool WindowFilterModule::parseLine( istringstream& line, Trace *whichTrace,
                                    vector<KWindow *>& windows,
                                    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strTag, strNumberParams, strValue;
  UINT8 numParams;
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

  getline( line, strTag, ' ' );          // Parameter type.
  getline( line, strNumberParams, ' ' ); // Number of following parameters.
  istringstream tmpNumberParams( strNumberParams );

  if ( !( tmpNumberParams >> numParams ) )
    return false;

  filter = ( ( KSingleWindow * )windows[ windows.size() - 1 ] )->getFilter();

  for ( UINT8 ii = 0; ii < numParams; ii++ )
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


bool WindowFilterLogical::parseLine( istringstream& line, Trace *whichTrace,
                                     vector<KWindow *>& windows,
                                     TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

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


bool WindowFilterPhysical::parseLine( istringstream& line, Trace *whichTrace,
                                      vector<KWindow *>& windows,
                                      TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

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


bool WindowFilterBoolOpFromTo::parseLine( istringstream& line, Trace *whichTrace,
    vector<KWindow *>& windows,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( "false" ) == 0 )
    {}
  else if ( strBool.compare( "true" ) == 0 )
    {}
  else
    return false;

  return true;
}


bool WindowFilterBoolOpTagSize::parseLine( istringstream& line, Trace *whichTrace,
    vector<KWindow *>& windows,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

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


bool WindowFilterBoolOpTypeVal::parseLine( istringstream& line, Trace *whichTrace,
    vector<KWindow *>& windows,
    TRecordTime& beginTime, TRecordTime& endTime )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

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
