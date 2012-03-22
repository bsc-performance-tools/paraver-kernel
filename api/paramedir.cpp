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

// for strdup
#include <string.h>

using namespace std;

bool showHelp = false;
bool showVersion = false;
bool onlySelectedPlane = false;
bool hideEmptyColumns = false;
bool multipleFiles = false;
bool dumpTrace = false;
bool noLoad = false;

PRV_INT32 numIter = 1;
Trace *trace;

TraceOptions *traceOptions = NULL;
//TraceCommunicationsFusion *traceCommunicationsFusion = NULL;
TraceCutter *traceCutter = NULL;
TraceFilter *traceFilter = NULL;
TraceSoftwareCounters *traceSoftwareCounters = NULL;

string strXMLOptions( "" );
map< string, string > cfgs;
string strTrace( "" );

void printHelp()
{
  cout << "USAGE" << endl;
  cout << "  paramedir [OPTION] trc {xml} cfg {cfgout | cfg}*" << endl;
  cout << endl;
  cout << "    -h: Prints this help" << endl;
  cout << "    -v: Prints version" << endl;
  cout << endl;
  cout << "  Histogram options:" << endl;
  cout << "    -p: Only the selected Plane of a 3D histogram is printed. Default is to print all of them." << endl;
  cout << "    -e: Hide Empty columns" << endl;
  cout << endl;
  cout << "  Output file options:" << endl;
  cout << "    -m: Prints on Multiple files." << endl;
  cout << endl;
  cout << "  Cutter/Filter options ( needed unique xml file with cutter/filter options):" << endl;
  cout << "    -c: Apply Cutter." << endl;
  cout << "    -f: Apply Filter." << endl;
  cout << "    -s: Apply Software Counters." << endl;
//  cout << "    -j: Apply Communications Fusion. (in progress)" << endl;
  cout << endl;
  cout << "  Parameters:" << endl;
  cout << "    trc: Paraver trace filename ( with extension .prv or .prv.gz )." << endl;
  cout << "    xml: Options for cutter/filter/software counters ( with extension .xml )." << endl;
  cout << "    cfg: Paraver configuration filename ( with extension .cfg ). If present, trace's loaded." << endl;
  cout << "    out: Filename for cfg output ( default name is cfg filename without, with extension .mcr )." << endl;
  cout << endl;
  cout << "  Examples:" << endl;
  cout << "    paramedir -m linpack.prv.gz mpi_stats.cfg" << endl;
  cout << "      Computes the mpi_stats.cfg analysis of compressed trace linpack.prv." << endl;
  cout << endl;
  cout << "    paramedir -c linpack.prv cutter.xml" << endl;
  cout << "      Reads parameters of the cutter from the xml and applies them to linpack.prv trace." << endl;
  cout << endl;
  cout << "    paramedir -f linpack.prv just_MPI_calls.xml" << endl;
  cout << "      Filters mpi calls of linpack.prv. Doesn't load it, just writes the file." << endl;
  cout << endl;
  cout << "    paramedir -s -c -f linpack.prv cut_filter_options.xml mpi_stats.cfg my_mpi_values.txt" << endl;
  cout << "      Reads parameters of the software counters, cutter and filter from the xml and applies them" << endl;
  cout << "      pipelined in the given order ( trace -> software counters | cutter | filter -> result trace)" << endl;
  cout << "      to linpack.prv trace, and the filtered trace is loaded and used to compute mpi_stats.cfg." << endl;
  cout << "      The computed mpi results are saved int my_mpi_values.txt." << endl;
  cout << endl;
}


void printVersion()
{
  cout << PACKAGE_STRING;

  bool reverseOrder = true;
  string auxDate = LabelConstructor::getDate( reverseOrder );

  if ( auxDate.compare("") != 0 )
    cout << " Build ";

  cout << auxDate << endl;
}


bool isOption( char *argument )
{
  return ( argument[ 0 ] == '-' );
}


bool isTrace( char *argument )
{
  string auxName( argument );
  string suffixCompressed( "" );
  string suffixNotCompressed( "" );

  if ( auxName.length() > string(".prv.gz").length() )
    suffixCompressed = auxName.substr( auxName.length() - string(".prv.gz").length() );

  if ( auxName.length() > string(".prv").length() )
    suffixNotCompressed = auxName.substr( auxName.length() - string(".prv").length() );

  return (( suffixCompressed.compare( ".prv.gz" ) == 0 ) ||
          ( suffixNotCompressed.compare( ".prv" ) == 0 ));
}


bool isXML( char *argument )
{
  string auxName( argument );
  string suffix( "" );

  if ( auxName.length() > string(".xml").length() )
    suffix = auxName.substr( auxName.length() - string(".xml").length() );

  return ( suffix.compare( ".xml" ) == 0 );
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
  else
    cout << "Unknown option " << argument << endl;
}

#ifdef BYTHREAD
void getDumpIterations( int &currentArg, char *argv[] )
{
  if ( dumpTrace )
  {
    ++currentArg;
    string strNumIter( argv[ currentArg ] );
    stringstream tmpNumIter( strNumIter );
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
    else if ( isTrace( arguments[ currentArg ] ))
    {
      strTrace = string( arguments[ currentArg ] );
    }
    else if ( isXML( arguments[ currentArg ] ))
    {
      strXMLOptions = string( arguments[ currentArg ] );
    }
    else if ( CFGLoader::isCFGFile( arguments[ currentArg ] ))
    {
      string strCfg( arguments[ currentArg ] );
      strOutputFile = strCfg.substr( 0, strCfg.length() - 4 );
      cfgs[ arguments[ currentArg ] ] = strOutputFile;
      previousCFGPosition = currentArg;
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
  return ( ( strXMLOptions != "" ) && ( filterToolOrder.size() > 0 ));
}


string applyFilters( KernelConnection *myKernel,
                     vector< string > &filterToolOrder,
                     vector< string > &tmpFiles )
{
  string tmpTraceIn, tmpTraceOut;
  char tmpNameIn[1024], tmpNameOut[1024], tmpPathOut[1024];
  string strOutputFile, strPathOut;

  // Only for cutter
  char *pcf_name;
  vector< TEventType > allTypes;
  vector< TEventType > typesWithValueZero;
  EventLabels labels;
  map< TEventValue, string > currentEventValues;
  ParaverTraceConfig *config;
  FILE *pcfFile;


  traceOptions = myKernel->newTraceOptions( );
  // The order is given by the command line, not the xml file.
  vector< string > xmlToolOrder = traceOptions->parseDoc( (char *)strXMLOptions.c_str() );

  // Concatenate Filter Utilities
  strcpy( tmpNameOut, (char *)strTrace.c_str() );

  size_t pos = strTrace.find_last_of( '/' );
  if ( pos != string::npos )
    strPathOut = strTrace.substr( 0, pos );
  else
    strPathOut = strTrace;

  strcpy( tmpPathOut, (char *)strPathOut.c_str() );

  for( PRV_UINT16 i = 0; i < filterToolOrder.size(); ++i )
  {
    strcpy( tmpNameIn, tmpNameOut );
    myKernel->getNewTraceName( tmpNameIn, tmpPathOut, filterToolOrder[ i ] );
    strcpy( tmpNameOut, tmpPathOut );

    if ( filterToolOrder[i] == TraceCutter::getID() )
    {
      pcf_name = myKernel->composeName( tmpNameIn, (char *)"pcf" );

      if(( pcfFile = fopen( pcf_name, "r" )) != NULL )
      {
        fclose( pcfFile );

        config = new ParaverTraceConfig( pcf_name );
        labels = EventLabels( *config, set<TEventType>() );
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

      traceCutter = myKernel->newTraceCutter( tmpNameIn,
                                              tmpNameOut,
                                              traceOptions,
                                              typesWithValueZero );
      myKernel->copyPCF( tmpNameIn, tmpNameOut );
    }
    else if ( filterToolOrder[i] == TraceFilter::getID() )
    {
      map< TTypeValuePair, TTypeValuePair > translation;
#if 1
      traceFilter = myKernel->newTraceFilter( tmpNameIn, tmpNameOut, traceOptions, translation );
#else
      translation[ make_pair( 30000000, 2 ) ] = make_pair( 666, 999 );
      translation[ make_pair( 30000001, 2 ) ] = make_pair( 666666, 999999 );
      translation[ make_pair( 30000002, 2 ) ] = make_pair( 666666666, 999999999 );
      translation[ make_pair( 30000003, 2 ) ] = make_pair( 666666666666, 999999999999 );

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

      traceFilter = myKernel->newTraceFilter( tmpNameIn, tmpNameOut, opts, translation );
#endif
      myKernel->copyPCF( tmpNameIn, tmpNameOut );
    }
    else if ( filterToolOrder[i] == TraceSoftwareCounters::getID() )
    {
      traceSoftwareCounters = myKernel->newTraceSoftwareCounters( tmpNameIn,
                                                                  tmpNameOut,
                                                                  traceOptions );
    }
    else
    {
    }

    myKernel->copyROW( tmpNameIn, tmpNameOut );
    tmpFiles.push_back( tmpNameOut );
  }

  // Delete intermediate files
  char *pcfName, *rowName;
  for( PRV_UINT16 i = 0; i < tmpFiles.size() - 1; ++i )
  {
    pcfName = myKernel->composeName( (char *)tmpFiles[ i ].c_str(), (char *)string("pcf").c_str() );
    rowName = myKernel->composeName( (char *)tmpFiles[ i ].c_str(), (char *)string("row").c_str() );
    remove( tmpFiles[ i ].c_str() );
    remove( pcfName );
    remove( rowName );
  }

  // Delete utilities
  delete traceOptions;
  delete traceCutter;
  delete traceFilter;
//  delete traceCommunicationsFusionTrace;
  delete traceSoftwareCounters;

  return string( tmpNameOut );
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
  for( map< string, string >::iterator it = cfgs.begin(); it != cfgs.end(); ++it )
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
      cout << "Cannot load '" << it->first << "' file." << endl;

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


int main( int argc, char *argv[] )
{
  if ( argc == 1 )
    printHelp();
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
          cout << "Cannot load " << strTrace << endl;
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
