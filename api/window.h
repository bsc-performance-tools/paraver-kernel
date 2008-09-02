#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "paraverkerneltypes.h"

class KernelConnection;
class Trace;
class RecordList;
class SemanticFunction;
class Filter;

class Window
{
  public:
    // Create Single Window
    static Window *create( KernelConnection *whichKernel, Trace *whichTrace );
    //Create Derived Window
    static Window *create( KernelConnection *whichKernel );
    static Window *create( KernelConnection *whichKernel, Window *whichParent1,
                           Window *whichParent2 );

    Window() {}
    Window( KernelConnection *whichKernel );
    virtual ~Window() {}

    // Specefic for WindowProxy because Single and Derived window
    // SingleWindow
    virtual Filter *getFilter() const;

    //DerivedWindow
    virtual void setFactor( UINT16 whichFactor, TSemanticValue newValue );
    virtual void setParent( UINT16 whichParent, Window *whichWindow );

    // Other
    virtual void setWindowBeginTime( TRecordTime whichTime );
    virtual void setWindowEndTime( TRecordTime whichTime );
    virtual TRecordTime getWindowBeginTime() const;
    virtual TRecordTime getWindowEndTime() const;

    virtual bool getYScaleComputed() const;
    virtual void computeYScale();
    virtual void setComputeYMaxOnInit( bool newValue );
    virtual bool getComputeYMaxOnInit() const;
    virtual void setMaximumY( TSemanticValue whichMax );
    virtual void setMinimumY( TSemanticValue whichMin );
    virtual TSemanticValue getMaximumY();
    virtual TSemanticValue getMinimumY();

    //------------------------------------------------------------
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
    virtual void init( TRecordTime initialTime, TCreateList create ) = 0;
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

    // Specific functions for WindowProxy
    virtual Window *getConcrete() const
    {
      return NULL;
    }

    virtual void setName( string& whichName )
    {}

    virtual string getName() const
    {
      return "";
    }

    virtual UINT16 getPosX() const
    {
      return 0;
    }
    virtual void setPosX( UINT16 whichPos )
    {}

    virtual UINT16 getPosY() const
    {
      return 0;
    }
    virtual void setPosY( UINT16 whichPos )
    {}

    virtual UINT16 getWidth() const
    {
      return 0;
    }
    virtual void setWidth( UINT16 whichPos )
    {}

    virtual UINT16 getHeight() const
    {
      return 0;
    }
    virtual void setHeight( UINT16 whichPos )
    {}

  protected:
    KernelConnection *myKernel;

};


class WindowProxy: public Window
{
  public:
    virtual ~WindowProxy();

    // Specefic for WindowProxy because Single and Derived window
    // SingleWindow
    virtual Filter *getFilter() const;

    //DerivedWindow
    virtual void setFactor( UINT16 whichFactor, TSemanticValue newValue );
    virtual void setParent( UINT16 whichParent, Window *whichWindow );

    // Other
    virtual void setWindowBeginTime( TRecordTime whichTime );
    virtual void setWindowEndTime( TRecordTime whichTime );
    virtual TRecordTime getWindowBeginTime() const;
    virtual TRecordTime getWindowEndTime() const;

    virtual bool getYScaleComputed() const;
    virtual void computeYScale();
    virtual void setComputeYMaxOnInit( bool newValue );
    virtual bool getComputeYMaxOnInit() const;
    virtual void setMaximumY( TSemanticValue whichMax );
    virtual void setMinimumY( TSemanticValue whichMin );
    virtual TSemanticValue getMaximumY();
    virtual TSemanticValue getMinimumY();

    //------------------------------------------------------------
    virtual Trace *getTrace() const;
    virtual TWindowLevel getLevel() const;
    virtual void setLevel( TWindowLevel whichLevel );
    virtual void setTimeUnit( TTimeUnit whichUnit );
    virtual TTimeUnit getTimeUnit();
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const;
    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   SemanticFunction *whichFunction );
    virtual SemanticFunction *getLevelFunction( TWindowLevel whichLevel );
    virtual SemanticFunction *getFirstUsefulFunction( );
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );
    virtual RecordList *getRecordList( TObjectOrder whichObject );
    virtual void init( TRecordTime initialTime, TCreateList create );
    virtual RecordList *calcNext( TObjectOrder whichObject );
    virtual RecordList *calcPrev( TObjectOrder whichObject );
    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const;
    virtual bool isDerivedWindow() const;
    virtual TObjectOrder cpuObjectToWindowObject( TCPUOrder whichCPU );
    virtual TObjectOrder threadObjectToWindowObject( TThreadOrder whichThread );
    virtual TObjectOrder getWindowLevelObjects();
    virtual TRecordTime traceUnitsToWindowUnits( TRecordTime whichTime );

    virtual Window *getConcrete() const;
    virtual void setName( string& whichName );
    virtual string getName() const;
    virtual UINT16 getPosX() const;
    virtual void setPosX( UINT16 whichPos );
    virtual UINT16 getPosY() const;
    virtual void setPosY( UINT16 whichPos );
    virtual UINT16 getWidth() const;
    virtual void setWidth( UINT16 whichPos );
    virtual UINT16 getHeight() const;
    virtual void setHeight( UINT16 whichPos );

  private:
    Window *myWindow;

    UINT16 posX;
    UINT16 posY;
    UINT16 width;
    UINT16 height;

    TRecordTime winBeginTime;
    TRecordTime winEndTime;

    bool computeYMaxOnInit;
    bool yScaleComputed;
    TSemanticValue maximumY;
    TSemanticValue minimumY;
    TSemanticValue computedMaxY;
    TSemanticValue computedMinY;

    vector<RecordList *> myLists;

    // Must store the associated proxies
    Window *parent1;
    Window *parent2;

    // GUI related attributes
    string name;

    // For Single Window
    WindowProxy( KernelConnection *whichKernel, Trace *whichTrace );
    // For Derived Window
    WindowProxy( KernelConnection *whichKernel );
    WindowProxy( KernelConnection *whichKernel, Window *whichParent1,
                 Window *whichParent2 );

    void init();

    friend Window *Window::create( KernelConnection *, Trace * );
    friend Window *Window::create( KernelConnection * );
    friend Window *Window::create( KernelConnection *, Window *, Window * );
};

#endif // WINDOW_H_INCLUDED
