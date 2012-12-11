/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                                  otf2prv                                  *
 *             Trace translator from OTF2 format to PRV format.              *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <map>
#include <wordexp.h>


using namespace std;

const string OTF2_VERSION_STRING = "0.29"; // added -t table


#include <scorep_utility/SCOREP_UtilityTypes.h>
#include <otf2/otf2.h>
#include "otf2prv.h"

#include "prvtypes.h"
#include "localkernel.h"
#include "paraverconfig.h"
#include "trace.h"
#include "labelconstructor.h" // for getDate
#include "paraverlabels.h"
#include "resourcemodel.h"
#include "processmodel.h"

#include "event_list.h"
#include "event_description.h"
#include "events.h"

class State
{
  public:
    State() {}
    State( prvTime_t whichTime, prvState_t whichState ) :
           beginTime( whichTime ), state( whichState ) {}
    ~State() {}

    prvTime_t getBeginTime() const { return beginTime; }
    void setBeginTime( prvTime_t whichTime ) { beginTime = whichTime; }

    prvState_t getState() const { return state; }
    void setState( prvState_t whichState ) { state = whichState; }

  protected:
    prvTime_t beginTime;
    prvState_t state;

  private:

};

typedef struct MainOptionsStruct MainOptions;

struct MainOptionsStruct
{
  bool showHelp;
  bool showVersion;
  bool printExternalTableExample;
};


struct LocalDefClockOffsetStruct
{
  uint64_t time;
  int64_t  offset;
  double   stddev;
};

typedef struct LocalDefClockOffsetStruct LocalDefClockOffsetData;

class TranslationLog
{
  public:
    TranslationLog();
    ~TranslationLog() {};

    void enableLog( bool whichLogEnabled );

    void write( const string & message );
    void write( const string & message, const string & value );
    void write( const string & message, const uint64_t & value );
    void write( const string & message, const int64_t & value );
    void write( const string & message, const double & value );
    void write( const string & message, const uint32_t & value );
    void write( const string & message, const uint32_t & value1, const uint32_t & value2 );
    void write( const string & message, const uint64_t & value1, const uint32_t & value2 );
    void write( const string & message, const uint64_t & value1, const uint64_t & value2 );
    void write( const string & message1, const string & message2, const uint32_t & value );

  private:  
    bool logEnabled;
    std::ostream *logFile;
};



class TranslationData
{
  public:

  OTF2_Reader *reader;
  std::fstream *PRVFile;
  string strOTF2Trace;

  TranslationLog myLog;
  bool useExternalTranslationTable;

  TTimeUnit timeUnit;
  // PRV_UINT64 timerResolution;
  PRV_UINT64 maxTraceTime;
  PRV_UINT64 globalOffset;
  bool correctedBeginTime;

  ResourceModel *resourcesModel; // to the Trace*
  ProcessModel  *processModel;   // to the Trace*
  RowLabels     *rowLabels;      // to the Trace*
  Trace         *trace;

  // PRV LOCAL MAPS --> TO THE API
  map< string, int > PRVEvent_ValueLabel2Value; //
  map< string, int > PRVEvent_TypeLabel2Type;   //
  //map< int, int >    PRVEvent_Value2Type;     // N - 1
  map< string, int >    PRVEvent_ValueLabel2Type;     // N - 1

  map< uint32_t, int > OTF2Region2PRVEventValue;
  map< uint32_t, int > OTF2Region2PRVEventType;

  map< uint32_t, uint32_t > regionName;// RegionId -> NameID
  map< string, uint32_t > regionIdent;  // Name -> regionI

  // Keep OTF2 definitions
  map< uint32_t, string >     symbols;
  map< uint32_t, uint32_t >   locationGroup2SystemTreeNode;
  map< uint32_t, TNodeOrder > systemTreeNode2GlobalNode;

  // Once dumpTrace is done, this maps can be deleted.
  // Here only for cout purposes.
  map< uint64_t, TTaskOrder >   location2Task;
  map< uint64_t, TThreadOrder > location2Thread;
  map< uint64_t, TCPUOrder >    location2CPU;

  uint64_t currentLocation;
  map< uint64_t, LocalDefClockOffsetData > localDefClockOffset;

  //map< uint32_t, OTF2_TypeID > attributeType;
  map< uint32_t, string > attributeName;

  // States generation
  map< TThreadOrder, stack< State > > stateStack;
};


TranslationLog::TranslationLog()
{
  logEnabled = false;
  logFile = &std::cout;
}


void TranslationLog::enableLog( bool whichLogEnabled )
{
  logEnabled = whichLogEnabled;
}


void TranslationLog::write( const string & message )
{
  if ( logEnabled )
  {
    (*logFile) << message << std::endl;
  }
}


void TranslationLog::write( const string & message, const string & value )
{
  if ( logEnabled )
  {
    (*logFile) << message << ": " << value << std::endl;
  }
}


void TranslationLog::write( const string & message, const uint64_t & value )
{
  if ( logEnabled )
  {
    stringstream aux;
    aux << value;
    (*logFile) << message << ": " << aux.str() << std::endl;
  }
}


void TranslationLog::write( const string & message, const int64_t & value )
{
  if ( logEnabled )
  {
    stringstream aux;
    aux << value;
    (*logFile) << message << ": " << aux.str() << std::endl;
  }
}


void TranslationLog::write( const string & message, const double & value )
{
  if ( logEnabled )
  {
    stringstream aux;
    aux << fixed;
    aux.precision( 4 );
    aux << value;
    (*logFile) << message << ": " << aux.str() << std::endl;
  }
}


void TranslationLog::write( const string & message, const uint32_t & value )
{
  if ( logEnabled )
  {
    stringstream aux;
    aux << value;
    (*logFile) << message << ": " << aux.str() << std::endl;
  }
}


void TranslationLog::write( const string & message, const uint32_t & value1, const uint32_t & value2 )
{
  if ( logEnabled )
  {
    (*logFile) << message << ": ";

    stringstream aux;
    aux << value1;
    (*logFile) << aux.str() << " - ";

    stringstream aux2;
    aux2.clear();
    aux2 << value2;
    (*logFile) << aux2.str() << std::endl;
  }
}


void TranslationLog::write( const string & message, const uint64_t & value1, const uint32_t & value2 )
{
  if ( logEnabled )
  {
    (*logFile) << message << ": ";

    stringstream aux;
    aux << value1;
    (*logFile) << aux.str() << " - ";

    stringstream aux2;
    aux2.clear();
    aux2 << value2;
    (*logFile) << aux2.str() << std::endl;
  }
}


void TranslationLog::write( const string & message, const uint64_t & value1, const uint64_t & value2 )
{
  if ( logEnabled )
  {
    (*logFile) << message << ": ";

    stringstream aux;
    aux << value1;
    (*logFile) << aux.str() << " - ";

    stringstream aux2;
    aux2.clear();
    aux2 << value2;
    (*logFile) << aux2.str() << std::endl;
  }
}


void TranslationLog::write( const string & message1, const string & message2, const uint32_t & value )
{
  if ( logEnabled )
  {
    stringstream aux;
    aux << value;
    (*logFile) << message1 << " " << message2 << ": " << aux.str() << std::endl;
  }
}


void writeHeaderTimes( TranslationData *transData )
{
  ostringstream ostr;

  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  *transData->PRVFile << "#Paraver ";
  *transData->PRVFile << "()" << ":";

  ostr << transData->maxTraceTime;
  *transData->PRVFile << ostr.str();

  *transData->PRVFile << "_" << LABEL_TIMEUNIT[ transData->timeUnit ];
  *transData->PRVFile << ":";
}


void writeHeaderResourceModel( TranslationData *transData )
{
  transData->resourcesModel->dumpToFile( *transData->PRVFile );
  *transData->PRVFile << ":";
}


void writeHeaderProcessModel( TranslationData *transData )
{
  transData->processModel->dumpToFile( *transData->PRVFile );
  *transData->PRVFile << ",0" << endl;
}


void writeComment( TranslationData *transData )
{
  wordexp_t wordInfo;
  char **auxPathExpanded;
  string otf2TraceExpanded;

  *transData->PRVFile << "# Translated from OTF2 trace ";

  string auxStr( transData->strOTF2Trace );
  wordexp( auxStr.c_str(), &wordInfo, 0 );
  auxPathExpanded = wordInfo.we_wordv;
  if ( wordInfo.we_wordc > 0 )
  {
    *transData->PRVFile << auxPathExpanded[ 0 ] << endl;
  }
  else
  {
    *transData->PRVFile << transData->strOTF2Trace << endl;
  }

  wordfree( &wordInfo );
}


// *****************************************************************************
// 2nd LEVEL - OTF2 level
// *****************************************************************************
void openPRV( const string &strPRVTrace, std::fstream &file )
{
  file.open( strPRVTrace.c_str(), fstream::out | fstream::trunc );
  file << fixed;
  file << dec;
  file.precision( 0 );
}


void loadExternalTranslationTable( const string &strExternalTrace,
                                   TranslationData *transData )
{
  string line;
  string token;
  string auxOTF2Label;
  string auxTypeLabel;
  string auxValueLabel;
  int newType;
  int newValue;

  if ( transData->useExternalTranslationTable )
  {
    ifstream extTableFile( strExternalTrace.c_str() );
    if ( extTableFile.good() )
    {
      while( !extTableFile.eof() )
      {
        getline( extTableFile, line );
        if ( line.length() == 0 )
          continue;

        if ( line[0] == '#' ||
             line.length() == 0 )
          continue;

        transData->myLog.write(  "[MSK] External definition ", line);

        istringstream auxLine( line );
        getline( auxLine, token, '"' );
        getline( auxLine, auxOTF2Label, '"' );
        // transData->myLog.write(  "[MSK] External Token ", token);

        while( !auxLine.eof() )
        {
          getline( auxLine, token, ' ' );

          if ( token.length() > 1 )
          {
            istringstream sstrTypeValue( token );
            getline( sstrTypeValue, token, ':' );
            istringstream auxType( token );
            if ( !( auxType >> newType ) )
              continue;

            getline( sstrTypeValue, token );
            if( token.length() == 0 )
              newValue = 0;
            else
            {
              istringstream auxValue( token );
              if ( !( auxValue >> newValue ) )
                continue;
            }

            break;
          }
        }

        getline( auxLine, token, '"' );
        getline( auxLine, auxTypeLabel, '"' );
        getline( auxLine, token, '"' );
        getline( auxLine, auxValueLabel, '"' );

        if( newValue == 0 && auxValueLabel == "" )
          EventList::getInstance()->insert( auxOTF2Label, false, 0, newType, newValue,
                                            auxTypeLabel, auxValueLabel, false, STATE_IDLE );
        else
          EventList::getInstance()->insert( auxOTF2Label, false, 0, newType, newValue,
                                            auxTypeLabel, auxValueLabel, true, STATE_RUNNING );
      }
    }
    extTableFile.close();
  }
}


SCOREP_Error_Code LocalDefClockOffsetHandler( void*    userData,
                                              uint64_t time,
                                              int64_t  offset,
                                              double   stddev )
{
  TranslationData *transData = ( TranslationData * )userData;
  //uint64_t* location_id_ptr = userData;
/*
  transData->localDefClockOffset[ transData->currentLocation ].time = time;
  transData->localDefClockOffset[ transData->currentLocation ].offset = offset;
  transData->localDefClockOffset[ transData->currentLocation ].stddev = stddev;
*/
  transData->myLog.write(  "[DEF][LOCAL] CLOCK OFFSET : " );
  transData->myLog.write(  "             Location ID  : ", transData->currentLocation );
  transData->myLog.write(  "             Clock Time   : ", time );
  transData->myLog.write(  "             Clock Offset : ", offset );
  transData->myLog.write(  "             Clock StDev  : ", stddev );

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefClockPropertiesHandler( void*    userData,
                                                 uint64_t timer_resolution,
                                                 uint64_t global_offset,
                                                 uint64_t trace_length )
{
  TranslationData *transData = ( TranslationData * )userData;

  double timeUnit = 1 / (double)timer_resolution;

  if ( timeUnit >= ( 1 / 1E9 ))
  {
    transData->timeUnit = NS;
    transData->myLog.write(  "[DEF] CLOCK UNIT: NS ", timeUnit );
  }
  else if ( timeUnit >= ( 1 / 1E6 ))
  {
    transData->timeUnit = US;
    transData->myLog.write(  "[DEF] CLOCK UNIT: US ", timeUnit );
  }
  else if ( timeUnit >= (1 / 1E3 ))
  {
    transData->timeUnit = MS;
    transData->myLog.write(  "[DEF] CLOCK UNIT: MS ", timeUnit );
  }
  else
  {
    transData->timeUnit = SEC;
    transData->myLog.write(  "[DEF] CLOCK UNIT: S ", timeUnit );
  }

  transData->timeUnit = NS;
  transData->myLog.write(  "[DEF] CLOCK UNIT: NS ", timeUnit );

  //transData->maxTraceTime = global_offset + trace_length; // ??
  transData->maxTraceTime = trace_length;
  transData->globalOffset = global_offset;

  transData->myLog.write(  "[DEF] CLOCK (max, offset):", trace_length, global_offset );

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefCallsiteHandler( void*    userData,
                                          uint32_t callsite_identifier,
                                          uint32_t source_file,
                                          uint32_t line_number,
                                          uint32_t region_entered,
                                          uint32_t region_left )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->myLog.write(  "[DEF-???] : CALLSITE" );
  /*
    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %12u  File: %s, Line: %u, "
            "Region entered: %s, Region left: %s\n",
            otf2_DEF_COLUMN_WIDTH, "CALLSITE",
            callsite_identifier,
            otf2_print_get_def_name( data->strings, source_file ),
            line_number,
            otf2_print_get_def_name( data->regions, region_entered ),
            otf2_print_get_def_name( data->regions, region_left ) );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefCallpathHandler( void*    userData,
                                          uint32_t callpath_identifier,
                                          uint32_t parent_callpath,
                                          uint32_t region_identifier )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->myLog.write(  "[DEF-???] : CALLPATH" );
  /*
    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %12u  Region: %s, Parent: %s\n",
            otf2_DEF_COLUMN_WIDTH, "CALLPATH",
            callpath_identifier,
            otf2_print_get_def_name( data->regions, region_identifier ),
            otf2_print_get_id( parent_callpath ) );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefMpiCommHandler( void*    userData,
                                         uint32_t comm_id,
                                         uint32_t comm_name,
                                         uint32_t group_id,
                                         uint32_t comm_parent )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->myLog.write(  "[DEF-???] : MPICOMM" );

  /*
    otf2_print_data* data = ( otf2_print_data* )userData;

    otf2_print_add_mpi_comm( data,
                             comm_id,
                             comm_name );

    if ( !otf2_GLOBDEFS )
    {
        return SCOREP_SUCCESS;
    }

    printf( "%-*s %12u  Name: %s, Group: %s, Parent Communicator: %s\n",
            otf2_DEF_COLUMN_WIDTH, "MPI_COMM",
            comm_id,
            otf2_print_get_def_name( data->strings, comm_name ),
            otf2_print_get_def_name( data->groups, group_id ),
            otf2_print_get_def_name( data->mpi_comms, comm_parent ) );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefMetricMemberHandler( void*           userData,
                                              uint32_t        metric_member_id,
                                              uint32_t        name,
                                              uint32_t        description,
                                              OTF2_MetricType type,
                                              OTF2_MetricMode mode,
                                              OTF2_TypeID     value_type,
                                              OTF2_MetricBase base,
                                              int64_t         exponent,
                                              uint32_t        unit )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->myLog.write(  "[DEF-???] : METRIC MEMBER" );
  /*
    otf2_print_data* data = ( otf2_print_data* )userData;

    otf2_print_add_metric( data,
                           metric_member_id,
                           name );

    if ( !otf2_GLOBDEFS )
    {
        return SCOREP_SUCCESS;
    }

    uint8_t base_number = 0;
    switch ( base )
    {
        case OTF2_BASE_BINARY:
            base_number = 2;
            break;
        case OTF2_BASE_DECIMAL:
            base_number = 10;
            break;
    }

    printf( "%-*s %12u  Name: %s, Descr.: %s, Type: %s, "
            "Mode: %s, Value Type: %s, Base: %u, Exponent: %" PRId64 ", "
            "Unit: %s\n",
            otf2_DEF_COLUMN_WIDTH, "METRIC_MEMBER",
            metric_member_id,
            otf2_print_get_def_name( data->strings, name ),
            otf2_print_get_def_name( data->strings, description ),
            otf2_print_get_metric_type( type ),
            otf2_print_get_metric_mode( mode ),
            otf2_print_get_type_id( value_type ),
            base_number, exponent,
            otf2_print_get_def_name( data->strings, unit ) );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefMetricClassHandler( void*                 userData,
                                             uint32_t              metric_class_id,
                                             uint8_t               number_of_metrics,
                                             const uint32_t*       metric_members,
                                             OTF2_MetricOccurrence occurrence )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->myLog.write(  "[DEF-???] : METRIC CLASS" );
  /*
    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %12u  Occurrence: %s, %u Members: ",
            otf2_DEF_COLUMN_WIDTH, "METRIC_CLASS",
            metric_class_id,
            otf2_print_get_metric_occurrence( occurrence ),
            number_of_metrics );

    const char* sep = "";
    for ( uint8_t i = 0; i < number_of_metrics; i++ )
    {
        printf( "%s%s",
                sep,
                otf2_print_get_def_name( data->metrics, metric_members[ i ] ) );
        sep = ", ";
    }
    printf( "\n" );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefMetricInstanceHandler( void*            userData,
                                                uint32_t         metricInstanceID,
                                                uint32_t         metricClass,
                                                uint64_t         recorder,
                                                OTF2_MetricScope scopeType,
                                                uint64_t         scope )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->myLog.write(  "[DEF-???] : METRIC INSTANCE" );
  /*
    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %12u  Class: %u, Recorder: %s, "
            "Scope: %s %s\n",
            otf2_DEF_COLUMN_WIDTH, "METRIC_INSTANCE",
            metricInstanceID,
            metricClass,
            otf2_print_get_def64_name( data->locations, recorder ),
            otf2_print_get_metric_scope( scopeType ),
            otf2_print_get_scope_name( data, scopeType, scope ) );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefParameterHandler( void*              userData,
                                           uint32_t           parameterID,
                                           uint32_t           name,
                                           OTF2_ParameterType type )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->myLog.write(  "[DEF-???] : PARAMETER" );
  /*
    otf2_print_data* data = ( otf2_print_data* )userData;

    otf2_print_add_parameter( data,
                              parameterID,
                              name );

    if ( !otf2_GLOBDEFS )
    {
        return SCOREP_SUCCESS;
    }

    printf( "%-*s %12u  Name: %s, Type: %s\n",
            otf2_DEF_COLUMN_WIDTH, "PARAMETER",
            parameterID,
            otf2_print_get_def_name( data->strings, name ),
            otf2_print_get_parameter_type( type ) );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefUnknownHandler( void* userData )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->myLog.write(  "[DEF-???] : UNKNOWN" );
/*
    ( void )userData;

    printf( "%-*s\n",
            otf2_DEF_COLUMN_WIDTH, "UNKNOWN" );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code DefMappingTableHandler( void*             userData,
                                          OTF2_MappingType  mapType,
                                          const OTF2_IdMap* iDMap )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->myLog.write(  "[DEF-???] : MAPPING TABLE" );
  /*
    uint64_t* location_id_ptr = userData;

    printf( "%-*s %12" PRIu64 "  Type: %s, ",
            otf2_DEF_COLUMN_WIDTH, "MAPPING_TABLE",
            *location_id_ptr,
            otf2_print_get_mapping_type( mapType ) );

    OTF2_IdMapMode map_mode;
    OTF2_IdMap_GetMode( iDMap, &map_mode );

    const char*                 sep;
    OTF2_IdMap_TraverseCallback traverse_cb;
    const char*                 end;
    if ( map_mode == OTF2_ID_MAP_DENSE )
    {
        sep         = "[";
        traverse_cb = map_traverse_dense;
        end         = "]";
    }
    else
    {
        sep         = "{";
        traverse_cb = map_traverse_sparse;
        end         = "}";
    }

    OTF2_IdMap_Traverse( iDMap, traverse_cb, &sep );

    puts( end );
*/
    return SCOREP_SUCCESS;
}

SCOREP_Error_Code DefClockOffsetHandler( void*    userData,
                                         uint64_t time,
                                         int64_t  offset,
                                         double   stddev )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->myLog.write(  "[DEF-???] : " );

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefStringHandler( void*       userData,
                                        uint32_t    stringID,
                                        const char* string )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->symbols[ stringID ] = std::string( string );

  transData->myLog.write(  "[DEF] STRING", transData->symbols[ stringID ] );
}


SCOREP_Error_Code GlobDefSystemTreeNodeHandler( void*    userData,
                                                uint32_t nodeID,
                                                uint32_t name,
                                                uint32_t className,
                                                uint32_t nodeParent )
{
  TranslationData *transData = ( TranslationData * )userData;

  // Translation: node of nodes => SYSTEM, NODE
  // How to translate many systems?
  // Many systems to one system and all the nodes merged?

  if ( nodeParent != OTF2_UNDEFINED_UINT32 )
  {
    // Add node to our structure
    transData->resourcesModel->addNode();
    transData->systemTreeNode2GlobalNode[ nodeID ] = transData->resourcesModel->totalNodes() - 1;

    // Keep name for ROW file
    transData->rowLabels->pushBack( NODE, transData->symbols[ name ] );
  }
  else
  {
    transData->rowLabels->pushBack( SYSTEM, transData->symbols[ name ] );
  }

  return SCOREP_SUCCESS;
}


// LocationGroup: { UNKNOWN, PROCESS }
// Translation: Map TASK->NODE
// Assumed OTF2 precond: groupID always growing in 1 unit.
SCOREP_Error_Code GlobDefLocationGroupHandler( void*                  userData,
                                               uint32_t               groupID,
                                               uint32_t               name,
                                               OTF2_LocationGroupType type,
                                               uint32_t               systemTreeParent )
{
  TranslationData *transData = ( TranslationData * )userData;

  TApplOrder currentApplication = transData->processModel->totalApplications() - 1;
  transData->processModel->addTask( currentApplication );
  transData->rowLabels->pushBack( TASK, transData->symbols[ name ] );
  transData->locationGroup2SystemTreeNode[ groupID ] = systemTreeParent; // undefined?

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefLocationHandler( void*             userData,
                                          uint64_t          locationID,
                                          uint32_t          name,
                                          OTF2_LocationType locationType,
                                          uint64_t          numberOfEvents,
                                          uint32_t          locationGroup )
{
  TranslationData *transData = ( TranslationData * )userData;

  // Adds CPU and THREAD to a NODE - TASK
  if ( locationType == OTF2_LOCATION_TYPE_CPU_THREAD )
  {
    TApplOrder currentApplication = transData->processModel->totalApplications() - 1;
    TNodeOrder currentNode = transData->systemTreeNode2GlobalNode[
            transData->locationGroup2SystemTreeNode[ locationGroup ] ];

    transData->processModel->addThread( currentApplication, locationGroup, currentNode ); // podria addThread devolver el global?
    transData->resourcesModel->addCPU( currentNode ); // podria addCPU devolver el global?

    TThreadOrder globalThread = transData->processModel->getLastThread( currentApplication, locationGroup );
//*transData->PRVFile << "#globalThread = " << globalThread << endl;
    TApplOrder app;
    TTaskOrder task;
    TThreadOrder thread;
    transData->processModel->getThreadLocation( globalThread, app, task, thread );
//*transData->PRVFile << "#app:task:thread = " << app << ":" << task << ":" << thread << endl;
    transData->location2Task[ locationID ] = task + 1;
    transData->location2Thread[ locationID ] = thread + 1;
    transData->location2CPU[ locationID ] = transData->resourcesModel->getLastCPU( currentNode );

    // Initialize lastState queue
    transData->stateStack[ globalThread ] = stack< State >();
    transData->stateStack[ globalThread ].push( State( 0, STATE_IDLE ) );
  }

  // Set Callbacks for Local read
  OTF2_EvtReader* evt_reader = OTF2_Reader_GetEvtReader( transData->reader, locationID );
  OTF2_DefReader* def_reader = OTF2_Reader_GetDefReader( transData->reader, locationID );
  OTF2_DefReaderCallbacks* local_def_callbacks = OTF2_DefReaderCallbacks_New();

  transData->currentLocation = locationID;
  OTF2_DefReaderCallbacks_SetClockOffsetCallback( local_def_callbacks, LocalDefClockOffsetHandler ); // ??

  OTF2_Reader_RegisterDefCallbacks( transData->reader, def_reader, local_def_callbacks, (void *)transData );

  uint64_t definitions_read = 0;
  OTF2_Reader_ReadAllLocalDefinitions( transData->reader, def_reader, &definitions_read );

  OTF2_Reader_CloseDefReader( transData->reader, def_reader );

  OTF2_DefReaderCallbacks_Delete( local_def_callbacks );

#if 0
      // LOCAL DEFINITIONS
      OTF2_DefReaderCallbacks* local_def_callbacks = OTF2_DefReaderCallbacks_New();

      OTF2_DefReaderCallbacks_SetClockOffsetCallback( local_def_callbacks, GlobDefClockOffsetHandler ); // ??
      for ( size_t i = 0; i < SCOREP_Vector_Size( user_data.locations_to_read ); i++ )
      {
        uint64_t* location_item      = SCOREP_Vector_At( user_data.locations_to_read, i );
        uint64_t  locationIdentifier = *location_item;

        OTF2_EvtReader* evt_reader = OTF2_Reader_GetEvtReader( reader,
                                                               locationIdentifier );

        /* Also open a definition reader and read all local definitions. */
        OTF2_DefReader* def_reader = OTF2_Reader_GetDefReader( reader,
                                                               locationIdentifier );
        status = OTF2_Reader_RegisterDefCallbacks( reader,
                                                        def_reader,
                                                        local_def_callbacks,
                                                        &locationIdentifier );

        uint64_t definitions_read = 0;
        status = OTF2_Reader_ReadAllLocalDefinitions( reader,
                                                      def_reader,
                                                      &definitions_read );

        /* Close def reader, it is no longer useful and occupies memory */
        status = OTF2_Reader_CloseDefReader( reader, def_reader );
      }

      OTF2_DefReaderCallbacks_Delete( local_def_callbacks );
#endif



/*
    otf2Handler_add_location( data, locationID, name );

    // Print definition if selected.
    if ( otf2_GLOBDEFS )
    {
        printf( "%-*s %12" PRIu64 "  Name: %s, Type: %s, "
                "# Events: %" PRIu64 ", Group: %s\n",
                otf2_DEF_COLUMN_WIDTH, "LOCATION",
                locationID,
                otf2Handler_get_def_name( data->strings, name ),
                otf2Handler_get_location_type( locationType ),
                numberOfEvents,
                otf2Handler_get_def_name( data->location_groups, locationGroup ) );
    }


    // Only proceed if either no local location is selected (i.e. read all) or
    // location ID matches provided location ID.
    if ( otf2_LOCAL != OTF2_UNDEFINED_UINT64 && otf2_LOCAL != locationID )
    {
        return SCOREP_SUCCESS;
    }

    // add location to the list of locations to read events from
    otf2Handler_add_location_to_read( data, locationID );

    otf2_LOCAL_FOUND = true;
*/
   return SCOREP_SUCCESS;
}

// *** otf2->prv translation concepts ***
// regionID: Every Entry/Leave record
// name: Key to the label used
// regionType: otf2-print uses it as groupType?
SCOREP_Error_Code GlobDefRegionHandler( void*           userData,
                                        uint32_t        regionID,
                                        uint32_t        name,
                                        uint32_t        description,
                                        OTF2_RegionType regionType,
                                        uint32_t        sourceFile,
                                        uint32_t        beginLineNumber,
                                        uint32_t        endLineNumber )
{
  TranslationData *transData = ( TranslationData * )userData;
  ++regionID;
  EventList *tmpList = EventList::getInstance();

  if( tmpList->getByStringID( transData->symbols[ name ] ) == NULL )
  {
    tmpList->insert( transData->symbols[ name ], false, 0, USER_FUNCTION, regionID,
                     USER_FUNCTION_LABEL, transData->symbols[ name ], true, STATE_RUNNING );
  }

  map< string, int >::iterator it;

  it = transData->PRVEvent_ValueLabel2Value.find( transData->symbols[ name ] );
  if (  ( it != transData->PRVEvent_ValueLabel2Value.end() ) )
  {
    // TYPE symbolic - VALUE symbolic
    transData->OTF2Region2PRVEventValue[ regionID ] = it->second;
    transData->OTF2Region2PRVEventType[ regionID ] =
            transData->PRVEvent_ValueLabel2Type[ transData->symbols[ name ] ];
    transData->myLog.write(  "[DEF] REGION as VALUE: ", transData->symbols[ name ] );
  }
  else
  {
    it = transData->PRVEvent_TypeLabel2Type.find( transData->symbols[ name ] );

    if ( it != transData->PRVEvent_TypeLabel2Type.end() )
    {
      // TYPE symbolic - VALUE variable
      transData->OTF2Region2PRVEventType[ regionID ] = it->second;
      transData->myLog.write(  "[DEF] REGION as TYPE : ", transData->symbols[ name ] );
    }
    else
    {
      // Careful with colision
      // TODO: Substitute by API call
      // transData->PRVEvent_TypeLabel2Value[ transData->symbols[ name ] ] = getNewType( transData->symbols[ name ] );
      transData->PRVEvent_TypeLabel2Type[ transData->symbols[ name ] ] = USER_FUNCTION;
      transData->OTF2Region2PRVEventType[ regionID ] = USER_FUNCTION;
      transData->myLog.write(  "[DEF] REGION as USER FUNCTION : ", transData->symbols[ name ] );
    }

  }

  transData->regionName[ regionID ] = name;
  transData->regionIdent[ transData->symbols[ name ] ] = regionID;

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefAttributeHandler( void*       userData,
                                           uint32_t    attributeID,
                                           uint32_t    name,
                                           OTF2_TypeID type )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->attributeName[ attributeID ] = transData->symbols[ name ]; // Quick access

  // The translator only uses otf2_msg_match
  // transData->attributeType[ attributeID ] = OTF2_TypeID; // Needed?

  transData->myLog.write(  "[DEF] ATTRIBUTE : ", transData->symbols[ name ], attributeID );

  return  SCOREP_SUCCESS;
}


uint64_t correctBeginTime( TranslationData *transData, uint64_t time )
{
  if ( !transData->correctedBeginTime )
  {
    transData->correctedBeginTime = true;
    transData->maxTraceTime = transData->maxTraceTime - ( time - transData->globalOffset );
    transData->globalOffset = time;

    // WRITE HEADER
    writeHeaderTimes( transData );
    writeHeaderResourceModel( transData );
    writeHeaderProcessModel( transData );
    // writeHeaderCommunicators( transData ); // must be written by proccess model

    writeComment( transData );
 }

  time = time - transData->globalOffset;

  return time;
}


SCOREP_Error_Code BufferFlushHandler( uint64_t            locationID,
                                      uint64_t            time,
                                      void*               userData,
                                      OTF2_AttributeList* attributes,
                                      OTF2_TimeStamp      stopTime )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: BufferFlush" << endl;

  transData->myLog.write(  "[EVT-???] : BufferFlush" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Stop Time: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "BUFFER_FLUSH",
            locationID, time, stopTime );

    otf2_print_attributes( data, attributes );
    */

    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MeasurementOnOffHandler( uint64_t             locationID,
                                           uint64_t             time,
                                           void*                userData,
                                           OTF2_AttributeList*  attributes,
                                           OTF2_MeasurementMode mode )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: MeasurementOnOff" << endl;

  transData->myLog.write(  "[EVT-???] : MeasurementOnOff" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Mode: %s\n",
            otf2_EVENT_COLUMN_WIDTH, "MEASUREMENT_ON_OFF",
            locationID, time,
            otf2_print_get_measurement_mode( mode ) );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MpiIsendHandler( uint64_t            locationID,
                                   uint64_t            time,
                                   void*               userData,
                                   OTF2_AttributeList* attributes,
                                   uint32_t            receiver,
                                   uint32_t            communicator,
                                   uint32_t            msgTag,
                                   uint64_t            msgLength,
                                   uint64_t            requestID )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: MpiIsend" << endl;

  transData->myLog.write(  "[EVT-???] : MpiIsend" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;


    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Receiver: %u, Communicator: %s, "
            "Tag: %u, Length: %" PRIu64 ", Request: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_ISEND",
            locationID, time,
            receiver,
            otf2_print_get_def_name( data->mpi_comms, communicator ),
            msgTag,
            msgLength,
            requestID );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MpiIsendCompleteHandler( uint64_t            locationID,
                                           uint64_t            time,
                                           void*               userData,
                                           OTF2_AttributeList* attributes,
                                           uint64_t            requestID )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: MpiIsendComplete" << endl;

  transData->myLog.write(  "[EVT-???] : MpiIsendComplete" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Request: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_ISEND_COMPLETE",
            locationID, time,
            requestID );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MpiIrecvRequestHandler( uint64_t            locationID,
                                          uint64_t            time,
                                          void*               userData,
                                          OTF2_AttributeList* attributes,
                                          uint64_t            requestID )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: MpiIrecvRequest" << endl;

  transData->myLog.write(  "[EVT-???] : MpiIrecvRequest" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Request: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_IRECV_REQUEST",
            locationID, time,
            requestID );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MpiRecvHandler( uint64_t            locationID,
                                  uint64_t            time,
                                  void*               userData,
                                  OTF2_AttributeList* attributes,
                                  uint32_t            sender,
                                  uint32_t            communicator,
                                  uint32_t            msgTag,
                                  uint64_t            msgLength )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: MpiRecv" << endl;

  transData->myLog.write(  "[EVT-???] : MpiRecv" );
  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Sender: %u, communicator: %s, "
            "Tag: %u, Length: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_RECV",
            locationID, time,
            sender,
            otf2_print_get_def_name( data->mpi_comms, communicator ),
            msgTag,
            msgLength );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MpiIrecvHandler( uint64_t            locationID,
                                   uint64_t            time,
                                   void*               userData,
                                   OTF2_AttributeList* attributes,
                                   uint32_t            sender,
                                   uint32_t            communicator,
                                   uint32_t            msgTag,
                                   uint64_t            msgLength,
                                   uint64_t            requestID )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: MpiIrecv" << endl;

  transData->myLog.write(  "[EVT-???] : MpiIrecv" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Sender: %u, Communicator: %s, "
            "Tag: %u, Length: %" PRIu64 ", Request: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_IRECV",
            locationID, time,
            sender,
            otf2_print_get_def_name( data->mpi_comms, communicator ),
            msgTag,
            msgLength,
            requestID );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MpiRequestTestHandler( uint64_t            locationID,
                                         uint64_t            time,
                                         void*               userData,
                                         OTF2_AttributeList* attributes,
                                         uint64_t            requestID )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: MpiRequestTest" << endl;

  transData->myLog.write(  "[EVT-???] : MpiRequestTest" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Request: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_REQUEST_TEST",
            locationID, time,
            requestID );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MpiRequestCancelledHandler( uint64_t            locationID,
                                              uint64_t            time,
                                              void*               userData,
                                              OTF2_AttributeList* attributes,
                                              uint64_t            requestID )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: MpiRequestCancelled" << endl;

  transData->myLog.write(  "[EVT-???] : MpiRequestCancelled" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Request: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_REQUEST_CANCELLED",
            locationID, time,
            requestID );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MpiCollectiveBeginHandler( uint64_t            locationID,
                                             uint64_t            time,
                                             void*               userData,
                                             OTF2_AttributeList* attribute )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: MpiCollectiveBegin" << endl;

  transData->myLog.write(  "[EVT-???] : MpiCollectiveBegin" );
  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_COLLECTIVE_BEGIN",
            locationID, time );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MpiCollectiveEndHandler( uint64_t               locationID,
                                           uint64_t               time,
                                           void*                  userData,
                                           OTF2_AttributeList*    attributes,
                                           OTF2_MpiCollectiveType type,
                                           uint32_t               commId,
                                           uint32_t               root,
                                           uint64_t               sizeSent,
                                           uint64_t               sizeReceived )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: MpiCollectiveEnd" << endl;

  transData->myLog.write(  "[EVT-???] : MpiCollectiveEnd" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Type: %s, Communicator: %s, "
            "Root: %s, Sent: %" PRIu64 ", Received: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_COLLECTIVE_END",
            locationID, time,
            otf2_print_get_mpi_collective_type( type ),
            otf2_print_get_def_name( data->mpi_comms, commId ),
            otf2_print_get_id( root ),
            sizeSent,
            sizeReceived );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code OmpForkHandler( uint64_t            locationID,
                                  uint64_t            time,
                                  void*               userData,
                                  OTF2_AttributeList* attributes,
                                  uint32_t            numberOfRequestedThreads )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: OmpFork" << endl;

  transData->myLog.write(  "[EVT-???] : OmpFork" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  # Requested Threads: %u\n",
            otf2_EVENT_COLUMN_WIDTH, "OPENMP_FORK",
            locationID, time,
            numberOfRequestedThreads );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code OmpJoinHandler( uint64_t            locationID,
                                  uint64_t            time,
                                  void*               userData,
                                  OTF2_AttributeList* attributes )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: OmpJoin" << endl;

  transData->myLog.write(  "[EVT-???] : OmpJoin" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "OPENMP_JOIN",
            locationID, time );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code OmpAcquireLockHandler( uint64_t            locationID,
                                         uint64_t            time,
                                         void*               userData,
                                         OTF2_AttributeList* attributes,
                                         uint32_t            lockID,
                                         uint32_t            acquisitionOrder )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: OmpAcquireLock" << endl;

  transData->myLog.write(  "[EVT-???] : OmpAcquireLock" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Lock: %u, "
            "Acquisition Order: %u\n",
            otf2_EVENT_COLUMN_WIDTH, "OPENMP_ACQUIRE_LOCK",
            locationID, time,
            lockID,
            acquisitionOrder );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code OmpReleaseLockHandler( uint64_t            locationID,
                                         uint64_t            time,
                                         void*               userData,
                                         OTF2_AttributeList* attributes,
                                         uint32_t            lockID,
                                         uint32_t            acquisitionOrder )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: OmpReleaseLock" << endl;

  transData->myLog.write(  "[EVT-???] : OmpReleaseLock" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Lock: %u, "
            "Acquisition Order: %u\n",
            otf2_EVENT_COLUMN_WIDTH, "OPENMP_RELEASE_LOCK",
            locationID, time,
            lockID,
            acquisitionOrder );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code OmpTaskCreateHandler( uint64_t            locationID,
                                        uint64_t            time,
                                        void*               userData,
                                        OTF2_AttributeList* attributes,
                                        uint64_t            taskID )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: OmpTaskCreate" << endl;

  transData->myLog.write(  "[EVT-???] : OmpTaskCreate" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Task: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "OPENMP_TASK_CREATE",
            locationID, time,
            taskID );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code OmpTaskSwitchHandler( uint64_t            locationID,
                                        uint64_t            time,
                                        void*               userData,
                                        OTF2_AttributeList* attributes,
                                        uint64_t            taskID )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: OmpTaskSwitch" << endl;

  transData->myLog.write(  "[EVT-???] : OmpTaskSwitch" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Task: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "OPENMP_TASK_SWITCH",
            locationID, time,
            taskID );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code OmpTaskCompleteHandler( uint64_t            locationID,
                                         uint64_t            time,
                                         void*               userData,
                                         OTF2_AttributeList* attributes,
                                         uint64_t            taskID )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: OmpTaskComplete" << endl;

  transData->myLog.write(  "[EVT-???] : OmpTaskComplete" );

  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Task: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "OPENMP_TASK_COMPLETE",
            locationID, time,
            taskID );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MetricHandler( uint64_t                locationID,
                                 uint64_t                time,
                                 void*                   userData,
                                 OTF2_AttributeList*     attributes,
                                 uint32_t                metricID,
                                 uint8_t                 numberOfMetrics,
                                 const OTF2_TypeID*      typeIDs,
                                 const OTF2_MetricValue* values )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: Metric" << endl;

  transData->myLog.write(  "[EVT-???] : Metric" );
  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Metric: %s, "
            "%u Values: ",
            otf2_EVENT_COLUMN_WIDTH, "METRIC",
            locationID, time,
            otf2_print_get_id( metricID ),
            numberOfMetrics );

    const char* sep = "";
    for ( uint8_t i = 0; i < numberOfMetrics; i++ )
    {
        switch ( typeIDs[ i ] )
        {
            case OTF2_INT64_T:
                printf( "%s(INT64_T; %" PRId64 ")", sep, values[ i ].signed_int );
                break;
            case OTF2_UINT64_T:
                printf( "%s(UINT64_T; %" PRIu64 ")", sep, values[ i ].unsigned_int );
                break;
            case OTF2_DOUBLE:
                printf( "%s(DOUBLE; %f)", sep, values[ i ].floating_point );
                break;
            default:
            {
                printf( "%s(%s; %08" PRIx64 ")",
                        sep,
                        otf2_print_get_invalid( typeIDs[ i ] ),
                        values[ i ].unsigned_int );
            }
        }
        sep = ", ";
    }
    printf( "\n" );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code ParameterStringHandler( uint64_t            locationID,
                                          uint64_t            time,
                                          void*               userData,
                                          OTF2_AttributeList* attributes,
                                          uint32_t            parameter,
                                          uint32_t            value )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: ParameterString" << endl;

  transData->myLog.write(  "[EVT-???] : ParameterString" );
  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Parameter: %s, "
            "Value: %s\n",
            otf2_EVENT_COLUMN_WIDTH, "PARAMETER_STRING",
            locationID, time,
            otf2_print_get_def_name( data->parameters, parameter ),
            otf2_print_get_def_name( data->strings, value ) );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code ParameterIntHandler( uint64_t            locationID,
                                       uint64_t            time,
                                       void*               userData,
                                       OTF2_AttributeList* attributes,
                                       uint32_t            parameter,
                                       int64_t             value )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: ParameterInt" << endl;

  transData->myLog.write(  "[EVT-???] : ParameterInt" );
  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Parameter: %s, "
            "Value: %" PRIi64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "PARAMETER_INT64",
            locationID, time,
            otf2_print_get_def_name( data->parameters, parameter ),
            value );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code ParameterUnsignedIntHandler( uint64_t            locationID,
                                               uint64_t            time,
                                               void*               userData,
                                               OTF2_AttributeList* attributes,
                                               uint32_t            parameter,
                                               uint64_t            value )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: ParameterUnsignedInt" << endl;

  transData->myLog.write(  "[EVT-???] : ParameterUnsignedInt" );
  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Parameter: %s, "
            "Value: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "PARAMETER_UINT64",
            locationID, time,
            otf2_print_get_def_name( data->parameters, parameter ),
            value );

    otf2_print_attributes( data, attributes );
*/
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code UnknownHandler( uint64_t            locationID,
                                  uint64_t            time,
                                  void*               userData,
                                  OTF2_AttributeList* attributes )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  *transData->PRVFile << "# OTF2 event not translated: Unknown" << endl;

  transData->myLog.write(  "[EVT-???] : Unknown" );
  /*
    if ( time < otf2_MINTIME || time > otf2_MAXTIME )
    {
        return SCOREP_SUCCESS;
    }

    otf2_print_data* data = ( otf2_print_data* )userData;

    printf( "%-*s %15" PRIu64 " %20" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "UNKNOWN",
            locationID, time );

    otf2_print_attributes( data, attributes );
    */
    return SCOREP_SUCCESS;
}


SCOREP_Error_Code MpiSendHandler( uint64_t            locationID,
                                  uint64_t            time,
                                  void*               userData,
                                  OTF2_AttributeList* attributes,
                                  uint32_t            receiver,
                                  uint32_t            communicator,
                                  uint32_t            msgTag,
                                  uint64_t            msgLength )
{
  TranslationData *transData = ( TranslationData * )userData;

  time = correctBeginTime( transData, time );

  transData->myLog.write(  "[EVT] MpiSend : ", locationID, receiver );

  if ( OTF2_AttributeList_GetNumberOfElements( attributes ) > 0 )
  {
    uint32_t index = 0;
    uint32_t attributeID;
    OTF2_TypeID type;
    OTF2_AttributeValue value;
    SCOREP_Error_Code error;
    error = OTF2_AttributeList_GetAttributeByIndex( attributes,
                                                    index,
                                                    &attributeID,
                                                    &type,
                                                    &value );
    if ( transData->attributeName[ attributeID ].compare( string( "rcv time" ) ) == 0  &&
         type == OTF2_UINT64_T )
    {
      stringstream commRecord;
      commRecord << fixed;
      commRecord << dec;
      commRecord.precision( 0 );

      commRecord << "3" << ":";

      // Sender
      commRecord << transData->location2CPU[ locationID ] << ":"; // CPU
      commRecord << transData->processModel->totalApplications() << ":"; // APP
      commRecord << transData->location2Task[ locationID ] << ":"; // TASK
      commRecord << transData->location2Thread[ locationID ] << ":"; // THREAD

      commRecord << time << ":"; // Logical Send
      commRecord << time << ":"; // Physical Send

      // Receiver
      commRecord << transData->location2CPU[ receiver ] << ":"; // CPU
      commRecord << transData->processModel->totalApplications() << ":"; // APP
      commRecord << transData->location2Task[ receiver ] << ":"; // TASK
      commRecord << transData->location2Thread[ receiver ] << ":"; // THREAD

      value.uint64 = correctBeginTime( transData, value.uint64 );
      commRecord << value.uint64 << ":"; // Logical Receive
      commRecord << value.uint64 << ":"; // Physical Receive

      // Tag:Length
      commRecord << msgTag << ":";
      commRecord << msgLength << std::endl;

      *transData->PRVFile << commRecord.str(); // change to Trace write.

      transData->myLog.write(  commRecord.str() );
    }
  }

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code EnterHandler( uint64_t locationID,
                                uint64_t time,
                                void* userData,
                                OTF2_AttributeList* attributes,
                                uint32_t regionID )
{
  TranslationData *transData = ( TranslationData * )userData;
  ++regionID;
  time = correctBeginTime( transData, time );

  EventDescription *evtDesc = EventList::getInstance()->getByStringID(
                                transData->symbols[ transData->regionName[ regionID ] ] );
  if( evtDesc != NULL )
  {
    if( evtDesc->getChangeState() )
    {
      State& tmpState = transData->stateStack[ locationID ].top();
      stringstream stateRecord;

      stateRecord << fixed;
      stateRecord << dec;
      stateRecord.precision( 0 );

      stateRecord << "1" << ":";
      stateRecord << transData->location2CPU[ locationID ] << ":"; // CPU
      stateRecord << transData->processModel->totalApplications() << ":"; // APP
      stateRecord << transData->location2Task[ locationID ] << ":"; // TASK
      stateRecord << transData->location2Thread[ locationID ] << ":"; // THREAD
      stateRecord << tmpState.getBeginTime() << ":";
      stateRecord << time << ":"; //Duration
      stateRecord << tmpState.getState(); // State

      *transData->PRVFile << stateRecord.str() << std::endl; // change to Trace write.

      transData->stateStack[ locationID ].push( State( time, evtDesc->getStateTransition() ) );
    }

    evtDesc->setUsed( true );

    stringstream eventRecord;
    eventRecord << fixed;
    eventRecord << dec;
    eventRecord.precision( 0 );

//*transData->PRVFile << "Location ID: " << locationID << endl;
    eventRecord << "2" << ":";
    eventRecord << transData->location2CPU[ locationID ] << ":"; // CPU
    eventRecord << transData->processModel->totalApplications() << ":"; // APP
    eventRecord << transData->location2Task[ locationID ] << ":"; // TASK

    eventRecord << transData->location2Thread[ locationID ] << ":"; // THREAD
    // eventRecord << time << ":";
    eventRecord << time << ":";
    // eventRecord << transData->PRVEvent_Value2Type[ it->second ] << ":";
    eventRecord << evtDesc->getType() << ":";
    eventRecord << evtDesc->getValue();

    *transData->PRVFile << eventRecord.str() << std::endl; // change to Trace write.

    transData->myLog.write(  eventRecord.str() );
  }

  return SCOREP_SUCCESS;
}


// This function is completely simmetric to Entry Handler, but writing 0's
SCOREP_Error_Code LeaveHandler( uint64_t locationID,
                                uint64_t time,
                                void* userData,
                                OTF2_AttributeList* attributes,
                                uint32_t regionID )
{
  TranslationData *transData = ( TranslationData * )userData;
  ++regionID;
  time = correctBeginTime( transData, time );

  EventDescription *evtDesc = EventList::getInstance()->getByStringID(
                                transData->symbols[ transData->regionName[ regionID ] ] );
  if( evtDesc != NULL )
  {
    if( evtDesc->getChangeState() )
    {
      State& tmpState = transData->stateStack[ locationID ].top();
      stringstream stateRecord;

      stateRecord << fixed;
      stateRecord << dec;
      stateRecord.precision( 0 );

      stateRecord << "1" << ":";
      stateRecord << transData->location2CPU[ locationID ] << ":"; // CPU
      stateRecord << transData->processModel->totalApplications() << ":"; // APP
      stateRecord << transData->location2Task[ locationID ] << ":"; // TASK
      stateRecord << transData->location2Thread[ locationID ] << ":"; // THREAD
      stateRecord << tmpState.getBeginTime() << ":";
      stateRecord << time << ":"; //Duration
      stateRecord << tmpState.getState(); // State

      *transData->PRVFile << stateRecord.str() << std::endl; // change to Trace write.

      transData->stateStack[ locationID ].pop();
      transData->stateStack[ locationID ].top().setBeginTime( time );
    }

    stringstream eventRecord;
    eventRecord << fixed;
    eventRecord << dec;
    eventRecord.precision( 0 );

    eventRecord << "2" << ":";
    eventRecord << transData->location2CPU[ locationID ] << ":"; // CPU
    eventRecord << transData->processModel->totalApplications() << ":"; // APP
    eventRecord << transData->location2Task[ locationID ] << ":"; // TASK
    eventRecord << transData->location2Thread[ locationID ] << ":"; // THREAD

    eventRecord << time << ":";
    //eventRecord << transData->PRVEvent_Value2Type[ it->second ] << ":";
    eventRecord << evtDesc->getType() << ":";

    eventRecord << "0"; // VALUE

    *transData->PRVFile << eventRecord.str() << std::endl;  // change to Trace write.

    transData->myLog.write(  eventRecord.str() );
  }

  return SCOREP_SUCCESS;
}


// *****************************************************************************
// 1st LEVEL - GENERAL
// *****************************************************************************

void printHelp()
{
  std::cout << std::endl;
  std::cout << "USAGE" << std::endl;
  std::cout << "  otf2prv [OPTION] trc.otf2 [trc.prv]" << std::endl;
  std::cout << std::endl;
  std::cout << "  Options:" << std::endl;
  std::cout << "    -h: Prints this help." << std::endl;
  std::cout << "    -v: Show version." << std::endl;
  std::cout << "    -t [translation_table] : Masks hardcoded events with the ones"
               " defined in file translation_table." << std::endl;
  std::cout << "                             If no table is given, an example of"
               " that table is written to the stdout." << std::endl;
  std::cout <<  "    -l: Prints log information to the stdout."  << std::endl;
  std::cout << std::endl;
  std::cout << "  Parameters:" << std::endl;
  std::cout << "    trc.otf2: OTF2 trace filename." << std::endl;
  std::cout << "    trc.prv: Paraver trace filename (optional; default is trc.otf2.prv)." << std::endl;
  std::cout << std::endl;
  std::cout << "  Examples:" << std::endl;
  std::cout << "    $ otf2prv linpack.otf2" << std::endl;
  std::cout << "          Generates translation from linpack OTF2 trace to linpack.prv PRV trace." << std::endl;
  std::cout << std::endl;
  std::cout << "    $ otf2prv -t > mask_example.txt" << std::endl;
  std::cout << "          Writes example of translation table." << std::endl;
  std::cout << std::endl;
  std::cout << "    $ otf2prv -l -t mask_mpi_types.txt linpack.otf2 newtrans.prv &> tmp.log" << std::endl;
  std::cout << "          Same than first example, but using external translation table "
               "and also printing log information of the generation of newtrans.prv trace." << std::endl;
  std::cout << std::endl;
}


void printVersion()
{
  //std::cout << PACKAGE_STRING;
  std::cout << "otf2prv ";
  std::cout << OTF2_VERSION_STRING << " (OTF2)";

  bool reverseOrder = true;
  string auxDate = LabelConstructor::getDate( reverseOrder );

  if ( auxDate.compare("") != 0 )
    std::cout << " Build ";

  std::cout << auxDate << std::endl;
}

void printExternalTableExample()
{
  std::cout << "#######################################"
               "#######################################" << std::endl;
  std::cout << "# otf2prv mask table example" << std::endl;
  std::cout << "#" << std::endl;
  std::cout << "#   <#> used for comments." << std::endl;
  std::cout << "#   This information is used for transl"
               "ation instead harcoded values." <<std::endl;
  std::cout << "#   Field <otf2_string> is matched against O"
               "TF2 defined strings." <<std::endl;
  std::cout << "#   Paraver labels will be written in the p"
               "cf file." <<std::endl;
  std::cout << "#" << std::endl;
  std::cout << "#####################################"
    "#########################################" << std::endl;
  std::cout << std::endl;
  std::cout << "#" << std::endl;
  std::cout << "# LINE FORMAT FOR TYPES (without values) :" << std::endl;
  std::cout << "#   <otf2_string>   <paraver_type>   <paraver_type_label>" << std::endl;
  std::cout << std::endl;
  std::cout << "    \"MPI Other\"  50000003  \"MPI Other\"" << std::endl;
  std::cout << std::endl;
  std::cout << "#" << std::endl;
  std::cout << "# LINE FORMAT FOR TYPE-VALUE :" << std::endl;
  std::cout << "#   <otf2_string>   <paraver_type>:<paraver_value>   <paraver_type_label>:<paraver_value_label>" << std::endl;
  std::cout << std::endl;
  std::cout << "    \"MPI_Init\"  50000003:31  \"MPI Other\":\"MPI_Init\"" << std::endl;
  std::cout << std::endl;
}


bool isOption( char *argument )
{
  return ( argument[ 0 ] == '-' );
}


void activateOption( char *argument,
                     MainOptions &options,
                     TranslationData &transData )
{
  if ( argument[ 1 ] == 'h' )
    options.showHelp = true;
  else if ( argument[ 1 ] == 'v' )
    options.showVersion = true;
  else if ( argument[ 1 ] == 'l')
  {
    transData.myLog.enableLog( true );
  }
  else if ( argument[ 1 ] == 't')
  {
    transData.useExternalTranslationTable = true;
  }
  else
    std::cout << "Unknown option " << argument << std::endl;
}


void readParameters( int argc,
                     char *arguments[],
                     MainOptions &options,
                     TranslationData &transData,
                     string &strPRVTrace,
                     string &strExternalTable )
{
  PRV_INT32 currentArg = 1;

  while ( currentArg < argc )
  {
    if ( isOption( arguments[ currentArg ] ))
    {
      activateOption( arguments[ currentArg ], options, transData );
      if ( transData.useExternalTranslationTable )
      {
        if ( currentArg + 1 < argc )
        {
          strExternalTable = string( arguments[ ++currentArg ] );
        }
        else
        {
          transData.useExternalTranslationTable = false;
          options.printExternalTableExample = true;
        }
      }
    }
    else if ( Trace::isTraceFile( string( arguments[ currentArg ] )))
    {
      strPRVTrace = string( arguments[ currentArg ] );
    }
    else if ( Trace::isOTF2TraceFile( string( arguments[ currentArg ] )))
    {
      transData.strOTF2Trace = string( arguments[ currentArg ] );
    }
    else
    {
      std::cout << "Unknown parameter " << arguments[ currentArg ] << std::endl;
    }

    ++currentArg;
  }
}


bool anyOTF2Trace( const string &strOTF2Trace )
{
  return ( strOTF2Trace != "" );
}


void initialize( TranslationData &transData )
{
  transData.timeUnit = NS;

  transData.maxTraceTime = 0;
  transData.globalOffset = 0;

  transData.resourcesModel = new ResourceModel();
  transData.processModel = new ProcessModel();
  transData.processModel->addApplication();
  transData.rowLabels = new RowLabels();

  EventList::getInstance()->init();

  transData.myLog.write(  "[REC] Labels for MPI types" );
  for( uint32_t i = 0; i < NUM_MPI_BLOCK_GROUPS; ++i )
  {
    transData.myLog.write(  "[REC]     ", prv_block_groups[ i ].label, prv_block_groups[ i ].type );
    transData.PRVEvent_TypeLabel2Type[ prv_block_groups[ i ].label ] = prv_block_groups[ i ].type;
  }

  transData.myLog.write(  "[REC] Labels for MPI values" );
  for( uint32_t i = 0; i < NUM_MPI_PRV_ELEMENTS; ++i )
  {
    transData.PRVEvent_ValueLabel2Value[ string( mpi_prv_val_label[ i ].label ) ] = mpi_prv_val_label[ i ].value;
    transData.myLog.write(  "[REC]     ", mpi_prv_val_label[ i ].label, mpi_prv_val_label[ i ].value );
    transData.PRVEvent_ValueLabel2Type[ string( mpi_prv_val_label[ i ].label ) ] =
            event_mpit2prv[ mpi_prv_val_label[ i ].value ].tipus_prv;
    transData.myLog.write(  "[REC]     ",
              mpi_prv_val_label[ i ].label,
              event_mpit2prv[ mpi_prv_val_label[ i ].value ].tipus_prv );
  }
/*
  transData.myLog.write(  "[REC] Declare values of every type" );
  for( uint32_t i = 0; i < NUM_MPI_PRV_ELEMENTS; ++i )
  {
    transData.myLog.write(  "[REC]     ", event_mpit2prv[ i ].tipus_prv, event_mpit2prv[ i ].valor_prv );
    transData.PRVEvent_ValueLabel2Type[ event_mpit2prv[ i ].valor_prv ] = event_mpit2prv[ i ].tipus_prv;
  }
  */
}


bool translate( const string &strPRVTrace,
                const string &strExternalTable,
                TranslationData *transData )
{
  bool translationReady = false;
  std::fstream file;
  openPRV( strPRVTrace, file );

  if ( file.good() )
  {
    transData->myLog.write(  "[FILE] Opened ", strPRVTrace );
    OTF2_Reader* reader = OTF2_Reader_Open( transData->strOTF2Trace.c_str() );
    if ( reader != NULL )
    {
      transData->myLog.write(  "[FILE] Opened ", transData->strOTF2Trace );

      transData->reader = reader;
      transData->PRVFile = &file;

      // LOAD TRANSLATION TABLE
      loadExternalTranslationTable( strExternalTable, transData );

      // BUILD HEADER
      transData->myLog.write( "[REC] Registering OTF2 Callbacks" );

      OTF2_GlobalDefReader* global_def_reader = OTF2_Reader_GetGlobalDefReader( reader );
      OTF2_GlobalDefReaderCallbacks* global_def_callbacks = OTF2_GlobalDefReaderCallbacks_New();

      OTF2_GlobalDefReaderCallbacks_SetUnknownCallback( global_def_callbacks, GlobDefUnknownHandler );
      OTF2_GlobalDefReaderCallbacks_SetStringCallback( global_def_callbacks, GlobDefStringHandler );
      OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeCallback( global_def_callbacks, GlobDefSystemTreeNodeHandler );
      OTF2_GlobalDefReaderCallbacks_SetLocationGroupCallback( global_def_callbacks, GlobDefLocationGroupHandler );
      OTF2_GlobalDefReaderCallbacks_SetLocationCallback( global_def_callbacks, GlobDefLocationHandler );
      OTF2_GlobalDefReaderCallbacks_SetRegionCallback( global_def_callbacks, GlobDefRegionHandler );
      //OTF2_GlobalDefReaderCallbacks_SetGroupCallback( global_def_callbacks, GlobDefGroupHandler );
      //OTF2_GlobalDefReaderCallbacks_SetMpiCommCallback( global_def_callbacks, GlobDefMpiCommHandler );
      OTF2_GlobalDefReaderCallbacks_SetMetricMemberCallback( global_def_callbacks, GlobDefMetricMemberHandler );
      OTF2_GlobalDefReaderCallbacks_SetAttributeCallback( global_def_callbacks, GlobDefAttributeHandler );
      OTF2_GlobalDefReaderCallbacks_SetParameterCallback( global_def_callbacks, GlobDefParameterHandler );

      // This group was protected in otf2-print by an if
      OTF2_GlobalDefReaderCallbacks_SetClockPropertiesCallback( global_def_callbacks, GlobDefClockPropertiesHandler );
      OTF2_GlobalDefReaderCallbacks_SetCallsiteCallback( global_def_callbacks, GlobDefCallsiteHandler );
      OTF2_GlobalDefReaderCallbacks_SetCallpathCallback( global_def_callbacks, GlobDefCallpathHandler );
      OTF2_GlobalDefReaderCallbacks_SetMetricClassCallback( global_def_callbacks, GlobDefMetricClassHandler );
      OTF2_GlobalDefReaderCallbacks_SetMetricInstanceCallback( global_def_callbacks, GlobDefMetricInstanceHandler );

      OTF2_Reader_RegisterGlobalDefCallbacks( reader, global_def_reader, global_def_callbacks, (void *)transData );
      OTF2_GlobalDefReaderCallbacks_Delete( global_def_callbacks );

      uint64_t definitions_read = 0;
      OTF2_Reader_ReadAllGlobalDefinitions( reader, global_def_reader, &definitions_read );

/*
      // LOG LOCAL DEFINITIONS
      for( map< uint64_t, LocalDefClockOffsetData >::iterator it = transData->localDefClockOffset.begin();
              it != transData->localDefClockOffset.end(); ++it )
      {
        transData->myLog.write(  "[DEF][LOCAL] CLOCK OFFSET : " );
        transData->myLog.write(  "[DEF][LOCAL] Location ID  : ", it->first );
        transData->myLog.write(  "             Clock Time   : ", it->second.time );
        transData->myLog.write(  "             Clock Offset : ", it->second.offset );
        transData->myLog.write(  "             Clock StDev  : ", it->second.stddev );
      }
*/

#if 0
      // LOCAL DEFINITIONS
      OTF2_DefReaderCallbacks* local_def_callbacks = OTF2_DefReaderCallbacks_New();

      OTF2_DefReaderCallbacks_SetClockOffsetCallback( local_def_callbacks, GlobDefClockOffsetHandler ); // ??
      for ( size_t i = 0; i < SCOREP_Vector_Size( user_data.locations_to_read ); i++ )
      {
        uint64_t* location_item      = SCOREP_Vector_At( user_data.locations_to_read, i );
        uint64_t  locationIdentifier = *location_item;

        OTF2_EvtReader* evt_reader = OTF2_Reader_GetEvtReader( reader,
                                                               locationIdentifier );

        /* Also open a definition reader and read all local definitions. */
        OTF2_DefReader* def_reader = OTF2_Reader_GetDefReader( reader,
                                                               locationIdentifier );
        status = OTF2_Reader_RegisterDefCallbacks( reader,
                                                        def_reader,
                                                        local_def_callbacks,
                                                        &locationIdentifier );

        uint64_t definitions_read = 0;
        status = OTF2_Reader_ReadAllLocalDefinitions( reader,
                                                      def_reader,
                                                      &definitions_read );

        /* Close def reader, it is no longer useful and occupies memory */
        status = OTF2_Reader_CloseDefReader( reader, def_reader );
      }

      OTF2_DefReaderCallbacks_Delete( local_def_callbacks );
#endif



      transData->resourcesModel->setReady( true );
      transData->processModel->setReady( true );

#if 0
      // WRITE HEADER
      writeHeaderTimes( transData );
      writeHeaderResourceModel( transData );
      writeHeaderProcessModel( transData );
      // writeHeaderCommunicators( transData ); // must be written by proccess model

      writeComment( transData );
#endif


      // TRANSLATE EVENTS
      // Initialize callbacks for events
      // Write events
      // Read states
      // Read communications
      OTF2_GlobalEvtReader* global_evt_reader = OTF2_Reader_GetGlobalEvtReader( reader );
      OTF2_GlobalEvtReaderCallbacks* event_callbacks = OTF2_GlobalEvtReaderCallbacks_New();

      OTF2_GlobalEvtReaderCallbacks_SetUnknownCallback( event_callbacks, UnknownHandler );
      OTF2_GlobalEvtReaderCallbacks_SetBufferFlushCallback( event_callbacks, BufferFlushHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMeasurementOnOffCallback( event_callbacks, MeasurementOnOffHandler );
      OTF2_GlobalEvtReaderCallbacks_SetEnterCallback( event_callbacks, EnterHandler );
      OTF2_GlobalEvtReaderCallbacks_SetLeaveCallback( event_callbacks, LeaveHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiSendCallback( event_callbacks, MpiSendHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiIsendCallback( event_callbacks, MpiIsendHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiIsendCompleteCallback( event_callbacks, MpiIsendCompleteHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiIrecvRequestCallback( event_callbacks, MpiIrecvRequestHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiRecvCallback( event_callbacks, MpiRecvHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiIrecvCallback( event_callbacks, MpiIrecvHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiRequestTestCallback( event_callbacks, MpiRequestTestHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiRequestCancelledCallback( event_callbacks, MpiRequestCancelledHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiCollectiveBeginCallback( event_callbacks, MpiCollectiveBeginHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiCollectiveEndCallback( event_callbacks, MpiCollectiveEndHandler );
      OTF2_GlobalEvtReaderCallbacks_SetOmpForkCallback( event_callbacks, OmpForkHandler );
      OTF2_GlobalEvtReaderCallbacks_SetOmpJoinCallback( event_callbacks, OmpJoinHandler );
      OTF2_GlobalEvtReaderCallbacks_SetOmpAcquireLockCallback( event_callbacks, OmpAcquireLockHandler );
      OTF2_GlobalEvtReaderCallbacks_SetOmpReleaseLockCallback( event_callbacks, OmpReleaseLockHandler );
      OTF2_GlobalEvtReaderCallbacks_SetOmpTaskCreateCallback( event_callbacks, OmpTaskCreateHandler );
      OTF2_GlobalEvtReaderCallbacks_SetOmpTaskSwitchCallback( event_callbacks, OmpTaskSwitchHandler );
      OTF2_GlobalEvtReaderCallbacks_SetOmpTaskCompleteCallback( event_callbacks, OmpTaskCompleteHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMetricCallback( event_callbacks, MetricHandler );
      OTF2_GlobalEvtReaderCallbacks_SetParameterStringCallback( event_callbacks, ParameterStringHandler );
      OTF2_GlobalEvtReaderCallbacks_SetParameterIntCallback( event_callbacks, ParameterIntHandler );
      OTF2_GlobalEvtReaderCallbacks_SetParameterUnsignedIntCallback( event_callbacks, ParameterUnsignedIntHandler );

      OTF2_Reader_RegisterGlobalEvtCallbacks( reader, global_evt_reader, event_callbacks, (void *)transData );
      OTF2_GlobalEvtReaderCallbacks_Delete( event_callbacks );

      // Read simbolic info
      // Read until last time
      uint64_t events_read = 0;
      OTF2_Reader_ReadAllGlobalEvents( reader, global_evt_reader, &events_read );

      // Close otf2 trace
      OTF2_Reader_Close( reader );

      for( map< TThreadOrder, stack< State > >::iterator it = transData->stateStack.begin();
           it != transData->stateStack.end(); ++it )
      {
        uint64_t locationID = it->first;
        State& tmpState = transData->stateStack[ locationID ].top();
        stringstream stateRecord;

        stateRecord << fixed;
        stateRecord << dec;
        stateRecord.precision( 0 );

        stateRecord << "1" << ":";
        stateRecord << transData->location2CPU[ locationID ] << ":"; // CPU
        stateRecord << transData->processModel->totalApplications() << ":"; // APP
        stateRecord << transData->location2Task[ locationID ] << ":"; // TASK
        stateRecord << transData->location2Thread[ locationID ] << ":"; // THREAD
        stateRecord << tmpState.getBeginTime() << ":";
        stateRecord << transData->maxTraceTime << ":"; //Duration
        stateRecord << tmpState.getState(); // State

        file << stateRecord.str() << std::endl; // change to Trace write.
      }

      // Summarized log of events not translated

      // WRITE PCF

      // WRITE ROW

      /*
      // old algorithm

      Initialize_Translation( parameters ))
      Translate(  parameters->otf_trace_name, Get_EVENTS_Name( parameters ), tmp_fd, pcf_fd );
      Generate_PCF( sym_otf_info, pcf_fd );
      Generate_Paraver_Header( header, prv_fd );
      Dump_Communications( comms );
      Merge_Files( prv_fd, tmp_fd, comm_fd );
      stats.otf_unmatched_sends    = Log_Unmatched_Comms( comms, ERROR_SEND );
      stats.otf_unmatched_receives = Log_Unmatched_Comms( unmatched_receives, ERROR_RECEIVE );
      Print_Statistics( &stats, parameters, argc, argv );
      Print_Otf_Symbolic_Info( sym_otf_info   );
      Finish( parameters );
      */
      file.close();

      translationReady = true;

    }
    else
    {
      std::cout << "ERROR: OTF2 Reader: Unable to open " << transData->strOTF2Trace << std::endl;
    }
  }

  return translationReady;
}


std::string buildPRVTraceName( const string &strOTF2Trace, const string &strPRVTrace )
{
  std::string auxPRVTrace( strPRVTrace );

  if ( strPRVTrace.compare( "" ) == 0 )
  {
    auxPRVTrace = strOTF2Trace + ".prv";
  }

  return auxPRVTrace;
}

std::string buildPCFFileName( const string &strOTF2Trace, const string &strPRVTrace )
{
  std::string auxPCFFileName( strPRVTrace );

  if ( strPRVTrace.compare( "" ) == 0 )
  {
    auxPCFFileName = strOTF2Trace + ".pcf";
  }
  else
  {
    auxPCFFileName = strPRVTrace.substr( 0, strPRVTrace.length() - 4 ) + ".pcf";
  }

  return auxPCFFileName;
}

struct color_t states_inf[STATES_NUMBER] = {
  {STATE_0, STATE0_LBL, STATE0_COLOR},
  {STATE_1, STATE1_LBL, STATE1_COLOR},
  {STATE_2, STATE2_LBL, STATE2_COLOR},
  {STATE_3, STATE3_LBL, STATE3_COLOR},
  {STATE_4, STATE4_LBL, STATE4_COLOR},
  {STATE_5, STATE5_LBL, STATE5_COLOR},
  {STATE_6, STATE6_LBL, STATE6_COLOR},
  {STATE_7, STATE7_LBL, STATE7_COLOR},
  {STATE_8, STATE8_LBL, STATE8_COLOR},
  {STATE_9, STATE9_LBL, STATE9_COLOR},
  {STATE_10, STATE10_LBL, STATE10_COLOR},
  {STATE_11, STATE11_LBL, STATE11_COLOR},
  {STATE_12, STATE12_LBL, STATE12_COLOR},
  {STATE_13, STATE13_LBL, STATE13_COLOR},
  {STATE_14, STATE14_LBL, STATE14_COLOR},
  {STATE_15, STATE15_LBL, STATE15_COLOR},
  {STATE_16, STATE16_LBL, STATE16_COLOR},
  {STATE_17, STATE17_LBL, STATE17_COLOR}
};

struct color_t gradient_inf[GRADIENT_NUMBER] = {
  {GRADIENT_0, GRADIENT0_LBL, GRADIENT0_COLOR},
  {GRADIENT_1, GRADIENT1_LBL, GRADIENT1_COLOR},
  {GRADIENT_2, GRADIENT2_LBL, GRADIENT2_COLOR},
  {GRADIENT_3, GRADIENT3_LBL, GRADIENT3_COLOR},
  {GRADIENT_4, GRADIENT4_LBL, GRADIENT4_COLOR},
  {GRADIENT_5, GRADIENT5_LBL, GRADIENT5_COLOR},
  {GRADIENT_6, GRADIENT6_LBL, GRADIENT6_COLOR},
  {GRADIENT_7, GRADIENT7_LBL, GRADIENT7_COLOR},
  {GRADIENT_8, GRADIENT8_LBL, GRADIENT8_COLOR},
  {GRADIENT_9, GRADIENT9_LBL, GRADIENT9_COLOR},
  {GRADIENT_10, GRADIENT10_LBL, GRADIENT10_COLOR},
  {GRADIENT_11, GRADIENT11_LBL, GRADIENT11_COLOR},
  {GRADIENT_12, GRADIENT12_LBL, GRADIENT12_COLOR},
  {GRADIENT_13, GRADIENT13_LBL, GRADIENT13_COLOR},
  {GRADIENT_14, GRADIENT14_LBL, GRADIENT14_COLOR}
};

void Paraver_state_labels( std::fstream& file )
{
  int i;

  file<<STATES_LBL<<endl;
  for (i = 0; i < STATES_NUMBER; i++)
  {
    file<<states_inf[i].value<<"    "<<states_inf[i].label<<endl;
  }

  LET_SPACES( file );
}

void Paraver_state_colors( std::fstream& file )
{
  int i;

  file<<STATES_COLOR_LBL<<endl;
  for (i = 0; i < STATES_NUMBER; i++)
  {
    file<<states_inf[i].value<<"    {"<<
          states_inf[i].rgb[0]<<","<<states_inf[i].rgb[1]<<","<<states_inf[i].rgb[2]<<"}"<<endl;
  }

  LET_SPACES( file );
}

void Paraver_gradient_colors( std::fstream& file )
{
  int i;

  file<<GRADIENT_COLOR_LBL<<endl;
  for (i = 0; i < GRADIENT_NUMBER; i++)
  {
    file<<gradient_inf[i].value<<"    {"<<
          gradient_inf[i].rgb[0]<<","<<gradient_inf[i].rgb[1]<<","<<gradient_inf[i].rgb[2]<<"}"<<endl;
  }

  LET_SPACES( file );
}

void Paraver_gradient_names( std::fstream& file )
{
  int i;

  file<<GRADIENT_LBL<<endl;
  for (i = 0; i < GRADIENT_NUMBER; i++)
    file<<gradient_inf[i].value<<"    "<<gradient_inf[i].label<<endl;

  LET_SPACES( file );
}

void Paraver_write_events( std::fstream& file, vector<EventDescription *>& usedEvents )
{
  prvEventType_t lastEventType;

  for( vector<EventDescription *>::iterator it = usedEvents.begin();
       it != usedEvents.end(); ++it )
  {
    if( it == usedEvents.begin() || (*it)->getType() != lastEventType )
    {
      lastEventType = (*it)->getType();
      LET_SPACES( file );
      file<<TYPE_LABEL<<endl;
      file<<"0    "<<(*it)->getType()<<"    "<<(*it)->getStrType()<<endl;
      if( (*it)->getValue() != 0 && (*it)->getStrValue() != "" )
      {
        file<<VALUES_LABEL<<endl;
        file<<"0   End"<<endl;
      }
    }
    if( (*it)->getValue() != 0 && (*it)->getStrValue() != "" )
    {
      file<<(*it)->getValue()<<"   "<<(*it)->getStrValue()<<endl;
    }
  }
}

void writePCFFile( string strPCFFileName, vector<EventDescription *>& usedEvents )
{
  std::fstream file;
  openPRV( strPCFFileName, file );

  if ( file.good() )
  {
    file<<"DEFAULT_OPTIONS\n\n";
    file<<"LEVEL               "<<DEFAULT_LEVEL<<endl;
    file<<"UNITS               "<<DEFAULT_UNITS<<endl;
    file<<"LOOK_BACK           "<<DEFAULT_LOOK_BACK<<endl;
    file<<"SPEED               "<<DEFAULT_SPEED<<endl;
    file<<"FLAG_ICONS          "<<DEFAULT_FLAG_ICONS<<endl;
    file<<"NUM_OF_STATE_COLORS "<<DEFAULT_NUM_OF_STATE_COLORS<<endl;
    file<<"YMAX_SCALE          "<<DEFAULT_YMAX_SCALE<<endl;

    LET_SPACES (file);

    file<<"DEFAULT_SEMANTIC\n\n";
    file<<"THREAD_FUNC          "<<DEFAULT_THREAD_FUNC<<endl;

    LET_SPACES( file );

    Paraver_state_labels( file );
    Paraver_state_colors( file );

    Paraver_gradient_colors( file );
    Paraver_gradient_names( file );

    Paraver_write_events( file, usedEvents );

    LET_SPACES( file );
  }

  file.close();
}

// *****************************************************************************
// MAIN
// *****************************************************************************
int main( int argc, char *argv[] )
{
  int globalError = 0;

  if ( argc == 1 )
    printHelp();
  else
  {
    // Initializations
    LocalKernel::init();
    KernelConnection *myKernel = new LocalKernel( NULL );
    ParaverConfig *config = ParaverConfig::getInstance();
    config->readParaverConfigFile();

    MainOptions options;
    options.showHelp = false;
    options.showVersion = false;
    options.printExternalTableExample = false;

    TranslationData transData;
    transData.myLog.enableLog( false );
    transData.useExternalTranslationTable = false;
    transData.correctedBeginTime = false;

    transData.strOTF2Trace = string( "" );

    string strPRVTrace = string( "" );
    string strExternalTable = string( "" );

    // Set parameters
    readParameters( argc, argv, options, transData, strPRVTrace, strExternalTable );

    if ( options.showHelp )
      printHelp();
    else if ( options.showVersion )
      printVersion();
    else if ( options.printExternalTableExample )
      printExternalTableExample();
    else if ( anyOTF2Trace( transData.strOTF2Trace ) )
    {
      initialize( transData );

      if ( translate( buildPRVTraceName( transData.strOTF2Trace, strPRVTrace ),
                      strExternalTable,
                      &transData ) )
      {
        std::cout << "Done." << std::endl;
      }
      else
      {
        // Error:
        globalError = -1;
      }

      vector<EventDescription *> usedEvents;
      EventList::getInstance()->getUsed( usedEvents );
      writePCFFile( buildPCFFileName( transData.strOTF2Trace, strPRVTrace), usedEvents );

      delete transData.resourcesModel;
      delete transData.processModel;
      delete transData.rowLabels;
    }
  }

  return globalError;
}
