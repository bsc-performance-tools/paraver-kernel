#include <iostream>
#include <sstream>

#include "kernelconnection.h"
#include "filter.h"
#include "trace.h"
#include "window.h"
#include "recordlist.h"

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

  number_of_clones = 0;
}

WindowProxy::WindowProxy( KernelConnection *whichKernel, Trace *whichTrace ):
    Window( whichKernel ), myTrace( whichTrace )
{
  parent1 = NULL;
  parent2 = NULL;
  myWindow = myKernel->newSingleWindow( whichTrace );
  myFilter = myKernel->newFilter( myWindow->getFilter() );
  number_of_clones = 0;
  init();
}

WindowProxy::WindowProxy( KernelConnection *whichKernel, Window *whichParent1,
                          Window *whichParent2 ):
    Window( whichKernel ), myTrace( whichParent1->getTrace() )
{
  parent1 = whichParent1;
  parent2 = whichParent2;
  myWindow = myKernel->newDerivedWindow( parent1, parent2 );
  myFilter = NULL;
  number_of_clones = 0;
  init();
}

WindowProxy::WindowProxy( KernelConnection *whichKernel ):
    Window( whichKernel ), myTrace( NULL )
{
  parent1 = NULL;
  parent2 = NULL;
  myWindow = myKernel->newDerivedWindow();
  myFilter = NULL;
  number_of_clones = 0;
//  init();
}

void WindowProxy::init()
{
  winBeginTime = 0.0;
  winEndTime = myTrace->getEndTime();

  computeYMaxOnInit = false;
  yScaleComputed = false;
  maximumY = Window::getMaximumY();
  minimumY = Window::getMinimumY();

  myCodeColor = myTrace->getCodeColor();
  myGradientColor = myTrace->getGradientColor();
  codeColor = true;

  drawModeObject = DRAW_MAXIMUM;
  drawModeTime = DRAW_MAXIMUM;

  showWindow = true;
  changed = false;
  redraw = false;
  commLines = true;

  child = NULL;

  selectedRow.init( getTrace() );
}

WindowProxy::~WindowProxy()
{
  if ( myFilter != NULL )
    delete myFilter;
  delete myWindow;
}

Filter *WindowProxy::getFilter() const
{
  if ( !myWindow->isDerivedWindow() )
    return myFilter;
  return NULL;
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

  if ( clonedWindow->isDerivedWindow())
  {
    clonedWindow->parent1 = parent1->clone();
    clonedWindow->myWindow->setParent( 0, clonedWindow->parent1->getConcrete() );
    clonedWindow->parent2 = parent2->clone();
    clonedWindow->myWindow->setParent( 1, clonedWindow->parent2->getConcrete() );
    clonedWindow->parent1->setChild( clonedWindow );
    clonedWindow->parent2->setChild( clonedWindow );
  }
  else
  {
    clonedWindow->myFilter = myKernel->newFilter( myWindow->getFilter() );
    clonedWindow->myFilter->copyEventsSection( myFilter );
    clonedWindow->myFilter->copyCommunicationsSection( myFilter );
  }

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

void WindowProxy::computeYScale()
{
  if ( !yScaleComputed )
  {
    vector< TObjectOrder > selected;
    getSelectedRows( getLevel(), selected );

    init( winBeginTime, NONE );

    for ( vector< TObjectOrder >::iterator obj = selected.begin(); obj != selected.end(); ++obj )
    {
      while ( getBeginTime( *obj ) < winEndTime )
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
  yScaleComputed = false;

  myWindow->setLevel( whichLevel );
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

void WindowProxy::init( TRecordTime initialTime, TCreateList create )
{
  if ( myLists.begin() != myLists.end() )
  {
    for ( vector<RecordList *>::iterator it = myLists.begin(); it != myLists.end(); ++it )
      delete *it;
    if (myLists.begin() != myLists.end()) // solo sintoma de que algo no va bien
      myLists.clear();
  }
  for ( int i = 0; i < myWindow->getWindowLevelObjects(); i++ )
    myLists.push_back( NULL );

  myWindow->init( initialTime, create );
  yScaleComputed = true;
  computeYMaxOnInit = false;
  computedMaxY = computedMinY = getValue( 0 );

//#warning Set rows as in CFG
  // Simple initialization - all showed; CHANGE THIS
//  vector< bool > selected;
//  selected.assign( myWindow->getWindowLevelObjects(), true );
//  setSelectedRows( selected );
}

RecordList *WindowProxy::calcNext( TObjectOrder whichObject )
{
  if ( myLists[ whichObject ] == NULL )
    myLists[ whichObject ] = RecordList::create( myWindow->calcNext( whichObject ) );
  else
    myWindow->calcNext( whichObject );

  TSemanticValue objValue = getValue( whichObject );
  if ( computedMaxY < objValue )
    computedMaxY = objValue;
  if ( computedMinY > objValue )
    computedMinY = objValue;

  return myLists[ whichObject ];
}

RecordList *WindowProxy::calcPrev( TObjectOrder whichObject )
{
  if ( myLists[ whichObject ] == NULL )
    myLists[ whichObject ] = RecordList::create( myWindow->calcPrev( whichObject ) );
  else
    myWindow->calcPrev( whichObject );

  TSemanticValue objValue = getValue( whichObject );
  if ( computedMaxY < objValue )
    computedMaxY = objValue;
  if ( computedMinY > objValue )
    computedMinY = objValue;

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
  return (!codeColor && (myGradientColor.getAllowOutOfScale()));
//  return !codeColor;
}


bool WindowProxy::IsNotNullGradientColorSet() const
{
  return (!codeColor && !(myGradientColor.getAllowOutOfScale()));
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

void WindowProxy::getSelectedRows( TWindowLevel onLevel, vector< bool > &selected )
{
  selectedRow.getSelected( selected, onLevel );
}

void WindowProxy::getSelectedRows( TWindowLevel onLevel, vector< TObjectOrder > &selected )
{
  selectedRow.getSelected( selected, onLevel );
}

void WindowProxy::getSelectedRows( TWindowLevel onLevel,
                                   vector< TObjectOrder > &selected,
                                   TObjectOrder first, TObjectOrder last )
{
  selectedRow.getSelected( selected, first, last, onLevel );
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
