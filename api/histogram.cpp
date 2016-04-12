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

#include <string>
#include <cmath>
#include "kernelconnection.h"
#include "histogram.h"
#include "window.h"
#include "histogramtotals.h"
#include "paraverconfig.h"
#include "labelconstructor.h"
#include "loadedwindows.h"
#include <iostream>

#ifdef WIN32
#undef min
#undef max
#endif

using namespace std;

Histogram *Histogram::create( KernelConnection *whichKernel )
{
  return new HistogramProxy( whichKernel );
}


Histogram::Histogram( KernelConnection *whichKernel ) : myKernel( whichKernel )
{}

HistogramProxy::HistogramProxy( KernelConnection *whichKernel ):
  Histogram( whichKernel )
{
  destroy = false;

  name = Histogram::getName();
  number_of_clones = 0;

  myTrace = NULL;
  controlWindow = NULL;
  dataWindow = NULL;
  extraControlWindow = NULL;
  myHisto = myKernel->newHistogram();

  width = Histogram::getWidth();
  height = Histogram::getHeight();
  horizontal = Histogram::getHorizontal();
  hideColumns = Histogram::getHideColumns();
  scientificNotation = Histogram::getScientificNotation();
  numDecimals = Histogram::getNumDecimals();
  thousandSep = Histogram::getThousandSeparator();
  showUnits = ParaverConfig::getInstance()->getHistogramShowUnits();
  sortColumns = Histogram::getSortColumns();
  sortCriteria = Histogram::getSortCriteria();
  minGradient = Histogram::getMinGradient();
  maxGradient = Histogram::getMaxGradient();
  computeControlScale = Histogram::getCompute2DScale();
  computeXtraScale = Histogram::getCompute3DScale();
  computeGradient = Histogram::getComputeGradient();
  showColor = Histogram::getShowColor();
  zoom = Histogram::getZoom();
  firstRowColored = Histogram::getFirstRowColored();
  futurePlane = false;
  planeMinValue = 0.0;
  selectedPlane = 0;
  commSelectedPlane = 0;
  drawModeObjects = Histogram::getDrawModeObjects();
  drawModeColumns = Histogram::getDrawModeColumns();
  myGradientColor.setGradientFunction( ParaverConfig::getInstance()->getHistogramGradientFunction() );
  if( ParaverConfig::getInstance()->getHistogramPixelSize() >= 0 &&
      ParaverConfig::getInstance()->getHistogramPixelSize() <= 3 )
    pixelSize = (PRV_UINT16)pow( float(2), (int)ParaverConfig::getInstance()->getHistogramPixelSize() );
  else
    pixelSize = ParaverConfig::getInstance()->getHistogramPixelSize();

  setCalculateAll( Histogram::getCalculateAll() );
  currentStat = Histogram::getCurrentStat();

  showWindow = true;
  changed = false;
  redraw = false;
  recalc = false;

  codeColor = Histogram::getCodeColor();

  zoomHistory.clear();

  isCFG4DEnabled = false;
  CFG4DMode = false;
}

HistogramProxy::~HistogramProxy()
{
  if( controlWindow != NULL )
    controlWindow->setUsedByHistogram( false );
  if( dataWindow != NULL )
    dataWindow->setUsedByHistogram( false );
  if( extraControlWindow != NULL )
    extraControlWindow->setUsedByHistogram( false );

  LoadedWindows::getInstance()->eraseHisto( this );
  delete myHisto;
}

void HistogramProxy::setWindowBeginTime( TRecordTime whichTime )
{
  winBeginTime = whichTime;
}

void HistogramProxy::setWindowEndTime( TRecordTime whichTime )
{
  winEndTime = whichTime;
}

bool HistogramProxy::getThreeDimensions() const
{
  return myHisto->getThreeDimensions();
}

TRecordTime HistogramProxy::getBeginTime() const
{
  return winBeginTime;
}

TRecordTime HistogramProxy::getEndTime() const
{
  return winEndTime;
}

Trace *HistogramProxy::getTrace() const
{
  return myTrace;
}

Window *HistogramProxy::getControlWindow() const
{
  return controlWindow;
}

Window *HistogramProxy::getDataWindow() const
{
  return dataWindow;
}

Window *HistogramProxy::getExtraControlWindow() const
{
  return extraControlWindow;
}

void HistogramProxy::setControlWindow( Window *whichWindow )
{
  if( controlWindow != NULL )
    controlWindow->setUsedByHistogram( false );
  controlWindow = whichWindow;
  controlWindow->setUsedByHistogram( true );
  myHisto->setControlWindow( whichWindow->getConcrete() );
  myTrace = controlWindow->getTrace();
}

void HistogramProxy::setDataWindow( Window *whichWindow )
{
  if( dataWindow != NULL )
    dataWindow->setUsedByHistogram( false );
  dataWindow = whichWindow;
  dataWindow->setUsedByHistogram( true );
  myHisto->setDataWindow( whichWindow->getConcrete() );
}

void HistogramProxy::setExtraControlWindow( Window *whichWindow )
{
  if( extraControlWindow != NULL )
    extraControlWindow->setUsedByHistogram( false );
  extraControlWindow = whichWindow;
  extraControlWindow->setUsedByHistogram( true );
  myHisto->setExtraControlWindow( whichWindow->getConcrete() );
}

void HistogramProxy::clearControlWindow()
{
  if( controlWindow != NULL )
    controlWindow->setUsedByHistogram( false );
  controlWindow = NULL;
  myHisto->clearControlWindow();
}

void HistogramProxy::clearDataWindow()
{
  if( dataWindow != NULL )
    dataWindow->setUsedByHistogram( false );
  dataWindow = NULL;
  myHisto->clearDataWindow();
}

void HistogramProxy::clearExtraControlWindow()
{
  if( extraControlWindow != NULL )
    extraControlWindow->setUsedByHistogram( false );
  extraControlWindow = NULL;
  selectedPlane = 0;
  commSelectedPlane = 0;
  myHisto->clearExtraControlWindow();
}

void HistogramProxy::setControlMin( THistogramLimit whichMin )
{
  myHisto->setControlMin( whichMin );
}

void HistogramProxy::setControlMax( THistogramLimit whichMax )
{
  myHisto->setControlMax( whichMax );
}

void HistogramProxy::setControlDelta( THistogramLimit whichDelta )
{
  myHisto->setControlDelta( whichDelta );
}

void HistogramProxy::setExtraControlMin( THistogramLimit whichMin )
{
  myHisto->setExtraControlMin( whichMin );
}

void HistogramProxy::setExtraControlMax( THistogramLimit whichMax )
{
  myHisto->setExtraControlMax( whichMax );
}

void HistogramProxy::setExtraControlDelta( THistogramLimit whichDelta )
{
  myHisto->setExtraControlDelta( whichDelta );
}

void HistogramProxy::setDataMin( TSemanticValue whichMin )
{
  myHisto->setDataMin( whichMin );
}

void HistogramProxy::setDataMax( TSemanticValue whichMax )
{
  myHisto->setDataMax( whichMax );
}

void HistogramProxy::setBurstMin( TRecordTime whichTime )
{
  myHisto->setBurstMin( whichTime );
}

void HistogramProxy::setBurstMax( TRecordTime whichTime )
{
  myHisto->setBurstMax( whichTime );
}

void HistogramProxy::setCommSizeMin( TCommSize whichSize )
{
  myHisto->setCommSizeMin( whichSize );
}

void HistogramProxy::setCommSizeMax( TCommSize whichSize )
{
  myHisto->setCommSizeMax( whichSize );
}

void HistogramProxy::setCommTagMin( TCommTag whichTag )
{
  myHisto->setCommTagMin( whichTag );
}

void HistogramProxy::setCommTagMax( TCommTag whichTag )
{
  myHisto->setCommTagMax( whichTag );
}

THistogramLimit HistogramProxy::getControlMin() const
{
  return myHisto->getControlMin();
}

THistogramLimit HistogramProxy::getControlMax() const
{
  return myHisto->getControlMax();
}

THistogramLimit HistogramProxy::getControlDelta() const
{
  return myHisto->getControlDelta();
}

THistogramLimit HistogramProxy::getExtraControlMin() const
{
  return myHisto->getExtraControlMin();
}

THistogramLimit HistogramProxy::getExtraControlMax() const
{
  return myHisto->getExtraControlMax();
}

THistogramLimit HistogramProxy::getExtraControlDelta() const
{
  return myHisto->getExtraControlDelta();
}

TSemanticValue HistogramProxy::getDataMin() const
{
  return myHisto->getDataMin();
}

TSemanticValue HistogramProxy::getDataMax() const
{
  return myHisto->getDataMax();
}

TRecordTime HistogramProxy::getBurstMin() const
{
  return myHisto->getBurstMin();
}

TRecordTime HistogramProxy::getBurstMax() const
{
  return myHisto->getBurstMax();
}

TCommSize HistogramProxy::getCommSizeMin() const
{
  return myHisto->getCommSizeMin();
}

TCommSize HistogramProxy::getCommSizeMax() const
{
  return myHisto->getCommSizeMax();
}

TCommTag HistogramProxy::getCommTagMin() const
{
  return myHisto->getCommTagMin();
}

TCommTag HistogramProxy::getCommTagMax() const
{
  return myHisto->getCommTagMax();
}

bool HistogramProxy::getInclusiveEnabled() const
{
  return myHisto->getInclusiveEnabled();
}

void HistogramProxy::setInclusive( bool newValue )
{
  myHisto->setInclusive( newValue );
}

bool HistogramProxy::getInclusive() const
{
  return myHisto->getInclusive();
}

THistogramColumn HistogramProxy::getNumPlanes() const
{
  return myHisto->getNumPlanes();
}

THistogramColumn HistogramProxy::getNumColumns( const string& whichStat ) const
{
  if ( itsCommunicationStat( whichStat ) )
    return ( THistogramColumn ) getNumRows();

  return getNumColumns();
}

THistogramColumn HistogramProxy::getNumColumns() const
{
  return myHisto->getNumColumns();
}

TObjectOrder HistogramProxy::getNumRows() const
{
  return myHisto->getNumRows();
}


TSemanticValue HistogramProxy::getCurrentValue( PRV_UINT32 col,
    PRV_UINT16 idStat,
    PRV_UINT32 plane ) const
{
  return myHisto->getCurrentValue( col, idStat, plane );
}

PRV_UINT32 HistogramProxy::getCurrentRow( PRV_UINT32 col, PRV_UINT32 plane ) const
{
  return myHisto->getCurrentRow( col, plane );
}

void HistogramProxy::setNextCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  myHisto->setNextCell( col, plane );
}

void HistogramProxy::setFirstCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  myHisto->setFirstCell( col, plane );
}

bool HistogramProxy::endCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  return myHisto->endCell( col, plane );
}

bool HistogramProxy::planeWithValues( PRV_UINT32 plane ) const
{
  return myHisto->planeWithValues( plane );
}

bool HistogramProxy::getCellValue( TSemanticValue& semVal,
                                   PRV_UINT32 whichRow,
                                   PRV_UINT32 whichCol,
                                   PRV_UINT16 idStat,
                                   PRV_UINT32 whichPlane ) const
{
  return myHisto->getCellValue( semVal, whichRow, whichCol, idStat, whichPlane );
}

TSemanticValue HistogramProxy::getCommCurrentValue( PRV_UINT32 col,
    PRV_UINT16 idStat,
    PRV_UINT32 plane ) const
{
  return myHisto->getCommCurrentValue( col, idStat, plane );
}

PRV_UINT32 HistogramProxy::getCommCurrentRow( PRV_UINT32 col, PRV_UINT32 plane ) const
{
  return myHisto->getCommCurrentRow( col, plane );
}

void HistogramProxy::setCommNextCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  myHisto->setCommNextCell( col, plane );
}

void HistogramProxy::setCommFirstCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  myHisto->setCommFirstCell( col, plane );
}

bool HistogramProxy::endCommCell( PRV_UINT32 col, PRV_UINT32 plane )
{
  return myHisto->endCommCell( col, plane );
}

bool HistogramProxy::planeCommWithValues( PRV_UINT32 plane ) const
{
  return myHisto->planeCommWithValues( plane );
}

bool HistogramProxy::getCommCellValue( TSemanticValue& semVal,
                                       PRV_UINT32 whichRow,
                                       PRV_UINT32 whichCol,
                                       PRV_UINT16 idStat,
                                       PRV_UINT32 whichPlane ) const
{
  return myHisto->getCommCellValue( semVal, whichRow, whichCol, idStat, whichPlane );
}

HistogramTotals *HistogramProxy::getTotals( const string& whichStat ) const
{
  if ( itsCommunicationStat( whichStat ) )
  {
    if ( horizontal )
      return getCommColumnTotals();
    else
      return getCommRowTotals();
  }
  else
  {
    if ( horizontal )
      return getColumnTotals();
    else
      return getRowTotals();
  }
}

HistogramTotals *HistogramProxy::getColumnTotals() const
{
  return HistogramTotals::create( myHisto->getColumnTotals() );
}

HistogramTotals *HistogramProxy::getCommColumnTotals() const
{
  return HistogramTotals::create( myHisto->getCommColumnTotals() );
}

HistogramTotals *HistogramProxy::getRowTotals() const
{
  return HistogramTotals::create( myHisto->getRowTotals() );
}

HistogramTotals *HistogramProxy::getCommRowTotals() const
{
  return HistogramTotals::create( myHisto->getCommRowTotals() );
}

void HistogramProxy::clearStatistics()
{
  myHisto->clearStatistics();
  calcStat.clear();
  commCalcStat.clear();
}

void HistogramProxy::pushbackStatistic( const string& whichStatistic )
{
  myHisto->pushbackStatistic( whichStatistic );
  if ( itsCommunicationStat( whichStatistic ) )
    commCalcStat.push_back( whichStatistic );
  else
    calcStat.push_back( whichStatistic );
}

void HistogramProxy::execute( TRecordTime whichBeginTime, TRecordTime whichEndTime,
                              vector<TObjectOrder>& selectedRows, ProgressController *progress )
{
  winBeginTime = whichBeginTime;
  winEndTime = whichEndTime;

#ifdef PARALLEL_ENABLED
  progress = NULL;
#endif // PARALLEL_ENABLED

  if ( computeControlScale )
    compute2DScale( progress );

  if ( getThreeDimensions() && computeXtraScale )
    compute3DScale( progress );

  myHisto->execute( whichBeginTime, whichEndTime, selectedRows, progress );

  if ( getThreeDimensions() && futurePlane )
  {
    THistogramLimit nPlanes = getNumPlanes();
    THistogramLimit min, max;
    PRV_UINT32 i;
    bool commFuturePlane = true;
    i = selectedPlane = commSelectedPlane = 0;

    while ( i < nPlanes )
    {
      min = getExtraControlMin() + getExtraControlDelta() * i;
      max = getExtraControlMin() + getExtraControlDelta() * i + 1;

      // get the first plane with values
      if ( futurePlane && planeWithValues( i ) )
      {
        selectedPlane = i;
        futurePlane = false;
      }

      if ( commFuturePlane && planeCommWithValues( i ) )
      {
        commSelectedPlane = i;
        commFuturePlane = false;
      }

      if ( planeMinValue >= min && planeMinValue < max )
      {
        if ( planeWithValues( i ) )
          selectedPlane = i;

        if ( planeCommWithValues( i ) )
          commSelectedPlane = i;
      }

      i++;
    }
    futurePlane = false;
  }
  else if ( getThreeDimensions() )
  {
    THistogramLimit nPlanes = getNumPlanes();
    PRV_UINT32 i;

    if( selectedPlane >= nPlanes )
      selectedPlane = 0;

    if ( !planeWithValues( selectedPlane ) )
    {
      i = selectedPlane = 0;
      while ( i < nPlanes )
      {
        // get the first plane with values
        if ( /*futurePlane &&*/ planeWithValues( i ) )
        {
          selectedPlane = i;
          break;
        }
        i++;
      }
    }

    if ( !planeCommWithValues( commSelectedPlane ) )
    {
      i = commSelectedPlane = 0;
      while ( i < nPlanes )
      {
        if ( /*commFuturePlane &&*/ planeCommWithValues( i ) )
        {
          commSelectedPlane = i;
          break;
        }
        i++;
      }
    }
  }
}

void HistogramProxy::setHorizontal( bool newValue )
{
  horizontal = newValue;
}

bool HistogramProxy::getHorizontal() const
{
  return horizontal;
}

void HistogramProxy::setHideColumns( bool newValue )
{
  hideColumns = newValue;
}

bool HistogramProxy::getHideColumns() const
{
  return hideColumns;
}

void HistogramProxy::setScientificNotation( bool newValue )
{
  scientificNotation = newValue;
}

bool HistogramProxy::getScientificNotation() const
{
  return scientificNotation;
}

void HistogramProxy::setNumDecimals( PRV_UINT16 newValue )
{
  numDecimals = newValue;
}

PRV_UINT16 HistogramProxy::getNumDecimals() const
{
  return numDecimals;
}

void HistogramProxy::setThousandSeparator( bool newValue )
{
  thousandSep = newValue;
}

bool HistogramProxy::getThousandSeparator() const
{
  return thousandSep;
}

void HistogramProxy::setShowUnits( bool newValue )
{
  showUnits = newValue;
}

bool HistogramProxy::getShowUnits() const
{
  return showUnits;
}

void HistogramProxy::setSortColumns( bool newValue )
{
  sortColumns = newValue;
}

bool HistogramProxy::getSortColumns() const
{
  return sortColumns;
}

void HistogramProxy::setSortCriteria( THistoTotals whichCriteria )
{
  sortCriteria = whichCriteria;
}

THistoTotals HistogramProxy::getSortCriteria() const
{
  return sortCriteria;
}

void HistogramProxy::setMinGradient( double whichMin )
{
  minGradient = whichMin;
}

double HistogramProxy::getMinGradient() const
{
  return minGradient;
}

void HistogramProxy::setMaxGradient( double whichMax )
{
  maxGradient = whichMax;
}

double HistogramProxy::getMaxGradient() const
{
  return maxGradient;
}

void HistogramProxy::setComputeScale( bool newValue )
{
  computeControlScale = newValue;
  computeXtraScale = newValue;
}

bool HistogramProxy::getComputeScale() const
{
  return computeControlScale && computeXtraScale;
}

void HistogramProxy::setCompute2DScale( bool newValue )
{
  computeControlScale = newValue;
}

bool HistogramProxy::getCompute2DScale() const
{
  return computeControlScale;
}

void HistogramProxy::setCompute3DScale( bool newValue )
{
  computeXtraScale = newValue;
}

bool HistogramProxy::getCompute3DScale() const
{
  return computeXtraScale;
}

void HistogramProxy::setComputeGradient( bool newValue )
{
  computeGradient = newValue;
}

bool HistogramProxy::getComputeGradient() const
{
  return computeGradient;
}

void HistogramProxy::setShowColor( bool newValue )
{
  showColor = newValue;
}

bool HistogramProxy::getShowColor() const
{
  return showColor;
}

rgb HistogramProxy::calcGradientColor( TSemanticValue whichValue ) const
{
  return myGradientColor.calcColor( whichValue, minGradient, maxGradient );
}

GradientColor& HistogramProxy::getGradientColor()
{
  return myGradientColor;
}

void HistogramProxy::recalcGradientLimits()
{
  TSemanticValue tmpMin = std::numeric_limits<TSemanticValue>::max();
  TSemanticValue tmpMax = 0.0;
  HistogramTotals *totals = NULL;
  PRV_UINT32 plane;
  PRV_UINT16 idStat;
  THistogramColumn numColumns = getNumColumns( getCurrentStat() );

  getIdStat( getCurrentStat(), idStat );

  if ( itsCommunicationStat( getCurrentStat() ) )
  {
    totals = getCommColumnTotals();
    plane = getCommSelectedPlane();
  }
  else
  {
    totals = getColumnTotals();
    plane = getSelectedPlane();
  }

  if ( !planeWithValues( plane ) )
  {
    minGradient = 0.0;
    maxGradient = std::numeric_limits<TSemanticValue>::max();
    delete totals;
    return;
  }

  for ( THistogramColumn iCol = 0; iCol < numColumns; ++iCol )
  {
    TSemanticValue curMin = totals->getMinimum( idStat, iCol, plane );
    TSemanticValue curMax = totals->getMaximum( idStat, iCol, plane );
    if ( curMin != 0 && curMin < tmpMin )
      tmpMin = curMin;
    if ( curMax > tmpMax )
      tmpMax = curMax;
  }

  if( tmpMin == std::numeric_limits<TSemanticValue>::max() )
    minGradient = 0;
  else
    minGradient = tmpMin;
  maxGradient = tmpMax;

  delete totals;
}

void HistogramProxy::setZoom( bool newValue )
{
  zoom = newValue;
}

bool HistogramProxy::getZoom() const
{
  return zoom;
}

void HistogramProxy::setFirstRowColored( bool newValue )
{
  firstRowColored = newValue;
}

bool HistogramProxy::getFirstRowColored() const
{
  return firstRowColored;
}

void HistogramProxy::setPlaneMinValue( double whichMin )
{
  planeMinValue = whichMin;
  futurePlane = true;
}

double HistogramProxy::getPlaneMinValue() const
{
  if( !futurePlane )
    return getExtraControlDelta() * getSelectedPlane() + getExtraControlMin();
  return planeMinValue;
}

void HistogramProxy::setSelectedPlane( PRV_INT32 plane )
{
  selectedPlane = plane;
}

PRV_INT32 HistogramProxy::getSelectedPlane() const
{
  return selectedPlane;
}

PRV_INT32 HistogramProxy::getCommSelectedPlane() const
{
  return commSelectedPlane;
}

bool HistogramProxy::itsCommunicationStat( const string& whichStat ) const
{
  return myHisto->itsCommunicationStat( whichStat );
}

void HistogramProxy::compute2DScale( ProgressController *progress )
{
  TSemanticValue tmpMinY = controlWindow->getMinimumY();
  TSemanticValue tmpMaxY = controlWindow->getMaximumY();
  TRecordTime tmpBeginTime = controlWindow->getWindowBeginTime();
  TRecordTime tmpEndTime = controlWindow->getWindowEndTime();
  controlWindow->setWindowBeginTime( getBeginTime(), true );
  controlWindow->setWindowEndTime( getEndTime(), true );
  controlWindow->computeYScale( progress );
  controlWindow->setWindowBeginTime( tmpBeginTime, true );
  controlWindow->setWindowEndTime( tmpEndTime, true );
  TSemanticValue minY = controlWindow->getMinimumY();
  TSemanticValue maxY = controlWindow->getMaximumY();
  controlWindow->setMinimumY( tmpMinY );
  controlWindow->setMaximumY( tmpMaxY );

  setControlMin( minY );
  setControlMax( maxY + ( ( maxY - minY ) * 0.05 ) );

  if ( ( maxY - minY ) == 0 )
  {
    setControlDelta( 1.0 );
  }
  else if ( controlWindow->isCodeColorSet() && ( maxY - minY ) <= 10000 )
  {
    setControlMax( maxY + ( ( maxY - minY ) * 0.05 ) );
    setControlDelta( 1.0 );
  }
  else
  {
    setControlDelta( ( maxY + ( ( maxY - minY ) * 0.05 ) - minY ) /
                     ParaverConfig::getInstance()->getHistogramNumColumns() );
  }
}

void HistogramProxy::compute3DScale( ProgressController *progress )
{
  TSemanticValue tmpMinY = extraControlWindow->getMinimumY();
  TSemanticValue tmpMaxY = extraControlWindow->getMaximumY();
  TRecordTime tmpBeginTime = extraControlWindow->getWindowBeginTime();
  TRecordTime tmpEndTime = extraControlWindow->getWindowEndTime();
  extraControlWindow->setWindowBeginTime( getBeginTime(), true );
  extraControlWindow->setWindowEndTime( getEndTime(), true );
  extraControlWindow->computeYScale( progress );
  extraControlWindow->setWindowBeginTime( tmpBeginTime, true );
  extraControlWindow->setWindowEndTime( tmpEndTime, true );
  TSemanticValue minY = extraControlWindow->getMinimumY();
  TSemanticValue maxY = extraControlWindow->getMaximumY();
  extraControlWindow->setMinimumY( tmpMinY );
  extraControlWindow->setMaximumY( tmpMaxY );

  setExtraControlMin( minY );
  setExtraControlMax( maxY );

  if ( ( maxY - minY ) == 0 )
  {
    setExtraControlDelta( 1.0 );
  }
  else if( extraControlWindow->isCodeColorSet() )
  {
    setExtraControlDelta( 1.0 );
  }
  else
  {
    setExtraControlDelta( ( maxY - minY ) /
                          ParaverConfig::getInstance()->getHistogramNumColumns() );
  }
}

string HistogramProxy::getRowLabel( TObjectOrder whichRow ) const
{
  if( controlWindow == NULL )
    return "";

  if( controlWindow->getLevel() == CPU || controlWindow->getLevel() == NODE )
    ++whichRow;

  return LabelConstructor::objectLabel( whichRow, controlWindow->getLevel(),
                                        controlWindow->getTrace() );
}

string HistogramProxy::getColumnLabel( THistogramColumn whichColumn ) const
{
  if( controlWindow == NULL )
    return "";

  if( itsCommunicationStat( getCurrentStat() ) )
    return getRowLabel( ( TObjectOrder ) whichColumn );

  return LabelConstructor::histoColumnLabel( whichColumn, controlWindow,
         getControlMin(),
         getControlMax(),
         getControlDelta() );
}

string HistogramProxy::getPlaneLabel( THistogramColumn whichPlane ) const
{
  Window *win = ( extraControlWindow != NULL ) ? extraControlWindow : controlWindow;
  if( win == NULL )
    return "";

  return LabelConstructor::histoColumnLabel( whichPlane, win,
         getExtraControlMin(),
         getExtraControlMax(),
         getExtraControlDelta() );
}

THistogramColumn HistogramProxy::getPlaneColumns( THistogramColumn iPlane,
    bool hideEmptyColumns, // need to override cfg value
    vector<THistogramColumn> &columns ) const
{
  THistogramColumn numColumns = 0;

  PRV_UINT16 idStat;

  if( getIdStat( getCurrentStat(), idStat ) )
  {
    bool commStat = itsCommunicationStat( getCurrentStat() );
    numColumns = getNumColumns( getCurrentStat() );

    if ( hideEmptyColumns )
    {
      // Return only the columns with values
      SelectionManagement<THistogramColumn, int> columnSelection;

      if( commStat )
        columnSelection.init( getCommColumnTotals(), idStat, numColumns, iPlane );
      else
        columnSelection.init( getColumnTotals(), idStat, numColumns, iPlane );

      columnSelection.getSelected( columns );

      numColumns = columns.size();
    }
    else
    {
      // return all the columns ??
      for( THistogramColumn iColumn = 0; iColumn < numColumns; ++iColumn )
        columns.push_back( iColumn );
    }
  }

  return numColumns;
}

TTimeUnit HistogramProxy::getTimeUnit() const
{
  return myHisto->getTimeUnit();
}

bool HistogramProxy::isZoomEmpty() const
{
  return zoomHistory.isEmpty();
}

bool HistogramProxy::emptyPrevZoom() const
{
  return zoomHistory.isEmpty( PREV_ZOOM );
}

bool HistogramProxy::emptyNextZoom() const
{
  return zoomHistory.isEmpty( NEXT_ZOOM );
}

void HistogramProxy::nextZoom()
{
  zoomHistory.nextZoom();
}

void HistogramProxy::prevZoom()
{
  zoomHistory.prevZoom();
}

void HistogramProxy::addZoom( TZoomInfo columnsInfo, TZoomInfo dummy,
                              TObjectOrder beginObject, TObjectOrder endObject )
{
  zoomHistory.addZoom( columnsInfo, dummy, beginObject, endObject );
}

void HistogramProxy::addZoom( TZoomInfo columnsInfo, TZoomInfo dummy )
{
  zoomHistory.addZoom( columnsInfo, dummy );
}

void HistogramProxy::setZoomFirstDimension( pair<TZoomInfo, TZoomInfo> &zinfo )
{
  zoomHistory.setFirstDimension( zinfo );
}

void HistogramProxy::setZoomSecondDimension( pair<TObjectOrder, TObjectOrder> &objects )
{
  zoomHistory.setSecondDimension( objects );
}

pair<HistogramProxy::TZoomInfo, HistogramProxy::TZoomInfo> HistogramProxy::getZoomFirstDimension() const
{
  return zoomHistory.getFirstDimension();
}

pair <TObjectOrder, TObjectOrder> HistogramProxy::getZoomSecondDimension() const
{
  return zoomHistory.getSecondDimension();
}

void HistogramProxy::setName( const string& whichName )
{
  name = whichName;
}

string HistogramProxy::getName() const
{
  return name;
}

void HistogramProxy::setCalculateAll( bool status )
{
  calculateAll = status;
  clearStatistics();
  if ( status )
  {
    vector<string> vStat;
    myKernel->getAllStatistics( vStat );
    for ( vector<string>::iterator it = vStat.begin(); it != vStat.end(); ++it )
      pushbackStatistic( *it );
  }
  else
    pushbackStatistic( currentStat );
}

bool HistogramProxy::getCalculateAll() const
{
  return calculateAll;
}

bool HistogramProxy::getIdStat( const string& whichStat, PRV_UINT16& idStat ) const
{
  idStat = 0;
  const vector<string> *vStat;

  if ( itsCommunicationStat( whichStat ) )
    vStat = &commCalcStat;
  else
    vStat = &calcStat;

  if ( vStat->begin() == vStat->end() )
    return false;

  for ( vector<string>::const_iterator it = vStat->begin(); it != vStat->end(); ++it )
  {
    if ( whichStat.compare( *it ) == 0 )
      return true;

    idStat++;
  }

  return false;
}

void HistogramProxy::setCurrentStat( const string& whichStat )
{
  currentStat = whichStat;
  if ( !calculateAll )
  {
    clearStatistics();
    pushbackStatistic( whichStat );
  }
}

string HistogramProxy::getCurrentStat() const
{
  return currentStat;
}

string HistogramProxy::getUnitsLabel( const string& whichStat ) const
{
  return myHisto->getUnitsLabel( whichStat );
}

Histogram *HistogramProxy::clone()
{
  HistogramProxy *clonedHistogramProxy = new HistogramProxy( myKernel );

  delete clonedHistogramProxy->myHisto;
  clonedHistogramProxy->myHisto = myHisto->clone();

  std::ostringstream tmp;
  tmp << ++number_of_clones;
  clonedHistogramProxy->name = name + ".c" + tmp.str();

  clonedHistogramProxy->posX = posX;
  clonedHistogramProxy->posY = posY;
  clonedHistogramProxy->width = width;
  clonedHistogramProxy->height = height;

  clonedHistogramProxy->horizontal = horizontal;
  clonedHistogramProxy->hideColumns = hideColumns;
  clonedHistogramProxy->scientificNotation = scientificNotation;
  clonedHistogramProxy->numDecimals = numDecimals;
  clonedHistogramProxy->thousandSep = thousandSep;
  clonedHistogramProxy->showUnits = showUnits;
  clonedHistogramProxy->sortColumns = sortColumns;
  clonedHistogramProxy->sortCriteria = sortCriteria;
  clonedHistogramProxy->minGradient = minGradient;
  clonedHistogramProxy->maxGradient = maxGradient;
  clonedHistogramProxy->computeControlScale = computeControlScale;
  clonedHistogramProxy->computeXtraScale = computeXtraScale;
  clonedHistogramProxy->computeGradient = computeGradient;
  clonedHistogramProxy->showColor = showColor;
  clonedHistogramProxy->zoom = zoom;
  clonedHistogramProxy->firstRowColored = firstRowColored;
  clonedHistogramProxy->codeColor = codeColor;
  clonedHistogramProxy->pixelSize = pixelSize;
  clonedHistogramProxy->futurePlane = futurePlane;
  clonedHistogramProxy->planeMinValue = planeMinValue;
  clonedHistogramProxy->selectedPlane = selectedPlane;
  clonedHistogramProxy->commSelectedPlane = commSelectedPlane;
  clonedHistogramProxy->showWindow = showWindow;
  clonedHistogramProxy->changed = changed;
  clonedHistogramProxy->redraw = redraw;
  clonedHistogramProxy->recalc = recalc;
  clonedHistogramProxy->drawModeColumns = drawModeColumns;
  clonedHistogramProxy->drawModeObjects = drawModeObjects;

  clonedHistogramProxy->winBeginTime = winBeginTime;
  clonedHistogramProxy->winEndTime = winEndTime;

  // Must store the associated proxies
  clonedHistogramProxy->setControlWindow( controlWindow->clone() );
  clonedHistogramProxy->getControlWindow()->setUsedByHistogram( true );
  if( controlWindow == dataWindow )
    clonedHistogramProxy->setDataWindow( clonedHistogramProxy->getControlWindow() );
  else
  {
    clonedHistogramProxy->setDataWindow( dataWindow->clone() );
    clonedHistogramProxy->getDataWindow()->setUsedByHistogram( true );
  }
  if ( extraControlWindow != NULL )
  {
    if( extraControlWindow == controlWindow )
      clonedHistogramProxy->setExtraControlWindow( clonedHistogramProxy->getControlWindow() );
    else if( extraControlWindow == dataWindow )
      clonedHistogramProxy->setExtraControlWindow( clonedHistogramProxy->getDataWindow() );
    else
    {
      clonedHistogramProxy->setExtraControlWindow( extraControlWindow->clone() );
      clonedHistogramProxy->getExtraControlWindow()->setUsedByHistogram( true );
    }
  }

  clonedHistogramProxy->calculateAll = calculateAll;
  clonedHistogramProxy->currentStat = currentStat;
  clonedHistogramProxy->calcStat = vector<string>( calcStat );
  clonedHistogramProxy->commCalcStat = vector<string>( commCalcStat );

  myGradientColor.copy( clonedHistogramProxy->myGradientColor );

  // CFG4D
  clonedHistogramProxy->isCFG4DEnabled = isCFG4DEnabled;
  clonedHistogramProxy->CFG4DMode = CFG4DMode;

  clonedHistogramProxy->propertiesAliasCFG4D = propertiesAliasCFG4D;
  clonedHistogramProxy->statisticsAliasCFG4D = statisticsAliasCFG4D;

  return clonedHistogramProxy;
}


bool HistogramProxy::getDestroy() const
{
  return destroy;
}


void HistogramProxy::setDestroy( bool newValue )
{
  destroy = newValue;
}


PRV_UINT16 HistogramProxy::getPosX() const
{
  return posX;
}

void HistogramProxy::setPosX( PRV_UINT16 whichPos )
{
  posX = whichPos;
}

PRV_UINT16 HistogramProxy::getPosY() const
{
  return posY;
}

void HistogramProxy::setPosY( PRV_UINT16 whichPos )
{
  posY = whichPos;
}

PRV_UINT16 HistogramProxy::getWidth() const
{
  return width;
}

void HistogramProxy::setWidth( PRV_UINT16 whichPos )
{
  width = whichPos;
}

PRV_UINT16 HistogramProxy::getHeight() const
{
  return height;
}

void HistogramProxy::setHeight( PRV_UINT16 whichPos )
{
  height = whichPos;
}

bool HistogramProxy::getShowWindow() const
{
  return showWindow;
}

void HistogramProxy::setShowWindow( bool newValue )
{
  showWindow = newValue;
}

DrawModeMethod HistogramProxy::getDrawModeObjects() const
{
  return drawModeObjects;
}

void HistogramProxy::setDrawModeObjects( DrawModeMethod whichMethod )
{
  drawModeObjects = whichMethod;
}

DrawModeMethod HistogramProxy::getDrawModeColumns() const
{
  return drawModeColumns;
}

void HistogramProxy::setDrawModeColumns( DrawModeMethod whichMethod )
{
  drawModeColumns = whichMethod;
}

bool HistogramProxy::getChanged() const
{
  return changed;
}

void HistogramProxy::setChanged( bool newValue )
{
  changed = newValue;
}

void HistogramProxy::getGroupsLabels( vector<string>& onVector ) const
{
  myHisto->getGroupsLabels( onVector );
}

void HistogramProxy::getStatisticsLabels( vector<string>& onVector,
                                          PRV_UINT32 whichGroup,
                                          bool getOriginalList ) const
{
  if ( getOriginalList )
  {
    myHisto->getStatisticsLabels( onVector, whichGroup );
  }
  else
  {
    vector< string > fullList;
    myHisto->getStatisticsLabels( fullList, whichGroup );
    map< string, string >::const_iterator itStat;

    for( vector< string >::iterator it = fullList.begin(); it != fullList.end(); ++it )
    {
      itStat = statisticsAliasCFG4D.find( *it );
      if ( itStat != statisticsAliasCFG4D.end() )
      onVector.push_back( itStat->second );
    }
  }
}

string HistogramProxy::getFirstStatistic() const
{
  return myHisto->getFirstStatistic();
}

string HistogramProxy::getFirstCommStatistic() const
{
  return myHisto->getFirstCommStatistic();
}

bool HistogramProxy::getControlOutOfLimits() const
{
  return myHisto->getControlOutOfLimits();
}

bool HistogramProxy::getExtraOutOfLimits() const
{
  return myHisto->getExtraOutOfLimits();
}

bool HistogramProxy::getRedraw() const
{
  return redraw;
}

void HistogramProxy::setRedraw( bool newValue )
{
  redraw = newValue;
}

bool HistogramProxy::getRecalc() const
{
  return recalc;
}

void HistogramProxy::setRecalc( bool newValue )
{
  recalc = newValue;
}

bool HistogramProxy::getCodeColor() const
{
  return codeColor;
}

void HistogramProxy::setCodeColor( bool newValue )
{
  codeColor = newValue;
}

PRV_UINT16 HistogramProxy::getPixelSize() const
{
  return pixelSize;
}

void HistogramProxy::setPixelSize( PRV_UINT16 whichSize )
{
  pixelSize = whichSize;
}

void HistogramProxy::setCFG4DMode( bool mode )
{
  if ( controlWindow != NULL )
    controlWindow->setCFG4DMode( mode );

  if ( dataWindow != NULL )
    dataWindow->setCFG4DMode( mode );

  if ( extraControlWindow != NULL )
    extraControlWindow->setCFG4DMode( mode );

  CFG4DMode = mode;
}

bool HistogramProxy::getCFG4DMode( ) const
{
  return CFG4DMode;
}

bool HistogramProxy::getCFG4DEnabled() const
{
  return isCFG4DEnabled;
}

void HistogramProxy::setCFG4DEnabled( bool enabled )
{

  if ( controlWindow != NULL )
    controlWindow->setCFG4DEnabled( enabled );

  if ( dataWindow != NULL )
    dataWindow->setCFG4DEnabled( enabled );

  if ( extraControlWindow != NULL )
    extraControlWindow->setCFG4DEnabled( enabled );

  isCFG4DEnabled = enabled;
}


bool HistogramProxy::existsCFG4DAlias( const string &property ) const
{
  bool found = false;

  if ( propertiesAliasCFG4D.size() > 0 )
  {
    map< string, string >::const_iterator itAlias = propertiesAliasCFG4D.find( property );
    if ( itAlias != propertiesAliasCFG4D.end() )
    {
      found = true;
    }
  }

  return found;
}


bool HistogramProxy::existsCFG4DAlias( const THistogramProperties &propertyIndex ) const
{
  bool found = false;

  if ( propertiesAliasCFG4D.size() > 0 )
  {
    string property( HistogramPropertyLabels[ propertyIndex ] );

    map< string, string >::const_iterator itAlias = propertiesAliasCFG4D.find( property );
    if ( itAlias != propertiesAliasCFG4D.end() )
    {
      found = true;
    }
  }

  return found;
}


string HistogramProxy::getCFG4DAlias( const string &property ) const
{
  string alias = "";

  if ( propertiesAliasCFG4D.size() > 0 )
  {
    map< string, string >::const_iterator itAlias = propertiesAliasCFG4D.find( property );
    if ( itAlias != propertiesAliasCFG4D.end() )
    {
      alias = itAlias->second;
    }
  }

  return alias;
}


string HistogramProxy::getCFG4DAlias( const THistogramProperties &propertyIndex ) const
{
  string alias = "";

  if ( propertiesAliasCFG4D.size() > 0 )
  {
    string property( HistogramPropertyLabels[ propertyIndex ] );

    map< string, string >::const_iterator itAlias = propertiesAliasCFG4D.find( property );
    if ( itAlias != propertiesAliasCFG4D.end() )
    {
      alias = itAlias->second;
    }
  }

  return alias;
}


void HistogramProxy::setCFG4DAlias( const string &property, const string &alias )
{
  propertiesAliasCFG4D[ property ] = alias;
}

void HistogramProxy::setCFG4DStatisticAlias( const string &statistic, const string &alias )
{
  statisticsAliasCFG4D[ statistic ] = alias;
}


void  HistogramProxy::setCFG4DAliasList( const map< string, string >& aliasList )
{
  propertiesAliasCFG4D = aliasList;
}


void  HistogramProxy::setCFG4DStatisticsAliasList( const map< string, string >& statisticsAliasList )
{
  statisticsAliasCFG4D = statisticsAliasList;
}

const map< string, string > HistogramProxy::getCFG4DAliasList() const
{
  return propertiesAliasCFG4D;
}

const map< string, string > HistogramProxy::getCFG4DStatisticsAliasList() const
{
  return statisticsAliasCFG4D;
}

const vector< string > HistogramProxy::getCFG4DFullTagList()
{
  vector< string > tags;

  for ( int iTag = 0; iTag < TOTAL_HISTOGRAM_PROPERTIES; ++iTag )
  {
    tags.push_back( HistogramPropertyLabels[ iTag ] );
  }

  return tags;
}
