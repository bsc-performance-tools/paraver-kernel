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

#include <iostream>
#include <sstream>

#include <algorithm> // CFG4D

#include "kernelconnection.h"
#include "filter.h"
#include "trace.h"
#include "window.h"
#include "recordlist.h"
#include "loadedwindows.h"
#include "paraverconfig.h"
#include "syncwindows.h"
#include "selectionrowsutils.h"

using namespace std;

Window *Window::create( KernelConnection *whichKernel, Trace *whichTrace )
{
  return new WindowProxy( whichKernel, whichTrace );
}

Window *Window::create( KernelConnection *whichKernel )
{
  return new WindowProxy( whichKernel );
}

Window *Window::create( KernelConnection *whichKernel, Window *parent1, Window *parent2 )
{
  return new WindowProxy( whichKernel, parent1, parent2 );
}

Window::Window( KernelConnection *whichKernel ) : myKernel( whichKernel )
{}

WindowProxy::WindowProxy():
  myCodeColor( this )
{
  parent1 = NULL;
  parent2 = NULL;
  myTrace = NULL;
  myFilter = NULL;
  init();
}

WindowProxy::WindowProxy( KernelConnection *whichKernel, Trace *whichTrace ):
  Window( whichKernel ), myTrace( whichTrace ), myCodeColor( this )
{
  parent1 = NULL;
  parent2 = NULL;
  myWindow = myKernel->newSingleWindow( whichTrace );
  myFilter = myKernel->newFilter( myWindow->getFilter() );
  init();
}

WindowProxy::WindowProxy( KernelConnection *whichKernel, Window *whichParent1,
                          Window *whichParent2 ):
  Window( whichKernel ), myTrace( whichParent1->getTrace() ), myCodeColor( this )
{
  parent1 = whichParent1;
  parent1->setChild( this );
  parent2 = whichParent2;
  parent2->setChild( this );
  myWindow = myKernel->newDerivedWindow( parent1, parent2 );
  myFilter = NULL;
  init();
}

WindowProxy::WindowProxy( KernelConnection *whichKernel ):
  Window( whichKernel ), myTrace( NULL ), myCodeColor( this )
{
  parent1 = NULL;
  parent2 = NULL;
  myWindow = myKernel->newDerivedWindow();
  myFilter = NULL;
  init();
}

void WindowProxy::init()
{
  destroy = false;
  number_of_clones = 0;
  winBeginTime = 0.0;
  computeYMaxOnInit = false;
  yScaleComputed = false;
  maximumY = Window::getMaximumY();
  minimumY = Window::getMinimumY();
  codeColor = ParaverConfig::getInstance()->getTimelineColor() == SemanticColor::COLOR;
  myGradientColor.setGradientFunction( ParaverConfig::getInstance()->getTimelineGradientFunction() );
  drawModeObject = ParaverConfig::getInstance()->getTimelineDrawmodeObjects();
  drawModeTime = ParaverConfig::getInstance()->getTimelineDrawmodeTime();
  if( ParaverConfig::getInstance()->getTimelinePixelSize() >= 0 &&
      ParaverConfig::getInstance()->getTimelinePixelSize() <= 3 )
    pixelSize = (PRV_UINT16)pow( float(2), (int)ParaverConfig::getInstance()->getTimelinePixelSize() );
  else
    pixelSize = ParaverConfig::getInstance()->getTimelinePixelSize();
  showWindow = true;
  raise = false;
  changed = false;
  redraw = false;
  commLines = ParaverConfig::getInstance()->getTimelineViewCommunicationsLines();
  flags = ParaverConfig::getInstance()->getTimelineViewEventsLines();
  functionLineColor = ParaverConfig::getInstance()->getTimelineViewFunctionAsColor();
  child = NULL;
  usedByHistogram = false;

  if( myTrace != NULL )
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
}

WindowProxy::~WindowProxy()
{
  if ( !myWindow->isDerivedWindow() && myFilter != NULL )
    delete myFilter;
  LoadedWindows::getInstance()->eraseWindow( this );
  if( sync )
    SyncWindows::getInstance()->removeWindow( this, syncGroup );
  delete myWindow;
}

Filter *WindowProxy::getFilter() const
{
  if ( myWindow->isDerivedWindow() )
    return parent1->getFilter();
  return myFilter;
}

void WindowProxy::setFactor( PRV_UINT16 whichFactor, TSemanticValue newValue )
{
  if ( myWindow->isDerivedWindow() )
  {
    yScaleComputed = false;

    myWindow->setFactor( whichFactor, newValue );
  }
}

TSemanticValue WindowProxy::getFactor( PRV_UINT16 whichFactor ) const
{
  return myWindow->getFactor( whichFactor );
}

void WindowProxy::setParent( PRV_UINT16 whichParent, Window *whichWindow )
{
  if ( myWindow->isDerivedWindow() )
  {
    yScaleComputed = false;

    if ( whichParent == 0 )
    {
      if ( parent1 != NULL )
        parent1->setChild( NULL );
      parent1 = whichWindow;
    }
    else if ( whichParent == 1 )
    {
      if ( parent2 != NULL )
        parent2->setChild( NULL );
      parent2 = whichWindow;
    }
    myWindow->setParent( whichParent, whichWindow->getConcrete() );

    whichWindow->setChild( this );

    if ( myTrace == NULL )
    {
      myTrace = whichWindow->getTrace();
      init();
    }
  }
}

void WindowProxy::setChild( Window *whichWindow )
{
  child = whichWindow;
}

Window *WindowProxy::getChild()
{
  return child;
}


Window *WindowProxy::getParent( PRV_UINT16 whichParent )
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
      return NULL;
      break;
  }
}


Window *WindowProxy::clone( )
{
  WindowProxy *clonedWindow = new WindowProxy();
  clonedWindow->myKernel = myKernel;
  clonedWindow->myTrace  = myTrace;
  clonedWindow->myWindow = myWindow->clone( );

  if ( clonedWindow->isDerivedWindow() )
  {
    clonedWindow->parent1 = parent1->clone();
    clonedWindow->myWindow->setParent( 0, clonedWindow->parent1->getConcrete() );
    clonedWindow->parent2 = parent2->clone();
    clonedWindow->myWindow->setParent( 1, clonedWindow->parent2->getConcrete() );
    clonedWindow->parent1->setChild( clonedWindow );
    clonedWindow->parent2->setChild( clonedWindow );
  }
  else
    clonedWindow->myFilter = myKernel->newFilter( clonedWindow->myWindow->getFilter() );

  clonedWindow->usedByHistogram = false;
  clonedWindow->winBeginTime = winBeginTime;
  clonedWindow->winEndTime = winEndTime;
  clonedWindow->computeYMaxOnInit = computeYMaxOnInit;
  clonedWindow->yScaleComputed = yScaleComputed;
  clonedWindow->computeYMaxOnInit = computeYMaxOnInit;
  clonedWindow->computedMaxY = computedMaxY;
  clonedWindow->computedMinY = computedMinY;
  clonedWindow->maximumY = maximumY;
  clonedWindow->minimumY = minimumY;

  std::ostringstream tmp;
  tmp << ++number_of_clones;
  clonedWindow->name = name + ".c" + tmp.str();

  clonedWindow->myCodeColor = myCodeColor;
  clonedWindow->myGradientColor = myGradientColor;
  clonedWindow->codeColor = codeColor;
  clonedWindow->drawModeObject = drawModeObject;
  clonedWindow->drawModeTime = drawModeTime;
  clonedWindow->showWindow = showWindow;
  clonedWindow->changed = changed;
  clonedWindow->redraw = redraw;
  clonedWindow->commLines = commLines;
  clonedWindow->flags = flags;
  clonedWindow->functionLineColor = functionLineColor;
  clonedWindow->child = NULL;
  clonedWindow->posX = posX;
  clonedWindow->posY = posY;
  clonedWindow->width = width;
  clonedWindow->height = height;

  /*
    for ( vector<RecordList *>::iterator it = myLists.begin(); it != myLists.end(); it++ )
      clonedWindow->myLists.push_back( (*it)->create( *it ) );
  */
  clonedWindow->zoomHistory = zoomHistory;

  clonedWindow->selectedRow.copy( selectedRow );

  clonedWindow->sync = sync;
  clonedWindow->syncGroup = syncGroup;
  if( clonedWindow->sync )
    SyncWindows::getInstance()->addWindow( clonedWindow, syncGroup );

  // CFG4D
  clonedWindow->isCFG4DEnabled   = isCFG4DEnabled;
  clonedWindow->CFG4DMode   = CFG4DMode;
  clonedWindow->propertiesAliasCFG4D = propertiesAliasCFG4D;
  clonedWindow->paramAliasCFG4D = paramAliasCFG4D;

  return clonedWindow;
}


void WindowProxy::setDestroy( bool newValue )
{
  destroy = newValue;
}


bool WindowProxy::getDestroy() const
{
  return destroy;
}


void WindowProxy::setUsedByHistogram( bool newValue )
{
  usedByHistogram = newValue;
}


bool WindowProxy::getUsedByHistogram()
{
  return usedByHistogram;
}


void WindowProxy::setWindowBeginTime( TRecordTime whichTime, bool isBroadcast )
{
  yScaleComputed = false;

  winBeginTime = whichTime;
  if( sync && !isBroadcast )
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, winBeginTime, winEndTime );
}

void WindowProxy::setWindowEndTime( TRecordTime whichTime, bool isBroadcast )
{
  yScaleComputed = false;

  winEndTime = whichTime;
  if( sync && !isBroadcast )
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, winBeginTime, winEndTime );
}

TRecordTime WindowProxy::getWindowBeginTime() const
{
  return winBeginTime;
}

TRecordTime WindowProxy::getWindowEndTime() const
{
  return winEndTime;
}

bool WindowProxy::getYScaleComputed() const
{
  return yScaleComputed;
}

void WindowProxy::computeYScaleMin()
{
  if ( !yScaleComputed )
  {
    vector< TObjectOrder > selected;
    getSelectedRows( getLevel(), selected,
                     getZoomSecondDimension().first, getZoomSecondDimension().second, true );

    init( winBeginTime, NONE );

    for ( vector< TObjectOrder >::iterator obj = selected.begin(); obj != selected.end(); ++obj )
    {
      initRow( *obj, winBeginTime, NONE );
      while ( getBeginTime( *obj ) < getTrace()->getEndTime() )
        calcNext( *obj );
    }
  }

  minimumY = computedMinY;
}

void WindowProxy::computeYScaleMax()
{
  if ( !yScaleComputed )
  {
    vector< TObjectOrder > selected;
    getSelectedRows( getLevel(), selected,
                     getZoomSecondDimension().first, getZoomSecondDimension().second, true );

    init( winBeginTime, NONE );

    for ( vector< TObjectOrder >::iterator obj = selected.begin(); obj != selected.end(); ++obj )
    {
      initRow( *obj, winBeginTime, NONE );
      while ( getBeginTime( *obj ) < getTrace()->getEndTime() )
        calcNext( *obj );
    }
  }

  maximumY = computedMaxY;
}

void WindowProxy::computeYScale()
{
  if ( !yScaleComputed )
  {
    vector< TObjectOrder > selected;
    getSelectedRows( getLevel(), selected,
                     getZoomSecondDimension().first, getZoomSecondDimension().second, true );

    init( winBeginTime, NONE );

    for ( vector< TObjectOrder >::iterator obj = selected.begin(); obj != selected.end(); ++obj )
    {
      initRow( *obj, winBeginTime, NONE );
      while ( getBeginTime( *obj ) < getTrace()->getEndTime() )
        calcNext( *obj );
    }
  }

  maximumY = computedMaxY;
  minimumY = computedMinY;
}

void WindowProxy::setComputeYMaxOnInit( bool newValue )
{
  computeYMaxOnInit = newValue;
}

bool WindowProxy::getComputeYMaxOnInit() const
{
  return computeYMaxOnInit;
}

void WindowProxy::setMaximumY( TSemanticValue whichMax )
{
  maximumY = whichMax;
}

void WindowProxy::setMinimumY( TSemanticValue whichMin )
{
  minimumY = whichMin;
}

TSemanticValue WindowProxy::getMaximumY()
{
  if ( computeYMaxOnInit )
  {
    computeYScale();
    computeYMaxOnInit = false;
  }
  return maximumY;
}

TSemanticValue WindowProxy::getMinimumY()
{
  if ( computeYMaxOnInit )
  {
    computeYScale();
    computeYMaxOnInit = false;
  }
  return minimumY;
}

Trace *WindowProxy::getTrace() const
{
  return myTrace;
}

TWindowLevel WindowProxy::getLevel() const
{
  return myWindow->getLevel();
}

void WindowProxy::setLevel( TWindowLevel whichLevel )
{
  if ( whichLevel == myWindow->getLevel() )
    return;

  yScaleComputed = false;

  myWindow->setLevel( whichLevel );
  zoomHistory.clear();
  zoomHistory.addZoom( winBeginTime, winEndTime, 0, getWindowLevelObjects() - 1 );
}

TWindowLevel WindowProxy::getMinAcceptableLevel() const
{
  return myWindow->getMinAcceptableLevel();
}

void WindowProxy::setTimeUnit( TTimeUnit whichUnit )
{
  yScaleComputed = false;

  myWindow->setTimeUnit( whichUnit );
}

TTimeUnit WindowProxy::getTimeUnit() const
{
  return myWindow->getTimeUnit();
}

TWindowLevel WindowProxy::getComposeLevel( TWindowLevel whichLevel ) const
{
  return myWindow->getComposeLevel( whichLevel );
}

bool WindowProxy::setLevelFunction( TWindowLevel whichLevel,
                                    const string& whichFunction )
{
  bool result = myWindow->setLevelFunction( whichLevel, whichFunction );
  if ( result )
    yScaleComputed = false;
  return result;
}

string WindowProxy::getLevelFunction( TWindowLevel whichLevel )
{
  return myWindow->getLevelFunction( whichLevel );
}

string WindowProxy::getFirstUsefulFunction( )
{
  return myWindow->getFirstUsefulFunction();
}

TWindowLevel WindowProxy::getFirstFreeCompose() const
{
  return myWindow->getFirstFreeCompose();
}

void WindowProxy::setFunctionParam( TWindowLevel whichLevel,
                                    TParamIndex whichParam,
                                    const TParamValue& newValue )
{
  yScaleComputed = false;

  myWindow->setFunctionParam( whichLevel, whichParam, newValue );
}

TParamIndex WindowProxy::getFunctionNumParam( TWindowLevel whichLevel ) const
{
  return myWindow->getFunctionNumParam( whichLevel );
}

TParamValue WindowProxy::getFunctionParam( TWindowLevel whichLevel,
    TParamIndex whichParam ) const
{
  return myWindow->getFunctionParam( whichLevel, whichParam );
}

string WindowProxy::getFunctionParamName( TWindowLevel whichLevel,
    TParamIndex whichParam ) const
{
  return myWindow->getFunctionParamName( whichLevel, whichParam );
}

RecordList *WindowProxy::getRecordList( TObjectOrder whichObject )
{
  if ( myLists.begin() == myLists.end() )
    return NULL;
  return myLists[ whichObject ];
}

void WindowProxy::init( TRecordTime initialTime, TCreateList create, bool updateLimits )
{
  if ( myLists.begin() != myLists.end() )
  {
    for ( vector<RecordList *>::iterator it = myLists.begin(); it != myLists.end(); ++it )
      delete *it;
    if ( myLists.begin() != myLists.end() ) // solo sintoma de que algo no va bien
      myLists.clear();
  }

  for ( int i = 0; i < myWindow->getWindowLevelObjects(); i++ )
    myLists.push_back( NULL );

  if ( getComputeYMaxOnInit() )
  {
    setComputeYMaxOnInit( false );
    computeYScale();
  }
  myWindow->init( initialTime, create );
  if ( updateLimits )
  {
    yScaleComputed = true;
    computedMaxY = computedMinY = 0.0;
  }
}

void WindowProxy::initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits )
{
  myWindow->initRow( whichRow, initialTime, create );
  if ( myLists[ whichRow ] == NULL )
    myLists[ whichRow ] = RecordList::create( myWindow->getRecordList( whichRow ) );

  if ( updateLimits )
  {
    TSemanticValue objValue = myWindow->getValue( whichRow );
    if ( computedMaxY < objValue )
      computedMaxY = objValue;
    if ( computedMinY == 0 || ( computedMinY > objValue && objValue != 0 ) )
      computedMinY = objValue;
  }
}

void WindowProxy::initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create,
                           TSemanticValue &rowComputedMaxY, TSemanticValue &rowComputedMinY,
                           bool updateLimits )
{
  myWindow->initRow( whichRow, initialTime, create );
  if ( myLists[ whichRow ] == NULL )
    myLists[ whichRow ] = RecordList::create( myWindow->getRecordList( whichRow ) );

  if ( updateLimits )
  {
    TSemanticValue objValue = myWindow->getValue( whichRow );
    if ( rowComputedMaxY < objValue )
      rowComputedMaxY = objValue;
    if ( rowComputedMinY == 0 || ( rowComputedMinY > objValue && objValue != 0 ) )
      rowComputedMinY = objValue;
  }
}

RecordList *WindowProxy::calcNext( TObjectOrder whichObject, bool updateLimits )
{
  if ( myLists[ whichObject ] == NULL )
    myLists[ whichObject ] = RecordList::create( myWindow->calcNext( whichObject ) );
  else
    myWindow->calcNext( whichObject );

  if ( updateLimits )
  {
    TSemanticValue objValue = myWindow->getValue( whichObject );
    if ( computedMaxY < objValue )
      computedMaxY = objValue;
    if ( computedMinY == 0 || ( computedMinY > objValue && objValue != 0 ) )
      computedMinY = objValue;
  }

  return myLists[ whichObject ];
}

RecordList *WindowProxy::calcNext( TObjectOrder whichObject,
                                   TSemanticValue &rowComputedMaxY, TSemanticValue &rowComputedMinY,
                                   bool updateLimits )
{
  if ( myLists[ whichObject ] == NULL )
    myLists[ whichObject ] = RecordList::create( myWindow->calcNext( whichObject ) );
  else
    myWindow->calcNext( whichObject );

  if ( updateLimits )
  {
    TSemanticValue objValue = myWindow->getValue( whichObject );
    if ( rowComputedMaxY < objValue )
      rowComputedMaxY = objValue;
    if ( rowComputedMinY == 0 || ( rowComputedMinY > objValue && objValue != 0 ) )
      rowComputedMinY = objValue;
  }

  return myLists[ whichObject ];
}

RecordList *WindowProxy::calcPrev( TObjectOrder whichObject, bool updateLimits )
{
  if ( myLists[ whichObject ] == NULL )
    myLists[ whichObject ] = RecordList::create( myWindow->calcPrev( whichObject ) );
  else
    myWindow->calcPrev( whichObject );

  if ( updateLimits )
  {
    TSemanticValue objValue = getValue( whichObject );
    if ( computedMaxY < objValue )
      computedMaxY = objValue;
    if ( computedMinY == 0 || ( computedMinY > objValue && objValue != 0 ) )
      computedMinY = objValue;
  }

  return myLists[ whichObject ];
}

TRecordTime WindowProxy::getBeginTime( TObjectOrder whichObject ) const
{
  return myWindow->getBeginTime( whichObject );
}

TRecordTime WindowProxy::getEndTime( TObjectOrder whichObject ) const
{
  return myWindow->getEndTime( whichObject );
}

TSemanticValue WindowProxy::getValue( TObjectOrder whichObject ) const
{
  return myWindow->getValue( whichObject );
}

bool WindowProxy::isDerivedWindow() const
{
  return myWindow->isDerivedWindow();
}

TObjectOrder WindowProxy::cpuObjectToWindowObject( TCPUOrder whichCPU )
{
  return myWindow->cpuObjectToWindowObject( whichCPU );
}

TObjectOrder WindowProxy::threadObjectToWindowObject( TThreadOrder whichThread )
{
  return myWindow->threadObjectToWindowObject( whichThread );
}

TObjectOrder WindowProxy::getWindowLevelObjects()
{
  return myWindow->getWindowLevelObjects();
}

TRecordTime WindowProxy::customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits )
{
  return myWindow->customUnitsToTraceUnits( whichTime, whichUnits );
}

TRecordTime WindowProxy::traceUnitsToWindowUnits( TRecordTime whichTime )
{
  return myWindow->traceUnitsToWindowUnits( whichTime );
}

TRecordTime WindowProxy::windowUnitsToTraceUnits( TRecordTime whichTime )
{
  return myWindow->windowUnitsToTraceUnits( whichTime );
}

Window *WindowProxy::getConcrete() const
{
  return myWindow;
}

void WindowProxy::setName( const string& whichName )
{
  name = whichName;
}

string WindowProxy::getName() const
{
  return name;
}

PRV_UINT16 WindowProxy::getPosX() const
{
  return posX;
}

void WindowProxy::setPosX( PRV_UINT16 whichPos )
{
  posX = whichPos;
}

PRV_UINT16 WindowProxy::getPosY() const
{
  return posY;
}

void WindowProxy::setPosY( PRV_UINT16 whichPos )
{
  posY = whichPos;
}

PRV_UINT16 WindowProxy::getWidth() const
{
  return width;
}

void WindowProxy::setWidth( PRV_UINT16 whichPos )
{
  width = whichPos;
}

PRV_UINT16 WindowProxy::getHeight() const
{
  return height;
}

void WindowProxy::setHeight( PRV_UINT16 whichPos )
{
  height = whichPos;
}

void WindowProxy::setDrawModeObject( DrawModeMethod method )
{
  drawModeObject = method;
}

DrawModeMethod WindowProxy::getDrawModeObject() const
{
  return drawModeObject;
}

void WindowProxy::setDrawModeTime( DrawModeMethod method )
{
  drawModeTime = method;
}

DrawModeMethod WindowProxy::getDrawModeTime() const
{
  return drawModeTime;
}

CodeColor& WindowProxy::getCodeColor()
{
  return myCodeColor;
}

GradientColor& WindowProxy::getGradientColor()
{
  return myGradientColor;
}

bool WindowProxy::getShowWindow() const
{
  return showWindow;
}

void WindowProxy::setShowWindow( bool newValue )
{
  showWindow = newValue;
}

void WindowProxy::setShowChildrenWindow( bool newValue )
{
  if ( getParent( 0 ) != NULL )
  {
    for ( PRV_UINT16 i = 0; i < 2; ++i )
    {
      getParent( i )->setShowWindow( newValue );
      getParent( i )->setShowChildrenWindow( newValue );
    }
  }
}

bool WindowProxy::getRaiseWindow() const
{
  return raise;
}

void WindowProxy::setRaiseWindow( bool newValue )
{
  raise = newValue;
}

void WindowProxy::setCodeColorMode()
{
  codeColor = true;
}

void WindowProxy::setGradientColorMode()
{
  codeColor = false;
}

bool WindowProxy::IsCodeColorSet() const
{
  return codeColor;
}

bool WindowProxy::IsGradientColorSet() const
{
  return ( !codeColor && ( myGradientColor.getAllowOutOfScale() ) );
//  return !codeColor;
}


bool WindowProxy::IsNotNullGradientColorSet() const
{
  return ( !codeColor && !( myGradientColor.getAllowOutOfScale() ) );
}


PRV_UINT16 WindowProxy::getPixelSize() const
{
  return pixelSize;
}

void WindowProxy::setPixelSize( PRV_UINT16 whichSize )
{
  pixelSize = whichSize;
}

void WindowProxy::allowOutOfScale( bool activate )
{
  myGradientColor.allowOutOfScale( activate );
}

void WindowProxy::allowOutliers( bool activate )
{
  myGradientColor.allowOutliers( activate );
}

rgb WindowProxy::calcColor( TSemanticValue whichValue, Window& whichWindow )
{
  if ( codeColor )
    return myCodeColor.calcColor( whichValue, minimumY, maximumY );

  return myGradientColor.calcColor( whichValue, minimumY, maximumY );
}

bool WindowProxy::calcValueFromColor( rgb whichColor,
                                      TSemanticValue& firstValue,
                                      TSemanticValue& secondValue ) const
{
  if ( codeColor )
    return myCodeColor.calcValue( whichColor, firstValue );

  return myGradientColor.calcValue( whichColor, minimumY, maximumY, firstValue, secondValue );
}

bool WindowProxy::getChanged() const
{
  return changed;
}

void WindowProxy::setChanged( bool newValue )
{
  changed = newValue;
}

bool WindowProxy::getRedraw() const
{
  return redraw;
}

void WindowProxy::setRedraw( bool newValue )
{
  redraw = newValue;
}

bool WindowProxy::getDrawCommLines() const
{
  return commLines;
}

void WindowProxy::setDrawCommLines( bool newValue )
{
  commLines = newValue;
}

bool WindowProxy::getDrawFlags() const
{
  return flags;
}

void WindowProxy::setDrawFlags( bool newValue )
{
  flags = newValue;
}

bool WindowProxy::getDrawFunctionLineColor() const
{
  return functionLineColor;
}

void WindowProxy::setDrawFunctionLineColor( bool newValue )
{
  functionLineColor = newValue;
}

SemanticInfoType WindowProxy::getSemanticInfoType() const
{
  return myWindow->getSemanticInfoType();
}

void WindowProxy::getAllSemanticFunctions( TSemanticGroup whichGroup,
    vector<string>& onVector ) const
{
  myKernel->getAllSemanticFunctions( whichGroup, onVector );
}

bool WindowProxy::emptyPrevZoom() const
{
  return zoomHistory.emptyPrevZoom();
}

bool WindowProxy::emptyNextZoom() const
{
  return zoomHistory.emptyNextZoom();
}

void WindowProxy::nextZoom()
{
  zoomHistory.nextZoom();
  if( sync )
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, zoomHistory.getFirstDimension().first,
                                                                zoomHistory.getFirstDimension().second );
}

void WindowProxy::prevZoom()
{
  zoomHistory.prevZoom();
  if( sync )
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, zoomHistory.getFirstDimension().first,
                                                                zoomHistory.getFirstDimension().second );
}

void WindowProxy::addZoom( TTime beginTime, TTime endTime,
                           TObjectOrder beginObject, TObjectOrder endObject,
                           bool isBroadCast )
{
  if( sync && !isBroadCast )
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, beginTime, endTime );
  zoomHistory.addZoom( beginTime, endTime, beginObject, endObject );
}

void WindowProxy::addZoom( TTime beginTime, TTime endTime, bool isBroadCast )
{
  if( sync && !isBroadCast )
    SyncWindows::getInstance()->broadcastTime( syncGroup, this, beginTime, endTime );
  zoomHistory.addZoom( beginTime, endTime );
}

void WindowProxy::addZoom( TObjectOrder beginObject, TObjectOrder endObject )
{
  zoomHistory.addZoom( beginObject, endObject );
}

void WindowProxy::setZoomFirstDimension( pair<TTime, TTime> &dim )
{
  zoomHistory.setFirstDimension( dim );
}

void WindowProxy::setZoomSecondDimension( pair<TObjectOrder, TObjectOrder>  &dim )
{
  zoomHistory.setSecondDimension( dim );
}

pair<TTime, TTime> WindowProxy::getZoomFirstDimension() const
{
  return zoomHistory.getFirstDimension();
}

pair<TObjectOrder, TObjectOrder> WindowProxy::getZoomSecondDimension() const
{
  return zoomHistory.getSecondDimension();
}

void WindowProxy::addToSyncGroup( unsigned int whichGroup )
{
  if( sync )
    return;
  syncGroup = whichGroup;
  sync = SyncWindows::getInstance()->addWindow( this, whichGroup );
}

void WindowProxy::removeFromSync()
{
  if( !sync )
    return;
  SyncWindows::getInstance()->removeWindow( this, syncGroup );
  sync = false;
}

bool WindowProxy::isSync() const
{
  return sync;
}

unsigned int WindowProxy::getSyncGroup() const
{
  return syncGroup;
}

SelectionManagement< TObjectOrder, TWindowLevel > *WindowProxy::getSelectedRows()
{
  return &selectedRow;
}

void WindowProxy::setSelectedRows( TWindowLevel onLevel, vector< bool > &selected )
{
  selectedRow.setSelected( selected, onLevel );
}

void WindowProxy::setSelectedRows( TWindowLevel onLevel, vector< TObjectOrder > &selected )
{
  selectedRow.setSelected( selected, myTrace->getLevelObjects( onLevel ), onLevel );
}

void WindowProxy::getSelectedRows( TWindowLevel onLevel, vector< bool > &selected, bool lookUpLevels )
{
  selectedRow.getSelected( selected, onLevel );

  if ( lookUpLevels )
  {
    TObjectOrder iAppl, jTask, globalTask, iNode, aux;
    // Only deselect those with higher levels deselected
    switch ( onLevel )
    {
      case TASK:
        for ( TTaskOrder iTask = 0; iTask < getTrace()->totalTasks(); ++iTask )
        {
          getTrace()->getTaskLocation( iTask, iAppl, aux );
          selected[ iTask ] = selectedRow.isSelectedPosition( iAppl, APPLICATION ) &&
                              selected[ iTask ];
        }

        break;

      case THREAD:
        for ( TThreadOrder iThread = 0; iThread < getTrace()->totalThreads(); ++iThread )
        {
          getTrace()->getThreadLocation( iThread, iAppl, jTask, aux );
          globalTask = getTrace()->getGlobalTask( iAppl, jTask );
          selected[ iThread ] = selectedRow.isSelectedPosition( iAppl, APPLICATION ) &&
                                selectedRow.isSelectedPosition( globalTask, TASK ) &&
                                selected[ iThread ];
        }

        break;

      case CPU:
        for ( TCPUOrder iCPU = 0; iCPU < getTrace()->totalCPUs(); ++iCPU )
        {
          getTrace()->getCPULocation( iCPU, iNode, aux );
          selected[ iCPU ] = selected[ iCPU ] &&
                             selectedRow.isSelectedPosition( iNode, NODE );
        }
        break;

      default:
        break;
    }
  }
}

void WindowProxy::getSelectedRows( TWindowLevel onLevel, vector< bool > &selected,
                                   TObjectOrder first, TObjectOrder last, bool lookUpLevels )
{
  selectedRow.getSelected( selected, first, last, onLevel );

  if ( lookUpLevels )
  {
    TObjectOrder iAppl, jTask, globalTask, iNode, aux;
    switch ( onLevel )
    {
      case TASK:
        for ( TObjectOrder iTask = first; iTask <= last; ++iTask )
        {
          getTrace()->getTaskLocation( iTask, iAppl, aux );
          selected[ iTask ] = selectedRow.isSelectedPosition( iAppl, APPLICATION ) &&
                              selected[ iTask ];
        }

        break;

      case THREAD:
        for ( TObjectOrder iThread = first; iThread <= last; ++iThread )
        {
          getTrace()->getThreadLocation( iThread, iAppl, jTask, aux );
          globalTask = getTrace()->getGlobalTask( iAppl, jTask );
          selected[ iThread ] = selectedRow.isSelectedPosition( iAppl, APPLICATION ) &&
                                selectedRow.isSelectedPosition( globalTask, TASK ) &&
                                selected[ iThread ];
        }

        break;

      case CPU:
        for ( TObjectOrder iCPU = first; iCPU <= last; ++iCPU )
        {
          getTrace()->getCPULocation( iCPU, iNode, aux );
          selected[ iCPU ] = selected[ iCPU ] &&
                             selectedRow.isSelectedPosition( iNode, NODE );
        }
        break;

      default:
        break;
    }
  }
}


void WindowProxy::getSelectedRows( TWindowLevel onLevel, vector< TObjectOrder > &selected, bool lookUpLevels )
{
  selectedRow.getSelected( selected, onLevel );

  if ( lookUpLevels )
  {
    SelectionRowsUtils::getAllLevelsSelectedRows( getTrace(), selectedRow, onLevel, selected );
  }
}

void WindowProxy::getSelectedRows( TWindowLevel onLevel,
                                   vector< TObjectOrder > &selected,
                                   TObjectOrder first, TObjectOrder last, bool lookUpLevels )
{
  selectedRow.getSelected( selected, first, last, onLevel );

  if ( lookUpLevels )
  {
    SelectionRowsUtils::getAllLevelsSelectedRows( getTrace(), selectedRow, onLevel, selected );
  }
}

void WindowProxy::getGroupLabels( PRV_UINT32 whichGroup, vector<string>& onVector ) const
{
  myWindow->getGroupLabels( whichGroup, onVector );
}

bool WindowProxy::getParametersOfFunction( string whichFunction,
    PRV_UINT32 &numParameters,
    vector<string> &nameParameters,
    vector< vector< double > >&defaultParameters ) const
{
  return myWindow->getParametersOfFunction( whichFunction,
         numParameters,
         nameParameters,
         defaultParameters );
}


void WindowProxy::setCFG4DMode( bool mode )
{
  if( isDerivedWindow() )
  {
    if ( parent1 != NULL)
      parent1->setCFG4DMode( mode );

    if ( parent2 != NULL)
      parent2->setCFG4DMode( mode );
  }

  CFG4DMode = mode;
}

bool WindowProxy::getCFG4DMode( ) const
{
  return CFG4DMode;
}

bool WindowProxy::getCFG4DEnabled() const
{
  return isCFG4DEnabled;
}

void WindowProxy::setCFG4DEnabled( bool enabled )
{
  if( isDerivedWindow() )
  {
    if ( parent1 != NULL)
      parent1->setCFG4DEnabled( enabled );

    if ( parent2 != NULL)
      parent2->setCFG4DEnabled( enabled );
  }

  isCFG4DEnabled = enabled;
}

bool WindowProxy::existsCFG4DAlias( const string &property ) const
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



bool WindowProxy::existsCFG4DAlias( const TSingleTimelineProperties &propertyIndex ) const
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


bool WindowProxy::existsCFG4DAlias( const TDerivedTimelineProperties &propertyIndex ) const
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


string WindowProxy::getCFG4DAlias( const string &property ) const
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


string WindowProxy::getCFG4DAlias( const TSingleTimelineProperties &propertyIndex ) const
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


string WindowProxy::getCFG4DAlias( const TDerivedTimelineProperties &propertyIndex ) const
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


void WindowProxy::setCFG4DAlias( const string &property, const string &alias )
{
  propertiesAliasCFG4D[ property ] = alias;
}


void WindowProxy::setCFG4DAliasList( const map< string, string >& aliasList )
{
  propertiesAliasCFG4D = aliasList;
}


const map< string, string > WindowProxy::getCFG4DAliasList() const
{
  return propertiesAliasCFG4D;
}

const vector< string > WindowProxy::getCFG4DFullTagList()
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


const vector< Window::TParamAliasKey > WindowProxy::getCFG4DCurrentSelectedFullParamList()
{
  vector< TParamAliasKey > retKeys;

  TWindowLevel curLevel, beginLevel, endLevel, beginCompose, endCompose;

  string semanticLevel, semanticFunction;
  PRV_UINT32 numParameter;
  TParamAliasKey key;

  beginLevel = getLevel();

  switch ( beginLevel )
  {
    case WORKLOAD:
    case APPLICATION:
    case TASK:
    case THREAD:
      endLevel = THREAD;
      beginCompose = COMPOSEWORKLOAD;
      endCompose = COMPOSETHREAD;
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


void WindowProxy::setCFG4DParamAlias( const TParamAlias &whichParamAlias )
{
  paramAliasCFG4D = whichParamAlias;
}


void WindowProxy::setCFG4DParamAlias( string semanticLevel,
                                      string function,
                                      PRV_UINT32 numParameter,
                                      string paramAlias )
{
  TParamAliasKey key( make_pair( make_pair( semanticLevel, function ), numParameter ) );
  paramAliasCFG4D[ key ] = paramAlias;
}

const Window::TParamAlias WindowProxy::getCFG4DParamAliasList() const
{
  return paramAliasCFG4D;
}

void WindowProxy::splitCFG4DParamAliasKey( const TParamAliasKey &pk,
                                           string &semanticLevel,
                                           string &function,
                                           PRV_UINT32 &numParameter ) const
{
  semanticLevel = pk.first.first;
  function = pk.first.second;
  numParameter = pk.second;
}


const Window::TParamAliasKey WindowProxy::buildCFG4DParamAliasKey( const string &semanticLevel,
                                                                   const string &function,
                                                                   const PRV_UINT32 &numParameter ) const
{
  TParamAliasKey key( make_pair( make_pair( semanticLevel, function ), numParameter ) );
  return key;
}


Window::TParamAliasKey WindowProxy::getCFG4DParamAliasKey( const TParamAlias::iterator it ) const
{
  return it->first;
}

const string WindowProxy::getCFG4DParamAlias( const TParamAlias::iterator &it ) const
{
  return it->second;
}


const string WindowProxy::getCFG4DParamAlias( const TParamAliasKey &pk ) const
{
  TParamAlias::const_iterator it = paramAliasCFG4D.find( pk );
  return ( it != paramAliasCFG4D.end()? it->second:string("") );
}


vector< Window::TParamAliasKey > WindowProxy::getCFG4DParamKeysBySemanticLevel( string whichSemanticLevel,
                                                                                const vector< Window::TParamAliasKey > &whichParamAliasKey ) const
{
  vector< TParamAliasKey > retKeys;
  string semanticLevel, function;
  PRV_UINT32 numParameter;

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
        retKeys.push_back( (Window::TParamAliasKey) it->first );
      }
    }
  }


  return retKeys;
}

#ifdef WIN32
void WindowProxy::computeSemanticParallel( vector< TObjectOrder >& selectedSet,
                                           vector< bool >& selected,
                                           TTime timeStep,
                                           PRV_INT32 timePos,
                                           PRV_INT32 objectAxisPos,
                                           vector< PRV_INT32 >& objectPosList,
                                           TObjectOrder maxObj,
                                           bool& drawCaution,
                                           vector< vector< TSemanticValue > >& valuesToDraw,
                                           vector< hash_set< PRV_INT32 > >& eventsToDraw,
                                           vector< hash_set< commCoord > >& commsToDraw )
#else
void WindowProxy::computeSemanticParallel( vector< TObjectOrder >& selectedSet,
                                           vector< bool >& selected,
                                           TTime timeStep,
                                           PRV_INT32 timePos,
                                           PRV_INT32 objectAxisPos,
                                           vector< PRV_INT32 >& objectPosList,
                                           TObjectOrder maxObj,
                                           bool& drawCaution,
                                           vector< vector< TSemanticValue > >& valuesToDraw,
                                           vector< hash_set< PRV_INT32 > >& eventsToDraw,
                                           vector< hash_set< commCoord, hashCommCoord > >& commsToDraw )
#endif
{
  vector< int > tmpDrawCaution;
  vector< TSemanticValue > tmpComputedMaxY;
  vector< TSemanticValue > tmpComputedMinY;

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

  // Drawmode: Group objects with same wxCoord in objectPosList
  for( vector< TObjectOrder >::iterator obj = selectedSet.begin(); obj != selectedSet.end(); ++obj )
  {
    TObjectOrder firstObj = *obj;
    TObjectOrder lastObj = firstObj;
    while( ( lastObj + 1 ) <= maxObj && objectPosList[ lastObj + 1 ] == objectPosList[ firstObj ] )
    {
      ++obj;
      lastObj = *obj;
    }
    valuesToDraw.push_back( vector< TSemanticValue >() );

    eventsToDraw.push_back( hash_set< PRV_INT32 >() );
#ifdef WIN32
    commsToDraw.push_back( hash_set< commCoord >() );
#else
    commsToDraw.push_back( hash_set< commCoord, hashCommCoord >() );
#endif

    tmpDrawCaution.push_back( drawCaution );
    tmpComputedMaxY.push_back( 0.0 );
    tmpComputedMinY.push_back( 0.0 );

    computeSemanticRowParallel(
            firstObj, lastObj, selectedSet, selected, timeStep, timePos,
            objectAxisPos, objectPosList,
            tmpDrawCaution[ tmpDrawCaution.size() - 1 ],
            tmpComputedMaxY[ tmpComputedMaxY.size() - 1 ],
            tmpComputedMinY[ tmpComputedMinY.size() - 1 ],
            valuesToDraw[ valuesToDraw.size() - 1 ],
            eventsToDraw[ eventsToDraw.size() - 1 ],
            commsToDraw[ commsToDraw.size() - 1 ] );
  }
//#pragma css barrier
#pragma omp taskwait

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
    computedMaxY = computedMaxY > tmpComputedMaxY[ pos ] ? computedMaxY : tmpComputedMaxY[ pos ];
    if ( computedMinY == 0.0 )
      computedMinY = tmpComputedMinY[ pos ];
    else
      computedMinY = computedMinY < tmpComputedMinY[ pos ] ? computedMinY : tmpComputedMinY[ pos ];
  }
}

#ifdef WIN32
void WindowProxy::computeSemanticRowParallel( TObjectOrder firstRow,
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
                                              vector< TSemanticValue >& valuesToDraw,
                                              hash_set< PRV_INT32 >& eventsToDraw,
                                              hash_set< commCoord >& commsToDraw )
#else
void WindowProxy::computeSemanticRowParallel( TObjectOrder firstRow,
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
                                              vector< TSemanticValue >& valuesToDraw,
                                              hash_set< PRV_INT32 >& eventsToDraw,
                                              hash_set< commCoord, hashCommCoord >& commsToDraw )
#endif
{
  float magnify = float( getPixelSize() );

  vector<TSemanticValue> timeValues;
  vector<TSemanticValue> rowValues;

  vector<TObjectOrder>::iterator first = find( selectedSet.begin(), selectedSet.end(), firstRow );
  vector<TObjectOrder>::iterator last  = find( selectedSet.begin(), selectedSet.end(), lastRow );

  for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
    initRow( *row, getWindowBeginTime(), CREATECOMMS + CREATEEVENTS, rowComputedMaxY, rowComputedMinY );

  for( TTime currentTime = getWindowBeginTime() + timeStep;
       currentTime <= getWindowEndTime() && currentTime <= getTrace()->getEndTime();
       currentTime += timeStep )
  {
    rowValues.clear();
    for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
    {
      timeValues.clear();

      while( getEndTime( *row ) <= currentTime - timeStep )
        calcNext( *row, rowComputedMaxY, rowComputedMinY  );

      timeValues.push_back( getValue( *row ) );
      while( getEndTime( *row ) < currentTime )
      {
        calcNext( *row, rowComputedMaxY, rowComputedMinY  );
        TSemanticValue currentValue = getValue( *row );
        timeValues.push_back( currentValue );
        if( currentValue != 0 && ( currentValue < getMinimumY()
                                   || currentValue > getMaximumY() ) )
          drawCaution = true;
      }
      rowValues.push_back( DrawMode::selectValue( timeValues, getDrawModeTime() ) );

      RecordList *rl = getRecordList( *row );
      if( rl != NULL )
        computeEventsCommsParallel( rl,
                                    currentTime - timeStep, currentTime, timeStep / magnify,
                                    timePos, objectAxisPos,
                                    selected, objectPosList,
                                    eventsToDraw, commsToDraw );
    }
    valuesToDraw.push_back( DrawMode::selectValue( rowValues, getDrawModeObject() ) );
    timePos += (int) magnify ;
  }

  // Erase events and comms remaining in RecordLists
  for( vector<TObjectOrder>::iterator row = first; row <= last; ++row )
  {
    RecordList *rl = getRecordList( *row );
    rl->erase( rl->begin(), rl->end() );
  }
}

#ifdef WIN32
void WindowProxy::computeEventsCommsParallel( RecordList *records,
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
void WindowProxy::computeEventsCommsParallel( RecordList *records,
                                              TTime from,
                                              TTime to,
                                              TTime step,
                                              PRV_INT32 timePos,
                                              PRV_INT32 objectAxisPos,
                                              vector< bool >& selected,
                                              vector< PRV_INT32 >& objectPosList,
                                              hash_set< PRV_INT32 >& eventsToDraw,
                                              hash_set< commCoord, hashCommCoord >& commsToDraw )
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
      if ( getLevel() >= WORKLOAD && getLevel() <= THREAD )
        partnerObject = threadObjectToWindowObject( it->getCommPartnerObject() );
      else
        partnerObject = cpuObjectToWindowObject( it->getCommPartnerObject() - 1 );

      if( ( recType & COMM ) &&
          partnerObject >= beginRow && partnerObject <= endRow && selected[ partnerObject ] &&
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
        tmpComm.toRow = objectPosList[ partnerObject ];
        commsToDraw.insert( tmpComm );
      }
    }
    ++it;
  }

  if( existEvents )
    eventsToDraw.insert( timePos );

  records->erase( records->begin(), it );
}
