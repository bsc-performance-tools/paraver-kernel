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

using namespace std;

bool multipleFiles = false;
Trace *trace;
UINT32 outputPrecision = 2;

int main( int argc, char *argv[] )
{
  LocalKernel::init();

  KernelConnection *myKernel = new LocalKernel();

  readParaverConfigFile();

  if ( argc == 1 )
    printHelp();
  else
  {
    INT32 currentArg = 1;
    // Read options ( Deberia ser un while si hay mas opciones. De momento solo hay esta)
    if ( argv[ currentArg ][ 0 ] == '-' )
    {
      if ( argv[ currentArg ][ 1 ] == 'm' )
      {
        multipleFiles = true;
      }
      currentArg++;
    }

    // Read the trace
    string strTrace( argv[ currentArg ] );
    try
    {
      trace = new Trace( strTrace );
    }
    catch ( ParaverKernelException& ex )
    {
      ex.printMessage();
      cout << "Cannot load " << strTrace << endl;
      exit( 0 );
    }
    currentArg++;

    //Read the cfgs list and create the output files for each one
    while ( currentArg < argc )
    {
      string strCfg( argv[ currentArg ] );
      vector<Window *> windows;
      Histogram *histogram = NULL;
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
            currentArg++;
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
        currentArg++;
      }

      if ( CFGLoader::loadCFG( myKernel, strCfg, trace, windows, histogram ) )
      {
        if ( histogram != NULL )
          dumpHistogram( histogram, strOutputFile );
        else
          dumpWindow( windows, strOutputFile );
      }
      else
        cout << "Cannot load '" << strCfg << "' file." << endl;

      for ( UINT32 i = 0; i < windows.size(); i++ )
      {
        if ( windows[ i ] != NULL )
          delete windows[ i ];
      }
      windows.clear();

      if ( histogram != NULL )
        delete histogram;
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

  beginTime = tmpWindow->getWindowBeginTime();
  endTime = tmpWindow->getWindowEndTime();

  tmpWindow->init( beginTime, NOCREATE );

  if ( endTime > trace->getEndTime() )
    endTime = trace->getEndTime();

  if ( !multipleFiles )
    outputFile.open( strOutputFile.c_str() );

  for ( TObjectOrder i = 0; i < tmpWindow->getWindowLevelObjects(); i++ )
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
      outputFile << setprecision( outputPrecision );
      if ( !multipleFiles )
        outputFile << i + 1 << "\t";
      outputFile << tmpWindow->traceUnitsToWindowUnits(
        tmpWindow->getBeginTime( i ) ) << "\t";
      outputFile << tmpWindow->traceUnitsToWindowUnits(
        tmpWindow->getEndTime( i ) - tmpWindow->getBeginTime( i ) ) << "\t";
      outputFile << tmpWindow->getValue( i ) << endl;
      tmpWindow->calcNext( i );
    }
    outputFile << setprecision( outputPrecision );
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

void dumpHistogram( Histogram *histo, string& strOutputFile )
{
  THistogramColumn numPlanes;
  THistogramColumn numColumns;
  TObjectOrder numRows;
  ofstream outputFile;

  histo->execute( histo->getBeginTime(), histo->getEndTime() );
  numPlanes = histo->getNumPlanes();
  numColumns = histo->getNumColumns();
  numRows = histo->getNumRows();

  outputFile.open( strOutputFile.c_str() );

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    if ( numPlanes > 1 )
      outputFile << "Plane no. " << iPlane << endl;

    outputFile << "\t";
    // Initialize all columns in this plane
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; iColumn++ )
    {
      histo->setFirstCell( iColumn, iPlane );
      outputFile << "Column no. " << iColumn << "\t";
    }
    outputFile << endl;

    for ( TObjectOrder iRow = 0; iRow < numRows; iRow++ )
    {
      outputFile << "Row no. " << iRow << "\t";
      for ( THistogramColumn iColumn = 0; iColumn < numColumns; iColumn++ )
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
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; iColumn++ )
    {
      outputFile << totals->getTotal( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    outputFile << "Average" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; iColumn++ )
    {
      outputFile << totals->getAverage( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    outputFile << "Maximum" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; iColumn++ )
    {
      outputFile << totals->getMaximum( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    outputFile << "Minimum" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; iColumn++ )
    {
      outputFile << totals->getMinimum( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    outputFile << "Stdev" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; iColumn++ )
    {
      outputFile << totals->getStdev( 0, iColumn, iPlane ) << "\t";
    }
    outputFile << endl;

    outputFile << "Avg/Max" << "\t";
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; iColumn++ )
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


void readParaverConfigFile()
{
  ifstream file;
  string strLine;
  string strTag;
  string strFile;

  strFile.append( getenv( "HOME" ) );
  strFile.append( "/.paraver/paraver" );

  file.open( strFile.c_str() );

  if ( !file )
    return;

  while ( !file.eof() )
  {
    getline( file, strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' || strLine[ 0 ] == '<' )
      continue;

    istringstream auxStream( strLine );
    getline( auxStream, strTag, ' ' );

    if ( strTag.compare( "WhatWhere.num_decimals:" ) == 0 )
    {
      string strNumDecimals;

      getline( auxStream, strNumDecimals );
      istringstream streamNumDecimals( strNumDecimals );

      streamNumDecimals >> outputPrecision;
    }
  }
  file.close();
}
