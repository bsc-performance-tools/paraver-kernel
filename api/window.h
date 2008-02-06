#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "paraverkerneltypes.h"

class RecordList;
class SemanticFunction;
class Trace;

class Window
{
  public:
    static Window *create();

    virtual ~Window() {}

    virtual Trace *getTrace() const = 0;
    virtual TWindowLevel getLevel() const = 0;
    virtual void setLevel( TWindowLevel whichLevel ) = 0;
    virtual void setTimeUnit( TTimeUnit whichUnit ) = 0;
    virtual TTimeUnit getTimeUnit() = 0;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const = 0;
    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   SemanticFunction *whichFunction ) = 0;
    virtual SemanticFunction *getLevelFunction( TWindowLevel whichLevel ) = 0;
    virtual SemanticFunction *getFirstUsefulFunction( ) = 0;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) = 0;
    virtual RecordList *getRecordList( TObjectOrder whichObject ) = 0;
    virtual RecordList *init( TRecordTime initialTime, TCreateList create ) = 0;
    virtual RecordList *calcNext( TObjectOrder whichObject ) = 0;
    virtual RecordList *calcPrev( TObjectOrder whichObject ) = 0;
    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const = 0;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const = 0;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const = 0;
    virtual bool isDerivedWindow() const = 0;
    virtual TObjectOrder cpuObjectToWindowObject( TCPUOrder whichCPU ) = 0;
    virtual TObjectOrder threadObjectToWindowObject( TThreadOrder whichThread ) = 0;
    virtual TObjectOrder getWindowLevelObjects() = 0;
    virtual TRecordTime traceUnitsToWindowUnits( TRecordTime whichTime ) = 0;
};

#endif // WINDOW_H_INCLUDED
