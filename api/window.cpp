#include "kwindow.h"
#include "kernelconnection.h"
#include "filter.h"

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

Filter *Window::getFilter() const
{
  return NULL;
}

void Window::setFactor( UINT16 whichFactor, TSemanticValue newValue )
{}

void Window::setParent( UINT16 whichParent, Window *whichWindow )
{}

void Window::setWindowBeginTime( TRecordTime whichTime )
{}

void Window::setWindowEndTime( TRecordTime whichTime )
{}

TRecordTime Window::getWindowBeginTime() const
{
  return 0;
}

TRecordTime Window::getWindowEndTime() const
{
  return 0;
}

WindowProxy::WindowProxy( KernelConnection *whichKernel, Trace *whichTrace ):
    Window( whichKernel )
{
  parent1 = NULL;
  parent2 = NULL;
  myWindow = myKernel->newSingleWindow( whichTrace );
}

WindowProxy::WindowProxy( KernelConnection *whichKernel, Window *whichParent1,
                          Window *whichParent2 ):
    Window( whichKernel )
{
  parent1 = whichParent1;
  parent2 = whichParent2;
  myWindow = myKernel->newDerivedWindow( parent1, parent2 );
}

WindowProxy::WindowProxy( KernelConnection *whichKernel ):
    Window( whichKernel )
{
  parent1 = NULL;
  parent2 = NULL;
  myWindow = myKernel->newDerivedWindow();
}

WindowProxy::~WindowProxy()
{
  delete myWindow;
}

Filter *WindowProxy::getFilter() const
{
  if ( !myWindow->isDerivedWindow() )
    return myWindow->getFilter();
  return NULL;
}

void WindowProxy::setFactor( UINT16 whichFactor, TSemanticValue newValue )
{
  if ( myWindow->isDerivedWindow() )
    myWindow->setFactor( whichFactor, newValue );
}

void WindowProxy::setParent( UINT16 whichParent, Window *whichWindow )
{
  if ( myWindow->isDerivedWindow() )
  {
    if ( whichParent == 0 )
      parent1 = whichWindow;
    else if ( whichParent == 1 )
      parent2 = whichWindow;
    myWindow->setParent( whichParent, whichWindow->getConcrete() );
  }
}

void WindowProxy::setWindowBeginTime( TRecordTime whichTime )
{
  winBeginTime = whichTime;
}

void WindowProxy::setWindowEndTime( TRecordTime whichTime )
{
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

Trace *WindowProxy::getTrace() const
{
  return myWindow->getTrace();
}

TWindowLevel WindowProxy::getLevel() const
{
  return myWindow->getLevel();
}

void WindowProxy::setLevel( TWindowLevel whichLevel )
{
  myWindow->setLevel( whichLevel );
}

void WindowProxy::setTimeUnit( TTimeUnit whichUnit )
{
  myWindow->setTimeUnit( whichUnit );
}

TTimeUnit WindowProxy::getTimeUnit()
{
  return myWindow->getTimeUnit();
}

TWindowLevel WindowProxy::getComposeLevel( TWindowLevel whichLevel ) const
{
  return myWindow->getComposeLevel( whichLevel );
}

bool WindowProxy::setLevelFunction( TWindowLevel whichLevel,
                                    SemanticFunction *whichFunction )
{
  return myWindow->setLevelFunction( whichLevel, whichFunction );
}

SemanticFunction *WindowProxy::getLevelFunction( TWindowLevel whichLevel )
{
  return myWindow->getLevelFunction( whichLevel );
}

SemanticFunction *WindowProxy::getFirstUsefulFunction( )
{
  return myWindow->getFirstUsefulFunction();
}

void WindowProxy::setFunctionParam( TWindowLevel whichLevel,
                                    TParamIndex whichParam,
                                    const TParamValue& newValue )
{
  myWindow->setFunctionParam( whichLevel, whichParam, newValue );
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
    for ( vector<RecordList *>::iterator it = myLists.begin(); it != myLists.end(); it++ )
      delete *it;
    myLists.clear();
  }
  for ( int i = 0; i < myWindow->getWindowLevelObjects(); i++ )
    myLists.push_back( NULL );

  myWindow->init( initialTime, create );
}

RecordList *WindowProxy::calcNext( TObjectOrder whichObject )
{
  if ( myLists[ whichObject ] == NULL )
    myLists[ whichObject ] = RecordList::create( myWindow->calcNext( whichObject ) );
  else
    myWindow->calcNext( whichObject );

  return myLists[ whichObject ];
}

RecordList *WindowProxy::calcPrev( TObjectOrder whichObject )
{
  if ( myLists[ whichObject ] == NULL )
    myLists[ whichObject ] = RecordList::create( myWindow->calcPrev( whichObject ) );
  else
    myWindow->calcPrev( whichObject );

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

Window *WindowProxy::getConcrete() const
{
  return myWindow;
}
