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


#include <iostream>
#include <sstream>

#include <cmath>
#include <algorithm> // CFG4D

#include "config.h"
#include "kernelconnection.h"
#include "filter.h"
#include "trace.h"
#include "window.h"
#include "recordlist.h"
#include "loadedwindows.h"
#include "paraverconfig.h"
#include "selectionrowsutils.h"
#include "progresscontroller.h"

#ifdef PARALLEL_ENABLED
#include "omp.h"
#endif

using namespace std;

stringstream TimelineProxy::sstrCFGS4DOriginalName;


Timeline *Timeline::create( KernelConnection *whichKernel, Trace *whichTrace )
{
  return new TimelineProxy( whichKernel, whichTrace );
}

Timeline *Timeline::create( KernelConnection *whichKernel )
{
  return new TimelineProxy( whichKernel );
}

Timeline *Timeline::create( KernelConnection *whichKernel, Timeline *parent1, Timeline *parent2 )
{
  return new TimelineProxy( whichKernel, parent1, parent2 );
}

bool Timeline::compatibleLevels( Timeline *window1, Timeline *window2 )
{
  return ( window1->getLevel() >= TTraceLevel::WORKLOAD && window1->getLevel() <= TTraceLevel::THREAD &&
           window2->getLevel() >= TTraceLevel::WORKLOAD && window2->getLevel() <= TTraceLevel::THREAD )
         ||
         ( window1->getLevel() >= TTraceLevel::SYSTEM && window1->getLevel() <= TTraceLevel::CPU &&
           window2->getLevel() >= TTraceLevel::SYSTEM && window2->getLevel() <= TTraceLevel::CPU );
}

Timeline::Timeline( KernelConnection *whichKernel ) : myKernel( whichKernel )
{}

TimelineProxy::TimelineProxy():
  myCodeColor( this )
{
  parent1 = nullptr;
  parent2 = nullptr;
  myTrace = nullptr;
  myFilter = nullptr;
  init();
}

TimelineProxy::TimelineProxy( KernelConnection *whichKernel, Trace *whichTrace ):
  Timeline( whichKernel ), myTrace( whichTrace ), myCodeColor( this )
{
  parent1 = nullptr;
  parent2 = nullptr;
  myWindow = myKernel->newSingleWindow( whichTrace );
  myFilter = myKernel->newFilter( myWindow->getFilter() );
  init();
}

TimelineProxy::TimelineProxy( KernelConnection *whichKernel, Timeline *whichParent1,
                          Timeline *whichParent2 ):
  Timeline( whichKernel ), myCodeColor( this )
{
  parent1 = whichParent1;
  parent1->setChild( this );
  parent2 = whichParent2;
  parent2->setChild( this );

  if( whichParent1->getTrace()->getEndTime() >= whichParent2->getTrace()->getEndTime() )
    myTrace = whichParent1->getTrace();
  else
    myTrace = whichParent2->getTrace();

  myWindow = myKernel->newDerivedWindow( parent1, parent2 );
  myFilter = nullptr;
  init();
}

TimelineProxy::TimelineProxy( KernelConnection *whichKernel ):
  Timeline( whichKernel ), myTrace( nullptr ), myCodeColor( this )
{
  parent1 = nullptr;
  parent2 = nullptr;
  myWindow = myKernel->newDerivedWindow();
  myFilter = nullptr;
  init();
}

void TimelineProxy::init()
{
  ready = false;
  destroy = false;
  number_of_clones = 0;
  winBeginTime = 0.0;
  computeYMaxOnInit = false;
  yScaleComputed = false;
  maximumY = Timeline::getMaximumY();
  minimumY = Timeline::getMinimumY();
  existSemanticZero = Timeline::getExistSemanticZero();
  semanticScaleMinAtZero = ParaverConfig::getInstance()->getTimelineSemanticScaleMinAtZero();
  colorMode = ParaverConfig::getInstance()->getTimelineColor();
  useCustomPalette = Timeline::getUseCustomPalette();
  if( colorMode == TColorFunction::GRADIENT )
    myGradientColor.allowOutOfScale( true );
  else if( colorMode == TColorFunction::NOT_NULL_GRADIENT )
    myGradientColor.allowOutOfScale( false );
  myGradientColor.setGradientFunction( ParaverConfig::getInstance()->getTimelineGradientFunction() );
  drawModeObject = ParaverConfig::getInstance()->getTimelineDrawmodeObjects();
  drawModeTime = ParaverConfig::getInstance()->getTimelineDrawmodeTime();
  if( ParaverConfig::getInstance()->getTimelinePixelSize() <= 3 )
    pixelSize = (PRV_UINT16)pow( float(2), (int)ParaverConfig::getInstance()->getTimelinePixelSize() );
  else
    pixelSize = ParaverConfig::getInstance()->getTimelinePixelSize();
  showWindow = true;
  raise = false;
  changed = false;
  redraw = false;
  forceRedraw = false;
  commLines = ParaverConfig::getInstance()->getTimelineViewCommunicationsLines();
  flags = ParaverConfig::getInstance()->getTimelineViewEventsLines();
  child = nullptr;

  punctualColorWindow = nullptr;

  objectLabels = ParaverConfig::getInstance()->getTimelineLabels();
  objectAxisSize = ParaverConfig::getInstance()->getTimelineObjectAxisSize();

  if( myTrace != nullptr )
  {
    winEndTime = myTrace->getEndTime();
    myCodeColor = myTrace->getCodeColor();
    selectedRow.init( getTrace() );
    zoomHistory.addZoom( 0, winEndTime, 0, getWindowLevelObjects() - 1 );
  }

  sync = false;
  syncGroup = 0;

  isCFG4DEnabled = false;
  CFG4DMode = false;
  globalIndexLink = 0;
}

TimelineProxy::~TimelineProxy()
{
  if ( !myWindow->isDerivedWindow() && myFilter != nullptr )
    delete myFilter;
  LoadedWindows::getInstance()->eraseWindow( this );
  if( sync )
    SyncWindows::getInstance()->removeWindow( this, syncGroup );
  delete myWindow;

  for ( vector< RecordList *>::iterator it = myLists.begin(); it != myLists.end(); ++it )
  {
    if( *it != nullptr )
      delete *it;
  }
}

Filter *TimelineProxy::getFilter() const
{
  if ( myWindow->isDerivedWindow() )
    return parent1->getFilter();
  return myFilter;
}

void TimelineProxy::setFactor( PRV_UINT16 whichFactor, TSemanticValue newValue )
{
  if ( myWindow->isDerivedWindow() )
  {
    yScaleComputed = false;

    myWindow->setFactor( whichFactor, newValue );
  }
}

TSemanticValue TimelineProxy::getFactor( PRV_UINT16 whichFactor ) const
{
  return myWindow->getFactor( whichFactor );
}

void TimelineProxy::setShift( PRV_UINT16 whichShift, TSemanticValue newValue )
{
  if ( myWindow->isDerivedWindow() )
  {
    yScaleComputed = false;

    myWindow->setShift( whichShift, newValue );
  }
}

PRV_INT16 TimelineProxy::getShift( PRV_UINT16 whichShift ) const
{
  return myWindow->getShift( whichShift );
}

void TimelineProxy::setParent( PRV_UINT16 whichParent, Timeline *whichWindow )
{
  if ( myWindow->isDerivedWindow() )
  {
    yScaleComputed = false;

    if ( whichParent == 0 )
    {
      if ( parent1 != nullptr )
        parent1->setChild( nullptr );
      parent1 = whichWindow;
    }
    else if ( whichParent == 1 )
    {
      if ( parent2 != nullptr )
        parent2->setChild( nullptr );
      parent2 = whichWindow;
    }
    myWindow->setParent( whichParent, whichWindow->getConcrete() );

    whichWindow->setChild( this );

    if ( parent1 != nullptr && parent2 != nullptr && myTrace == nullptr )
    {
      if( parent1->getTrace()->getEndTime() >= parent2->getTrace()->getEndTime() )
        myTrace = parent1->getTrace();
      else
        myTrace = parent2->getTrace();
      init();
    }
  }
}

void TimelineProxy::setChild( Timeline *whichWindow )
{
  child = whichWindow;
}

Timeline *TimelineProxy::getChild()
{
  return child;
}


Timeline *TimelineProxy::getParent( PRV_UINT16 whichParent ) const
{
  switch ( whichParent )
  {
    case 0:
      return parent1;
      break;
    case 1:
      return parent2;
      break;
    default:
      return nullptr;
      break;
  }
}


Timeline *TimelineProxy::clone( bool recursiveClone )
{
  TimelineProxy *clonedWindow = new TimelineProxy();
  clonedWindow->myKernel = myKernel;
  clonedWindow->myTrace  = myTrace;
  clonedWindow->myWindow = myWindow->clone();

  if ( clonedWindow->isDerivedWindow() )
  {
    clonedWindow->parent1 = parent1->clone();
    clonedWindow->myWindow->setParent( 0, clonedWindow->parent1->getConcrete() );
    clonedWindow->parent2 = parent2->clone();
    clonedWindow->myWindow->setParent( 1, clonedWindow->parent2->getConcrete() );
    // setParent changes level to default, so we must reset the current level
    clonedWindow->myWindow->setLevel( myWindow->getLevel() );
    clonedWindow->parent1->setChild( clonedWindow );
    clonedWindow->parent2->setChild( clonedWindow );
  }
  else
    clonedWindow->myFilter = myKernel->newFilter( clonedWindow->myWindow->getFilter() );

  clonedWindow->winBeginTime = winBeginTime;
  clonedWindow->winEndTime = winEndTime;
  clonedWindow->computeYMaxOnInit = computeYMaxOnInit;
  clonedWindow->yScaleComputed = yScaleComputed;
  clonedWindow->computeYMaxOnInit = computeYMaxOnInit;
  clonedWindow->computedMaxY = computedMaxY;
  clonedWindow->computedMinY = computedMinY;
  clonedWindow->computedZeros = computedZeros;
  clonedWindow->maximumY = maximumY;
  clonedWindow->minimumY = minimumY;
  clonedWindow->existSemanticZero = existSemanticZero;

  std::ostringstream tmp;
  tmp << ++number_of_clones;
  clonedWindow->name = name + ".c" + tmp.str();

  clonedWindow->myCodeColor = myCodeColor;
  clonedWindow->myGradientColor = myGradientColor;
  clonedWindow->colorMode = colorMode;
  clonedWindow->punctualColorWindow = punctualColorWindow;
  clonedWindow->semanticScaleMinAtZero = semanticScaleMinAtZero;
  clonedWindow->drawModeObject = drawModeObject;
  clonedWindow->drawModeTime = drawModeTime;
  clonedWindow->showWindow = showWindow;
  clonedWindow->changed = changed;
  clonedWindow->redraw = redraw;
  clonedWindow->commLines = commLines;
  clonedWindow->flags = flags;
  clonedWindow->child = nullptr;
  clonedWindow->posX = posX;
  clonedWindow->posY = posY;
  clonedWindow->width = width;
  clonedWindow->height = height;

  clonedWindow->objectLabels = objectLabels;
  clonedWindow->objectAxisSize = objectAxisSize;
  /*
    for ( vector<RecordList *>::iterator it = myLists.begin(); it != myLists.end(); it++ )
      clonedWindow->myLists.push_back( (*it)->create( *it ) );
  */
  clonedWindow->zoomHistory = zoomHistory;

  clonedWindow->selectedRow.copy( selectedRow );

  if ( ParaverConfig::getInstance()->getTimelineKeepSyncGroupClone() )
  {
    clonedWindow->sync = sync;
    clonedWindow->syncGroup = syncGroup;
    if( clonedWindow->sync )
      SyncWindows::getInstance()->addWindow( clonedWindow, syncGroup );
  }

  // CFG4D
  clonedWindow->isCFG4DEnabled   = isCFG4DEnabled;
  clonedWindow->CFG4DMode   = CFG4DMode;
  clonedWindow->propertiesAliasCFG4D = propertiesAliasCFG4D;
  clonedWindow->paramAliasCFG4D = paramAliasCFG4D;

  return clonedWindow;
}


bool TimelineProxy::getShowProgressBar() const
{
  if ( !myWindow->isDerivedWindow() )
    return myTrace->getShowProgressBar();

  return parent1->getShowProgressBar() || parent2->getShowProgressBar();
}


void TimelineProxy::setDestroy( bool newValue )
{
  destroy = newValue;
}


bool TimelineProxy::getDestroy() const
{
  return destroy;
}


void TimelineProxy::setReady( bool newValue )
{
  ready = newValue;
}


bool TimelineProxy::getReady() const
{
  return ready;
}


void TimelineProxy::setUsedByHistogram( Histogram *whichHisto )
{
  usedByHistogram.insert( whichHisto );
}


void TimelineProxy::unsetUsedByHistogram( Histogram *whichHisto )
{
  usedByHistogram.erase( whichHisto );
}


bool TimelineProxy::getUsedByHistogram() const
{
  return usedByHistogram.size() > 0;
}


set<Histogram *> TimelineProxy::getHistograms() const
{
  return usedByHistogram;
}


void TimelineProxy::setWindowBeginTime( TRecordTime whichTime, bool isBroadcast )
{
  winBeginTime = whichTime;
  if( sync && !isBroadcast )
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, winBeginTime, winEndTime );
}

void TimelineProxy::setWindowEndTime( TRecordTime whichTime, bool isBroadcast )
{
  winEndTime = whichTime;
  if( sync && !isBroadcast )
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, winBeginTime, winEndTime );
}

TRecordTime TimelineProxy::getWindowBeginTime() const
{
  return winBeginTime;
}

TRecordTime TimelineProxy::getWindowEndTime() const
{
  return winEndTime;
}

bool TimelineProxy::getYScaleComputed() const
{
  return yScaleComputed;
}

void TimelineProxy::computeYScaleMin()
{
  if ( !yScaleComputed )
  {
    vector< TObjectOrder > selected;
    getSelectedRows( getLevel(), selected,
                     getZoomSecondDimension().first, getZoomSecondDimension().second, true );

    init( winBeginTime, NOCREATE );

    for ( vector< TObjectOrder >::iterator obj = selected.begin(); obj != selected.end(); ++obj )
    {
      initRow( *obj, winBeginTime, NOCREATE );
      while ( getBeginTime( *obj ) < getTrace()->getEndTime() &&
              getBeginTime( *obj ) < myTrace->getEndTime() )
        calcNext( *obj );
    }
  }

  minimumY = computedMinY;
}

void TimelineProxy::computeYScaleMax()
{
  if ( !yScaleComputed )
  {
    vector< TObjectOrder > selected;
    getSelectedRows( getLevel(), selected,
                     getZoomSecondDimension().first, getZoomSecondDimension().second, true );

    init( winBeginTime, NOCREATE );

    for ( vector< TObjectOrder >::iterator obj = selected.begin(); obj != selected.end(); ++obj )
    {
      initRow( *obj, winBeginTime, NOCREATE );
      while ( getBeginTime( *obj ) < getTrace()->getEndTime() &&
              getBeginTime( *obj ) < myTrace->getEndTime() )
        calcNext( *obj );
    }
  }

  maximumY = computedMaxY;
}

void TimelineProxy::computeYScale( ProgressController *progress )
{
  vector< TSemanticValue > tmpComputedMaxY;
  vector< TSemanticValue > tmpComputedMinY;
  vector< int > tmpComputedZeros;

  if ( !yScaleComputed )
  {
    vector< TObjectOrder > selected;
    getSelectedRows( getLevel(), selected,
                     getZoomSecondDimension().first, getZoomSecondDimension().second, true );
    int progressDelta;
    if( progress != nullptr )
      progressDelta = (int)floor( selected.size() * 0.005 );

    init( winBeginTime, NOCREATE );

    std::string previousMessage;
    int currentObject = 0;
    if( progress != nullptr )
    {
      previousMessage = progress->getMessage();
      progress->setMessage( "Computing semantic scale..." );
      progress->setEndLimit( selected.size() + 1 );
      progress->setCurrentProgress( currentObject );
    }

    tmpComputedMaxY.reserve( selected.size() );
    tmpComputedMinY.reserve( selected.size() );
    tmpComputedZeros.reserve( selected.size() );

    #pragma omp parallel
    {
      #pragma omp single
      {

#ifdef PARALLEL_ENABLED
        if( selected.size() > 1 ||
            ( myWindow->isDerivedWindow() && myWindow->getTrace()->getLevelObjects( myWindow->getParent( 0 )->getLevel() ) !=
                                             myWindow->getTrace()->getLevelObjects( myWindow->getParent( 1 )->getLevel() )
            )
          )
        {
          for( int i = 0; i != omp_get_num_threads(); ++i )
            parallelClone.push_back( myWindow->clone( true ) );
        }
#endif // PARALLEL_ENABLED
        for ( int i = 0; i < selected.size(); ++i )
        {
          tmpComputedMaxY.push_back( 0.0 );
          tmpComputedMinY.push_back( 0.0 );
          tmpComputedZeros.push_back( false );
          
          int tmpComputedMaxYSize = tmpComputedMaxY.size();
          int tmpComputedMinYSize = tmpComputedMinY.size();
          int tmpComputedZerosSize = tmpComputedZeros.size();

          #pragma omp task shared ( currentObject, progress, tmpComputedMaxY, tmpComputedMinY, tmpComputedZeros ) \
                           firstprivate( tmpComputedMaxYSize, tmpComputedMinYSize, tmpComputedZerosSize )
          {
            TObjectOrder obj = selected[ i ];
            initRow( obj, winBeginTime, NOCREATE, tmpComputedMaxY[ tmpComputedMaxYSize - 1 ], tmpComputedMinY[ tmpComputedMinYSize - 1 ], tmpComputedZeros[ tmpComputedZerosSize - 1 ]  );
            if( progress == nullptr || ( progress != nullptr && !progress->getStop() ) )
            {
              while ( getBeginTime( obj ) < winEndTime &&
                      getBeginTime( obj ) < myTrace->getEndTime() )
                calcNext( obj, tmpComputedMaxY[ tmpComputedMaxYSize - 1 ], tmpComputedMinY[ tmpComputedMinYSize - 1 ], tmpComputedZeros[ tmpComputedZerosSize - 1 ] );

              #pragma omp atomic
              ++currentObject;
              if( progress != nullptr )
              {
                if( selected.size() <= 200 || currentObject % progressDelta == 0 )
                {
                  #pragma omp critical
                  progress->setCurrentProgress( currentObject );
                }
              }
            }
          }
        }

      } // omp single
    } // omp parallel

    for ( int pos = 0; pos < selected.size(); ++pos )
    {
      computedZeros = computedZeros || tmpComputedZeros[ pos ];
      computedMaxY = computedMaxY > tmpComputedMaxY[ pos ] ? computedMaxY : tmpComputedMaxY[ pos ];
      if ( computedMinY == 0.0 )
        computedMinY = tmpComputedMinY[ pos ];
      else if( tmpComputedMinY[ pos ] != 0.0 )
        computedMinY = computedMinY < tmpComputedMinY[ pos ] ? computedMinY : tmpComputedMinY[ pos ];
    }


#ifdef PARALLEL_ENABLED
    for( vector<Timeline *>::iterator it = parallelClone.begin(); it != parallelClone.end(); ++it )
      delete *it;
    parallelClone.clear();
#endif // PARALLEL_ENABLED

    if( progress != nullptr )
    {
      progress->setMessage( previousMessage );
    }
  }

  maximumY = computedMaxY;
  minimumY = computedMinY;
  existSemanticZero = computedZeros;
}

void TimelineProxy::setComputeYMaxOnInit( bool newValue )
{
  computeYMaxOnInit = newValue;
}

bool TimelineProxy::getComputeYMaxOnInit() const
{
  return computeYMaxOnInit;
}

void TimelineProxy::setMaximumY( TSemanticValue whichMax )
{
  maximumY = whichMax;
}

void TimelineProxy::setMinimumY( TSemanticValue whichMin )
{
  minimumY = whichMin;
}

TSemanticValue TimelineProxy::getMaximumY()
{
  if ( computeYMaxOnInit )
  {
    computeYScale();
    computeYMaxOnInit = false;
  }
  return maximumY;
}

TSemanticValue TimelineProxy::getMinimumY()
{
  if ( computeYMaxOnInit )
  {
    computeYScale();
    computeYMaxOnInit = false;
  }
  return minimumY;
}

bool TimelineProxy::getExistSemanticZero() const
{
  return existSemanticZero;
}

Trace *TimelineProxy::getTrace() const
{
  return myTrace;
}

TTraceLevel TimelineProxy::getLevel() const
{
  return myWindow->getLevel();
}

void TimelineProxy::setLevel( TTraceLevel whichLevel )
{
  if ( whichLevel == myWindow->getLevel() )
    return;

  yScaleComputed = false;

  myWindow->setLevel( whichLevel );
  zoomHistory.clear();
  zoomHistory.addZoom( winBeginTime, winEndTime, 0, getWindowLevelObjects() - 1 );
}

bool TimelineProxy::isLevelProcessModel() const
{
  return getLevel() >= TTraceLevel::WORKLOAD && getLevel() <= TTraceLevel::THREAD;
}

TTraceLevel TimelineProxy::getMinAcceptableLevel() const
{
  return myWindow->getMinAcceptableLevel();
}

void TimelineProxy::setTimeUnit( TTimeUnit whichUnit )
{
  yScaleComputed = false;

  myWindow->setTimeUnit( whichUnit );
}

TTimeUnit TimelineProxy::getTimeUnit() const
{
  return myWindow->getTimeUnit();
}

TWindowLevel TimelineProxy::getComposeLevel( TTraceLevel whichLevel ) const
{
  return myWindow->getComposeLevel( whichLevel );
}

bool TimelineProxy::setLevelFunction( TWindowLevel whichLevel,
                                      const string& whichFunction )
{
  bool result = myWindow->setLevelFunction( whichLevel, whichFunction );
  if ( result )
    yScaleComputed = false;

  return result;
}

string TimelineProxy::getLevelFunction( TWindowLevel whichLevel ) const
{
  return myWindow->getLevelFunction( whichLevel );
}

string TimelineProxy::getFirstUsefulFunction( )
{
  return myWindow->getFirstUsefulFunction();
}

TWindowLevel TimelineProxy::getFirstFreeCompose() const
{
  return myWindow->getFirstFreeCompose();
}

void TimelineProxy::setFunctionParam( TWindowLevel whichLevel,
                                    TParamIndex whichParam,
                                    const TParamValue& newValue )
{
  yScaleComputed = false;

  myWindow->setFunctionParam( whichLevel, whichParam, newValue );
}

TParamIndex TimelineProxy::getFunctionNumParam( TWindowLevel whichLevel ) const
{
  return myWindow->getFunctionNumParam( whichLevel );
}

TParamValue TimelineProxy::getFunctionParam( TWindowLevel whichLevel,
    TParamIndex whichParam ) const
{
  return myWindow->getFunctionParam( whichLevel, whichParam );
}

string TimelineProxy::getFunctionParamName( TWindowLevel whichLevel,
    TParamIndex whichParam ) const
{
  return myWindow->getFunctionParamName( whichLevel, whichParam );
}

// Extra composes
void TimelineProxy::addExtraCompose( TWindowLevel whichLevel )
{
  myWindow->addExtraCompose( whichLevel );
}

void TimelineProxy::removeExtraCompose( TWindowLevel whichLevel )
{
  myWindow->removeExtraCompose( whichLevel );
}

size_t TimelineProxy::getExtraNumPositions( TWindowLevel whichLevel ) const
{
  return myWindow->getExtraNumPositions( whichLevel );
}

bool TimelineProxy::setExtraLevelFunction( TWindowLevel whichLevel,
                                         size_t whichPosition,
                                         const string& whichFunction )
{
  return myWindow->setExtraLevelFunction( whichLevel, whichPosition, whichFunction );
}

string TimelineProxy::getExtraLevelFunction( TWindowLevel whichLevel,
                                           size_t whichPosition )
{
  return myWindow->getExtraLevelFunction( whichLevel, whichPosition );
}

void TimelineProxy::setExtraFunctionParam( TWindowLevel whichLevel,
                                         size_t whichPosition,
                                         TParamIndex whichParam,
                                         const TParamValue& newValue )
{
  myWindow->setExtraFunctionParam( whichLevel, whichPosition, whichParam, newValue );
}

TParamIndex TimelineProxy::getExtraFunctionNumParam( TWindowLevel whichLevel, size_t whichPosition ) const
{
  return myWindow->getExtraFunctionNumParam( whichLevel, whichPosition );
}

TParamValue TimelineProxy::getExtraFunctionParam( TWindowLevel whichLevel,
                                                size_t whichPosition,
                                                TParamIndex whichParam ) const
{
  return myWindow->getExtraFunctionParam( whichLevel, whichPosition, whichParam );
}

string TimelineProxy::getExtraFunctionParamName( TWindowLevel whichLevel,
                                               size_t whichPosition,
                                               TParamIndex whichParam ) const
{
  return myWindow->getExtraFunctionParamName( whichLevel, whichPosition, whichParam );
}


RecordList *TimelineProxy::getRecordList( TObjectOrder whichObject )
{
  if ( myLists.begin() == myLists.end() )
    return nullptr;
  return myLists[ whichObject ];
}

void TimelineProxy::init( TRecordTime initialTime, TCreateList create, bool updateLimits )
{
  if ( getComputeYMaxOnInit() )
  {
    setComputeYMaxOnInit( false );
    computeYScale();
  }

  if ( myLists.begin() != myLists.end() )
  {
    for ( vector<RecordList *>::iterator it = myLists.begin(); it != myLists.end(); ++it )
      delete *it;
    if ( myLists.begin() != myLists.end() )
      myLists.clear();
  }

  for ( int i = 0; i < myWindow->getWindowLevelObjects(); ++i )
    myLists.push_back( nullptr );

  myWindow->init( initialTime, create );
  if ( updateLimits )
  {
    yScaleComputed = true;
    computedMaxY = computedMinY = 0.0;
    computedZeros = false;
  }
}

void TimelineProxy::initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits )
{
  Timeline *tmpMyWindow = myWindow;
#ifdef PARALLEL_ENABLED
  if( parallelClone.size() > 0 )
    tmpMyWindow = parallelClone[ omp_get_thread_num() ];
#endif // PARALLEL_ENABLED

  tmpMyWindow->initRow( whichRow, initialTime, create );
  if ( create != NOCREATE && myLists[ whichRow ] == nullptr )
    myLists[ whichRow ] = RecordList::create( tmpMyWindow->getRecordList( whichRow ) );

  if ( updateLimits )
  {
    TSemanticValue objValue = tmpMyWindow->getValue( whichRow );
    computedZeros = computedZeros || objValue == 0.0;
    if ( computedMaxY < objValue )
      computedMaxY = objValue;
    if ( computedMinY == 0 || ( computedMinY > objValue && objValue != 0 ) )
      computedMinY = objValue;
  }
}

void TimelineProxy::initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create,
                           TSemanticValue& rowComputedMaxY, TSemanticValue& rowComputedMinY,
                           int& rowComputedZeros,
                           bool updateLimits )
{
  Timeline *tmpMyWindow = myWindow;
#ifdef PARALLEL_ENABLED
  if( parallelClone.size() > 0 )
    tmpMyWindow = parallelClone[ omp_get_thread_num() ];
#endif // PARALLEL_ENABLED

  tmpMyWindow->initRow( whichRow, initialTime, create );
  if ( create != NOCREATE && myLists[ whichRow ] == nullptr )
    myLists[ whichRow ] = RecordList::create( tmpMyWindow->getRecordList( whichRow ) );

  if ( updateLimits )
  {
    TSemanticValue objValue = tmpMyWindow->getValue( whichRow );
    rowComputedZeros = rowComputedZeros || objValue == 0.0;
    if ( rowComputedMaxY < objValue )
      rowComputedMaxY = objValue;
    if ( rowComputedMinY == 0 || ( rowComputedMinY > objValue && objValue != 0 ) )
      rowComputedMinY = objValue;
  }
}

RecordList *TimelineProxy::calcNext( TObjectOrder whichObject, bool updateLimits )
{
  Timeline *tmpMyWindow = myWindow;
#ifdef PARALLEL_ENABLED
  if( parallelClone.size() > 0 )
    tmpMyWindow = parallelClone[ omp_get_thread_num() ];
#endif // PARALLEL_ENABLED

  if ( myLists[ whichObject ] == nullptr )
    myLists[ whichObject ] = RecordList::create( tmpMyWindow->calcNext( whichObject ) );
  else
    tmpMyWindow->calcNext( whichObject );

  if ( updateLimits )
  {
    TSemanticValue objValue = tmpMyWindow->getValue( whichObject );
    computedZeros = computedZeros || objValue == 0.0;
    if ( computedMaxY < objValue )
      computedMaxY = objValue;
    if ( computedMinY == 0 || ( computedMinY > objValue && objValue != 0 ) )
      computedMinY = objValue;
  }

  return myLists[ whichObject ];
}

RecordList *TimelineProxy::calcNext( TObjectOrder whichObject,
                                   TSemanticValue& rowComputedMaxY, TSemanticValue& rowComputedMinY,
                                   int& rowComputedZeros,
                                   bool updateLimits )
{
  Timeline *tmpMyWindow = myWindow;
#ifdef PARALLEL_ENABLED
  if( parallelClone.size() > 0 )
    tmpMyWindow = parallelClone[ omp_get_thread_num() ];
#endif // PARALLEL_ENABLED

  if ( myLists[ whichObject ] == nullptr )
    myLists[ whichObject ] = RecordList::create( tmpMyWindow->calcNext( whichObject ) );
  else
    tmpMyWindow->calcNext( whichObject );

  if ( updateLimits )
  {
    TSemanticValue objValue = tmpMyWindow->getValue( whichObject );
    rowComputedZeros = rowComputedZeros || objValue == 0.0;
    if ( rowComputedMaxY < objValue )
      rowComputedMaxY = objValue;
    if ( rowComputedMinY == 0 || ( rowComputedMinY > objValue && objValue != 0 ) )
      rowComputedMinY = objValue;
  }

  return myLists[ whichObject ];
}

RecordList *TimelineProxy::calcPrev( TObjectOrder whichObject, bool updateLimits )
{
  Timeline *tmpMyWindow = myWindow;
#ifdef PARALLEL_ENABLED
  if( parallelClone.size() > 0 )
    tmpMyWindow = parallelClone[ omp_get_thread_num() ];
#endif // PARALLEL_ENABLED

  if ( myLists[ whichObject ] == nullptr )
    myLists[ whichObject ] = RecordList::create( tmpMyWindow->calcPrev( whichObject ) );
  else
    tmpMyWindow->calcPrev( whichObject );

  if ( updateLimits )
  {
    TSemanticValue objValue = getValue( whichObject );
    computedZeros = computedZeros || objValue == 0.0;
    if ( computedMaxY < objValue )
      computedMaxY = objValue;
    if ( computedMinY == 0 || ( computedMinY > objValue && objValue != 0 ) )
      computedMinY = objValue;
  }

  return myLists[ whichObject ];
}

TRecordTime TimelineProxy::getBeginTime( TObjectOrder whichObject ) const
{
  Timeline *tmpMyWindow = myWindow;
#ifdef PARALLEL_ENABLED
  if( parallelClone.size() > 0 )
    tmpMyWindow = parallelClone[ omp_get_thread_num() ];
#endif // PARALLEL_ENABLED

  return tmpMyWindow->getBeginTime( whichObject );
}

TRecordTime TimelineProxy::getEndTime( TObjectOrder whichObject ) const
{
  Timeline *tmpMyWindow = myWindow;
#ifdef PARALLEL_ENABLED
  if( parallelClone.size() > 0 )
    tmpMyWindow = parallelClone[ omp_get_thread_num() ];
#endif // PARALLEL_ENABLED

  return tmpMyWindow->getEndTime( whichObject );
}

TSemanticValue TimelineProxy::getValue( TObjectOrder whichObject ) const
{
  Timeline *tmpMyWindow = myWindow;
#ifdef PARALLEL_ENABLED
  if( parallelClone.size() > 0 )
    tmpMyWindow = parallelClone[ omp_get_thread_num() ];
#endif // PARALLEL_ENABLED

  return tmpMyWindow->getValue( whichObject );
}

bool TimelineProxy::isDerivedWindow() const
{
  return myWindow->isDerivedWindow();
}

TObjectOrder TimelineProxy::cpuObjectToWindowObject( TCPUOrder whichCPU )
{
  return myWindow->cpuObjectToWindowObject( whichCPU );
}

TObjectOrder TimelineProxy::threadObjectToWindowObject( TThreadOrder whichThread )
{
  return myWindow->threadObjectToWindowObject( whichThread );
}

TObjectOrder TimelineProxy::getWindowLevelObjects() const
{
  return myWindow->getWindowLevelObjects();
}

TRecordTime TimelineProxy::customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const
{
  return myWindow->customUnitsToTraceUnits( whichTime, whichUnits );
}

TRecordTime TimelineProxy::traceUnitsToCustomUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const
{
  return myWindow->traceUnitsToCustomUnits( whichTime, whichUnits );
}

TRecordTime TimelineProxy::traceUnitsToWindowUnits( TRecordTime whichTime ) const
{
  return myWindow->traceUnitsToWindowUnits( whichTime );
}

TRecordTime TimelineProxy::windowUnitsToTraceUnits( TRecordTime whichTime ) const
{
  return myWindow->windowUnitsToTraceUnits( whichTime );
}

Timeline *TimelineProxy::getConcrete() const
{
  return myWindow;
}

void TimelineProxy::setName( const string& whichName )
{
  name = whichName;
}

string TimelineProxy::getName() const
{
  return name;
}

PRV_UINT16 TimelineProxy::getPosX() const
{
  return posX;
}

void TimelineProxy::setPosX( PRV_UINT16 whichPos )
{
  posX = whichPos;
}

PRV_UINT16 TimelineProxy::getPosY() const
{
  return posY;
}

void TimelineProxy::setPosY( PRV_UINT16 whichPos )
{
  posY = whichPos;
}

PRV_UINT16 TimelineProxy::getWidth() const
{
  return width;
}

void TimelineProxy::setWidth( PRV_UINT16 whichPos )
{
  width = whichPos;
}

PRV_UINT16 TimelineProxy::getHeight() const
{
  return height;
}

void TimelineProxy::setHeight( PRV_UINT16 whichPos )
{
  height = whichPos;
}

void TimelineProxy::setDrawModeObject( DrawModeMethod method )
{
  drawModeObject = method;
}

DrawModeMethod TimelineProxy::getDrawModeObject() const
{
  return drawModeObject;
}

void TimelineProxy::setDrawModeTime( DrawModeMethod method )
{
  drawModeTime = method;
}

DrawModeMethod TimelineProxy::getDrawModeTime() const
{
  return drawModeTime;
}

CodeColor& TimelineProxy::getCodeColor()
{
  return myCodeColor;
}

GradientColor& TimelineProxy::getGradientColor()
{
  return myGradientColor;
}

bool TimelineProxy::getSemanticScaleMinAtZero()
{
  return semanticScaleMinAtZero;
}

bool TimelineProxy::getShowWindow() const
{
  return showWindow;
}

void TimelineProxy::setShowWindow( bool newValue )
{
  showWindow = newValue;
}

void TimelineProxy::setShowChildrenWindow( bool newValue )
{
  if ( getParent( 0 ) != nullptr )
  {
    for ( PRV_UINT16 i = 0; i < 2; ++i )
    {
      getParent( i )->setShowWindow( newValue );
      getParent( i )->setShowChildrenWindow( newValue );
    }
  }
}

bool TimelineProxy::getRaiseWindow() const
{
  return raise;
}

void TimelineProxy::setRaiseWindow( bool newValue )
{
  raise = newValue;
}

void TimelineProxy::setCodeColorMode()
{
  colorMode = TColorFunction::COLOR;
}

void TimelineProxy::setGradientColorMode()
{
  colorMode = TColorFunction::GRADIENT;
  myGradientColor.allowOutOfScale( true );
}

void TimelineProxy::setNotNullGradientColorMode()
{
  colorMode = TColorFunction::NOT_NULL_GRADIENT;
  myGradientColor.allowOutOfScale( false );
}

void TimelineProxy::setFunctionLineColorMode()
{
  colorMode = TColorFunction::FUNCTION_LINE;
}

void TimelineProxy::setFusedLinesColorMode()
{
  colorMode = TColorFunction::FUSED_LINES;
}

void TimelineProxy::setPunctualColorMode()
{
  colorMode = TColorFunction::PUNCTUAL;
}

bool TimelineProxy::isCodeColorSet() const
{
  return colorMode == TColorFunction::COLOR;
}

bool TimelineProxy::isGradientColorSet() const
{
  return colorMode == TColorFunction::GRADIENT;
}

bool TimelineProxy::isNotNullGradientColorSet() const
{
  return colorMode == TColorFunction::NOT_NULL_GRADIENT;
}

bool TimelineProxy::isFunctionLineColorSet() const
{
  return colorMode == TColorFunction::FUNCTION_LINE;
}

bool TimelineProxy::isFusedLinesColorSet() const
{
  return colorMode == TColorFunction::FUSED_LINES;
}

bool TimelineProxy::isPunctualColorSet() const
{
  return colorMode == TColorFunction::PUNCTUAL;
}

PRV_UINT16 TimelineProxy::getPixelSize() const
{
  return pixelSize;
}

void TimelineProxy::setPixelSize( PRV_UINT16 whichSize )
{
  pixelSize = whichSize;
}

Timeline *TimelineProxy::getPunctualColorWindow() const
{
  return punctualColorWindow;
}

void TimelineProxy::setPunctualColorWindow( Timeline *whichWindow )
{
  punctualColorWindow = whichWindow;
}

void  TimelineProxy::setSemanticScaleMinAtZero( bool newValue )
{
  semanticScaleMinAtZero = newValue;
}

void TimelineProxy::allowOutOfScale( bool activate )
{
  myGradientColor.allowOutOfScale( activate );
}

void TimelineProxy::allowOutliers( bool activate )
{
  myGradientColor.allowOutliers( activate );
}

rgb TimelineProxy::calcColor( TSemanticValue whichValue, Timeline& whichWindow )
{
  if ( colorMode == TColorFunction::COLOR )
    return myCodeColor.calcColor( whichValue, minimumY, maximumY, useCustomPalette );

  return myGradientColor.calcColor( whichValue, minimumY, maximumY );
}

bool TimelineProxy::isColorOutlier( rgb whichColor ) const
{
  if( colorMode != TColorFunction::GRADIENT )
    return false;
  return myGradientColor.isColorOutlier( whichColor );
}

bool TimelineProxy::getUseCustomPalette() const
{
  return useCustomPalette;
}

void TimelineProxy::setUseCustomPalette( bool newValue ) 
{
  useCustomPalette = newValue;
}

bool TimelineProxy::getChanged() const
{
  return changed;
}

void TimelineProxy::setChanged( bool newValue )
{
  changed = newValue;
}

bool TimelineProxy::getRedraw() const
{
  return redraw;
}

void TimelineProxy::setRedraw( bool newValue )
{
  redraw = newValue;
}

bool TimelineProxy::getForceRedraw() const
{
  return forceRedraw;
}

void TimelineProxy::setForceRedraw( bool newValue )
{
  forceRedraw = newValue;
}

bool TimelineProxy::getDrawCommLines() const
{
  return commLines;
}

void TimelineProxy::setDrawCommLines( bool newValue )
{
  commLines = newValue;
}

bool TimelineProxy::getDrawFlags() const
{
  return flags;
}

void TimelineProxy::setDrawFlags( bool newValue )
{
  flags = newValue;
}

SemanticInfoType TimelineProxy::getSemanticInfoType() const
{
  return myWindow->getSemanticInfoType();
}

void TimelineProxy::getAllSemanticFunctions( TSemanticGroup whichGroup,
    vector<string>& onVector ) const
{
  myKernel->getAllSemanticFunctions( whichGroup, onVector );
}

bool TimelineProxy::emptyPrevZoom() const
{
  return zoomHistory.isEmpty( TZoomPosition::PREV_ZOOM );
}

bool TimelineProxy::emptyNextZoom() const
{
  return zoomHistory.isEmpty( TZoomPosition::NEXT_ZOOM );
}

void TimelineProxy::nextZoom()
{
  zoomHistory.nextZoom();
  if( sync )
  {
    TTime nanoBeginTime, nanoEndTime;
    nanoBeginTime = traceUnitsToCustomUnits( zoomHistory.getFirstDimension().first, NS );
    nanoEndTime = traceUnitsToCustomUnits( zoomHistory.getFirstDimension().second, NS );
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, nanoBeginTime, nanoEndTime );
  }
}

void TimelineProxy::prevZoom()
{
  zoomHistory.prevZoom();
  if( sync )
  {
    TTime nanoBeginTime, nanoEndTime;
    nanoBeginTime = traceUnitsToCustomUnits( zoomHistory.getFirstDimension().first, NS );
    nanoEndTime = traceUnitsToCustomUnits( zoomHistory.getFirstDimension().second, NS );
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, nanoBeginTime, nanoEndTime );
  }
}

void TimelineProxy::addZoom( TTime beginTime, TTime endTime,
                           TObjectOrder beginObject, TObjectOrder endObject,
                           bool isBroadCast )
{
  if( sync && !isBroadCast )
  {
    TTime nanoBeginTime, nanoEndTime;
    nanoBeginTime = traceUnitsToCustomUnits( beginTime, NS );
    nanoEndTime = traceUnitsToCustomUnits( endTime, NS );
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, nanoBeginTime, nanoEndTime );
  }
  zoomHistory.addZoom( beginTime, endTime, beginObject, endObject );
}

void TimelineProxy::addZoom( TTime beginTime, TTime endTime, bool isBroadCast )
{
  if( sync && !isBroadCast )
  {
    TTime nanoBeginTime, nanoEndTime;
    nanoBeginTime = traceUnitsToCustomUnits( beginTime, NS );
    nanoEndTime = traceUnitsToCustomUnits( endTime, NS );
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, nanoBeginTime, nanoEndTime );
  }
  zoomHistory.addZoom( beginTime, endTime );
}

void TimelineProxy::addZoom( TObjectOrder beginObject, TObjectOrder endObject )
{
  zoomHistory.addZoom( beginObject, endObject );
}

void TimelineProxy::setZoomFirstDimension( pair<TTime, TTime> &dim )
{
  zoomHistory.setFirstDimension( dim );
}

void TimelineProxy::setZoomSecondDimension( pair<TObjectOrder, TObjectOrder>  &dim )
{
  zoomHistory.setSecondDimension( dim );
}

pair<TTime, TTime> TimelineProxy::getZoomFirstDimension() const
{
  return zoomHistory.getFirstDimension();
}

pair<TObjectOrder, TObjectOrder> TimelineProxy::getZoomSecondDimension() const
{
  return zoomHistory.getSecondDimension();
}

pair<TObjectOrder, TObjectOrder> TimelineProxy::getPrevZoomSecondDimension() const
{
  return zoomHistory.getSecondDimension( TZoomPosition::PREV_ZOOM );
}

pair<TObjectOrder, TObjectOrder> TimelineProxy::getNextZoomSecondDimension() const
{
  return zoomHistory.getSecondDimension( TZoomPosition::NEXT_ZOOM );
}

void TimelineProxy::addToSyncGroup( TGroupId whichGroup )
{
  SyncWindows::getInstance()->removeWindow( this, syncGroup );
  syncGroup = whichGroup;
  sync = SyncWindows::getInstance()->addWindow( this, whichGroup );
}

void TimelineProxy::removeFromSync()
{
  if( !sync )
    return;
  SyncWindows::getInstance()->removeWindow( this, syncGroup );
  sync = false;
}

bool TimelineProxy::isSync() const
{
  return sync;
}

TGroupId TimelineProxy::getSyncGroup() const
{
  return syncGroup;
}

SelectionManagement< TObjectOrder, TTraceLevel > *TimelineProxy::getSelectedRows()
{
  return &selectedRow;
}

void TimelineProxy::setSelectedRows( TTraceLevel onLevel, vector< bool > &selected )
{
  if( selected.size() == myTrace->getLevelObjects( onLevel ) )
    selectedRow.setSelected( selected, onLevel );
}

void TimelineProxy::setSelectedRows( TTraceLevel onLevel, vector< TObjectOrder > &selected )
{
  selectedRow.setSelected( selected, myTrace->getLevelObjects( onLevel ), onLevel );
}

void TimelineProxy::getSelectedRows( TTraceLevel onLevel, vector< bool > &selected, bool lookUpLevels )
{
  selectedRow.getSelected( selected, onLevel );

  if ( lookUpLevels )
  {
    TObjectOrder iAppl, jTask, globalTask, iNode, aux;
    // Only deselect those with higher levels deselected
    switch ( onLevel )
    {
      case TTraceLevel::TASK:
        for ( TTaskOrder iTask = 0; iTask < getTrace()->totalTasks(); ++iTask )
        {
          getTrace()->getTaskLocation( iTask, iAppl, aux );
          selected[ iTask ] = selectedRow.isSelectedPosition( iAppl, TTraceLevel::APPLICATION ) &&
                              selected[ iTask ];
        }

        break;

      case TTraceLevel::THREAD:
        for ( TThreadOrder iThread = 0; iThread < getTrace()->totalThreads(); ++iThread )
        {
          getTrace()->getThreadLocation( iThread, iAppl, jTask, aux );
          globalTask = getTrace()->getGlobalTask( iAppl, jTask );
          selected[ iThread ] = selectedRow.isSelectedPosition( iAppl, TTraceLevel::APPLICATION ) &&
                                selectedRow.isSelectedPosition( globalTask, TTraceLevel::TASK ) &&
                                selected[ iThread ];
        }

        break;

      case TTraceLevel::CPU:
        for ( TCPUOrder iCPU = 0; iCPU < getTrace()->totalCPUs(); ++iCPU )
        {
          getTrace()->getCPULocation( iCPU, iNode, aux );
          selected[ iCPU ] = selected[ iCPU ] &&
                             selectedRow.isSelectedPosition( iNode, TTraceLevel::NODE );
        }
        break;

      default:
        break;
    }
  }
}

void TimelineProxy::getSelectedRows( TTraceLevel onLevel, vector< bool > &selected,
                                     TObjectOrder first, TObjectOrder last, bool lookUpLevels )
{
  selectedRow.getSelected( selected, first, last, onLevel );

  if ( lookUpLevels )
  {
    TObjectOrder iAppl, jTask, globalTask, iNode, aux;
    switch ( onLevel )
    {
      case TTraceLevel::TASK:
        for ( TObjectOrder iTask = first; iTask <= last; ++iTask )
        {
          getTrace()->getTaskLocation( iTask, iAppl, aux );
          selected[ iTask ] = selectedRow.isSelectedPosition( iAppl, TTraceLevel::APPLICATION ) &&
                              selected[ iTask ];
        }

        break;

      case TTraceLevel::THREAD:
        for ( TObjectOrder iThread = first; iThread <= last; ++iThread )
        {
          getTrace()->getThreadLocation( iThread, iAppl, jTask, aux );
          globalTask = getTrace()->getGlobalTask( iAppl, jTask );
          selected[ iThread ] = selectedRow.isSelectedPosition( iAppl, TTraceLevel::APPLICATION ) &&
                                selectedRow.isSelectedPosition( globalTask, TTraceLevel::TASK ) &&
                                selected[ iThread ];
        }

        break;

      case TTraceLevel::CPU:
        for ( TObjectOrder iCPU = first; iCPU <= last; ++iCPU )
        {
          getTrace()->getCPULocation( iCPU, iNode, aux );
          selected[ iCPU ] = selected[ iCPU ] &&
                             selectedRow.isSelectedPosition( iNode, TTraceLevel::NODE );
        }
        break;

      default:
        break;
    }
  }
}


void TimelineProxy::getSelectedRows( TTraceLevel onLevel, vector< TObjectOrder > &selected, bool lookUpLevels )
{
  selectedRow.getSelected( selected, onLevel );

  if ( lookUpLevels )
  {
    SelectionRowsUtils::getAllLevelsSelectedRows( getTrace(), selectedRow, onLevel, selected );
  }
}

void TimelineProxy::getSelectedRows( TTraceLevel onLevel,
                                     vector< TObjectOrder > &selected,
                                     TObjectOrder first, TObjectOrder last, bool lookUpLevels )
{
  selectedRow.getSelected( selected, first, last, onLevel );

  if ( lookUpLevels )
  {
    SelectionRowsUtils::getAllLevelsSelectedRows( getTrace(), selectedRow, onLevel, selected );
  }
}

TObjectOrder TimelineProxy::shiftFirst( TObjectOrder whichFirst, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, TTraceLevel level ) const
{
  return selectedRow.shiftFirst( whichFirst, shiftAmount, appliedAmount, level );
}

TObjectOrder TimelineProxy::shiftLast( TObjectOrder whichLast, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, TTraceLevel level ) const
{
  return selectedRow.shiftLast( whichLast, shiftAmount, appliedAmount, level );
}

void TimelineProxy::getGroupLabels( PRV_UINT32 whichGroup, vector<string>& onVector ) const
{
  myWindow->getGroupLabels( whichGroup, onVector );
}

bool TimelineProxy::getParametersOfFunction( string whichFunction,
    PRV_UINT32 &numParameters,
    vector<string> &nameParameters,
    vector< vector< double > >&defaultParameters ) const
{
  return myWindow->getParametersOfFunction( whichFunction,
         numParameters,
         nameParameters,
         defaultParameters );
}

void TimelineProxy::setObjectLabels( TObjectLabels whichLabels )
{
  objectLabels = whichLabels;
}

TObjectLabels TimelineProxy::getObjectLabels() const
{
  return objectLabels;
}

void TimelineProxy::setObjectAxisSize( TObjectAxisSize whichSize )
{
  objectAxisSize = whichSize;
}

TObjectAxisSize TimelineProxy::getObjectAxisSize() const
{
  return objectAxisSize;
}


bool TimelineProxy::hasLevelSomeSelectedObject( TTraceLevel onLevel )
{
  std::vector< TObjectOrder > selection;

  selectedRow.getSelected( selection, onLevel );

  return ( selection.size() > 0 );
}


void TimelineProxy::setCFG4DMode( bool mode )
{
  if( isDerivedWindow() )
  {
    if ( parent1 != nullptr)
      parent1->setCFG4DMode( mode );

    if ( parent2 != nullptr)
      parent2->setCFG4DMode( mode );
  }

  CFG4DMode = mode;
}

bool TimelineProxy::getCFG4DMode( ) const
{
  return CFG4DMode;
}

bool TimelineProxy::getCFG4DEnabled() const
{
  return isCFG4DEnabled;
}

void TimelineProxy::setCFG4DEnabled( bool enabled )
{
  if( isDerivedWindow() )
  {
    if ( parent1 != nullptr)
      parent1->setCFG4DEnabled( enabled );

    if ( parent2 != nullptr)
      parent2->setCFG4DEnabled( enabled );
  }

  isCFG4DEnabled = enabled;
}

bool TimelineProxy::existsCFG4DAlias( const string &property ) const
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



bool TimelineProxy::existsCFG4DAlias( const TSingleTimelineProperties &propertyIndex ) const
{
  bool found = false;

  if ( propertiesAliasCFG4D.size() > 0 )
  {
    string property( SingleTimelinePropertyLabels[ propertyIndex ] );

    map< string, string >::const_iterator itAlias = propertiesAliasCFG4D.find( property );
    if ( itAlias != propertiesAliasCFG4D.end() )
    {
      found = true;
    }
  }

  return found;
}


bool TimelineProxy::existsCFG4DAlias( const TDerivedTimelineProperties &propertyIndex ) const
{
  bool found = false;

  if ( propertiesAliasCFG4D.size() > 0 )
  {
    string property( DerivedTimelinePropertyLabels[ propertyIndex ] );

    map< string, string >::const_iterator itAlias = propertiesAliasCFG4D.find( property );
    if ( itAlias != propertiesAliasCFG4D.end() )
    {
      found = true;
    }
  }

  return found;
}


string TimelineProxy::getCFG4DAlias( const string &property ) const
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


string TimelineProxy::getCFG4DAlias( const TSingleTimelineProperties &propertyIndex ) const
{
  string alias = "";

  if ( propertiesAliasCFG4D.size() > 0 )
  {
    string property( SingleTimelinePropertyLabels[ propertyIndex ] );

    map< string, string >::const_iterator itAlias = propertiesAliasCFG4D.find( property );
    if ( itAlias != propertiesAliasCFG4D.end() )
    {
      alias = itAlias->second;
    }
  }

  return alias;
}


string TimelineProxy::getCFG4DAlias( const TDerivedTimelineProperties &propertyIndex ) const
{
  string alias = "";

  if ( propertiesAliasCFG4D.size() > 0 )
  {
    string property( DerivedTimelinePropertyLabels[ propertyIndex ] );

    map< string, string >::const_iterator itAlias = propertiesAliasCFG4D.find( property );
    if ( itAlias != propertiesAliasCFG4D.end() )
    {
      alias = itAlias->second;
    }
  }

  return alias;
}


void TimelineProxy::setCFG4DAlias( const string &property, const string &alias )
{
  propertiesAliasCFG4D[ property ] = alias;
}


void TimelineProxy::setCFG4DAliasList( const map< string, string >& aliasList )
{
  propertiesAliasCFG4D = aliasList;
}


const map< string, string > TimelineProxy::getCFG4DAliasList() const
{
  return propertiesAliasCFG4D;
}

const vector< string > TimelineProxy::getCFG4DFullTagList()
{
  vector< string > tags;

  if ( isDerivedWindow() )
  {
    for ( int iTag = 0; iTag < TOTAL_DERIVED_PROPERTIES; ++iTag )
    {
      tags.push_back( DerivedTimelinePropertyLabels[ iTag ] );
    }
  }
  else
  {
    for ( int iTag = 0; iTag < TOTAL_SINGLE_PROPERTIES; ++iTag )
    {
      tags.push_back( SingleTimelinePropertyLabels[ iTag ] );
    }
  }

  return tags;
}


const vector< Timeline::TParamAliasKey > TimelineProxy::getCFG4DCurrentSelectedFullParamList()
{
  vector< TParamAliasKey > retKeys;

  TWindowLevel curLevel, beginLevel, endLevel, beginCompose, endCompose;

  string semanticLevel, semanticFunction;
  PRV_UINT32 numParameter;
  TParamAliasKey key;

  beginLevel = static_cast<TWindowLevel>( getLevel() );

  switch ( beginLevel )
  {
    case WORKLOAD:
    case APPLICATION:
    case TASK:
    case THREAD:
      if ( isDerivedWindow() )
      {
        endLevel = TASK;
        beginCompose = COMPOSEWORKLOAD;
        endCompose = COMPOSETHREAD;
      }
      else
      {
        endLevel = THREAD;
        beginCompose = COMPOSEWORKLOAD;
        endCompose = COMPOSETHREAD;
      }
      break;

    case SYSTEM:
    case NODE:
    case CPU:
      endLevel = CPU;
      beginCompose = COMPOSESYSTEM;
      endCompose = COMPOSECPU;
      break;

    default:
      break;
  }

  for( int level = TOPCOMPOSE1; level <= TOPCOMPOSE2; ++level )
  {
    curLevel = static_cast< TWindowLevel >( level );
    semanticLevel = TimelineLevelLabels[ curLevel ];
    semanticFunction = getLevelFunction( curLevel );
    numParameter = getFunctionNumParam( curLevel );
    for( PRV_UINT32 i = 0; i < (PRV_UINT32)numParameter; ++i )
    {
      key = make_pair( make_pair( semanticLevel, semanticFunction ), i );
      retKeys.push_back( key );
    }
  }

  for( int level = beginCompose; level <= endCompose; ++level )
  {
    curLevel = static_cast< TWindowLevel >( level );
    semanticLevel = TimelineLevelLabels[ curLevel ];
    semanticFunction = getLevelFunction( curLevel );
    numParameter = getFunctionNumParam( curLevel );
    for( PRV_UINT32 i = 0; i < (PRV_UINT32)numParameter; ++i )
    {
      key = make_pair( make_pair( semanticLevel, semanticFunction ), i );
      retKeys.push_back( key );
    }
  }

  for( int level = beginLevel; level <= endLevel; ++level )
  {
    curLevel = static_cast< TWindowLevel >( level );
    semanticLevel = TimelineLevelLabels[ curLevel ];
    semanticFunction = getLevelFunction( curLevel );
    numParameter = getFunctionNumParam( curLevel );
    for( PRV_UINT32 i = 0; i < (PRV_UINT32)numParameter; ++i )
    {
      key = make_pair( make_pair( semanticLevel, semanticFunction ), i );
      retKeys.push_back( key );
    }
  }

  return retKeys;
}


void TimelineProxy::setCFG4DParamAlias( const TParamAlias &whichParamAlias )
{
  paramAliasCFG4D = whichParamAlias;
}


void TimelineProxy::setCFG4DParamAlias( string semanticLevel,
                                      string function,
                                      PRV_UINT32 numParameter,
                                      string paramAlias )
{
  TParamAliasKey key( make_pair( make_pair( semanticLevel, function ), numParameter ) );
  paramAliasCFG4D[ key ] = paramAlias;
}

const Timeline::TParamAlias TimelineProxy::getCFG4DParamAliasList() const
{
  return paramAliasCFG4D;
}

void TimelineProxy::splitCFG4DParamAliasKey( const TParamAliasKey &pk,
                                             string &semanticLevel,
                                             string &function,
                                             TParamIndex &numParameter ) const
{
  semanticLevel = pk.first.first;
  function = pk.first.second;
  numParameter = pk.second;
}


void TimelineProxy::splitCFG4DParamAliasKey( const TParamAliasKey &pk,
                                             TWindowLevel &semanticLevel,
                                             string &function,
                                             TParamIndex &numParameter ) const
{
  string tmpSemanticLevel = pk.first.first;
  int iSemLevel;
  for( iSemLevel = 0; iSemLevel < DERIVED; ++iSemLevel )
  {
    if( TimelineLevelLabels[ iSemLevel ] == tmpSemanticLevel )
      break;
  }

  semanticLevel = TWindowLevel( iSemLevel );
  function = pk.first.second;
  numParameter = pk.second;
}


const Timeline::TParamAliasKey TimelineProxy::buildCFG4DParamAliasKey( const string &semanticLevel,
                                                                       const string &function,
                                                                       const TParamIndex &numParameter ) const
{
  return TParamAliasKey( make_pair( make_pair( semanticLevel, function ), numParameter ) );
}


std::string TimelineProxy::getCFG4DParameterOriginalName( TWindowLevel whichLevel, TParamIndex whichParam ) const
{
  sstrCFGS4DOriginalName.clear();
  sstrCFGS4DOriginalName.str( "" );
  sstrCFGS4DOriginalName << TimelineLevelLabels[ whichLevel ] << PARAM_SEPARATOR << whichParam << PARAM_SEPARATOR
                         << getLevelFunction( whichLevel ) << "." << getFunctionParamName( whichLevel, whichParam );

  return sstrCFGS4DOriginalName.str();
}


Timeline::TParamAliasKey TimelineProxy::getCFG4DParamAliasKey( const TParamAlias::iterator it ) const
{
  return it->first;
}

const string TimelineProxy::getCFG4DParamAlias( const TParamAlias::iterator &it ) const
{
  return it->second;
}


const string TimelineProxy::getCFG4DParamAlias( const TParamAliasKey &pk ) const
{
  TParamAlias::const_iterator it = paramAliasCFG4D.find( pk );
  return ( it != paramAliasCFG4D.end()? it->second:string("") );
}


vector< Timeline::TParamAliasKey > TimelineProxy::getCFG4DParamKeysBySemanticLevel( string whichSemanticLevel,
                                                                                const vector< Timeline::TParamAliasKey > &whichParamAliasKey ) const
{
  vector< TParamAliasKey > retKeys;
  string semanticLevel, function;
  TParamIndex numParameter;

  // change to a single class, inside Window
  if ( whichParamAliasKey.size() > 0 )
  {
    for( vector< TParamAliasKey >::const_iterator it = whichParamAliasKey.begin(); it != whichParamAliasKey.end(); ++it )
    {
      splitCFG4DParamAliasKey( *it, semanticLevel, function, numParameter );
      if ( semanticLevel == whichSemanticLevel )
      {
        retKeys.push_back( *it );
      }
    }
  }
  else
  {
    for( TParamAlias::const_iterator it = paramAliasCFG4D.begin(); it != paramAliasCFG4D.end(); ++it )
    {
      splitCFG4DParamAliasKey( it->first, semanticLevel, function, numParameter );
      
      if ( semanticLevel == whichSemanticLevel )
      {
        retKeys.push_back( (Timeline::TParamAliasKey) it->first );
      }
    }
  }

  return retKeys;
}


void TimelineProxy::setCFGS4DIndexLink( TCFGS4DIndexLink whichIndex )
{
  globalIndexLink = whichIndex;
}

TCFGS4DIndexLink TimelineProxy::getCFGS4DIndexLink() const
{
  return globalIndexLink;
}

void TimelineProxy::setCFGS4DGroupLink( std::string originalName, TCFGS4DGroup whichGroup )
{
  groupLinkFromPropName[ originalName ] = whichGroup;
}

TCFGS4DGroup TimelineProxy::getCFGS4DGroupLink( std::string originalName ) const
{
  map< string, TCFGS4DGroup >::const_iterator it = groupLinkFromPropName.find( originalName );
  if( it != groupLinkFromPropName.end() )
    return it->second;

  return NO_GROUP_LINK;
}

#ifdef _MSC_VER
void TimelineProxy::computeSemanticParallel( vector< TObjectOrder >& selectedSet,
                                           vector< bool >& selected,
                                           TTime timeStep,
                                           PRV_INT32 timePos,
                                           PRV_INT32 objectAxisPos,
                                           vector< PRV_INT32 >& objectPosList,
                                           TObjectOrder maxObj,
                                           bool& drawCaution,
                                           vector< vector< TSemanticValue > >& valuesToDraw,
                                           vector< hash_set< PRV_INT32 > >& eventsToDraw,
                                           vector< hash_set< commCoord > >& commsToDraw,
                                           ProgressController *progress )
#else
void TimelineProxy::computeSemanticParallel( vector< TObjectOrder >& selectedSet,
                                           vector< bool >& selected,
                                           TTime timeStep,
                                           PRV_INT32 timePos,
                                           PRV_INT32 objectAxisPos,
                                           vector< PRV_INT32 >& objectPosList,
                                           TObjectOrder maxObj,
                                           bool& drawCaution,
                                           vector< vector< TSemanticValue > >& valuesToDraw,
                                           vector< unordered_set< PRV_INT32 > >& eventsToDraw,
                                           vector< unordered_set< commCoord, hashCommCoord > >& commsToDraw,
                                           ProgressController *progress )
#endif
{
  vector< int > tmpDrawCaution;
  vector< TSemanticValue > tmpComputedMaxY;
  vector< TSemanticValue > tmpComputedMinY;
  vector< int > tmpComputedZeros;
  ProgressController *paramProgress = nullptr;

  if( getWindowBeginTime() == getWindowEndTime() )
    return;

  int numRows = 0;
  if( isFusedLinesColorSet() )
  {
    numRows = selectedSet.size();
  }
  else
  {
    for( vector< TObjectOrder >::iterator obj = selectedSet.begin(); obj != selectedSet.end(); ++obj )
    {
      TObjectOrder firstObj = *obj;
      TObjectOrder lastObj = firstObj;
      while( ( lastObj + 1 ) <= maxObj && objectPosList[ lastObj + 1 ] == objectPosList[ firstObj ] )
      {
        ++obj;
        lastObj = *obj;
      }

      ++numRows;
    }
  }

  valuesToDraw.reserve( numRows );
  eventsToDraw.reserve( numRows );
  commsToDraw.reserve( numRows );
  tmpDrawCaution.reserve( numRows );
  tmpComputedMaxY.reserve( numRows );
  tmpComputedMinY.reserve( numRows );
  tmpComputedZeros.reserve( numRows );

  paramProgress = progress;

  if ( progress != nullptr )
  {
    if( numRows > 1 )
      progress->setEndLimit( numRows );
    else if( numRows == 1 )
      progress->setEndLimit( getWindowEndTime() - getWindowBeginTime() );

    progress->setCurrentProgress( 0 );
  }

  // Drawmode: Group objects with same wxCoord in objectPosList
  #pragma omp parallel
  {
    #pragma omp single
    {
#ifdef PARALLEL_ENABLED
      if( selected.size() > 1 ||
          ( myWindow->isDerivedWindow() && myWindow->getTrace()->getLevelObjects( myWindow->getParent( 0 )->getLevel() ) !=
                                           myWindow->getTrace()->getLevelObjects( myWindow->getParent( 1 )->getLevel() )
          )
        )
      {
        for( int i = 0; i != omp_get_num_threads(); ++i )
          parallelClone.push_back( myWindow->clone( true ) );
      }
#endif // PARALLEL_ENABLED

      int currentRow = 0;
      for( vector< TObjectOrder >::iterator obj = selectedSet.begin(); obj != selectedSet.end(); ++obj )
      {
        TObjectOrder firstObj = *obj;
        TObjectOrder lastObj = firstObj;
        if( !isFusedLinesColorSet() )
        {
          while( ( lastObj + 1 ) <= maxObj && objectPosList[ lastObj + 1 ] == objectPosList[ firstObj ] )
          {
            ++obj;
            lastObj = *obj;
          }
        }
        valuesToDraw.push_back( vector< TSemanticValue >() );

        eventsToDraw.push_back( unordered_set< PRV_INT32 >() );
#ifdef _MSC_VER
        commsToDraw.push_back( hash_set< commCoord >() );
#else
        commsToDraw.push_back( unordered_set< commCoord, hashCommCoord >() );
#endif

        tmpDrawCaution.push_back( drawCaution );
        tmpComputedMaxY.push_back( 0.0 );
        tmpComputedMinY.push_back( 0.0 );
        tmpComputedZeros.push_back( false );

        int tmpDrawCautionSize = tmpDrawCaution.size();
        int tmpComputedMaxYSize = tmpComputedMaxY.size();
        int tmpComputedMinYSize = tmpComputedMinY.size();
        int tmpComputedZerosSize = tmpComputedZeros.size();
        int valuesToDrawSize = valuesToDraw.size();
        int eventsToDrawSize = eventsToDraw.size();
        int commsToDrawSize = eventsToDraw.size();

        if( numRows == 1 )
        {
          computeSemanticRowParallel(
                  numRows, firstObj, lastObj, selectedSet, selected, timeStep, timePos,
                  objectAxisPos, objectPosList,
                  tmpDrawCaution[ tmpDrawCautionSize - 1 ],
                  tmpComputedMaxY[ tmpComputedMaxYSize - 1 ],
                  tmpComputedMinY[ tmpComputedMinYSize - 1 ],
                  tmpComputedZeros[ tmpComputedZerosSize - 1 ],
                  valuesToDraw[ valuesToDrawSize - 1 ],
                  eventsToDraw[ eventsToDrawSize - 1 ],
                  commsToDraw[ commsToDrawSize - 1 ],
                  paramProgress );
        }
        else if( numRows > 1 )
        {
          #pragma omp task firstprivate(numRows, firstObj, lastObj, timeStep, timePos, objectAxisPos) \
                          shared(currentRow, paramProgress, selectedSet, selected, objectPosList, tmpDrawCaution, tmpComputedMaxY, tmpComputedMinY, tmpComputedZeros, valuesToDraw, eventsToDraw, commsToDraw) \
                          firstprivate(tmpDrawCautionSize, tmpComputedMaxYSize, tmpComputedMinYSize, tmpComputedZerosSize, valuesToDrawSize, eventsToDrawSize, commsToDrawSize) \
                          default(none)
          {
            if( paramProgress == nullptr ||
                ( paramProgress != nullptr && !paramProgress->getStop() ) )
            {
              computeSemanticRowParallel(
                      numRows, firstObj, lastObj, selectedSet, selected, timeStep, timePos,
                      objectAxisPos, objectPosList,
                      tmpDrawCaution[ tmpDrawCautionSize - 1 ],
                      tmpComputedMaxY[ tmpComputedMaxYSize - 1 ],
                      tmpComputedMinY[ tmpComputedMinYSize - 1 ],
                      tmpComputedZeros[ tmpComputedZerosSize - 1 ],
                      valuesToDraw[ valuesToDrawSize - 1 ],
                      eventsToDraw[ eventsToDrawSize - 1 ],
                      commsToDraw[ commsToDrawSize - 1 ],
                      paramProgress );
            }

            if( paramProgress != nullptr && !paramProgress->getStop() )
            {
              #pragma omp critical
              paramProgress->setCurrentProgress( ++currentRow );
            }

          } // end omp task

        } // end if numRows

      } // end for selectedSet

    } // end omp single
  } // end omp parallel

  for( size_t pos = 0; pos < tmpComputedMaxY.size(); ++pos )
  {
    drawCaution = drawCaution || tmpDrawCaution[ pos ];
    computedZeros = computedZeros || tmpComputedZeros[ pos ];
    computedMaxY = computedMaxY > tmpComputedMaxY[ pos ] ? computedMaxY : tmpComputedMaxY[ pos ];
    if ( computedMinY == 0.0 )
      computedMinY = tmpComputedMinY[ pos ];
    else if( tmpComputedMinY[ pos ] != 0.0 )
      computedMinY = computedMinY < tmpComputedMinY[ pos ] ? computedMinY : tmpComputedMinY[ pos ];
  }

#ifdef PARALLEL_ENABLED
  for( vector<Timeline *>::iterator it = parallelClone.begin(); it != parallelClone.end(); ++it )
    delete *it;
  parallelClone.clear();
#endif // PARALLEL_ENABLED
}

#ifdef _MSC_VER
void TimelineProxy::computeSemanticRowParallel( int numRows,
                                              TObjectOrder firstRow,
                                              TObjectOrder lastRow,
                                              vector< TObjectOrder >& selectedSet,
                                              vector< bool >& selected,
                                              TTime timeStep,
                                              PRV_INT32 timePos,
                                              PRV_INT32 objectAxisPos,
                                              vector< PRV_INT32 >& objectPosList,
                                              int& drawCaution,
                                              TSemanticValue& rowComputedMaxY,
                                              TSemanticValue& rowComputedMinY,
                                              int& rowComputedZeros,
                                              vector< TSemanticValue >& valuesToDraw,
                                              hash_set< PRV_INT32 >& eventsToDraw,
                                              hash_set< commCoord >& commsToDraw,
                                              ProgressController *progress )
#else
void TimelineProxy::computeSemanticRowParallel( int numRows,
                                              TObjectOrder firstRow,
                                              TObjectOrder lastRow,
                                              vector< TObjectOrder >& selectedSet,
                                              vector< bool >& selected,
                                              TTime timeStep,
                                              PRV_INT32 timePos,
                                              PRV_INT32 objectAxisPos,
                                              vector< PRV_INT32 >& objectPosList,
                                              int& drawCaution,
                                              TSemanticValue& rowComputedMaxY,
                                              TSemanticValue& rowComputedMinY,
                                              int& rowComputedZeros,
                                              vector< TSemanticValue >& valuesToDraw,
                                              unordered_set< PRV_INT32 >& eventsToDraw,
                                              unordered_set< commCoord, hashCommCoord >& commsToDraw,
                                              ProgressController *progress )
#endif
{
  float magnify = float( getPixelSize() );

  vector<TSemanticValue> timeValues;
  vector<TSemanticValue> rowValues;

  vector<TObjectOrder>::iterator first = find( selectedSet.begin(), selectedSet.end(), firstRow );
  vector<TObjectOrder>::iterator last  = find( selectedSet.begin(), selectedSet.end(), lastRow );

  TRecordTime tmpLastTime = getWindowBeginTime();

  for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
  {
    if( isFusedLinesColorSet() )
      initRow( *row, getWindowBeginTime(), NOCREATE, rowComputedMaxY, rowComputedMinY, rowComputedZeros );
    else
      initRow( *row, getWindowBeginTime(), CREATECOMMS + CREATEEVENTS, rowComputedMaxY, rowComputedMinY, rowComputedZeros );
  }

  TTime currentTime = getWindowBeginTime() + timeStep;
  while( currentTime <= getWindowEndTime() && currentTime <= getTrace()->getEndTime() )
  {
    rowValues.clear();
    for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
    {
      timeValues.clear();

      while( getEndTime( *row ) <= currentTime - timeStep )
        calcNext( *row, rowComputedMaxY, rowComputedMinY, rowComputedZeros );

      timeValues.push_back( getValue( *row ) );
      while( getEndTime( *row ) < currentTime )
      {
        // Making cancel button more responsive for 1 row drawing
        if( numRows == 1 && progress != nullptr && progress->getStop() )
          break;

        calcNext( *row, rowComputedMaxY, rowComputedMinY, rowComputedZeros );
        TSemanticValue currentValue = getValue( *row );
        timeValues.push_back( currentValue );
        if( currentValue != 0 && ( currentValue < getMinimumY()
                                   || currentValue > getMaximumY() ) )
          drawCaution = true;
      }
      rowValues.push_back( DrawMode::selectValue( timeValues, getDrawModeTime() ) );

      RecordList *rl = getRecordList( *row );
      if( rl != nullptr && !isFusedLinesColorSet() )
        computeEventsCommsParallel( rl,
                                    currentTime - timeStep, currentTime, timeStep / magnify,
                                    timePos, objectAxisPos,
                                    selected, objectPosList,
                                    eventsToDraw, commsToDraw );
    }
    valuesToDraw.push_back( DrawMode::selectValue( rowValues, getDrawModeObject() ) );
    timePos += (int) magnify;

    if( progress != nullptr )
    {
      if( progress->getStop() )
        break;
      if( numRows == 1 )
      {
        if( currentTime - tmpLastTime > ( getWindowEndTime() - getWindowBeginTime() ) / 50 )
        {
          #pragma omp critical
          {
            progress->setCurrentProgress( currentTime - getWindowBeginTime() );
          }
          tmpLastTime = currentTime;
        }
      }
    }

    if( currentTime == myTrace->getEndTime() )
      currentTime += timeStep;
    else
    {
      currentTime += timeStep;
      if( currentTime > myTrace->getEndTime() )
        currentTime = myTrace->getEndTime();
    }
  }

  for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
  {
    TSemanticValue dumbMinMax = 0.0;
    int dumbZeros = 0;
    calcNext( *row, dumbMinMax, dumbMinMax, dumbZeros );
    RecordList *rl = getRecordList( *row );
    if( rl != nullptr && !isFusedLinesColorSet() )
      computeEventsCommsParallel( rl,
                                  currentTime - timeStep, currentTime, timeStep / magnify,
                                  timePos, objectAxisPos,
                                  selected, objectPosList,
                                  eventsToDraw, commsToDraw );
  }

  // Erase events and comms remaining in RecordLists
  for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
  {
    RecordList *rl = getRecordList( *row );
    rl->erase( rl->begin(), rl->end() );
  }
}

#ifdef _MSC_VER
void TimelineProxy::computeEventsCommsParallel( RecordList *records,
                                              TTime from,
                                              TTime to,
                                              TTime step,
                                              PRV_INT32 timePos,
                                              PRV_INT32 objectAxisPos,
                                              vector< bool >& selected,
                                              vector< PRV_INT32 >& objectPosList,
                                              hash_set< PRV_INT32 >& eventsToDraw,
                                              hash_set< commCoord >& commsToDraw )
#else
void TimelineProxy::computeEventsCommsParallel( RecordList *records,
                                              TTime from,
                                              TTime to,
                                              TTime step,
                                              PRV_INT32 timePos,
                                              PRV_INT32 objectAxisPos,
                                              vector< bool >& selected,
                                              vector< PRV_INT32 >& objectPosList,
                                              unordered_set< PRV_INT32 >& eventsToDraw,
                                              unordered_set< commCoord, hashCommCoord >& commsToDraw )
#endif
{
  bool existEvents = false;
  TObjectOrder beginRow = getZoomSecondDimension().first;
  TObjectOrder endRow =  getZoomSecondDimension().second;

  RecordList::iterator it = records->begin();

  step = ( 1 / step );

  while( it != records->end() && it->getTime() < from )
    ++it;
  while( it != records->end() && it->getTime() <= to )
  {
    TRecordType recType = it->getType();

    if( recType & EVENT )
      existEvents = true;
    else
    {
      TObjectOrder partnerObject;
      if ( getLevel() >= TTraceLevel::WORKLOAD && getLevel() <= TTraceLevel::THREAD )
        partnerObject = threadObjectToWindowObject( it->getCommPartnerObject() );
      else if( getLevel() >= TTraceLevel::SYSTEM && getLevel() <= TTraceLevel::NODE )
        partnerObject = cpuObjectToWindowObject( it->getCommPartnerObject() );
      else //CPU
        partnerObject = cpuObjectToWindowObject( it->getCommPartnerObject() ) - 1;

      if( ( recType & COMM ) &&
          /*partnerObject >= beginRow && partnerObject <= endRow &&*/ selected[ partnerObject ] &&
          ( ( recType & RECV ) ||
            ( ( recType & SEND ) && it->getCommPartnerTime() > getWindowEndTime() ) )
        )
      {
        PRV_INT32 posPartner = ( ( it->getCommPartnerTime() - getWindowBeginTime() ) * step );
        posPartner += objectAxisPos;
        if( posPartner > 10000 )
          posPartner = 10000;
        else if( posPartner < -10000 )
          posPartner = -10000;
        commCoord tmpComm;
        tmpComm.recType = recType;
        tmpComm.fromTime = timePos;
        tmpComm.toTime = posPartner;
        PRV_INT32 tmpPixelDensity;
        if( endRow - beginRow > 0 )
          tmpPixelDensity = objectPosList[ endRow ] - objectPosList[ beginRow ] + 1;
        else
          tmpPixelDensity = 100;

        if( partnerObject >= beginRow && partnerObject <= endRow )
          tmpComm.toRow = objectPosList[ partnerObject ];
        else if( partnerObject < beginRow )
          tmpComm.toRow = 0 - ( beginRow - partnerObject ) * tmpPixelDensity;
        else if( partnerObject > endRow )
          tmpComm.toRow = objectPosList[ endRow ] + ( partnerObject - endRow ) * tmpPixelDensity;

        if( tmpComm.toRow > 10000 )
          tmpComm.toRow = 10000;
        else if( tmpComm.toRow < -10000 )
          tmpComm.toRow = -10000;

        commsToDraw.insert( tmpComm );
      }
    }
    ++it;
  }

  if( existEvents )
    eventsToDraw.insert( timePos );

  records->erase( records->begin(), it );
}

#ifdef _MSC_VER
void TimelineProxy::computeSemanticPunctualParallel( vector< TObjectOrder >& selectedSet,
                                                   vector< bool >& selected,
                                                   TTime timeStep,
                                                   PRV_INT32 timePos,
                                                   PRV_INT32 objectAxisPos,
                                                   vector< PRV_INT32 >& objectPosList,
                                                   TObjectOrder maxObj,
                                                   bool& drawCaution,
                                                   vector< vector< vector< pair<TSemanticValue,TSemanticValue> > > >& valuesToDraw,
                                                   vector< hash_set< PRV_INT32 > >& eventsToDraw,
                                                   vector< hash_set< commCoord > >& commsToDraw,
                                                   ProgressController *progress )
#else
void TimelineProxy::computeSemanticPunctualParallel( vector< TObjectOrder >& selectedSet,
                                                   vector< bool >& selected,
                                                   TTime timeStep,
                                                   PRV_INT32 timePos,
                                                   PRV_INT32 objectAxisPos,
                                                   vector< PRV_INT32 >& objectPosList,
                                                   TObjectOrder maxObj,
                                                   bool& drawCaution,
                                                   vector< vector< vector< pair<TSemanticValue,TSemanticValue> > > >& valuesToDraw,
                                                   vector< unordered_set< PRV_INT32 > >& eventsToDraw,
                                                   vector< unordered_set< commCoord, hashCommCoord > >& commsToDraw,
                                                   ProgressController *progress )
#endif
{
  vector< int > tmpDrawCaution;
  vector< TSemanticValue > tmpComputedMaxY;
  vector< TSemanticValue > tmpComputedMinY;
  vector< int > tmpComputedZeros;
  ProgressController *paramProgress = nullptr;

  if( getWindowBeginTime() == getWindowEndTime() )
    return;

  int numRows = 0;
  for( vector< TObjectOrder >::iterator obj = selectedSet.begin(); obj != selectedSet.end(); ++obj )
  {
    TObjectOrder firstObj = *obj;
    TObjectOrder lastObj = firstObj;
    while( ( lastObj + 1 ) <= maxObj && objectPosList[ lastObj + 1 ] == objectPosList[ firstObj ] )
    {
      ++obj;
      lastObj = *obj;
    }

    ++numRows;
  }

  valuesToDraw.reserve( numRows );
  eventsToDraw.reserve( numRows );
  commsToDraw.reserve( numRows );
  tmpDrawCaution.reserve( numRows );
  tmpComputedMaxY.reserve( numRows );
  tmpComputedMinY.reserve( numRows );
  tmpComputedZeros.reserve( numRows );

#ifndef PARALLEL_ENABLED
  paramProgress = progress;
  if( progress != nullptr )
  {
    if( numRows > 1 )
      progress->setEndLimit( numRows );
    else
      progress->setEndLimit( getWindowEndTime() - getWindowBeginTime() );
  }
#endif // PARALLEL_ENABLED

  // Drawmode: Group objects with same wxCoord in objectPosList
  #pragma omp parallel
  {
    #pragma omp single
    {
#ifdef PARALLEL_ENABLED
      if( selected.size() > 1 ||
          ( myWindow->isDerivedWindow() && myWindow->getTrace()->getLevelObjects( myWindow->getParent( 0 )->getLevel() ) !=
                                           myWindow->getTrace()->getLevelObjects( myWindow->getParent( 1 )->getLevel() )
          )
        )
      {
        for( int i = 0; i != omp_get_num_threads(); ++i )
          parallelClone.push_back( myWindow->clone( true ) );
      }
#endif // PARALLEL_ENABLED
      int currentRow = 0;
      for( vector< TObjectOrder >::iterator obj = selectedSet.begin(); obj != selectedSet.end(); ++obj )
      {
        TObjectOrder firstObj = *obj;
        TObjectOrder lastObj = firstObj;
        while( ( lastObj + 1 ) <= maxObj && objectPosList[ lastObj + 1 ] == objectPosList[ firstObj ] )
        {
          ++obj;
          lastObj = *obj;
        }
        valuesToDraw.push_back( vector< vector< pair<TSemanticValue,TSemanticValue> > >() );

        eventsToDraw.push_back( unordered_set< PRV_INT32 >() );
#ifdef _MSC_VER
        commsToDraw.push_back( hash_set< commCoord >() );
#else
        commsToDraw.push_back( unordered_set< commCoord, hashCommCoord >() );
#endif

        tmpDrawCaution.push_back( drawCaution );
        tmpComputedMaxY.push_back( 0.0 );
        tmpComputedMinY.push_back( 0.0 );
        tmpComputedZeros.push_back( false );

        int tmpDrawCautionSize = tmpDrawCaution.size();
        int tmpComputedMaxYSize = tmpComputedMaxY.size();
        int tmpComputedMinYSize = tmpComputedMinY.size();
        int tmpComputedZerosSize = tmpComputedZeros.size();
        int valuesToDrawSize = valuesToDraw.size();
        int eventsToDrawSize = eventsToDraw.size();
        int commsToDrawSize = eventsToDraw.size();

        #pragma omp task firstprivate(numRows, firstObj, lastObj, timeStep, timePos, objectAxisPos, paramProgress) \
                        shared(selectedSet, selected, objectPosList, tmpDrawCaution, tmpComputedMaxY, tmpComputedMinY, tmpComputedZeros, valuesToDraw, eventsToDraw, commsToDraw) \
                        firstprivate(tmpDrawCautionSize, tmpComputedMaxYSize, tmpComputedMinYSize, tmpComputedZerosSize, valuesToDrawSize, eventsToDrawSize, commsToDrawSize) \
                        default(none)
        {
            computeSemanticRowPunctualParallel(
                    numRows, firstObj, lastObj, selectedSet, selected, timeStep, timePos,
                    objectAxisPos, objectPosList,
                    tmpDrawCaution[ tmpDrawCautionSize - 1 ],
                    tmpComputedMaxY[ tmpComputedMaxYSize - 1 ],
                    tmpComputedMinY[ tmpComputedMinYSize - 1 ],
                    tmpComputedZeros[ tmpComputedZerosSize - 1 ],
                    valuesToDraw[ valuesToDrawSize - 1 ],
                    eventsToDraw[ eventsToDrawSize - 1 ],
                    commsToDraw[ commsToDrawSize - 1 ],
                    paramProgress );
        }

#ifndef PARALLEL_ENABLED
        if( numRows > 1 && progress != nullptr )
        {
          if( progress->getStop() )
            break;
          progress->setCurrentProgress( currentRow );
        }
        ++currentRow;
#endif // PARALLEL_ENABLED
      }
    }
  }

  for( vector< int >::iterator it = tmpDrawCaution.begin(); it != tmpDrawCaution.end(); ++it )
  {
    if ( *it )
    {
      drawCaution = true;
      break;
    }
  }

  for( size_t pos = 0; pos < tmpComputedMaxY.size(); ++pos )
  {
    drawCaution = drawCaution || tmpDrawCaution[ pos ];
    computedZeros = computedZeros || tmpComputedZeros[ pos ];
    computedMaxY = computedMaxY > tmpComputedMaxY[ pos ] ? computedMaxY : tmpComputedMaxY[ pos ];
    if ( computedMinY == 0.0 )
      computedMinY = tmpComputedMinY[ pos ];
    else if( tmpComputedMinY[ pos ] != 0.0 )
      computedMinY = computedMinY < tmpComputedMinY[ pos ] ? computedMinY : tmpComputedMinY[ pos ];
  }

#ifdef PARALLEL_ENABLED
  for( vector<Timeline *>::iterator it = parallelClone.begin(); it != parallelClone.end(); ++it )
    delete *it;
  parallelClone.clear();
#endif // PARALLEL_ENABLED
}

#ifdef _MSC_VER
void TimelineProxy::computeSemanticRowPunctualParallel( int numRows,
                                                      TObjectOrder firstRow,
                                                      TObjectOrder lastRow,
                                                      vector< TObjectOrder >& selectedSet,
                                                      vector< bool >& selected,
                                                      TTime timeStep,
                                                      PRV_INT32 timePos,
                                                      PRV_INT32 objectAxisPos,
                                                      vector< PRV_INT32 >& objectPosList,
                                                      int& drawCaution,
                                                      TSemanticValue& rowComputedMaxY,
                                                      TSemanticValue& rowComputedMinY,
                                                      int& rowComputedZeros,
                                                      vector< vector< pair<TSemanticValue,TSemanticValue> > >& valuesToDraw,
                                                      hash_set< PRV_INT32 >& eventsToDraw,
                                                      hash_set< commCoord >& commsToDraw,
                                                      ProgressController *progress )
#else
void TimelineProxy::computeSemanticRowPunctualParallel( int numRows,
                                                      TObjectOrder firstRow,
                                                      TObjectOrder lastRow,
                                                      vector< TObjectOrder >& selectedSet,
                                                      vector< bool >& selected,
                                                      TTime timeStep,
                                                      PRV_INT32 timePos,
                                                      PRV_INT32 objectAxisPos,
                                                      vector< PRV_INT32 >& objectPosList,
                                                      int& drawCaution,
                                                      TSemanticValue& rowComputedMaxY,
                                                      TSemanticValue& rowComputedMinY,
                                                      int& rowComputedZeros,
                                                      vector< vector< pair<TSemanticValue,TSemanticValue> > >& valuesToDraw,
                                                      unordered_set< PRV_INT32 >& eventsToDraw,
                                                      unordered_set< commCoord, hashCommCoord >& commsToDraw,
                                                      ProgressController *progress )
#endif
{
  float magnify = float( getPixelSize() );

  vector<pair<TSemanticValue,TSemanticValue> > values;
  pair<TSemanticValue,TSemanticValue> tmpPairSemantic;

  vector<TObjectOrder>::iterator first = find( selectedSet.begin(), selectedSet.end(), firstRow );
  vector<TObjectOrder>::iterator last  = find( selectedSet.begin(), selectedSet.end(), lastRow );

  TSemanticValue dummyMaxY, dummyMinY;
  int dummyZeros;
  for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
  {
    initRow( *row, getWindowBeginTime(), CREATECOMMS + CREATEEVENTS, rowComputedMaxY, rowComputedMinY, rowComputedZeros );
    if( punctualColorWindow != nullptr )
      punctualColorWindow->initRow( *row, getWindowBeginTime(), NOCREATE, dummyMaxY, dummyMinY, dummyZeros, false );
  }

  TTime currentTime;
  for( currentTime = getWindowBeginTime() + timeStep;
       currentTime <= getWindowEndTime() && currentTime <= getTrace()->getEndTime();
       currentTime += timeStep )
  {
    values.clear();
    for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
    {
      while( getEndTime( *row ) <= currentTime - timeStep )
        calcNext( *row, rowComputedMaxY, rowComputedMinY, rowComputedZeros );

      if( getBeginTime( *row ) >= currentTime - timeStep && getBeginTime( *row ) < currentTime )
      {
        tmpPairSemantic.first = getValue( *row );
        if( punctualColorWindow != nullptr )
        {
          while( getBeginTime( *row ) >= punctualColorWindow->getEndTime( *row ) )
            punctualColorWindow->calcNext( *row, dummyMaxY, dummyMinY, dummyZeros );

          vector<TSemanticValue> tmpValues;
          while( getEndTime( *row ) >= punctualColorWindow->getEndTime( *row ) )
          {
            tmpValues.push_back( punctualColorWindow->getValue( *row ) );
            punctualColorWindow->calcNext( *row, dummyMaxY, dummyMinY, dummyZeros );
          }

          if( tmpValues.empty() )
            tmpValues.push_back( punctualColorWindow->getValue( *row ) );

          tmpPairSemantic.second = DrawMode::selectValue( tmpValues, getDrawModeTime() );
        }
        values.push_back( tmpPairSemantic );
      }

      while( getEndTime( *row ) < currentTime )
      {
        calcNext( *row, rowComputedMaxY, rowComputedMinY, rowComputedZeros );
        TSemanticValue currentValue = getValue( *row );

        if( getBeginTime( *row ) >= currentTime - timeStep && getBeginTime( *row ) < currentTime )
        {
          tmpPairSemantic.first = currentValue;
          if( punctualColorWindow != nullptr )
          {
            while( getBeginTime( *row ) >= punctualColorWindow->getEndTime( *row ) )
              punctualColorWindow->calcNext( *row, dummyMaxY, dummyMinY, dummyZeros );
            tmpPairSemantic.second = punctualColorWindow->getValue( *row );
          }
          values.push_back( tmpPairSemantic );
        }

        if( currentValue != 0 && ( currentValue < getMinimumY()
                                   || currentValue > getMaximumY() ) )
          drawCaution = true;
      }

      RecordList *rl = getRecordList( *row );
      if( rl != nullptr )
        computeEventsCommsParallel( rl,
                                    currentTime - timeStep, currentTime, timeStep / magnify,
                                    timePos, objectAxisPos,
                                    selected, objectPosList,
                                    eventsToDraw, commsToDraw );
    }
    valuesToDraw.push_back( values );
    timePos += (int) magnify;

    if( progress != nullptr )
    {
      if( progress->getStop() )
        break;
      if( numRows == 1 )
      {
        static unsigned short tmpCount = 0;
        ++tmpCount;
        if( tmpCount == 10000 )
        {
          progress->setCurrentProgress( currentTime - getWindowBeginTime() );
          tmpCount = 0;
        }
      }
    }
  }

  for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
  {
    TSemanticValue dumbMinMax;
    int dumbZeros;
    calcNext( *row, dumbMinMax, dumbMinMax, dumbZeros );
    RecordList *rl = getRecordList( *row );
    if( rl != nullptr )
      computeEventsCommsParallel( rl,
                                  currentTime - timeStep, currentTime, timeStep / magnify,
                                  timePos, objectAxisPos,
                                  selected, objectPosList,
                                  eventsToDraw, commsToDraw );
  }

  // Erase events and comms remaining in RecordLists
  for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
  {
    RecordList *rl = getRecordList( *row );
    rl->erase( rl->begin(), rl->end() );
  }
}
