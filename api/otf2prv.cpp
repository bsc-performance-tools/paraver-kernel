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
const string OTF2_VERSION_STRING = "0.8";

// GLOBAL VARIABLES
bool showHelp = false;
bool showVersion = false;
bool verbose = false;

Trace *trace;
string strOTF2Trace( "" );
string strPRVTrace( "" );
std::fstream file;

TTimeUnit timeUnit = NS;
string strTimeUnit("");
PRV_UINT64 timerResolution;
PRV_UINT64 maxTraceTime;

uint32_t systems = 0;

ResourceModel *resourcesModel;
ProcessModel *processModel;


// *****************************************************************************
// 3rd LEVEL - PRV Write
// *****************************************************************************

void writeHeaderTimes()
{
  ostringstream ostr;

  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  file << "#Paraver ";
  file << "()" << ':';

  ostr << maxTraceTime;
  file << ostr.str();

  file << "_" << LABEL_TIMEUNIT[ timeUnit ];
  file << ':';
}


void writeHeaderResourceModel()
{
  resourcesModel->dumpToFile( file );
  file << ':';
}


void writeHeaderProcessModel()
{
  processModel->dumpToFile( file );
}


void writeHeaderCommunicators()
{
  file << ",0" << endl;
}


// *****************************************************************************
// 2nd LEVEL - OTF2 level
// *****************************************************************************


SCOREP_Error_Code
GlobDefClockProperties_print
(
    void*    userData,
    uint64_t timer_resolution,
    uint64_t global_offset,
    uint64_t trace_length
)
{
  /* Dummies to suppress compiler warnings for unused parameters. */
  ( void )userData;

  if ( timer_resolution <= 1 )
    timeUnit = SEC;
  else if ( timer_resolution <= 1000 )
    timeUnit = MS;
  else if ( timer_resolution <= 1000000 )
    timeUnit = US;
  else if ( timer_resolution <= 1000000000 )
    timeUnit = NS;
  else
    timeUnit = NS;

  maxTraceTime = global_offset + trace_length;

  return SCOREP_SUCCESS;
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
  otf2_print_data* data = ( otf2_print_data* )userData;

  // Translation: node of nodes => SYSTEM, NODE
  // How to translate many systems?
  // Many systems to one system and all the nodes merged?

  if ( nodeParent != OTF2_UNDEFINED_UINT32 )
  {
    // Add node to our structure
    resourcesModel->addNode( nodeID - systems );

    // Keep name for ROW file
    // Use Hash de scorep?
  }
  else
  {
    ++systems;
  }
/*
  // Adds to the hash
  // See otf2_print : otf2_print_add_def64_name
  otf2_print_add_system_tree_node( data,
                                   nodeID,
                                   name );

        printf( "%-*s %12u  Name: %s, Class: %s, Parent: %s\n",
                otf2_DEF_COLUMN_WIDTH, "SYSTEM_TREE_NODE",
                nodeID,
                otf2_print_get_def_name( data->strings, name ),
                otf2_print_get_def_name( data->strings, className ),
                otf2_print_get_def_name( data->system_tree_nodes, nodeParent ) );

*/

  return SCOREP_SUCCESS;
}


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
    otf2_print_data* data = ( otf2_print_data* )userData;

    processModel->addTask( 0, groupID );

    // LocationGroup: { UNKNOWN, PROCESS }
    // In both cases PROCESS?

    // Tranlation: Map TASK->NODE




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
    otf2_print_data* data = ( otf2_print_data* )userData;
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
  otf2_print_data* data = ( otf2_print_data* )userData;
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


void activateOption( char *argument )
{
  if ( argument[ 1 ] == 'h' )
    showHelp = true;
  else if ( argument[ 1 ] == 'i' )
    showVersion = true;
  else if ( argument[ 1 ] == 'v')
    verbose = true;
  else
    std::cout << "Unknown option " << argument << std::endl;
}


void readParameters( int argc, char *arguments[] )
{
  PRV_INT32 currentArg = 1;

  while ( currentArg < argc )
  {
    if ( isOption( arguments[ currentArg ] ))
    {
      activateOption( arguments[ currentArg ] );
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


bool anyOTF2Trace()
{
  return ( strOTF2Trace != "" );
}


bool translate()
{
  // BUILD HEADER
  // Open otf2 trace
  OTF2_Reader* reader = OTF2_Reader_Open( strOTF2Trace.c_str() );

  // Initialize

  // Initialize callbacks for get last time
  OTF2_GlobalDefReader* global_def_reader = OTF2_Reader_GetGlobalDefReader( reader );
  OTF2_GlobalDefReaderCallbacks* global_def_callbacks = OTF2_GlobalDefReaderCallbacks_New();

  OTF2_GlobalDefReaderCallbacks_SetClockPropertiesCallback( global_def_callbacks, GlobDefClockProperties_print );
  OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeCallback( global_def_callbacks, GlobDefSystemTreeNode_print );
  //OTF2_GlobalDefReaderCallbacks_SetLocationGroupCallback( global_def_callbacks, GlobDefLocationGroup_print );
  //OTF2_GlobalDefReaderCallbacks_SetLocationCallback( global_def_callbacks, GlobDefLocation_print );
  OTF2_GlobalDefReaderCallbacks_SetLocationCallback( global_def_callbacks, &GlobDefLocation_Register );

  //OTF2_GlobalDefReaderCallbacks_SetRegionCallback( global_def_callbacks, GlobDefRegion_print );
  //OTF2_GlobalDefReaderCallbacks_SetGroupCallback( global_def_callbacks, GlobDefGroup_print );
  //OTF2_GlobalDefReaderCallbacks_SetMpiCommCallback( global_def_callbacks, GlobDefMpiComm_print );

  OTF2_Reader_RegisterGlobalDefCallbacks( reader, global_def_reader, global_def_callbacks, reader );
  OTF2_GlobalDefReaderCallbacks_Delete( global_def_callbacks );

  uint64_t definitions_read = 0;
  OTF2_Reader_ReadAllGlobalDefinitions( reader, global_def_reader, &definitions_read );

  resourcesModel->setReady( true );
  processModel->setReady( true );

  // WRITE HEADER
  writeHeaderTimes();
  writeHeaderResourceModel();
  writeHeaderProcessModel();
  writeHeaderCommunicators();

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

  return true;
}


void buildPRVTraceName()
{
  if ( strPRVTrace.compare( "" ) == 0 )
  {
    strPRVTrace = strOTF2Trace + ".prv";
  }
}

std::fstream & openPRV()
{
  file.open( strPRVTrace.c_str(), fstream::out | fstream::trunc );
  file << fixed;
  file << dec;
  file.precision( 0 );
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

    readParameters( argc, argv );

    if ( showHelp )
      printHelp();
    else if ( showVersion )
      printVersion();
    else if ( anyOTF2Trace() )
    {
      resourcesModel = new ResourceModel();
      processModel = new ProcessModel();
      processModel->addApplication( 0 );

      buildPRVTraceName();
      openPRV();
      if ( translate() )
      {
        std::cout << "Done" << std::endl;
      }
      else
      {
        // Error:
        globalError = -1;
      }

      file.close();

      delete resourcesModel;
      delete processModel;
    }
  }

  return globalError;
}
