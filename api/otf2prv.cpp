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
#include <string>

#include <otf2/otf2.h>
#include <scorep_utility/SCOREP_UtilityTypes.h>
#include "otf2prv.h"

#include "prvtypes.h"
#include "localkernel.h"
#include "paraverconfig.h"
#include "trace.h"
#include "labelconstructor.h" // for getDate
#include "paraverlabels.h"
#include "resourcemodel.h"
#include "processmodel.h"

//using std::string;
using namespace std;
//
const string OTF2_VERSION_STRING = "0.9";

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
  bool verbose;

  TTimeUnit timeUnit;
  // PRV_UINT64 timerResolution;
  PRV_UINT64 maxTraceTime;

  ResourceModel *resourcesModel;
  ProcessModel *processModel;
  RowLabels *rowLabels;
  map< uint32_t, string > symbols;
  map< uint32_t, uint32_t > locationGroup2SystemTreeNode;
  map< uint32_t, TNodeOrder > systemTreeNode2GlobalNode;
};


// *****************************************************************************
// 3rd LEVEL - PRV Write
// *****************************************************************************

void writeHeaderTimes( TranslationData *tmpData, std::fstream &file )
{
  ostringstream ostr;

  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  file << "#Paraver ";
  file << "()" << ':';

  ostr << tmpData->maxTraceTime;
  file << ostr.str();

  file << "_" << LABEL_TIMEUNIT[ tmpData->timeUnit ];
  file << ':';
}


void writeHeaderResourceModel( TranslationData *tmpData, std::fstream &file )
{
  tmpData->resourcesModel->dumpToFile( file );
  file << ':';
}


void writeHeaderProcessModel( TranslationData *tmpData, std::fstream &file )
{
  tmpData->processModel->dumpToFile( file );
}


void writeHeaderCommunicators( TranslationData *tmpData, std::fstream &file )
{
  file << ",0" << endl;
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


SCOREP_Error_Code
GlobDefClockProperties_print
(
    void*    userData,
    uint64_t timer_resolution,
    uint64_t global_offset,
    uint64_t trace_length
)
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

  transData->maxTraceTime = global_offset + trace_length;

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code
GlobDefString_print
(
    void*       userData,
    uint32_t    stringID,
    const char* string
)
{
  TranslationData *transData = ( TranslationData * )userData;

  transData->symbols[ stringID ] = std::string( string );
}


SCOREP_Error_Code
GlobDefSystemTreeNode_print
(
    void*    userData,
    uint32_t nodeID,
    uint32_t name,
    uint32_t className,
    uint32_t nodeParent
)
{
  TranslationData *transData = ( TranslationData * )userData;

  // Translation: node of nodes => SYSTEM, NODE
  // How to translate many systems?
  // Many systems to one system and all the nodes merged?
std::cout << nodeID << " " << name << " " << className << " "<< nodeParent << std::endl;

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
SCOREP_Error_Code
GlobDefLocationGroup_print
(
    void*                  userData,
    uint32_t               groupID,
    uint32_t               name,
    OTF2_LocationGroupType type,
    uint32_t               systemTreeParent
)
{
  TranslationData *transData = ( TranslationData * )userData;

  TApplOrder currentApplication = transData->processModel->totalApplications() - 1;
  transData->processModel->addTask( currentApplication );
  transData->rowLabels->pushBack( TASK, transData->symbols[ name ] );
  transData->locationGroup2SystemTreeNode[ groupID ] = systemTreeParent; // undefined?


/*
     otf2_print_add_location_group( data,
                                    groupID,
                                    name );

     // Print definition if selected.
     if ( otf2_GLOBDEFS )
     {
         printf( "%-*s %12u  Name: %s, Type: %s, Parent: %s\n",
                 otf2_DEF_COLUMN_WIDTH, "LOCATION_GROUP",
                 groupID,
                 otf2_print_get_def_name( data->strings, name ),
                 otf2_print_get_location_group_type( type ),
                 otf2_print_get_def_name( data->system_tree_nodes,
                                          systemTreeParent ) );
     }

        if ( systemTreeParent != OTF2_UNDEFINED_UINT32 )
        {
        }
    }
*/
    return SCOREP_SUCCESS;
}

SCOREP_Error_Code
GlobDefLocation_print
(
    void*             userData,
    uint64_t          locationID,
    uint32_t          name,
    OTF2_LocationType locationType,
    uint64_t          numberOfEvents,
    uint32_t          locationGroup
)
{
  TranslationData *transData = ( TranslationData * )userData;

  // Adds CPU and THREAD to a NODE - TASK
  if ( locationType == OTF2_LOCATION_TYPE_CPU_THREAD )
  {
    TApplOrder currentApplication = transData->processModel->totalApplications() - 1;
    TNodeOrder currentNode = transData->systemTreeNode2GlobalNode[
            transData->locationGroup2SystemTreeNode[ locationGroup ] ];

    transData->processModel->addThread( currentApplication, locationGroup, currentNode );
    transData->resourcesModel->addCPU( currentNode );
  }
/*
    otf2_print_add_location( data, locationID, name );

    // Print definition if selected.
    if ( otf2_GLOBDEFS )
    {
        printf( "%-*s %12" PRIu64 "  Name: %s, Type: %s, "
                "# Events: %" PRIu64 ", Group: %s\n",
                otf2_DEF_COLUMN_WIDTH, "LOCATION",
                locationID,
                otf2_print_get_def_name( data->strings, name ),
                otf2_print_get_location_type( locationType ),
                numberOfEvents,
                otf2_print_get_def_name( data->location_groups, locationGroup ) );
    }


    // Only proceed if either no local location is selected (i.e. read all) or
    // location ID matches provided location ID.
    if ( otf2_LOCAL != OTF2_UNDEFINED_UINT64 && otf2_LOCAL != locationID )
    {
        return SCOREP_SUCCESS;
    }

    // add location to the list of locations to read events from
    otf2_print_add_location_to_read( data, locationID );

    otf2_LOCAL_FOUND = true;
*/
   return SCOREP_SUCCESS;
}

SCOREP_Error_Code
MpiSend_print
(
    uint64_t            locationID,
    uint64_t            time,
    void*               userData,
    OTF2_AttributeList* attributes,
    uint32_t            receiver,
    uint32_t            communicator,
    uint32_t            msgTag,
    uint64_t            msgLength
)
{
  TranslationData *transData = ( TranslationData * )userData;
/*
  printf( "%-*s %15" PRIu64 " %20" PRIu64 "  Receiver: %u, "
            "Communicator: %s, "
            "Tag: %u, Length: %" PRIu64 "\n",
            otf2_EVENT_COLUMN_WIDTH, "MPI_SEND",
            locationID, time,
            receiver,
            otf2_print_get_def_name( data->mpi_comms, communicator ),
            msgTag,
            msgLength );

    otf2_print_attributes( data, attributes );
*/


    return SCOREP_SUCCESS;
}
// ******* de la web


SCOREP_Error_Code Enter_print( uint64_t locationID,
                               uint64_t time,
                               void* userData,
                               OTF2_AttributeList* attributes,
                               uint32_t regionID )
{
  cout << "Entering region " << regionID;
  cout << "at location: " << locationID;
  cout << " at time " << time << endl;
  //printf( "Entering region %u at location: %" PRIu64 " at time %" PRIu64 ".\n", regionID, locationID, time );

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code Leave_print( uint64_t locationID,
                               uint64_t time,
                               void* userData,
                               OTF2_AttributeList* attributes,
                               uint32_t regionID )
{
  cout << "Entering region " << regionID;
  cout << " at location: " << locationID;
  cout << " at time " << time << endl;
//  printf( "Leaving region %u at location: %" PRIu64 " at time %" PRIu64 ".\n", regionID, locationID, time );

  return SCOREP_SUCCESS;
}


SCOREP_Error_Code GlobDefLocation_Register( void*             userData,
                                            uint64_t          locationIdentifier,
                                            uint32_t          name,
                                            OTF2_LocationType location_type,
                                            uint64_t          numberOfEvents,
                                            uint32_t          locationGroup )
{
  OTF2_Reader* reader = ( OTF2_Reader* )userData;
  OTF2_EvtReader* evt_reader = OTF2_Reader_GetEvtReader( reader, locationIdentifier );
  OTF2_DefReader* def_reader = OTF2_Reader_GetDefReader( reader, locationIdentifier );

  uint64_t definitions_read = 0;
  OTF2_Reader_ReadAllLocalDefinitions( reader, def_reader, &definitions_read );

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
  std::cout << "    -h: Prints this help" << std::endl;
  std::cout << "    -i: Prints info about version" << std::endl;
  std::cout << "    -v: Set verbose mode" << std::endl;
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
  else if ( argument[ 1 ] == 'i' )
    options.showVersion = true;
  else if ( argument[ 1 ] == 'v')
    transData.verbose = true;
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
      // BUILD HEADER
      // Initialize callbacks for get last time
      OTF2_GlobalDefReader* global_def_reader = OTF2_Reader_GetGlobalDefReader( reader );
      OTF2_GlobalDefReaderCallbacks* global_def_callbacks = OTF2_GlobalDefReaderCallbacks_New();

      OTF2_GlobalDefReaderCallbacks_SetClockPropertiesCallback( global_def_callbacks, GlobDefClockProperties_print );
      OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeCallback( global_def_callbacks, GlobDefSystemTreeNode_print );
      OTF2_GlobalDefReaderCallbacks_SetLocationGroupCallback( global_def_callbacks, GlobDefLocationGroup_print );
      OTF2_GlobalDefReaderCallbacks_SetLocationCallback( global_def_callbacks, GlobDefLocation_print );
      //OTF2_GlobalDefReaderCallbacks_SetLocationCallback( global_def_callbacks, GlobDefLocation_Register );

      //OTF2_GlobalDefReaderCallbacks_SetRegionCallback( global_def_callbacks, GlobDefRegion_print );
      //OTF2_GlobalDefReaderCallbacks_SetGroupCallback( global_def_callbacks, GlobDefGroup_print );
      //OTF2_GlobalDefReaderCallbacks_SetMpiCommCallback( global_def_callbacks, GlobDefMpiComm_print );

      OTF2_Reader_RegisterGlobalDefCallbacks( reader, global_def_reader, global_def_callbacks, (void *)tmpData );
      OTF2_GlobalDefReaderCallbacks_Delete( global_def_callbacks );

      uint64_t definitions_read = 0;
      OTF2_Reader_ReadAllGlobalDefinitions( reader, global_def_reader, &definitions_read );

      tmpData->resourcesModel->setReady( true );
      tmpData->processModel->setReady( true );

      // WRITE HEADER
      writeHeaderTimes( tmpData, file );
      writeHeaderResourceModel( tmpData, file );
      writeHeaderProcessModel( tmpData, file );
      writeHeaderCommunicators( tmpData, file );

      // TRANSLATE EVENTS
      // Initialize callbacks for events
      // Write events
      // Read states
      // Read communications
      OTF2_GlobalEvtReader* global_evt_reader = OTF2_Reader_GetGlobalEvtReader( reader );
      OTF2_GlobalEvtReaderCallbacks* event_callbacks = OTF2_GlobalEvtReaderCallbacks_New();
      OTF2_GlobalEvtReaderCallbacks_SetEnterCallback( event_callbacks, Enter_print );
      OTF2_GlobalEvtReaderCallbacks_SetLeaveCallback( event_callbacks, Leave_print );
      OTF2_GlobalEvtReaderCallbacks_SetMpiSendCallback( event_callbacks, MpiSend_print );


      OTF2_Reader_RegisterGlobalEvtCallbacks( reader, global_evt_reader, event_callbacks, NULL );
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
    tmpData.verbose = false;


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
      tmpData.timeUnit = NS;
      tmpData.resourcesModel = new ResourceModel();
      tmpData.processModel = new ProcessModel();
      tmpData.processModel->addApplication();
      tmpData.rowLabels = new RowLabels();

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
