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
#include <map>

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

const string OTF2_VERSION_STRING = "0.16"; // Added level wirte

Trace *trace;

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
  bool translateStates;
  bool translateEvents;
  bool translateCommunications;

  TTimeUnit timeUnit;
  // PRV_UINT64 timerResolution;
  PRV_UINT64 maxTraceTime;
  PRV_UINT64 globalOffset;

  ResourceModel *resourcesModel;
  ProcessModel *processModel;
  RowLabels *rowLabels;
  map< uint32_t, string > symbols;
  map< uint32_t, uint32_t > locationGroup2SystemTreeNode;
  map< uint32_t, TNodeOrder > systemTreeNode2GlobalNode;

  map< uint32_t, TTaskOrder > location2Task;
  map< uint32_t, TThreadOrder > location2Thread;
  map< uint32_t, TCPUOrder > location2CPU;

  map< string, int > PRVEvent_ValueLabel2Value;
  map< int, int >    PRVEvent_Value2Type;
  map< int, string > PRVEvent_Type2TypeLabel;

  map< string, int > PRVEvent_TypeLabel2Value;

  // MPI maps values
  map< uint32_t, int > OTF2Region2PRVEventValue;
  map< uint32_t, int > OTF2Region2PRVEventType;
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
}


void writeHeaderCommunicators( TranslationData *tmpData )
{
  *tmpData->PRVFile << ",0" << endl;
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

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefStringHandler( void*       userData,
                                        uint32_t    stringID,
                                        const char* string )
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->symbols[ stringID ] = std::string( string );

  writeLog( transData, "Registering STRING", transData->symbols[ stringID ] );
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


/*
     otf2Handler_add_location_group( data,
                                    groupID,
                                    name );

     // Print definition if selected.
     if ( otf2_GLOBDEFS )
     {
         printf( "%-*s %12u  Name: %s, Type: %s, Parent: %s\n",
                 otf2_DEF_COLUMN_WIDTH, "LOCATION_GROUP",
                 groupID,
                 otf2Handler_get_def_name( data->strings, name ),
                 otf2Handler_get_location_group_type( type ),
                 otf2Handler_get_def_name( data->system_tree_nodes,
                                          systemTreeParent ) );
     }

        if ( systemTreeParent != OTF2_UNDEFINED_UINT32 )
        {
        }
    }
*/
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

  }

  // PROGRAM LOCAL READ
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

  bool found;
  map< string, int >::iterator it =
          transData->PRVEvent_ValueLabel2Value.find( transData->symbols[ name ] );

  found = ( it != transData->PRVEvent_ValueLabel2Value.end() );
  if ( found )
  {
    transData->OTF2Region2PRVEventValue[ regionID ] = it->second;
    writeLog( transData, "Registered REGION as VALUE: ", transData->symbols[ name ] );
  }
  else
  {
    map< string, int >::iterator it2 =
            transData->PRVEvent_TypeLabel2Value.find( transData->symbols[ name ] );

    found = ( it2 != transData->PRVEvent_TypeLabel2Value.end() );
    if ( found )
    {
      transData->OTF2Region2PRVEventType[ regionID ] = it2->second;
      writeLog( transData, "Registered REGION as TYPE : ", transData->symbols[ name ] );
    }
    else
    {
      writeLog( transData, "** NOT FOUND ", transData->symbols[ name ] );
    }
  }

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
/*
  printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Receiver: %u, "
            "Communicator: %s, "
            "Tag: %u, Length: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_SEND",
            locationID, time,
            receiver,
            otf2Handler_get_def_name( data->mpi_comms, communicator ),
            msgTag,
            msgLength );

    otf2Handler_attributes( data, attributes );
*/


    return SCOREP_SUCCESS;
}
// ******* de la web


SCOREP_Error_Code EnterHandler( uint64_t locationID,
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
    // eventRecord << time << ":";
    eventRecord << time - transData->globalOffset << ":";
    eventRecord << transData->PRVEvent_Value2Type[ it->second ] << ":";
    eventRecord << it->second << std::endl;

    *transData->PRVFile << eventRecord.str(); // change to Trace write.

    writeLog( transData, eventRecord.str() );
  }
  else
  {

    //map< uint32_t, int >::iterator it2 =
      //      transData->OTF2Region2PRVTypeValue.find( regionID );
  }


  return SCOREP_SUCCESS;
}


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
    eventRecord << "0" << std::endl; // VALUE

    *transData->PRVFile << eventRecord.str();  // change to Trace write.

    writeLog( transData, eventRecord.str() );
  }
  else
  {

    //map< uint32_t, int >::iterator it2 =
      //      transData->OTF2Region2PRVTypeValue.find( regionID );
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
  std::cout <<  "[in development]    -l [file], --log: Set verbose mode"
        " and  the stdout, or if present, to file." << std::endl;
  std::cout << "[in development]    -i, --info: Translate only pcf and row files."
        " Also an empty prv including only header and idle states." << std::endl;
  std::cout << "[in development]    -e, --events: Translate event records." << std::endl;
  std::cout << "[in development]    -s, --states: Translate states records." << std::endl;
  std::cout << "[in development]    -c, --communications: Translate communication records." << std::endl;
  std::cout << " By default all kind of records are translated. If a subset is wanted,"
        " its specific options must be given." << std::endl;
  std::cout << std::endl;
  std::cout << "  Parameters:" << std::endl;
  std::cout << "    trc.otf2: OTF2 trace filename." << std::endl;
  std::cout << "    trc.prv: Paraver trace filename." << std::endl;
  std::cout << std::endl;
  std::cout << "  Examples:" << std::endl;
  std::cout << "    otf2prv linpack.otf2" << std::endl;
  std::cout << "      Generates translation from linpack OTF2 trace to linpack.prv PRV trace." << std::endl;
  std::cout << "    otf2prv -e -c linpack.otf2" << std::endl;
  std::cout << "      Same than before, but including only events." << std::endl;
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
  transData.translateStates = false;
  transData.translateEvents = false;
  transData.translateCommunications = false;

  if ( argument[ 1 ] == 'h' )
    options.showHelp = true;
  else if ( argument[ 1 ] == 'v' )
    options.showVersion = true;
  else if ( argument[ 1 ] == 'l')
  {
    transData.printLog = true;
  }
  else if ( argument[ 1 ] == 's')
    transData.translateStates = true;
  else if ( argument[ 1 ] == 'e' )
    transData.translateEvents = true;
  else if ( argument[ 1 ] == 'c')
    transData.translateCommunications = true;
  else
    std::cout << "Unknown option " << argument << std::endl;

  // If no translation option about records is given, default is translate them all.
  if ( !transData.translateStates && !transData.translateStates && !transData.translateCommunications )
  {
    transData.translateStates = true;
    transData.translateEvents = true;
    transData.translateCommunications = true;
  }
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
std::cout << "initialize:: " << endl;
  tmpData.timeUnit = NS;
  tmpData.resourcesModel = new ResourceModel();
  tmpData.processModel = new ProcessModel();
  tmpData.processModel->addApplication();
  tmpData.rowLabels = new RowLabels();

std::cout << "MPI types" << std::endl;
  for( uint32_t i = 0; i < NUM_MPI_BLOCK_GROUPS; ++i )
  {
std::cout << prv_block_groups[ i ].type << " "<< prv_block_groups[ i ].label << std::endl;
    tmpData.PRVEvent_Type2TypeLabel[ prv_block_groups[ i ].type ] = string( prv_block_groups[ i ].label );
  }

std::cout << "MPI values" << std::endl;
  for( uint32_t i = 0; i < NUM_MPI_PRV_ELEMENTS; ++i )
  {
std::cout << string( mpi_prv_val_label[ i ].label ) << " "<< mpi_prv_val_label[ i ].value << std::endl;
    tmpData.PRVEvent_ValueLabel2Value[ string( mpi_prv_val_label[ i ].label ) ] = mpi_prv_val_label[ i ].value;
  }

std::cout << "MPI values to types" << std::endl;
  for( uint32_t i = 0; i < NUM_MPI_PRV_ELEMENTS; ++i )
  {
std::cout << event_mpit2prv[ i ].valor_prv << " "<< event_mpit2prv[ i ].tipus_prv << std::endl;
    tmpData.PRVEvent_Value2Type [ event_mpit2prv[ i ].valor_prv ] = event_mpit2prv[ i ].tipus_prv;
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
    OTF2_Reader* reader = OTF2_Reader_Open( strOTF2Trace.c_str() );
    if ( reader != NULL )
    {
      tmpData->reader = reader;
      tmpData->PRVFile = &file;

      // BUILD HEADER
      // Initialize callbacks for get last time
      OTF2_GlobalDefReader* global_def_reader = OTF2_Reader_GetGlobalDefReader( reader );
      OTF2_GlobalDefReaderCallbacks* global_def_callbacks = OTF2_GlobalDefReaderCallbacks_New();

      OTF2_GlobalDefReaderCallbacks_SetClockPropertiesCallback( global_def_callbacks, GlobDefClockPropertiesHandler );
      OTF2_GlobalDefReaderCallbacks_SetStringCallback( global_def_callbacks, GlobDefStringHandler );
      OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeCallback( global_def_callbacks, GlobDefSystemTreeNodeHandler );
      OTF2_GlobalDefReaderCallbacks_SetLocationGroupCallback( global_def_callbacks, GlobDefLocationGroupHandler );
      OTF2_GlobalDefReaderCallbacks_SetLocationCallback( global_def_callbacks, GlobDefLocationHandler );
      OTF2_GlobalDefReaderCallbacks_SetRegionCallback( global_def_callbacks, GlobDefRegionHandler );
      //OTF2_GlobalDefReaderCallbacks_SetGroupCallback( global_def_callbacks, GlobDefGroupHandler );
      //OTF2_GlobalDefReaderCallbacks_SetMpiCommCallback( global_def_callbacks, GlobDefMpiCommHandler );

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
      writeHeaderCommunicators( tmpData );

      // TRANSLATE EVENTS
      // Initialize callbacks for events
      // Write events
      // Read states
      // Read communications
      OTF2_GlobalEvtReader* global_evt_reader = OTF2_Reader_GetGlobalEvtReader( reader );
      OTF2_GlobalEvtReaderCallbacks* event_callbacks = OTF2_GlobalEvtReaderCallbacks_New();
      OTF2_GlobalEvtReaderCallbacks_SetEnterCallback( event_callbacks, EnterHandler );
      OTF2_GlobalEvtReaderCallbacks_SetLeaveCallback( event_callbacks, LeaveHandler );
      OTF2_GlobalEvtReaderCallbacks_SetMpiSendCallback( event_callbacks, MpiSendHandler );


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
