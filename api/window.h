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


#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <string>
#include <map>

#include "paraverkerneltypes.h"
#include "semanticcolor.h"
#include "drawmode.h"
#include "zoomhistory.h"
#include "selectionmanagement.h"
#include "paraverlabels.h"
#include "syncwindows.h"

#ifdef WIN32
#include <hash_set>
#else
#include <ext/hash_set>
#endif

#ifdef WIN32
using namespace stdext;
#else
using namespace __gnu_cxx;
#endif

struct commCoord
{
  PRV_INT32 fromTime;
  PRV_INT32 toTime;
  PRV_INT32 toRow;
  TRecordType recType;

  bool operator==( const commCoord& b ) const
  {
    return fromTime == b.fromTime &&
           toTime   == b.toTime   &&
           toRow    == b.toRow    &&
           recType  == b.recType;
  }

#ifdef WIN32
  bool operator<( const commCoord& b ) const
  {
    return true;
  }

  size_t hash() const {
    return ( ( fromTime + toTime + toRow ) * 100 ) + recType;
  }
#endif
};

#ifdef WIN32
  namespace stdext
  {
    template<> class hash_compare<commCoord>
    {
      public :
        static const size_t bucket_size = 4;
        static const size_t min_buckets = 8;
        hash_compare() { }

        size_t operator()(const commCoord &cc) const
        {
          return cc.hash();
        }

        bool operator()(const commCoord &cc1, const commCoord &cc2) const
        {
          return (cc1 < cc2);
        }
    };
  }
#endif

#ifndef WIN32
struct hashCommCoord
{
  size_t operator()( const commCoord& x ) const
  {
    return ( ( x.fromTime + x.toTime + x.toRow ) * 100 ) + x.recType;
  }
};
#endif

class KernelConnection;
class Trace;
class RecordList;
class SemanticFunction;
class Filter;
class ProgressController;
class Histogram;

class Window
{
  public:
    enum TObjectLabels
    {
      ALL_LABELS = 0,
      SPACED_LABELS,
      POWER2_LABELS
    };

    enum TObjectAxisSize
    {
      CURRENT_LEVEL = 0,
      ALL_LEVELS,
      ZERO_PERC,
      FIVE_PERC,
      TEN_PERC,
      TWENTYFIVE_PERC
    };

    // Create Single Window
    static Window *create( KernelConnection *whichKernel, Trace *whichTrace );
    //Create Derived Window
    static Window *create( KernelConnection *whichKernel );
    static Window *create( KernelConnection *whichKernel, Window *whichParent1,
                           Window *whichParent2 );

    static bool compatibleLevels( Window *window1, Window *window2 );

    Window() {}
    Window( KernelConnection *whichKernel );
    virtual ~Window() {}

    KernelConnection *getKernel() const
    {
      return myKernel;
    }

    // Specific for WindowProxy because Single and Derived window
    // SingleWindow
    virtual Filter *getFilter() const
    {
      return NULL;
    }

    //DerivedWindow
    virtual void setFactor( PRV_UINT16 whichFactor, TSemanticValue newValue ) {}
    virtual TSemanticValue getFactor( PRV_UINT16 whichFactor ) const
    {
      return 1.0;
    }
    virtual void setParent( PRV_UINT16 whichParent, Window *whichWindow ) {}
    virtual void setChild( Window *whichWindow ) {}
    virtual Window *getChild()
    {
      return NULL;
    }
    virtual Window *getParent( PRV_UINT16 whichParent ) const
    {
      return NULL;
    }


    // Other
    virtual void setDestroy( bool newValue ) {}
    virtual bool getDestroy() const
    {
      return false;
    }
    virtual void setReady( bool newValue ) {}
    virtual bool getReady() const
    {
      return false;
    }

    virtual void setUsedByHistogram( Histogram *whichHisto ) {}
    virtual void unsetUsedByHistogram( Histogram *whichHisto ) {}
    virtual bool getUsedByHistogram() const
    {
      return false;
    }
    virtual std::set<Histogram *> getHistograms() const
    {
      return std::set<Histogram *>();
    }

    virtual void setWindowBeginTime( TRecordTime whichTime, bool isBroadcast = false ) {}
    virtual void setWindowEndTime( TRecordTime whichTime, bool isBroadcast = false ) {}
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
    virtual void computeYScaleMin() {}
    virtual void computeYScaleMax() {}
    virtual void computeYScale( ProgressController *progress = NULL ) {}
    virtual void setComputeYMaxOnInit( bool newValue ) {}
    virtual bool getComputeYMaxOnInit() const
    {
      return false;
    }
    virtual void setMaximumY( TSemanticValue whichMax ) {}
    virtual void setMinimumY( TSemanticValue whichMin ) {}
    virtual TSemanticValue getMaximumY()
    {
      return 18.0;
    }
    virtual TSemanticValue getMinimumY()
    {
      return 0.0;
    }

    virtual bool getExistSemanticZero() const
    {
      return false;
    }

    virtual Window* clone( bool recursiveClone = false )
    {
      return NULL;
    }

    virtual bool getShowProgressBar() const
    {
      return true;
    }

    //------------------------------------------------------------
    virtual Trace *getTrace() const = 0;
    virtual TWindowLevel getLevel() const = 0;
    virtual void setLevel( TWindowLevel whichLevel ) = 0;
    virtual TWindowLevel getMinAcceptableLevel() const = 0;
    virtual void setTimeUnit( TTimeUnit whichUnit ) = 0;
    virtual TTimeUnit getTimeUnit() const = 0;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const = 0;
    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const std::string& whichFunction ) = 0;
    virtual std::string getLevelFunction( TWindowLevel whichLevel ) = 0;
    virtual std::string getFirstUsefulFunction( ) = 0;
    virtual TWindowLevel getFirstFreeCompose() const = 0;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) = 0;

    // Extra composes
    virtual void addExtraCompose( TWindowLevel whichLevel ) = 0;
    virtual void removeExtraCompose( TWindowLevel whichLevel ) = 0;
    virtual size_t getExtraNumPositions( TWindowLevel whichLevel ) const = 0;
    virtual bool setExtraLevelFunction( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        const std::string& whichFunction ) = 0;
    virtual std::string getExtraLevelFunction( TWindowLevel whichLevel,
                                               size_t whichPosition ) = 0;
    virtual void setExtraFunctionParam( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        TParamIndex whichParam,
                                        const TParamValue& newValue ) = 0;
    virtual TParamIndex getExtraFunctionNumParam( TWindowLevel whichLevel, size_t whichPosition ) const = 0;
    virtual TParamValue getExtraFunctionParam( TWindowLevel whichLevel,
                                               size_t whichPosition,
                                               TParamIndex whichParam ) const = 0;
    virtual std::string getExtraFunctionParamName( TWindowLevel whichLevel,
                                                   size_t whichPosition,
                                                   TParamIndex whichParam ) const = 0;

    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const = 0;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const = 0;
    virtual std::string getFunctionParamName( TWindowLevel whichLevel,
                                         TParamIndex whichParam ) const = 0;
    virtual RecordList *getRecordList( TObjectOrder whichObject ) = 0;
    virtual void init( TRecordTime initialTime, TCreateList create, bool updateLimits = true ) = 0;
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits = true ) = 0;
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create,
                          TSemanticValue &rowComputedMaxY, TSemanticValue &rowComputedMinY,
                          int& rowComputedZeros,
                          bool updateLimits = true )
    {}
    virtual RecordList *calcNext( TObjectOrder whichObject, bool updateLimits = true ) = 0;
    virtual RecordList *calcNext( TObjectOrder whichObject,
                                  TSemanticValue& rowComputedMaxY, TSemanticValue& rowComputedMinY,
                                  int& rowComputedZeros,
                                  bool updateLimits = true )
    { return NULL; }
    virtual RecordList *calcPrev( TObjectOrder whichObject, bool updateLimits = true ) = 0;
    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const = 0;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const = 0;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const = 0;
    virtual bool isDerivedWindow() const = 0;
    virtual TObjectOrder cpuObjectToWindowObject( TCPUOrder whichCPU ) = 0;
    virtual TObjectOrder threadObjectToWindowObject( TThreadOrder whichThread ) = 0;
    virtual TObjectOrder getWindowLevelObjects() const = 0;
    virtual TRecordTime customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const = 0;
    virtual TRecordTime traceUnitsToCustomUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const = 0;
    virtual TRecordTime traceUnitsToWindowUnits( TRecordTime whichTime ) const = 0;
    virtual TRecordTime windowUnitsToTraceUnits( TRecordTime whichTime ) const = 0;
    virtual SemanticInfoType getSemanticInfoType() const = 0;

    // Specific functions for WindowProxy
    virtual Window *getConcrete() const
    {
      return NULL;
    }

    virtual void setName( const std::string& whichName )
    {}

    virtual std::string getName() const
    {
      return "";
    }

    virtual bool isLevelProcessModel() const
    {
      return true;
    }

    virtual PRV_UINT16 getPosX() const
    {
      return 0;
    }
    virtual void setPosX( PRV_UINT16 whichPos )
    {}

    virtual PRV_UINT16 getPosY() const
    {
      return 0;
    }
    virtual void setPosY( PRV_UINT16 whichPos )
    {}

    virtual PRV_UINT16 getWidth() const
    {
      return 0;
    }
    virtual void setWidth( PRV_UINT16 whichPos )
    {}

    virtual PRV_UINT16 getHeight() const
    {
      return 0;
    }
    virtual void setHeight( PRV_UINT16 whichPos )
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
    virtual bool getSemanticScaleMinAtZero()
    {
      return false;
    }
    virtual bool getShowWindow() const
    {
      return true;
    }
    virtual void setShowWindow( bool newValue )
    {}
    virtual void setShowChildrenWindow( bool newValue )
    {}
    virtual bool getRaiseWindow() const
    {
      return false;
    }
    virtual void setRaiseWindow( bool newValue )
    {}
    virtual void setCodeColorMode()
    {}
    virtual void setGradientColorMode()
    {}
    virtual void setNotNullGradientColorMode()
    {}
    virtual void setFunctionLineColorMode()
    {}
    virtual void setFusedLinesColorMode()
    {}
    virtual void setPunctualColorMode()
    {}
    virtual void setSemanticScaleMinAtZero( bool newValue )
    {}

    virtual bool isCodeColorSet() const
    {
      return true;
    }
    virtual bool isGradientColorSet() const
    {
      return false;
    }
    virtual bool isNotNullGradientColorSet() const
    {
      return false;
    }
    virtual bool isFunctionLineColorSet() const
    {
      return false;
    }
    virtual bool isFusedLinesColorSet() const
    {
      return false;
    }
    virtual bool isPunctualColorSet() const
    {
      return false;
    }
    virtual PRV_UINT16 getPixelSize() const
    {
      return 1;
    }
    virtual void setPixelSize( PRV_UINT16 whichSize )
    {}
    virtual Window *getPunctualColorWindow() const
    {
      return NULL;
    }
    virtual void setPunctualColorWindow( Window *whichWindow )
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
    virtual bool isColorOutlier( rgb whichColor ) const
    {
      return true;
    }
    virtual bool getUseCustomPalette() const
    {
      return false;
    }
    virtual void setUseCustomPalette( bool newValue ) 
    {}
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
    virtual bool getForceRedraw() const
    {
      return false;
    }
    virtual void setForceRedraw( bool newValue )
    {}
    virtual bool getDrawCommLines() const
    {
      return true;
    }
    virtual void setDrawCommLines( bool newValue )
    {}
    virtual bool getDrawFlags() const
    {
      return false;
    }
    virtual void setDrawFlags( bool newValue )
    {}
    virtual void getAllSemanticFunctions( TSemanticGroup whichGroup,
                                          std::vector<std::string>& onVector ) const
    {}

    virtual bool emptyPrevZoom() const
    {
      return true;
    }

    virtual bool emptyNextZoom() const
    {
      return true;
    }

    virtual void addZoom( TTime beginTime, TTime endTime,
                          TObjectOrder beginObject, TObjectOrder endObject,
                          bool isBroadCast = false )
    {}
    virtual void addZoom( TTime beginTime, TTime endTime, bool isBroadCast = false )
    {}
    virtual void addZoom( TObjectOrder beginObject, TObjectOrder endObject )
    {}
    virtual void nextZoom()
    {}
    virtual void prevZoom()
    {}
    virtual void setZoomFirstDimension( std::pair<TTime, TTime> &dim )
    {}
    virtual void setZoomSecondDimension( std::pair<TObjectOrder, TObjectOrder>  &dim )
    {}
    virtual std::pair<TTime, TTime> getZoomFirstDimension() const
    {
      return std::pair<TTime, TTime>();
    }
    virtual std::pair<TObjectOrder, TObjectOrder> getZoomSecondDimension() const
    {
      return std::pair<TObjectOrder, TObjectOrder>();
    }
    virtual std::pair<TObjectOrder, TObjectOrder> getPrevZoomSecondDimension() const
    {
      return std::pair<TObjectOrder, TObjectOrder>();
    }
    virtual std::pair<TObjectOrder, TObjectOrder> getNextZoomSecondDimension() const
    {
      return std::pair<TObjectOrder, TObjectOrder>();
    }

    virtual void addToSyncGroup( TGroupId whichGroup )
    {}
    virtual void removeFromSync()
    {}
    virtual bool isSync() const
    {
      return false;
    }
    virtual TGroupId getSyncGroup() const
    {
      return 0;
    }


    virtual SelectionManagement< TObjectOrder, TWindowLevel > *getSelectedRows()
    {
      return NULL;
    }
    virtual void setSelectedRows( TWindowLevel onLevel, std::vector< bool > &selected )
    {}
    virtual void setSelectedRows( TWindowLevel onLevel, std::vector< TObjectOrder > &selection )
    {}
    virtual void getSelectedRows( TWindowLevel onLevel, std::vector< bool > &selected, bool lookUpLevels = false )
    {}
    virtual void getSelectedRows( TWindowLevel onLevel, std::vector< bool > &selected,
                                  TObjectOrder first, TObjectOrder last, bool lookUpLevels = false )
    {}
    virtual void getSelectedRows( TWindowLevel onLevel, std::vector< TObjectOrder > &selection, bool lookUpLevels = false )
    {}
    virtual void getSelectedRows( TWindowLevel onLevel,
                                  std::vector< TObjectOrder > &selection,
                                  TObjectOrder first, TObjectOrder last, bool lookUpLevels = false )
    {}
    virtual TObjectOrder shiftFirst( TObjectOrder whichFirst, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, TWindowLevel level ) const
    {
      return 0;
    }
    virtual TObjectOrder shiftLast( TObjectOrder whichLast, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, TWindowLevel level ) const
    {
      return 0;
    }

    virtual bool hasLevelSomeSelectedObject( TWindowLevel onLevel )
    {
      return true;
    }

    virtual void getGroupLabels( PRV_UINT32 whichGroup, std::vector<std::string>& onVector ) const = 0;
    virtual bool getParametersOfFunction( std::string whichFunction,
                                          PRV_UINT32 &numParameters,
                                          std::vector<std::string> &nameParameters,
                                          std::vector< std::vector< double > >&defaultParameters ) const = 0;

    virtual void setObjectLabels( Window::TObjectLabels whichLabels )
    {}
    virtual Window::TObjectLabels getObjectLabels() const
    {
      return Window::SPACED_LABELS;
    }

    virtual void setObjectAxisSize( Window::TObjectAxisSize whichSize )
    {}
    virtual Window::TObjectAxisSize getObjectAxisSize() const
    {
      return Window::CURRENT_LEVEL;
    }

    // CFG4D
    virtual void setCFG4DEnabled( bool enabled )
    {}
    virtual bool getCFG4DEnabled() const
    {
      return false;
    }

    virtual void setCFG4DMode( bool mode )
    {}
    virtual bool getCFG4DMode() const
    {
      return false;
    }

    virtual bool existsCFG4DAlias( const std::string &property ) const
    {
      return false;
    }
    virtual bool existsCFG4DAlias( const TSingleTimelineProperties &propertyIndex ) const
    {
      return false;
    }

    virtual bool existsCFG4DAlias( const TDerivedTimelineProperties &propertyIndex ) const
    {
      return false;
    }

    virtual void setCFG4DAlias( const std::string &property, const std::string &alias )
    {}

    virtual std::string getCFG4DAlias( const std::string &property ) const
    {
      return std::string( "" );
    }
    virtual std::string getCFG4DAlias( const TSingleTimelineProperties &propertyIndex ) const
    {
      return std::string( "" );
    }
    virtual std::string getCFG4DAlias( const TDerivedTimelineProperties &propertyIndex ) const
    {
      return std::string( "" );
    }

    virtual void setCFG4DAliasList( const std::map< std::string, std::string >& aliasList )
    {}

    virtual const std::map< std::string, std::string > getCFG4DAliasList() const
    {
      return std::map< std::string, std::string >();
    }

    virtual const std::vector< std::string > getCFG4DFullTagList()
    {
      return std::vector< std::string >();
    }

    typedef std::pair< std::pair< std::string, std::string >, PRV_UINT32 > TParamAliasKey;
    typedef std::map< TParamAliasKey, std::string > TParamAlias;

    virtual void setCFG4DParamAlias( const TParamAlias &whichParamAlias )
    {}

    virtual void setCFG4DParamAlias( std::string semanticLevel,
                                     std::string function,
                                     PRV_UINT32 numParameter,
                                     std::string paramAlias )
    {}

    virtual const std::vector< TParamAliasKey > getCFG4DCurrentSelectedFullParamList()
    {
      return std::vector< TParamAliasKey >();
    }

    virtual const TParamAlias getCFG4DParamAliasList() const
    {
      return TParamAlias();
    }

    virtual void splitCFG4DParamAliasKey( const TParamAliasKey &pk,
                                          std::string &semanticLevel,
                                          std::string &function,
                                          PRV_UINT32 &numParameter ) const
    {
    }

    virtual const TParamAliasKey buildCFG4DParamAliasKey( const std::string &semanticLevel,
                                                          const std::string &function,
                                                          const PRV_UINT32 &numParameter ) const
    {
      return TParamAliasKey();
    }

    virtual Window::TParamAliasKey getCFG4DParamAliasKey( const TParamAlias::iterator it ) const
    {
      return TParamAliasKey();
    }

    virtual const std::string getCFG4DParamAlias( const TParamAliasKey &pk ) const
    {
      return std::string( "" );
    }

    virtual const std::string getCFG4DParamAlias( const TParamAlias::iterator &it ) const
    {
      return std::string( "" );
    }

    virtual std::vector<Window::TParamAliasKey > getCFG4DParamKeysBySemanticLevel( std::string whichSemanticLevel,
                                                                                   const std::vector< Window::TParamAliasKey > &whichParamAlias = std::vector<Window::TParamAliasKey >() ) const
    {
      return std::vector<Window::TParamAliasKey >();
    }

#ifdef WIN32
    virtual void computeSemanticParallel( std::vector< TObjectOrder >& selectedSet,
                                          std::vector< bool >& selected,
                                          TTime timeStep,
                                          PRV_INT32 timePos,
                                          PRV_INT32 objectAxisPos,
                                          std::vector< PRV_INT32 >& objectPosList,
                                          TObjectOrder maxObj,
                                          bool& drawCaution,                      // I/O
                                          std::vector< std::vector< TSemanticValue > >& valuesToDraw, // I/O
                                          std::vector< hash_set< PRV_INT32 > >& eventsToDraw,    // I/O
                                          std::vector< hash_set< commCoord > >& commsToDraw,    // I/O
                                          ProgressController *progress )
#else
    virtual void computeSemanticParallel( std::vector< TObjectOrder >& selectedSet,
                                          std::vector< bool >& selected,
                                          TTime timeStep,
                                          PRV_INT32 timePos,
                                          PRV_INT32 objectAxisPos,
                                          std::vector< PRV_INT32 >& objectPosList,
                                          TObjectOrder maxObj,
                                          bool& drawCaution,                                  // I/O
                                          std::vector< std::vector< TSemanticValue > >& valuesToDraw,             // I/O
                                          std::vector< hash_set< PRV_INT32 > >& eventsToDraw,                // I/O
                                          std::vector< hash_set< commCoord, hashCommCoord > >& commsToDraw,    // I/O
                                          ProgressController *progress )
#endif
    {}

#ifdef WIN32
    virtual void computeSemanticRowParallel( TObjectOrder firstRow,
                                             TObjectOrder lastRow,
                                             std::vector< TObjectOrder >& selectedSet,
                                             std::vector< bool >& selected,
                                             TTime timeStep,
                                             PRV_INT32 timePos,
                                             PRV_INT32 objectAxisPos,
                                             std::vector< PRV_INT32 >& objectPosList,
                                             int& drawCaution,                      // I/O
                                             TSemanticValue &rowComputedMaxY,
                                             TSemanticValue &rowComputedMinY,
                                             int& rowComputedZeros,
                                             std::vector< TSemanticValue >& valuesToDraw, // I/O
                                             hash_set< PRV_INT32 >& eventsToDraw,    // I/O
                                             hash_set< commCoord >& commsToDraw )    // I/O
#else
    virtual void computeSemanticRowParallel( TObjectOrder firstRow,
                                             TObjectOrder lastRow,
                                             std::vector< TObjectOrder >& selectedSet,
                                             std::vector< bool >& selected,
                                             TTime timeStep,
                                             PRV_INT32 timePos,
                                             PRV_INT32 objectAxisPos,
                                             std::vector< PRV_INT32 >& objectPosList,
                                             int& drawCaution,                                  // I/O
                                             TSemanticValue &rowComputedMaxY,
                                             TSemanticValue &rowComputedMinY,
                                             int& rowComputedZeros,
                                             std::vector< TSemanticValue >& valuesToDraw,             // I/O
                                             hash_set< PRV_INT32 >& eventsToDraw,                // I/O
                                             hash_set< commCoord, hashCommCoord >& commsToDraw ) // I/O
#endif
    {}

#ifdef WIN32
    virtual void computeEventsCommsParallel( RecordList *records,
                                             TTime from,
                                             TTime to,
                                             TTime step,
                                             PRV_INT32 timePos,
                                             PRV_INT32 objectAxisPos,
                                             std::vector< bool >& selected,
                                             std::vector< PRV_INT32 >& objectPosList,
                                             hash_set< PRV_INT32 >& eventsToDraw, // I/O
                                             hash_set< commCoord >& commsToDraw ) // I/O
#else
    virtual void computeEventsCommsParallel( RecordList *records,
                                             TTime from,
                                             TTime to,
                                             TTime step,
                                             PRV_INT32 timePos,
                                             PRV_INT32 objectAxisPos,
                                             std::vector< bool >& selected,
                                             std::vector< PRV_INT32 >& objectPosList,
                                             hash_set< PRV_INT32 >& eventsToDraw,                // I/O
                                             hash_set< commCoord, hashCommCoord >& commsToDraw ) // I/O
#endif
  {}

/*******************************************************************************
 EXPERIMENTAL FEATURE FOR PUNCTUAL INFORMATION
*******************************************************************************/

#ifdef WIN32
    virtual void computeSemanticPunctualParallel( std::vector< TObjectOrder >& selectedSet,
                                                  std::vector< bool >& selected,
                                                  TTime timeStep,
                                                  PRV_INT32 timePos,
                                                  PRV_INT32 objectAxisPos,
                                                  std::vector< PRV_INT32 >& objectPosList,
                                                  TObjectOrder maxObj,
                                                  bool& drawCaution,                      // I/O
                                                  std::vector< std::vector< std::vector< std::pair<TSemanticValue,TSemanticValue> > > >& valuesToDraw, // I/O
                                                  std::vector< hash_set< PRV_INT32 > >& eventsToDraw,    // I/O
                                                  std::vector< hash_set< commCoord > >& commsToDraw,    // I/O
                                                  ProgressController *progress )
#else
    virtual void computeSemanticPunctualParallel( std::vector< TObjectOrder >& selectedSet,
                                                  std::vector< bool >& selected,
                                                  TTime timeStep,
                                                  PRV_INT32 timePos,
                                                  PRV_INT32 objectAxisPos,
                                                  std::vector< PRV_INT32 >& objectPosList,
                                                  TObjectOrder maxObj,
                                                  bool& drawCaution,                                  // I/O
                                                  std::vector< std::vector< std::vector< std::pair<TSemanticValue,TSemanticValue> > > >& valuesToDraw,             // I/O
                                                  std::vector< hash_set< PRV_INT32 > >& eventsToDraw,                // I/O
                                                  std::vector< hash_set< commCoord, hashCommCoord > >& commsToDraw,    // I/O
                                                  ProgressController *progress )
#endif
    {}

#ifdef WIN32
    virtual void computeSemanticRowPunctualParallel( TObjectOrder firstRow,
                                                     TObjectOrder lastRow,
                                                     std::vector< TObjectOrder >& selectedSet,
                                                     std::vector< bool >& selected,
                                                     TTime timeStep,
                                                     PRV_INT32 timePos,
                                                     PRV_INT32 objectAxisPos,
                                                     std::vector< PRV_INT32 >& objectPosList,
                                                     int& drawCaution,                      // I/O
                                                     TSemanticValue &rowComputedMaxY,
                                                     TSemanticValue &rowComputedMinY,
                                                     int& rowComputedZeros,
                                                     std::vector< std::vector< std::pair<TSemanticValue,TSemanticValue> > >& valuesToDraw, // I/O
                                                     hash_set< PRV_INT32 >& eventsToDraw,    // I/O
                                                     hash_set< commCoord >& commsToDraw )    // I/O
#else
    virtual void computeSemanticRowPunctualParallel( TObjectOrder firstRow,
                                                     TObjectOrder lastRow,
                                                     std::vector< TObjectOrder >& selectedSet,
                                                     std::vector< bool >& selected,
                                                     TTime timeStep,
                                                     PRV_INT32 timePos,
                                                     PRV_INT32 objectAxisPos,
                                                     std::vector< PRV_INT32 >& objectPosList,
                                                     int& drawCaution,                                  // I/O
                                                     TSemanticValue &rowComputedMaxY,
                                                     TSemanticValue &rowComputedMinY,
                                                     int& rowComputedZeros,
                                                     std::vector< std::vector< std::pair<TSemanticValue,TSemanticValue> > >& valuesToDraw,             // I/O
                                                     hash_set< PRV_INT32 >& eventsToDraw,                // I/O
                                                     hash_set< commCoord, hashCommCoord >& commsToDraw ) // I/O
#endif
    {}

  protected:
    KernelConnection *myKernel;

};


class WindowProxy: public Window
{
  public:
    virtual ~WindowProxy();

    // Specific for WindowProxy because Single and Derived window
    // SingleWindow
    virtual Filter *getFilter() const;

    //DerivedWindow
    virtual void setFactor( PRV_UINT16 whichFactor, TSemanticValue newValue );
    virtual TSemanticValue getFactor( PRV_UINT16 whichFactor ) const;
    virtual void setParent( PRV_UINT16 whichParent, Window *whichWindow );
    virtual void setChild( Window *whichWindow );
    virtual Window *getChild();
    virtual Window *getParent( PRV_UINT16 whichParent ) const;

    virtual Window *clone( bool recursiveClone = false );

    // Other
    virtual void setDestroy( bool newValue );
    virtual bool getDestroy() const;
    virtual void setReady( bool newValue );
    virtual bool getReady() const;

    virtual void setUsedByHistogram( Histogram *whichHisto );
    virtual void unsetUsedByHistogram( Histogram *whichHisto );
    virtual bool getUsedByHistogram() const;
    virtual std::set<Histogram *> getHistograms() const;

    virtual void setWindowBeginTime( TRecordTime whichTime, bool isBroadcast = false );
    virtual void setWindowEndTime( TRecordTime whichTime, bool isBroadcast = false );
    virtual TRecordTime getWindowBeginTime() const;
    virtual TRecordTime getWindowEndTime() const;

    virtual bool getYScaleComputed() const;
    virtual void computeYScaleMin();
    virtual void computeYScaleMax();
    virtual void computeYScale( ProgressController *progress = NULL );
    virtual void setComputeYMaxOnInit( bool newValue );
    virtual bool getComputeYMaxOnInit() const;
    virtual void setMaximumY( TSemanticValue whichMax );
    virtual void setMinimumY( TSemanticValue whichMin );
    virtual TSemanticValue getMaximumY();
    virtual TSemanticValue getMinimumY();
    virtual bool getExistSemanticZero() const;
    virtual bool getShowProgressBar() const;

    //------------------------------------------------------------
    virtual Trace *getTrace() const;
    virtual TWindowLevel getLevel() const;
    virtual void setLevel( TWindowLevel whichLevel );
    virtual bool isLevelProcessModel() const;
    virtual TWindowLevel getMinAcceptableLevel() const;
    virtual void setTimeUnit( TTimeUnit whichUnit );
    virtual TTimeUnit getTimeUnit() const;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const;
    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const std::string& whichFunction );
    virtual std::string getLevelFunction( TWindowLevel whichLevel );
    virtual std::string getFirstUsefulFunction( );
    virtual TWindowLevel getFirstFreeCompose() const;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );
    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const;
    virtual std::string getFunctionParamName( TWindowLevel whichLevel,
                                         TParamIndex whichParam ) const;

    // Extra composes
    virtual void addExtraCompose( TWindowLevel whichLevel );
    virtual void removeExtraCompose( TWindowLevel whichLevel );
    virtual size_t getExtraNumPositions( TWindowLevel whichLevel ) const;
    virtual bool setExtraLevelFunction( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        const std::string& whichFunction );
    virtual std::string getExtraLevelFunction( TWindowLevel whichLevel,
                                               size_t whichPosition );
    virtual void setExtraFunctionParam( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        TParamIndex whichParam,
                                        const TParamValue& newValue );
    virtual TParamIndex getExtraFunctionNumParam( TWindowLevel whichLevel, size_t whichPosition ) const;
    virtual TParamValue getExtraFunctionParam( TWindowLevel whichLevel,
                                               size_t whichPosition,
                                               TParamIndex whichParam ) const;
    virtual std::string getExtraFunctionParamName( TWindowLevel whichLevel,
                                                   size_t whichPosition,
                                                   TParamIndex whichParam ) const;

    virtual RecordList *getRecordList( TObjectOrder whichObject );
    virtual void init( TRecordTime initialTime, TCreateList create, bool updateLimits = true );
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits = true );
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create,
                          TSemanticValue &rowComputedMaxY, TSemanticValue &rowComputedMinY,
                          int& rowComputedZeros,
                          bool updateLimits = true );
    virtual RecordList *calcNext( TObjectOrder whichObject, bool updateLimits = true );
    virtual RecordList *calcNext( TObjectOrder whichObject,
                                  TSemanticValue &rowComputedMaxY, TSemanticValue &rowComputedMinY,
                                  int& rowComputedZeros,
                                  bool updateLimits = true );
    virtual RecordList *calcPrev( TObjectOrder whichObject, bool updateLimits = true );
    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const;
    virtual bool isDerivedWindow() const;
    virtual TObjectOrder cpuObjectToWindowObject( TCPUOrder whichCPU );
    virtual TObjectOrder threadObjectToWindowObject( TThreadOrder whichThread );
    virtual TObjectOrder getWindowLevelObjects() const;
    virtual TRecordTime customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const;
    virtual TRecordTime traceUnitsToCustomUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const;
    virtual TRecordTime traceUnitsToWindowUnits( TRecordTime whichTime ) const;
    virtual TRecordTime windowUnitsToTraceUnits( TRecordTime whichTime ) const;
    virtual SemanticInfoType getSemanticInfoType() const;
    virtual void getAllSemanticFunctions( TSemanticGroup whichGroup,
                                          std::vector<std::string>& onVector ) const;

    virtual Window *getConcrete() const;
    virtual void setName( const std::string& whichName );
    virtual std::string getName() const;
    virtual PRV_UINT16 getPosX() const;
    virtual void setPosX( PRV_UINT16 whichPos );
    virtual PRV_UINT16 getPosY() const;
    virtual void setPosY( PRV_UINT16 whichPos );
    virtual PRV_UINT16 getWidth() const;
    virtual void setWidth( PRV_UINT16 whichPos );
    virtual PRV_UINT16 getHeight() const;
    virtual void setHeight( PRV_UINT16 whichPos );
    virtual void setDrawModeObject( DrawModeMethod method );
    virtual DrawModeMethod getDrawModeObject() const;
    virtual void setDrawModeTime( DrawModeMethod method );
    virtual DrawModeMethod getDrawModeTime() const;
    virtual CodeColor& getCodeColor();
    virtual GradientColor& getGradientColor();
    virtual bool getSemanticScaleMinAtZero();
    virtual bool getShowWindow() const;
    virtual void setShowWindow( bool newValue );
    virtual void setShowChildrenWindow( bool newValue ); // recursively sets children
    virtual bool getRaiseWindow() const;
    virtual void setRaiseWindow( bool newValue );
    virtual void setCodeColorMode();
    virtual void setGradientColorMode();
    virtual void setNotNullGradientColorMode();
    virtual void setFunctionLineColorMode();
    virtual void setFusedLinesColorMode();
    virtual void setPunctualColorMode();
    virtual bool isCodeColorSet() const;
    virtual bool isGradientColorSet() const;
    virtual bool isNotNullGradientColorSet() const;
    virtual bool isFunctionLineColorSet() const;
    virtual bool isFusedLinesColorSet() const;
    virtual bool isPunctualColorSet() const;
    virtual PRV_UINT16 getPixelSize() const;
    virtual void setPixelSize( PRV_UINT16 whichSize );
    virtual Window *getPunctualColorWindow() const;
    virtual void setPunctualColorWindow( Window *whichWindow );
    virtual void setSemanticScaleMinAtZero( bool newValue );

    virtual void allowOutOfScale( bool activate );
    virtual void allowOutliers( bool activate );
    virtual rgb calcColor( TSemanticValue whichValue, Window& whichWindow );
    virtual bool isColorOutlier( rgb whichColor ) const;
    virtual bool getUseCustomPalette() const;
    virtual void setUseCustomPalette( bool newValue );
    virtual bool getChanged() const;
    virtual void setChanged( bool newValue );
    virtual bool getRedraw() const;
    virtual void setRedraw( bool newValue );
    virtual bool getForceRedraw() const;
    virtual void setForceRedraw( bool newValue );
    virtual bool getDrawCommLines() const;
    virtual void setDrawCommLines( bool newValue );
    virtual bool getDrawFlags() const;
    virtual void setDrawFlags( bool newValue );

    virtual bool emptyPrevZoom() const;
    virtual bool emptyNextZoom() const;
    virtual void addZoom( TTime beginTime, TTime endTime,
                          TObjectOrder beginObject, TObjectOrder endObject,
                          bool isBroadCast = false );
    virtual void addZoom( TTime beginTime, TTime endTime, bool isBroadCast = false );
    virtual void addZoom( TObjectOrder beginObject, TObjectOrder endObject );
    virtual void nextZoom();
    virtual void prevZoom();
    virtual void setZoomFirstDimension( std::pair<TTime, TTime> &dim );
    virtual void setZoomSecondDimension( std::pair<TObjectOrder, TObjectOrder>  &dim );
    virtual std::pair<TTime, TTime> getZoomFirstDimension() const;
    virtual std::pair<TObjectOrder, TObjectOrder> getZoomSecondDimension() const;
    virtual std::pair<TObjectOrder, TObjectOrder> getPrevZoomSecondDimension() const;
    virtual std::pair<TObjectOrder, TObjectOrder> getNextZoomSecondDimension() const;

    virtual void addToSyncGroup( TGroupId whichGroup );
    virtual void removeFromSync();
    virtual bool isSync() const;
    virtual TGroupId getSyncGroup() const;

    virtual SelectionManagement< TObjectOrder, TWindowLevel > *getSelectedRows();
    virtual void setSelectedRows( TWindowLevel onLevel, std::vector< bool > &selected );
    virtual void setSelectedRows( TWindowLevel onLevel, std::vector< TObjectOrder > &selected );
    virtual void getSelectedRows( TWindowLevel onLevel, std::vector< bool > &selected, bool lookUpLevels = false );
    virtual void getSelectedRows( TWindowLevel onLevel, std::vector< bool > &selected,
                                  TObjectOrder first, TObjectOrder last, bool lookUpLevels = false );
    virtual void getSelectedRows( TWindowLevel onLevel, std::vector< TObjectOrder > &selected, bool lookUpLevels = false );
    virtual void getSelectedRows( TWindowLevel onLevel, std::vector< TObjectOrder > &selected,
                                  TObjectOrder first, TObjectOrder last, bool lookUpLevels = false );
    virtual TObjectOrder shiftFirst( TObjectOrder whichFirst, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, TWindowLevel level ) const;
    virtual TObjectOrder shiftLast( TObjectOrder whichLast, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, TWindowLevel level ) const;
    virtual bool hasLevelSomeSelectedObject( TWindowLevel onLevel );

    virtual void getGroupLabels( PRV_UINT32 whichGroup, std::vector<std::string>& onVector ) const;
    virtual bool getParametersOfFunction( std::string whichFunction,
                                          PRV_UINT32 &numParameters,
                                          std::vector<std::string> &nameParameters,
                                          std::vector< std::vector< double > >&defaultParameters ) const;

    virtual void setObjectLabels( Window::TObjectLabels whichLabels );
    virtual Window::TObjectLabels getObjectLabels() const;

    virtual void setObjectAxisSize( Window::TObjectAxisSize whichSize );
    virtual Window::TObjectAxisSize getObjectAxisSize() const;

    // CFG4D
    virtual void setCFG4DEnabled( bool enabled );
    virtual bool getCFG4DEnabled() const;

    // If CFG4D is enabled, mode can be changed:
    //   false => no CFG4D replacement
    //   true  => CFG4D replacement
    virtual void setCFG4DMode( bool mode );
    virtual bool getCFG4DMode() const;

    virtual bool existsCFG4DAlias( const std::string &property ) const; // DEPRECATED
    virtual bool existsCFG4DAlias( const TSingleTimelineProperties &propertyIndex ) const;
    virtual bool existsCFG4DAlias( const TDerivedTimelineProperties &propertyIndex ) const;

    virtual std::string getCFG4DAlias( const std::string &property ) const; // DEPRECATED
    virtual std::string getCFG4DAlias( const TSingleTimelineProperties &propertyIndex ) const;
    virtual std::string getCFG4DAlias( const TDerivedTimelineProperties &propertyIndex ) const;

    virtual void setCFG4DAlias( const std::string &property, const std::string &alias );

    virtual void setCFG4DAliasList( const std::map< std::string, std::string >& aliasList );
    virtual const std::map< std::string, std::string > getCFG4DAliasList() const;

    virtual const std::vector< std::string > getCFG4DFullTagList();

    // Returns the keys ( semantic level, function, num parameter ) of the parameters
    //   of the current selected functions in the visible levels.
    virtual const std::vector< TParamAliasKey > getCFG4DCurrentSelectedFullParamList();
    virtual void setCFG4DParamAlias( const TParamAlias &whichParamAlias );
    virtual void setCFG4DParamAlias( std::string semanticLevel,
                                     std::string function,
                                     PRV_UINT32 numParameter,
                                     std::string paramAlias );
    virtual const TParamAlias getCFG4DParamAliasList() const;
    virtual void splitCFG4DParamAliasKey( const TParamAliasKey &pk,
                                          std::string &semanticLevel,
                                          std::string &function,
                                          PRV_UINT32 &numParameter ) const;

    virtual Window::TParamAliasKey getCFG4DParamAliasKey( const TParamAlias::iterator it ) const;
    virtual const std::string getCFG4DParamAlias( const TParamAlias::iterator &it ) const;
    virtual const std::string getCFG4DParamAlias( const TParamAliasKey &pk ) const;
    virtual std::vector< Window::TParamAliasKey > getCFG4DParamKeysBySemanticLevel(
                    std::string whichSemanticLevel,
                    const std::vector< Window::TParamAliasKey > &whichParamAlias = std::vector<Window::TParamAliasKey >()  ) const;
    virtual const Window::TParamAliasKey buildCFG4DParamAliasKey(
                                                const std::string &semanticLevel,
                                                const std::string &function,
                                                const PRV_UINT32 &numParameter ) const;


#ifdef WIN32
    virtual void computeSemanticParallel( std::vector< TObjectOrder >& selectedSet,
                                          std::vector< bool >& selected,
                                          TTime timeStep,
                                          PRV_INT32 timePos,
                                          PRV_INT32 objectAxisPos,
                                          std::vector< PRV_INT32 >& objectPosList,
                                          TObjectOrder maxObj,
                                          bool& drawCaution,                      // O
                                          std::vector< std::vector< TSemanticValue > >& valuesToDraw, // O
                                          std::vector< hash_set< PRV_INT32 > >& eventsToDraw,    // O
                                          std::vector< hash_set< commCoord > >& commsToDraw,    // O
                                          ProgressController *progress );

    virtual void computeSemanticRowParallel( int numRows,
                                             TObjectOrder firstRow,
                                             TObjectOrder lastRow,
                                             std::vector< TObjectOrder >& selectedSet,
                                             std::vector< bool >& selected,
                                             TTime timeStep,
                                             PRV_INT32 timePos,
                                             PRV_INT32 objectAxisPos,
                                             std::vector< PRV_INT32 >& objectPosList,
                                             int& drawCaution,  // O
                                             TSemanticValue &rowComputedMaxY,
                                             TSemanticValue &rowComputedMinY,
                                             int& rowComputedZeros,
                                             std::vector< TSemanticValue >& valuesToDraw, // O
                                             hash_set< PRV_INT32 >& eventsToDraw,    // O
                                             hash_set< commCoord >& commsToDraw,
                                             ProgressController *progress );   // O

    virtual void computeEventsCommsParallel( RecordList *records,
                                             TTime from,
                                             TTime to,
                                             TTime step,
                                             PRV_INT32 timePos,
                                             PRV_INT32 objectAxisPos,
                                             std::vector< bool >& selected,
                                             std::vector< PRV_INT32 >& objectPosList,
                                             hash_set< PRV_INT32 >& eventsToDraw,  // I/O
                                             hash_set< commCoord >& commsToDraw ); // I/O
#else
    virtual void computeSemanticParallel( std::vector< TObjectOrder >& selectedSet,
                                          std::vector< bool >& selected,
                                          TTime timeStep,
                                          PRV_INT32 timePos,
                                          PRV_INT32 objectAxisPos,
                                          std::vector< PRV_INT32 >& objectPosList,
                                          TObjectOrder maxObj,
                                          bool& drawCaution,                                                  // I/O
                                          std::vector< std::vector< TSemanticValue > >& valuesToDraw,         // I/O
                                          std::vector< hash_set< PRV_INT32 > >& eventsToDraw,                 // I/O
                                          std::vector< hash_set< commCoord, hashCommCoord > >& commsToDraw,   // I/O
                                          ProgressController *progress );

    void computeSemanticRowParallel( int numRows,
                                     TObjectOrder firstRow,
                                     TObjectOrder lastRow,
                                     std::vector< TObjectOrder >& selectedSet,
                                     std::vector< bool >& selected,
                                     TTime timeStep,
                                     PRV_INT32 timePos,
                                     PRV_INT32 objectAxisPos,
                                     std::vector< PRV_INT32 >& objectPosList,
                                     int& drawCaution,                                    // I/O
                                     TSemanticValue& rowComputedMaxY,
                                     TSemanticValue& rowComputedMinY,
                                     int& rowComputedZeros,
                                     std::vector< TSemanticValue >& valuesToDraw,         // I/O
                                     hash_set< PRV_INT32 >& eventsToDraw,                 // I/O
                                     hash_set< commCoord, hashCommCoord >& commsToDraw,
                                     ProgressController *progress ); // I/O

    virtual void computeEventsCommsParallel( RecordList *records,
                                             TTime from,
                                             TTime to,
                                             TTime step,
                                             PRV_INT32 timePos,
                                             PRV_INT32 objectAxisPos,
                                             std::vector< bool >& selected,
                                             std::vector< PRV_INT32 >& objectPosList,
                                             hash_set< PRV_INT32 >& eventsToDraw,                 // I/O
                                             hash_set< commCoord, hashCommCoord >& commsToDraw ); // I/O
#endif // WIN32

#ifdef WIN32
    virtual void computeSemanticPunctualParallel( std::vector< TObjectOrder >& selectedSet,
                                                  std::vector< bool >& selected,
                                                  TTime timeStep,
                                                  PRV_INT32 timePos,
                                                  PRV_INT32 objectAxisPos,
                                                  std::vector< PRV_INT32 >& objectPosList,
                                                  TObjectOrder maxObj,
                                                  bool& drawCaution,                      // I/O
                                                  std::vector< std::vector< std::vector< std::pair<TSemanticValue,TSemanticValue> > > >& valuesToDraw, // I/O
                                                  std::vector< hash_set< PRV_INT32 > >& eventsToDraw,    // I/O
                                                  std::vector< hash_set< commCoord > >& commsToDraw,    // I/O
                                                  ProgressController *progress );
#else
    virtual void computeSemanticPunctualParallel( std::vector< TObjectOrder >& selectedSet,
                                                  std::vector< bool >& selected,
                                                  TTime timeStep,
                                                  PRV_INT32 timePos,
                                                  PRV_INT32 objectAxisPos,
                                                  std::vector< PRV_INT32 >& objectPosList,
                                                  TObjectOrder maxObj,
                                                  bool& drawCaution,                                  // I/O
                                                  std::vector< std::vector< std::vector< std::pair<TSemanticValue,TSemanticValue> > > >& valuesToDraw,             // I/O
                                                  std::vector< hash_set< PRV_INT32 > >& eventsToDraw,                // I/O
                                                  std::vector< hash_set< commCoord, hashCommCoord > >& commsToDraw,    // I/O
                                                  ProgressController *progress );
#endif

#ifdef WIN32
    virtual void computeSemanticRowPunctualParallel( int numRows,
                                                     TObjectOrder firstRow,
                                                     TObjectOrder lastRow,
                                                     std::vector< TObjectOrder >& selectedSet,
                                                     std::vector< bool >& selected,
                                                     TTime timeStep,
                                                     PRV_INT32 timePos,
                                                     PRV_INT32 objectAxisPos,
                                                     std::vector< PRV_INT32 >& objectPosList,
                                                     int& drawCaution,  // O
                                                     TSemanticValue& rowComputedMaxY,
                                                     TSemanticValue& rowComputedMinY,
                                                     int& rowComputedZeros,
                                                     std::vector< std::vector< std::pair<TSemanticValue,TSemanticValue> > >& valuesToDraw, // O
                                                     hash_set< PRV_INT32 >& eventsToDraw,    // O
                                                     hash_set< commCoord >& commsToDraw,
                                                     ProgressController *progress );   // O
#else
    void computeSemanticRowPunctualParallel( int numRows,
                                             TObjectOrder firstRow,
                                             TObjectOrder lastRow,
                                             std::vector< TObjectOrder >& selectedSet,
                                             std::vector< bool >& selected,
                                             TTime timeStep,
                                             PRV_INT32 timePos,
                                             PRV_INT32 objectAxisPos,
                                             std::vector< PRV_INT32 >& objectPosList,
                                             int& drawCaution,                                    // I/O
                                             TSemanticValue& rowComputedMaxY,
                                             TSemanticValue& rowComputedMinY,
                                             int& rowComputedZeros,
                                             std::vector< std::vector< std::pair<TSemanticValue,TSemanticValue> > >& valuesToDraw,         // I/O
                                             hash_set< PRV_INT32 >& eventsToDraw,                 // I/O
                                             hash_set< commCoord, hashCommCoord >& commsToDraw,
                                             ProgressController *progress ); // I/O
#endif

  private:
    Window *myWindow;
#ifdef PARALLEL_ENABLED
    vector<Window *> parallelClone;
#endif // PARALLEL_ENABLED

    Trace *myTrace;
    Filter *myFilter;

    bool destroy;
    bool ready;

    PRV_UINT16 posX;
    PRV_UINT16 posY;
    PRV_UINT16 width;
    PRV_UINT16 height;

    TRecordTime winBeginTime;
    TRecordTime winEndTime;

    bool computeYMaxOnInit;
    bool yScaleComputed;
    TSemanticValue maximumY;
    TSemanticValue minimumY;
    bool existSemanticZero;
    TSemanticValue computedMaxY;
    TSemanticValue computedMinY;
    bool computedZeros;

    std::vector<RecordList *> myLists;

    // Must store the associated proxies
    Window *parent1;
    Window *parent2;
    Window *child;

    std::set<Histogram *> usedByHistogram;

    // GUI related attributes
    std::string name;
    CodeColor myCodeColor;
    GradientColor myGradientColor;
    bool useCustomPalette;
    DrawModeMethod drawModeObject;
    DrawModeMethod drawModeTime;
    SemanticColor::TColorFunction colorMode;
    bool semanticScaleMinAtZero;
    bool showWindow;
    bool raise;
    bool changed;
    bool redraw;
    bool forceRedraw;
    bool commLines;
    bool flags;
    PRV_UINT16 pixelSize;

    Window *punctualColorWindow;
    // Zoom history
    ZoomHistory<TTime, TObjectOrder> zoomHistory;

    // Synchronize
    bool sync;
    TGroupId syncGroup;

    // Row selection
    SelectionManagement< TObjectOrder, TWindowLevel > selectedRow;

    Window::TObjectLabels objectLabels;
    Window::TObjectAxisSize objectAxisSize;

    // CFG4D
    bool isCFG4DEnabled;
    bool CFG4DMode;
    std::map< std::string, std::string > propertiesAliasCFG4D;
    TParamAlias paramAliasCFG4D;

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

    void getAllLevelsSelectedRows( TWindowLevel onLevel, std::vector< TObjectOrder > &selected );

    friend Window *Window::create( KernelConnection *, Trace * );
    friend Window *Window::create( KernelConnection * );
    friend Window *Window::create( KernelConnection *, Window *, Window * );
};

#endif // WINDOW_H_INCLUDED
