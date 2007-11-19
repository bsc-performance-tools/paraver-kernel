#include "histogram.h"
#include "histogramstatistic.h"
#include "histogramexception.h"


Histogram::Histogram()
{
  controlWindow = NULL;
  dataWindow = NULL;
  xtraControlWindow = NULL;

  controlMin = 0;
  controlMax = 1;
  controlDelta = 1;
  xtraControlMin = 0;
  xtraControlMax = 1;
  xtraControlDelta = 1;
  dataMin = 0;
  dataMax = 1;

  numStatistics = 0;

  rowsTranslator = NULL;
  columnTranslator = NULL;
  planeTranslator = NULL;

  cube = NULL;
  matrix = NULL;
}


Histogram::~Histogram()
{
  clearStatistics();
}


void Histogram::setControlWindow( KWindow *whichWindow )
{
  controlWindow = whichWindow;
}


void Histogram::setDataWindow( KWindow *whichWindow )
{
  dataWindow = whichWindow;
}


void Histogram::setExtraControlWindow( KWindow *whichWindow )
{
  xtraControlWindow = whichWindow;
}


void Histogram::clearControlWindow()
{
  controlWindow = NULL;
}


void Histogram::clearDataWindow()
{
  dataWindow = NULL;
}


void Histogram::clearExtraControlWindow()
{
  xtraControlWindow = NULL;
}


void Histogram::setControlMin( THistogramLimit whichMin )
{
  controlMin = whichMin;
}


void Histogram::setControlMax( THistogramLimit whichMax )
{
  controlMax = whichMax;
}


void Histogram::setControlDelta( THistogramLimit whichDelta )
{
  controlDelta = whichDelta;
}


void Histogram::setExtraControlMin( THistogramLimit whichMin )
{
  xtraControlMin = whichMin;
}


void Histogram::setExtraControlMax( THistogramLimit whichMax )
{
  xtraControlMax = whichMax;
}


void Histogram::setExtraControlDelta( THistogramLimit whichDelta )
{
  xtraControlDelta = whichDelta;
}


void Histogram::setDataMin( THistogramLimit whichMin )
{
  dataMin = whichMin;
}


void Histogram::setDataMax( THistogramLimit whichMax )
{
  dataMax = whichMax;
}


THistogramLimit Histogram::getControlMin()
{
  return controlMin;
}


THistogramLimit Histogram::getControlMax()
{
  return controlMax;
}


THistogramLimit Histogram::getControlDelta()
{
  return controlDelta;
}


THistogramLimit Histogram::getExtraControlMin()
{
  return xtraControlMin;
}


THistogramLimit Histogram::getExtraControlMax()
{
  return xtraControlMax;
}


THistogramLimit Histogram::getExtraControlDelta()
{
  return xtraControlDelta;
}


THistogramLimit Histogram::getDataMin()
{
  return dataMin;
}


THistogramLimit Histogram::getDataMax()
{
  return dataMax;
}


void Histogram::clearStatistics()
{
  vector<HistogramStatistic *>::iterator it = statisticFunctions.begin();

  while ( it != statisticFunctions.end() )
    delete *it;

  statisticFunctions.clear();
  numStatistics = 0;
}


void Histogram::pushbackStatistic( HistogramStatistic *whichStatistic )
{
  statisticFunctions.push_back( whichStatistic );
  numStatistics++;
}


void Histogram::execute( TRecordTime beginTime, TRecordTime endTime )
{
  TObjectOrder numRows;
  THistogramColumn numCols;
  THistogramColumn numPlanes;
  bool threeDimensions;

  if ( controlWindow == NULL )
    throw HistogramException( HistogramException::noControlWindow );

  if ( dataWindow == NULL )
    dataWindow = controlWindow;

  threeDimensions = ( xtraControlWindow != NULL );

  orderWindows();

  if ( rowsTranslator != NULL )
    delete rowsTranslator;
  rowsTranslator = new RowsTranslator( orderedWindows );

  if ( columnTranslator != NULL )
    delete columnTranslator;
  columnTranslator = new ColumnTranslator( controlMin, controlMax, controlDelta );

  if ( planeTranslator != NULL )
    delete planeTranslator;
  if( threeDimensions )
    planeTranslator = new ColumnTranslator( xtraControlMin, xtraControlMax, xtraControlDelta );

  numRows = rowsTranslator->totalRows();
  numCols = columnTranslator->totalColumns();
  if( threeDimensions )
    numPlanes = planeTranslator->totalColumns();

  if( cube != NULL )
    delete cube;
  if( matrix != NULL )
    delete matrix;

  if( threeDimensions )
    cube = new Cube<TSemanticValue>( numPlanes, numCols, numStatistics );
  else
    matrix = new Matrix<TSemanticValue>( numCols, numStatistics );
  // - Prepara la matriz o el cubo.
  // - Inicializa el semantico de las ventanas.
  // - Lanza la ejecucion recursiva (calculo parcial de totales?).
  // - Calculo de totales.
  // - Se ordenan las columnas si es necesario.
}


void Histogram::orderWindows()
{
  orderedWindows.clear();


}
