/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
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


#include <unordered_map>

#include "cube.h"
#include "histogram.h"
#include "memorytrace.h"
#include "recordlist.h"
#include "histogramstatistic.h"

#ifdef PARALLEL_ENABLED
#include "cubebuffer.h"
#endif

class KHistogramTotals;
class KTimeline;


class WindowCloneManager
{
  public:
    WindowCloneManager();
    ~WindowCloneManager();

    Timeline *operator()( Timeline *originalWindow ) const;
#ifdef PARALLEL_ENABLED
    void update( const KHistogram *whichHistogram );
    void clear();

  private:
    std::unordered_map< Timeline *, std::vector< Timeline * > > clonedWindows;

    void clone( Timeline *whichWindow );
#endif
};


class RowsTranslator
{
  public:
    /*
        PRECOND1: windows std::vector comes downwards level ordered.
        PRECOND2: KWindows in window std::vector have same hierarchy.
        PRECOND3: windows std::vector size is 2 or 3 KWindows.
    */
    RowsTranslator( const RowsTranslator& source );
    RowsTranslator( std::vector<KTimeline *>& windows );
    ~RowsTranslator();

    TObjectOrder globalTranslate( PRV_UINT16 winIndex,
                                  TObjectOrder rowIndex ) const;
    void getRowChildren( PRV_UINT16 winIndex,
                       TObjectOrder rowIndex,
                       TObjectOrder& iniRow,
                       TObjectOrder& endRow ) const;
    TObjectOrder totalRows() const;

  protected:

  private:
    struct RowChildInfo
    {
      bool oneToOne;
      std::vector< std::pair< TObjectOrder, TObjectOrder > > rowChildren;
      TObjectOrder numRows;
    };

    std::vector< RowChildInfo > childInfo;
};


class ColumnTranslator
{
  public:
    ColumnTranslator( const ColumnTranslator& source );
    ColumnTranslator( THistogramLimit whichMin, THistogramLimit whichMax,
                      THistogramLimit whichDelta );
    ColumnTranslator( THistogramLimit whichMin, THistogramLimit whichMax,
                      THistogramColumn whichNumColumns );
    ~ColumnTranslator();

    bool getColumn( THistogramLimit whichValue,
                    THistogramColumn& column ) const;
    THistogramColumn totalColumns() const;
    THistogramLimit getDelta() const;

  protected:

  private:
    THistogramColumn numColumns;
    THistogramLimit minLimit;
    THistogramLimit maxLimit;
    THistogramLimit delta;
};


struct CalculateData
{
  TObjectOrder row;
  THistogramColumn plane;
  THistogramColumn column;

  TObjectOrder controlRow;
  TObjectOrder dataRow;

  TRecordTime beginTime;
  TRecordTime endTime;

  RecordList *rList;
  RecordList::iterator comm;
};


class KHistogram : public Histogram
{
  public:
    KHistogram();
    ~KHistogram();

    bool getThreeDimensions() const override;

    TRecordTime getBeginTime() const override;
    TRecordTime getEndTime() const override;

    Timeline *getControlWindow() const override;
    Timeline *getDataWindow() const override;
    Timeline *getExtraControlWindow() const override;
    void setControlWindow( Timeline *whichWindow ) override;
    void setDataWindow( Timeline *whichWindow ) override;
    void setExtraControlWindow( Timeline *whichWindow ) override;
    void clearControlWindow() override;
    void clearDataWindow() override;
    void clearExtraControlWindow() override;

    void setUseCustomDelta( bool whichValue ) override;
    void setControlMin( THistogramLimit whichMin ) override;
    void setControlMax( THistogramLimit whichMax ) override;
    void setControlDelta( THistogramLimit whichDelta ) override;
    void setExtraControlMin( THistogramLimit whichMin ) override;
    void setExtraControlMax( THistogramLimit whichMax ) override;
    void setExtraControlDelta( THistogramLimit whichDelta ) override;
    void setDataMin( TSemanticValue whichMin ) override;
    void setDataMax( TSemanticValue whichMax ) override;
    void setBurstMin( TRecordTime whichTime ) override;
    void setBurstMax( TRecordTime whichTime ) override;
    void setCommSizeMin( TCommSize whichSize ) override;
    void setCommSizeMax( TCommSize whichSize ) override;
    void setCommTagMin( TCommTag whichTag ) override;
    void setCommTagMax( TCommTag whichTag ) override;


    bool getUseCustomDelta() const override;
    THistogramLimit getControlMin() const override;
    THistogramLimit getControlMax() const override;
    THistogramLimit getControlDelta() const override;
    THistogramLimit getExtraControlMin() const override;
    THistogramLimit getExtraControlMax() const override;
    THistogramLimit getExtraControlDelta() const override;
    TSemanticValue getDataMin() const override;
    TSemanticValue getDataMax() const override;
    TRecordTime getBurstMin() const override;
    TRecordTime getBurstMax() const override;
    TCommSize getCommSizeMin() const override;
    TCommSize getCommSizeMax() const override;
    TCommTag getCommTagMin() const override;
    TCommTag getCommTagMax() const override;

    bool getInclusiveEnabled() const override;
    void setInclusive( bool newValue ) override;
    bool getInclusive() const override;

    void setNumColumns( THistogramColumn whichNumColumns ) override;

    THistogramColumn getNumPlanes() const override;
    THistogramColumn getNumColumns() const override;
    THistogramColumn getCommNumColumns() const override;

    TObjectOrder getNumRows() const override;

    TSemanticValue getCurrentValue( PRV_UINT32 col,
                                    PRV_UINT16 idStat,
                                    PRV_UINT32 plane = 0 ) const override;
    PRV_UINT32 getCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const override;
    void setNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    void setFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    bool endCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    bool planeWithValues( PRV_UINT32 plane = 0 ) const override;
    bool getCellValue( TSemanticValue& semVal,
                       PRV_UINT32 whichRow,
                       PRV_UINT32 whichCol,
                       PRV_UINT16 idStat,
                       PRV_UINT32 whichPlane = 0 ) const override;
    bool getNotZeroValue( PRV_UINT32 whichRow,
                          PRV_UINT32 whichCol,
                          PRV_UINT16 idStat,
                          PRV_UINT32 whichPlane = 0 ) const override;

    TSemanticValue getCommCurrentValue( PRV_UINT32 col,
                                        PRV_UINT16 idStat,
                                        PRV_UINT32 plane = 0 ) const override;
    PRV_UINT32 getCommCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const override;
    void setCommNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    void setCommFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    bool endCommCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    bool planeCommWithValues( PRV_UINT32 plane = 0 ) const override;
    bool getCommCellValue( TSemanticValue& semVal,
                           PRV_UINT32 whichRow,
                           PRV_UINT32 whichCol,
                           PRV_UINT16 idStat,
                           PRV_UINT32 whichPlane = 0 ) const override;

    HistogramTotals *getColumnTotals() const override;
    HistogramTotals *getCommColumnTotals() const override;
    HistogramTotals *getRowTotals() const override;
    HistogramTotals *getCommRowTotals() const override;

    void clearStatistics() override;
    void pushbackStatistic( const std::string& whichStatistic ) override;

    bool isCommunicationStat( const std::string& whichStat ) const override;

    bool isNotZeroStat( const std::string& whichStat ) const override;

    std::string getUnitsLabel( const std::string& whichStat ) const override;

    void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime,
                  std::vector<TObjectOrder>& selectedRows, ProgressController *progress ) override;

    void getGroupsLabels( std::vector<std::string>& onVector ) const override;
    void getStatisticsLabels( std::vector<std::string>& onVector,
                              PRV_UINT32 whichGroup,
                              bool dummy ) const override;
    std::string getFirstStatistic() const override;
    std::string getFirstCommStatistic() const override;

    bool getControlOutOfLimits() const override;
    bool getExtraOutOfLimits() const override;

    TTimeUnit getTimeUnit() const override;

    virtual KHistogram *clone() override;

    Timeline *getClonedWindow( Timeline *whichWindow ) const;

  protected:

  private:
    KTimeline *controlWindow;
    KTimeline *dataWindow;
    KTimeline *xtraControlWindow;

    TRecordTime beginTime;
    TRecordTime endTime;

    TObjectOrder numRows;
    THistogramColumn numCols;
    THistogramColumn numPlanes;

    bool useCustomDelta;

    THistogramLimit controlMin;
    THistogramLimit controlMax;
    THistogramLimit controlDelta;
    THistogramLimit xtraControlMin;
    THistogramLimit xtraControlMax;
    THistogramLimit xtraControlDelta;
    TSemanticValue dataMin;
    TSemanticValue dataMax;
    TRecordTime burstMin;
    TRecordTime burstMax;
    TCommSize commSizeMin;
    TCommSize commSizeMax;
    TCommTag commTagMin;
    TCommTag commTagMax;

    bool controlOutOfLimits;
    std::vector<bool> tmpControlOutOfLimits;
    bool xtraOutOfLimits;
    std::vector<bool> tmpXtraOutOfLimits;

    bool inclusive;

    /*std::vector<HistogramStatistic *> statisticFunctions;
    std::vector<HistogramStatistic *> commStatisticFunctions;*/
    Statistics statistics;

    std::vector<KTimeline *> orderedWindows;
    RowsTranslator *rowsTranslator;
    ColumnTranslator *columnTranslator;
    ColumnTranslator *planeTranslator;

    Cube<TSemanticValue, NUM_SEMANTIC_STATS> *cube;
    Matrix<TSemanticValue, NUM_SEMANTIC_STATS> *matrix;
    Cube<TSemanticValue, NUM_COMM_STATS> *commCube;
    Matrix<TSemanticValue, NUM_COMM_STATS> *commMatrix;

#ifdef PARALLEL_ENABLED
    CubeBuffer<NUM_SEMANTIC_STATS> *semanticBuffer;
    CubeBuffer<NUM_COMM_STATS> *commBuffer;
#endif

    KHistogramTotals *totals;
    KHistogramTotals *rowTotals;
    KHistogramTotals *commTotals;
    KHistogramTotals *rowCommTotals;

    TTimeUnit myTimeUnit;

    WindowCloneManager windowCloneManager;
    
    //Selection of rows
    SelectionManagement< TObjectOrder, TWindowLevel > rowSelection;

    void orderWindows();
    bool createComms() const;
    void initTranslators();
    void initMatrix( THistogramColumn planes, THistogramColumn cols,
                     TObjectOrder rows );
    void initTotals();
    void initSemantic( TRecordTime beginTime );
    void initStatistics();
    void initTmpBuffers( THistogramColumn planes, TObjectOrder rows );
#ifdef PARALLEL_ENABLED
    void finishAllRows();
#endif
    void finishOutLimits();

#ifdef PARALLEL_ENABLED
    void parallelExecution( TRecordTime fromTime, TRecordTime toTime,
                            TObjectOrder fromRow, TObjectOrder toRow,
                            std::vector<TObjectOrder>& selectedRows,
                            ProgressController *progress );

    void executionTask( TRecordTime fromTime, TRecordTime toTime,
                        TObjectOrder fromRow, TObjectOrder toRow,
                        std::vector<TObjectOrder>& selectedRows,
                        ProgressController *progress );
#endif

    void recursiveExecution( TRecordTime fromTime, TRecordTime toTime,
                             TObjectOrder fromRow, TObjectOrder toRow,
                             std::vector<TObjectOrder>& selectedRows,
                             std::vector<bool>& needInit,
                             bool calcSemanticStats,
                             ProgressController *progress,
                             PRV_UINT16 winIndex = 0, CalculateData *data = nullptr );
    void calculate( TObjectOrder iRow,
                    TRecordTime fromTime, TRecordTime toTime,
                    PRV_UINT16 winIndex, CalculateData *data,
                    std::vector<bool>& needInit,
                    bool calcSemanticStats );
    void finishRow( CalculateData *data );
};



