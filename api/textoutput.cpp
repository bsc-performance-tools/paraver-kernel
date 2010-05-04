#include "textoutput.h"
#include "window.h"
#include "histogram.h"

TextOutput::TextOutput()
    : multipleFiles( false )
{}

TextOutput::~TextOutput()
{}

void TextOutput::dumpWindow( Window *whichWindow, string& strOutputFile )
{
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

  for ( TObjectOrder i = 0; i < whichWindow->getWindowLevelObjects(); ++i )
  {
    if ( multipleFiles )
    {
      ostringstream tmpName;
      tmpName << strOutputFile  << "_" << setw( 5 ) << setfill( '0' ) << i + 1;
      outputFile.open( tmpName.str().c_str() );
    }

    outputFile << fixed;
    outputFile << showpoint;

    whichWindow->initRow( i, beginTime, NOCREATE );
    while ( whichWindow->getEndTime( i ) < endTime )
    {
      outputFile << setprecision( config->getTimelinePrecision() );
      if ( !multipleFiles )
        outputFile << i + 1 << "\t";
      outputFile << whichWindow->traceUnitsToWindowUnits(
        whichWindow->getBeginTime( i ) ) << "\t";
      outputFile << whichWindow->traceUnitsToWindowUnits(
        whichWindow->getEndTime( i ) - whichWindow->getBeginTime( i ) ) << "\t";
      outputFile << whichWindow->getValue( i ) << endl;
      whichWindow->calcNext( i );
    }
    outputFile << setprecision( config->getTimelinePrecision() );
    if ( !multipleFiles )
      outputFile << i + 1 << "\t";
    outputFile << whichWindow->traceUnitsToWindowUnits(
      whichWindow->getBeginTime( i ) ) << "\t";
    outputFile << whichWindow->traceUnitsToWindowUnits(
      whichWindow->getEndTime( i ) - whichWindow->getBeginTime( i ) ) << "\t";
    outputFile << whichWindow->getValue( i ) << endl;

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

  if ( !multipleFiles )
    outputFile.close();
}

void TextOutput::dumpHistogram( Histogram *whichHisto, string& strOutputFile )
{
  THistogramColumn numPlanes;
  THistogramColumn numColumns;
  TObjectOrder numRows;
  UINT16 currentStat;
  ofstream outputFile;

  whichHisto->getIdStat( whichHisto->getCurrentStat(), currentStat );

  vector<TObjectOrder> selectedRows;
  TObjectOrder beginRow = whichHisto->getControlWindow()->getZoomSecondDimension().first;
  TObjectOrder endRow =  whichHisto->getControlWindow()->getZoomSecondDimension().second;
  whichHisto->getControlWindow()->getSelectedRows( whichHisto->getControlWindow()->getLevel(),
      selectedRows, beginRow, endRow );
  whichHisto->execute( whichHisto->getBeginTime(), whichHisto->getEndTime(), selectedRows );

  numPlanes = whichHisto->getNumPlanes();
  numColumns = whichHisto->getNumColumns();
  numRows = whichHisto->getNumRows();

  outputFile.open( strOutputFile.c_str() );

  outputFile << fixed;
  outputFile << showpoint;

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; ++iPlane )
  {
    if ( numPlanes > 1 )
      outputFile << whichHisto->getPlaneLabel( iPlane ) << endl;

    outputFile << "\t";
    // Initialize all columns in this plane
    for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
    {
      whichHisto->setFirstCell( iColumn, iPlane );
      if ( whichHisto->getHorizontal() )
        outputFile << whichHisto->getColumnLabel( iColumn ) << "\t";
    }
    if ( whichHisto->getHorizontal() )
      outputFile << endl;

    if ( whichHisto->getHorizontal() )
    {
      for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
      {
        outputFile << whichHisto->getRowLabel( iRow ) << "\t";
        for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
        {
          if ( !whichHisto->endCell( iColumn ) )
          {
            if ( whichHisto->getCurrentRow( iColumn, iPlane ) == iRow )
            {
              outputFile << whichHisto->getCurrentValue( iColumn, currentStat, iPlane ) << "\t";
              whichHisto->setNextCell( iColumn, iPlane );
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
    else
    {
      for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
        outputFile << whichHisto->getRowLabel( iRow ) << "\t";

      outputFile << endl;
      for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
      {
        outputFile << whichHisto->getColumnLabel( iColumn ) << "\t";
        for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
        {
          if ( !whichHisto->endCell( iColumn ) )
          {
            if ( whichHisto->getCurrentRow( iColumn, iPlane ) == iRow )
            {
              outputFile << whichHisto->getCurrentValue( iColumn, currentStat, iPlane ) << "\t";
              whichHisto->setNextCell( iColumn, iPlane );
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
    outputFile << endl;
    // Print totals
    HistogramTotals *totals;
    if ( whichHisto->getHorizontal() )
    {
      totals = whichHisto->getColumnTotals();
      outputFile << "Total" << "\t";
      for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
        outputFile << totals->getTotal( currentStat, iColumn, iPlane ) << "\t";
      outputFile << endl;

      outputFile << "Average" << "\t";
      for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
        outputFile << totals->getAverage( currentStat, iColumn, iPlane ) << "\t";
      outputFile << endl;

      outputFile << "Maximum" << "\t";
      for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
        outputFile << totals->getMaximum( currentStat, iColumn, iPlane ) << "\t";
      outputFile << endl;

      outputFile << "Minimum" << "\t";
      for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
        outputFile << totals->getMinimum( currentStat, iColumn, iPlane ) << "\t";
      outputFile << endl;

      outputFile << "Stdev" << "\t";
      for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
        outputFile << totals->getStdev( currentStat, iColumn, iPlane ) << "\t";
      outputFile << endl;

      outputFile << "Avg/Max" << "\t";
      for ( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
        outputFile << totals->getAvgDivMax( currentStat, iColumn, iPlane ) << "\t";
      outputFile << endl;
    }
    else
    {
      totals = whichHisto->getRowTotals();
      outputFile << "Total" << "\t";
      for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
        outputFile << totals->getTotal( currentStat, iRow, iPlane ) << "\t";
      outputFile << endl;

      outputFile << "Average" << "\t";
      for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
        outputFile << totals->getAverage( currentStat, iRow, iPlane ) << "\t";
      outputFile << endl;

      outputFile << "Maximum" << "\t";
      for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
        outputFile << totals->getMaximum( currentStat, iRow, iPlane ) << "\t";
      outputFile << endl;

      outputFile << "Minimum" << "\t";
      for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
        outputFile << totals->getMinimum( currentStat, iRow, iPlane ) << "\t";
      outputFile << endl;

      outputFile << "Stdev" << "\t";
      for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
        outputFile << totals->getStdev( currentStat, iRow, iPlane ) << "\t";
      outputFile << endl;

      outputFile << "Avg/Max" << "\t";
      for ( TObjectOrder iRow = 0; iRow < numRows; ++iRow )
        outputFile << totals->getAvgDivMax( currentStat, iRow, iPlane ) << "\t";
      outputFile << endl;
    }


    delete totals;
    outputFile << endl;
  }
  outputFile.close();
}

bool TextOutput::getMultipleFiles() const
{
  return multipleFiles;
}

void TextOutput::setMultipleFiles( bool newValue )
{
  multipleFiles = newValue;
}
