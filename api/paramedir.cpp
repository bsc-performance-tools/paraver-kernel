#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "paramedir.h"
#include "cfg.h"
#include "trace.h"
#include "kwindow.h"
#include "cube.h"
#include "paraverkernelinit.h"

using namespace std;

bool multipleFiles = false;
Trace *trace;
UINT32 outputPrecision = 2;

int main( int argc, char *argv[] )
{
  // Cube< TSemanticValue > cube( 10, 10, 10 );

  paraverKernelInit();

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
      vector<KWindow *> windows;
      Histogram *histogram;
      TRecordTime beginTime;
      TRecordTime endTime;
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

      if ( CFGLoader::loadCFG( strCfg, trace, windows, histogram, beginTime, endTime ) )
      {
        ofstream outputFile;
        KWindow *tmpWindow = windows[ windows.size() - 1 ];

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
      else
        cout << "Cannot load '" << strCfg << "' file." << endl;

      for ( UINT32 i = 0; i < windows.size(); i++ )
      {
        if ( windows[ i ] != NULL )
          delete windows[ i ];
      }
      windows.clear();
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
