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
  if ( rowsTranslator != NULL )
    delete rowsTranslator;
  if ( columnTranslator != NULL )
    delete columnTranslator;
  if ( planeTranslator != NULL )
    delete planeTranslator;

  if ( cube != NULL )
    delete cube;
  if ( matrix != NULL )
    delete matrix;
  if ( commCube != NULL )
    delete commCube;
  if ( commMatrix != NULL )
    delete commMatrix;

  clearStatistics();
}


bool Histogram::getThreeDimensions() const
{
  return threeDimensions;
}


TRecordTime Histogram::getBeginTime() const
{
  return beginTime;
}


TRecordTime Histogram::getEndTime() const
{
  return endTime;
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


void Histogram::execute( TRecordTime whichBeginTime, TRecordTime whichEndTime )
{
  if ( controlWindow == NULL )
    throw HistogramException( HistogramException::noControlWindow );

  if ( dataWindow == NULL )
    dataWindow = controlWindow;

  beginTime = whichBeginTime;
  endTime = whichEndTime;

  threeDimensions = ( xtraControlWindow != NULL );

  orderWindows();

  initTranslators();

  numRows = rowsTranslator->totalRows();
  numCols = columnTranslator->totalColumns();
  if ( threeDimensions )
    numPlanes = planeTranslator->totalColumns();

  initMatrix( numPlanes, numCols, numRows );

  initSemantic( beginTime );

  initStatistics();

  // - Los totales podrian ser una clase aparte.
  // - Lanza la ejecucion recursiva (calculo parcial de totales?).
  recursiveExecution( beginTime, endTime, 0, rowsTranslator->totalRows() );
  if ( threeDimensions )
  {
    cube->finish();
    if ( createComms() )
      commCube->finish();
  }
  else
  {
    matrix->finish();
    if ( createComms() )
      commMatrix->finish();
  }
  // - Finalizacion del calculo y de los totales.
  // - Se ordenan las columnas si es necesario.
}


void Histogram::orderWindows()
{
  orderedWindows.clear();

  if ( controlWindow->getLevel() >= xtraControlWindow->getLevel() )
  {
    orderedWindows.push_back( controlWindow );
    orderedWindows.push_back( xtraControlWindow );
  }
  else
  {
    orderedWindows.push_back( xtraControlWindow );
    orderedWindows.push_back( controlWindow );
  }

  orderedWindows.push_back( dataWindow );
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
    planeTranslator = new ColumnTranslator( xtraControlMin, xtraControlMax,
                                            xtraControlDelta );
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


void Histogram::initStatistics()
{
  vector<HistogramStatistic *>::iterator it = statisticFunctions.begin();

  while ( it != statisticFunctions.end() )
  {
    ( *it )->init( this );
  }
}


void Histogram::recursiveExecution( TRecordTime fromTime, TRecordTime toTime,
                                    TObjectOrder fromRow, TObjectOrder toRow,
                                    UINT16 winIndex, CalculateData *data )
{
  TObjectOrder childFromRow;
  TObjectOrder childToRow;
  TRecordTime childFromTime;
  TRecordTime childToTime;
  KWindow *currentWindow = orderedWindows[ winIndex ];

  if ( data == NULL )
    data = new CalculateData;

  for ( TObjectOrder iRow = fromRow; iRow <= toRow; iRow++ )
  {
    if ( winIndex == 0 )
      data->row = rowsTranslator->globalTranslate( winIndex, iRow );
    if ( currentWindow == controlWindow )
      data->controlRow = iRow;
    if ( currentWindow == dataWindow )
      data->dataRow = iRow;

    while ( currentWindow->getEndTime( iRow ) <= toTime )
    {
      if ( currentWindow == controlWindow )
        data->column = columnTranslator->getColumn( controlWindow->getValue( iRow ) );
      if ( threeDimensions && currentWindow == xtraControlWindow )
        data->plane = planeTranslator->getColumn( xtraControlWindow->getValue( iRow ) );

      if ( winIndex == orderedWindows.size() - 1 )
      {
        TSemanticValue value;

        for ( UINT16 iStat = 0; iStat < statisticFunctions.size(); iStat++ )
        {
          value = statisticFunctions[ iStat ]->execute( data );
          Cube<TSemanticValue> *myCube;
          Matrix<TSemanticValue> *myMatrix;
          if ( statisticFunctions[ iStat ]->createComms() )
          {
            myCube = commCube;
            myMatrix = commMatrix;
          }
          else
          {
            myCube = cube;
            myMatrix = matrix;
          }

          if ( threeDimensions )
          {
            if ( myCube->getCurrentRow( data->plane, data->column ) != data->row )
              myCube->newRow( data->plane, data->column, data->row );
            myCube->addValue( data->plane, data->column, iStat, value );
          }
          else
          {
            if ( myMatrix->getCurrentRow( data->column ) != data->row )
              myMatrix->newRow( data->column, data->row );
            myMatrix->addValue( data->column, iStat, value );
          }
        }
      }
      else
      {
        childFromTime = ( fromTime < currentWindow->getBeginTime( iRow ) ) ?
                        currentWindow->getBeginTime( iRow ) :
                        fromTime;
        childToTime = ( toTime > currentWindow->getEndTime( iRow ) ) ?
                      currentWindow->getEndTime( iRow ) :
                      toTime;
        rowsTranslator->getRowChilds( winIndex, iRow, childFromRow, childToRow );

        recursiveExecution( childFromTime, childToTime, childFromRow, childToRow,
                            winIndex + 1, data );
      }

      currentWindow->calcNext( iRow );
    }

    // Puede que quede un valor si el begintime de la ventana es menor (o igual?)
    // que el totime
  }

  delete data;
}
