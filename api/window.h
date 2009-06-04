#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <string>
#include "paraverkerneltypes.h"
#include "semanticcolor.h"
#include "drawmode.h"
#include "selectionmanagement.h"

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
    virtual Filter *getFilter() const
    {
      return NULL;
    }

    //DerivedWindow
    virtual void setFactor( UINT16 whichFactor, TSemanticValue newValue ) {}
    virtual TSemanticValue getFactor( UINT16 whichFactor ) const
    {
      return 1.0;
    }
    virtual void setParent( UINT16 whichParent, Window *whichWindow ) {}
    virtual void setChild( Window *whichWindow ) {}
    virtual Window *getChild()
    {
      return NULL;
    }
    virtual Window *getParent( UINT16 whichParent )
    {
      return NULL;
    }


    // Other
    virtual void setWindowBeginTime( TRecordTime whichTime ) {}
    virtual void setWindowEndTime( TRecordTime whichTime ) {}
    virtual TRecordTime getWindowBeginTime() const
    {
      return 0;
    }
    virtual TRecordTime getWindowEndTime() const
    {
      return 0;
    }

    virtual bool getYScaleComputed() const
    {
      return false;
    }
    virtual void computeYScale() {}
    virtual void setComputeYMaxOnInit( bool newValue ) {}
    virtual bool getComputeYMaxOnInit() const
    {
      return false;
    }
    virtual void setMaximumY( TSemanticValue whichMax ) {}
    virtual void setMinimumY( TSemanticValue whichMin ) {}
    virtual TSemanticValue getMaximumY()
    {
      return 15.0;
    }
    virtual TSemanticValue getMinimumY()
    {
      return 0.0;
    }

    virtual Window* clone( )
    {
      return NULL;
    }

    //------------------------------------------------------------
    virtual Trace *getTrace() const = 0;
    virtual TWindowLevel getLevel() const = 0;
    virtual void setLevel( TWindowLevel whichLevel ) = 0;
    virtual void setTimeUnit( TTimeUnit whichUnit ) = 0;
    virtual TTimeUnit getTimeUnit() const = 0;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const = 0;
    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const string& whichFunction ) = 0;
    virtual string getLevelFunction( TWindowLevel whichLevel ) = 0;
    virtual string getFirstUsefulFunction( ) = 0;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) = 0;
    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const = 0;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const = 0;
    virtual string getFunctionParamName( TWindowLevel whichLevel,
                                         TParamIndex whichParam ) const = 0;
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
    virtual SemanticInfoType getSemanticInfoType() const = 0;

    // Specific functions for WindowProxy
    virtual Window *getConcrete() const
    {
      return NULL;
    }

    virtual void setName( const string& whichName )
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
    virtual void setDrawModeObject( DrawModeMethod method )
    {}
    virtual DrawModeMethod getDrawModeObject() const
    {
      return DRAW_MAXIMUM;
    }
    virtual void setDrawModeTime( DrawModeMethod method )
    {}
    virtual DrawModeMethod getDrawModeTime() const
    {
      return DRAW_MAXIMUM;
    }
    virtual CodeColor& getCodeColor()
    {
      CodeColor *tmp = NULL;
      return *tmp;
    }
    virtual GradientColor& getGradientColor()
    {
      GradientColor *tmp = NULL;
      return *tmp;
    }
    virtual bool getShowWindow() const
    {
      return true;
    }
    virtual void setShowWindow( bool newValue )
    {}
    virtual void setCodeColorMode()
    {}
    virtual bool IsCodeColorSet() const
    {
      return true;
    }
    virtual bool IsGradientColorSet() const
    {
      return false;
    }
    virtual bool IsNotNullGradientColorSet() const
    {
      return false;
    }
    virtual void setGradientColorMode()
    {}
    virtual void allowOutOfScale( bool activate )
    {}
    virtual void allowOutliers( bool activate )
    {}
    virtual rgb calcColor( TSemanticValue whichValue, Window& whichWindow )
    {
      rgb tmp = { 0, 0, 0 };
      return tmp;
    }
    virtual bool getChanged() const
    {
      return false;
    }
    virtual void setChanged( bool newValue )
    {}
    virtual bool getRedraw() const
    {
      return false;
    }
    virtual void setRedraw( bool newValue )
    {}
    virtual bool getDrawCommLines() const
    {
      return true;
    }
    virtual void setDrawCommLines( bool newValue )
    {}
    virtual void getAllSemanticFunctions( TSemanticGroup whichGroup,
                                          vector<string>& onVector ) const
    {}

    virtual void setSelectedRows( vector< bool > &selected )
    {}
    virtual void setSelectedRows( vector< TObjectOrder > &selection )
    {}
    virtual void getSelectedRows( vector< bool > &selected )
    {}
    virtual void getSelectedRows( vector< TObjectOrder > &selection )
    {}
    virtual void getSelectedRows( vector< TObjectOrder > &selection,
                                  TObjectOrder first, TObjectOrder last )
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
    virtual TSemanticValue getFactor( UINT16 whichFactor ) const;
    virtual void setParent( UINT16 whichParent, Window *whichWindow );
    virtual void setChild( Window *whichWindow );
    virtual Window *getChild();
    virtual Window *getParent( UINT16 whichParent );

    virtual Window *clone( );

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
    virtual TTimeUnit getTimeUnit() const;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const;
    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const string& whichFunction );
    virtual string getLevelFunction( TWindowLevel whichLevel );
    virtual string getFirstUsefulFunction( );
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );
    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const;
    virtual string getFunctionParamName( TWindowLevel whichLevel,
                                         TParamIndex whichParam ) const;
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
    virtual SemanticInfoType getSemanticInfoType() const;
    virtual void getAllSemanticFunctions( TSemanticGroup whichGroup,
                                          vector<string>& onVector ) const;

    virtual Window *getConcrete() const;
    virtual void setName( const string& whichName );
    virtual string getName() const;
    virtual UINT16 getPosX() const;
    virtual void setPosX( UINT16 whichPos );
    virtual UINT16 getPosY() const;
    virtual void setPosY( UINT16 whichPos );
    virtual UINT16 getWidth() const;
    virtual void setWidth( UINT16 whichPos );
    virtual UINT16 getHeight() const;
    virtual void setHeight( UINT16 whichPos );
    virtual void setDrawModeObject( DrawModeMethod method );
    virtual DrawModeMethod getDrawModeObject() const;
    virtual void setDrawModeTime( DrawModeMethod method );
    virtual DrawModeMethod getDrawModeTime() const;
    virtual CodeColor& getCodeColor();
    virtual GradientColor& getGradientColor();
    virtual bool getShowWindow() const;
    virtual void setShowWindow( bool newValue );
    virtual void setCodeColorMode();
    virtual void setGradientColorMode();
    virtual bool IsCodeColorSet() const;
    virtual bool IsGradientColorSet() const;
    virtual bool IsNotNullGradientColorSet() const;

    virtual void allowOutOfScale( bool activate );
    virtual void allowOutliers( bool activate );
    virtual rgb calcColor( TSemanticValue whichValue, Window& whichWindow );
    virtual bool getChanged() const;
    virtual void setChanged( bool newValue );
    virtual bool getRedraw() const;
    virtual void setRedraw( bool newValue );
    virtual bool getDrawCommLines() const;
    virtual void setDrawCommLines( bool newValue );

    virtual void setSelectedRows( vector< bool > &selected );
    virtual void setSelectedRows( vector< TObjectOrder > &selected );
    virtual void getSelectedRows( vector< bool > &selected );
    virtual void getSelectedRows( vector< TObjectOrder > &selected );
    virtual void getSelectedRows( vector< TObjectOrder > &selected,
                                  TObjectOrder first, TObjectOrder last );
//    virtual TObjectOrder getFirstSelectedRow();
//    virtual TObjectOrder getLastSelectedRow();

  private:
    Window *myWindow;
    Trace *myTrace;
    Filter *myFilter;

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
    Window *child;

    // GUI related attributes
    string name;
    CodeColor myCodeColor;
    GradientColor myGradientColor;
    bool codeColor;
    DrawModeMethod drawModeObject;
    DrawModeMethod drawModeTime;
    bool showWindow;
    bool changed;
    bool redraw;
    bool commLines;

    // Row selection
    SelectionManagement< TObjectOrder > selectedRow;

    // For Clone
    WindowProxy();
    // For Single Window
    WindowProxy( KernelConnection *whichKernel, Trace *whichTrace );
    // For Derived Window
    WindowProxy( KernelConnection *whichKernel );
    WindowProxy( KernelConnection *whichKernel, Window *whichParent1,
                 Window *whichParent2 );

    int number_of_clones;

    void init();

    friend Window *Window::create( KernelConnection *, Trace * );
    friend Window *Window::create( KernelConnection * );
    friend Window *Window::create( KernelConnection *, Window *, Window * );
};

#endif // WINDOW_H_INCLUDED
