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
  cout << "Window" << endl;
  return NULL;
}

void Window::setFactor( UINT16 whichFactor, TSemanticValue newValue )
{}

void Window::setParent( UINT16 whichParent, Window *whichWindow )
{}

WindowProxy::WindowProxy( KernelConnection *whichKernel, Trace *whichTrace ):
    Window( whichKernel )
{
  myWindow = myKernel->newSingleWindow( whichTrace );
}

WindowProxy::WindowProxy( KernelConnection *whichKernel, Window *parent1, Window *parent2 ):
    Window( whichKernel )
{
  myWindow = myKernel->newDerivedWindow( parent1, parent2 );
}

WindowProxy::WindowProxy( KernelConnection *whichKernel ):
    Window( whichKernel )
{
  myWindow = myKernel->newDerivedWindow();
}

WindowProxy::~WindowProxy()
{
  delete myWindow;
}

Filter *WindowProxy::getFilter() const
{
  cout << "WindowProxy" << endl;
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
    myWindow->setParent( whichParent, whichWindow );
}

Trace *WindowProxy::getTrace() const
{
  return myWindow->getTrace();;
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
  return RecordList::create( myWindow->getRecordList( whichObject ) );
}

RecordList *WindowProxy::init( TRecordTime initialTime, TCreateList create )
{
  return myWindow->init( initialTime, create );
}

RecordList *WindowProxy::calcNext( TObjectOrder whichObject )
{
  return RecordList::create( myWindow->calcNext( whichObject ) );
}

RecordList *WindowProxy::calcPrev( TObjectOrder whichObject )
{
  return RecordList::create( myWindow->calcPrev( whichObject ) );
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
