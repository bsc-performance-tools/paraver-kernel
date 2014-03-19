/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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
#include <fstream>
#include <iomanip>
#include <map>
#include <set>
#include <algorithm>

//#include <boost/filesystem.hpp>
//#include <boost/error_code.hpp>
//using namespace boost::filesystem;
#include "config.h"
#include "localkernel.h"
#include "paramedir.h"
#include "cfg.h"
#include "trace.h"
#include "window.h"
#include "histogram.h"
#include "histogramtotals.h"
#include "paraverkernelexception.h"
#include "paraverconfig.h"
#include "textoutput.h"
#include "traceoptions.h"
#include "labelconstructor.h" // for getDate

#include "tracecutter.h"
#include "tracefilter.h"
#include "tracesoftwarecounters.h"
#include "traceshifter.h"
#include "eventdrivencutter.h"


// PARAMEDIR OPTIONS

// All fields are descriptive but 'active' field, used as a flag
typedef struct TOptionParamedir
{
  string shortForm;
  string longForm;
  bool active;       // Detected parameter in current execution?
  int numParameters; // 0,1 => no parameterSeparator is needed
  string parameterSeparators; // p.e. : ",-"
  string parameterUsage; // Only for message; p.e. : "p1-p3" or "alfa,beta,delta"
  string helpMessage;
}
TOptionParamedir;


enum TOptionID
{
  // GENERAL
  SHOW_HELP = 0,
  SHOW_VERSION,

  // FILES
  MANY_FILES,
  OUTPUT_NAME,

  // HISTOGRAMS
  EMPTY_COLUMNS,
  PRINT_PLANE,

  // PRV TOOLSET
  CUTTER,
  FILTER,
  EVENT_CUTTER,
  SOFTWARE_COUNTERS,
  TRACE_SHIFTER,

  // SENTINEL
  INVALID_OPTION,

  // HIDDEN
  DUMP_TRACE,
  NO_LOAD
};


TOptionParamedir definedOption[] =
  {
    // GENERAL
    { "-h", "--help", false, 0, "", "", "Show help" },
    { "-v", "--version", false, 0, "", "", "Show version" },

    // FILES
    { "-m", "--many-files", false, 0, "", "", "Allows to separate cfg output (default in a unique file)" },
    { "-o", "--output-name", false, 1, "", "<tracename>",  "Output trace name" },

    // HISTOGRAMS
    { "-e", "--empty-columns", false, 0, "", "", "Hide empty columns" },
    { "-p", "--print-plane", false, 0, "", "", "Only the selected Plane of a 3D histogram is saved (by default saves all planes)" },

    // PRV TOOLSET
    { "-c", "--cutter", false, 0, "", "", "Apply Cutter tool" },
    { "-f", "--filter", false, 0, "", "", "Apply Filter tool" },
    { "-g", "--event-cutter", false, 1, "", "<event-type>", "Apply Event Driven Cutter" },
    { "-s", "--software-counters", false, 0, "", "", "Apply Software counters tool" },
    { "-t", "--trace-shifter", false, 1, "", "<times-file>", "Apply Trace Shifter tool given these shift times" },

    // SENTINEL
    { "", "", false, 0, "", "", "none" },

    // HIDDEN
    { "-d", "--dump-trace", false, 0, "", "", "" },
    { "-n", "--no-load", false, 0, "", "", "" }
  };


// Options
std::map< TOptionID, TOptionParamedir > option;

#ifdef BYTHREAD
PRV_INT32 numIter = 1; // DUMP_TRACE by THREAD
#endif

// PRVs
string sourceTraceName( "" );
string outputTraceName("");
Trace *trace;

// CFGs
std::map< string, string > cfgs;

// PRV Toolset
string strXMLOptions( "" );

TraceOptions *traceOptions = NULL;

TraceCutter *traceCutter = NULL;
TraceFilter *traceFilter = NULL;
TraceSoftwareCounters *traceSoftwareCounters = NULL;

TraceShifter *traceShifter = NULL;
string strShiftTimesFile( "" );

EventDrivenCutter *eventDrivenCutter = NULL;
TEventType eventType = 0;


// Loads option map
void initOptions()
{
  for ( int i = SHOW_HELP; i < INVALID_OPTION; ++i )
  {
    TOptionID id = TOptionID( i );
    option[ id ]  = definedOption[ id ];
  }
}


// Searches argument in option map
// Unknown argument => returns INVALID_OPTION
TOptionID findOption( string argument )
{
  TOptionID whichOption = INVALID_OPTION;

  for ( int i = SHOW_HELP; i < INVALID_OPTION; ++i )
  {
    TOptionID id = TOptionID( i );
    if (( argument == option[ id ].shortForm ) ||
        ( argument == option[ id ].longForm ))
    {
      whichOption = id;
      break;
    }
  }

  return whichOption;
}


// Prints help for concrete option
void printOptionHelp( TOptionID id )
{
  std::stringstream helpFormat, helpFormat2;

  helpFormat << option[ id ].shortForm;

  if ( option[ id ].numParameters == 0 )
  {
    helpFormat << ", " << option[ id ].longForm;
  }
  else
  {
    helpFormat << " " << option[ id ].parameterUsage << ",";
    helpFormat2 << "    " << option[ id ].longForm << " " << option[ id ].parameterUsage;
  }

  std::cout << "    " << std::setw(35) << std::left << helpFormat.str();
  if ( option[ id ].numParameters > 0 )
  {
    std::cout << std::setw(0) << std::endl;
    std::cout << "    " << std::setw(35) << std::left << helpFormat2.str();
  }
  std::cout << " " << std::setw(45) << option[ id ].helpMessage << std::endl;
}


void printHelp()
{
  std::cout << "USAGE" << std::endl;
  std::cout << "  paramedir [OPTION] trc [xml] cfg [cfgout | cfg]*" << std::endl;

  std::cout << std::endl;
  std::cout << "  General options:" << std::endl;
  for ( int i = SHOW_HELP; i < MANY_FILES; ++i )
    printOptionHelp( TOptionID( i ) );

  std::cout << std::endl;
  std::cout << "  Output file options:" << std::endl;

  for ( int i = MANY_FILES; i < EMPTY_COLUMNS; ++i )
    printOptionHelp( TOptionID( i ) );

  std::cout << std::endl;
  std::cout << "  Histogram options:" << std::endl;

  for ( int i = EMPTY_COLUMNS; i < CUTTER; ++i )
    printOptionHelp( TOptionID( i ) );

  std::cout << std::endl;
  std::cout << "  Cutter/Filter options ( needed unique xml file with cutter/filter options):" << std::endl;

  for ( int i = CUTTER; i < INVALID_OPTION; ++i )
    printOptionHelp( TOptionID( i ) );

  std::cout << std::endl;
  std::cout << "  Parameters:" << std::endl;
  std::cout << "    trc: Paraver trace filename ( with extension .prv or .prv.gz )." << std::endl;
  std::cout << "    xml: Options for cutter/filter/software counters ( with extension .xml )." << std::endl;
  std::cout << "    cfg: Paraver configuration filename ( with extension .cfg ). If present, trace's loaded." << std::endl;
  std::cout << "    out: Filename for cfg output ( default name is cfg filename without, with extension .mcr )." << std::endl;
  std::cout << std::endl;
  std::cout << "  Examples:" << std::endl;
  std::cout << "    paramedir -m linpack.prv.gz mpi_stats.cfg" << std::endl;
  std::cout << "      Computes the mpi_stats.cfg analysis of compressed trace linpack.prv." << std::endl;
  std::cout << std::endl;
  std::cout << "    paramedir -c linpack.prv cutter.xml" << std::endl;
  std::cout << "      Reads parameters of the cutter from the xml and applies them to linpack.prv trace." << std::endl;
  std::cout << std::endl;
  std::cout << "    paramedir -f linpack.prv just_MPI_calls.xml" << std::endl;
  std::cout << "      Filters mpi calls of linpack.prv. Doesn't load it, just writes the file." << std::endl;
  std::cout << std::endl;
  std::cout << "    paramedir -s -c -f linpack.prv cut_filter_options.xml mpi_stats.cfg my_mpi_values.txt" << std::endl;
  std::cout << "      Reads parameters of the software counters, cutter and filter from the xml and applies them" << std::endl;
  std::cout << "      pipelined in the given order ( trace -> software counters | cutter | filter -> result trace)" << std::endl;
  std::cout << "      to linpack.prv trace, and the filtered trace is loaded and used to compute mpi_stats.cfg." << std::endl;
  std::cout << "      The computed mpi results are saved int my_mpi_values.txt." << std::endl;
  std::cout << std::endl;
  std::cout << "    paramedir -t times.txt linpack.prv" << std::endl;
  std::cout << "    paramedir -t times.txt linpack.prv -o mylinpack.shifted.prv" << std::endl;
  std::cout << std::endl;
}


void printVersion()
{
  std::cout << PACKAGE_STRING;

  bool reverseOrder = true;
  string auxDate = LabelConstructor::getDate( reverseOrder );

  if ( auxDate.compare("") != 0 )
    std::cout << " Build ";

  std::cout << auxDate << std::endl;
}


string getToolID( TOptionID whichOption )
{
  string toolID("");

  switch ( whichOption )
  {
    case CUTTER:
      toolID = TraceCutter::getID();
      break;

    case FILTER:
      toolID =  TraceFilter::getID();
      break;

    case SOFTWARE_COUNTERS:
      toolID =  TraceSoftwareCounters::getID();
      break;

    case TRACE_SHIFTER:
      toolID = TraceShifter::getID();
      break;

    case EVENT_CUTTER:
      toolID = EventDrivenCutter::getID();
      break;

    default:
      break;
  }

  return toolID;
}


// Register any specified tool; remembers order
void registerTool( TOptionID whichOption, vector< string > &registeredTool )
{
  string toolID = getToolID( whichOption );

  if ( !toolID.empty() )
    registeredTool.push_back( toolID );
}


void unregisterTool( TOptionID whichOption, vector< string > &registeredTool )
{
  string toolID = getToolID( whichOption );

  if ( !toolID.empty() )
  {
    vector< string >::iterator it = std::find( registeredTool.begin(), registeredTool.end(), toolID );
    if ( it != registeredTool.end() )
    {
      registeredTool.erase( it );
    }
  }
}


#ifdef BYTHREAD
void getDumpIterations( int &numArg, char *argv[] )
{
  if ( option[ DUMP_TRACE ].active )
  {
    ++numArg;
    string strNumIter( argv[ numArg ] );
    std::stringstream tmpNumIter( strNumIter );
    if( !( tmpNumIter >> numIter ) )
    {
      numIter = 1;
      --numArg;
    }
  }
}
#endif


void parseArguments( int argc,
                     char *arguments[],
                     vector< string > &registeredTool )
{
  PRV_INT32 numArg;
  string currentArgument;
  TOptionID prevParamPendingOption = INVALID_OPTION;
  TOptionID currentOption = INVALID_OPTION;

  PRV_INT32 previousCFGPosition = 0;
  PRV_INT32 readParameter = 0;

  numArg = 1;
  while ( numArg < argc )
  {
    currentArgument = string( arguments[ numArg ] );
    currentOption = findOption( currentArgument );
    if ( currentOption != INVALID_OPTION )
    {
      if ( readParameter > 0 )
      {
        // Wrong! I.e: If -o option needs a parameter, we got:
        //    "paramedir -o -x ..." instead of "paramedir -o PARAM -x ..."
        std::cout << "  [Warning]: Parameter expected but option " << currentArgument << " was found instead."<< std::endl;

        // Deactivate previous pending option
        readParameter = 0;
        option[ prevParamPendingOption ].active = false;
        unregisterTool( prevParamPendingOption, registeredTool );
      }

      option[ currentOption ].active = true;
      readParameter = option[ currentOption ].numParameters;
      registerTool( currentOption, registeredTool );
#ifdef BYTHREAD
      getDumpIterations( numArg, arguments );
#endif
    }
    else if ( readParameter > 0 )
    {
      // Previous loop detected an option needing parameters
      // TODO: numParameters > 1
      if ( option[ OUTPUT_NAME ].active && outputTraceName.empty() )
      {
        outputTraceName = currentArgument;
      }
      else if ( option[ EVENT_CUTTER ].active && eventType == 0 )
      {
        std::stringstream sstr( currentArgument );
        if( !( sstr >> eventType ) )
        {
          std::cerr << "  [WARNING] '" << currentArgument << "' not a valid event type." << std::endl;
          option[ EVENT_CUTTER ].active = false;
//          unregisterTool( prevParamPendingOption, registeredTool );
        }
      }
      else if ( option[ TRACE_SHIFTER ].active && strShiftTimesFile.empty() )
      {
        strShiftTimesFile = currentArgument;
      }
      else
      {

      }

      --readParameter;
    }
    else if ( Trace::isTraceFile( currentArgument ) )
    {
      sourceTraceName = currentArgument;
    }
    else if ( TraceOptions::isTraceToolsOptionsFile( currentArgument ) )
    {
      // TODO: Detect which kind of xml do we have. See implementation.
      strXMLOptions = currentArgument;
    }
    else if ( CFGLoader::isCFGFile( currentArgument ) )
    {
      cfgs[ currentArgument ] = currentArgument.substr( 0, currentArgument.length() - CFG_SUFFIX.length() );;
      previousCFGPosition = numArg;
    }
    else
    {
      // Unknown parameter! Maybe is it an output file for cfgs?
      if ( ( previousCFGPosition > 0 ) && ( previousCFGPosition == numArg - 1 ) )
      {
        cfgs[ string( arguments[ numArg - 1 ] ) ] = currentArgument;
        previousCFGPosition = 0; // We allow only one output name per cfg
      }
    }

    ++numArg;
  }
}


// Returns $PWD/whichPath
std::string appendPathWorkingDirectory( KernelConnection *myKernel,
                                        std::string whichPath )
{
  string PATHSEP = myKernel->getPathSeparator();

  if ( whichPath.substr( 0, 1 ) != PATHSEP )
  {
    char *currentWorkingDir = getenv( "PWD" );
    if ( currentWorkingDir != NULL )
    {
      string auxPWD( currentWorkingDir );
      if ( auxPWD.substr( auxPWD.length() - 1, 1 ) == PATHSEP )
        whichPath = currentWorkingDir + whichPath;
      else
        whichPath = currentWorkingDir + PATHSEP + whichPath;
    }
  }

  return whichPath;
}


string applyFilters( KernelConnection *myKernel,
                     vector< string > &registeredTool,
                     vector< string > &tmpFiles )
{
  string intermediateNameIn, intermediateNameOut;
  string destinyTraceName;

  // Only for cutter
  string pcf_name;
  vector< TEventType > allTypes;
  vector< TEventType > typesWithValueZero;
  EventLabels labels;
  std::map< TEventValue, string > currentEventValues;
  ParaverTraceConfig *config;
  FILE *pcfFile;

  // Name initializations
  if ( outputTraceName.empty() )
  {
    destinyTraceName = appendPathWorkingDirectory( myKernel, sourceTraceName );
  }
  else
  {
    destinyTraceName = appendPathWorkingDirectory( myKernel, outputTraceName );
  }

  intermediateNameOut = appendPathWorkingDirectory( myKernel, sourceTraceName );

  traceOptions = myKernel->newTraceOptions( );

  // The order is given by the command line, not the xml file.
  vector< string > xmlToolOrder = traceOptions->parseDoc( (char *)strXMLOptions.c_str() );

  // Concatenate Filter Utilities
  for( size_t i = 0; i < registeredTool.size(); ++i )
  {
    intermediateNameIn = intermediateNameOut;

    bool commitName;
    if ( i < registeredTool.size() - 1 )
    {
      // Intermediate trace:
      //   Get partial name (suffix for one tool)
      //   Don't modify global list of recent treated traces
      commitName = false;
      intermediateNameOut = myKernel->getNewTraceName( intermediateNameIn , registeredTool[ i ], commitName );
    }
    else
    {
      // Final trace:
      //   Get full name (including all tools)
      //   Remember name in global list of recent treated traces
      commitName = true;
      intermediateNameOut = myKernel->getNewTraceName( destinyTraceName, registeredTool, commitName );
    }

    if ( registeredTool[ i ] == TraceCutter::getID() )
    {
      pcf_name = LocalKernel::composeName( intermediateNameIn, string( "pcf" ) );

      if(( pcfFile = fopen( pcf_name.c_str(), "r" )) != NULL )
      {
        fclose( pcfFile );

        config = new ParaverTraceConfig( pcf_name );
        labels = EventLabels( *config, std::set<TEventType>() );
        labels.getTypes( allTypes );
        for( vector< TEventType >::iterator it = allTypes.begin(); it != allTypes.end(); ++it )
        {
          if ( labels.getValues( *it, currentEventValues ) )
          {
            if ( currentEventValues.find( TEventValue( 0 )) != currentEventValues.end() )
            {
              typesWithValueZero.push_back( *it );
            }
            currentEventValues.clear();
          }
        }

        delete config;
      }

      traceCutter = myKernel->newTraceCutter( traceOptions, typesWithValueZero );
      traceCutter->execute( (char *)intermediateNameIn.c_str(), (char *)intermediateNameOut.c_str() );
      myKernel->copyPCF( intermediateNameIn, intermediateNameOut );
    }
    else if ( registeredTool[ i ] == TraceFilter::getID() )
    {
      std::map< TTypeValuePair, TTypeValuePair > translation;
#if 1
      traceFilter = myKernel->newTraceFilter( (char *)intermediateNameIn.c_str(),
                                              (char *)intermediateNameOut.c_str(),
                                              traceOptions,
                                              translation );
#else
      //translation[ make_pair( 30000000, 2 ) ] = make_pair( 666, 999 );
      translation[ make_pair( 50000001, 1 ) ] = make_pair( 666666, 999999 );
      translation[ make_pair( 50000002, 9 ) ] = make_pair( 666666666, 999999999 );
      translation[ make_pair( 50000003, 31 ) ] = make_pair( 666666666666, 999999999999 );

      TraceOptions *opts = myKernel->newTraceOptions( );

      opts->set_filter_by_call_time( false );

      opts->set_filter_states( true );
      opts->set_all_states( true );

      opts->set_filter_events( true );
      opts->set_discard_given_types( true );
      TraceOptions::TFilterTypes dummyTypes;
      dummyTypes[0].type = 1234567890;
      opts->set_filter_last_type( 1 );

      opts->set_filter_comms( true );
      opts->set_min_comm_size( 1 );

      traceFilter = myKernel->newTraceFilter( (char *)intermediateNameIn.c_str(), (char *)intermediateNameOut.c_str(), opts, translation );
#endif
      myKernel->copyPCF( intermediateNameIn, intermediateNameOut );
    }
    else if ( registeredTool[ i ] == TraceSoftwareCounters::getID() )
    {
      traceSoftwareCounters = myKernel->newTraceSoftwareCounters( (char *)intermediateNameIn.c_str(),
                                                                  (char *)intermediateNameOut.c_str(),
                                                                  traceOptions );
    }
    else if ( registeredTool[ i ] == TraceShifter::getID() )
    {
      // TODO: if kernel isn't going to use the traces, it doesn't make sense to pass names
      traceShifter = myKernel->newTraceShifter( intermediateNameIn, intermediateNameOut, strShiftTimesFile, THREAD );
      traceShifter->execute( intermediateNameIn, intermediateNameOut );
      delete traceShifter;
    }
    else if ( registeredTool[ i ] == EventDrivenCutter::getID() )
    {
      eventDrivenCutter = myKernel->newEventDrivenCutter( intermediateNameIn, intermediateNameOut, eventType );
      eventDrivenCutter->execute( intermediateNameIn, intermediateNameOut );
      delete eventDrivenCutter;
    }

    myKernel->copyROW( intermediateNameIn, intermediateNameOut );
    tmpFiles.push_back( intermediateNameOut );
  }

  // Delete intermediate files
  string pcfName, rowName;
  for( PRV_UINT16 i = 0; i < tmpFiles.size() - 1; ++i )
  {
    pcfName = LocalKernel::composeName( tmpFiles[ i ], string( "pcf" ) );
    rowName = LocalKernel::composeName( tmpFiles[ i ], string( "row" ) );
    remove( tmpFiles[ i ].c_str() );
    remove( pcfName.c_str() );
    remove( rowName.c_str() );
  }

  // Delete utilities
  delete traceOptions;
  delete traceCutter;
  delete traceFilter;
  delete traceSoftwareCounters;

  return intermediateNameOut;
}


bool loadTrace( KernelConnection *myKernel )
{
  bool loaded = false;

  try
  {
    trace = Trace::create( myKernel, sourceTraceName, option[ NO_LOAD ].active, NULL );
    loaded = true;
  }
  catch ( ParaverKernelException& ex )
  {
    ex.printMessage();
  }

  return loaded;
}


void loadCFGs( KernelConnection *myKernel )
{
  for( std::map< string, string >::iterator it = cfgs.begin(); it != cfgs.end(); ++it )
  {
    vector<Window *> windows;
    vector<Histogram *> histograms;
    SaveOptions options; // P4D: Should options.enabledP4DMode be taken in this context?

    if ( CFGLoader::loadCFG( myKernel, it->first, trace, windows, histograms, options ) )
    {
      TextOutput output;
      output.setMultipleFiles( option[ MANY_FILES ].active );

      if ( histograms.begin() != histograms.end() &&
           histograms[ histograms.size() - 1 ] != NULL )
        output.dumpHistogram( histograms[ histograms.size() - 1 ],
                              it->second,
                              option[ PRINT_PLANE ].active,
                              option[ EMPTY_COLUMNS ].active );
      else if( windows.begin() != windows.end() &&
               windows[ windows.size() - 1 ] != NULL )
        output.dumpWindow( windows[ windows.size() - 1 ], it->second );
    }
    else
      std::cout << "  [Warning] Cannot load '" << it->first << "' file." << std::endl;

    for ( PRV_UINT32 i = 0; i < histograms.size(); ++i )
    {
      if ( histograms[ i ] != NULL )
        delete histograms[ i ];
    }

    histograms.clear();

    for ( PRV_UINT32 i = 0; i < windows.size(); ++i )
    {
      if ( windows[ i ] != NULL )
        delete windows[ i ];
    }

    windows.clear();
  }
}


#include "traceeditsequence.h"
#include "traceeditactions.h"
#include "traceeditstates.h"

void testSequence( KernelConnection *myKernel )
{
  vector<std::string> tmpV;
  tmpV.push_back( "/home/eloy/traces/mpi_ping0000089274.prv" );
  TraceEditSequence *seq = TraceEditSequence::create( myKernel );
  seq->pushbackAction( TraceEditSequence::testAction );
  seq->pushbackAction( TraceEditSequence::traceCutterAction );
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );
  TraceOptionsState *tmpOptionsState = new TraceOptionsState( seq );
  tmpOptionsState->setData( tmpOptions );
  seq->addState( TraceEditSequence::traceOptionsState, tmpOptionsState );
  seq->execute( tmpV );
}


int main( int argc, char *argv[] )
{
  initOptions();

  if ( argc == 1 )
#if 0
  {
    KernelConnection *myKernel = new LocalKernel( NULL );
    testSequence( myKernel );
    return 1;
  }
#else
    printHelp();
#endif
  else
  {
    vector< string > registeredTool;
    vector< string > tmpFiles;

    // Initializations
    LocalKernel::init();
    KernelConnection *myKernel = new LocalKernel( NULL );
    ParaverConfig *config = ParaverConfig::getInstance();
    config->readParaverConfigFile();

    parseArguments( argc, argv, registeredTool );

    if ( option[ SHOW_HELP ].active )
      printHelp();
    else if ( option[ SHOW_VERSION ].active )
      printVersion();
    else if ( !sourceTraceName.empty() )
    {
      if ( registeredTool.size() > 0 )
      {
        // We can pass a filtered trace to compute some further cfgs
        sourceTraceName = applyFilters( myKernel, registeredTool, tmpFiles );
      }

      if ( cfgs.size() > 0 || option[ DUMP_TRACE ].active )
      {
        if ( !loadTrace( myKernel ) )
        {
          std::cout << "  [Error] Cannot load " << sourceTraceName << std::endl;
          exit( 1 );
        }

        if ( option[ DUMP_TRACE ].active )
    #ifdef BYTHREAD
          trace->dumpFile( sourceTraceName + ".new.bythread", numIter );
    #else
          trace->dumpFile( sourceTraceName + ".new.global" );
    #endif

        loadCFGs( myKernel );

        delete trace;
      }
    }
  }

  return 0;
}
