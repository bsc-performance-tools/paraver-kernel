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


#include <limits>
#include "paraverkerneltypes.h"
#include "paraverconfig.h"
#include "semanticcolor.h"
#include "drawmode.h"
#include "zoomhistory.h"
#include "cfgs4d.h"

#ifdef _WIN32
#undef max
#undef min
#endif

class KernelConnection;
class HistogramTotals;
class Timeline;
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

    virtual Timeline *getControlWindow() const = 0;
    virtual Timeline *getDataWindow() const = 0;
    virtual Timeline *getExtraControlWindow() const = 0;
    virtual void setControlWindow( Timeline *whichWindow ) = 0;
    virtual void setDataWindow( Timeline *whichWindow ) = 0;
    virtual void setExtraControlWindow( Timeline *whichWindow ) = 0;
    virtual void clearControlWindow() = 0;
    virtual void clearDataWindow() = 0;
    virtual void clearExtraControlWindow() = 0;

    virtual void setUseCustomDelta( bool whichValue ) = 0;
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

    virtual bool getUseCustomDelta() const = 0;
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

    virtual void setNumColumns( THistogramColumn whichNumColumns ) = 0;

    virtual THistogramColumn getNumPlanes() const = 0;
    virtual THistogramColumn getNumColumns() const = 0;
    virtual THistogramColumn getCommNumColumns() const = 0;
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
    virtual bool getNotZeroValue( PRV_UINT32 whichRow,
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

    virtual bool isNotZeroStat( const std::string& whichStat ) const = 0;

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

    virtual Histogram* clone( ) { return nullptr; }

    // Specific methods of HistogramProxy
    virtual Trace *getTrace() const { return nullptr; }
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
      return nullptr;
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
    virtual void setSemanticSortCriteria( THistoSortCriteria whichCriteria ) {}
    virtual THistoSortCriteria getSemanticSortCriteria() const
    {
      return THistoSortCriteria::AVERAGE;
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
    virtual void setFixedSemanticSort( bool newValue ) {}
    virtual bool getFixedSemanticSort() const
    {
      return false;
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
      GradientColor *tmp = nullptr;
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
    virtual void setCommSelectedPlane( PRV_INT32 plane ) {}
    virtual PRV_INT32 getCommSelectedPlane() const
    {
      return 0;
    }
    virtual void compute2DScale( ProgressController *progress = nullptr ) {}
    virtual void compute3DScale( ProgressController *progress = nullptr ) {}
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
    virtual TColorFunction getColorMode() const
    {
      return TColorFunction::GRADIENT;
    }
    virtual void setColorMode( TColorFunction whichMode )
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

    virtual void setCFGS4DGroupLink( std::string originalName, TCFGS4DGroup whichGroup )
    {}

    virtual TCFGS4DGroup getCFGS4DGroupLink( std::string originalName ) const
    {
      return NO_GROUP_LINK;
    }

    virtual void setCFGS4DIndexLink( TCFGS4DIndexLink whichLink )
    {}

    virtual TCFGS4DIndexLink getCFGS4DIndexLink() const
    {
      return NO_INDEX_LINK;
    }

    virtual SelectionManagement< TObjectOrder, TTraceLevel > * getRowSelectionManagement()
    {
      return ( SelectionManagement< TObjectOrder, TTraceLevel > * ) nullptr;
    }

    virtual void setRowSelectionManager( SelectionManagement< TObjectOrder, TTraceLevel > &rowSel )
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

    virtual std::vector<int> getCurrentSemanticSort() const
    {
      return std::vector<int>();
    }

    virtual void setCurrentSemanticSort( const std::vector<int>& whichSort )
    {}

  protected:
    KernelConnection *myKernel;

};


class HistogramProxy : public Histogram
{
  public:
    virtual ~HistogramProxy();

    virtual void setWindowBeginTime( TRecordTime whichTime, bool isBroadcast = false ) override;
    virtual void setWindowEndTime( TRecordTime whichTime, bool isBroadcast = false ) override;

    virtual bool getThreeDimensions() const override;
    virtual TRecordTime getBeginTime() const override;
    virtual TRecordTime getEndTime() const override;
    virtual Trace *getTrace() const override;
    virtual Timeline *getControlWindow() const override;
    virtual Timeline *getDataWindow() const override;
    virtual Timeline *getExtraControlWindow() const override;
    virtual void setControlWindow( Timeline *whichWindow ) override;
    virtual void setDataWindow( Timeline *whichWindow ) override;
    virtual void setExtraControlWindow( Timeline *whichWindow ) override;
    virtual void clearControlWindow() override;
    virtual void clearDataWindow() override;
    virtual void clearExtraControlWindow() override;

    virtual void setUseCustomDelta( bool whichValue ) override;
    virtual void setControlMin( THistogramLimit whichMin ) override;
    virtual void setControlMax( THistogramLimit whichMax ) override;
    virtual void setControlDelta( THistogramLimit whichDelta ) override;
    virtual void setExtraControlMin( THistogramLimit whichMin ) override;
    virtual void setExtraControlMax( THistogramLimit whichMax ) override;
    virtual void setExtraControlDelta( THistogramLimit whichDelta ) override;
    virtual void setDataMin( THistogramLimit whichMin ) override;
    virtual void setDataMax( THistogramLimit whichMax ) override;
    virtual void setBurstMin( TRecordTime whichTime ) override;
    virtual void setBurstMax( TRecordTime whichTime ) override;
    virtual void setCommSizeMin( TCommSize whichSize ) override;
    virtual void setCommSizeMax( TCommSize whichSize ) override;
    virtual void setCommTagMin( TCommTag whichTag ) override;
    virtual void setCommTagMax( TCommTag whichTag ) override;

    virtual bool getUseCustomDelta() const override;
    virtual THistogramLimit getControlMin() const override;
    virtual THistogramLimit getControlMax() const override;
    virtual THistogramLimit getControlDelta() const override;
    virtual THistogramLimit getExtraControlMin() const override;
    virtual THistogramLimit getExtraControlMax() const override;
    virtual THistogramLimit getExtraControlDelta() const override;
    virtual THistogramLimit getDataMin() const override;
    virtual THistogramLimit getDataMax() const override;
    virtual TRecordTime getBurstMin() const override;
    virtual TRecordTime getBurstMax() const override;
    virtual TCommSize getCommSizeMin() const override;
    virtual TCommSize getCommSizeMax() const override;
    virtual TCommTag getCommTagMin() const override;
    virtual TCommTag getCommTagMax() const override;

    virtual bool getInclusiveEnabled() const override;
    virtual void setInclusive( bool newValue ) override;
    virtual bool getInclusive() const override;

    virtual void setNumColumns( THistogramColumn whichNumColumns ) override;

    virtual THistogramColumn getNumPlanes() const override;
    virtual THistogramColumn getNumColumns( const std::string& whichStat ) const override;
    virtual THistogramColumn getNumColumns() const override;
    virtual THistogramColumn getCommNumColumns() const override;
    virtual TObjectOrder     getNumRows() const override;

    virtual TSemanticValue getCurrentValue( PRV_UINT32 col,
                                            PRV_UINT16 idStat,
                                            PRV_UINT32 plane = 0 ) const override;
    virtual PRV_UINT32 getCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const override;
    virtual void setNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    virtual void setFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    virtual bool endCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    virtual bool planeWithValues( PRV_UINT32 plane = 0 ) const override;
    virtual bool getCellValue( TSemanticValue& semVal,
                               PRV_UINT32 whichRow,
                               PRV_UINT32 whichCol,
                               PRV_UINT16 idStat,
                               PRV_UINT32 whichPlane = 0 ) const override;
    virtual bool getNotZeroValue( PRV_UINT32 whichRow,
                                  PRV_UINT32 whichCol,
                                  PRV_UINT16 idStat,
                                  PRV_UINT32 whichPlane = 0 ) const override;

    virtual TSemanticValue getCommCurrentValue( PRV_UINT32 col,
        PRV_UINT16 idStat,
        PRV_UINT32 plane = 0 ) const override;
    virtual PRV_UINT32 getCommCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const override;
    virtual void setCommNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    virtual void setCommFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    virtual bool endCommCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    virtual bool planeCommWithValues( PRV_UINT32 plane = 0 ) const override;
    virtual bool getCommCellValue( TSemanticValue& semVal,
                                   PRV_UINT32 whichRow,
                                   PRV_UINT32 whichCol,
                                   PRV_UINT16 idStat,
                                   PRV_UINT32 whichPlane = 0 ) const override;

    virtual HistogramTotals *getTotals( const std::string& whichStat ) const override;
    virtual HistogramTotals *getColumnTotals() const override;
    virtual HistogramTotals *getCommColumnTotals() const override;
    virtual HistogramTotals *getRowTotals() const override;
    virtual HistogramTotals *getCommRowTotals() const override;

    virtual void clearStatistics() override;
    virtual void pushbackStatistic( const std::string& whichStatistic ) override;

    virtual void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime,
                          std::vector<TObjectOrder>& selectedRows, ProgressController *progress ) override;

    virtual void setHorizontal( bool newValue ) override;
    virtual bool getHorizontal() const override;
    virtual void setHideColumns( bool newValue ) override;
    virtual bool getHideColumns() const override;
    virtual void setScientificNotation( bool newValue ) override;
    virtual bool getScientificNotation() const override;
    virtual void setNumDecimals( PRV_UINT16 newValue ) override;
    virtual PRV_UINT16 getNumDecimals() const override;
    virtual void setThousandSeparator( bool newValue ) override;
    virtual bool getThousandSeparator() const override;
    virtual void setShowUnits( bool newValue ) override;
    virtual bool getShowUnits() const override;
    virtual void setSemanticSortColumns( bool newValue ) override;
    virtual bool getSemanticSortColumns() const override;
    virtual void setSemanticSortCriteria( THistoSortCriteria whichCriteria ) override;
    virtual THistoSortCriteria getSemanticSortCriteria() const override;
    virtual void setSemanticSortReverse( bool newValue ) override;
    virtual bool getSemanticSortReverse() const override;
    virtual PRV_UINT32 getSemanticSortedColumn( PRV_UINT32 col ) const override;
    virtual void setFixedSemanticSort( bool newValue ) override;
    virtual bool getFixedSemanticSort() const override;
    virtual void setMinGradient( double whichMin ) override;
    virtual double getMinGradient() const override;
    virtual void setMaxGradient( double whichMax ) override;
    virtual double getMaxGradient() const override;
    virtual void setComputeScale( bool newValue ) override;
    virtual bool getComputeScale() const override;
    virtual void setCompute2DScale( bool newValue ) override;
    virtual bool getCompute2DScale() const override;
    virtual void setCompute2DScaleZero( bool newValue ) override;
    virtual bool getCompute2DScaleZero() const override;
    virtual void setCompute3DScale( bool newValue ) override;
    virtual bool getCompute3DScale() const override;
    virtual void setComputeGradient( bool newValue ) override;
    virtual bool getComputeGradient() const override;
    virtual void setShowColor( bool newValue ) override;
    virtual bool getShowColor() const override;
    virtual rgb calcGradientColor( TSemanticValue whichValue ) const override;
    virtual GradientColor& getGradientColor() override;
    virtual void recalcGradientLimits() override;
    virtual void setZoom( bool newValue ) override;
    virtual bool getZoom() const override;
    virtual void setFirstRowColored( bool newValue ) override;
    virtual bool getFirstRowColored() const override;
    virtual void setPlaneMinValue( double whichMin ) override;
    virtual double getPlaneMinValue() const override;
    virtual void setSelectedPlane( PRV_INT32 plane ) override;
    virtual PRV_INT32 getSelectedPlane() const override;
    virtual void setCommSelectedPlane( PRV_INT32 plane ) override;
    virtual PRV_INT32 getCommSelectedPlane() const override;

    virtual void compute2DScale( ProgressController *progress = nullptr ) override;
    virtual void compute3DScale( ProgressController *progress = nullptr ) override;

    virtual std::string getRowLabel( TObjectOrder whichRow ) const override;
    virtual std::string getColumnLabel( THistogramColumn whichColumn ) const override;
    virtual std::string getPlaneLabel( THistogramColumn whichPlane ) const override;
    virtual bool getShortLabels() const override;
    virtual void setShortLabels( bool newValue ) override;

    virtual THistogramColumn getPlaneColumns( THistogramColumn iPlane,
                                              bool hideEmptyColumns,
                                              std::vector<THistogramColumn> &noVoidSemRanges ) const override;

    // Zoom history
    virtual bool isZoomEmpty() const override;
    virtual bool emptyPrevZoom() const override;
    virtual bool emptyNextZoom() const override;
    virtual void nextZoom() override;
    virtual void prevZoom() override;
    virtual void addZoom( TZoomInfo columnInfo, TZoomInfo dummy,
                          TObjectOrder beginObject, TObjectOrder endObject ) override;
    virtual void addZoom( TZoomInfo columnInfo, TZoomInfo dummy ) override;
    virtual void addZoom( TObjectOrder beginObject, TObjectOrder endObject ) override;
    virtual void setZoomFirstDimension( std::pair<TZoomInfo, TZoomInfo> &zinfo ) override;
    virtual void setZoomSecondDimension( std::pair<TObjectOrder, TObjectOrder> &objects ) override;
    virtual std::pair<TZoomInfo, TZoomInfo> getZoomFirstDimension() const override;
    virtual std::pair<TObjectOrder, TObjectOrder> getZoomSecondDimension() const override;

    virtual void addToSyncGroup( TGroupId whichGroup ) override;
    virtual void removeFromSync() override;
    virtual bool isSync() const override;
    virtual TGroupId getSyncGroup() const override;

    virtual void setName( const std::string& whichName ) override;
    virtual std::string getName() const override;

    virtual void setCalculateAll( bool status ) override;
    virtual bool getCalculateAll() const override;

    virtual bool getIdStat( const std::string& whichStat, PRV_UINT16& idStat ) const override;

    virtual void setCurrentStat( const std::string& whichStat ) override;
    virtual std::string getCurrentStat() const override;

    bool isCommunicationStat( const std::string& whichStat ) const override;

    bool isNotZeroStat( const std::string& whichStat ) const override;

    THistogramColumn getSemanticRealColumn( THistogramColumn whichCol, const std::vector<THistogramColumn>& noVoidSemRanges ) const override;

    std::string getUnitsLabel( const std::string& whichStat ) const override;
    virtual void getGroupsLabels( std::vector<std::string>& onVector ) const override;
    virtual void getStatisticsLabels( std::vector<std::string>& onVector,
                                      PRV_UINT32 whichGroup,
                                      bool getOriginalList = true ) const override;
    virtual std::string getFirstStatistic() const override;
    virtual std::string getFirstCommStatistic() const override;

    virtual bool getControlOutOfLimits() const override;
    virtual bool getExtraOutOfLimits() const override;

    virtual TTimeUnit getTimeUnit() const override;

    virtual Histogram *clone() override;

    virtual bool getDestroy() const override;
    virtual void setDestroy( bool newValue ) override;
    virtual PRV_UINT16 getPosX() const override;
    virtual void setPosX( PRV_UINT16 whichPos ) override;
    virtual PRV_UINT16 getPosY() const override;
    virtual void setPosY( PRV_UINT16 whichPos ) override;
    virtual PRV_UINT16 getWidth() const override;
    virtual void setWidth( PRV_UINT16 whichPos ) override;
    virtual PRV_UINT16 getHeight() const override;
    virtual void setHeight( PRV_UINT16 whichPos ) override;
    virtual bool getShowWindow() const override;
    virtual void setShowWindow( bool newValue ) override;
    virtual DrawModeMethod getDrawModeObjects() const override;
    virtual void setDrawModeObjects( DrawModeMethod whichMethod ) override;
    virtual DrawModeMethod getDrawModeColumns() const override;
    virtual void setDrawModeColumns( DrawModeMethod whichMethod ) override;
    virtual bool getChanged() const override;
    virtual void setChanged( bool newValue ) override;
    virtual bool getRedraw() const override;
    virtual void setRedraw( bool newValue ) override;
    virtual bool getRecalc() const override;
    virtual void setRecalc( bool newValue ) override;
    virtual bool getForceRecalc() const override;
    virtual void setForceRecalc( bool newValue ) override;
    // DEPRECATED
    virtual bool getCodeColor() const override;
    // DEPRECATED
    virtual TColorFunction getColorMode() const override;
    virtual void setColorMode( TColorFunction whichMode ) override;
    virtual void setCodeColor( bool newValue ) override;
    virtual PRV_UINT16 getPixelSize() const override;
    virtual void setPixelSize( PRV_UINT16 whichSize ) override;
    virtual void setOnlyTotals( bool newValue ) override;
    virtual bool getOnlyTotals() const override;
    virtual bool getShowProgressBar() const override;

    // CFG4D
    // Enabled => mode = false
    virtual void setCFG4DEnabled( bool enabled ) override;
    virtual bool getCFG4DEnabled() const override;

    // If CFG4D is enabled, mode can be changed:
    //   false => no CFG4D replacement
    //   true  => CFG4D replacement
    virtual void setCFG4DMode( bool mode ) override;
    virtual bool getCFG4DMode() const override;

    virtual bool existsCFG4DAlias( const std::string &property ) const override; // DEPRECATED
    virtual bool existsCFG4DAlias( const THistogramProperties &propertyIndex ) const override;

    virtual void setCFG4DAlias( const std::string &property, const std::string &alias ) override;
    virtual void setCFG4DStatisticAlias( const std::string &statistic, const std::string &alias ) override;

    virtual std::string getCFG4DAlias( const std::string &property ) const override; // DEPRECATED
    virtual std::string getCFG4DAlias( const THistogramProperties &propertyIndex ) const override;

    virtual void setCFG4DAliasList( const std::map< std::string, std::string >& aliasList ) override;
    virtual void setCFG4DStatisticsAliasList( const std::map< std::string, std::string >& aliasList ) override;
    virtual const std::map< std::string, std::string > getCFG4DAliasList() const override;
    virtual const std::map< std::string, std::string > getCFG4DStatisticsAliasList() const override;

    virtual const std::vector< std::string > getCFG4DFullTagList() override;

    virtual void setCFGS4DGroupLink( std::string originalName, TCFGS4DGroup whichGroup ) override;
    virtual TCFGS4DGroup getCFGS4DGroupLink( std::string originalName ) const override;

    virtual void setCFGS4DIndexLink( TCFGS4DIndexLink whichLink ) override;
    virtual TCFGS4DIndexLink getCFGS4DIndexLink() const override;
    
    virtual SelectionManagement< TObjectOrder, TTraceLevel > * getRowSelectionManagement() override;
    virtual void setRowSelectionManager( SelectionManagement< TObjectOrder, TTraceLevel > &rowSel ) override;

    virtual std::vector< TObjectOrder > getSelectedRows() const override;
    virtual std::vector< TObjectOrder > getSelectedRows( TObjectOrder whichBeginRow, TObjectOrder whichEndRow ) const override;
    virtual std::vector< bool > getSelectedBooleanRows() const override;
    virtual void setSelectedRows( std::vector< bool > &selected ) override;
    virtual void setSelectedRows( std::vector< TObjectOrder > &selected ) override;

    virtual std::vector<int> getCurrentSemanticSort() const override;
    virtual void setCurrentSemanticSort( const std::vector<int>& whichSort ) override;

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
    THistoSortCriteria semanticSortCriteria;
    bool sortSemanticReverse;
    std::vector<int> currentSemanticSort;
    std::vector<int> customSemanticSort;
    bool fixedSemanticSort;
    THistoSortCriteria lastSortCriteria = THistoSortCriteria::TOTAL;

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
    TColorFunction colorMode;
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
    Timeline *controlWindow;
    Timeline *dataWindow;
    Timeline *extraControlWindow;
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
    std::map< std::string, TCFGS4DGroup > groupLinkFromPropName;
    TCFGS4DIndexLink globalIndexLink;

    //Selection of rows
    SelectionManagement< TObjectOrder, TTraceLevel > rowSelection;

    HistogramProxy( KernelConnection *whichKernel );

    void fillSemanticSort();

    friend Histogram *Histogram::create( KernelConnection * );
};



