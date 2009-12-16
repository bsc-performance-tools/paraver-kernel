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

using namespace std;

bool multipleFiles = false;
bool dumpTrace = false;
bool noLoad = false;
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
        dumpTrace = true;
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
      trace->dumpFile( strTrace + ".new.bythread" );
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
        if ( histograms.begin() != histograms.end() &&
             histograms[ histograms.size() - 1 ] != NULL )
          dumpHistogram( histograms, strOutputFile );
        else
          dumpWindow( windows, strOutputFile );
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


void dumpWindow( vector<Window *>& windows, string& strOutputFile )
{
  TRecordTime beginTime;
  TRecordTime endTime;
  ofstream outputFile;
  Window *tmpWindow = windows[ windows.size() - 1 ];
  ParaverConfig *config = ParaverConfig::getInstance();

  beginTime = tmpWindow->getWindowBeginTime();
  endTime = tmpWindow->getWindowEndTime();

  tmpWindow->init( beginTime, NOCREATE );

  if ( endTime > trace->getEndTime() )
    endTime = trace->getEndTime();

  if ( !multipleFiles )
    outputFile.open( strOutputFile.c_str() );

  for ( TObjectOrder i = 0; i < tmpWindow->getWindowLevelObjects(); ++i )
  {
    if ( multipleFiles )
    {
      ostringstream tmpName;
      tmpName << strOutputFile  << "_" << setw( 5 ) << setfill( '0' ) << i + 1;
      outputFile.open( tmpName.str().c_str() );
    }

    outputFile << fixed;
    outputFile << showpoint;

    while ( tmpWindow->getEndTime( i ) < endTime )
    {
      outputFile << setprecision( config->getTimelinePrecision() );
      if ( !multipleFiles )
        outputFile << i + 1 << "\t";
      outputFile << tmpWindow->traceUnitsToWindowUnits(
        tmpWindow->getBeginTime( i ) ) << "\t";
      outputFile << tmpWindow->traceUnitsToWindowUnits(
        tmpWindow->getEndTime( i ) - tmpWindow->getBeginTime( i ) ) << "\t";
      outputFile << tmpWindow->getValue( i ) << endl;
      tmpWindow->calcNext( i );
    }
    outputFile << setprecision( config->getHistogramPrecision() );
    if ( !multipleFiles )
      outputFile << i + 1 << "\t";
    outputFile << tmpWindow->traceUnitsToWindowUnits(
      tmpWindow->getBeginTime( i ) ) << "\t";
    outputFile << tmpWindow->traceUnitsToWindowUnits(
      tmpWindow->getEndTime( i ) - tmpWindow->getBeginTime( i ) ) << "\t";
    outputFile << tmpWindow->getValue( i ) << endl;

    if ( multipleFiles )
      outputFile.close();
  }

  if ( multipleFiles )
    cout << strOutputFile << "_* files wrote." << endl;
  else
  {
    outputFile.close();
    cout << strOutputFile << " file wrote." << endl;
  }
}

void dumpHistogram( vector<Histogram *>& histograms, string& strOutputFile )
{
  THistogramColumn numPlanes;
  THistogramColumn numColumns;
  TObjectOrder numRows;
  ofstream outputFile;
  Histogram *histo = histograms[ histograms.size() - 1 ];

  vector<TObjectOrder> selectedRows;
  TObjectOrder beginRow = histo->getControlWindow()->getZoomSecondDimension().first;
  TObjectOrder endRow =  histo->getControlWindow()->getZoomSecondDimension().second;
  histo->getControlWindow()->getSelectedRows( histo->getControlWindow()->getLevel(),
      selectedRows, beginRow, endRow );
  histo->execute( histo->getBeginTime(), histo->getEndTime(), selectedRows );
  numPlanes = histo->getNumPlanes();
  numColumns = histo->getNumColumns();
  numRows = histo->getNumRows();

  outputFile.open( strOutputFile.c_str() );

  outputFile << fixed;
  outputFile << showpoint;

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; ++iPlane )
  {
    if ( numPlanes > 1 )
      outputFile << histo->getPlaneLabel( iPlane ) << endl;

    outputFile << "\t";
    // Initialize all columns in this plane
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
    {
      histo->setFirstCell( iColumn, iPlane );
      outputFile << histo->getColumnLabel( iColumn ) << "\t";
    }
    outputFile << endl;

    for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
    {
      outputFile << histo->getRowLabel( iRow ) << "\t";
      for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
      {
        if ( histo->getCurrentRow( iColumn, iPlane ) == iRow )
        {
          outputFile << histo->getCurrentValue( iColumn, 0, iPlane ) << "\t";
          histo->setNextCell( iColumn, iPlane );
        }
        else
          outputFile << 0.0 << "\t";
      }
      outputFile << endl;
    }
    outputFile << endl;
    // Print totals
    HistogramTotals *totals = histo->getColumnTotals();
    outputFile << "Total" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
    {
      outputFile << totals->getTotal( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    outputFile << "Average" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
    {
      outputFile << totals->getAverage( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    outputFile << "Maximum" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
    {
      outputFile << totals->getMaximum( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    outputFile << "Minimum" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
    {
      outputFile << totals->getMinimum( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    outputFile << "Stdev" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
    {
      outputFile << totals->getStdev( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    outputFile << "Avg/Max" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
    {
      outputFile << totals->getAvgDivMax( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    delete totals;
    outputFile << endl;
  }

  outputFile.close();
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

