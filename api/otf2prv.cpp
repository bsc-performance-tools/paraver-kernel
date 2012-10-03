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
#include <sstream>
#include <string>
#include <stack>
#include <map>
#include <wordexp.h>

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

using namespace std;

const string OTF2_VERSION_STRING = "0.17"; // Added communications

typedef struct MainOptionsStruct MainOptions;

struct MainOptionsStruct
{
  bool showHelp;
  bool showVersion;
};

typedef struct TranslationDataStruct TranslationData;

struct TranslationDataStruct
{
  OTF2_Reader *reader;
  std::fstream *PRVFile;

  bool printLog;
  std::ostream *logFile; // printLog == true => logFile exists and is open.

  TTimeUnit timeUnit;
  // PRV_UINT64 timerResolution;
  PRV_UINT64 maxTraceTime;
  PRV_UINT64 globalOffset;

  ResourceModel *resourcesModel; // to the Trace*
  ProcessModel  *processModel;   // to the Trace*
  RowLabels     *rowLabels;      // to the Trace*
  Trace         *trace;

  // PRV LOCAL MAPS --> TO THE API
  map< string, int > PRVEvent_ValueLabel2Value; //
  map< string, int > PRVEvent_TypeLabel2Type;   //
  map< int, int >    PRVEvent_Value2Type;     // N - 1

  map< uint32_t, int > OTF2Region2PRVEventValue;
  map< uint32_t, int > OTF2Region2PRVEventType;

  // Keep OTF2 definitions
  map< uint32_t, string >     symbols;
  map< uint32_t, uint32_t >   locationGroup2SystemTreeNode;
  map< uint32_t, TNodeOrder > systemTreeNode2GlobalNode;

  // Once dumpTrace is done, this maps can be deleted.
  // Here only for cout purposes.
  map< uint32_t, TTaskOrder >   location2Task;
  map< uint32_t, TThreadOrder > location2Thread;
  map< uint32_t, TCPUOrder >    location2CPU;

  //map< uint32_t, OTF2_TypeID > attributeType;
  map< uint32_t, string > attributeName;
  // States generation
  map< uint32_t, stack< TState > > lastState; // p.e: lastState[ location ].push( IDLE );
};


// *****************************************************************************
// 3rd LEVEL - PRV Write
// *****************************************************************************
void writeLog( TranslationData *tmpData,
               const string & message )
{
  if ( tmpData->printLog )
  {
    (*tmpData->logFile) << message << std::endl;
  }
}


void writeLog( TranslationData *tmpData,
               const string & message,
               const string & value )
{
  if ( tmpData->printLog )
  {
    (*tmpData->logFile) << message << ": " << value << std::endl;
  }
}


void writeLog( TranslationData *tmpData,
               const string & message,
               const uint32_t & value )
{
  if ( tmpData->printLog )
  {
    stringstream aux( message );
    aux << value;
    (*tmpData->logFile) << message << ": " << aux.str() << std::endl;
  }
}


void writeLog( TranslationData *tmpData,
               const string & message,
               const uint32_t & value1,
               const uint32_t & value2 )
{
  if ( tmpData->printLog )
  {
    (*tmpData->logFile) << message << ": ";

    stringstream aux;
    aux << value1;
    (*tmpData->logFile) << aux.str() << " - ";

    aux.clear();
    aux << value2;
    (*tmpData->logFile) << aux.str() << std::endl;
  }
}


void writeLog( TranslationData *tmpData,
               const string & message1,
               const string & message2,
               const uint32_t & value )
{
  if ( tmpData->printLog )
  {
    stringstream aux;
    aux << value;
    (*tmpData->logFile) << message1 << " " << message2 << ": " << aux.str() << std::endl;
  }
}


void writeHeaderTimes( TranslationData *tmpData )
{
  ostringstream ostr;

  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  *tmpData->PRVFile << "#Paraver ";
  *tmpData->PRVFile << "()" << ":";

  ostr << tmpData->maxTraceTime;
  *tmpData->PRVFile << ostr.str();

  *tmpData->PRVFile << "_" << LABEL_TIMEUNIT[ tmpData->timeUnit ];
  *tmpData->PRVFile << ":";
}


void writeHeaderResourceModel( TranslationData *tmpData )
{
  tmpData->resourcesModel->dumpToFile( *tmpData->PRVFile );
  *tmpData->PRVFile << ":";
}


void writeHeaderProcessModel( TranslationData *tmpData )
{
  tmpData->processModel->dumpToFile( *tmpData->PRVFile );
  *tmpData->PRVFile << ",0" << endl;
}


void writeComment( TranslationData *tmpData, const string &otf2Trace )
{
  wordexp_t wordInfo;
  char **auxPathExpanded;
  string otf2TraceExpanded;

  *tmpData->PRVFile << "# Translated from OTF2 trace ";

  wordexp( otf2Trace.c_str(), &wordInfo, 0 );
  auxPathExpanded = wordInfo.we_wordv;
  if ( wordInfo.we_wordc > 0 )
  {
    *tmpData->PRVFile << auxPathExpanded[ 0 ] << endl;
  }
  else
  {
    *tmpData->PRVFile << otf2Trace << endl;
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


SCOREP_Error_Code GlobDefClockPropertiesHandler( void*    userData,
                                                 uint64_t timer_resolution,
                                                 uint64_t global_offset,
                                                 uint64_t trace_length )
{
  TranslationData *transData = ( TranslationData * )userData;

  if ( timer_resolution <= 1E3 - 1 )
    transData->timeUnit = SEC;
  else if ( timer_resolution <= 1E6 - 1 )
    transData->timeUnit = MS;
  else if ( timer_resolution <= 1E9 - 1 )
    transData->timeUnit = US;
  else
    transData->timeUnit = NS;

  //transData->maxTraceTime = global_offset + trace_length;
  transData->maxTraceTime = trace_length;
  transData->globalOffset = global_offset;

  writeLog( transData, "[DEF] CLOCK (max, offset):", trace_length, global_offset );

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

  writeLog( transData, "[DEF-???] : CALLSITE" );
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

  writeLog( transData, "[DEF-???] : CALLPATH" );
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

  writeLog( transData, "[DEF-???] : MPICOMM" );

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

  writeLog( transData, "[DEF-???] : METRIC MEMBER" );
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

  writeLog( transData, "[DEF-???] : METRIC CLASS" );
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

  writeLog( transData, "[DEF-???] : METRIC INSTANCE" );
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

  writeLog( transData, "[DEF-???] : PARAMETER" );
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

  writeLog( transData, "[DEF-???] : UNKNOWN" );
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

  writeLog( transData, "[DEF-???] : MAPPING TABLE" );
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

  writeLog( transData, "[DEF-???] : " );

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefStringHandler( void*       userData,
                                        uint32_t    stringID,
                                        const char* string )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->symbols[ stringID ] = std::string( string );

  writeLog( transData, "[DEF] STRING", transData->symbols[ stringID ] );
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
    TApplOrder app;
    TTaskOrder task;
    TThreadOrder thread;
    transData->processModel->getThreadLocation( globalThread, app, task, thread );

    transData->location2Task[ locationID ] = task + 1;
    transData->location2Thread[ locationID ] = thread + 1;
    transData->location2CPU[ locationID ] = transData->resourcesModel->getLastCPU( currentNode );

    // Initialize lastState queue
    transData->lastState[ globalThread ] = stack< TState >();
  }

  // Set Callbacks for Local read
  OTF2_EvtReader* evt_reader = OTF2_Reader_GetEvtReader( transData->reader, locationID );
  OTF2_DefReader* def_reader = OTF2_Reader_GetDefReader( transData->reader, locationID );

  uint64_t definitions_read = 0;
  OTF2_Reader_ReadAllLocalDefinitions( transData->reader, def_reader, &definitions_read );


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

  map< string, int >::iterator it;

  it = transData->PRVEvent_ValueLabel2Value.find( transData->symbols[ name ] );
  if (  ( it != transData->PRVEvent_ValueLabel2Value.end() ) )
  {
    // TYPE symbolic - VALUE symbolic
    transData->OTF2Region2PRVEventValue[ regionID ] = it->second;
    writeLog( transData, "[DEF] REGION as VALUE: ", transData->symbols[ name ] );
  }
  else
  {
    it = transData->PRVEvent_TypeLabel2Type.find( transData->symbols[ name ] );

    if ( it != transData->PRVEvent_TypeLabel2Type.end() )
    {
      // TYPE symbolic - VALUE variable
      transData->OTF2Region2PRVEventType[ regionID ] = it->second;
      writeLog( transData, "[DEF] REGION as TYPE : ", transData->symbols[ name ] );
    }
    else
    {
      // Careful with colision
      // TODO: Substitute by API call
      // transData->PRVEvent_TypeLabel2Value[ transData->symbols[ name ] ] = getNewType( transData->symbols[ name ] );
      transData->PRVEvent_TypeLabel2Type[ transData->symbols[ name ] ] = USER_FUNCTION;
      transData->OTF2Region2PRVEventType[ regionID ] = USER_FUNCTION;
      writeLog( transData, "[DEF] REGION as USER FUNCTION : ", transData->symbols[ name ] );
    }
  }

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

  writeLog( transData, "[DEF] ATTRIBUTE : ", transData->symbols[ name ], attributeID );

  return  SCOREP_SUCCESS;
}


SCOREP_Error_Code BufferFlushHandler( uint64_t            locationID,
                                      uint64_t            time,
                                      void*               userData,
                                      OTF2_AttributeList* attributes,
                                      OTF2_TimeStamp      stopTime )
{
  TranslationData *transData = ( TranslationData * )userData;

  *transData->PRVFile << "# OTF2 event not translated: BufferFlush" << endl;

  writeLog( transData, "[EVT-???] : BufferFlush" );

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

  *transData->PRVFile << "# OTF2 event not translated: MeasurementOnOff" << endl;

  writeLog( transData, "[EVT-???] : MeasurementOnOff" );

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

  *transData->PRVFile << "# OTF2 event not translated: MpiIsend" << endl;

  writeLog( transData, "[EVT-???] : MpiIsend" );

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

  *transData->PRVFile << "# OTF2 event not translated: MpiIsendComplete" << endl;

  writeLog( transData, "[EVT-???] : MpiIsendComplete" );

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

  *transData->PRVFile << "# OTF2 event not translated: MpiIrecvRequest" << endl;

  writeLog( transData, "[EVT-???] : MpiIrecvRequest" );

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

  *transData->PRVFile << "# OTF2 event not translated: MpiRecv" << endl;

  writeLog( transData, "[EVT-???] : MpiRecv" );
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

  *transData->PRVFile << "# OTF2 event not translated: MpiIrecv" << endl;

  writeLog( transData, "[EVT-???] : MpiIrecv" );

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

  *transData->PRVFile << "# OTF2 event not translated: MpiRequestTest" << endl;

  writeLog( transData, "[EVT-???] : MpiRequestTest" );

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

  *transData->PRVFile << "# OTF2 event not translated: MpiRequestCancelled" << endl;

  writeLog( transData, "[EVT-???] : MpiRequestCancelled" );

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

  *transData->PRVFile << "# OTF2 event not translated: MpiCollectiveBegin" << endl;

  writeLog( transData, "[EVT-???] : MpiCollectiveBegin" );
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

  *transData->PRVFile << "# OTF2 event not translated: MpiCollectiveEnd" << endl;

  writeLog( transData, "[EVT-???] : MpiCollectiveEnd" );

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

  *transData->PRVFile << "# OTF2 event not translated: OmpFork" << endl;

  writeLog( transData, "[EVT-???] : OmpFork" );

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

  *transData->PRVFile << "# OTF2 event not translated: OmpJoin" << endl;

  writeLog( transData, "[EVT-???] : OmpJoin" );

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

  *transData->PRVFile << "# OTF2 event not translated: OmpAcquireLock" << endl;

  writeLog( transData, "[EVT-???] : OmpAcquireLock" );

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

  *transData->PRVFile << "# OTF2 event not translated: OmpReleaseLock" << endl;

  writeLog( transData, "[EVT-???] : OmpReleaseLock" );

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

  *transData->PRVFile << "# OTF2 event not translated: OmpTaskCreate" << endl;

  writeLog( transData, "[EVT-???] : OmpTaskCreate" );

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

  *transData->PRVFile << "# OTF2 event not translated: OmpTaskSwitch" << endl;

  writeLog( transData, "[EVT-???] : OmpTaskSwitch" );

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

  *transData->PRVFile << "# OTF2 event not translated: OmpTaskComplete" << endl;

  writeLog( transData, "[EVT-???] : OmpTaskComplete" );

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

  *transData->PRVFile << "# OTF2 event not translated: Metric" << endl;

  writeLog( transData, "[EVT-???] : Metric" );
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

  *transData->PRVFile << "# OTF2 event not translated: ParameterString" << endl;

  writeLog( transData, "[EVT-???] : ParameterString" );
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

  *transData->PRVFile << "# OTF2 event not translated: ParameterInt" << endl;

  writeLog( transData, "[EVT-???] : ParameterInt" );
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

  *transData->PRVFile << "# OTF2 event not translated: ParameterUnsignedInt" << endl;

  writeLog( transData, "[EVT-???] : ParameterUnsignedInt" );
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

  *transData->PRVFile << "# OTF2 event not translated: Unknown" << endl;

  writeLog( transData, "[EVT-???] : Unknown" );
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

  writeLog( transData, "[EVT] MpiSend : ", locationID, receiver );

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

      commRecord << time - transData->globalOffset << ":"; // Logical Send
      commRecord << time - transData->globalOffset << ":"; // Physical Send

      // Receiver
      commRecord << transData->location2CPU[ receiver ] << ":"; // CPU
      commRecord << transData->processModel->totalApplications() << ":"; // APP
      commRecord << transData->location2Task[ receiver ] << ":"; // TASK
      commRecord << transData->location2Thread[ receiver ] << ":"; // THREAD

      commRecord << value.uint64 - transData->globalOffset << ":"; // Logical Receive
      commRecord << value.uint64 - transData->globalOffset << ":"; // Physical Receive

      // Tag:Length
      commRecord << msgTag << ":";
      commRecord << msgLength << std::endl;

      *transData->PRVFile << commRecord.str(); // change to Trace write.

      writeLog( transData, commRecord.str() );
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

  map< uint32_t, int >::iterator it;
  it = transData->OTF2Region2PRVEventValue.find( regionID );
  if ( it != transData->OTF2Region2PRVEventValue.end() )
  {
    stringstream eventRecord;
    eventRecord << fixed;
    eventRecord << dec;
    eventRecord.precision( 0 );

    eventRecord << "2" << ":";
    eventRecord << transData->location2CPU[ locationID ] << ":"; // CPU
    eventRecord << transData->processModel->totalApplications() << ":"; // APP
    eventRecord << transData->location2Task[ locationID ] << ":"; // TASK
    eventRecord << transData->location2Thread[ locationID ] << ":"; // THREAD
    // eventRecord << time << ":";
    eventRecord << time - transData->globalOffset << ":";
    eventRecord << transData->PRVEvent_Value2Type[ it->second ] << ":";
    eventRecord << it->second;

    *transData->PRVFile << eventRecord.str() << std::endl; // change to Trace write.

    writeLog( transData, eventRecord.str() );
  }
  else
  {
    it = transData->OTF2Region2PRVEventType.find( regionID );
    if ( it != transData->OTF2Region2PRVEventType.end() )
    {
      stringstream eventRecord;
      eventRecord << fixed;
      eventRecord << dec;
      eventRecord.precision( 0 );

      eventRecord << "2" << ":";
      eventRecord << transData->location2CPU[ locationID ] << ":"; // CPU
      eventRecord << transData->processModel->totalApplications() << ":"; // APP
      eventRecord << transData->location2Task[ locationID ] << ":"; // TASK
      eventRecord << transData->location2Thread[ locationID ] << ":"; // THREAD
      // eventRecord << time << ":";
      eventRecord << time - transData->globalOffset << ":";
      eventRecord << it->second << ":";
      eventRecord << regionID;

      *transData->PRVFile << eventRecord.str()  << std::endl; // change to Trace write.

      writeLog( transData, eventRecord.str() );
    }
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

  map< uint32_t, int >::iterator it = transData->OTF2Region2PRVEventValue.find( regionID );
  if ( it != transData->OTF2Region2PRVEventValue.end() )
  {
    stringstream eventRecord;
    eventRecord << fixed;
    eventRecord << dec;
    eventRecord.precision( 0 );

    eventRecord << "2" << ":";
    eventRecord << transData->location2CPU[ locationID ] << ":"; // CPU
    eventRecord << transData->processModel->totalApplications() << ":"; // APP
    eventRecord << transData->location2Task[ locationID ] << ":"; // TASK
    eventRecord << transData->location2Thread[ locationID ] << ":"; // THREAD
    // eventRecord << time  << ":";
    eventRecord << time - transData->globalOffset << ":";
    eventRecord << transData->PRVEvent_Value2Type[ it->second ] << ":";
    eventRecord << "0"; // VALUE

    *transData->PRVFile << eventRecord.str() << std::endl;  // change to Trace write.

    writeLog( transData, eventRecord.str() );
  }
  else
  {
    it = transData->OTF2Region2PRVEventType.find( regionID );
    if ( it != transData->OTF2Region2PRVEventType.end() )
    {
      stringstream eventRecord;
      eventRecord << fixed;
      eventRecord << dec;
      eventRecord.precision( 0 );

      eventRecord << "2" << ":";
      eventRecord << transData->location2CPU[ locationID ] << ":"; // CPU
      eventRecord << transData->processModel->totalApplications() << ":"; // APP
      eventRecord << transData->location2Task[ locationID ] << ":"; // TASK
      eventRecord << transData->location2Thread[ locationID ] << ":"; // THREAD
      // eventRecord << time << ":";
      eventRecord << time - transData->globalOffset << ":";
      eventRecord << it->second << ":";
      eventRecord << "0";  // VALUE

      *transData->PRVFile << eventRecord.str()  << std::endl; // change to Trace write.

      writeLog( transData, eventRecord.str() );
    }
  }

  return SCOREP_SUCCESS;
}


// *****************************************************************************
// 1st LEVEL - GENERAL
// *****************************************************************************

void printHelp()
{
  std::cout << "USAGE" << std::endl;
  std::cout << "  otf2prv [OPTION] trc.otf2 [trc.prv]" << std::endl;
  std::cout << std::endl;
  std::cout << "    -h, --help: Prints this help" << std::endl;
  std::cout << "    -v, --version: Show version" << std::endl;
  std::cout <<  "[in development]    -l [file], --log: Set log mode "
                "to the stdout or to file, if present."  << std::endl;
  std::cout << std::endl;
  std::cout << "  Parameters:" << std::endl;
  std::cout << "    trc.otf2: OTF2 trace filename." << std::endl;
  std::cout << "    trc.prv: Paraver trace filename." << std::endl;
  std::cout << std::endl;
  std::cout << "  Examples:" << std::endl;
  std::cout << "    otf2prv linpack.otf2" << std::endl;
  std::cout << "      Generates translation from linpack OTF2 trace to linpack.prv PRV trace." << std::endl;
  std::cout << std::endl;
}


void printVersion()
{
  // std::cout << PACKAGE_STRING;
  std::cout << OTF2_VERSION_STRING;

  bool reverseOrder = true;
  string auxDate = LabelConstructor::getDate( reverseOrder );

  if ( auxDate.compare("") != 0 )
    std::cout << " Build ";

  std::cout << auxDate << std::endl;
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
    transData.printLog = true;
  }
  else
    std::cout << "Unknown option " << argument << std::endl;
}


void readParameters( int argc,
                     char *arguments[],
                     MainOptions &options,
                     TranslationData &transData,
                     string &strOTF2Trace,
                     string &strPRVTrace )
{
  PRV_INT32 currentArg = 1;

  while ( currentArg < argc )
  {
    if ( isOption( arguments[ currentArg ] ))
    {
      activateOption( arguments[ currentArg ], options, transData );
    }
    else if ( Trace::isTraceFile( string( arguments[ currentArg ] )))
    {
      strPRVTrace = string( arguments[ currentArg ] );
    }
    else if ( Trace::isOTF2TraceFile( string( arguments[ currentArg ] )))
    {
      strOTF2Trace = string( arguments[ currentArg ] );
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


void initialize( TranslationData &tmpData )
{
  tmpData.timeUnit = NS;

  tmpData.maxTraceTime = 0;
  tmpData.globalOffset = 0;

  tmpData.resourcesModel = new ResourceModel();
  tmpData.processModel = new ProcessModel();
  tmpData.processModel->addApplication();
  tmpData.rowLabels = new RowLabels();

  writeLog( &tmpData, "[REC] Labels for MPI types" );
  for( uint32_t i = 0; i < NUM_MPI_BLOCK_GROUPS; ++i )
  {
    writeLog( &tmpData, "[REC]     ", prv_block_groups[ i ].label, prv_block_groups[ i ].type );
    tmpData.PRVEvent_TypeLabel2Type[ prv_block_groups[ i ].label ] = prv_block_groups[ i ].type;
  }

  writeLog( &tmpData, "[REC] Labels for MPI values" );
  for( uint32_t i = 0; i < NUM_MPI_PRV_ELEMENTS; ++i )
  {
    writeLog( &tmpData, "[REC]     ", mpi_prv_val_label[ i ].label, mpi_prv_val_label[ i ].value );
    tmpData.PRVEvent_ValueLabel2Value[ string( mpi_prv_val_label[ i ].label ) ] = mpi_prv_val_label[ i ].value;
  }

  writeLog( &tmpData, "[REC] Declare values of every type" );
  for( uint32_t i = 0; i < NUM_MPI_PRV_ELEMENTS; ++i )
  {
    writeLog( &tmpData, "[REC]     ", event_mpit2prv[ i ].tipus_prv, event_mpit2prv[ i ].valor_prv );
    tmpData.PRVEvent_Value2Type[ event_mpit2prv[ i ].valor_prv ] = event_mpit2prv[ i ].tipus_prv;
  }
}


bool translate( const string &strOTF2Trace,
                const string &strPRVTrace,
                TranslationData *tmpData )
{
  bool translationReady = false;
  std::fstream file;
  openPRV( strPRVTrace, file );

  if ( file.good() )
  {
    writeLog( tmpData, "[FILE] Opened ", strPRVTrace );
    OTF2_Reader* reader = OTF2_Reader_Open( strOTF2Trace.c_str() );
    if ( reader != NULL )
    {
      writeLog( tmpData, "[FILE] Opened ", strOTF2Trace );

      tmpData->reader = reader;
      tmpData->PRVFile = &file;

      // BUILD HEADER
      writeLog( tmpData, "[REC] Registering OTF2 Callbacks" );

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

      OTF2_Reader_RegisterGlobalDefCallbacks( reader, global_def_reader, global_def_callbacks, (void *)tmpData );
      OTF2_GlobalDefReaderCallbacks_Delete( global_def_callbacks );




      uint64_t definitions_read = 0;
      OTF2_Reader_ReadAllGlobalDefinitions( reader, global_def_reader, &definitions_read );

      tmpData->resourcesModel->setReady( true );
      tmpData->processModel->setReady( true );

      // WRITE HEADER

      writeHeaderTimes( tmpData );
      writeHeaderResourceModel( tmpData );
      writeHeaderProcessModel( tmpData );
      // writeHeaderCommunicators( tmpData ); // must be written by proccess model

      writeComment( tmpData, strOTF2Trace );

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

      OTF2_Reader_RegisterGlobalEvtCallbacks( reader, global_evt_reader, event_callbacks, (void *)tmpData );
      OTF2_GlobalEvtReaderCallbacks_Delete( event_callbacks );

      // Read simbolic info
      // Read until last time
      uint64_t events_read = 0;
      OTF2_Reader_ReadAllGlobalEvents( reader, global_evt_reader, &events_read );

      // Close otf2 trace
      OTF2_Reader_Close( reader );

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
      std::cout << "ERROR: OTF2 Reader: Unable to open " << strOTF2Trace << std::endl;
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

    TranslationData tmpData;
    tmpData.printLog = false;
    tmpData.logFile = &std::cout;

    string strOTF2Trace = string( "" );
    string strPRVTrace = string( "" );

    // Set parameters
    readParameters( argc, argv, options, tmpData, strOTF2Trace, strPRVTrace );

    if ( options.showHelp )
      printHelp();
    else if ( options.showVersion )
      printVersion();
    else if ( anyOTF2Trace( strOTF2Trace ) )
    {
      initialize( tmpData );

      if ( translate( strOTF2Trace,
                      buildPRVTraceName( strOTF2Trace, strPRVTrace ),
                      &tmpData ) )
      {
        std::cout << "Done." << std::endl;
      }
      else
      {
        // Error:
        globalError = -1;
      }

      delete tmpData.resourcesModel;
      delete tmpData.processModel;
      delete tmpData.rowLabels;
    }
  }

  return globalError;
}
