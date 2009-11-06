#include <string>
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
  showUnits = ParaverConfig::getInstance()->getShowUnits();
  sortColumns = Histogram::getSortColumns();
  sortCriteria = Histogram::getSortCriteria();
  minGradient = Histogram::getMinGradient();
  maxGradient = Histogram::getMaxGradient();
  computeControlScale = Histogram::getCompute2DScale();
  computeXtraScale = Histogram::getCompute3DScale();
  computeGradient = Histogram::getComputeGradient();
  showColor = Histogram::getShowColor();
  zoom = Histogram::getZoom();
  futurePlane = false;
  planeMinValue = 0.0;
  selectedPlane = 0;
  commSelectedPlane = 0;
  drawModeObjects = Histogram::getDrawModeObjects();
  drawModeColumns = Histogram::getDrawModeColumns();

  setCalculateAll( Histogram::getCalculateAll() );
  currentStat = Histogram::getCurrentStat();

  showWindow = true;
  changed = false;
  redraw = false;
  recalc = false;
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

void HistogramProxy::setInclusive( bool newValue )
{
  myHisto->setInclusive( newValue );
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


TSemanticValue HistogramProxy::getCurrentValue( UINT32 col,
    UINT16 idStat,
    UINT32 plane ) const
{
  return myHisto->getCurrentValue( col, idStat, plane );
}

UINT32 HistogramProxy::getCurrentRow( UINT32 col, UINT32 plane ) const
{
  return myHisto->getCurrentRow( col, plane );
}

void HistogramProxy::setNextCell( UINT32 col, UINT32 plane )
{
  myHisto->setNextCell( col, plane );
}

void HistogramProxy::setFirstCell( UINT32 col, UINT32 plane )
{
  myHisto->setFirstCell( col, plane );
}

bool HistogramProxy::endCell( UINT32 col, UINT32 plane )
{
  return myHisto->endCell( col, plane );
}

bool HistogramProxy::planeWithValues( UINT32 plane ) const
{
  return myHisto->planeWithValues( plane );
}

TSemanticValue HistogramProxy::getCommCurrentValue( UINT32 col,
    UINT16 idStat,
    UINT32 plane ) const
{
  return myHisto->getCommCurrentValue( col, idStat, plane );
}

UINT32 HistogramProxy::getCommCurrentRow( UINT32 col, UINT32 plane ) const
{
  return myHisto->getCommCurrentRow( col, plane );
}

void HistogramProxy::setCommNextCell( UINT32 col, UINT32 plane )
{
  myHisto->setCommNextCell( col, plane );
}

void HistogramProxy::setCommFirstCell( UINT32 col, UINT32 plane )
{
  myHisto->setCommFirstCell( col, plane );
}

bool HistogramProxy::endCommCell( UINT32 col, UINT32 plane )
{
  return myHisto->endCommCell( col, plane );
}

bool HistogramProxy::planeCommWithValues( UINT32 plane ) const
{
  return myHisto->planeCommWithValues( plane );
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
                              vector<TObjectOrder>& selectedRows )
{
  winBeginTime = whichBeginTime;
  winEndTime = whichEndTime;

  if ( computeControlScale )
    compute2DScale();

  if ( getThreeDimensions() && computeXtraScale )
    compute3DScale();

  myHisto->execute( whichBeginTime, whichEndTime, selectedRows );

  if ( getThreeDimensions() && futurePlane )
  {
    THistogramLimit nPlanes = getNumPlanes();
    THistogramLimit min, max;
    UINT32 i;
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
    UINT32 i;

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

void HistogramProxy::setNumDecimals( UINT16 newValue )
{
  numDecimals = newValue;
}

UINT16 HistogramProxy::getNumDecimals() const
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
  UINT32 plane;
  UINT16 idStat;
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

void HistogramProxy::setPlaneMinValue( double whichMin )
{
  planeMinValue = whichMin;
  futurePlane = true;
}

double HistogramProxy::getPlaneMinValue() const
{
  return planeMinValue;
}

void HistogramProxy::setSelectedPlane( INT32 plane )
{
  selectedPlane = plane;
}

INT32 HistogramProxy::getSelectedPlane() const
{
  return selectedPlane;
}

INT32 HistogramProxy::getCommSelectedPlane() const
{
  return commSelectedPlane;
}

bool HistogramProxy::itsCommunicationStat( const string& whichStat ) const
{
  return myHisto->itsCommunicationStat( whichStat );
}

void HistogramProxy::compute2DScale()
{
  TSemanticValue tmpMinY = controlWindow->getMinimumY();
  TSemanticValue tmpMaxY = controlWindow->getMaximumY();
  TRecordTime tmpBeginTime = controlWindow->getWindowBeginTime();
  TRecordTime tmpEndTime = controlWindow->getWindowEndTime();
  controlWindow->setWindowBeginTime( getBeginTime() );
  controlWindow->setWindowEndTime( getEndTime() );
  controlWindow->computeYScale();
  controlWindow->setWindowBeginTime( tmpBeginTime );
  controlWindow->setWindowEndTime( tmpEndTime );
  TSemanticValue minY = controlWindow->getMinimumY();
  TSemanticValue maxY = controlWindow->getMaximumY();
  controlWindow->setMinimumY( tmpMinY );
  controlWindow->setMaximumY( tmpMaxY );

  setControlMin( minY );
  setControlMax( maxY );

  if ( ( maxY - minY ) == 0 )
  {
    setControlDelta( 1.0 );
  }
  else if ( ( maxY - minY ) < 1.0 )
  {
    setControlDelta( ( maxY - minY ) /
                     ParaverConfig::getInstance()->getHistoNumColumns() );
  }
  else if ( ( maxY - minY ) <
            ParaverConfig::getInstance()->getHistoNumColumns() )
  {
    setControlDelta( 1.0 );
  }
  else
  {
    setControlDelta( ( maxY - minY ) /
                     ParaverConfig::getInstance()->getHistoNumColumns() );
  }
}

void HistogramProxy::compute3DScale()
{
  TSemanticValue tmpMinY = extraControlWindow->getMinimumY();
  TSemanticValue tmpMaxY = extraControlWindow->getMaximumY();
  TRecordTime tmpBeginTime = extraControlWindow->getWindowBeginTime();
  TRecordTime tmpEndTime = extraControlWindow->getWindowEndTime();
  extraControlWindow->setWindowBeginTime( getBeginTime() );
  extraControlWindow->setWindowEndTime( getEndTime() );
  extraControlWindow->computeYScale();
  extraControlWindow->setWindowBeginTime( tmpBeginTime );
  extraControlWindow->setWindowEndTime( tmpEndTime );
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
  else if ( ( maxY - minY ) < 1.0 )
  {
    setExtraControlDelta( ( maxY - minY ) /
                          ParaverConfig::getInstance()->getHistoNumColumns() );
  }
  else if ( ( maxY - minY ) <
            ParaverConfig::getInstance()->getHistoNumColumns() )
  {
    setExtraControlDelta( 1.0 );
  }
  else
  {
    setExtraControlDelta( ( maxY - minY ) /
                          ParaverConfig::getInstance()->getHistoNumColumns() );
  }
}

string HistogramProxy::getRowLabel( TObjectOrder whichRow ) const
{
  return LabelConstructor::objectLabel( whichRow, controlWindow->getLevel(),
                                        controlWindow->getTrace() );
}

string HistogramProxy::getColumnLabel( THistogramColumn whichColumn ) const
{
  return LabelConstructor::histoColumnLabel( whichColumn, controlWindow,
         getControlMin(),
         getControlMax(),
         getControlDelta() );
}

string HistogramProxy::getPlaneLabel( THistogramColumn whichPlane ) const
{
  Window *win = ( extraControlWindow != NULL ) ? extraControlWindow : controlWindow;
  return LabelConstructor::histoColumnLabel( whichPlane, win,
         getExtraControlMin(),
         getExtraControlMax(),
         getExtraControlDelta() );
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

bool HistogramProxy::getIdStat( const string& whichStat, UINT16& idStat ) const
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
  clonedHistogramProxy->futurePlane = futurePlane;
  clonedHistogramProxy->planeMinValue = planeMinValue;
  clonedHistogramProxy->selectedPlane = selectedPlane;
  clonedHistogramProxy->commSelectedPlane = commSelectedPlane;
  clonedHistogramProxy->showWindow = showWindow;
  clonedHistogramProxy->changed = changed;
  clonedHistogramProxy->redraw = redraw;
  clonedHistogramProxy->recalc = recalc;

  clonedHistogramProxy->winBeginTime = winBeginTime;
  clonedHistogramProxy->winEndTime = winEndTime;

  // Must store the associated proxies
  clonedHistogramProxy->setControlWindow( controlWindow->clone() );
  clonedHistogramProxy->setDataWindow( dataWindow->clone() );
  if ( extraControlWindow != NULL )
  {
    clonedHistogramProxy->setExtraControlWindow( extraControlWindow->clone() );
  }

  clonedHistogramProxy->calculateAll = calculateAll;
  clonedHistogramProxy->currentStat = currentStat;
  clonedHistogramProxy->calcStat = vector<string>( calcStat );
  clonedHistogramProxy->commCalcStat = vector<string>( commCalcStat );

  myGradientColor.copy( clonedHistogramProxy->myGradientColor );

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


UINT16 HistogramProxy::getPosX() const
{
  return posX;
}

void HistogramProxy::setPosX( UINT16 whichPos )
{
  posX = whichPos;
}

UINT16 HistogramProxy::getPosY() const
{
  return posY;
}

void HistogramProxy::setPosY( UINT16 whichPos )
{
  posY = whichPos;
}

UINT16 HistogramProxy::getWidth() const
{
  return width;
}

void HistogramProxy::setWidth( UINT16 whichPos )
{
  width = whichPos;
}

UINT16 HistogramProxy::getHeight() const
{
  return height;
}

void HistogramProxy::setHeight( UINT16 whichPos )
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

void HistogramProxy::getStatisticsLabels( vector<string>& onVector, UINT32 whichGroup ) const
{
  myHisto->getStatisticsLabels( onVector, whichGroup );
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
