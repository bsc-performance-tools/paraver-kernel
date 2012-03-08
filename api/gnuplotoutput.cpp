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

#include "gnuplotoutput.h"
#include "window.h"
#include "histogram.h"
#include "labelconstructor.h"

using std::string;

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
                                   bool hideEmptyColumns,
                                   bool withLabels )
{
  if( strOutputFile.rfind( string( ".gnuplot" ) ) == string::npos )
    strOutputFile += ".gnuplot";

  Output *textOutput = Output::createOutput( Output::TEXT );
  string strTextOutputFilename = strOutputFile + string( ".csv" );

  textOutput->setMultipleFiles( false );
  textOutput->dumpHistogram( whichHisto, strTextOutputFilename, onlySelectedPlane, hideEmptyColumns, false );

  delete textOutput;

  ofstream outputFile;
  outputFile.open( strOutputFile.c_str() );
  outputFile << "splot " << "\""<< strTextOutputFilename << "\" matrix with pm3d" << endl;
  outputFile << "pause -1" << endl;
  outputFile.close();
}

bool GNUPlotOutput::getMultipleFiles() const
{
  return false;
}

void GNUPlotOutput::setMultipleFiles( bool newValue )
{

}

