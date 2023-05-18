/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
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


#include <math.h>
#include <limits>
#include "config.h"
#include "kwindow.h"
#include "khistogram.h"
#include "histogramstatistic.h"
#include "histogramexception.h"
#include "khistogramtotals.h"
#include "functionmanagement.h"
#include "kprogresscontroller.h"

#ifdef PARALLEL_ENABLED
#include "omp.h"
#endif

#ifdef _WIN32
#undef max
#undef min
#endif

using namespace std;


WindowCloneManager::WindowCloneManager()
{}


WindowCloneManager::~WindowCloneManager()
{
#ifdef PARALLEL_ENABLED
  for( auto it : clonedWindows )
  {
    for( vector< Timeline * >::iterator itWin = it.second.begin(); itWin != it.second.end(); ++itWin )
      delete *itWin;
  }
#endif
}


Timeline *WindowCloneManager::operator()( Timeline *originalWindow ) const
{
#ifdef PARALLEL_ENABLED
  auto it = clonedWindows.find( originalWindow );
  if ( it != clonedWindows.end() )
    return it->second[ omp_get_thread_num() ];
#endif

  return originalWindow;
}


#ifdef PARALLEL_ENABLED
void WindowCloneManager::update( const KHistogram *whichHistogram )
{
  clone( whichHistogram->getControlWindow() );

  if ( whichHistogram->getDataWindow() != whichHistogram->getControlWindow() )
    clone( whichHistogram->getDataWindow() );
  
  if ( whichHistogram->getExtraControlWindow() != nullptr &&
       whichHistogram->getExtraControlWindow() != whichHistogram->getControlWindow() &&
       whichHistogram->getExtraControlWindow() != whichHistogram->getDataWindow() )
    clone( whichHistogram->getExtraControlWindow() );

}


void WindowCloneManager::clear()
{
  for( auto it : clonedWindows )
  {
    for( auto itWin : it.second )
      delete itWin;
  }

  clonedWindows.clear();
}


void WindowCloneManager::clone( Timeline *whichWindow )
{
  vector< Timeline * > tmpClones;

  for( int i = 0; i != omp_get_num_threads(); ++i )
    tmpClones.push_back( whichWindow->clone( true ) );

  clonedWindows[ whichWindow ] = tmpClones;
}
#endif // PARALLEL_ENABLED


RowsTranslator::RowsTranslator( const RowsTranslator& source )
{
  childInfo = source.childInfo;
}


RowsTranslator::RowsTranslator( vector<KTimeline *>& kwindows )
{
  for ( size_t ii = 0; ii < kwindows.size() - 1; ++ii )
  {
    childInfo.push_back( RowChildInfo() );
    childInfo[ii].oneToOne = ( kwindows[ ii ]->getWindowLevelObjects() ==
                               kwindows[ ii + 1 ]->getWindowLevelObjects() );
    childInfo[ii].numRows = kwindows[ ii ]->getWindowLevelObjects();
    if ( !childInfo[ ii ].oneToOne )
    {
      KTrace *auxTrace = ( KTrace* )kwindows[ ii ]->getTrace();
      for ( TObjectOrder iRow = 0; iRow < kwindows[ ii ]->getWindowLevelObjects(); ++iRow )
      {
        pair< TObjectOrder, TObjectOrder > range;

        range.first = auxTrace->getFirst( iRow,
                                          kwindows[ ii ]->getLevel(),
                                          kwindows[ ii+1 ]->getLevel() );
        range.second =  auxTrace->getLast( iRow,
                                           kwindows[ ii ]->getLevel(),
                                           kwindows[ ii+1 ]->getLevel() );
        childInfo[ii].rowChildren.push_back( range );
      }
    }
  }
}


RowsTranslator::~RowsTranslator()
{}


inline TObjectOrder RowsTranslator::globalTranslate( PRV_UINT16 winIndex,
    TObjectOrder rowIndex ) const
{
  // This method will translate Kwindow rows to 2D rows.
  return rowIndex;
}


inline void RowsTranslator::getRowChildren( PRV_UINT16 winIndex,
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
    iniRow = childInfo[ winIndex ].rowChildren[ rowIndex ].first;
    endRow = childInfo[ winIndex ].rowChildren[ rowIndex ].second;
  }
}

//
inline TObjectOrder RowsTranslator::totalRows() const
{
  return childInfo[ 0 ].numRows;
}


ColumnTranslator::ColumnTranslator( const ColumnTranslator& source )
{
  numColumns = source.numColumns;
  minLimit = source.minLimit;
  maxLimit = source.maxLimit;
  delta = source.delta;
}


ColumnTranslator::ColumnTranslator( THistogramLimit whichMin,
                                    THistogramLimit whichMax,
                                    THistogramLimit whichDelta ):
  minLimit( whichMin ), maxLimit( whichMax ), delta( whichDelta )
{
  double tmpNumColumns;

  // PRECOND: Min <= Max
  tmpNumColumns = ceil( ( maxLimit - minLimit ) / delta );

  if ( delta == 1 && ( tmpNumColumns * delta ) + minLimit <= maxLimit )
    ++tmpNumColumns;

  if( tmpNumColumns <= 0 )
    tmpNumColumns = 1;

  numColumns = static_cast<THistogramColumn>( tmpNumColumns );
}


ColumnTranslator::ColumnTranslator( THistogramLimit whichMin,
                                    THistogramLimit whichMax,
                                    THistogramColumn whichNumColumns ):
  minLimit( whichMin ), maxLimit( whichMax ), numColumns( whichNumColumns )
{
  delta = ( maxLimit - minLimit ) / static_cast<THistogramLimit>( numColumns );
}

ColumnTranslator::~ColumnTranslator()
{}

// returns whichValue in [min,max)
inline bool ColumnTranslator::getColumn( THistogramLimit whichValue,
    THistogramColumn& column ) const
{
  if ( whichValue < minLimit || whichValue > maxLimit )
    return false;

  column = THistogramColumn( floor( ( whichValue - minLimit ) / delta ) );

  if ( column >= numColumns )
    column = numColumns - 1;

  return true;
}


inline THistogramColumn ColumnTranslator::totalColumns() const
{
  return numColumns;
}

inline THistogramLimit ColumnTranslator::getDelta() const
{
  return delta;
}

KHistogram::KHistogram()
{
  controlWindow = nullptr;
  dataWindow = nullptr;
  xtraControlWindow = nullptr;

  useFixedDelta = false;
  controlMin = 0;
  controlMax = 1;
  controlDelta = 1;
  xtraControlMin = 0;
  xtraControlMax = 1;
  xtraControlDelta = 1;
  dataMin = -std::numeric_limits<TSemanticValue>::max();
  dataMax = std::numeric_limits<TSemanticValue>::max();
  burstMin = -std::numeric_limits<TRecordTime>::max();
  burstMax = std::numeric_limits<TRecordTime>::max();
  commSizeMin = std::numeric_limits<TCommSize>::min();
  commSizeMax = std::numeric_limits<TCommSize>::max();
  commTagMin = std::numeric_limits<TCommTag>::min();
  commTagMax = std::numeric_limits<TCommTag>::max();

  controlOutOfLimits = false;
  xtraOutOfLimits = false;

  inclusive = false;

  rowsTranslator = nullptr;
  columnTranslator = nullptr;
  planeTranslator = nullptr;

  cube = nullptr;
  matrix = nullptr;
  commCube = nullptr;
  commMatrix = nullptr;

#ifdef PARALLEL_ENABLED
  semanticBuffer = nullptr;
  commBuffer = nullptr;
#endif

  totals = nullptr;
  rowTotals = nullptr;
  commTotals = nullptr;
  rowCommTotals = nullptr;
}


KHistogram::~KHistogram()
{
  if ( rowsTranslator != nullptr )
    delete rowsTranslator;
  if ( columnTranslator != nullptr )
    delete columnTranslator;
  if ( planeTranslator != nullptr )
    delete planeTranslator;

  if ( cube != nullptr )
    delete cube;
  if ( matrix != nullptr )
    delete matrix;
  if ( commCube != nullptr )
    delete commCube;
  if ( commMatrix != nullptr )
    delete commMatrix;

  if ( totals != nullptr )
    delete totals;
  if ( rowTotals != nullptr )
    delete rowTotals;
  if ( commTotals != nullptr )
    delete commTotals;
  if ( rowCommTotals != nullptr )
    delete rowCommTotals;
 
#ifdef PARALLEL_ENABLED
  if ( semanticBuffer != nullptr )
    delete semanticBuffer;
  if ( commBuffer != nullptr )
    delete commBuffer;
#endif

  clearStatistics();
}


inline bool KHistogram::getThreeDimensions() const
{
  return ( xtraControlWindow != nullptr );
}


inline TRecordTime KHistogram::getBeginTime() const
{
  return beginTime;
}


inline TRecordTime KHistogram::getEndTime() const
{
  return endTime;
}


inline Timeline *KHistogram::getControlWindow() const
{
  return controlWindow;
}


inline Timeline *KHistogram::getDataWindow() const
{
  return dataWindow;
}


inline Timeline *KHistogram::getExtraControlWindow() const
{
  return xtraControlWindow;
}


inline void KHistogram::setControlWindow( Timeline *whichWindow )
{
  controlWindow = ( KTimeline * ) whichWindow;
}


inline void KHistogram::setDataWindow( Timeline *whichWindow )
{
  dataWindow = ( KTimeline * ) whichWindow;
}


inline void KHistogram::setExtraControlWindow( Timeline *whichWindow )
{
  xtraControlWindow = ( KTimeline * ) whichWindow;
}


inline void KHistogram::clearControlWindow()
{
  controlWindow = nullptr;
}


inline void KHistogram::clearDataWindow()
{
  dataWindow = nullptr;
}


inline void KHistogram::clearExtraControlWindow()
{
  xtraControlWindow = nullptr;
}


inline void KHistogram::setUseFixedDelta( bool whichValue )
{
  useFixedDelta = whichValue;
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


inline bool KHistogram::getUseFixedDelta() const
{
  return useFixedDelta;
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


inline bool KHistogram::getInclusiveEnabled() const
{
  if ( controlWindow != nullptr
       && controlWindow->getFirstSemUsefulFunction()->getStack() == nullptr )
    return false;
  return true;
}


inline void KHistogram::setInclusive( bool newValue )
{
  if ( newValue )
  {
    if ( controlWindow->getFirstSemUsefulFunction()->getStack() == nullptr )
      return;
  }
  inclusive = newValue;
}


inline bool KHistogram::getInclusive() const
{
  return inclusive;
}


inline void KHistogram::setNumColumns( THistogramColumn whichNumColumns )
{
  numCols = whichNumColumns;
}


inline THistogramColumn KHistogram::getNumPlanes() const
{
  if ( getThreeDimensions() )
  {
    if( planeTranslator == nullptr )
      return 0;
    return planeTranslator->totalColumns();
  }
  return 1;
}


inline THistogramColumn KHistogram::getNumColumns() const
{
  return numCols;
}


inline THistogramColumn KHistogram::getCommNumColumns() const
{
  return rowsTranslator->totalRows();
}


inline TObjectOrder KHistogram::getNumRows() const
{
  return numRows;
}


inline TSemanticValue KHistogram::getCurrentValue( PRV_UINT32 col,
    PRV_UINT16 idStat,
    PRV_UINT32 plane ) const
{
  if ( getThreeDimensions() )
    return cube->getCurrentValue( plane, col, idStat );
  else
    return matrix->getCurrentValue( col, idStat );

  return TSemanticValue( 0 );
}

inline PRV_UINT32 KHistogram::getCurrentRow( PRV_UINT32 col, PRV_UINT32 plane ) const
{
  if ( getThreeDimensions() )
    return cube->getCurrentRow( plane, col );
  else
    return matrix->getCurrentRow( col );

  return 0;
}

inline void KHistogram::setNextCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  if ( getThreeDimensions() )
    cube->setNextCell( plane, col );
  else
    matrix->setNextCell( col );
}

inline void KHistogram::setFirstCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  if ( getThreeDimensions() )
    cube->setFirstCell( plane, col );
  else
    matrix->setFirstCell( col );
}

inline bool KHistogram::endCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  if ( getThreeDimensions() )
    return cube->endCell( plane, col );
  else
    return matrix->endCell( col );

  return true;
}

inline bool KHistogram::planeWithValues( PRV_UINT32 plane ) const
{
  if ( getThreeDimensions() )
    return cube->planeWithValues( plane );

  return true;
}


inline bool KHistogram::getCellValue( TSemanticValue& semVal,
                                      PRV_UINT32 whichRow,
                                      PRV_UINT32 whichCol,
                                      PRV_UINT16 idStat,
                                      PRV_UINT32 whichPlane ) const
{
  if( getThreeDimensions() )
    return cube->getCellValue( semVal, whichPlane, whichRow, whichCol, idStat );

  return matrix->getCellValue( semVal, whichRow, whichCol, idStat );
}


bool KHistogram::getNotZeroValue( PRV_UINT32 whichRow,
                                  PRV_UINT32 whichCol,
                                  PRV_UINT16 idStat,
                                  PRV_UINT32 whichPlane ) const
{
  if( getThreeDimensions() )
    return cube->getNotZeroValue( whichPlane, whichRow, whichCol, idStat );

  return matrix->getNotZeroValue( whichRow, whichCol, idStat );
}


inline TSemanticValue KHistogram::getCommCurrentValue( PRV_UINT32 col,
                                                       PRV_UINT16 idStat,
                                                       PRV_UINT32 plane ) const
{
  if ( getThreeDimensions() )
    return commCube->getCurrentValue( plane, col, idStat );
  else
    return commMatrix->getCurrentValue( col, idStat );

  return TSemanticValue( 0 );
}


inline PRV_UINT32 KHistogram::getCommCurrentRow( PRV_UINT32 col, PRV_UINT32 plane ) const
{
  if ( getThreeDimensions() )
    return commCube->getCurrentRow( plane, col );
  else
    return commMatrix->getCurrentRow( col );

  return 0;
}

inline void KHistogram::setCommNextCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  if ( getThreeDimensions() )
    commCube->setNextCell( plane, col );
  else
    commMatrix->setNextCell( col );
}

inline void KHistogram::setCommFirstCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  if ( getThreeDimensions() )
    commCube->setFirstCell( plane, col );
  else
    commMatrix->setFirstCell( col );
}

inline bool KHistogram::endCommCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  if ( getThreeDimensions() )
    return commCube->endCell( plane, col );
  else
    return commMatrix->endCell( col );

  return true;
}

inline bool KHistogram::planeCommWithValues( PRV_UINT32 plane ) const
{
  if ( getThreeDimensions() )
    return commCube->planeWithValues( plane );

  return true;
}

inline bool KHistogram::getCommCellValue( TSemanticValue& semVal,
    PRV_UINT32 whichRow,
    PRV_UINT32 whichCol,
    PRV_UINT16 idStat,
    PRV_UINT32 whichPlane ) const
{
  if( getThreeDimensions() )
    return commCube->getCellValue( semVal, whichPlane, whichRow, whichCol, idStat );

  return commMatrix->getCellValue( semVal, whichRow, whichCol, idStat );
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
  /*  vector<HistogramStatistic *>::iterator it = statisticFunctions.begin();

    while ( it != statisticFunctions.end() )
    {
      delete *it;
      ++it;
    }
    statisticFunctions.clear();

    it = commStatisticFunctions.begin();

    while ( it != commStatisticFunctions.end() )
    {
      delete *it;
      ++it;
    }
    commStatisticFunctions.clear();*/
}


inline void KHistogram::pushbackStatistic( const string& whichStatistic )
{
  /*  HistogramStatistic *stat;

    stat = ( FunctionManagement<HistogramStatistic>::getInstance() )->getFunction( whichStatistic );

    if ( stat->createComms() )
      commStatisticFunctions.push_back( stat );
    else
      statisticFunctions.push_back( stat );
    
    delete stat;
  */
}


void KHistogram::execute( TRecordTime whichBeginTime, TRecordTime whichEndTime,
                          vector<TObjectOrder>& selectedRows, ProgressController *progress )
{
  if ( controlWindow == nullptr )
    throw HistogramException( THistogramErrorCode::noControlWindow );

  myTimeUnit = controlWindow->getTimeUnit();

  if ( dataWindow == nullptr )
    dataWindow = controlWindow;

  controlOutOfLimits = false;
  xtraOutOfLimits = false;

  beginTime = whichBeginTime;
  endTime = whichEndTime;
  if ( endTime > controlWindow->getTrace()->getEndTime() )
    endTime = controlWindow->getTrace()->getEndTime();

  orderWindows();

  initTranslators();

  numRows = selectedRows.size();
  if( useFixedDelta )
    numCols = columnTranslator->totalColumns();
  else
    controlDelta = columnTranslator->getDelta();

  if ( getThreeDimensions() )
    numPlanes = planeTranslator->totalColumns();
  else
    numPlanes = 1;

  if( progress != nullptr )
  {
    if( numRows > 1 )
      progress->setEndLimit( numRows );
    else
      progress->setEndLimit( endTime - beginTime );

    progress->setCurrentProgress( 0 );
  }

  initMatrix( numPlanes, numCols, numRows );

  initSemantic( beginTime );

  initStatistics();

  initTotals();

#ifdef PARALLEL_ENABLED
  initTmpBuffers( numPlanes, rowsTranslator->totalRows() );
#else
  initTmpBuffers( numPlanes, numRows );
#endif

  vector<bool> needInit( 3, true );
#ifdef PARALLEL_ENABLED
  if ( orderedWindows[ 0 ]->getWindowLevelObjects() == orderedWindows[ 1 ]->getWindowLevelObjects()
       &&
        ( ( !getThreeDimensions() )
          ||
          ( getThreeDimensions() && orderedWindows[ 1 ]->getWindowLevelObjects() == orderedWindows[ 2 ]->getWindowLevelObjects() )
        )
     )
  {
    parallelExecution( beginTime, endTime, 0, numRows - 1, selectedRows, progress );
  }
  else
#endif

  recursiveExecution( beginTime, endTime, 0, numRows - 1, selectedRows, needInit, true, progress );

#ifdef PARALLEL_ENABLED
  finishAllRows();
#endif

  finishOutLimits();

  if ( getThreeDimensions() )
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

  if ( totals != nullptr )
    totals->finish();
  if ( rowTotals != nullptr )
    rowTotals->finish();
  if ( commTotals != nullptr )
    commTotals->finish();
  if ( rowCommTotals != nullptr )
    rowCommTotals->finish();
  // - Columns will be ordered if necesary
}


void KHistogram::orderWindows()
{
  orderedWindows.clear();

  if ( getThreeDimensions() )
  {
    if ( controlWindow == dataWindow )
    {
      orderedWindows.push_back( xtraControlWindow );
      orderedWindows.push_back( controlWindow );
    }
    else if ( controlWindow->getLevel() >= xtraControlWindow->getLevel() )
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
//  return commStatisticFunctions.size() > 0;
  return true;
}


void KHistogram::initTranslators()
{
  if ( rowsTranslator != nullptr )
    delete rowsTranslator;
  rowsTranslator = new RowsTranslator( orderedWindows );

  if ( columnTranslator != nullptr )
    delete columnTranslator;
  if( useFixedDelta )
    columnTranslator = new ColumnTranslator( controlMin, controlMax, controlDelta );
  else
    columnTranslator = new ColumnTranslator( controlMin, controlMax, numCols );

  if ( planeTranslator != nullptr )
  {
    delete planeTranslator;
    planeTranslator = nullptr;
  }
  if ( getThreeDimensions() )
    planeTranslator = new ColumnTranslator( xtraControlMin, xtraControlMax, xtraControlDelta );
}


void KHistogram::initMatrix( THistogramColumn planes, THistogramColumn cols,
                             TObjectOrder rows )
{
  if ( cube != nullptr )
  {
    delete cube;
    cube = nullptr;
  }
  if ( matrix != nullptr )
  {
    delete matrix;
    matrix = nullptr;
  }
  if ( commCube != nullptr )
  {
    delete commCube;
    commCube = nullptr;
  }
  if ( commMatrix != nullptr )
  {
    delete commMatrix;
    commMatrix = nullptr;
  }

  if ( getThreeDimensions() )
  {
    cube = new Cube<TSemanticValue, NUM_SEMANTIC_STATS>( planes, cols );
    if ( createComms() )
      commCube = new Cube<TSemanticValue, NUM_COMM_STATS>( planes, rowsTranslator->totalRows() );
  }
  else
  {
    matrix = new Matrix<TSemanticValue, NUM_SEMANTIC_STATS>( cols );
    if ( createComms() )
      commMatrix = new Matrix<TSemanticValue, NUM_COMM_STATS>( rowsTranslator->totalRows() );
  }
}


void KHistogram::initTotals()
{
  if ( totals != nullptr )
    delete totals;
  if ( rowTotals != nullptr )
    delete rowTotals;
  if ( commTotals != nullptr )
    delete commTotals;
  if ( rowCommTotals != nullptr )
    delete rowCommTotals;

  if ( getThreeDimensions() )
  {
    totals = new KHistogramTotals( NUM_SEMANTIC_STATS, numCols, numPlanes );
    rowTotals = new KHistogramTotals( NUM_SEMANTIC_STATS, numRows, numPlanes );
    if ( createComms() )
    {
      commTotals = new KHistogramTotals( NUM_COMM_STATS,
                                         rowsTranslator->totalRows(), numPlanes );
      rowCommTotals = new KHistogramTotals( NUM_COMM_STATS,
                                            numRows, numPlanes );
    }
  }
  else
  {
    totals = new KHistogramTotals( NUM_SEMANTIC_STATS, numCols, 1 );
    rowTotals = new KHistogramTotals( NUM_SEMANTIC_STATS, numRows, 1 );
    if ( createComms() )
    {
      commTotals = new KHistogramTotals( NUM_COMM_STATS,
                                         rowsTranslator->totalRows(), 1 );
      rowCommTotals = new KHistogramTotals( NUM_COMM_STATS,
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

  if ( xtraControlWindow != nullptr && xtraControlWindow != controlWindow )
    xtraControlWindow->init( beginTime, NOCREATE );

  if ( dataWindow != controlWindow && dataWindow != xtraControlWindow )
    dataWindow->init( beginTime, NOCREATE );
}


void KHistogram::initStatistics()
{
  statistics.initAll( this );
  statistics.initAllComm( this );
}

void KHistogram::initTmpBuffers( THistogramColumn planes, TObjectOrder rows )
{
  tmpControlOutOfLimits.clear();
  tmpControlOutOfLimits.insert( tmpControlOutOfLimits.begin(),
                                controlWindow->getWindowLevelObjects(),
                                false );

  if ( getThreeDimensions() )
  {
    tmpXtraOutOfLimits.clear();
    tmpXtraOutOfLimits.insert( tmpXtraOutOfLimits.begin(),
                               xtraControlWindow->getWindowLevelObjects(),
                               false );
  }

#ifdef PARALLEL_ENABLED
  if ( semanticBuffer != nullptr )
  {
    delete semanticBuffer;
    semanticBuffer = nullptr;
  }
  semanticBuffer = new CubeBuffer<NUM_SEMANTIC_STATS>( planes, rows );

  if ( commBuffer != nullptr )
  {
    delete commBuffer;
    commBuffer = nullptr;
  }
  commBuffer = new CubeBuffer<NUM_COMM_STATS>( planes, rows );
#endif
}

#ifdef PARALLEL_ENABLED
void KHistogram::finishAllRows()
{
  array<TSemanticValue, NUM_COMM_STATS> commValues;

  if ( getThreeDimensions() )
  {
    for ( TObjectOrder iRow = 0; iRow < rowsTranslator->totalRows(); ++iRow )
    {
      for ( THistogramColumn iPlane = 0; iPlane < planeTranslator->totalColumns(); ++iPlane )
      {
        auto& rowValues = commBuffer->getRowValues( iPlane, iRow );
        for ( auto it = rowValues.begin(); it != rowValues.end(); ++it )
        {
          commValues = it->second;
          commCube->setValue( iPlane, it->first, commValues );
          for ( PRV_UINT16 iStat = 0; iStat < NUM_COMM_STATS; ++iStat )
          {
            commTotals->newValue( commValues[ iStat ], iStat, it->first, iPlane );
            rowCommTotals->newValue( commValues[ iStat ], iStat, iRow, iPlane );
          }
        }
      }

      commCube->newRow();
    }
  }
  else
  {
    for ( TObjectOrder iRow = 0; iRow < rowsTranslator->totalRows(); ++iRow )
    {
      auto& rowValues = commBuffer->getRowValues( 0, iRow );
      for ( auto it = rowValues.begin(); it != rowValues.end(); ++it )
      {
        commValues = it->second;
        commMatrix->setValue( it->first, commValues );
        for ( PRV_UINT16 iStat = 0; iStat < NUM_COMM_STATS; ++iStat )
        {
          commTotals->newValue( commValues[ iStat ], iStat, it->first );
          rowCommTotals->newValue( commValues[ iStat ], iStat, iRow );
        }
      }

      commMatrix->newRow();
    }
  }

  array<TSemanticValue, NUM_SEMANTIC_STATS> semanticValues;

  if ( getThreeDimensions() )
  {
    for ( TObjectOrder iRow = 0; iRow < rowsTranslator->totalRows(); ++iRow )
    {
      for ( THistogramColumn iPlane = 0; iPlane < planeTranslator->totalColumns();
            ++iPlane )
      {
        auto& rowValues = semanticBuffer->getRowValues( iPlane, iRow );
        auto& rowNotZeroValues = semanticBuffer->getNotZeroValue( iPlane, iRow );
        auto itZero = rowNotZeroValues.begin();

        for ( auto it = rowValues.begin(); it != rowValues.end(); ++it, ++itZero )
        {
          semanticValues = it->second;
          cube->setValue( iPlane, it->first, semanticValues, itZero->second );

          for ( PRV_UINT16 iStat = 0; iStat < NUM_SEMANTIC_STATS; ++iStat )
          {
            totals->newValue( semanticValues[ iStat ], iStat, it->first, iPlane );
            rowTotals->newValue( semanticValues[ iStat ], iStat, iRow, iPlane );
          }
        }
      }
      cube->newRow();
    }
  }
  else
  {
    THistogramColumn iPlane = 0;
    for ( TObjectOrder iRow = 0; iRow < rowsTranslator->totalRows(); ++iRow )
    {
      auto& rowValues = semanticBuffer->getRowValues( 0, iRow );
      auto& rowNotZeroValues = semanticBuffer->getNotZeroValue( 0, iRow );
      auto itZero = rowNotZeroValues.begin();

      for ( auto it = rowValues.begin(); it != rowValues.end(); ++it, ++itZero )
      {
        semanticValues = it->second;
        matrix->setValue( it->first, semanticValues, itZero->second );

        for ( PRV_UINT16 iStat = 0; iStat < NUM_SEMANTIC_STATS; ++iStat )
        {
          totals->newValue( semanticValues[ iStat ], iStat, it->first, iPlane );
          rowTotals->newValue( semanticValues[ iStat ], iStat, iRow, iPlane );
        }
      }

      matrix->newRow();
    }
  }

  statistics.resetAllComm();
  statistics.resetAll();
}
#endif

void KHistogram::finishOutLimits()
{
  controlOutOfLimits = false;
  for( vector<bool>::iterator it = tmpControlOutOfLimits.begin();
       it != tmpControlOutOfLimits.end(); ++it )
  {
    if ( *it )
    {
      controlOutOfLimits = true;
      break;
    }
  }
  tmpControlOutOfLimits.clear();

  if ( getThreeDimensions() )
  {
    xtraOutOfLimits = false;
    for( vector<bool>::iterator it = tmpXtraOutOfLimits.begin();
         it != tmpXtraOutOfLimits.end(); ++it )
    {
      if ( *it )
      {
        xtraOutOfLimits = true;
        break;
      }
    }
    tmpXtraOutOfLimits.clear();
  }
}

#ifdef PARALLEL_ENABLED
void KHistogram::parallelExecution( TRecordTime fromTime, TRecordTime toTime,
                                    TObjectOrder fromRow, TObjectOrder toRow,
                                    std::vector<TObjectOrder>& selectedRows,
                                    ProgressController *progress )
{
  int currentRow = 0;
  int progressDelta;
  if( progress != nullptr )
    progressDelta = (int)floor( selectedRows.size() * 0.005 );

  #pragma omp parallel
  {
    #pragma omp single
    {
      windowCloneManager.update( this );

      for ( TObjectOrder i = fromRow; i <= toRow; ++i )
      {
        #pragma omp task firstprivate(fromTime, toTime, i) shared(selectedRows, progress, progressDelta)
        {
          if( progress == nullptr ||
              ( progress != nullptr && !progress->getStop() ) )
            executionTask( fromTime, toTime, i, i, selectedRows, progress );

          if( progress != nullptr && numRows > 1 && !progress->getStop() )
          {
            #pragma omp atomic
            ++currentRow;
            if( selectedRows.size() <= 200 || currentRow % progressDelta == 0 )
            {
              #pragma omp critical
              progress->setCurrentProgress( currentRow );
            }
          }
        }
      }
    }
  }

  windowCloneManager.clear();
}


void KHistogram::executionTask( TRecordTime fromTime, TRecordTime toTime,
                                TObjectOrder fromRow, TObjectOrder toRow,
                                std::vector<TObjectOrder>& selectedRows,
                                ProgressController *progress )
{
  vector<bool> needInit( 3, true );
  recursiveExecution( fromTime, toTime, fromRow, toRow, selectedRows, needInit, true, progress );
}
#endif

void KHistogram::recursiveExecution( TRecordTime fromTime, TRecordTime toTime,
                                     TObjectOrder fromRow, TObjectOrder toRow,
                                     vector<TObjectOrder>& selectedRows,
                                     vector<bool>& needInit,
                                     bool calcSemanticStats,
                                     ProgressController *progress,
                                     PRV_UINT16 winIndex, CalculateData *data )
{
  Timeline *currentWindow = orderedWindows[ winIndex ];
  int currentRow = 0;
#ifndef PARALLEL_ENABLED
  int progressDelta;
  if( progress != nullptr )
    progressDelta = (int)floor( selectedRows.size() * 0.005 );
#endif // PARALLEL_ENABLED

  if ( data == nullptr )
  {
    data = new CalculateData;
    data->plane = 0;
    data->beginTime = 0;
    data->endTime = 0;
  }

  for ( TObjectOrder i = fromRow; i <= toRow; ++i )
  {
    TObjectOrder iRow = i;

    if ( winIndex == 0 )
    {
      iRow = selectedRows[ i ];
      data->row = i;

      needInit[ 0 ] = true;
      needInit[ 1 ] = true;
      needInit[ 2 ] = true;
    }

    if ( currentWindow == controlWindow )
      data->controlRow = iRow;
    if ( currentWindow == dataWindow )
      data->dataRow = iRow;

    if( needInit[ winIndex ] && ( winIndex == 0 || ( winIndex > 0 && orderedWindows[ winIndex ] != orderedWindows[ winIndex - 1 ] ) ) )
    {
      if( orderedWindows[ winIndex ] == orderedWindows[ 0 ] )
        windowCloneManager( currentWindow )->initRow( iRow, fromTime, CREATECOMMS );
      else
        windowCloneManager( currentWindow )->initRow( iRow, fromTime, NOCREATE );

      needInit[ winIndex ] = false;
    }

    while ( windowCloneManager( currentWindow )->getEndTime( iRow ) <= fromTime
            && windowCloneManager( currentWindow )->getBeginTime( iRow ) < currentWindow->getTrace()->getEndTime() )
    {
      windowCloneManager( currentWindow )->calcNext( iRow );
    }

    int progressSteps = 0;
    TRecordTime tmpLastTime = windowCloneManager( currentWindow )->getBeginTime( iRow );
    while ( windowCloneManager( currentWindow )->getEndTime( iRow ) < toTime
            && windowCloneManager( currentWindow )->getBeginTime( iRow ) < currentWindow->getTrace()->getEndTime() )
    {
      if( windowCloneManager( currentWindow )->getBeginTime( iRow ) != windowCloneManager( currentWindow )->getEndTime( iRow ) )
        calculate( iRow, fromTime, toTime, winIndex, data, needInit, calcSemanticStats );

      windowCloneManager( currentWindow )->calcNext( iRow );
      if( progress != nullptr )
      {
        if( progress->getStop() )
          break;
        if( numRows > 1 )
        {
          ++progressSteps;
          if( progressSteps == 100000 )
          {
            progressSteps = 0;
            #pragma omp critical
            {
              progress->setCurrentProgress( progress->getCurrentProgress() );
            }
          }
        }
        else
        {
          if( windowCloneManager( currentWindow )->getEndTime( iRow ) - tmpLastTime >
              ( toTime - fromTime ) / 50 )
          {
            #pragma omp critical
            {
              progress->setCurrentProgress( windowCloneManager( currentWindow )->getEndTime( iRow ) - beginTime );
            }
            tmpLastTime = windowCloneManager( currentWindow )->getEndTime( iRow );
          }
        }
      }
    }

    if ( windowCloneManager( currentWindow )->getBeginTime( iRow ) < toTime )
      calculate( iRow, fromTime, toTime, winIndex, data, needInit, calcSemanticStats );

    while ( windowCloneManager( currentWindow )->getBeginTime( iRow ) == windowCloneManager( currentWindow )->getEndTime( iRow ) &&
            windowCloneManager( currentWindow )->getEndTime( iRow ) <= toTime &&
            windowCloneManager( currentWindow )->getEndTime( iRow ) < getEndTime() &&
            windowCloneManager( currentWindow )->getBeginTime( iRow ) < currentWindow->getTrace()->getEndTime() )
    {
      windowCloneManager( currentWindow )->calcNext( iRow );
    }

    if ( winIndex == 0 )
    {
      finishRow( data );
    }

#ifndef PARALLEL_ENABLED
    if( progress != nullptr && numRows > 1 )
    {
      if( progress->getStop() )
        break;
      if( selectedRows.size() <= 200 || currentRow % progressDelta == 0 )
      {
        progress->setCurrentProgress( currentRow );
      }
    }
    ++currentRow;
#endif // PARALLEL_ENABLED

  }

  if ( winIndex == 0 )
  {
    delete data;
    data = nullptr;
  }
}


void KHistogram::calculate( TObjectOrder iRow,
                            TRecordTime fromTime, TRecordTime toTime,
                            PRV_UINT16 winIndex, CalculateData *data,
                            vector<bool>& needInit,
                            bool calcSemanticStats )
{
  TObjectOrder childFromRow;
  TObjectOrder childToRow;
  TRecordTime childFromTime;
  TRecordTime childToTime;
  Timeline *currentWindow = orderedWindows[ winIndex ];

  if ( currentWindow == controlWindow )
  {
    if ( !columnTranslator->getColumn( windowCloneManager( currentWindow )->getValue( iRow ),
                                       data->column ) )
    {
      if ( windowCloneManager( currentWindow )->getValue( iRow ) != 0 )
        tmpControlOutOfLimits[ iRow ] = true;
      calcSemanticStats = false;
    }
    else
      calcSemanticStats = true;
    data->rList = windowCloneManager( currentWindow )->getRecordList( iRow );
  }
  if ( getThreeDimensions() && currentWindow == xtraControlWindow )
  {
    if ( !planeTranslator->getColumn( windowCloneManager( currentWindow )->getValue( iRow ),
                                      data->plane ) )
    {
      if ( windowCloneManager( currentWindow )->getValue( iRow ) != 0 )
        tmpXtraOutOfLimits[ iRow ] = true;
      return;
    }
  }

  if ( winIndex == orderedWindows.size() - 1 )
  {
    data->beginTime = ( fromTime < windowCloneManager( currentWindow )->getBeginTime( iRow ) ) ?
                      windowCloneManager( currentWindow )->getBeginTime( iRow ) :
                      fromTime;
    data->endTime = ( toTime > windowCloneManager( currentWindow )->getEndTime( iRow ) ) ?
                    windowCloneManager( currentWindow )->getEndTime( iRow ) :
                    toTime;

    // Communication statistics
    vector<bool> filter;
    array<TSemanticValue, NUM_COMM_STATS> commValues;
    RecordList::iterator itComm = data->rList->begin();
    while ( itComm != data->rList->end()/* &&
            itComm->getTime() >= getBeginTime() &&
            itComm->getTime() <= getEndTime()*/ )
    {
      if ( !( itComm->getRecordType() & COMM ) ||
           !( itComm->getTime() >= getBeginTime() && itComm->getTime() <= getEndTime() )
         )
      {
        ++itComm;
        continue;
      }

      data->comm = itComm;
      statistics.executeAllComm( data, commValues );
      if ( statistics.filterAllComm( data ) )
      {
        TObjectOrder column;
        if ( controlWindow->getLevel() >= TTraceLevel::WORKLOAD && controlWindow->getLevel() <= TTraceLevel::THREAD )
          column = controlWindow->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
        else if( controlWindow->getLevel() >= TTraceLevel::SYSTEM && controlWindow->getLevel() <= TTraceLevel::NODE )
          column = controlWindow->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
        else // CPU
          column = controlWindow->cpuObjectToWindowObject( data->comm->getCommPartnerObject() ) - 1;
#ifdef PARALLEL_ENABLED
        commBuffer->addValue( data->plane, data->row, column, commValues );
#else
        if ( getThreeDimensions() )
          commCube->addValue( data->plane, column, commValues );
        else
          commMatrix->addValue( column, commValues );
#endif
      }

      ++itComm;
    }

    data->rList->erase( data->rList->begin(), itComm );

    if( !calcSemanticStats )
      return;

    bool isNotZeroValue = false;
    array< TSemanticValue, NUM_SEMANTIC_STATS > semanticValues;
    
    // Semantic statistics
    if ( inclusive )
    {
      THistogramColumn column;
      auto *tmp = controlWindow->getFirstSemUsefulFunction()->getStack();
      if( tmp->find( data->controlRow ) != tmp->end() )
      {
        vector<TSemanticValue>::iterator it = ( *tmp )[ data->controlRow ].begin();
        while ( it != ( *tmp )[ data->controlRow ].end() )
        {
          if ( columnTranslator->getColumn( *it, column ) )
          {
            statistics.executeAll( data, semanticValues, isNotZeroValue );

            if ( statistics.filterAll( data ) )
            {
#ifdef PARALLEL_ENABLED
              semanticBuffer->addValue( data->plane, data->row, column, semanticValues, isNotZeroValue );
#else
              if ( getThreeDimensions() )
                cube->addValue( data->plane, column, semanticValues );
              else
                matrix->addValue( column, semanticValues );
#endif
            }
          }
          ++it;
        }
      }
    }
    else
    {
      statistics.executeAll( data, semanticValues, isNotZeroValue );

      if ( statistics.filterAll( data ) )
      {
#ifdef PARALLEL_ENABLED
        semanticBuffer->addValue( data->plane, data->row, data->column, semanticValues, isNotZeroValue );
#else
        if ( getThreeDimensions() )
          cube->addValue( data->plane, data->column, semanticValues );
        else
          matrix->addValue( data->column, semanticValues );
#endif
      }
    }
  }
  else
  {
    childFromTime = ( fromTime < windowCloneManager( currentWindow )->getBeginTime( iRow ) ) ?
                    windowCloneManager( currentWindow )->getBeginTime( iRow ) :
                    fromTime;
    childToTime = ( toTime > windowCloneManager( currentWindow )->getEndTime( iRow ) ) ?
                  windowCloneManager( currentWindow )->getEndTime( iRow ) :
                  toTime;
    rowsTranslator->getRowChildren( winIndex, iRow, childFromRow, childToRow );

    vector<TObjectOrder> *dummy = nullptr;
    recursiveExecution( childFromTime, childToTime, childFromRow, childToRow,
                        *dummy, needInit, calcSemanticStats, nullptr, winIndex + 1, data );
  }
}


void KHistogram::finishRow( CalculateData *data )
{
  array<TSemanticValue, NUM_COMM_STATS> commValues;

  if ( getThreeDimensions() )
  {
    for ( THistogramColumn iPlane = 0; iPlane < planeTranslator->totalColumns();
          ++iPlane )
    {
#ifdef PARALLEL_ENABLED
      auto& rowValues = commBuffer->getRowValues( iPlane, data->row );
      for ( auto it = rowValues.begin(); it != rowValues.end(); ++it )
      {
        //values = it->second;
        commValues = statistics.finishRowAllComm( it->second, it->first, data->row, iPlane );
        commBuffer->setValue( iPlane, data->row, it->first, commValues );
      }
#else
      if ( commCube->planeWithValues( iPlane ) )
      {
        for ( TObjectOrder iColumn = 0;
              iColumn < rowsTranslator->totalRows(); ++iColumn )
        {
          if ( commCube->currentCellModified( iPlane, iColumn ) )
          {
            commValues = commCube->getCurrentValue( iPlane, iColumn );
            commValues = statistics.finishRowAllComm( commValues, iColumn, data->row, iPlane );

            commCube->setValue( iPlane, iColumn, commValues );
            for ( PRV_UINT16 iStat = 0; iStat < NUM_COMM_STATS; ++iStat )
            {
              commTotals->newValue( commValues[ iStat ], iStat, iColumn, iPlane );
              rowCommTotals->newValue( commValues[ iStat ], iStat, data->row, iPlane );
            }
          }
        }
      }
#endif
    }
  }
  else
  {
#ifdef PARALLEL_ENABLED
    auto& rowValues = commBuffer->getRowValues( 0, data->row );
    for ( auto it = rowValues.begin(); it != rowValues.end(); ++it )
    {
      //values = it->second;
      commValues = statistics.finishRowAllComm( it->second, it->first, data->row );
      commBuffer->setValue( 0, data->row, it->first, commValues );
    }
#else
    for ( TObjectOrder iColumn = 0;
          iColumn < rowsTranslator->totalRows(); ++iColumn )
    {
      if ( commMatrix->currentCellModified( iColumn ) )
      {
        commValues = commMatrix->getCurrentValue( iColumn );
        commValues = statistics.finishRowAllComm( commValues, iColumn, data->row );

        commMatrix->setValue( iColumn, commValues );
        for ( PRV_UINT16 iStat = 0; iStat < NUM_COMM_STATS; ++iStat )
        {
          commTotals->newValue( commValues[ iStat ], iStat, iColumn );
          rowCommTotals->newValue( commValues[ iStat ], iStat, data->row );
        }
      }
    }
#endif
  }

#ifndef PARALLEL_ENABLED
  statistics.resetAllComm();
#endif

  array<TSemanticValue, NUM_SEMANTIC_STATS> semanticValues;
  
  if ( getThreeDimensions() )
  {
    for ( THistogramColumn iPlane = 0; iPlane < planeTranslator->totalColumns(); ++iPlane )
    {
#ifdef PARALLEL_ENABLED
      auto& rowValues = semanticBuffer->getRowValues( iPlane, data->row );
      for ( auto it = rowValues.begin(); it != rowValues.end(); ++it )
      {
        //values = it->second;
        semanticValues = statistics.finishRowAll( it->second, it->first, data->row, iPlane );
        semanticBuffer->setValue( iPlane, data->row, it->first, semanticValues );
      }
#else
      if ( cube->planeWithValues( iPlane ) )
      {
        for ( THistogramColumn iColumn = 0;
              iColumn < columnTranslator->totalColumns(); ++iColumn )
        {
          if ( cube->currentCellModified( iPlane, iColumn ) )
          {
            semanticValues = cube->getCurrentValue( iPlane, iColumn );
            semanticValues = statistics.finishRowAll( semanticValues, iColumn, data->row, iPlane );

            cube->setValue( iPlane, iColumn, semanticValues );
            for ( PRV_UINT16 iStat = 0; iStat < NUM_SEMANTIC_STATS; ++iStat )
            {
              totals->newValue( semanticValues[ iStat ], iStat, iColumn, iPlane );
              rowTotals->newValue( semanticValues[ iStat ], iStat, data->row, iPlane );
            }
          }
        }
      }
#endif
    }
  }
  else
  {
#ifdef PARALLEL_ENABLED
    auto& rowValues = semanticBuffer->getRowValues( 0, data->row );
    for ( auto it = rowValues.begin(); it != rowValues.end(); ++it )
    {
      //values = it->second;
      semanticValues = statistics.finishRowAll( it->second, it->first, data->row );
      semanticBuffer->setValue( 0, data->row, it->first, semanticValues );
    }
#else
    for ( THistogramColumn iColumn = 0; iColumn < columnTranslator->totalColumns(); ++iColumn )
    {
      if ( matrix->currentCellModified( iColumn ) )
      {
        semanticValues = matrix->getCurrentValue( iColumn );
        semanticValues = statistics.finishRowAll( semanticValues, iColumn, data->row );
        matrix->setValue( iColumn, semanticValues );
        for ( PRV_UINT16 iStat = 0; iStat < NUM_SEMANTIC_STATS; ++iStat )
        {
          totals->newValue( semanticValues[ iStat ], iStat, iColumn );
          rowTotals->newValue( semanticValues[ iStat ], iStat, data->row );
        }
      }
    }
#endif
  }

#ifndef PARALLEL_ENABLED
  statistics.resetAll();

  // Next row
  if ( createComms() )
  {
    if ( getThreeDimensions() )
      commCube->newRow();
    else
      commMatrix->newRow();
  }

  if ( getThreeDimensions() )
    cube->newRow();
  else
    matrix->newRow();
#endif
}

bool KHistogram::isCommunicationStat( const string& whichStat ) const
{
  return FunctionManagement<HistogramStatistic>::getInstance()->getFunctionNoClone( whichStat )->createComms();
}

bool KHistogram::isNotZeroStat( const string& whichStat ) const
{
  return FunctionManagement<HistogramStatistic>::getInstance()->getFunctionNoClone( whichStat )->isNotZeroStat();
}

string KHistogram::getUnitsLabel( const string& whichStat ) const
{
  return FunctionManagement<HistogramStatistic>::getInstance()->getFunctionNoClone( whichStat )->getUnits( this );
}

void KHistogram::getGroupsLabels( vector<string>& onVector ) const
{
  FunctionManagement<HistogramStatistic>::getInstance()->getNameGroups( onVector );
}

void KHistogram::getStatisticsLabels( vector<string>& onVector,
                                      PRV_UINT32 whichGroup,
                                      bool dummy ) const
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

bool KHistogram::getControlOutOfLimits() const
{
  return controlOutOfLimits;
}

bool KHistogram::getExtraOutOfLimits() const
{
  return xtraOutOfLimits;
}

TTimeUnit KHistogram::getTimeUnit() const
{
  return myTimeUnit;
}


Timeline *KHistogram::getClonedWindow( Timeline *whichWindow ) const
{
  return windowCloneManager( whichWindow );
}


KHistogram *KHistogram::clone()
{
  KHistogram *clonedKHistogram = new KHistogram();

  clonedKHistogram->controlWindow = nullptr;
  clonedKHistogram->dataWindow = nullptr;
  if ( clonedKHistogram->xtraControlWindow != nullptr )
    clonedKHistogram->xtraControlWindow = nullptr;


  clonedKHistogram->beginTime = beginTime;
  clonedKHistogram->endTime = endTime;

  clonedKHistogram->numRows = numRows;
  clonedKHistogram->numCols = numCols;
  clonedKHistogram->numPlanes = numPlanes;

  clonedKHistogram->useFixedDelta = useFixedDelta;
  clonedKHistogram->controlMin = controlMin;
  clonedKHistogram->controlMax = controlMax;
  clonedKHistogram->controlDelta = controlDelta;
  clonedKHistogram->xtraControlMin = xtraControlMin;
  clonedKHistogram->xtraControlMax = xtraControlMax;
  clonedKHistogram->xtraControlDelta = xtraControlDelta;
  clonedKHistogram->dataMin = dataMin;
  clonedKHistogram->dataMax = dataMax;
  clonedKHistogram->burstMin = burstMin;
  clonedKHistogram->burstMax = burstMax;
  clonedKHistogram->commSizeMin = commSizeMin;
  clonedKHistogram->commSizeMax = commSizeMax;
  clonedKHistogram->commTagMin = commTagMin;
  clonedKHistogram->commTagMax = commTagMax;

  clonedKHistogram->inclusive = inclusive;

  /*  for ( vector<HistogramStatistic *>::iterator it = statisticFunctions.begin();
          it != statisticFunctions.end(); it++ )
      clonedKHistogram->statisticFunctions.push_back( ( *it )->clone() );

    for ( vector<HistogramStatistic *>::iterator it = commStatisticFunctions.begin();
          it != commStatisticFunctions.end(); it++ )
      clonedKHistogram->commStatisticFunctions.push_back( ( *it )->clone() );*/

  clonedKHistogram->rowsTranslator = new RowsTranslator( *rowsTranslator );
  clonedKHistogram->columnTranslator = new ColumnTranslator( *columnTranslator );

  if ( clonedKHistogram->planeTranslator != nullptr )
    clonedKHistogram->planeTranslator = new ColumnTranslator( *planeTranslator );

  if ( cube != nullptr )
    clonedKHistogram->cube = new Cube<TSemanticValue, NUM_SEMANTIC_STATS>( *cube );
  if ( matrix != nullptr )
    clonedKHistogram->matrix = new Matrix<TSemanticValue, NUM_SEMANTIC_STATS>( *matrix );
  if ( commCube != nullptr )
    clonedKHistogram->commCube = new Cube<TSemanticValue, NUM_COMM_STATS>( *commCube );
  if ( commMatrix != nullptr )
    clonedKHistogram->commMatrix = new Matrix<TSemanticValue, NUM_COMM_STATS>( *commMatrix );

  clonedKHistogram->totals = new KHistogramTotals( totals );
  clonedKHistogram->rowTotals = new KHistogramTotals( rowTotals );
  clonedKHistogram->commTotals = new KHistogramTotals( commTotals );
  clonedKHistogram->rowCommTotals = new KHistogramTotals( rowCommTotals );

  clonedKHistogram->orderWindows();

  return clonedKHistogram;
}
