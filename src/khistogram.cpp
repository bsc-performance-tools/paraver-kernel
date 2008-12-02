#include <math.h>
#include <limits>
#include "kwindow.h"
#include "khistogram.h"
#include "histogramstatistic.h"
#include "histogramexception.h"
#include "khistogramtotals.h"
#include "functionmanagement.h"

RowsTranslator::RowsTranslator( vector<KWindow *>& kwindows )
{
  for ( size_t ii = 0; ii < kwindows.size() - 1; ii++ )
  {
    childInfo.push_back( RowChildInfo() );
    childInfo[ii].oneToOne = ( kwindows[ii]->getWindowLevelObjects() ==
                               kwindows[ii + 1]->getWindowLevelObjects() );
    childInfo[ii].numRows = kwindows[ii]->getWindowLevelObjects();
    if ( !childInfo[ii].oneToOne )
    {
      KTrace *auxTrace = (KTrace*)kwindows[ii]->getTrace();
      for ( TObjectOrder iRow = 0; iRow < kwindows[ii]->getWindowLevelObjects(); iRow++ )
      {
        pair< TObjectOrder, TObjectOrder > range;

        range.first = auxTrace->getFirst( iRow,
                                          kwindows[ii]->getLevel(),
                                          kwindows[ii+1]->getLevel() );
        range.second =  auxTrace->getLast( iRow,
                                           kwindows[ii]->getLevel(),
                                           kwindows[ii+1]->getLevel() );
        childInfo[ii].rowChilds.push_back( range );
      }
    }
  }
}


RowsTranslator::~RowsTranslator()
{}


inline TObjectOrder RowsTranslator::globalTranslate( UINT16 winIndex,
    TObjectOrder rowIndex ) const
{
  // This method will translate Kwindow rows to 2D rows.
  return rowIndex;
}


inline void RowsTranslator::getRowChilds( UINT16 winIndex,
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
inline TObjectOrder RowsTranslator::totalRows() const
{
  return childInfo[ 0 ].numRows;
}


ColumnTranslator::ColumnTranslator( THistogramLimit whichMin,
                                    THistogramLimit whichMax,
                                    THistogramLimit whichDelta ):
    minLimit( whichMin ), maxLimit( whichMax ), delta( whichDelta )
{
  // PRECOND: Min < Max
  numColumns = THistogramColumn( ceil( ( maxLimit - minLimit ) / delta ) );

  if ( ( numColumns * delta ) + minLimit <= maxLimit )
    numColumns++;
}


ColumnTranslator::~ColumnTranslator()
{}

// returns whichValue in [min,max)
inline bool ColumnTranslator::getColumn( THistogramLimit whichValue,
                                  THistogramColumn& column ) const
{
  if ( whichValue < minLimit || whichValue > maxLimit )
    return false;

  column = THistogramColumn( floor( ( ( whichValue - minLimit ) * numColumns ) /
                                    ( maxLimit - minLimit ) ) );

  if ( column >= numColumns )
    column = numColumns - 1;

  return true;
}


inline THistogramColumn ColumnTranslator::totalColumns() const
{
  return numColumns;
}


KHistogram::KHistogram()
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
  dataMin = std::numeric_limits<TSemanticValue>::min();
  dataMax = std::numeric_limits<TSemanticValue>::max();
  burstMin = std::numeric_limits<TRecordTime>::min();
  burstMax = std::numeric_limits<TRecordTime>::max();
  commSizeMin = std::numeric_limits<TCommSize>::min();
  commSizeMax = std::numeric_limits<TCommSize>::max();
  commTagMin = std::numeric_limits<TCommTag>::min();
  commTagMax = std::numeric_limits<TCommTag>::max();

  inclusive = false;

  rowsTranslator = NULL;
  columnTranslator = NULL;
  planeTranslator = NULL;

  cube = NULL;
  matrix = NULL;
  commCube = NULL;
  commMatrix = NULL;

  totals = NULL;
  rowTotals = NULL;
  commTotals = NULL;
  rowCommTotals = NULL;
}


KHistogram::~KHistogram()
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

  if ( totals != NULL )
    delete totals;
  if ( rowTotals != NULL )
    delete rowTotals;
  if ( commTotals != NULL )
    delete commTotals;
  if ( rowCommTotals != NULL )
    delete rowCommTotals;

  clearStatistics();
}


inline bool KHistogram::getThreeDimensions() const
{
  return threeDimensions;
}


inline TRecordTime KHistogram::getBeginTime() const
{
  return beginTime;
}


inline TRecordTime KHistogram::getEndTime() const
{
  return endTime;
}


inline Window *KHistogram::getControlWindow() const
{
  return controlWindow;
}


inline Window *KHistogram::getDataWindow() const
{
  return dataWindow;
}


inline Window *KHistogram::getExtraControlWindow() const
{
  return xtraControlWindow;
}


inline void KHistogram::setControlWindow( Window *whichWindow )
{
  controlWindow = ( KWindow * ) whichWindow;
}


inline void KHistogram::setDataWindow( Window *whichWindow )
{
  dataWindow = ( KWindow * ) whichWindow;
}


inline void KHistogram::setExtraControlWindow( Window *whichWindow )
{
  xtraControlWindow = ( KWindow * ) whichWindow;
}


inline void KHistogram::clearControlWindow()
{
  controlWindow = NULL;
}


inline void KHistogram::clearDataWindow()
{
  dataWindow = NULL;
}


inline void KHistogram::clearExtraControlWindow()
{
  xtraControlWindow = NULL;
}


inline void KHistogram::setControlMin( THistogramLimit whichMin )
{
  controlMin = whichMin;
}


inline void KHistogram::setControlMax( THistogramLimit whichMax )
{
  controlMax = whichMax;
}


inline void KHistogram::setControlDelta( THistogramLimit whichDelta )
{
  controlDelta = whichDelta;
}


inline void KHistogram::setExtraControlMin( THistogramLimit whichMin )
{
  xtraControlMin = whichMin;
}


inline void KHistogram::setExtraControlMax( THistogramLimit whichMax )
{
  xtraControlMax = whichMax;
}


inline void KHistogram::setExtraControlDelta( THistogramLimit whichDelta )
{
  xtraControlDelta = whichDelta;
}


inline void KHistogram::setDataMin( TSemanticValue whichMin )
{
  dataMin = whichMin;
}


inline void KHistogram::setDataMax( TSemanticValue whichMax )
{
  dataMax = whichMax;
}


inline void KHistogram::setBurstMin( TRecordTime whichTime )
{
  burstMin = whichTime;
}


inline void KHistogram::setBurstMax( TRecordTime whichTime )
{
  burstMax = whichTime;
}


inline void KHistogram::setCommSizeMin( TCommSize whichSize )
{
  commSizeMin = whichSize;
}


inline void KHistogram::setCommSizeMax( TCommSize whichSize )
{
  commSizeMax = whichSize;
}


inline void KHistogram::setCommTagMin( TCommTag whichTag )
{
  commTagMin = whichTag;
}


inline void KHistogram::setCommTagMax( TCommTag whichTag )
{
  commTagMax = whichTag;
}


inline THistogramLimit KHistogram::getControlMin() const
{
  return controlMin;
}


inline THistogramLimit KHistogram::getControlMax() const
{
  return controlMax;
}


inline THistogramLimit KHistogram::getControlDelta() const
{
  return controlDelta;
}


inline THistogramLimit KHistogram::getExtraControlMin() const
{
  return xtraControlMin;
}


inline THistogramLimit KHistogram::getExtraControlMax() const
{
  return xtraControlMax;
}


inline THistogramLimit KHistogram::getExtraControlDelta() const
{
  return xtraControlDelta;
}


inline TSemanticValue KHistogram::getDataMin() const
{
  return dataMin;
}


inline TSemanticValue KHistogram::getDataMax() const
{
  return dataMax;
}


inline TRecordTime KHistogram::getBurstMin() const
{
  return burstMin;
}


inline TRecordTime KHistogram::getBurstMax() const
{
  return burstMax;
}


inline TCommSize KHistogram::getCommSizeMin() const
{
  return commSizeMin;
}


inline TCommSize KHistogram::getCommSizeMax() const
{
  return commSizeMax;
}


inline TCommTag KHistogram::getCommTagMin() const
{
  return commTagMin;
}


inline TCommTag KHistogram::getCommTagMax() const
{
  return commTagMax;
}


inline void KHistogram::setInclusive( bool newValue )
{
  if ( newValue )
  {
    if ( controlWindow->getFirstUsefulFunction()->getStack() == NULL )
      return;
  }
  inclusive = newValue;
}


inline THistogramColumn KHistogram::getNumPlanes() const
{
  if ( threeDimensions )
    return planeTranslator->totalColumns();
  return 1;
}


inline THistogramColumn KHistogram::getNumColumns() const
{
  return numCols;
}


inline TObjectOrder KHistogram::getNumRows() const
{
  return numRows;
}


inline TSemanticValue KHistogram::getCurrentValue( UINT32 col,
    UINT16 idStat,
    UINT32 plane ) const
{
  if ( threeDimensions )
    return cube->getCurrentValue( plane, col, idStat );
  else
    return matrix->getCurrentValue( col, idStat );

  return TSemanticValue( 0 );
}

inline UINT32 KHistogram::getCurrentRow( UINT32 col, UINT32 plane ) const
{
  if ( threeDimensions )
    return cube->getCurrentRow( plane, col );
  else
    return matrix->getCurrentRow( col );

  return 0;
}

inline void KHistogram::setNextCell( UINT32 col, UINT32 plane )
{
  if ( threeDimensions )
    cube->setNextCell( plane, col );
  else
    matrix->setNextCell( col );
}

inline void KHistogram::setFirstCell( UINT32 col, UINT32 plane )
{
  if ( threeDimensions )
    cube->setFirstCell( plane, col );
  else
    matrix->setFirstCell( col );
}

inline bool KHistogram::endCell( UINT32 col, UINT32 plane )
{
  if ( threeDimensions )
    return cube->endCell( plane, col );
  else
    return matrix->endCell( col );

  return true;
}

inline bool KHistogram::planeWithValues( UINT32 plane ) const
{
  if ( threeDimensions )
    return cube->planeWithValues( plane );

  return true;
}


inline TSemanticValue KHistogram::getCommCurrentValue( UINT32 col,
    UINT16 idStat,
    UINT32 plane ) const
{
  if ( threeDimensions )
    return commCube->getCurrentValue( plane, col, idStat );
  else
    return commMatrix->getCurrentValue( col, idStat );

  return TSemanticValue( 0 );
}

inline UINT32 KHistogram::getCommCurrentRow( UINT32 col, UINT32 plane ) const
{
  if ( threeDimensions )
    return commCube->getCurrentRow( plane, col );
  else
    return commMatrix->getCurrentRow( col );

  return 0;
}

inline void KHistogram::setCommNextCell( UINT32 col, UINT32 plane )
{
  if ( threeDimensions )
    commCube->setNextCell( plane, col );
  else
    commMatrix->setNextCell( col );
}

inline void KHistogram::setCommFirstCell( UINT32 col, UINT32 plane )
{
  if ( threeDimensions )
    commCube->setFirstCell( plane, col );
  else
    commMatrix->setFirstCell( col );
}

inline bool KHistogram::endCommCell( UINT32 col, UINT32 plane )
{
  if ( threeDimensions )
    return commCube->endCell( plane, col );
  else
    return commMatrix->endCell( col );

  return true;
}

inline bool KHistogram::planeCommWithValues( UINT32 plane ) const
{
  if ( threeDimensions )
    return commCube->planeWithValues( plane );

  return true;
}


inline HistogramTotals *KHistogram::getColumnTotals() const
{
  return totals;
}


inline HistogramTotals *KHistogram::getCommColumnTotals() const
{
  return commTotals;
}


inline HistogramTotals *KHistogram::getRowTotals() const
{
  return rowTotals;
}


inline HistogramTotals *KHistogram::getCommRowTotals() const
{
  return rowCommTotals;
}


inline void KHistogram::clearStatistics()
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
  commStatisticFunctions.clear();
}


inline void KHistogram::pushbackStatistic( const string& whichStatistic )
{
  HistogramStatistic *stat;

  stat = ( FunctionManagement<HistogramStatistic>::getInstance() )->getFunction( whichStatistic );

  if ( stat->createComms() )
    commStatisticFunctions.push_back( stat );
  else
    statisticFunctions.push_back( stat );
}


void KHistogram::execute( TRecordTime whichBeginTime, TRecordTime whichEndTime )
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

  initTotals();

  recursiveExecution( beginTime, endTime, 0, rowsTranslator->totalRows() - 1 );

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

  if ( totals != NULL )
    totals->finish();
  if ( rowTotals != NULL )
    rowTotals->finish();
  if ( commTotals != NULL )
    commTotals->finish();
  if ( rowCommTotals != NULL )
    rowCommTotals->finish();
  // - Columns will be ordered if necesary
}


void KHistogram::orderWindows()
{
  orderedWindows.clear();

  if ( getThreeDimensions() )
  {
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
  }
  else
    orderedWindows.push_back( controlWindow );

  orderedWindows.push_back( dataWindow );
}


bool KHistogram::createComms() const
{
  return commStatisticFunctions.size() > 0;
}


void KHistogram::initTranslators()
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


void KHistogram::initMatrix( THistogramColumn planes, THistogramColumn cols,
                             TObjectOrder rows )
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
    cube = new Cube<TSemanticValue>( planes, cols, statisticFunctions.size() );
    if ( createComms() )
      commCube = new Cube<TSemanticValue>( planes, rows, commStatisticFunctions.size() );
  }
  else
  {
    matrix = new Matrix<TSemanticValue>( cols, statisticFunctions.size() );
    if ( createComms() )
      commMatrix = new Matrix<TSemanticValue>( rows, commStatisticFunctions.size() );
  }
}


void KHistogram::initTotals()
{
  if ( totals != NULL )
    delete totals;
  if ( rowTotals != NULL )
    delete rowTotals;
  if ( commTotals != NULL )
    delete commTotals;
  if ( rowCommTotals != NULL )
    delete rowCommTotals;

  if ( threeDimensions )
  {
    totals = new KHistogramTotals( statisticFunctions.size(), numCols, numPlanes );
    rowTotals = new KHistogramTotals( statisticFunctions.size(), numRows, numPlanes );
    if ( createComms() )
    {
      commTotals = new KHistogramTotals( commStatisticFunctions.size(),
                                         numRows, numPlanes );
      rowCommTotals = new KHistogramTotals( commStatisticFunctions.size(),
                                            numRows, numPlanes );
    }
  }
  else
  {
    totals = new KHistogramTotals( statisticFunctions.size(), numCols, 1 );
    rowTotals = new KHistogramTotals( statisticFunctions.size(), numRows, 1 );
    if ( createComms() )
    {
      commTotals = new KHistogramTotals( commStatisticFunctions.size(),
                                         numRows, 1 );
      rowCommTotals = new KHistogramTotals( commStatisticFunctions.size(),
                                            numRows, 1 );
    }
  }
}


void KHistogram::initSemantic( TRecordTime beginTime )
{
  TCreateList create = NOCREATE;

  if ( createComms() )
    create = CREATECOMMS;

  controlWindow->init( beginTime, create );

  if ( xtraControlWindow != NULL && xtraControlWindow != controlWindow )
    xtraControlWindow->init( beginTime, NOCREATE );

  if ( dataWindow != controlWindow && dataWindow != xtraControlWindow )
    dataWindow->init( beginTime, NOCREATE );
}


void KHistogram::initStatistics()
{
  vector<HistogramStatistic *>::iterator it = statisticFunctions.begin();

  while ( it != statisticFunctions.end() )
  {
    ( *it )->init( this );
    it++;
  }

  it = commStatisticFunctions.begin();

  while ( it != commStatisticFunctions.end() )
  {
    ( *it )->init( this );
    it++;
  }
}


void KHistogram::recursiveExecution( TRecordTime fromTime, TRecordTime toTime,
                                     TObjectOrder fromRow, TObjectOrder toRow,
                                     UINT16 winIndex, CalculateData *data )
{
  Window *currentWindow = orderedWindows[ winIndex ];

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

  if ( winIndex == 0 )
  {
    delete data;
    data = NULL;
  }
}


void KHistogram::calculate( TObjectOrder iRow,
                            TRecordTime fromTime, TRecordTime toTime,
                            TObjectOrder fromRow, TObjectOrder toRow,
                            UINT16 winIndex, CalculateData *data )
{
  TObjectOrder childFromRow;
  TObjectOrder childToRow;
  TRecordTime childFromTime;
  TRecordTime childToTime;
  Window *currentWindow = orderedWindows[ winIndex ];

  if ( currentWindow == controlWindow )
  {
    if ( !columnTranslator->getColumn( controlWindow->getValue( iRow ),
                                       data->column ) )
      return;
    data->rList = controlWindow->getRecordList( iRow );
  }
  if ( threeDimensions && currentWindow == xtraControlWindow )
  {
    if ( !planeTranslator->getColumn( xtraControlWindow->getValue( iRow ),
                                      data->plane ) )
      return;
  }

  if ( winIndex == orderedWindows.size() - 1 )
  {
    TSemanticValue value;

    data->beginTime = fromTime;
    data->endTime = toTime;

    // Communication statistics
    RecordList::iterator itComm = data->rList->begin();
    while ( itComm != data->rList->end() &&
            itComm->getTime() >= fromTime &&
            itComm->getTime() <= toTime )
    {
      data->comm = itComm;
      for ( UINT16 iStat = 0; iStat < commStatisticFunctions.size(); iStat++ )
      {
        if ( commStatisticFunctions[ iStat ]->filter( data ) )
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
      }

      itComm++;
    }

    data->rList->erase( data->rList->begin(), itComm );

    // Semantic statistics
    if ( inclusive )
    {
      THistogramColumn column;
      vector<vector<TSemanticValue> > *tmp =
        controlWindow->getFirstUsefulFunction()->getStack();
      vector<TSemanticValue>::iterator it = ( *tmp )[ data->controlRow ].begin();
      while ( it != ( *tmp )[ data->controlRow ].end() )
      {
        if ( columnTranslator->getColumn( *it, column ) )
        {
          for ( UINT16 iStat = 0; iStat < statisticFunctions.size(); iStat++ )
          {
            if ( statisticFunctions[ iStat ]->filter( data ) )
            {
              value = statisticFunctions[ iStat ]->execute( data );

              if ( threeDimensions )
                cube->addValue( data->plane, column, iStat, value );
              else
                matrix->addValue( column, iStat, value );
            }
          }
        }
        it++;
      }
    }
    else
    {
      for ( UINT16 iStat = 0; iStat < statisticFunctions.size(); iStat++ )
      {
        if ( statisticFunctions[ iStat ]->filter( data ) )
        {
          value = statisticFunctions[ iStat ]->execute( data );

          if ( threeDimensions )
            cube->addValue( data->plane, data->column, iStat, value );
          else
            matrix->addValue( data->column, iStat, value );
        }
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
}


void KHistogram::finishRow( CalculateData *data )
{
  // Communication statistics
  if ( createComms() )
  {
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
                commTotals->newValue( value, iStat, iColumn, iPlane );
                rowCommTotals->newValue( value, iStat, data->row, iPlane );
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
            commTotals->newValue( value, iStat, iColumn );
            rowCommTotals->newValue( value, iStat, data->row );
          }
        }
      }
    }

    for ( UINT16 iStat = 0; iStat < commStatisticFunctions.size(); iStat++ )
      commStatisticFunctions[ iStat ]->reset();
  }
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
              totals->newValue( value, iStat, iColumn, iPlane );
              rowTotals->newValue( value, iStat, data->row, iPlane );
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
          totals->newValue( value, iStat, iColumn );
          rowTotals->newValue( value, iStat, data->row );
        }
      }
    }
  }

  for ( UINT16 iStat = 0; iStat < statisticFunctions.size(); iStat++ )
    statisticFunctions[ iStat ]->reset();

  // Next row
  if ( createComms() )
  {
    if ( threeDimensions )
      commCube->newRow();
    else
      commMatrix->newRow();
  }

  if ( threeDimensions )
    cube->newRow();
  else
    matrix->newRow();
}

bool KHistogram::itsCommunicationStat( const string& whichStat ) const
{
  return FunctionManagement<HistogramStatistic>::getInstance()
         ->getFunction( whichStat )->createComms();
}

string KHistogram::getUnitsLabel( const string& whichStat ) const
{
  return FunctionManagement<HistogramStatistic>::getInstance()
         ->getFunction( whichStat )->getUnits( this );
}

void KHistogram::getGroupsLabels( vector<string>& onVector ) const
{
  FunctionManagement<HistogramStatistic>::getInstance()->getNameGroups( onVector );
}

void KHistogram::getStatisticsLabels( vector<string>& onVector, UINT32 whichGroup ) const
{
  FunctionManagement<HistogramStatistic>::getInstance()->getAll( onVector, whichGroup );
}

string KHistogram::getFirstStatistic() const
{
  vector<string> v;
  FunctionManagement<HistogramStatistic>::getInstance()->getAll( v, 1 );
  return v[ 0 ];
}

string KHistogram::getFirstCommStatistic() const
{
  vector<string> v;
  FunctionManagement<HistogramStatistic>::getInstance()->getAll( v, 0 );
  return v[ 0 ];
}
