#include "histogram.h"
#include "histogramstatistic.h"
#include "histogramexception.h"

RowsTranslator::RowsTranslator( vector<KWindow *>& windows )
{
}


RowsTranslator::~RowsTranslator()
{
}


TObjectOrder RowsTranslator::globalTranslate( UINT16 winIndex,
                                              TObjectOrder rowIndex ) const
{
  TObjectOrder rt;

  return rt;
}


void RowsTranslator::getRowChilds( UINT16 winIndex,
                                   TObjectOrder rowIndex,
                                   TObjectOrder& iniRow,
                                   TObjectOrder& endRow ) const
{
}


ColumnTranslator::ColumnTranslator( THistogramLimit whichMin,
                                    THistogramLimit whichMax,
                                    THistogramLimit whichDelta )
{
}


ColumnTranslator::~ColumnTranslator()
{
}


THistogramColumn ColumnTranslator::getColumn( THistogramLimit whichValue ) const
{
  THistogramColumn hc;

  return hc;
}


THistogramColumn ColumnTranslator::totalColumns() const
{
  THistogramColumn hc;

  return hc;
}


Histogram::Histogram()
{
  controlWindow = NULL;
  dataWindow = NULL;
  xtraControlWindow = NULL;

  threeDimensions = false;

  controlMin = 0;
  controlMax = 1;
  controlDelta = 1;
  xtraControlMin = 0;
  xtraControlMax = 1;
  xtraControlDelta = 1;
  dataMin = 0;
  dataMax = 1;

  numStatistics = 0;
  numCommStatistics = 0;

  rowsTranslator = NULL;
  columnTranslator = NULL;
  planeTranslator = NULL;

  cube = NULL;
  matrix = NULL;
  commCube = NULL;
  commMatrix = NULL;
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


THistogramLimit Histogram::getControlMin() const
{
  return controlMin;
}


THistogramLimit Histogram::getControlMax() const
{
  return controlMax;
}


THistogramLimit Histogram::getControlDelta() const
{
  return controlDelta;
}


THistogramLimit Histogram::getExtraControlMin() const
{
  return xtraControlMin;
}


THistogramLimit Histogram::getExtraControlMax() const
{
  return xtraControlMax;
}


THistogramLimit Histogram::getExtraControlDelta() const
{
  return xtraControlDelta;
}


THistogramLimit Histogram::getDataMin() const
{
  return dataMin;
}


THistogramLimit Histogram::getDataMax() const
{
  return dataMax;
}


void Histogram::clearStatistics()
{
  vector<HistogramStatistic *>::iterator it = statisticFunctions.begin();

  while ( it != statisticFunctions.end() )
  {
    delete *it;
    it++;
  }

  statisticFunctions.clear();
  numStatistics = 0;
  numCommStatistics = 0;
}


void Histogram::pushbackStatistic( HistogramStatistic *whichStatistic )
{
  statisticFunctions.push_back( whichStatistic );
  if ( whichStatistic->createComms() )
    numCommStatistics++;
  else
    numStatistics++;
}


void Histogram::execute( TRecordTime beginTime, TRecordTime endTime )
{
  TObjectOrder numRows;
  THistogramColumn numCols;
  THistogramColumn numPlanes;

  if ( controlWindow == NULL )
    throw HistogramException( HistogramException::noControlWindow );

  if ( dataWindow == NULL )
    dataWindow = controlWindow;

  threeDimensions = ( xtraControlWindow != NULL );

  orderWindows();

  initTranslators();

  numRows = rowsTranslator->totalRows();
  numCols = columnTranslator->totalColumns();
  if ( threeDimensions )
    numPlanes = planeTranslator->totalColumns();

  initMatrix( numPlanes, numCols, numRows );

  initSemantic( beginTime );

  // - Lanza la ejecucion recursiva (calculo parcial de totales?).
  // - Calculo de totales.
  // - Se ordenan las columnas si es necesario.
}


void Histogram::orderWindows()
{
  orderedWindows.clear();


}

bool Histogram::createComms() const
{
  return numCommStatistics > 0;
}


void Histogram::initTranslators()
{
  if ( rowsTranslator != NULL )
    delete rowsTranslator;
  rowsTranslator = new RowsTranslator( orderedWindows );

  if ( columnTranslator != NULL )
    delete columnTranslator;
  columnTranslator = new ColumnTranslator( controlMin, controlMax, controlDelta );

  if ( planeTranslator != NULL )
  {
    delete planeTranslator;
    planeTranslator = NULL;
  }
  if ( threeDimensions )
    planeTranslator = new ColumnTranslator( xtraControlMin, xtraControlMax, xtraControlDelta );
}


void Histogram::initMatrix( THistogramColumn numPlanes, THistogramColumn numCols,
                            TObjectOrder numRows )
{
  if ( cube != NULL )
  {
    delete cube;
    cube = NULL;
  }
  if ( matrix != NULL )
  {
    delete matrix;
    matrix = NULL;
  }
  if ( commCube != NULL )
  {
    delete commCube;
    commCube = NULL;
  }
  if ( commMatrix != NULL )
  {
    delete commMatrix;
    commMatrix = NULL;
  }

  if ( threeDimensions )
  {
    cube = new Cube<TSemanticValue>( numPlanes, numCols, numStatistics );
    if ( createComms() )
      commCube = new Cube<TSemanticValue>( numPlanes, numRows, numCommStatistics );
  }
  else
  {
    matrix = new Matrix<TSemanticValue>( numCols, numStatistics );
    if ( createComms() )
      commMatrix = new Matrix<TSemanticValue>( numRows, numCommStatistics );
  }
}


void Histogram::initSemantic( TRecordTime beginTime )
{
  TCreateList create = NOCREATE;

  if ( createComms() )
    create = CREATECOMMS;

  controlWindow->init( beginTime, create );

  if ( xtraControlWindow != controlWindow )
    xtraControlWindow->init( beginTime, NOCREATE );

  if ( dataWindow != controlWindow && dataWindow != xtraControlWindow )
    dataWindow->init( beginTime, NOCREATE );
}
