#include "gnuplotoutput.h"
#include "window.h"
#include "histogram.h"
#include "labelconstructor.h"

void GNUPlotOutput::dumpWindow( Window *whichWindow, string& strOutputFile )
{
  if( strOutputFile.rfind( string( ".gnuplot" ) ) == string::npos )
    strOutputFile += ".gnuplot";

  Output *textOutput = Output::createOutput( Output::TEXT );
  string strTextOutputFilename = strOutputFile + string( ".csv" );

  textOutput->setMultipleFiles( false );
  textOutput->dumpWindow( whichWindow, strTextOutputFilename );

  delete textOutput;

  ofstream outputFile;
  outputFile.open( strOutputFile.c_str() );

  outputFile << "plot ";
  for ( TObjectOrder i = 0; i < whichWindow->getWindowLevelObjects(); ++i )
  {
    outputFile << "\""<< strTextOutputFilename << "\" ";
    outputFile << "using 2:($1 == " << i + 1 << " ? $4 : 1/0) with lines title \"";
    outputFile << LabelConstructor::objectLabel( i, whichWindow->getLevel(), whichWindow->getTrace() ) << "\"";
    if( i < whichWindow->getWindowLevelObjects() - 1 )
      outputFile << ", \\";
    outputFile << endl;
  }
  outputFile << "pause -1" << endl;

  outputFile.close();
}

void GNUPlotOutput::dumpHistogram( Histogram *whichHisto,
                                   string& strOutputFile,
                                   bool onlySelectedPlane,
                                   bool hideEmptyColumns )
{

}

bool GNUPlotOutput::getMultipleFiles() const
{
  return false;
}

void GNUPlotOutput::setMultipleFiles( bool newValue )
{

}

