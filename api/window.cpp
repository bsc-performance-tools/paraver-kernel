#include "kwindow.h"

Window *Window::create( KernelConnection *whichKernel )
{
  return new WindowProxy( whichKernel );
}

Window::Window( KernelConnection *whichKernel ) : myKernel( whichKernel )
{}

WindowProxy::WindowProxy( KernelConnection *whichKernel ):
    Window( whichKernel )
{}

WindowProxy::~WindowProxy()
{}

Trace *WindowProxy::getTrace() const
{
  return 0;
}

TWindowLevel WindowProxy::getLevel() const
{
  return ( TWindowLevel ) 0;
}

void WindowProxy::setLevel( TWindowLevel whichLevel )
{}

void WindowProxy::setTimeUnit( TTimeUnit whichUnit )
{}

TTimeUnit WindowProxy::getTimeUnit()
{
  return 0;
}

TWindowLevel WindowProxy::getComposeLevel( TWindowLevel whichLevel ) const
{
  return ( TWindowLevel ) 0;
}

bool WindowProxy::setLevelFunction( TWindowLevel whichLevel,
                                    SemanticFunction *whichFunction )
{
  return 0;
}

SemanticFunction *WindowProxy::getLevelFunction( TWindowLevel whichLevel )
{
  return 0;
}

SemanticFunction *WindowProxy::getFirstUsefulFunction( )
{
  return 0;
}

void WindowProxy::setFunctionParam( TWindowLevel whichLevel,
                                    TParamIndex whichParam,
                                    const TParamValue& newValue )
{}

RecordList *WindowProxy::getRecordList( TObjectOrder whichObject )
{
  return 0;
}

RecordList *WindowProxy::init( TRecordTime initialTime, TCreateList create )
{
  return 0;
}

RecordList *WindowProxy::calcNext( TObjectOrder whichObject )
{
  return 0;
}

RecordList *WindowProxy::calcPrev( TObjectOrder whichObject )
{
  return 0;
}

TRecordTime WindowProxy::getBeginTime( TObjectOrder whichObject ) const
{
  return 0;
}

TRecordTime WindowProxy::getEndTime( TObjectOrder whichObject ) const
{
  return 0;
}

TSemanticValue WindowProxy::getValue( TObjectOrder whichObject ) const
{
  return 0;
}

bool WindowProxy::isDerivedWindow() const
{
  return 0;
}

TObjectOrder WindowProxy::cpuObjectToWindowObject( TCPUOrder whichCPU )
{
  return 0;
}

TObjectOrder WindowProxy::threadObjectToWindowObject( TThreadOrder whichThread )
{
  return 0;
}

TObjectOrder WindowProxy::getWindowLevelObjects()
{
  return 0;
}

TRecordTime WindowProxy::traceUnitsToWindowUnits( TRecordTime whichTime )
{
  return 0;
}
