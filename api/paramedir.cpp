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

using namespace std;

bool multipleFiles = false;
bool dumpTrace = false;
bool noLoad = false;
INT32 numIter = 1;
Trace *trace;

int main( int argc, char *argv[] )
{
  LocalKernel::init();

  KernelConnection *myKernel = new LocalKernel( NULL );

  ParaverConfig *config = ParaverConfig::getInstance();

  config->readParaverConfigFile();

  if ( argc == 1 )
    printHelp();
  else
  {
    INT32 currentArg = 1;
    // Read options
    while ( argv[ currentArg ][ 0 ] == '-' )
    {
      if ( argv[ currentArg ][ 1 ] == 'm' )
        multipleFiles = true;
      else if ( argv[ currentArg ][ 1 ] == 'd' )
      {
        dumpTrace = true;
        ++currentArg;
        string strNumIter( argv[ currentArg ] );
        stringstream tmpNumIter( strNumIter );
        if( !( tmpNumIter >> numIter ) )
        {
          numIter = 1;
          --currentArg;
        }
      }
      else if ( argv[ currentArg ][ 1 ] == 'n' )
        noLoad = true;
      ++currentArg;
    }

    // Read the trace
    string strTrace( argv[ currentArg ] );
    try
    {
      trace = Trace::create( myKernel, strTrace, noLoad, NULL );
    }
    catch ( ParaverKernelException& ex )
    {
      ex.printMessage();
      cout << "Cannot load " << strTrace << endl;
      exit( 0 );
    }
    currentArg++;

    if ( dumpTrace )
#ifdef BYTHREAD
      trace->dumpFile( strTrace + ".new.bythread", numIter );
#else
      trace->dumpFile( strTrace + ".new.global" );
#endif
    //Read the cfgs list and create the output files for each one
    while ( currentArg < argc )
    {
      string strCfg( argv[ currentArg ] );
      vector<Window *> windows;
      vector<Histogram *> histograms;
      string strOutputFile;

      if ( CFGLoader::isCFGFile( strCfg ) )
      {
        string strOut;
        if ( argc > currentArg + 1 )
        {
          strOut = argv[ currentArg + 1 ];
          if ( !CFGLoader::isCFGFile( strOut ) )
          {
            strOutputFile = strOut;
            ++currentArg;
          }
          else
          {
            strOutputFile = strCfg.substr( 0, strCfg.length() - 4 );
          }

        }
        else
        {
          strOutputFile = strCfg.substr( 0, strCfg.length() - 4 );
        }
      }

      if ( CFGLoader::loadCFG( myKernel, strCfg, trace, windows, histograms ) )
      {
        TextOutput output;
        output.setMultipleFiles( multipleFiles );

        if ( histograms.begin() != histograms.end() &&
             histograms[ histograms.size() - 1 ] != NULL )
          output.dumpHistogram( histograms[ histograms.size() - 1 ], strOutputFile );
        else if( windows.begin() != windows.end() &&
                 windows[ windows.size() - 1 ] != NULL )
          output.dumpWindow( windows[ windows.size() - 1 ], strOutputFile );
      }
      else
        cout << "Cannot load '" << strCfg << "' file." << endl;


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

      ++currentArg;
    }

    delete trace;
  }

  return 1;
}


void printHelp()
{
  cout << "USAGE" << endl;
  cout << "  paramedir [OPTION] trc cfg {out | cfg}*" << endl;
  cout << endl;
  cout << "  Options:" << endl;
  cout << "  -m: Prints on multiple files." << endl;
  cout << endl;
  cout << "  Parameters:" << endl;
  cout << "  trc: Paraver trace filename (ASCII: *.prv)." << endl;
  cout << "  cfg: Paraver configuration filename (*.cfg)." << endl;
  cout << "  out: Filename for output (default name is cfg filename without .cfg)." << endl;
}

