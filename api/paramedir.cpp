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

using namespace std;

bool showHelp = false;
bool multipleFiles = false;
bool dumpTrace = false;
bool noLoad = false;

INT32 numIter = 1;
Trace *trace;

// Filter / Cutter global variables
bool cutTrace = false;
bool filterTrace = false;
bool commFusionTrace = false;
bool softwareCountersTrace = false;
// bool communicationsFusionTrace = false;
TraceOptions *traceOptions = NULL;
TraceCutter *traceCutter = NULL;
TraceFilter *traceFilter = NULL;
//TraceCommunicationsFusion *traceCommunicationsFusion = NULL;
TraceSoftwareCounters *traceSoftwareCounters = NULL;
string strXMLOptions( "" );
map< string, string > cfgs;
string strTrace( "" );

void printHelp()
{
  cout << "USAGE" << endl;
  cout << "  paramedir [OPTION] trc {xml} cfg {out | cfg}*" << endl;
  cout << endl;
  cout << "    -h: Prints this help" << endl;
  cout << endl;
  cout << "  Output file options:" << endl;
  cout << "    -m: Prints on multiple files." << endl;
  cout << endl;
  cout << "  Cutter/Filter options ( xml options file needed ):" << endl;
  cout << "    -c: Apply Cutter." << endl;
//  cout << "    -f: Apply Filter. (in progress)" << endl;
//  cout << "    -j: Apply Communications Fusion. (in progress)" << endl;
//  cout << "    -s: Apply Software Counters. (in progress)" << endl;
  cout << endl;
  cout << "  Parameters:" << endl;
  cout << "    trc: Paraver trace filename ( with extension .prv or .prv.gz )." << endl;
  cout << "    xml: Options for cutter/filter ( with extension .xml )." << endl;
  cout << "    cfg: Paraver configuration filename ( with extension .cfg ). If present, trace's loaded." << endl;
  cout << "    out: Filename for output ( default name is cfg filename without, with extension .mcr )." << endl;
  cout << endl;
  cout << "  Examples:" << endl;
  cout << "    paramedir -m linpack.prv.gz mpi_stats.cfg" << endl;
  cout << "      Computes the mpi_stats.cfg analysis of compressed trace linpack.prv." << endl;
  cout << endl;
  cout << "    paramedir -f linpack.prv just_MPI_calls.xml" << endl;
  cout << "      Filters mpi calls of linpack.prv. Doesn't load it, just writes the file." << endl;
  cout << endl;
  cout << "    paramedir -c -f linpack.prv cut_filter_options.xml mpi_stats.cfg" << endl;
  cout << "      Reads parameters of the cutter and the filter from the xml and applies them to" << endl;
  cout << "      linpack.prv trace before load it and compute mpi_stats.cfg." << endl;
  cout << endl;
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


void activateOption( char *argument )
{
  if ( argument[ 1 ] == 'h' )
    showHelp = true;
  else if ( argument[ 1 ] == 'm' )
    multipleFiles = true;
  else if ( argument[ 1 ] == 'd' )
    dumpTrace = true;
  else if ( argument[ 1 ] == 'n' )
    noLoad = true;
  else if ( argument[ 1 ] == 'c' )
    cutTrace = true;
  else if ( argument[ 1 ] == 'f' )
    filterTrace = true;
//      else if ( argv[ currentArg ][ 1 ] ) == 'j' )
//        communicationsFusionTrace = true;
  else if ( argument[ 1 ] == 's' )
    softwareCountersTrace = true;
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


void readParameters( int argc, char *arguments[] )
{
  string strOut;
  string strOutputFile;
  INT32 previousCFGPosition = 0;
  INT32 currentArg = 1;

  while ( currentArg < argc )
  {
    if ( isOption( arguments[ currentArg ] ))
    {
      activateOption( arguments[ currentArg ] );
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
  if ( !cutTrace && !filterTrace && !softwareCountersTrace && ( strXMLOptions != "" ) )
  {
    cutTrace = true;
    // filterTrace = true;
    // softwareCountersTrace = true;
  }
}


bool anyTrace()
{
  return ( strTrace != "" );
}

bool anyCFG()
{
  return ( cfgs.size() > 0 );
}


bool anyFilter()
{
  return ( ( strXMLOptions != "" ) &&
           ( cutTrace || filterTrace || softwareCountersTrace ));
}


string applyFilters( KernelConnection *myKernel )
{
  string tmpTraceIn, tmpTraceOut;
  char *tmpNameIn, *tmpNameOut;
  string strOutputFile;

  traceOptions = myKernel->newTraceOptions( );
  traceOptions->parseDoc( (char *)strXMLOptions.c_str() );

  // Generate trace name properly; how do we number it?

  // Concatenate Filter Utilities

  if ( !cutTrace )
    tmpNameOut = (char *)strTrace.c_str();
  else
  {
    tmpNameIn = (char *)strTrace.c_str();
    tmpNameOut = (char *)string( "tmpcutted.prv" ).c_str();

    traceCutter = myKernel->newTraceCutter( tmpNameIn,
                                            tmpNameOut,
                                            traceOptions );
  }

  // tmpNameOut set
  // <---
  filterTrace = false;

  if ( filterTrace )
  {
    tmpNameIn = tmpNameOut;
    tmpNameOut = (char *)string( "tmpcutted2.prv" ).c_str();
    traceFilter = myKernel->newTraceFilter( tmpNameIn,
                                            tmpNameOut,
                                            traceOptions );
  }
/*
  if ( !softwareCountersTrace )
    strOutputFile = tmpFile2;
  else
    traceSoftwareCounters = myKernel->newTraceSoftwareCounters( (char *)tmpFile2.c_str(),
                                                                (char *)strOutputFile.c_str(),
                                                                traceOptions );
*/

//      traceCommunicationsFusionTrace = myKernel->newTraceCommunicationsFusionTrace();

  // Delete temporal files

  // Delete utilities
//        delete traceOptions;

//        delete traceCutter;
//        delete traceFilter;
//  delete traceCommunicationsFusionTrace;
//  delete traceSoftwareCounters;
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

    if ( CFGLoader::loadCFG( myKernel, it->first, trace, windows, histograms ) )
    {
      TextOutput output;
      output.setMultipleFiles( multipleFiles );

      if ( histograms.begin() != histograms.end() &&
           histograms[ histograms.size() - 1 ] != NULL )
        output.dumpHistogram( histograms[ histograms.size() - 1 ], it->second );
      else if( windows.begin() != windows.end() &&
               windows[ windows.size() - 1 ] != NULL )
        output.dumpWindow( windows[ windows.size() - 1 ], it->second );
    }
    else
      cout << "Cannot load '" << it->first << "' file." << endl;

    for ( UINT32 i = 0; i < histograms.size(); ++i )
    {
      if ( histograms[ i ] != NULL )
        delete histograms[ i ];
    }

    histograms.clear();


    for ( UINT32 i = 0; i < windows.size(); ++i )
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
    // Initializations
    LocalKernel::init();
    KernelConnection *myKernel = new LocalKernel( NULL );
    ParaverConfig *config = ParaverConfig::getInstance();
    config->readParaverConfigFile();

    readParameters( argc, argv );

    // Execute
    if ( showHelp )
      printHelp();
    else if ( anyTrace() )
    {
      if ( anyFilter() )
      {
        strTrace = applyFilters( myKernel );
        cout << " ----> written trace!" << strTrace << endl;
        // copy .pcf and .row ?
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

  return 1;
}
