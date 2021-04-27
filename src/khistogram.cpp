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
#include "cubebuffer.h"
#include "omp.h"
#endif

#ifdef WIN32
#undef max
#undef min
#endif

using namespace std;


WindowCloneManager::WindowCloneManager()
{}


WindowCloneManager::~WindowCloneManager()
{
#ifdef PARALLEL_ENABLED
  for( map< Window *, vector< Window * > >::iterator it = clonedWindows.begin(); it != clonedWindows.end(); ++it )
  {
    for( vector< Window * >::iterator itWin = it->second.begin(); itWin != it->second.end(); ++itWin )
      delete *itWin;
  }
#endif
}


Window *WindowCloneManager::operator()( Window *originalWindow ) const
{
#ifdef PARALLEL_ENABLED
  map< Window *, vector< Window * > >::const_iterator it = clonedWindows.find( originalWindow );
  if ( it != clonedWindows.end() )
    return it->second[ omp_get_thread_num() ];
#endif

  return originalWindow;
}


#ifdef PARALLEL_ENABLED
void WindowCloneManager::update( const KHistogram *whichHistogram )
{
  Window *tmpWindow = whichHistogram->getControlWindow();
  if ( isClonable( tmpWindow ) )
  {
    clone( tmpWindow );
  }

  if ( tmpWindow != whichHistogram->getDataWindow() )
  {
    tmpWindow = whichHistogram->getDataWindow();
    if ( isClonable( tmpWindow ) )
    {
      clone( tmpWindow );
    }
  }

  if ( whichHistogram->getExtraControlWindow() != nullptr &&
       whichHistogram->getExtraControlWindow() != whichHistogram->getControlWindow() &&
       whichHistogram->getExtraControlWindow() != tmpWindow )
  {
    tmpWindow = whichHistogram->getExtraControlWindow();
    if ( isClonable( tmpWindow ) )
    {
      clone( tmpWindow );
    }
  }
}


void WindowCloneManager::clear()
{
  for( map< Window *, vector< Window * > >::iterator it = clonedWindows.begin(); it != clonedWindows.end(); ++it )
  {
    for( vector< Window * >::iterator itWin = it->second.begin(); itWin != it->second.end(); ++itWin )
      delete *itWin;
  }

  clonedWindows.clear();
}


bool WindowCloneManager::isClonable( Window *whichWindow )
{
  return whichWindow->isDerivedWindow() &&
         whichWindow->getTrace()->getLevelObjects( whichWindow->getParent( 0 )->getLevel() ) !=
         whichWindow->getTrace()->getLevelObjects( whichWindow->getParent( 1 )->getLevel() );
}


void WindowCloneManager::clone( Window *whichWindow )
{
  vector< Window * > tmpClones;

  for( int i = 0; i != omp_get_num_threads(); ++i )
    tmpClones.push_back( whichWindow->clone( true ) );

  clonedWindows[ whichWindow ] = tmpClones;
}
#endif // PARALLEL_ENABLED


RowsTranslator::RowsTranslator( const RowsTranslator& source )
{
  for ( size_t ii = 0; ii < source.childInfo.size() - 1; ii++ )
  {
    childInfo.push_back( RowChildInfo() );
    childInfo[ii].oneToOne = source.childInfo[ii].oneToOne;
    childInfo[ii].rowChildren = vector< pair< TObjectOrder, TObjectOrder > > ( source.childInfo[ii].rowChildren );
    childInfo[ii].numRows = source.childInfo[ii].numRows;
  }
}


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
      KTrace *auxTrace = ( KTrace* )kwindows[ii]->getTrace();
      for ( TObjectOrder iRow = 0; iRow < kwindows[ii]->getWindowLevelObjects(); iRow++ )
      {
        pair< TObjectOrder, TObjectOrder > range;

        range.first = auxTrace->getFirst( iRow,
                                          kwindows[ii]->getLevel(),
                                          kwindows[ii+1]->getLevel() );
        range.second =  auxTrace->getLast( iRow,
                                           kwindows[ii]->getLevel(),
                                           kwindows[ii+1]->getLevel() );
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


KHistogram::KHistogram()
{
  controlWindow = nullptr;
  dataWindow = nullptr;
  xtraControlWindow = nullptr;

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
  numCols = columnTranslator->totalColumns();
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
  columnTranslator = new ColumnTranslator( controlMin, controlMax, controlDelta );

  if ( planeTranslator != nullptr )
  {
    delete planeTranslator;
    planeTranslator = nullptr;
  }
  if ( getThreeDimensions() )
    planeTranslator = new ColumnTranslator( xtraControlMin, xtraControlMax,
                                            xtraControlDelta );
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
    cube = new Cube<TSemanticValue>( planes, cols, Statistics::getNumStats() );
    if ( createComms() )
      commCube = new Cube<TSemanticValue>( planes, rowsTranslator->totalRows(), Statistics::getNumCommStats() );
  }
  else
  {
    matrix = new Matrix<TSemanticValue>( cols, Statistics::getNumStats() );
    if ( createComms() )
      commMatrix = new Matrix<TSemanticValue>( rowsTranslator->totalRows(), Statistics::getNumCommStats() );
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
    totals = new KHistogramTotals( Statistics::getNumStats(), numCols, numPlanes );
    rowTotals = new KHistogramTotals( Statistics::getNumStats(), numRows, numPlanes );
    if ( createComms() )
    {
      commTotals = new KHistogramTotals( Statistics::getNumCommStats(),
                                         rowsTranslator->totalRows(), numPlanes );
      rowCommTotals = new KHistogramTotals( Statistics::getNumCommStats(),
                                            numRows, numPlanes );
    }
  }
  else
  {
    totals = new KHistogramTotals( Statistics::getNumStats(), numCols, 1 );
    rowTotals = new KHistogramTotals( Statistics::getNumStats(), numRows, 1 );
    if ( createComms() )
    {
      commTotals = new KHistogramTotals( Statistics::getNumCommStats(),
                                         rowsTranslator->totalRows(), 1 );
      rowCommTotals = new KHistogramTotals( Statistics::getNumCommStats(),
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
  semanticBuffer = new CubeBuffer( planes, rows );

  if ( commBuffer != nullptr )
  {
    delete commBuffer;
    commBuffer = nullptr;
  }
  commBuffer = new CubeBuffer( planes, rows );
#endif
}

#ifdef PARALLEL_ENABLED
void KHistogram::finishAllRows()
{
  vector<TSemanticValue> values;

  if ( getThreeDimensions() )
  {
    for ( TObjectOrder iRow = 0; iRow < rowsTranslator->totalRows(); ++iRow )
    {
      for ( THistogramColumn iPlane = 0; iPlane < planeTranslator->totalColumns();
            ++iPlane )
      {
        const map< THistogramColumn, vector< TSemanticValue > >& rowValues =
            commBuffer->getRowValues( iPlane, iRow );
        for ( map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = rowValues.begin();
              it != rowValues.end(); ++it )
        {
          values = it->second;
          commCube->setValue( iPlane, it->first, values );
          for ( PRV_UINT16 iStat = 0; iStat < values.size(); ++iStat )
          {
            commTotals->newValue( values[ iStat ], iStat, it->first, iPlane );
            rowCommTotals->newValue( values[ iStat ], iStat, iRow, iPlane );
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
      const map< THistogramColumn, vector< TSemanticValue > >& rowValues =
          commBuffer->getRowValues( 0, iRow );
      for ( map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = rowValues.begin();
            it != rowValues.end(); ++it )
      {
        values = it->second;
        commMatrix->setValue( it->first, values );
        for ( PRV_UINT16 iStat = 0; iStat < values.size(); ++iStat )
        {
          commTotals->newValue( values[ iStat ], iStat, it->first );
          rowCommTotals->newValue( values[ iStat ], iStat, iRow );
        }
      }

      commMatrix->newRow();
    }
  }

  if ( getThreeDimensions() )
  {
    for ( TObjectOrder iRow = 0; iRow < rowsTranslator->totalRows(); ++iRow )
    {
      for ( THistogramColumn iPlane = 0; iPlane < planeTranslator->totalColumns();
            ++iPlane )
      {
        const map< THistogramColumn, vector< TSemanticValue > >& rowValues =
            semanticBuffer->getRowValues( iPlane, iRow );
        for ( map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = rowValues.begin();
              it != rowValues.end(); ++it )
        {
          values = it->second;
          cube->setValue( iPlane, it->first, values );
          for ( PRV_UINT16 iStat = 0; iStat < values.size(); ++iStat )
          {
            totals->newValue( values[ iStat ], iStat, it->first, iPlane );
            rowTotals->newValue( values[ iStat ], iStat, iRow, iPlane );
          }
        }
      }
      cube->newRow();
    }
  }
  else
  {
    for ( TObjectOrder iRow = 0; iRow < rowsTranslator->totalRows(); ++iRow )
    {
      const map< THistogramColumn, vector< TSemanticValue > >& rowValues =
          semanticBuffer->getRowValues( 0, iRow );
      for ( map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = rowValues.begin();
            it != rowValues.end(); ++it )
      {
        values = it->second;
        matrix->setValue( it->first, values );
        for ( PRV_UINT16 iStat = 0; iStat < values.size(); ++iStat )
        {
          totals->newValue( values[ iStat ], iStat, it->first );
          rowTotals->newValue( values[ iStat ], iStat, iRow );
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
  Window *currentWindow = orderedWindows[ winIndex ];
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
          if( progressSteps == 1000 )
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
  Window *currentWindow = orderedWindows[ winIndex ];

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
    vector<TSemanticValue> values;
    RecordList::iterator itComm = data->rList->begin();
    while ( itComm != data->rList->end()/* &&
            itComm->getTime() >= getBeginTime() &&
            itComm->getTime() <= getEndTime()*/ )
    {
      if ( !( itComm->getType() & COMM ) ||
           !( itComm->getTime() >= getBeginTime() && itComm->getTime() <= getEndTime() )
         )
      {
        ++itComm;
        continue;
      }

      data->comm = itComm;
      filter = statistics.filterAllComm( data );
      values = statistics.executeAllComm( data );
      for ( PRV_UINT16 iStat = 0; iStat < filter.size(); iStat++ )
      {
        if ( filter[ iStat ] )
        {
          TObjectOrder column;
          if ( controlWindow->getLevel() >= WORKLOAD && controlWindow->getLevel() <= THREAD )
            column = controlWindow->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
          else if( controlWindow->getLevel() >= SYSTEM && controlWindow->getLevel() <= NODE )
            column = controlWindow->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
          else // CPU
            column = controlWindow->cpuObjectToWindowObject( data->comm->getCommPartnerObject() ) - 1;
#ifdef PARALLEL_ENABLED
          commBuffer->addValue( data->plane, data->row, column, values );
#else
          if ( getThreeDimensions() )
            commCube->addValue( data->plane, column, values );
          else
            commMatrix->addValue( column, values );
#endif
          break;
        }
      }

      ++itComm;
    }

    data->rList->erase( data->rList->begin(), itComm );

    if( !calcSemanticStats )
      return;

    // Semantic statistics
    if ( inclusive )
    {
      THistogramColumn column;
      vector<vector<TSemanticValue> > *tmp =
        controlWindow->getFirstSemUsefulFunction()->getStack();
      vector<TSemanticValue>::iterator it = ( *tmp )[ data->controlRow ].begin();
      while ( it != ( *tmp )[ data->controlRow ].end() )
      {
        if ( columnTranslator->getColumn( *it, column ) )
        {
          filter = statistics.filterAll( data );
          values = statistics.executeAll( data );

          for ( PRV_UINT16 iStat = 0; iStat < filter.size(); ++iStat )
          {
            if ( filter[ iStat ] )
            {
#ifdef PARALLEL_ENABLED
              semanticBuffer->addValue( data->plane, data->row, column, values );
#else
              if ( getThreeDimensions() )
                cube->addValue( data->plane, column, values );
              else
                matrix->addValue( column, values );
#endif

              break;
            }
          }
        }
        ++it;
      }
    }
    else
    {
      filter = statistics.filterAll( data );
      values = statistics.executeAll( data );

      for ( PRV_UINT16 iStat = 0; iStat < filter.size(); ++iStat )
      {
        if ( filter[ iStat ] )
        {
#ifdef PARALLEL_ENABLED
          semanticBuffer->addValue( data->plane, data->row, data->column, values );
#else
          if ( getThreeDimensions() )
            cube->addValue( data->plane, data->column, values );
          else
            matrix->addValue( data->column, values );
#endif
          break;
        }
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
  vector<TSemanticValue> values;

  if ( getThreeDimensions() )
  {
    for ( THistogramColumn iPlane = 0; iPlane < planeTranslator->totalColumns();
          ++iPlane )
    {
#ifdef PARALLEL_ENABLED
      const map< THistogramColumn, vector< TSemanticValue > >& rowValues =
          commBuffer->getRowValues( iPlane, data->row );
      for ( map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = rowValues.begin();
            it != rowValues.end(); ++it )
      {
        //values = it->second;
        values = statistics.finishRowAllComm( it->second, it->first, data->row, iPlane );
        commBuffer->setValue( iPlane, data->row, it->first, values );
      }
#else
      if ( commCube->planeWithValues( iPlane ) )
      {
        for ( TObjectOrder iColumn = 0;
              iColumn < rowsTranslator->totalRows(); ++iColumn )
        {
          if ( commCube->currentCellModified( iPlane, iColumn ) )
          {
            values = commCube->getCurrentValue( iPlane, iColumn );
            values = statistics.finishRowAllComm( values, iColumn, data->row, iPlane );

            commCube->setValue( iPlane, iColumn, values );
            for ( PRV_UINT16 iStat = 0; iStat < values.size(); ++iStat )
            {
              commTotals->newValue( values[ iStat ], iStat, iColumn, iPlane );
              rowCommTotals->newValue( values[ iStat ], iStat, data->row, iPlane );
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
    const map< THistogramColumn, vector< TSemanticValue > >& rowValues =
        commBuffer->getRowValues( 0, data->row );
    for ( map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = rowValues.begin();
          it != rowValues.end(); ++it )
    {
      //values = it->second;
      values = statistics.finishRowAllComm( it->second, it->first, data->row );
      commBuffer->setValue( 0, data->row, it->first, values );
    }
#else
    for ( TObjectOrder iColumn = 0;
          iColumn < rowsTranslator->totalRows(); ++iColumn )
    {
      if ( commMatrix->currentCellModified( iColumn ) )
      {
        values = commMatrix->getCurrentValue( iColumn );
        values = statistics.finishRowAllComm( values, iColumn, data->row );

        commMatrix->setValue( iColumn, values );
        for ( PRV_UINT16 iStat = 0; iStat < values.size(); ++iStat )
        {
          commTotals->newValue( values[ iStat ], iStat, iColumn );
          rowCommTotals->newValue( values[ iStat ], iStat, data->row );
        }
      }
    }
#endif
  }

#ifndef PARALLEL_ENABLED
  statistics.resetAllComm();
#endif

  if ( getThreeDimensions() )
  {
    for ( THistogramColumn iPlane = 0; iPlane < planeTranslator->totalColumns();
          ++iPlane )
    {
#ifdef PARALLEL_ENABLED
      const map< THistogramColumn, vector< TSemanticValue > >& rowValues =
          semanticBuffer->getRowValues( iPlane, data->row );
      for ( map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = rowValues.begin();
            it != rowValues.end(); ++it )
      {
        //values = it->second;
        values = statistics.finishRowAll( it->second, it->first, data->row, iPlane );
        semanticBuffer->setValue( iPlane, data->row, it->first, values );
      }
#else
      if ( cube->planeWithValues( iPlane ) )
      {
        for ( THistogramColumn iColumn = 0;
              iColumn < columnTranslator->totalColumns(); ++iColumn )
        {
          if ( cube->currentCellModified( iPlane, iColumn ) )
          {
            values = cube->getCurrentValue( iPlane, iColumn );
            values = statistics.finishRowAll( values, iColumn, data->row, iPlane );

            cube->setValue( iPlane, iColumn, values );
            for ( PRV_UINT16 iStat = 0; iStat < values.size(); ++iStat )
            {
              totals->newValue( values[ iStat ], iStat, iColumn, iPlane );
              rowTotals->newValue( values[ iStat ], iStat, data->row, iPlane );
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
    const map< THistogramColumn, vector< TSemanticValue > >& rowValues =
        semanticBuffer->getRowValues( 0, data->row );
    for ( map< THistogramColumn, vector< TSemanticValue > >::const_iterator it = rowValues.begin();
          it != rowValues.end(); ++it )
    {
      //values = it->second;
      values = statistics.finishRowAll( it->second, it->first, data->row );
      semanticBuffer->setValue( 0, data->row, it->first, values );
    }
#else
    for ( THistogramColumn iColumn = 0;
          iColumn < columnTranslator->totalColumns(); ++iColumn )
    {
      if ( matrix->currentCellModified( iColumn ) )
      {
        values = matrix->getCurrentValue( iColumn );
        values = statistics.finishRowAll( values, iColumn, data->row );
        matrix->setValue( iColumn, values );
        for ( PRV_UINT16 iStat = 0; iStat < values.size(); ++iStat )
        {
          totals->newValue( values[ iStat ], iStat, iColumn );
          rowTotals->newValue( values[ iStat ], iStat, data->row );
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
  HistogramStatistic *tmpFunction = FunctionManagement<HistogramStatistic>::getInstance()->getFunction( whichStat );

  bool retval = tmpFunction->createComms();
  delete tmpFunction;

  return retval;
}

string KHistogram::getUnitsLabel( const string& whichStat ) const
{
  HistogramStatistic *tmpFunction = FunctionManagement<HistogramStatistic>::getInstance()->getFunction( whichStat );

  string retval = tmpFunction->getUnits( this );
  delete tmpFunction;

  return retval;
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


Window *KHistogram::getClonedWindow( Window *whichWindow ) const
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
    clonedKHistogram->cube = new Cube<TSemanticValue>( *cube );
  if ( matrix != nullptr )
    clonedKHistogram->matrix = new Matrix<TSemanticValue>( *matrix );
  if ( commCube != nullptr )
    clonedKHistogram->commCube = new Cube<TSemanticValue>( *commCube );
  if ( commMatrix != nullptr )
    clonedKHistogram->commMatrix = new Matrix<TSemanticValue>( *commMatrix );

  clonedKHistogram->totals = new KHistogramTotals( totals );
  clonedKHistogram->rowTotals = new KHistogramTotals( rowTotals );
  clonedKHistogram->commTotals = new KHistogramTotals( commTotals );
  clonedKHistogram->rowCommTotals = new KHistogramTotals( rowCommTotals );

  clonedKHistogram->orderWindows();

  return clonedKHistogram;
}
