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

// for strdup
//#include <string.h>

bool showHelp = false;
bool showVersion = false;
bool onlySelectedPlane = false;
bool hideEmptyColumns = false;
bool multipleFiles = false;
bool dumpTrace = false;
bool noLoad = false;
bool shiftTimes = false;

PRV_INT32 numIter = 1;
Trace *trace;

TraceOptions *traceOptions = NULL;
//TraceCommunicationsFusion *traceCommunicationsFusion = NULL;
TraceCutter *traceCutter = NULL;
TraceFilter *traceFilter = NULL;
TraceSoftwareCounters *traceSoftwareCounters = NULL;
TraceShifter *traceShifter = NULL;

string strXMLOptions( "" );
std::map< string, string > cfgs;
string strTrace( "" );
string strShiftTimesFile( "" );

void printHelp()
{
  std::cout << "USAGE" << std::endl;
  std::cout << "  paramedir [OPTION] trc {xml} cfg {cfgout | cfg}*" << std::endl;
  std::cout << std::endl;
  std::cout << "    -h: Prints this help" << std::endl;
  std::cout << "    -v: Prints version" << std::endl;
  std::cout << std::endl;
  std::cout << "  Histogram options:" << std::endl;
  std::cout << "    -p: Only the selected Plane of a 3D histogram is printed. Default is to print all of them." << std::endl;
  std::cout << "    -e: Hide Empty columns" << std::endl;
  std::cout << std::endl;
  std::cout << "  Output file options:" << std::endl;
  std::cout << "    -m: Prints on Multiple files." << std::endl;
  std::cout << std::endl;
  std::cout << "  Cutter/Filter options ( needed unique xml file with cutter/filter options):" << std::endl;
  std::cout << "    -c: Apply Cutter." << std::endl;
  std::cout << "    -f: Apply Filter." << std::endl;
  std::cout << "    -s: Apply Software Counters." << std::endl;
  std::cout << "    -t: Apply Trace Times Shifter." << std::endl;
//  std::cout << "    -j: Apply Communications Fusion. (in progress)" << std::endl;
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
  std::cout << "    paramedir -t linpack.prv times.txt" << std::endl;
  std::cout << "    paramedir -t linpack.prv times.txt linpack.shifted.prv" << std::endl;
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


bool isOption( char *argument )
{
  return ( argument[ 0 ] == '-' );
}


void activateOption( char *argument, vector< string > &filterToolOrder )
{
  if ( argument[ 1 ] == 'h' )
    showHelp = true;
  else if ( argument[ 1 ] == 'v' )
    showVersion = true;
  else if ( argument[ 1 ] == 'p' )
    onlySelectedPlane = true;
  else if ( argument[ 1 ] == 'e' )
    hideEmptyColumns = true;
  else if ( argument[ 1 ] == 'm' )
    multipleFiles = true;
  else if ( argument[ 1 ] == 'd' )
    dumpTrace = true;
  else if ( argument[ 1 ] == 'n' )
    noLoad = true;
  else if ( argument[ 1 ] == 'c' )
  {
    filterToolOrder.push_back( TraceCutter::getID() );
  }
  else if ( argument[ 1 ] == 'f' )
  {
    filterToolOrder.push_back( TraceFilter::getID() );
  }
  else if ( argument[ 1 ] == 's' )
  {
    filterToolOrder.push_back( TraceSoftwareCounters::getID() );
  }
  else if ( argument[ 1 ] == 't' )
  {
    filterToolOrder.push_back( TraceShifter::getID() );
    shiftTimes = true;
  }
  else
    std::cout << "Unknown option " << argument << std::endl;
}

#ifdef BYTHREAD
void getDumpIterations( int &currentArg, char *argv[] )
{
  if ( dumpTrace )
  {
    ++currentArg;
    string strNumIter( argv[ currentArg ] );
    std::stringstream tmpNumIter( strNumIter );
    if( !( tmpNumIter >> numIter ) )
    {
      numIter = 1;
      --currentArg;
    }
  }
}
#endif


void readParameters( int argc,
                     char *arguments[],
                     vector< string > &filterToolOrder)
{
  string strOut;
  string strOutputFile;
  PRV_INT32 previousCFGPosition = 0;
  PRV_INT32 currentArg = 1;

  while ( currentArg < argc )
  {
    if ( isOption( arguments[ currentArg ] ))
    {
      activateOption( arguments[ currentArg ], filterToolOrder );
#ifdef BYTHREAD
      getDumpIterations( currentArg, arguments );
#endif
    }
    else if ( Trace::isTraceFile( string( arguments[ currentArg ] )))
    {
      strTrace = string( arguments[ currentArg ] );
    }
    else if ( TraceOptions::isTraceToolsOptionsFile( string( arguments[ currentArg ] )))
    {
      strXMLOptions = string( arguments[ currentArg ] );
    }
    else if ( CFGLoader::isCFGFile( string( arguments[ currentArg ] )))
    {
      string strCfg( arguments[ currentArg ] );
      strOutputFile = strCfg.substr( 0, strCfg.length() - CFG_SUFFIX.length() );
      cfgs[ arguments[ currentArg ] ] = strOutputFile;
      previousCFGPosition = currentArg;
    }
    else if ( shiftTimes )
    {
      // First version: paramedir -t trace.prv shifttimes  or
      //                paramedir -t shifttimes trace.prv

      strShiftTimesFile = string( arguments[ currentArg ] );
    }
    else
    {
      // Unknown parameter; maybe a output file?
      if ( previousCFGPosition > 0 )
      {
        if ( previousCFGPosition == currentArg - 1 )
        {
          strOutputFile = string( arguments[ currentArg ] );
          cfgs[ string( arguments[ currentArg - 1 ] ) ] = strOutputFile;
          previousCFGPosition = 0;
        }
      }
    }

    ++currentArg;
  }

  // Default filters to apply if no option given: all?
  // IMPROVE: Detect which kind of xml do we have.
}

bool anyTrace()
{
  return ( strTrace != "" );
}

bool anyCFG()
{
  return ( cfgs.size() > 0 );
}


bool anyFilter( const vector< string >& filterToolOrder )
{
  //return ( ( strXMLOptions != "" ) && ( filterToolOrder.size() > 0 ));
  return ( filterToolOrder.size() > 0 );
}


string applyFilters( KernelConnection *myKernel,
                     vector< string > &filterToolOrder,
                     vector< string > &tmpFiles )
{
 // string tmpTraceIn, tmpTraceOut;
  string tmpNameIn, tmpNameOut;
  string fullTmpNameIn;
//  string tmpNameIn, tmpNameOut, tmpPathOut;
 // string strOutputFile, strPathOut;

  // Only for cutter
  string pcf_name;
  vector< TEventType > allTypes;
  vector< TEventType > typesWithValueZero;
  EventLabels labels;
  std::map< TEventValue, string > currentEventValues;
  ParaverTraceConfig *config;
  FILE *pcfFile;

  traceOptions = myKernel->newTraceOptions( );
  // The order is given by the command line, not the xml file.
  vector< string > xmlToolOrder = traceOptions->parseDoc( (char *)strXMLOptions.c_str() );

  // Concatenate Filter Utilities
  tmpNameOut = strTrace;

  string PATHSEP = myKernel->getPathSeparator();
  if ( tmpNameOut.substr( 0, 1 ) != PATHSEP )
  {
    char *currentWorkingDir = getenv( "PWD" );
    if ( currentWorkingDir != NULL)
    {
      string auxPWD( currentWorkingDir );
      if ( auxPWD.substr( auxPWD.length() - 1, 1 ) == PATHSEP )
        tmpNameOut = currentWorkingDir + tmpNameOut;
      else
        tmpNameOut = currentWorkingDir + PATHSEP + tmpNameOut;
    }
  }

  fullTmpNameIn = tmpNameOut;
/*
  size_t pos = strTrace.find_last_of( '/' );
  if ( pos != string::npos )
    strPathOut = strTrace.substr( 0, pos );
  else
    strPathOut = strTrace;
*/

  //tmpPathOut = strPathOut;

  for( PRV_UINT16 i = 0; i < filterToolOrder.size(); ++i )
  {
    tmpNameIn = tmpNameOut;

    if ( i < filterToolOrder.size() - 1 )
    {
      // Get partial name (one tool) and don't modify file that list recent treated traces
      //tmpNameIn += "x";
      tmpNameOut = myKernel->getNewTraceName( tmpNameIn , filterToolOrder[ i ], false );
    }
    else
    {
      // Get full name (all tools) and append to file that list recent treated traces
      tmpNameOut = myKernel->getNewTraceName( fullTmpNameIn, filterToolOrder, true );
    }
    //myKernel->getNewTraceName( tmpNameIn, tmpPathOut, filterToolOrder[ i ] );
    //tmpNameOut = tmpPathOut;

    if ( filterToolOrder[i] == TraceCutter::getID() )
    {
      pcf_name = LocalKernel::composeName( tmpNameIn, string( "pcf" ) );

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
      traceCutter->execute( (char *)tmpNameIn.c_str(), (char *)tmpNameOut.c_str() );
      myKernel->copyPCF( tmpNameIn, tmpNameOut );
    }
    else if ( filterToolOrder[i] == TraceFilter::getID() )
    {
      std::map< TTypeValuePair, TTypeValuePair > translation;
#if 1
      traceFilter = myKernel->newTraceFilter( (char *)tmpNameIn.c_str(),
                                              (char *)tmpNameOut.c_str(),
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

      traceFilter = myKernel->newTraceFilter( (char *)tmpNameIn.c_str(), (char *)tmpNameOut.c_str(), opts, translation );
#endif
      myKernel->copyPCF( tmpNameIn, tmpNameOut );
    }
    else if ( filterToolOrder[i] == TraceSoftwareCounters::getID() )
    {
      traceSoftwareCounters = myKernel->newTraceSoftwareCounters( (char *)tmpNameIn.c_str(),
                                                                  (char *)tmpNameOut.c_str(),
                                                                  traceOptions );
    }
    else if ( filterToolOrder[i] == TraceShifter::getID() )
    {
      // TODO: si el kernel no va a hacer nada con las trazas, no tiene sentido pasar nombres
      traceShifter = myKernel->newTraceShifter( tmpNameIn, tmpNameOut, strShiftTimesFile );
      traceShifter->execute( tmpNameIn, tmpNameOut );
    }

    myKernel->copyROW( tmpNameIn, tmpNameOut );
    tmpFiles.push_back( tmpNameOut );
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

  return tmpNameOut;
}


bool loadTrace( KernelConnection *myKernel )
{
  bool loaded = false;

  try
  {
    trace = Trace::create( myKernel, strTrace, noLoad, NULL );
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
      output.setMultipleFiles( multipleFiles );

      if ( histograms.begin() != histograms.end() &&
           histograms[ histograms.size() - 1 ] != NULL )
        output.dumpHistogram( histograms[ histograms.size() - 1 ],
                              it->second,
                              onlySelectedPlane,
                              hideEmptyColumns );
      else if( windows.begin() != windows.end() &&
               windows[ windows.size() - 1 ] != NULL )
        output.dumpWindow( windows[ windows.size() - 1 ], it->second );
    }
    else
      std::cout << "Cannot load '" << it->first << "' file." << std::endl;

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
    vector< string > filterToolOrder;
    vector< string > tmpFiles;

    // Initializations
    LocalKernel::init();
    KernelConnection *myKernel = new LocalKernel( NULL );
    ParaverConfig *config = ParaverConfig::getInstance();
    config->readParaverConfigFile();

    readParameters( argc, argv, filterToolOrder );

    // Execute
    if ( showHelp )
      printHelp();
    else if ( showVersion )
      printVersion();
    else if ( anyTrace() )
    {
      if ( anyFilter( filterToolOrder ) )
      {
        strTrace = applyFilters( myKernel, filterToolOrder, tmpFiles );
      }

      if ( anyCFG() || dumpTrace )
      {
        if ( !loadTrace( myKernel ) )
        {
          std::cout << "Cannot load " << strTrace << std::endl;
          exit( 0 );
        }

        if ( dumpTrace )
    #ifdef BYTHREAD
          trace->dumpFile( strTrace + ".new.bythread", numIter );
    #else
          trace->dumpFile( strTrace + ".new.global" );
    #endif

        loadCFGs( myKernel );

        delete trace;
      }
    }
  }

  return 0;
}
