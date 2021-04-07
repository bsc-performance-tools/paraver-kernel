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


#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include <limits>
#include "paraverkerneltypes.h"
#include "paraverconfig.h"
#include "semanticcolor.h"
#include "drawmode.h"
#include "zoomhistory.h"
#include "paraverlabels.h"
#include "cfgs4d.h"

#ifdef WIN32
#undef max
#undef min
#endif

class KernelConnection;
class HistogramTotals;
class Window;
class Trace;

class Histogram
{
  public:
    static Histogram *create( KernelConnection *whichKernel );

    Histogram() {};
    Histogram( KernelConnection *whichKernel );
    virtual ~Histogram() {}

    virtual void setWindowBeginTime( TRecordTime whichTime, bool isBroadcast = false ) {};
    virtual void setWindowEndTime( TRecordTime whichTime, bool isBroadcast = false ) {};

    virtual bool getThreeDimensions() const = 0;

    virtual TRecordTime getBeginTime() const = 0;
    virtual TRecordTime getEndTime() const = 0;

    virtual Window *getControlWindow() const = 0;
    virtual Window *getDataWindow() const = 0;
    virtual Window *getExtraControlWindow() const = 0;
    virtual void setControlWindow( Window *whichWindow ) = 0;
    virtual void setDataWindow( Window *whichWindow ) = 0;
    virtual void setExtraControlWindow( Window *whichWindow ) = 0;
    virtual void clearControlWindow() = 0;
    virtual void clearDataWindow() = 0;
    virtual void clearExtraControlWindow() = 0;

    virtual void setControlMin( THistogramLimit whichMin ) = 0;
    virtual void setControlMax( THistogramLimit whichMax ) = 0;
    virtual void setControlDelta( THistogramLimit whichDelta ) = 0;
    virtual void setExtraControlMin( THistogramLimit whichMin ) = 0;
    virtual void setExtraControlMax( THistogramLimit whichMax ) = 0;
    virtual void setExtraControlDelta( THistogramLimit whichDelta ) = 0;
    virtual void setDataMin( TSemanticValue whichMin ) = 0;
    virtual void setDataMax( TSemanticValue whichMax ) = 0;
    virtual void setBurstMin( TRecordTime whichTime ) = 0;
    virtual void setBurstMax( TRecordTime whichTime ) = 0;
    virtual void setCommSizeMin( TCommSize whichSize ) = 0;
    virtual void setCommSizeMax( TCommSize whichSize ) = 0;
    virtual void setCommTagMin( TCommTag whichTag ) = 0;
    virtual void setCommTagMax( TCommTag whichTag ) = 0;

    virtual THistogramLimit getControlMin() const = 0;
    virtual THistogramLimit getControlMax() const = 0;
    virtual THistogramLimit getControlDelta() const = 0;
    virtual THistogramLimit getExtraControlMin() const = 0;
    virtual THistogramLimit getExtraControlMax() const = 0;
    virtual THistogramLimit getExtraControlDelta() const = 0;
    virtual TSemanticValue getDataMin() const = 0;
    virtual TSemanticValue getDataMax() const = 0;
    virtual TRecordTime getBurstMin() const = 0;
    virtual TRecordTime getBurstMax() const = 0;
    virtual TCommSize getCommSizeMin() const = 0;
    virtual TCommSize getCommSizeMax() const = 0;
    virtual TCommTag getCommTagMin() const = 0;
    virtual TCommTag getCommTagMax() const = 0;

    virtual bool getInclusiveEnabled() const = 0;
    virtual void setInclusive( bool newValue ) = 0;
    virtual bool getInclusive() const = 0;

    virtual THistogramColumn getNumPlanes() const = 0;
    virtual THistogramColumn getNumColumns() const = 0;
    virtual TObjectOrder getNumRows() const = 0;

    virtual TSemanticValue getCurrentValue( PRV_UINT32 col,
                                            PRV_UINT16 idStat,
                                            PRV_UINT32 plane = 0 ) const = 0;
    virtual PRV_UINT32 getCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const = 0;
    virtual void setNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) = 0;
    virtual void setFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) = 0;
    virtual bool endCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) = 0;
    virtual bool planeWithValues( PRV_UINT32 plane = 0 ) const = 0;
    virtual bool getCellValue( TSemanticValue& semVal,
                               PRV_UINT32 whichRow,
                               PRV_UINT32 whichCol,
                               PRV_UINT16 idStat,
                               PRV_UINT32 whichPlane = 0 ) const = 0;

    virtual TSemanticValue getCommCurrentValue( PRV_UINT32 col,
        PRV_UINT16 idStat,
        PRV_UINT32 plane = 0 ) const = 0;
    virtual PRV_UINT32 getCommCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const = 0;
    virtual void setCommNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) = 0;
    virtual void setCommFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) = 0;
    virtual bool endCommCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) = 0;
    virtual bool planeCommWithValues( PRV_UINT32 plane = 0 ) const = 0;
    virtual bool getCommCellValue( TSemanticValue& semVal,
                                   PRV_UINT32 whichRow,
                                   PRV_UINT32 whichCol,
                                   PRV_UINT16 idStat,
                                   PRV_UINT32 whichPlane = 0 ) const = 0;

    virtual HistogramTotals *getColumnTotals() const = 0;
    virtual HistogramTotals *getCommColumnTotals() const = 0;
    virtual HistogramTotals *getRowTotals() const = 0;
    virtual HistogramTotals *getCommRowTotals() const = 0;

    virtual void clearStatistics() = 0;
    virtual void pushbackStatistic( const std::string& whichStatistic ) = 0;

    virtual void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime,
                          std::vector<TObjectOrder>& selectedRows, ProgressController *progress ) = 0;

    virtual bool isCommunicationStat( const std::string& whichStat ) const = 0;

    virtual std::string getUnitsLabel( const std::string& whichStat ) const = 0;

    virtual void getGroupsLabels( std::vector<std::string>& onVector ) const = 0;
    virtual void getStatisticsLabels( std::vector<std::string>& onVector,
                                      PRV_UINT32 whichGroup,
                                      bool getOriginalList = true ) const = 0;
    virtual std::string getFirstStatistic() const = 0;
    virtual std::string getFirstCommStatistic() const = 0;

    virtual bool getControlOutOfLimits() const = 0;
    virtual bool getExtraOutOfLimits() const = 0;

    virtual TTimeUnit getTimeUnit() const = 0;

    virtual Histogram* clone( ) { return NULL; }

    // Specific methods of HistogramProxy
    virtual Trace *getTrace() const { return NULL; }
    virtual bool getDestroy() const { return false; }
    virtual void setDestroy( bool newValue ) {}
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
      return 600;
    }
    virtual void setWidth( PRV_UINT16 whichPos )
    {}

    virtual PRV_UINT16 getHeight() const
    {
      return 300;
    }
    virtual void setHeight( PRV_UINT16 whichPos )
    {}
    virtual HistogramTotals *getTotals( const std::string& whichStat ) const
    {
      return NULL;
    }
    virtual void setHorizontal( bool newValue ) {}
    virtual bool getHorizontal() const
    {
      return ParaverConfig::getInstance()->getHistogramViewHorizontal();
    }
    virtual void setHideColumns( bool newValue ) {}
    virtual bool getHideColumns() const
    {
      return !ParaverConfig::getInstance()->getHistogramViewEmptyColumns();
    }
    virtual void setOnlyTotals( bool newValue ) {}
    virtual bool getOnlyTotals() const
    {
      return ParaverConfig::getInstance()->getHistogramOnlyTotals();
    }
    virtual void setShortLabels( bool newValue ) {}
    virtual bool getShortLabels() const
    {
      return ParaverConfig::getInstance()->getHistogramShortLabels();
    }
    virtual void setScientificNotation( bool newValue ) {}
    virtual bool getScientificNotation() const
    {
      return ParaverConfig::getInstance()->getHistogramScientificNotation();
    }
    virtual void setNumDecimals( PRV_UINT16 newValue ) {}
    virtual PRV_UINT16 getNumDecimals() const
    {
      return ParaverConfig::getInstance()->getHistogramPrecision();
    }
    virtual void setThousandSeparator( bool newValue ) {}
    virtual bool getThousandSeparator() const
    {
      return ParaverConfig::getInstance()->getHistogramThousandSep();
    }
    virtual void setShowUnits( bool newValue ) {}
    virtual bool getShowUnits() const
    {
      return ParaverConfig::getInstance()->getHistogramShowUnits();
    }
    virtual void setSemanticSortColumns( bool newValue ) {}
    virtual bool getSemanticSortColumns() const
    {
      return false;
    }
    virtual void setSemanticSortCriteria( THistoTotals whichCriteria ) {}
    virtual THistoTotals getSemanticSortCriteria() const
    {
      return AVERAGE;
    }
    virtual void setSemanticSortReverse( bool newValue ) {}
    virtual bool getSemanticSortReverse() const
    {
      return false;
    }
    virtual PRV_UINT32 getSemanticSortedColumn( PRV_UINT32 col ) const
    {
      return 0;
    }
    virtual void setMinGradient( double whichMin ) {}
    virtual double getMinGradient() const
    {
      return -std::numeric_limits<double>::max();
    }
    virtual void setMaxGradient( double whichMax ) {}
    virtual double getMaxGradient() const
    {
      return std::numeric_limits<double>::max();
    }
    virtual void setComputeScale( bool newValue ) {}
    virtual bool getComputeScale() const
    {
      return true;
    }
    virtual void setCompute2DScale( bool newValue ) {}
    virtual bool getCompute2DScale() const
    {
      return ParaverConfig::getInstance()->getHistogramAutofitControlScale();
    }
    virtual void setCompute2DScaleZero( bool newValue ) {}
    virtual bool getCompute2DScaleZero() const
    {
      return ParaverConfig::getInstance()->getHistogramAutofitControlScaleZero();
    }
    virtual void setCompute3DScale( bool newValue ) {}
    virtual bool getCompute3DScale() const
    {
      return ParaverConfig::getInstance()->getHistogramAutofitThirdDimensionScale();
    }
    virtual void setComputeGradient( bool newValue ) {}
    virtual bool getComputeGradient() const
    {
      return ParaverConfig::getInstance()->getHistogramAutofitDataGradient();
    }
    virtual void setShowColor( bool newValue ) {}
    virtual bool getShowColor() const
    {
      return ParaverConfig::getInstance()->getHistogramViewGradientColors();
    }
    virtual rgb calcGradientColor( TSemanticValue whichValue ) const
    {
      rgb tmp = SemanticColor::BACKGROUND;
      return tmp;
    }
    virtual GradientColor& getGradientColor()
    {
      GradientColor *tmp = NULL;
      return *tmp;
    }
    virtual void recalcGradientLimits() {}
    virtual void setZoom( bool newValue ) {}
    virtual bool getZoom() const
    {
      return ParaverConfig::getInstance()->getHistogramViewZoom();
    }
    virtual void setFirstRowColored( bool newValue ) {}
    virtual bool getFirstRowColored() const
    {
      return ParaverConfig::getInstance()->getHistogramViewFirstRowColored();
    }

    virtual void setPlaneMinValue( double whichMin ) {}
    virtual double getPlaneMinValue() const
    {
      return 0;
    }
    virtual void setSelectedPlane( PRV_INT32 plane ) {}
    virtual PRV_INT32 getSelectedPlane() const
    {
      return 0;
    }
    virtual PRV_INT32 getCommSelectedPlane() const
    {
      return 0;
    }
    virtual void compute2DScale( ProgressController *progress = NULL ) {}
    virtual void compute3DScale( ProgressController *progress = NULL ) {}
    virtual std::string getRowLabel( TObjectOrder whichRow ) const
    {
      return "Unnamed row";
    }
    virtual std::string getColumnLabel( THistogramColumn whichColumn ) const
    {
      return "Unnamed column";
    }
    virtual std::string getPlaneLabel( THistogramColumn whichPlane ) const
    {
      return "Unnamed plane";
    }

    virtual THistogramColumn getPlaneColumns( THistogramColumn iPlane,
                                              bool hideEmptyColumns,
                                              std::vector<THistogramColumn> &noVoidSemRanges ) const
    {
      return 0;
    }

    virtual THistogramColumn getSemanticRealColumn( THistogramColumn whichCol, 
                                                    const std::vector<THistogramColumn>& noVoidSemRanges ) const 
    { 
      return 0;
    }

    // Zoom history
    typedef struct TZoomInfo
    {
      THistogramLimit begin;
      THistogramLimit end;

      bool operator==( const TZoomInfo& obj )
      {
        return begin == obj.begin && end == obj.end;
      }
    } TZoomInfo;

    virtual bool isZoomEmpty() const
    {
      return true;
    }
    virtual bool emptyPrevZoom() const
    {
      return true;
    }
    virtual bool emptyNextZoom() const
    {
      return true;
    }
    virtual void nextZoom()
    {}
    virtual void prevZoom()
    {}
    virtual void addZoom( TZoomInfo columnInfo, TZoomInfo dummy,
                          TObjectOrder beginObject, TObjectOrder endObject )
    {}
    virtual void addZoom( TZoomInfo columnInfo, TZoomInfo dummy )
    {}
    virtual void addZoom( TObjectOrder beginObject, TObjectOrder endObject )
    {}
    virtual void setZoomFirstDimension( std::pair<TZoomInfo, TZoomInfo> &zinfo )
    {}
    virtual void setZoomSecondDimension( std::pair<TObjectOrder, TObjectOrder> &objects )
    {}
    virtual std::pair<TZoomInfo, TZoomInfo> getZoomFirstDimension() const
    {
      return std::pair<TZoomInfo, TZoomInfo>();
    }
    virtual std::pair<TObjectOrder, TObjectOrder> getZoomSecondDimension() const
    {
      return std::pair<TObjectOrder, TObjectOrder>();
    }

    // Synchronize
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

    virtual void setName( const std::string& whichName ) {}
    virtual std::string getName() const
    {
      return "Unnamed histogram";
    }
    virtual void setCalculateAll( bool status ) {}
    virtual bool getCalculateAll() const
    {
      return true;
    }
    virtual bool getIdStat( const std::string& whichStat, PRV_UINT16& idStat ) const
    {
      return false;
    }
    virtual void setCurrentStat( const std::string& whichStat ) {}
    virtual std::string getCurrentStat() const
    {
      return "";
    }
    virtual THistogramColumn getNumColumns( const std::string& whichStat ) const
    {
      return getNumColumns();
    }
    virtual bool getShowWindow() const
    {
      return true;
    }
    virtual void setShowWindow( bool newValue )
    {}
    virtual DrawModeMethod getDrawModeObjects() const
    {
      return ParaverConfig::getInstance()->getHistogramDrawmodeObjects();
    }
    virtual void setDrawModeObjects( DrawModeMethod whichMethod )
    {}
    virtual DrawModeMethod getDrawModeColumns() const
    {
      return ParaverConfig::getInstance()->getHistogramDrawmodeSemantic();
    }
    virtual void setDrawModeColumns( DrawModeMethod whichMethod )
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
    virtual bool getRecalc() const
    {
      return false;
    }
    virtual void setRecalc( bool newValue )
    {}
    virtual bool getForceRecalc() const
    {
      return false;
    }
    virtual void setForceRecalc( bool newValue )
    {}
    // DEPRECATED
    virtual bool getCodeColor() const
    {
      return false;
    }
    // DEPRECATED
    virtual void setCodeColor( bool newValue )
    {}
    virtual SemanticColor::TColorFunction getColorMode() const
    {
      return SemanticColor::GRADIENT;
    }
    virtual void setColorMode( SemanticColor::TColorFunction whichMode )
    {}
    virtual PRV_UINT16 getPixelSize() const
    {
      return 1;
    }
    virtual void setPixelSize( PRV_UINT16 whichSize )
    {}
    virtual bool getShowProgressBar() const
    {
      return true;
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

    virtual bool existsCFG4DAlias( const THistogramProperties &propertyIndex ) const
    {
      return false;
    }

    virtual std::string getCFG4DAlias( const std::string &property ) const
    {
      return std::string( "" );
    }

    virtual std::string getCFG4DAlias( const THistogramProperties &propertyIndex ) const
    {
      return std::string( "" );
    }

    virtual void setCFG4DAlias( const std::string &property, const std::string &alias )
    {}
    virtual void setCFG4DStatisticAlias( const std::string &statistic, const std::string &alias )
    {}

    virtual void setCFG4DAliasList( const std::map< std::string, std::string >& aliasList )
    {}

    virtual void setCFG4DStatisticsAliasList( const std::map< std::string, std::string >& statisticsAliasList )
    {}

    virtual const std::map< std::string, std::string > getCFG4DAliasList() const
    {
      return std::map< std::string, std::string >();
    }

    virtual const std::map< std::string, std::string > getCFG4DStatisticsAliasList() const
    {
      return std::map< std::string, std::string >();
    }

    virtual const std::vector< std::string > getCFG4DFullTagList()
    {
      return std::vector< std::string >();
    }
    
    virtual void setCFGS4DIndexLink( std::string whichName, TCFGS4DIndexLink whichLink )
    {}

    virtual TCFGS4DIndexLink getCFGS4DIndexLink( std::string whichName ) const
    {
      return 0;
    }

    virtual SelectionManagement< TObjectOrder, TWindowLevel > * getRowSelectionManagement()
    {
      return ( SelectionManagement< TObjectOrder, TWindowLevel > * ) NULL;
    }

    virtual void setRowSelectionManager( SelectionManagement< TObjectOrder, TWindowLevel > &rowSel )
    {}

    virtual std::vector< TObjectOrder > getSelectedRows() const
    {
      return std::vector< TObjectOrder > ();
    }

    virtual std::vector< TObjectOrder > getSelectedRows( TObjectOrder whichBeginRow, TObjectOrder whichEndRow ) const
    {
      return std::vector< TObjectOrder > ();
    }
 
    virtual std::vector< bool > getSelectedBooleanRows() const 
    {
      return std::vector< bool > ();
    }

    virtual void setSelectedRows( std::vector< bool > &selected )
    {}

    virtual void setSelectedRows( std::vector< TObjectOrder > &selected )
    {}

  protected:
    KernelConnection *myKernel;

};


class HistogramProxy : public Histogram
{
  public:
    virtual ~HistogramProxy();

    virtual void setWindowBeginTime( TRecordTime whichTime, bool isBroadcast = false );
    virtual void setWindowEndTime( TRecordTime whichTime, bool isBroadcast = false );

    virtual bool getThreeDimensions() const;
    virtual TRecordTime getBeginTime() const;
    virtual TRecordTime getEndTime() const;
    virtual Trace *getTrace() const;
    virtual Window *getControlWindow() const;
    virtual Window *getDataWindow() const;
    virtual Window *getExtraControlWindow() const;
    virtual void setControlWindow( Window *whichWindow );
    virtual void setDataWindow( Window *whichWindow );
    virtual void setExtraControlWindow( Window *whichWindow );
    virtual void clearControlWindow();
    virtual void clearDataWindow();
    virtual void clearExtraControlWindow();
    virtual void setControlMin( THistogramLimit whichMin );
    virtual void setControlMax( THistogramLimit whichMax );
    virtual void setControlDelta( THistogramLimit whichDelta );
    virtual void setExtraControlMin( THistogramLimit whichMin );
    virtual void setExtraControlMax( THistogramLimit whichMax );
    virtual void setExtraControlDelta( THistogramLimit whichDelta );
    virtual void setDataMin( THistogramLimit whichMin );
    virtual void setDataMax( THistogramLimit whichMax );
    virtual void setBurstMin( TRecordTime whichTime );
    virtual void setBurstMax( TRecordTime whichTime );
    virtual void setCommSizeMin( TCommSize whichSize );
    virtual void setCommSizeMax( TCommSize whichSize );
    virtual void setCommTagMin( TCommTag whichTag );
    virtual void setCommTagMax( TCommTag whichTag );

    virtual THistogramLimit getControlMin() const;
    virtual THistogramLimit getControlMax() const;
    virtual THistogramLimit getControlDelta() const;
    virtual THistogramLimit getExtraControlMin() const;
    virtual THistogramLimit getExtraControlMax() const;
    virtual THistogramLimit getExtraControlDelta() const;
    virtual THistogramLimit getDataMin() const;
    virtual THistogramLimit getDataMax() const;
    virtual TRecordTime getBurstMin() const;
    virtual TRecordTime getBurstMax() const;
    virtual TCommSize getCommSizeMin() const;
    virtual TCommSize getCommSizeMax() const;
    virtual TCommTag getCommTagMin() const;
    virtual TCommTag getCommTagMax() const;

    virtual bool getInclusiveEnabled() const;
    virtual void setInclusive( bool newValue );
    virtual bool getInclusive() const;

    virtual THistogramColumn getNumPlanes() const;
    virtual THistogramColumn getNumColumns( const std::string& whichStat ) const;
    virtual TObjectOrder getNumRows() const;

    virtual TSemanticValue getCurrentValue( PRV_UINT32 col,
                                            PRV_UINT16 idStat,
                                            PRV_UINT32 plane = 0 ) const;
    virtual PRV_UINT32 getCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const;
    virtual void setNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    virtual void setFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    virtual bool endCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    virtual bool planeWithValues( PRV_UINT32 plane = 0 ) const;
    virtual bool getCellValue( TSemanticValue& semVal,
                               PRV_UINT32 whichRow,
                               PRV_UINT32 whichCol,
                               PRV_UINT16 idStat,
                               PRV_UINT32 whichPlane = 0 ) const;

    virtual TSemanticValue getCommCurrentValue( PRV_UINT32 col,
        PRV_UINT16 idStat,
        PRV_UINT32 plane = 0 ) const;
    virtual PRV_UINT32 getCommCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const;
    virtual void setCommNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    virtual void setCommFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    virtual bool endCommCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    virtual bool planeCommWithValues( PRV_UINT32 plane = 0 ) const;
    virtual bool getCommCellValue( TSemanticValue& semVal,
                                   PRV_UINT32 whichRow,
                                   PRV_UINT32 whichCol,
                                   PRV_UINT16 idStat,
                                   PRV_UINT32 whichPlane = 0 ) const;

    virtual HistogramTotals *getTotals( const std::string& whichStat ) const;
    virtual HistogramTotals *getColumnTotals() const;
    virtual HistogramTotals *getCommColumnTotals() const;
    virtual HistogramTotals *getRowTotals() const;
    virtual HistogramTotals *getCommRowTotals() const;

    virtual void clearStatistics();
    virtual void pushbackStatistic( const std::string& whichStatistic );

    virtual void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime,
                          std::vector<TObjectOrder>& selectedRows, ProgressController *progress );

    virtual void setHorizontal( bool newValue );
    virtual bool getHorizontal() const;
    virtual void setHideColumns( bool newValue );
    virtual bool getHideColumns() const;
    virtual void setScientificNotation( bool newValue );
    virtual bool getScientificNotation() const;
    virtual void setNumDecimals( PRV_UINT16 newValue );
    virtual PRV_UINT16 getNumDecimals() const;
    virtual void setThousandSeparator( bool newValue );
    virtual bool getThousandSeparator() const;
    virtual void setShowUnits( bool newValue );
    virtual bool getShowUnits() const;
    virtual void setSemanticSortColumns( bool newValue );
    virtual bool getSemanticSortColumns() const;
    virtual void setSemanticSortCriteria( THistoTotals whichCriteria );
    virtual THistoTotals getSemanticSortCriteria() const;
    virtual void setSemanticSortReverse( bool newValue );
    virtual bool getSemanticSortReverse() const;
    virtual PRV_UINT32 getSemanticSortedColumn( PRV_UINT32 col ) const;
    virtual void setMinGradient( double whichMin );
    virtual double getMinGradient() const;
    virtual void setMaxGradient( double whichMax );
    virtual double getMaxGradient() const;
    virtual void setComputeScale( bool newValue );
    virtual bool getComputeScale() const;
    virtual void setCompute2DScale( bool newValue );
    virtual bool getCompute2DScale() const;
    virtual void setCompute2DScaleZero( bool newValue );
    virtual bool getCompute2DScaleZero() const;
    virtual void setCompute3DScale( bool newValue );
    virtual bool getCompute3DScale() const;
    virtual void setComputeGradient( bool newValue );
    virtual bool getComputeGradient() const;
    virtual void setShowColor( bool newValue );
    virtual bool getShowColor() const;
    virtual rgb calcGradientColor( TSemanticValue whichValue ) const;
    virtual GradientColor& getGradientColor();
    virtual void recalcGradientLimits();
    virtual void setZoom( bool newValue );
    virtual bool getZoom() const;
    virtual void setFirstRowColored( bool newValue );
    virtual bool getFirstRowColored() const;
    virtual void setPlaneMinValue( double whichMin );
    virtual double getPlaneMinValue() const;
    virtual void setSelectedPlane( PRV_INT32 plane );
    virtual PRV_INT32 getSelectedPlane() const;
    virtual PRV_INT32 getCommSelectedPlane() const;

    virtual void compute2DScale( ProgressController *progress = NULL );
    virtual void compute3DScale( ProgressController *progress = NULL );

    virtual std::string getRowLabel( TObjectOrder whichRow ) const;
    virtual std::string getColumnLabel( THistogramColumn whichColumn ) const;
    virtual std::string getPlaneLabel( THistogramColumn whichPlane ) const;
    virtual bool getShortLabels() const;
    virtual void setShortLabels( bool newValue );

    virtual THistogramColumn getPlaneColumns( THistogramColumn iPlane,
                                              bool hideEmptyColumns,
                                              std::vector<THistogramColumn> &noVoidSemRanges ) const;

    // Zoom history
    virtual bool isZoomEmpty() const;
    virtual bool emptyPrevZoom() const;
    virtual bool emptyNextZoom() const;
    virtual void nextZoom();
    virtual void prevZoom();
    virtual void addZoom( TZoomInfo columnInfo, TZoomInfo dummy,
                          TObjectOrder beginObject, TObjectOrder endObject );
    virtual void addZoom( TZoomInfo columnInfo, TZoomInfo dummy );
    virtual void addZoom( TObjectOrder beginObject, TObjectOrder endObject );
    virtual void setZoomFirstDimension( std::pair<TZoomInfo, TZoomInfo> &zinfo );
    virtual void setZoomSecondDimension( std::pair<TObjectOrder, TObjectOrder> &objects );
    virtual std::pair<TZoomInfo, TZoomInfo> getZoomFirstDimension() const;
    virtual std::pair<TObjectOrder, TObjectOrder> getZoomSecondDimension() const;

    virtual void addToSyncGroup( TGroupId whichGroup );
    virtual void removeFromSync();
    virtual bool isSync() const;
    virtual TGroupId getSyncGroup() const;

    virtual void setName( const std::string& whichName );
    virtual std::string getName() const;

    virtual void setCalculateAll( bool status );
    virtual bool getCalculateAll() const;

    virtual bool getIdStat( const std::string& whichStat, PRV_UINT16& idStat ) const;

    virtual void setCurrentStat( const std::string& whichStat );
    virtual std::string getCurrentStat() const;

    bool isCommunicationStat( const std::string& whichStat ) const;

    THistogramColumn getSemanticRealColumn( THistogramColumn whichCol, const std::vector<THistogramColumn>& noVoidSemRanges ) const;

    std::string getUnitsLabel( const std::string& whichStat ) const;
    virtual void getGroupsLabels( std::vector<std::string>& onVector ) const;
    virtual void getStatisticsLabels( std::vector<std::string>& onVector,
                                      PRV_UINT32 whichGroup,
                                      bool getOriginalList = true ) const;
    virtual std::string getFirstStatistic() const;
    virtual std::string getFirstCommStatistic() const;

    virtual bool getControlOutOfLimits() const;
    virtual bool getExtraOutOfLimits() const;

    virtual TTimeUnit getTimeUnit() const;

    virtual Histogram *clone();

    virtual bool getDestroy() const;
    virtual void setDestroy( bool newValue );
    virtual PRV_UINT16 getPosX() const;
    virtual void setPosX( PRV_UINT16 whichPos );
    virtual PRV_UINT16 getPosY() const;
    virtual void setPosY( PRV_UINT16 whichPos );
    virtual PRV_UINT16 getWidth() const;
    virtual void setWidth( PRV_UINT16 whichPos );
    virtual PRV_UINT16 getHeight() const;
    virtual void setHeight( PRV_UINT16 whichPos );
    virtual bool getShowWindow() const;
    virtual void setShowWindow( bool newValue );
    virtual DrawModeMethod getDrawModeObjects() const;
    virtual void setDrawModeObjects( DrawModeMethod whichMethod );
    virtual DrawModeMethod getDrawModeColumns() const;
    virtual void setDrawModeColumns( DrawModeMethod whichMethod );
    virtual bool getChanged() const;
    virtual void setChanged( bool newValue );
    virtual bool getRedraw() const;
    virtual void setRedraw( bool newValue );
    virtual bool getRecalc() const;
    virtual void setRecalc( bool newValue );
    virtual bool getForceRecalc() const;
    virtual void setForceRecalc( bool newValue );
    // DEPRECATED
    virtual bool getCodeColor() const;
    // DEPRECATED
    virtual SemanticColor::TColorFunction getColorMode() const;
    virtual void setColorMode( SemanticColor::TColorFunction whichMode );
    virtual void setCodeColor( bool newValue );
    virtual PRV_UINT16 getPixelSize() const;
    virtual void setPixelSize( PRV_UINT16 whichSize );
    virtual void setOnlyTotals( bool newValue );
    virtual bool getOnlyTotals() const;
    virtual bool getShowProgressBar() const;

    // CFG4D
    // Enabled => mode = false
    virtual void setCFG4DEnabled( bool enabled );
    virtual bool getCFG4DEnabled() const;

    // If CFG4D is enabled, mode can be changed:
    //   false => no CFG4D replacement
    //   true  => CFG4D replacement
    virtual void setCFG4DMode( bool mode );
    virtual bool getCFG4DMode() const;

    virtual bool existsCFG4DAlias( const std::string &property ) const; // DEPRECATED
    virtual bool existsCFG4DAlias( const THistogramProperties &propertyIndex ) const;

    virtual void setCFG4DAlias( const std::string &property, const std::string &alias );
    virtual void setCFG4DStatisticAlias( const std::string &statistic, const std::string &alias );

    virtual std::string getCFG4DAlias( const std::string &property ) const; // DEPRECATED
    virtual std::string getCFG4DAlias( const THistogramProperties &propertyIndex ) const;

    virtual void setCFG4DAliasList( const std::map< std::string, std::string >& aliasList );
    virtual void setCFG4DStatisticsAliasList( const std::map< std::string, std::string >& aliasList );
    virtual const std::map< std::string, std::string > getCFG4DAliasList() const;
    virtual const std::map< std::string, std::string > getCFG4DStatisticsAliasList() const;

    virtual const std::vector< std::string > getCFG4DFullTagList();
    virtual void setCFGS4DIndexLink( std::string whichName, TCFGS4DIndexLink whichLink );
    virtual TCFGS4DIndexLink getCFGS4DIndexLink( std::string whichName ) const;
    
    virtual SelectionManagement< TObjectOrder, TWindowLevel > * getRowSelectionManagement();
    virtual void setRowSelectionManager( SelectionManagement< TObjectOrder, TWindowLevel > &rowSel );

    virtual std::vector< TObjectOrder > getSelectedRows() const;
    virtual std::vector< TObjectOrder > getSelectedRows( TObjectOrder whichBeginRow, TObjectOrder whichEndRow ) const;
    virtual std::vector< bool > getSelectedBooleanRows() const;
    virtual void setSelectedRows( std::vector< bool > &selected );
    virtual void setSelectedRows( std::vector< TObjectOrder > &selected );


  private:
    std::string name;

    bool destroy;

    PRV_UINT16 posX;
    PRV_UINT16 posY;
    PRV_UINT16 width;
    PRV_UINT16 height;

    bool horizontal;
    bool hideColumns;
    bool scientificNotation;
    PRV_UINT16 numDecimals;
    bool thousandSep;
    bool showUnits;

    bool sortSemanticColumns;
    THistoTotals semanticSortCriteria;
    bool sortSemanticReverse;
    std::vector<int> currentSemanticSort;

    double minGradient;
    double maxGradient;
    bool computeControlScale;
    bool computeControlScaleZero;
    bool computeXtraScale;
    bool computeGradient;
    bool showColor;
    bool zoom;
    bool firstRowColored;
    bool futurePlane;
    double planeMinValue;
    PRV_INT32 selectedPlane;
    PRV_INT32 commSelectedPlane;
    bool showWindow;
    DrawModeMethod drawModeObjects;
    DrawModeMethod drawModeColumns;
    bool changed;
    bool redraw;
    bool recalc;
    bool forceRecalc;
    SemanticColor::TColorFunction colorMode;
    PRV_UINT16 pixelSize;
    bool onlyTotals;
    bool shortLabels;

    TRecordTime winBeginTime;
    TRecordTime winEndTime;

    // Zoom history
    ZoomHistory< TZoomInfo, TObjectOrder > zoomHistory;

    // Synchronize
    bool sync;
    TGroupId syncGroup;

    // Must store the associated proxies
    Window *controlWindow;
    Window *dataWindow;
    Window *extraControlWindow;
    Trace *myTrace;

    bool calculateAll;
    std::string currentStat;
    std::vector<std::string> calcStat;
    std::vector<std::string> commCalcStat;

    GradientColor myGradientColor;

    Histogram *myHisto;

    int number_of_clones;

    // CFG4D
    bool isCFG4DEnabled;
    bool CFG4DMode;
    std::map< std::string, std::string > propertiesAliasCFG4D;
    std::map< std::string, std::string > statisticsAliasCFG4D;
    std::map< std::string, TCFGS4DIndexLink > indexLinkFromPropName;

    //Selection of rows
    SelectionManagement< TObjectOrder, TWindowLevel > rowSelection;

    HistogramProxy( KernelConnection *whichKernel );

    THistogramColumn getNumColumns() const;

    void fillSemanticSort();

    friend Histogram *Histogram::create( KernelConnection * );
};


#endif // HISTOGRAM_H_INCLUDED
