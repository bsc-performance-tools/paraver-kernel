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

#include "kernelconnection.h"
#include "filter.h"
#include "trace.h"
#include "window.h"
#include "recordlist.h"
#include "loadedwindows.h"
#include "paraverconfig.h"

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

WindowProxy::WindowProxy()
{
  parent1 = NULL;
  parent2 = NULL;
  myTrace = NULL;
  init();
}

WindowProxy::WindowProxy( KernelConnection *whichKernel, Trace *whichTrace ):
    Window( whichKernel ), myTrace( whichTrace )
{
  parent1 = NULL;
  parent2 = NULL;
  myWindow = myKernel->newSingleWindow( whichTrace );
  myFilter = myKernel->newFilter( myWindow->getFilter() );
  init();
}

WindowProxy::WindowProxy( KernelConnection *whichKernel, Window *whichParent1,
                          Window *whichParent2 ):
    Window( whichKernel ), myTrace( whichParent1->getTrace() )
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
    Window( whichKernel ), myTrace( NULL )
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
  }
}

WindowProxy::~WindowProxy()
{
  if ( myFilter != NULL )
    delete myFilter;
  LoadedWindows::getInstance()->eraseWindow( this );
  delete myWindow;
}

Filter *WindowProxy::getFilter() const
{
  if ( myWindow->isDerivedWindow() )
    return parent1->getFilter();
  return myFilter;
}

void WindowProxy::setFactor( UINT16 whichFactor, TSemanticValue newValue )
{
  if ( myWindow->isDerivedWindow() )
  {
    yScaleComputed = false;

    myWindow->setFactor( whichFactor, newValue );
  }
}

TSemanticValue WindowProxy::getFactor( UINT16 whichFactor ) const
{
  return myWindow->getFactor( whichFactor );
}

void WindowProxy::setParent( UINT16 whichParent, Window *whichWindow )
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


Window *WindowProxy::getParent( UINT16 whichParent )
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
  clonedWindow->child = child;
  clonedWindow->posX = posX;
  clonedWindow->posY = posY;
  clonedWindow->width = width;
  clonedWindow->height = height;

  /*
    for ( vector<RecordList *>::iterator it = myLists.begin(); it != myLists.end(); it++ )
      clonedWindow->myLists.push_back( (*it)->create( *it ) );
  */
  clonedWindow->zoomHistory = zoomHistory;

  clonedWindow->selectedRow = selectedRow;

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


void WindowProxy::setWindowBeginTime( TRecordTime whichTime )
{
  yScaleComputed = false;

  winBeginTime = whichTime;
}

void WindowProxy::setWindowEndTime( TRecordTime whichTime )
{
  yScaleComputed = false;

  winEndTime = whichTime;
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

UINT16 WindowProxy::getPosX() const
{
  return posX;
}

void WindowProxy::setPosX( UINT16 whichPos )
{
  posX = whichPos;
}

UINT16 WindowProxy::getPosY() const
{
  return posY;
}

void WindowProxy::setPosY( UINT16 whichPos )
{
  posY = whichPos;
}

UINT16 WindowProxy::getWidth() const
{
  return width;
}

void WindowProxy::setWidth( UINT16 whichPos )
{
  width = whichPos;
}

UINT16 WindowProxy::getHeight() const
{
  return height;
}

void WindowProxy::setHeight( UINT16 whichPos )
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
    for ( UINT16 i = 0; i < 2; ++i )
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
}

void WindowProxy::prevZoom()
{
  zoomHistory.prevZoom();
}

void WindowProxy::addZoom( TTime beginTime, TTime endTime,
                           TObjectOrder beginObject, TObjectOrder endObject )
{
  zoomHistory.addZoom( beginTime, endTime, beginObject, endObject );
}

void WindowProxy::addZoom( TTime beginTime, TTime endTime )
{
  zoomHistory.addZoom( beginTime, endTime );
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

void WindowProxy::getAllLevelsSelectedRows( TWindowLevel onLevel, vector< TObjectOrder > &selected )
{
  vector< TObjectOrder > allLevelsSelected;
  TObjectOrder iAppl, iTask, globalTask, iNode, aux;
  switch ( onLevel )
  {
    case TASK:
      for ( vector< TObjectOrder >::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        getTrace()->getTaskLocation( *it, iAppl, aux );
        if ( selectedRow.isSelectedPosition( iAppl, APPLICATION ) )
          allLevelsSelected.push_back( *it );
      }

      break;

    case THREAD:
      for ( vector< TObjectOrder >::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        getTrace()->getThreadLocation( *it, iAppl, iTask, aux );
        globalTask = getTrace()->getGlobalTask( iAppl, iTask );

        if ( selectedRow.isSelectedPosition( iAppl, APPLICATION ) &&
             selectedRow.isSelectedPosition( globalTask, TASK ) )
          allLevelsSelected.push_back( *it );
      }

      break;

    case CPU:
      for ( vector< TObjectOrder >::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        getTrace()->getCPULocation( *it, iNode, aux );
        if ( selectedRow.isSelectedPosition( iNode, NODE ) )
          allLevelsSelected.push_back( *it );
      }
      break;

    default:
      break;
  }

  if ( allLevelsSelected.size() > 0 )
    selected.swap( allLevelsSelected );
}

void WindowProxy::getSelectedRows( TWindowLevel onLevel, vector< TObjectOrder > &selected, bool lookUpLevels )
{
  selectedRow.getSelected( selected, onLevel );

  if ( lookUpLevels )
  {
    getAllLevelsSelectedRows( onLevel, selected );
  }
}

void WindowProxy::getSelectedRows( TWindowLevel onLevel,
                                   vector< TObjectOrder > &selected,
                                   TObjectOrder first, TObjectOrder last, bool lookUpLevels )
{
  selectedRow.getSelected( selected, first, last, onLevel );

  if ( lookUpLevels )
  {
    getAllLevelsSelectedRows( onLevel, selected );
  }
}

void WindowProxy::getGroupLabels( UINT32 whichGroup, vector<string>& onVector ) const
{
  myWindow->getGroupLabels( whichGroup, onVector );
}

bool WindowProxy::getParametersOfFunction( string whichFunction,
    UINT32 &numParameters,
    vector<string> &nameParameters,
    vector< vector< double > >&defaultParameters ) const
{
  return myWindow->getParametersOfFunction( whichFunction,
         numParameters,
         nameParameters,
         defaultParameters );
}
