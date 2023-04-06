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


#pragma once


#include <string>
#include <map>

#include "paraverkerneltypes.h"
#include "semanticcolor.h"
#include "drawmode.h"
#include "zoomhistory.h"
#include "selectionmanagement.h"
#include "paraverlabels.h"
#include "syncwindows.h"
#include "cfgs4d.h"

#ifdef _MSC_VER
#include <hash_set>
#else
#include  <unordered_set>
#endif

#ifdef _MSC_VER
using namespace stdext;
#endif

constexpr char PARAM_SEPARATOR = '|';

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

#ifdef _MSC_VER
  bool operator<( const commCoord& b ) const
  {
    return true;
  }

  size_t hash() const {
    return ( ( fromTime + toTime + toRow ) * 100 ) + recType;
  }
#endif
};

#ifdef _MSC_VER
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

#ifndef _MSC_VER
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

enum class TObjectLabels
{
  ALL_LABELS = 0,
  SPACED_LABELS,
  POWER2_LABELS
};

enum class TObjectAxisSize
{
  CURRENT_LEVEL = 0,
  ALL_LEVELS,
  ZERO_PERC,
  FIVE_PERC,
  TEN_PERC,
  TWENTYFIVE_PERC
};

class Timeline
{
  public:

    // Create Single Timeline
    static Timeline *create( KernelConnection *whichKernel, Trace *whichTrace );
    //Create Derived Timeline
    static Timeline *create( KernelConnection *whichKernel );
    static Timeline *create( KernelConnection *whichKernel, Timeline *whichParent1,
                           Timeline *whichParent2 );

    static bool compatibleLevels( Timeline *window1, Timeline *window2 );

    Timeline() {}
    Timeline( KernelConnection *whichKernel );
    virtual ~Timeline() {}

    KernelConnection *getKernel() const
    {
      return myKernel;
    }

    // Specific for WindowProxy because Single and Derived window
    // SingleWindow
    virtual Filter *getFilter() const
    {
      return nullptr;
    }

    //DerivedWindow
    virtual void setFactor( PRV_UINT16 whichFactor, TSemanticValue newValue ) {}
    virtual TSemanticValue getFactor( PRV_UINT16 whichFactor ) const
    {
      return 1.0;
    }
    virtual void setShift( PRV_UINT16 whichShift, TSemanticValue newValue ) {}
    virtual PRV_INT16 getShift( PRV_UINT16 whichShift ) const
    {
      return 0;
    }
    virtual void setParent( PRV_UINT16 whichParent, Timeline *whichWindow ) {}
    virtual void setChild( Timeline *whichWindow ) {}
    virtual Timeline *getChild()
    {
      return nullptr;
    }
    virtual Timeline *getParent( PRV_UINT16 whichParent ) const
    {
      return nullptr;
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
    virtual void computeYScale( ProgressController *progress = nullptr ) {}
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

    virtual Timeline* clone( bool recursiveClone = false )
    {
      return nullptr;
    }

    virtual bool getShowProgressBar() const
    {
      return true;
    }

    //------------------------------------------------------------
    virtual Trace *getTrace() const = 0;
    virtual TTraceLevel getLevel() const = 0;
    virtual void setLevel( TTraceLevel whichLevel ) = 0;
    virtual TTraceLevel getMinAcceptableLevel() const = 0;
    virtual void setTimeUnit( TTimeUnit whichUnit ) = 0;
    virtual TTimeUnit getTimeUnit() const = 0;
    virtual TWindowLevel getComposeLevel( TTraceLevel whichLevel ) const = 0;
    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const std::string& whichFunction ) = 0;
    virtual std::string getLevelFunction( TWindowLevel whichLevel ) const = 0;
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
    { return nullptr; }
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
    virtual Timeline *getConcrete() const
    {
      return nullptr;
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
      return DrawModeMethod::DRAW_MAXIMUM;
    }
    virtual void setDrawModeTime( DrawModeMethod method )
    {}
    virtual DrawModeMethod getDrawModeTime() const
    {
      return DrawModeMethod::DRAW_MAXIMUM;
    }
    virtual CodeColor& getCodeColor()
    {
      CodeColor *tmp = nullptr;
      return *tmp;
    }
    virtual GradientColor& getGradientColor()
    {
      GradientColor *tmp = nullptr;
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
    virtual Timeline *getPunctualColorWindow() const
    {
      return nullptr;
    }
    virtual void setPunctualColorWindow( Timeline *whichWindow )
    {}
    virtual void allowOutOfScale( bool activate )
    {}
    virtual void allowOutliers( bool activate )
    {}
    virtual rgb calcColor( TSemanticValue whichValue, Timeline& whichWindow )
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


    virtual SelectionManagement< TObjectOrder, TTraceLevel > *getSelectedRows()
    {
      return nullptr;
    }
    virtual void setSelectedRows( TTraceLevel onLevel, std::vector< bool > &selected )
    {}
    virtual void setSelectedRows( TTraceLevel onLevel, std::vector< TObjectOrder > &selection )
    {}
    virtual void getSelectedRows( TTraceLevel onLevel, std::vector< bool > &selected, bool lookUpLevels = false )
    {}
    virtual void getSelectedRows( TTraceLevel onLevel, std::vector< bool > &selected,
                                  TObjectOrder first, TObjectOrder last, bool lookUpLevels = false )
    {}
    virtual void getSelectedRows( TTraceLevel onLevel, std::vector< TObjectOrder > &selection, bool lookUpLevels = false )
    {}
    virtual void getSelectedRows( TTraceLevel onLevel,
                                  std::vector< TObjectOrder > &selection,
                                  TObjectOrder first, TObjectOrder last, bool lookUpLevels = false )
    {}
    virtual TObjectOrder shiftFirst( TObjectOrder whichFirst, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, TTraceLevel level ) const
    {
      return 0;
    }
    virtual TObjectOrder shiftLast( TObjectOrder whichLast, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, TTraceLevel level ) const
    {
      return 0;
    }

    virtual bool hasLevelSomeSelectedObject( TTraceLevel onLevel )
    {
      return true;
    }

    virtual void getGroupLabels( PRV_UINT32 whichGroup, std::vector<std::string>& onVector ) const = 0;
    virtual bool getParametersOfFunction( std::string whichFunction,
                                          PRV_UINT32 &numParameters,
                                          std::vector<std::string> &nameParameters,
                                          std::vector< std::vector< double > >&defaultParameters ) const = 0;

    virtual void setObjectLabels( TObjectLabels whichLabels )
    {}
    virtual TObjectLabels getObjectLabels() const
    {
      return TObjectLabels::SPACED_LABELS;
    }

    virtual void setObjectAxisSize( TObjectAxisSize whichSize )
    {}
    virtual TObjectAxisSize getObjectAxisSize() const
    {
      return TObjectAxisSize::CURRENT_LEVEL;
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

    typedef std::pair< std::pair< std::string, std::string >, TParamIndex > TParamAliasKey;
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
                                          TParamIndex &numParameter ) const
    {
    }

    virtual void splitCFG4DParamAliasKey( const TParamAliasKey &pk,
                                          TWindowLevel &semanticLevel,
                                          std::string &function,
                                          TParamIndex &numParameter ) const
    {      
    }

    virtual const TParamAliasKey buildCFG4DParamAliasKey( const std::string &semanticLevel,
                                                          const std::string &function,
                                                          const TParamIndex &numParameter ) const
    {
      return TParamAliasKey();
    }

    virtual std::string getCFG4DParameterOriginalName( TWindowLevel whichLevel, TParamIndex whichParam ) const
    {
      return "";
    }


    virtual Timeline::TParamAliasKey getCFG4DParamAliasKey( const TParamAlias::iterator it ) const
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

    virtual std::vector<Timeline::TParamAliasKey > getCFG4DParamKeysBySemanticLevel( std::string whichSemanticLevel,
                                                                                   const std::vector< Timeline::TParamAliasKey > &whichParamAlias = std::vector<Timeline::TParamAliasKey >() ) const
    {
      return std::vector<Timeline::TParamAliasKey >();
    }

    virtual void setCFGS4DGroupLink( std::string originalName, TCFGS4DGroup whichGroup )
    {}

    virtual TCFGS4DGroup getCFGS4DGroupLink( std::string originalName ) const
    {
      return NO_GROUP_LINK;
    }

    virtual void setCFGS4DIndexLink( TCFGS4DIndexLink whichIndex )
    {}

    virtual TCFGS4DIndexLink getCFGS4DIndexLink() const
    {
      return NO_INDEX_LINK;
    }

#ifdef _MSC_VER
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
                                          std::vector< std::unordered_set< PRV_INT32 > >& eventsToDraw,                // I/O
                                          std::vector< std::unordered_set< commCoord, hashCommCoord > >& commsToDraw,    // I/O
                                          ProgressController *progress )
#endif
    {}

/*******************************************************************************
 EXPERIMENTAL FEATURE FOR PUNCTUAL INFORMATION
*******************************************************************************/

#ifdef _MSC_VER
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
                                                  std::vector< std::unordered_set< PRV_INT32 > >& eventsToDraw,                // I/O
                                                  std::vector< std::unordered_set< commCoord, hashCommCoord > >& commsToDraw,    // I/O
                                                  ProgressController *progress )
#endif
    {}

  protected:
    KernelConnection *myKernel;

};


class TimelineProxy: public Timeline
{
  public:
    virtual ~TimelineProxy();

    // Specific for WindowProxy because Single and Derived window
    // SingleWindow
    virtual Filter *getFilter() const override;

    //DerivedWindow
    virtual void setFactor( PRV_UINT16 whichFactor, TSemanticValue newValue ) override;
    virtual TSemanticValue getFactor( PRV_UINT16 whichFactor ) const override;
    virtual void setShift( PRV_UINT16 whichShift, TSemanticValue newValue ) override;
    virtual PRV_INT16 getShift( PRV_UINT16 whichShift ) const override;
    virtual void setParent( PRV_UINT16 whichParent, Timeline *whichWindow ) override;
    virtual void setChild( Timeline *whichWindow ) override;
    virtual Timeline *getChild() override;
    virtual Timeline *getParent( PRV_UINT16 whichParent ) const override;

    virtual Timeline *clone( bool recursiveClone = false ) override;

    // Other
    virtual void setDestroy( bool newValue ) override;
    virtual bool getDestroy() const override;
    virtual void setReady( bool newValue ) override;
    virtual bool getReady() const override;

    virtual void setUsedByHistogram( Histogram *whichHisto ) override;
    virtual void unsetUsedByHistogram( Histogram *whichHisto ) override;
    virtual bool getUsedByHistogram() const override;
    virtual std::set<Histogram *> getHistograms() const override;

    virtual void setWindowBeginTime( TRecordTime whichTime, bool isBroadcast = false ) override;
    virtual void setWindowEndTime( TRecordTime whichTime, bool isBroadcast = false ) override;
    virtual TRecordTime getWindowBeginTime() const override;
    virtual TRecordTime getWindowEndTime() const override;

    virtual bool getYScaleComputed() const override;
    virtual void computeYScaleMin() override;
    virtual void computeYScaleMax() override;
    virtual void computeYScale( ProgressController *progress = nullptr ) override;
    virtual void setComputeYMaxOnInit( bool newValue ) override;
    virtual bool getComputeYMaxOnInit() const override;
    virtual void setMaximumY( TSemanticValue whichMax ) override;
    virtual void setMinimumY( TSemanticValue whichMin ) override;
    virtual TSemanticValue getMaximumY() override;
    virtual TSemanticValue getMinimumY() override;
    virtual bool getExistSemanticZero() const override;
    virtual bool getShowProgressBar() const override;

    //------------------------------------------------------------
    virtual Trace *getTrace() const override;
    virtual TTraceLevel getLevel() const override;
    virtual void setLevel( TTraceLevel whichLevel ) override;
    virtual bool isLevelProcessModel() const override;
    virtual TTraceLevel getMinAcceptableLevel() const override;
    virtual void setTimeUnit( TTimeUnit whichUnit ) override;
    virtual TTimeUnit getTimeUnit() const override;
    virtual TWindowLevel getComposeLevel( TTraceLevel whichLevel ) const override;
    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const std::string& whichFunction ) override;
    virtual std::string getLevelFunction( TWindowLevel whichLevel ) const override;
    virtual std::string getFirstUsefulFunction( ) override;
    virtual TWindowLevel getFirstFreeCompose() const override;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) override;
    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const override;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const override;
    virtual std::string getFunctionParamName( TWindowLevel whichLevel,
                                         TParamIndex whichParam ) const override;

    // Extra composes
    virtual void addExtraCompose( TWindowLevel whichLevel ) override; 
    virtual void removeExtraCompose( TWindowLevel whichLevel ) override;
    virtual size_t getExtraNumPositions( TWindowLevel whichLevel ) const override;
    virtual bool setExtraLevelFunction( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        const std::string& whichFunction ) override;
    virtual std::string getExtraLevelFunction( TWindowLevel whichLevel,
                                               size_t whichPosition ) override;
    virtual void setExtraFunctionParam( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        TParamIndex whichParam,
                                        const TParamValue& newValue ) override;
    virtual TParamIndex getExtraFunctionNumParam( TWindowLevel whichLevel, size_t whichPosition ) const override;
    virtual TParamValue getExtraFunctionParam( TWindowLevel whichLevel,
                                               size_t whichPosition,
                                               TParamIndex whichParam ) const override;
    virtual std::string getExtraFunctionParamName( TWindowLevel whichLevel,
                                                   size_t whichPosition,
                                                   TParamIndex whichParam ) const override;

    virtual RecordList *getRecordList( TObjectOrder whichObject ) override;
    virtual void init( TRecordTime initialTime, TCreateList create, bool updateLimits = true ) override;
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits = true ) override;
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create,
                          TSemanticValue &rowComputedMaxY, TSemanticValue &rowComputedMinY,
                          int& rowComputedZeros,
                          bool updateLimits = true ) override;
    virtual RecordList *calcNext( TObjectOrder whichObject, bool updateLimits = true ) override;
    virtual RecordList *calcNext( TObjectOrder whichObject,
                                  TSemanticValue &rowComputedMaxY, TSemanticValue &rowComputedMinY,
                                  int& rowComputedZeros,
                                  bool updateLimits = true ) override;
    virtual RecordList *calcPrev( TObjectOrder whichObject, bool updateLimits = true ) override;
    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const override;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const override;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const override;
    virtual bool isDerivedWindow() const override;
    virtual TObjectOrder cpuObjectToWindowObject( TCPUOrder whichCPU ) override;
    virtual TObjectOrder threadObjectToWindowObject( TThreadOrder whichThread ) override;
    virtual TObjectOrder getWindowLevelObjects() const override;
    virtual TRecordTime customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const override;
    virtual TRecordTime traceUnitsToCustomUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const override;
    virtual TRecordTime traceUnitsToWindowUnits( TRecordTime whichTime ) const override;
    virtual TRecordTime windowUnitsToTraceUnits( TRecordTime whichTime ) const override;
    virtual SemanticInfoType getSemanticInfoType() const override;
    virtual void getAllSemanticFunctions( TSemanticGroup whichGroup,
                                          std::vector<std::string>& onVector ) const override;

    virtual Timeline *getConcrete() const override;
    virtual void setName( const std::string& whichName ) override;
    virtual std::string getName() const override;
    virtual PRV_UINT16 getPosX() const override;
    virtual void setPosX( PRV_UINT16 whichPos ) override;
    virtual PRV_UINT16 getPosY() const override;
    virtual void setPosY( PRV_UINT16 whichPos ) override;
    virtual PRV_UINT16 getWidth() const override;
    virtual void setWidth( PRV_UINT16 whichPos ) override;
    virtual PRV_UINT16 getHeight() const override;
    virtual void setHeight( PRV_UINT16 whichPos ) override;
    virtual void setDrawModeObject( DrawModeMethod method ) override;
    virtual DrawModeMethod getDrawModeObject() const override;
    virtual void setDrawModeTime( DrawModeMethod method ) override;
    virtual DrawModeMethod getDrawModeTime() const override;
    virtual CodeColor& getCodeColor() override;
    virtual GradientColor& getGradientColor() override;
    virtual bool getSemanticScaleMinAtZero() override;
    virtual bool getShowWindow() const override;
    virtual void setShowWindow( bool newValue ) override;
    virtual void setShowChildrenWindow( bool newValue ) override; // recursively sets children
    virtual bool getRaiseWindow() const override;
    virtual void setRaiseWindow( bool newValue ) override;
    virtual void setCodeColorMode() override;
    virtual void setGradientColorMode() override;
    virtual void setNotNullGradientColorMode() override;
    virtual void setFunctionLineColorMode() override;
    virtual void setFusedLinesColorMode() override;
    virtual void setPunctualColorMode() override;
    virtual bool isCodeColorSet() const override;
    virtual bool isGradientColorSet() const override;
    virtual bool isNotNullGradientColorSet() const override;
    virtual bool isFunctionLineColorSet() const override;
    virtual bool isFusedLinesColorSet() const override;
    virtual bool isPunctualColorSet() const override;
    virtual PRV_UINT16 getPixelSize() const override;
    virtual void setPixelSize( PRV_UINT16 whichSize ) override;
    virtual Timeline *getPunctualColorWindow() const override;
    virtual void setPunctualColorWindow( Timeline *whichWindow ) override;
    virtual void setSemanticScaleMinAtZero( bool newValue ) override;

    virtual void allowOutOfScale( bool activate ) override;
    virtual void allowOutliers( bool activate ) override;
    virtual rgb calcColor( TSemanticValue whichValue, Timeline& whichWindow ) override;
    virtual bool isColorOutlier( rgb whichColor ) const override;
    virtual bool getUseCustomPalette() const override;
    virtual void setUseCustomPalette( bool newValue ) override;
    virtual bool getChanged() const override;
    virtual void setChanged( bool newValue ) override;
    virtual bool getRedraw() const override;
    virtual void setRedraw( bool newValue ) override;
    virtual bool getForceRedraw() const override;
    virtual void setForceRedraw( bool newValue ) override;
    virtual bool getDrawCommLines() const override;
    virtual void setDrawCommLines( bool newValue ) override;
    virtual bool getDrawFlags() const override;
    virtual void setDrawFlags( bool newValue ) override;

    virtual bool emptyPrevZoom() const override;
    virtual bool emptyNextZoom() const override;
    virtual void addZoom( TTime beginTime, TTime endTime,
                          TObjectOrder beginObject, TObjectOrder endObject,
                          bool isBroadCast = false ) override;
    virtual void addZoom( TTime beginTime, TTime endTime, bool isBroadCast = false ) override;
    virtual void addZoom( TObjectOrder beginObject, TObjectOrder endObject ) override;
    virtual void nextZoom() override;
    virtual void prevZoom() override;
    virtual void setZoomFirstDimension( std::pair<TTime, TTime> &dim ) override;
    virtual void setZoomSecondDimension( std::pair<TObjectOrder, TObjectOrder>  &dim ) override;
    virtual std::pair<TTime, TTime> getZoomFirstDimension() const override;
    virtual std::pair<TObjectOrder, TObjectOrder> getZoomSecondDimension() const override;
    virtual std::pair<TObjectOrder, TObjectOrder> getPrevZoomSecondDimension() const override;
    virtual std::pair<TObjectOrder, TObjectOrder> getNextZoomSecondDimension() const override;

    virtual void addToSyncGroup( TGroupId whichGroup ) override;
    virtual void removeFromSync() override;
    virtual bool isSync() const override;
    virtual TGroupId getSyncGroup() const override;

    virtual SelectionManagement< TObjectOrder, TTraceLevel > *getSelectedRows() override;
    virtual void setSelectedRows( TTraceLevel onLevel, std::vector< bool > &selected ) override;
    virtual void setSelectedRows( TTraceLevel onLevel, std::vector< TObjectOrder > &selected ) override;
    virtual void getSelectedRows( TTraceLevel onLevel, std::vector< bool > &selected, bool lookUpLevels = false ) override;
    virtual void getSelectedRows( TTraceLevel onLevel, std::vector< bool > &selected,
                                  TObjectOrder first, TObjectOrder last, bool lookUpLevels = false ) override;
    virtual void getSelectedRows( TTraceLevel onLevel, std::vector< TObjectOrder > &selected, bool lookUpLevels = false ) override;
    virtual void getSelectedRows( TTraceLevel onLevel, std::vector< TObjectOrder > &selected,
                                  TObjectOrder first, TObjectOrder last, bool lookUpLevels = false ) override;
    virtual TObjectOrder shiftFirst( TObjectOrder whichFirst, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, TTraceLevel level ) const override;
    virtual TObjectOrder shiftLast( TObjectOrder whichLast, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, TTraceLevel level ) const override;
    virtual bool hasLevelSomeSelectedObject( TTraceLevel onLevel ) override;

    virtual void getGroupLabels( PRV_UINT32 whichGroup, std::vector<std::string>& onVector ) const override;
    virtual bool getParametersOfFunction( std::string whichFunction,
                                          PRV_UINT32 &numParameters,
                                          std::vector<std::string> &nameParameters,
                                          std::vector< std::vector< double > >&defaultParameters ) const override;

    virtual void setObjectLabels( TObjectLabels whichLabels ) override;
    virtual TObjectLabels getObjectLabels() const override;

    virtual void setObjectAxisSize( TObjectAxisSize whichSize ) override;
    virtual TObjectAxisSize getObjectAxisSize() const override;

    // CFG4D
    virtual void setCFG4DEnabled( bool enabled ) override;
    virtual bool getCFG4DEnabled() const override;

    // If CFG4D is enabled, mode can be changed:
    //   false => no CFG4D replacement
    //   true  => CFG4D replacement
    virtual void setCFG4DMode( bool mode ) override;
    virtual bool getCFG4DMode() const override;

    virtual bool existsCFG4DAlias( const std::string &property ) const override; // DEPRECATED
    virtual bool existsCFG4DAlias( const TSingleTimelineProperties &propertyIndex ) const override;
    virtual bool existsCFG4DAlias( const TDerivedTimelineProperties &propertyIndex ) const override;

    virtual std::string getCFG4DAlias( const std::string &property ) const override; // DEPRECATED
    virtual std::string getCFG4DAlias( const TSingleTimelineProperties &propertyIndex ) const override;
    virtual std::string getCFG4DAlias( const TDerivedTimelineProperties &propertyIndex ) const override;

    virtual void setCFG4DAlias( const std::string &property, const std::string &alias ) override;

    virtual void setCFG4DAliasList( const std::map< std::string, std::string >& aliasList ) override;
    virtual const std::map< std::string, std::string > getCFG4DAliasList() const override;

    virtual const std::vector< std::string > getCFG4DFullTagList() override;

    // Returns the keys ( semantic level, function, num parameter ) of the parameters
    //   of the current selected functions in the visible levels.
    virtual const std::vector< TParamAliasKey > getCFG4DCurrentSelectedFullParamList() override;
    virtual void setCFG4DParamAlias( const TParamAlias &whichParamAlias ) override;
    virtual void setCFG4DParamAlias( std::string semanticLevel,
                                     std::string function,
                                     PRV_UINT32 numParameter,
                                     std::string paramAlias ) override;
    virtual const TParamAlias getCFG4DParamAliasList() const override;
    virtual void splitCFG4DParamAliasKey( const TParamAliasKey &pk,
                                          std::string &semanticLevel,
                                          std::string &function,
                                          TParamIndex &numParameter ) const override;
    virtual void splitCFG4DParamAliasKey( const TParamAliasKey &pk,
                                          TWindowLevel &semanticLevel,
                                          std::string &function,
                                          TParamIndex &numParameter ) const override;

    virtual Timeline::TParamAliasKey getCFG4DParamAliasKey( const TParamAlias::iterator it ) const override;
    virtual const std::string getCFG4DParamAlias( const TParamAlias::iterator &it ) const override;
    virtual const std::string getCFG4DParamAlias( const TParamAliasKey &pk ) const override;
    virtual std::vector< Timeline::TParamAliasKey > getCFG4DParamKeysBySemanticLevel(
                    std::string whichSemanticLevel,
                    const std::vector< Timeline::TParamAliasKey > &whichParamAlias = std::vector<Timeline::TParamAliasKey >()  ) const override;
    virtual const Timeline::TParamAliasKey buildCFG4DParamAliasKey(
                                                const std::string &semanticLevel,
                                                const std::string &function,
                                                const TParamIndex &numParameter ) const override;
    virtual std::string getCFG4DParameterOriginalName( TWindowLevel whichLevel, TParamIndex whichParam ) const override;

    virtual void setCFGS4DGroupLink( std::string originalName, TCFGS4DGroup whichGroup ) override;
    virtual TCFGS4DGroup getCFGS4DGroupLink( std::string originalName ) const override;

    virtual void setCFGS4DIndexLink( TCFGS4DIndexLink whichIndex ) override;
    virtual TCFGS4DIndexLink getCFGS4DIndexLink() const override;

#ifdef _MSC_VER
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
                                          ProgressController *progress ) override;

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
                                          std::vector< std::unordered_set< PRV_INT32 > >& eventsToDraw,                 // I/O
                                          std::vector< std::unordered_set< commCoord, hashCommCoord > >& commsToDraw,   // I/O
                                          ProgressController *progress ) override;
#endif // _WIN32

#ifdef _MSC_VER
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
                                                  ProgressController *progress ) override;
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
                                                  std::vector< std::unordered_set< PRV_INT32 > >& eventsToDraw,                // I/O
                                                  std::vector< std::unordered_set< commCoord, hashCommCoord > >& commsToDraw,    // I/O
                                                  ProgressController *progress ) override;
#endif

  private:
    Timeline *myWindow;
#ifdef PARALLEL_ENABLED
    std::vector<Timeline *> parallelClone;
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
    Timeline *parent1;
    Timeline *parent2;
    Timeline *child;

    std::set<Histogram *> usedByHistogram;

    // GUI related attributes
    std::string name;
    CodeColor myCodeColor;
    GradientColor myGradientColor;
    GradientColor myAltGradientColor{ std::vector<rgb>{ {0,255,0}, {255,255,0}, {255,0,0} } };
    bool useCustomPalette;
    DrawModeMethod drawModeObject;
    DrawModeMethod drawModeTime;
    TColorFunction colorMode;
    bool semanticScaleMinAtZero;
    bool showWindow;
    bool raise;
    bool changed;
    bool redraw;
    bool forceRedraw;
    bool commLines;
    bool flags;
    PRV_UINT16 pixelSize;

    Timeline *punctualColorWindow;
    // Zoom history
    ZoomHistory<TTime, TObjectOrder> zoomHistory;

    // Synchronize
    bool sync;
    TGroupId syncGroup;

    // Row selection
    SelectionManagement< TObjectOrder, TTraceLevel > selectedRow;

    TObjectLabels objectLabels;
    TObjectAxisSize objectAxisSize;

    // CFG4D
    bool isCFG4DEnabled;
    bool CFG4DMode;
    std::map< std::string, std::string > propertiesAliasCFG4D;
    TParamAlias paramAliasCFG4D;
    std::map< std::string, TCFGS4DGroup > groupLinkFromPropName;
    TCFGS4DIndexLink globalIndexLink;
    static std::stringstream sstrCFGS4DOriginalName;


    // For Clone
    TimelineProxy();
    // For Single Timeline
    TimelineProxy( KernelConnection *whichKernel, Trace *whichTrace );
    // For Derived Timeline
    TimelineProxy( KernelConnection *whichKernel );
    TimelineProxy( KernelConnection *whichKernel, Timeline *whichParent1,
                 Timeline *whichParent2 );

    int number_of_clones;

    void init();

    void getAllLevelsSelectedRows( TWindowLevel onLevel, std::vector< TObjectOrder > &selected );

#ifdef _MSC_VER
    void computeSemanticRowPunctualParallel( int numRows,
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

    void computeSemanticRowParallel( int numRows,
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

    void computeEventsCommsParallel( RecordList *records,
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
                                             std::unordered_set< PRV_INT32 >& eventsToDraw,                 // I/O
                                             std::unordered_set< commCoord, hashCommCoord >& commsToDraw,
                                             ProgressController *progress ); // I/O

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
                                     std::unordered_set< PRV_INT32 >& eventsToDraw,                 // I/O
                                     std::unordered_set< commCoord, hashCommCoord >& commsToDraw,
                                     ProgressController *progress ); // I/O

    void computeEventsCommsParallel( RecordList *records,
                                     TTime from,
                                     TTime to,
                                     TTime step,
                                     PRV_INT32 timePos,
                                     PRV_INT32 objectAxisPos,
                                     std::vector< bool >& selected,
                                     std::vector< PRV_INT32 >& objectPosList,
                                     std::unordered_set< PRV_INT32 >& eventsToDraw,                 // I/O
                                     std::unordered_set< commCoord, hashCommCoord >& commsToDraw ); // I/O
#endif

    friend Timeline *Timeline::create( KernelConnection *, Trace * );
    friend Timeline *Timeline::create( KernelConnection * );
    friend Timeline *Timeline::create( KernelConnection *, Timeline *, Timeline * );
};


