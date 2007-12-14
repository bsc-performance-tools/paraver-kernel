#include "histogram.h"
#include "histogramstatistic.h"
#include "histogramexception.h"


RowsTranslator::RowsTranslator( vector<KWindow *>& kwindows )
{
  for ( size_t ii = 0; ii < kwindows.size() - 1; ii++ )
  {
    childInfo.push_back( RowChildInfo() );
    childInfo[ii].oneToOne = ( kwindows[ii]->getWindowLevelObjects() ==
                               kwindows[ii + 1]->getWindowLevelObjects() );
    if ( !childInfo[ii].oneToOne )
    {
      Trace *auxTrace = kwindows[ii]->getTrace();
      for ( TObjectOrder iRow = 0; iRow < kwindows[ii]->getWindowLevelObjects(); iRow++ )
      {
        pair< TObjectOrder, TObjectOrder > range;

        range.first = auxTrace->getFirst( iRow,
                                          kwindows[ii]->getLevel(),
                                          kwindows[ii+1]->getLevel() );
        range.second =  auxTrace->getLast( iRow,
                                           kwindows[ii]->getLevel(),
                                           kwindows[ii+1]->getLevel() );
        ;
        childInfo[ii].rowChilds.push_back( range );
      }
    }
  }
}


RowsTranslator::~RowsTranslator()
{}


TObjectOrder RowsTranslator::globalTranslate( UINT16 winIndex,
    TObjectOrder rowIndex ) const
{
  // This method will translate Kwindow rows to 2D rows.
  return rowIndex;
}


void RowsTranslator::getRowChilds( UINT16 winIndex,
                                   TObjectOrder rowIndex,
                                   TObjectOrder& iniRow,
                                   TObjectOrder& endRow ) const
{
  if ( childInfo[winIndex].oneToOne )
  {
    iniRow = rowIndex;
    endRow = rowIndex;
  }
  else
  {
    iniRow = childInfo[ winIndex ].rowChilds[ rowIndex ].first;
    endRow = childInfo[ winIndex ].rowChilds[ rowIndex ].second;
  }
}

//
TObjectOrder RowsTranslator::totalRows() const
{
  return childInfo[ 0 ].rowChilds.size() - 1;
}


ColumnTranslator::ColumnTranslator( THistogramLimit whichMin,
                                    THistogramLimit whichMax,
                                    THistogramLimit whichDelta )
{}


ColumnTranslator::~ColumnTranslator()
{}


bool ColumnTranslator::getColumn( THistogramLimit whichValue,
                                  THistogramColumn& column ) const
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


KWindow *Histogram::getControlWindow() const
{
  return controlWindow;
}


KWindow *Histogram::getDataWindow() const
{
  return dataWindow;
}


KWindow *Histogram::getExtraControlWindow() const
{
  return xtraControlWindow;
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


THistogramColumn Histogram::getNumPlanes() const
{
  if ( threeDimensions )
    return planeTranslator->totalColumns();
  return 1;
}


THistogramColumn Histogram::getNumColumns() const
{
  return numCols;
}


TObjectOrder Histogram::getNumRows() const
{
  return numRows;
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

  it = commStatisticFunctions.begin();

  while ( it != commStatisticFunctions.end() )
  {
    delete *it;
    it++;
  }
}


void Histogram::pushbackStatistic( HistogramStatistic *whichStatistic )
{
  if ( whichStatistic->createComms() )
    commStatisticFunctions.push_back( whichStatistic );
  else
    statisticFunctions.push_back( whichStatistic );
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
  return commStatisticFunctions.size() > 0;
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
    cube = new Cube<TSemanticValue>( numPlanes, numCols, statisticFunctions.size() );
    if ( createComms() )
      commCube = new Cube<TSemanticValue>( numPlanes, numRows, commStatisticFunctions.size() );
  }
  else
  {
    matrix = new Matrix<TSemanticValue>( numCols, statisticFunctions.size() );
    if ( createComms() )
      commMatrix = new Matrix<TSemanticValue>( numRows, commStatisticFunctions.size() );
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

  it = commStatisticFunctions.begin();

  while ( it != commStatisticFunctions.end() )
  {
    ( *it )->init( this );
  }
}


void Histogram::recursiveExecution( TRecordTime fromTime, TRecordTime toTime,
                                    TObjectOrder fromRow, TObjectOrder toRow,
                                    UINT16 winIndex, CalculateData *data )
{
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

    while ( currentWindow->getEndTime( iRow ) <= fromTime )
      currentWindow->calcNext( iRow );

    while ( currentWindow->getEndTime( iRow ) < toTime )
    {
      calculate( iRow, fromTime, toTime, fromRow, toRow, winIndex, data );
      currentWindow->calcNext( iRow );
    }

    if ( currentWindow->getBeginTime( iRow ) < toTime )
      calculate( iRow, fromTime, toTime, fromRow, toRow, winIndex, data );

    if ( winIndex == 0 )
      finishRow( data );
  }

  delete data;
}


void Histogram::calculate( TObjectOrder iRow,
                           TRecordTime fromTime, TRecordTime toTime,
                           TObjectOrder fromRow, TObjectOrder toRow,
                           UINT16 winIndex, CalculateData *data )
{
  TObjectOrder childFromRow;
  TObjectOrder childToRow;
  TRecordTime childFromTime;
  TRecordTime childToTime;
  KWindow *currentWindow = orderedWindows[ winIndex ];

  if ( currentWindow == controlWindow )
  {
    if ( !columnTranslator->getColumn( controlWindow->getValue( iRow ),
                                       data->column ) )
      return;
    data->rList = controlWindow->getRecordList( iRow );
  }
  if ( threeDimensions && currentWindow == xtraControlWindow )
    if ( !planeTranslator->getColumn( xtraControlWindow->getValue( iRow ),
                                      data->plane ) )
      return;

  if ( winIndex == orderedWindows.size() - 1 )
  {
    TSemanticValue value;

    data->beginTime = fromTime;
    data->endTime = toTime;

    // Communication statistics
    RecordList::iterator itComm = data->rList->begin();
    while ( itComm != data->rList->end() &&
            ( *itComm )->getTime() >= fromTime &&
            ( *itComm )->getTime() <= toTime )
    {
      data->comm = *itComm;
      for ( UINT16 iStat = 0; iStat < commStatisticFunctions.size(); iStat++ )
      {
        value = commStatisticFunctions[ iStat ]->execute( data );
        if ( value != 0.0 )
        {
          TObjectOrder column = commStatisticFunctions[ iStat ]->getPartner( data );
          if ( threeDimensions )
            commCube->addValue( data->plane, column, iStat, value );
          else
            commMatrix->addValue( column, iStat, value );
        }
      }

      delete *itComm;
      itComm++;
    }

    // Semantic statistics
    for ( UINT16 iStat = 0; iStat < statisticFunctions.size(); iStat++ )
    {
      value = statisticFunctions[ iStat ]->execute( data );

      if ( threeDimensions )
        cube->addValue( data->plane, data->column, iStat, value );
      else
        matrix->addValue( data->column, iStat, value );
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
}


void Histogram::finishRow( CalculateData *data )
{
  // Communication statistics
  if ( threeDimensions )
  {
    for ( THistogramColumn iPlane = 0; iPlane < planeTranslator->totalColumns();
          iPlane++ )
    {
      if ( commCube->planeWithValues( iPlane ) )
      {
        for ( TObjectOrder iColumn = 0;
              iColumn < rowsTranslator->totalRows(); iColumn++ )
        {
          if ( commCube->currentCellModified( iPlane, iColumn ) )
          {
            TSemanticValue value;
            for ( UINT16 iStat = 0; iStat < commStatisticFunctions.size(); iStat++ )
            {
              value = commCube->getCurrentValue( iPlane, iColumn, iStat );
              value = commStatisticFunctions[ iStat ]->finishRow( value, iColumn, iPlane );
              commCube->setValue( iPlane, iColumn, iStat, value );
            }
          }
        }
      }
    }
  }
  else
  {
    for ( TObjectOrder iColumn = 0;
          iColumn < rowsTranslator->totalRows(); iColumn++ )
    {
      if ( commMatrix->currentCellModified( iColumn ) )
      {
        TSemanticValue value;
        for ( UINT16 iStat = 0; iStat < commStatisticFunctions.size(); iStat++ )
        {
          value = commMatrix->getCurrentValue( iColumn, iStat );
          value = commStatisticFunctions[ iStat ]->finishRow( value, iColumn );
          commMatrix->setValue( iColumn, iStat, value );
        }
      }
    }
  }

  for ( UINT16 iStat = 0; iStat < commStatisticFunctions.size(); iStat++ )
    commStatisticFunctions[ iStat ]->reset();

  // Semantic statistics
  if ( threeDimensions )
  {
    for ( THistogramColumn iPlane = 0; iPlane < planeTranslator->totalColumns();
          iPlane++ )
    {
      if ( cube->planeWithValues( iPlane ) )
      {
        for ( THistogramColumn iColumn = 0;
              iColumn < columnTranslator->totalColumns(); iColumn++ )
        {
          if ( cube->currentCellModified( iPlane, iColumn ) )
          {
            TSemanticValue value;
            for ( UINT16 iStat = 0; iStat < statisticFunctions.size(); iStat++ )
            {
              value = cube->getCurrentValue( iPlane, iColumn, iStat );
              value = statisticFunctions[ iStat ]->finishRow( value, iColumn, iPlane );
              cube->setValue( iPlane, iColumn, iStat, value );
            }
          }
        }
      }
    }
  }
  else
  {
    for ( THistogramColumn iColumn = 0;
          iColumn < columnTranslator->totalColumns(); iColumn++ )
    {
      if ( matrix->currentCellModified( iColumn ) )
      {
        TSemanticValue value;
        for ( UINT16 iStat = 0; iStat < statisticFunctions.size(); iStat++ )
        {
          value = matrix->getCurrentValue( iColumn, iStat );
          value = statisticFunctions[ iStat ]->finishRow( value, iColumn );
          matrix->setValue( iColumn, iStat, value );
        }
      }
    }
  }

  for ( UINT16 iStat = 0; iStat < statisticFunctions.size(); iStat++ )
    statisticFunctions[ iStat ]->reset();

  // Next row
  if ( createComms() )
    if ( threeDimensions )
      commCube->newRow();
    else
      commMatrix->newRow();

  if ( threeDimensions )
    cube->newRow();
  else
    matrix->newRow();
}
