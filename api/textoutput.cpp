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


#include "textoutput.h"
#include "window.h"
#include "histogram.h"
#include "labelconstructor.h"

using namespace std;

TextOutput::TextOutput()
    : multipleFiles( false ), objectHierarchy( false ), windowTimeUnits( true ),
      textualSemantic( false ), minTime( 0.0 )
{}

TextOutput::~TextOutput()
{}

void TextOutput::dumpWindow( Window *whichWindow, string& strOutputFile, ProgressController *progress )
{
  static short int progressSteps = 0;

  if( strOutputFile.rfind( string( ".csv" ) ) == string::npos )
    strOutputFile += ".csv";

  TRecordTime beginTime;
  TRecordTime endTime;
  ofstream outputFile;

  ParaverConfig *config = ParaverConfig::getInstance();
  Trace *trace = whichWindow->getTrace();

  beginTime = whichWindow->getWindowBeginTime();
  endTime = whichWindow->getWindowEndTime();

  whichWindow->setComputeYMaxOnInit( false );
  whichWindow->init( beginTime, NOCREATE );

  if ( endTime > trace->getEndTime() )
    endTime = trace->getEndTime();

  if ( !multipleFiles )
    outputFile.open( strOutputFile.c_str() );

  maxTime = 0.0;

  // Progress controller
  std::string previousMessage;
  if( progress != NULL )
  {
    //previousMessage = progress->getMessage();
    //progress->setMessage( strOutputFile );
    if ( whichWindow->getWindowLevelObjects() > 1 )
      progress->setEndLimit( whichWindow->getWindowLevelObjects() - 1 );
    else
      progress->setEndLimit( endTime - beginTime );
    progress->setCurrentProgress( 0 );
  }

  for ( TObjectOrder i = 0; i < whichWindow->getWindowLevelObjects(); ++i )
  {
    if( progress != NULL )
    {
      if ( whichWindow->getWindowLevelObjects() > 1 )
        progress->setCurrentProgress( i );
    }

    if ( multipleFiles )
    {
      ostringstream tmpName;
      tmpName << strOutputFile  << "_" << setw( 5 ) << setfill( '0' ) << i + 1;
      outputFile.open( tmpName.str().c_str() );
    }

    outputFile << fixed;
    outputFile << showpoint;

    whichWindow->initRow( i, beginTime, NOCREATE );
    if( i == 0 || whichWindow->getBeginTime( i ) < minTime )
      minTime = whichWindow->getBeginTime( i );

    while ( whichWindow->getEndTime( i ) < endTime )
    {
      if( progress != NULL )
      {
        if ( whichWindow->getWindowLevelObjects() == 1 )
        {
          if( ++progressSteps >= 100 )
          {
            progressSteps = 0;
            progress->setCurrentProgress( whichWindow->getEndTime( i ) - beginTime );
          }
        }
      }

      outputFile << setprecision( config->getTimelinePrecision() );
      if ( !multipleFiles )
      {
        if( objectHierarchy )
          outputFile << LabelConstructor::objectLabel( i, whichWindow->getLevel(), trace, false, false ) << "\t";
        else
          outputFile << i + 1 << "\t";
      }

      if( windowTimeUnits )
      {
        outputFile << whichWindow->traceUnitsToWindowUnits(
          whichWindow->getBeginTime( i ) ) << "\t";
        outputFile << whichWindow->traceUnitsToWindowUnits(
          whichWindow->getEndTime( i ) - whichWindow->getBeginTime( i ) ) << "\t";
      }
      else
      {
        outputFile << (PRV_UINT64)whichWindow->getBeginTime( i ) << "\t";
        outputFile << (PRV_UINT64)( whichWindow->getEndTime( i ) - whichWindow->getBeginTime( i ) ) << "\t";
      }

      if( textualSemantic )
        outputFile << LabelConstructor::semanticLabel( whichWindow, whichWindow->getValue( i ), true, config->getTimelinePrecision() ) << endl;
      else
        outputFile << whichWindow->getValue( i ) << endl;

      whichWindow->calcNext( i );
    }
    outputFile << setprecision( config->getTimelinePrecision() );
    if ( !multipleFiles )
    {
      if( objectHierarchy )
        outputFile << LabelConstructor::objectLabel( i, whichWindow->getLevel(), trace, false, false ) << "\t";
      else
        outputFile << i + 1 << "\t";
    }
    if( windowTimeUnits )
    {
      outputFile << whichWindow->traceUnitsToWindowUnits(
        whichWindow->getBeginTime( i ) ) << "\t";
      outputFile << whichWindow->traceUnitsToWindowUnits(
        whichWindow->getEndTime( i ) - whichWindow->getBeginTime( i ) ) << "\t";
    }
    else
    {
      outputFile << (PRV_UINT64)whichWindow->getBeginTime( i ) << "\t";
      outputFile << (PRV_UINT64)( whichWindow->getEndTime( i ) - whichWindow->getBeginTime( i ) ) << "\t";
    }

    if( textualSemantic )
      outputFile << LabelConstructor::semanticLabel( whichWindow, whichWindow->getValue( i ), true, config->getTimelinePrecision() ) << endl;
    else
      outputFile << whichWindow->getValue( i ) << endl;

    if( whichWindow->getEndTime( i ) > maxTime )
      maxTime = whichWindow->getEndTime( i );

    if ( multipleFiles )
      outputFile.close();
  }

  if ( multipleFiles )
    cout << strOutputFile << "_* files written." << endl;
  else
  {
    outputFile.close();
    cout << strOutputFile << " file written." << endl;
  }

  if ( !multipleFiles )
    outputFile.close();

  //if( progress != NULL )
  //{
  //  progress->setMessage( previousMessage );
  //}
}


void TextOutput::dumpHistogram( Histogram *whichHisto,
                                string& strOutputFile,
                                bool onlySelectedPlane,
                                bool hideEmptyColumns,
                                bool withLabels,
                                bool withPreferencesPrecision,
                                bool recalcHisto,
                                ProgressController *progress )
{
  THistogramColumn numPlanes;
  THistogramColumn numColumns;
  THistogramColumn iPlane;
  TObjectOrder numRows;
  PRV_UINT16 currentStat;
  ofstream outputFile;

  vector<TObjectOrder> selectedRows;
  TObjectOrder beginRow = whichHisto->getControlWindow()->getZoomSecondDimension().first;
  TObjectOrder endRow =  whichHisto->getControlWindow()->getZoomSecondDimension().second;
  whichHisto->getControlWindow()->getSelectedRows( whichHisto->getControlWindow()->getLevel(),
      selectedRows, beginRow, endRow );

  if( recalcHisto )
    whichHisto->execute( whichHisto->getBeginTime(), whichHisto->getEndTime(), selectedRows, progress );

  outputFile.open( strOutputFile.c_str() );

  outputFile << fixed;
  outputFile << showpoint;
  if( withPreferencesPrecision )
    outputFile.precision( ParaverConfig::getInstance()->getHistogramPrecision() );
  else
    outputFile.precision( 10 );

  numPlanes = whichHisto->getNumPlanes();
  numColumns = whichHisto->getNumColumns();
  numRows = whichHisto->getNumRows();

  // Column vector for normal behavior (print all columns)
  vector<THistogramColumn> printedColumns;
  // if !hideEmptyColumns, histogram preferences are used.
  hideEmptyColumns = hideEmptyColumns || whichHisto->getHideColumns();

  whichHisto->getIdStat( whichHisto->getCurrentStat(), currentStat );

  bool isCommStatistic = whichHisto->itsCommunicationStat( whichHisto->getCurrentStat() );
  if ( !isCommStatistic )
  {
    if ( onlySelectedPlane )
      iPlane = whichHisto->getSelectedPlane();
    else
      iPlane = 0;

    while ( iPlane < numPlanes )
    {
      if ( whichHisto->planeWithValues( iPlane ) )
      {
        if ( numPlanes > 1 && withLabels )
          outputFile << whichHisto->getPlaneLabel( iPlane ) << endl;

        outputFile << "\t";

        // Initialize all columns in this plane
        printedColumns.clear();
        numColumns = whichHisto->getPlaneColumns( iPlane, hideEmptyColumns, printedColumns );

        for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
        {
          whichHisto->setFirstCell( printedColumns[ iColumn ], iPlane );
          if ( whichHisto->getHorizontal() && withLabels )
            outputFile << whichHisto->getColumnLabel( printedColumns[ iColumn ] ) << "\t";
        }

        // Dump data
        HistogramTotals *totals = NULL;
        if ( whichHisto->getHorizontal() )
        {
          dumpMatrixHorizontal( whichHisto, numRows, numColumns, currentStat, printedColumns, iPlane, outputFile, withLabels );

          outputFile << endl;

          if( withLabels )
          {
            totals = whichHisto->getColumnTotals();
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( TOTAL ),     &HistogramTotals::getTotal,     currentStat, printedColumns, iPlane, outputFile );
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( AVERAGE ),   &HistogramTotals::getAverage,   currentStat, printedColumns, iPlane, outputFile );
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( MAXIMUM ),   &HistogramTotals::getMaximum,   currentStat, printedColumns, iPlane, outputFile );
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( MINIMUM ),   &HistogramTotals::getMinimum,   currentStat, printedColumns, iPlane, outputFile );
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( STDEV ),     &HistogramTotals::getStdev,     currentStat, printedColumns, iPlane, outputFile );
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( AVGDIVMAX ), &HistogramTotals::getAvgDivMax, currentStat, printedColumns, iPlane, outputFile );
          }
        }
        else
        {
          dumpMatrixVertical( whichHisto, numRows, numColumns, currentStat, printedColumns, iPlane, outputFile, withLabels );

          outputFile << endl;

          if( withLabels )
          {
            totals = whichHisto->getRowTotals();
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( TOTAL ),     &HistogramTotals::getTotal,     currentStat, numRows, iPlane, outputFile );
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( AVERAGE ),   &HistogramTotals::getAverage,   currentStat, numRows, iPlane, outputFile );
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( MAXIMUM ),   &HistogramTotals::getMaximum,   currentStat, numRows, iPlane, outputFile );
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( MINIMUM ),   &HistogramTotals::getMinimum,   currentStat, numRows, iPlane, outputFile );
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( STDEV ),     &HistogramTotals::getStdev,     currentStat, numRows, iPlane, outputFile );
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( AVGDIVMAX ), &HistogramTotals::getAvgDivMax, currentStat, numRows, iPlane, outputFile );
          }
        }

        if( totals != NULL )
          delete totals;
        outputFile << endl;
      } // plane whith values

      if ( onlySelectedPlane )
        iPlane = numPlanes; // this will end the external loop
      else
        ++iPlane;
    }
  }
  else // Communications statistic
  {
    if ( onlySelectedPlane )
      iPlane = whichHisto->getCommSelectedPlane();
    else
      iPlane = 0;
    while ( iPlane < numPlanes )
    {
      if ( whichHisto->planeCommWithValues( iPlane ) )
      {
        if ( numPlanes > 1 && withLabels )
          outputFile << whichHisto->getPlaneLabel( iPlane ) << endl;

        outputFile << "\t";

        // Initialize all columns in this plane
        printedColumns.clear();
        numColumns = whichHisto->getPlaneColumns( iPlane, hideEmptyColumns, printedColumns );

        for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
        {
          whichHisto->setCommFirstCell( printedColumns[ iColumn ], iPlane );
          if ( whichHisto->getHorizontal() && withLabels )
            outputFile << whichHisto->getColumnLabel( printedColumns[ iColumn ] ) << "\t";
        }

        HistogramTotals *totals = NULL;
        if ( whichHisto->getHorizontal() )
        {
          dumpMatrixCommHorizontal( whichHisto, numRows, numColumns, currentStat, printedColumns, iPlane, outputFile, withLabels );

          outputFile << endl;

          if( withLabels )
          {
            totals = whichHisto->getCommColumnTotals();
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( TOTAL ),     &HistogramTotals::getTotal,     currentStat, printedColumns, iPlane, outputFile );
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( AVERAGE ),   &HistogramTotals::getAverage,   currentStat, printedColumns, iPlane, outputFile );
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( MAXIMUM ),   &HistogramTotals::getMaximum,   currentStat, printedColumns, iPlane, outputFile );
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( MINIMUM ),   &HistogramTotals::getMinimum,   currentStat, printedColumns, iPlane, outputFile );
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( STDEV ),     &HistogramTotals::getStdev,     currentStat, printedColumns, iPlane, outputFile );
            dumpTotalColumns( totals, LabelConstructor::histoTotalLabel( AVGDIVMAX ), &HistogramTotals::getAvgDivMax, currentStat, printedColumns, iPlane, outputFile );
          }
        }
        else
        {
          dumpMatrixCommVertical( whichHisto, numRows, numColumns, currentStat, printedColumns, iPlane, outputFile, withLabels );

          outputFile << endl;

          if( withLabels )
          {
            totals = whichHisto->getCommRowTotals();
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( TOTAL ),     &HistogramTotals::getTotal,     currentStat, numRows, iPlane, outputFile );
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( AVERAGE ),   &HistogramTotals::getAverage,   currentStat, numRows, iPlane, outputFile );
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( MAXIMUM ),   &HistogramTotals::getMaximum,   currentStat, numRows, iPlane, outputFile );
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( MINIMUM ),   &HistogramTotals::getMinimum,   currentStat, numRows, iPlane, outputFile );
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( STDEV ),     &HistogramTotals::getStdev,     currentStat, numRows, iPlane, outputFile );
            dumpTotalRows( totals, LabelConstructor::histoTotalLabel( AVGDIVMAX ), &HistogramTotals::getAvgDivMax, currentStat, numRows, iPlane, outputFile );
          }
        }

        if( totals != NULL )
          delete totals;
        outputFile << endl;
      } // plane whith values

      if ( onlySelectedPlane )
        iPlane = numPlanes; // this will end the external loop
      else
        ++iPlane;
    }
  }

  outputFile.close();
}


void TextOutput::dumpMatrixHorizontal( Histogram *whichHisto,
                                       TObjectOrder numRows,
                                       THistogramColumn numColumns,
                                       PRV_UINT16 currentStat,
                                       vector<THistogramColumn> printedColumns,
                                       THistogramColumn iPlane,
                                       ofstream &outputFile,
                                       bool withLabels,
                                       ProgressController *progress )
{
  outputFile << endl;

  for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
  {
    if( withLabels )
      outputFile << whichHisto->getRowLabel( iRow ) << "\t";

    // progress advanced by rows (external loop)
    if( progress != NULL )
    {
      if ( numRows > 1 )
        progress->setCurrentProgress( (int)iRow );
    }

    for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
    {
      if ( !whichHisto->endCell( printedColumns[ iColumn ], iPlane ) )
      {
        if ( whichHisto->getCurrentRow( printedColumns[ iColumn ], iPlane ) == iRow )
        {
          outputFile << whichHisto->getCurrentValue( printedColumns[ iColumn ], currentStat, iPlane ) << "\t";
          whichHisto->setNextCell( printedColumns[ iColumn ], iPlane );
        }
        else
          outputFile << 0.0 << "\t";
      }
      else
        outputFile << 0.0 << "\t";
    }
    outputFile << endl;
  }
}

// progress( columns )
void TextOutput::dumpMatrixVertical( Histogram *whichHisto,
                                     TObjectOrder numRows,
                                     THistogramColumn numColumns,
                                     PRV_UINT16 currentStat,
                                     vector<THistogramColumn> printedColumns,
                                     THistogramColumn iPlane,
                                     ofstream &outputFile,
                                     bool withLabels,
                                     ProgressController *progress )
{
  if( withLabels )
  {
    for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
      outputFile << whichHisto->getRowLabel( iRow ) << "\t";
  }

  outputFile << endl;
  for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
  {
    if( withLabels )
      outputFile << whichHisto->getColumnLabel( printedColumns[ iColumn ] ) << "\t";

    // progress advanced by columns (external loop)
    if( progress != NULL )
    {
      if ( numColumns > 1 )
        progress->setCurrentProgress( (int)iColumn );
    }

    for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
    {
      if ( !whichHisto->endCell( printedColumns[ iColumn ], iPlane ) )
      {
        if ( whichHisto->getCurrentRow( printedColumns[ iColumn ], iPlane ) == iRow )
        {
          outputFile << whichHisto->getCurrentValue( printedColumns[ iColumn ], currentStat, iPlane ) << "\t";
          whichHisto->setNextCell( printedColumns[ iColumn ], iPlane );
        }
        else
          outputFile << 0.0 << "\t";
      }
      else
        outputFile << 0.0 << "\t";
    }
    outputFile << endl;
  }
}


void TextOutput::dumpMatrixCommHorizontal( Histogram *whichHisto,
                                           TObjectOrder numRows,
                                           THistogramColumn numColumns,
                                           PRV_UINT16 currentStat,
                                           vector<THistogramColumn> printedColumns,
                                           THistogramColumn iPlane,
                                           ofstream &outputFile,
                                           bool withLabels,
                                           ProgressController *progress )
{
  outputFile << endl;

  for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
  {
    if( withLabels )
      outputFile << whichHisto->getRowLabel( iRow ) << "\t";

    // progress advanced by rows (external loop)
    if( progress != NULL )
    {
      if ( numRows > 1 )
        progress->setCurrentProgress( (int)iRow );
    }

    for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
    {
      if ( !whichHisto->endCommCell( printedColumns[ iColumn ], iPlane ) )
      {
        if ( whichHisto->getCommCurrentRow( printedColumns[ iColumn ], iPlane ) == iRow )
        {
          outputFile << whichHisto->getCommCurrentValue( printedColumns[ iColumn ], currentStat, iPlane ) << "\t";
          whichHisto->setCommNextCell( printedColumns[ iColumn ], iPlane );
        }
        else
          outputFile << 0.0 << "\t";
      }
      else
        outputFile << 0.0 << "\t";
    }
    outputFile << endl;
  }
}


void TextOutput::dumpMatrixCommVertical( Histogram *whichHisto,
                                         TObjectOrder numRows,
                                         THistogramColumn numColumns,
                                         PRV_UINT16 currentStat,
                                         vector<THistogramColumn> printedColumns,
                                         THistogramColumn iPlane,
                                         ofstream &outputFile,
                                         bool withLabels,
                                         ProgressController *progress )
{
  if( withLabels )
  {
    for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
      outputFile << whichHisto->getRowLabel( iRow ) << "\t";
  }

  outputFile << endl;
  for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
  {
    if( withLabels )
      outputFile << whichHisto->getColumnLabel( printedColumns[ iColumn ] ) << "\t";

    // progress advanced by columns (external loop)
    if( progress != NULL )
    {
      if ( numColumns > 1 )
        progress->setCurrentProgress( (int)iColumn );
    }

    for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
    {
      if ( !whichHisto->endCommCell( printedColumns[ iColumn ], iPlane ) )
      {
        if ( whichHisto->getCommCurrentRow( printedColumns[ iColumn ], iPlane ) == iRow )
        {
          outputFile << whichHisto->getCommCurrentValue( printedColumns[ iColumn ], currentStat, iPlane ) << "\t";
          whichHisto->setCommNextCell( printedColumns[ iColumn ], iPlane );
        }
        else
          outputFile << 0.0 << "\t";
      }
      else
        outputFile << 0.0 << "\t";
    }
    outputFile << endl;
  }
}


void TextOutput::dumpTotalColumns( HistogramTotals *totals,
                                   string totalName,
                                   THistogramTotalsMethod totalFunction,
                                   PRV_UINT16 currentStat,
                                   vector<THistogramColumn> printedColumns,
                                   THistogramColumn iPlane,
                                   ofstream &outputFile,
                                   ProgressController *progress )
{
  THistogramColumn numColumns = (THistogramColumn)printedColumns.size();

  outputFile << totalName << "\t";
  for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
  {
    // progress advanced by columns (external loop)
    if( progress != NULL )
    {
      if ( numColumns > 1 )
        progress->setCurrentProgress( (int)iColumn );
    }

    outputFile << (totals->*totalFunction)( currentStat, printedColumns[ iColumn ], iPlane ) << "\t";
  }

  outputFile << endl;
}


void TextOutput::dumpTotalRows( HistogramTotals *totals,
                                string totalName,
                                THistogramTotalsMethod totalFunction,
                                PRV_UINT16 currentStat,
                                TObjectOrder numRows,
                                THistogramColumn iPlane,
                                ofstream &outputFile,
                                ProgressController *progress )
{
  outputFile << totalName << "\t";
  for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
  {
    // progress advanced by rows (external loop)
    if( progress != NULL )
    {
      if ( numRows > 1 )
        progress->setCurrentProgress( (int)iRow );
    }

    outputFile << (totals->*totalFunction)( currentStat, iRow, iPlane ) << "\t";
  }

  outputFile << endl;
}


bool TextOutput::getMultipleFiles() const
{
  return multipleFiles;
}

void TextOutput::setMultipleFiles( bool newValue )
{
  multipleFiles = newValue;
}

bool TextOutput::getObjectHierarchy() const
{
  return objectHierarchy;
}

void TextOutput::setObjectHierarchy( bool newValue )
{
  objectHierarchy = newValue;
}

bool TextOutput::getWindowTimeUnits() const
{
  return windowTimeUnits;
}

void TextOutput::setWindowTimeUnits( bool newValue )
{
  windowTimeUnits = newValue;
}

bool TextOutput::getTextualSemantic() const
{
  return textualSemantic;
}

void TextOutput::setTextualSemantic( bool newValue )
{
  textualSemantic = newValue;
}

TTime TextOutput::getMinTime() const
{
  return minTime;
}

TTime TextOutput::getMaxTime() const
{
  return maxTime;
}

