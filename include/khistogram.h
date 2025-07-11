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
#include "cubecontainer.h"
#include "functionmanagement.h"
#include "khistogramtotals.h"
#include "histogram.h"
#include "histogramstatistic.h"
#include "memorytrace.h"
#include "recordlist.h"
#include "semanticderivedhistogram.h"
#include "semanticinfo.h"



// #ifdef PARALLEL_ENABLED
#include "cubebuffer.h"
// #endif

using TColumnsMergeMode = uint16_t;

// Related to min - max
constexpr TColumnsMergeMode MERGE_MIN_MAX           = 0x0001;
constexpr TColumnsMergeMode KEEP_MIN_MAX_AS_PARENT1 = 0x0002;

// Related to delta
//constexpr TColumnsMergeMode DELTA_1                 = 0x0010; //?
constexpr TColumnsMergeMode RECOMPUTE_DELTA_PARENT2 = 0x0010; //?
constexpr TColumnsMergeMode KEEP_DELTA_AS_PARENT1   = 0x0020;

// Maybe not necessary
//constexpr TColumnsMergeMode KEEP_COLUMNS_AS_PARENT1 = 0x0100;
//constexpr TColumnsMergeMode RECOMPUTE_COLUMNS       = 0x0200;

constexpr TColumnsMergeMode DISCRETE_MAXIMUM_EXPANSION = MERGE_MIN_MAX + KEEP_DELTA_AS_PARENT1;
// constexpr TColumnsMergeMode AS_PARENT1                 = KEEP_MIN_MAX_AS_PARENT1 + KEEP_DELTA_AS_PARENT1;
constexpr TColumnsMergeMode CONTINUOUS_FIXED_PARENT1   = MERGE_MIN_MAX + RECOMPUTE_DELTA_PARENT2;


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
  THistogramColumn plane = 0;
  THistogramColumn column;

  TObjectOrder controlRow;
  TObjectOrder dataRow;

  TRecordTime beginTime;
  TRecordTime endTime;

  RecordList *rList;
  RecordList::iterator comm;
};


/***************************************************************
***                     KHistogram                           ***
****************************************************************/
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

    void setUseFixedDelta( bool whichValue ) override;
    void setControlMin( THistogramLimit whichMin ) override;
    void setControlMax( THistogramLimit whichMax ) override;
    void setControlDelta( THistogramLimit whichDelta ) override;
    void setExtraControlMin( THistogramLimit whichMin ) override;
    void setExtraControlMax( THistogramLimit whichMax ) override;
    void setExtraControlDelta( THistogramLimit whichDelta ) override;
    void setDataMin( TSemanticValue whichMin ) override;
    void setDataMax( TSemanticValue whichMax ) override;

    bool getUseFixedDelta() const override;
    THistogramLimit getControlMin() const override;
    THistogramLimit getControlMax() const override;
    THistogramLimit getControlDelta() const override;
    THistogramLimit getExtraControlMin() const override;
    THistogramLimit getExtraControlMax() const override;
    THistogramLimit getExtraControlDelta() const override;
    TSemanticValue getDataMin() const override;
    TSemanticValue getDataMax() const override;

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

    virtual bool isDerivedHistogram() const override;
    
    ColumnTranslator *getColumnTranslator() const;
    ColumnTranslator *getPlaneTranslator() const;

    virtual void setReady( bool whichReady ) override;
    virtual bool isReady() const override;

  protected:
    TObjectOrder numRows = 0;
    THistogramColumn numCols = 0;
    THistogramColumn numPlanes = 0;

    Cube<TSemanticValue, NUM_SEMANTIC_STATS> *cube = nullptr;
    Matrix<TSemanticValue, NUM_SEMANTIC_STATS> *matrix = nullptr;
    Cube<TSemanticValue, NUM_COMM_STATS> *commCube = nullptr;
    Matrix<TSemanticValue, NUM_COMM_STATS> *commMatrix = nullptr;

#ifdef PARALLEL_ENABLED
    CubeBuffer<NUM_SEMANTIC_STATS> *semanticBuffer = nullptr;
    CubeBuffer<NUM_COMM_STATS> *commBuffer = nullptr;
#endif

    KHistogramTotals *totals = nullptr;
    KHistogramTotals *rowTotals = nullptr;
    KHistogramTotals *commTotals = nullptr;
    KHistogramTotals *rowCommTotals = nullptr;

    Statistics statistics;

    //Selection of rows
    SelectionManagement< TObjectOrder, TWindowLevel > rowSelection;
    std::vector<KTimeline *> orderedWindows;

    bool createComms() const;
    void initTranslators();
    void initTotals();
    void initMatrix( THistogramColumn planes, THistogramColumn cols, TObjectOrder rows );

  private:
    KTimeline *controlWindow = nullptr;
    KTimeline *dataWindow = nullptr;
    KTimeline *xtraControlWindow = nullptr;

    TRecordTime beginTime;
    TRecordTime endTime;

    bool useFixedDelta;

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

    RowsTranslator *rowsTranslator = nullptr;
    ColumnTranslator *columnTranslator = nullptr;
    ColumnTranslator *planeTranslator = nullptr;

    TTimeUnit myTimeUnit;

    WindowCloneManager windowCloneManager;
   
    bool ready = false;

    void orderWindows();
    void clearTranslators();
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


/***************************************************************
***                     KDerivedHistogram                    ***
****************************************************************/


using TPlaneOrder = THistogramColumn;

struct TRemoteIndex
{
  TPlaneOrder plane;
  TObjectOrder row;
  THistogramColumn column;
};

class KDerivedHistogram : public KHistogram
{
  public:
    KDerivedHistogram( std::vector< KHistogram * >& whichParents );
    ~KDerivedHistogram();

    bool getThreeDimensions() const override;

    // TRecordTime getBeginTime() const override;
    // TRecordTime getEndTime() const override;

    Timeline *getControlWindow() const override;
    Timeline *getDataWindow() const override;
    Timeline *getExtraControlWindow() const override;
    void setControlWindow( Timeline *whichWindow ) override;
    void setDataWindow( Timeline *whichWindow ) override;
    void setExtraControlWindow( Timeline *whichWindow ) override;
    void clearControlWindow() override;
    void clearDataWindow() override;
    void clearExtraControlWindow() override;

    void setUseFixedDelta( bool whichValue ) override;
    void setControlMin( THistogramLimit whichMin ) override;
    void setControlMax( THistogramLimit whichMax ) override;
    void setControlDelta( THistogramLimit whichDelta ) override;
    void setExtraControlMin( THistogramLimit whichMin ) override;
    void setExtraControlMax( THistogramLimit whichMax ) override;
    void setExtraControlDelta( THistogramLimit whichDelta ) override;
    void setDataMin( TSemanticValue whichMin ) override;
    void setDataMax( TSemanticValue whichMax ) override;

    // bool getUseFixedDelta() const override;
    // THistogramLimit getControlMin() const override;
    // THistogramLimit getControlMax() const override;
    // THistogramLimit getControlDelta() const override;
    // THistogramLimit getExtraControlMin() const override;
    // THistogramLimit getExtraControlMax() const override;
    // THistogramLimit getExtraControlDelta() const override;
    TSemanticValue getDataMin() const override;
    TSemanticValue getDataMax() const override;

    // bool getInclusiveEnabled() const override;
    void setInclusive( bool newValue ) override;
    // bool getInclusive() const override;

    void setNumColumns( THistogramColumn whichNumColumns ) override;

    THistogramColumn getNumPlanes() const override;
    THistogramColumn getNumColumns() const override;
    THistogramColumn getCommNumColumns() const override;

    // TObjectOrder getNumRows() const override;

    // TSemanticValue getCurrentValue( PRV_UINT32 col,
    //                                 PRV_UINT16 idStat,
    //                                 PRV_UINT32 plane = 0 ) const override;
    // PRV_UINT32 getCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const override;
    // void setNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    // void setFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    // bool endCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    // bool planeWithValues( PRV_UINT32 plane = 0 ) const override;
    // bool getCellValue( TSemanticValue& semVal,
    //                    PRV_UINT32 whichRow,
    //                    PRV_UINT32 whichCol,
    //                    PRV_UINT16 idStat,
    //                    PRV_UINT32 whichPlane = 0 ) const override;
    // bool getNotZeroValue( PRV_UINT32 whichRow,
    //                       PRV_UINT32 whichCol,
    //                       PRV_UINT16 idStat,
    //                       PRV_UINT32 whichPlane = 0 ) const override;

    // TSemanticValue getCommCurrentValue( PRV_UINT32 col,
    //                                     PRV_UINT16 idStat,
    //                                     PRV_UINT32 plane = 0 ) const override;
    // PRV_UINT32 getCommCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const override;
    // void setCommNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    // void setCommFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    // bool endCommCell( PRV_UINT32 col, PRV_UINT32 plane = 0 ) override;
    // bool planeCommWithValues( PRV_UINT32 plane = 0 ) const override;
    // bool getCommCellValue( TSemanticValue& semVal,
    //                        PRV_UINT32 whichRow,
    //                        PRV_UINT32 whichCol,
    //                        PRV_UINT16 idStat,
    //                        PRV_UINT32 whichPlane = 0 ) const override;

    // HistogramTotals *getColumnTotals() const override;
    // HistogramTotals *getCommColumnTotals() const override;
    // HistogramTotals *getRowTotals() const override;
    // HistogramTotals *getCommRowTotals() const override;

    void clearStatistics() override;
    void pushbackStatistic( const std::string& whichStatistic ) override;

    // bool isCommunicationStat( const std::string& whichStat ) const override;

    // bool isNotZeroStat( const std::string& whichStat ) const override;

    std::string getUnitsLabel( const std::string& whichStat ) const override;

    void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime,
                  std::vector<TObjectOrder>& selectedRows, ProgressController *progress ) override;

    bool getControlOutOfLimits() const override;
    bool getExtraOutOfLimits() const override;

    TTimeUnit getTimeUnit() const override;

    virtual KHistogram *clone() override;

    
    // Derived histogram specific methods
    //virtual Histogram *getParent( PRV_UINT16 whichParent ) const override;
    virtual bool isDerivedHistogram() const override;

    virtual void setDerivedOperation( const std::string& whichOperation ) override;
    virtual std::string getDerivedOperation() const override;

    virtual void getDerivedOperationGroupsLabels( std::vector<std::string>& onVector ) const override;
    virtual void getDerivedOperationLabels( std::vector<std::string>& onVector,
                                            PRV_UINT32 whichGroup,
                                            bool getOriginalList = true ) const override;
    virtual void getDerivedOperationLabelsAndSymbols( std::map<std::string, std::string>& onVector,
                                                      PRV_UINT32 whichGroup,
                                                      bool getOriginalList = true ) const override;

    void setColumnsMergeMode( TColumnsMergeMode whichMode );
    TColumnsMergeMode getColumnsMergeMode() const;  


  protected:

  private:
    const size_t MAIN = 0;
    std::vector< KHistogram * > parents = {};

    std::string currentDerivedOperation = "add";

    //std::unordered_map< PRV_UINT32, PRV_UINT32 > currentRow;
    
    // TODO: Is a better idea thah CubeContainer? May vary columns depending on the plane?
    // std::unordered_map< THistogramColumn, THistogramColumn > planeCorrespondence;
    // std::unordered_map< TObjectOrder, TObjectOrder > rowCorrespondence;
    // std::unordered_map< THistogramColumn, THistogramColumn > colCorrespondence;
    using THistogramCorrespondenceInfo = CubeContainer< TPlaneOrder, TObjectOrder, THistogramColumn, TRemoteIndex >;
    THistogramCorrespondenceInfo cellCorrespondence;
    THistogramCorrespondenceInfo cellCommCorrespondence;


    void fillCellCorrespondence();
    bool getCellCorrespondence( THistogramCorrespondenceInfo& whichCellCorrespondence,
                                THistogramColumn whichPlane, TObjectOrder whichRow, THistogramColumn whichColumn,
                                THistogramCorrespondenceInfo::iterator& whichIt );

    TColumnsMergeMode columnsMergeMode = DISCRETE_MAXIMUM_EXPANSION;

    template <size_t NUM_STATS>
    void combineValues( std::array< TSemanticValue, NUM_STATS >& wholeSemVals,
                        KHistogramTotals* whichTotals, KHistogramTotals* whichRowTotals, 
                        THistogramColumn iPlane, THistogramColumn iCol, THistogramColumn iRow,
                        THistogramColumn i2Plane = 0, THistogramColumn i2Col = 0, THistogramColumn i2Row = 0,
                        bool existCorrespondence = false );

    // template< std::function< void( THistogramColumn, THistogramColumn ) > FSetFirstCell,
    //           std::function< bool( THistogramColumn, THistogramColumn ) > FEndCell,
    //           std::function< PRV_INT32( THistogramColumn, THistogramColumn ) > FGetCurrentRow,
    // template< class FSetFirstCell,
    //           class FSetParentFirstCell,
    //           class FEndCell,
    //           class FGetCurrentRow,
    //           class TMatrix,
    //           class TCube >
    // bool startFirstCell( FSetFirstCell whichThisSetFirstCell,
    //                      FSetParentFirstCell whichParentSetFirstCell,
    //                      FEndCell whichParentEndCell,
    //                      FGetCurrentRow whichParentGetCurrentRow,
    //                      PRV_INT32& iRow,
    //                      THistogramColumn iCol,
    //                      THistogramColumn iPlane,
    //                      TMatrix *matrix,
    //                      TCube *cube );
 
    void combineHistograms();

    void orderWindows();
    void initStatistics();

    void mergeColumns( TRecordTime whichBeginTime, TRecordTime whichEndTime,
                       std::vector<TObjectOrder>& selectedRows, ProgressController *progress );
};


template <size_t NUM_STATS>
void KDerivedHistogram::combineValues( std::array< TSemanticValue, NUM_STATS >& wholeSemVals,
                                       KHistogramTotals* whichTotals, KHistogramTotals* whichRowTotals, 
                                       THistogramColumn iPlane, THistogramColumn iCol, THistogramColumn iRow,
                                       THistogramColumn i2Plane, THistogramColumn i2Col, THistogramColumn i2Row,
                                       bool existCorrespondence )
{
  TSemanticValue semVal1 = 0.0;
  TSemanticValue semVal2 = 0.0;
  std::vector< TSemanticValue > semValues;

  DerivedHistogramFunctionInfo tmpValues;
  SemanticDerivedHistogram *op = FunctionManagement<SemanticDerivedHistogram>::getInstance()->getFunction( getDerivedOperation() );

  // auto findAllValues = []()
  // {
  //   foundVal1 = parents[ MAIN ]->getCellValue( semVal1, iRow, iCol, currentStat, iPlane );
  //   if ( existCorrespondence )
  //     foundVal2 = parents[ 1 ]->getCellValue( semVal2, i2Row, i2Col, currentStat, i2Plane );
  // };


  // TODO: hay una llamada que recibe el vector
  for ( size_t currentStat = 0; currentStat < NUM_STATS; ++currentStat )
   {
    //TODO:  Refactor1 -> template to separate comm statistics.
    //TODO:  Refactor2 -> lambda to compute foundValues.

    bool foundVal1 = false;
    bool foundVal2 = false;
    if constexpr( NUM_STATS == NUM_SEMANTIC_STATS )
    {
      foundVal1 = parents[ MAIN ]->getCellValue( semVal1, iRow, iCol, currentStat, iPlane );
      if ( existCorrespondence )
        foundVal2 = parents[ 1 ]->getCellValue( semVal2, i2Row, i2Col, currentStat, i2Plane );

      // alg1: all of them
      // alg2: any of them
    }
    else
    {
      foundVal1 = parents[ MAIN ]->getCommCellValue( semVal1, iRow, iCol, currentStat, iPlane );
      if ( existCorrespondence )
        foundVal2 = parents[ 1 ]->getCommCellValue( semVal2, i2Row, i2Col, currentStat, i2Plane );
    }

    if ( !foundVal1 && !foundVal2 )
      continue;

    tmpValues.values = { semVal1, semVal2 };

    // wholeSemVals[ currentStat ] = op->execute( &tmpValues );
    TSemanticValue result = op->execute( &tmpValues );
    wholeSemVals[ currentStat ] = result;
    //if (iRow == 0 && currentStat == 0)
    //  std::cout << "(v1,v2)[ "<<  iPlane <<", " << iCol << ", " << iRow << ", " << currentStat << " ] = (" << semVal1 << "," << semVal2 << ") = "<< result << std::endl;

    // TODO: this is kind of mixing semantic/comm detection with no detection at all --> to unite
    if constexpr( NUM_STATS == NUM_SEMANTIC_STATS )
    {
      totals->newValue( wholeSemVals[ currentStat ], currentStat, iCol, iPlane );
      rowTotals->newValue( wholeSemVals[ currentStat ], currentStat, iRow, iPlane );
    }
    else
    {
      commTotals->newValue( wholeSemVals[ currentStat ], currentStat, iCol, iPlane );
      rowCommTotals->newValue( wholeSemVals[ currentStat ], currentStat, iRow, iPlane );
    }
  }
};

// template< std::function< void( THistogramColumn, THistogramColumn ) > FSetFirstCell,
//           std::function< bool( THistogramColumn, THistogramColumn ) > FEndCell,
//           std::function< PRV_INT32( THistogramColumn, THistogramColumn ) > FGetCurrentRow,
// template< class FSetFirstCell,
//           class FSetParentFirstCell,
//           class FEndCell,
//           class FGetCurrentRow,
//           class TMatrix,
//           class TCube >
// bool KDerivedHistogram::startFirstCell( FSetFirstCell whichThisSetFirstCell,
//                                         FSetParentFirstCell whichParentSetFirstCell,
//                                         FEndCell whichParentEndCell,
//                                         FGetCurrentRow whichParentGetCurrentRow,
//                                         PRV_INT32& iRow,
//                                         THistogramColumn iCol,
//                                         THistogramColumn iPlane,
//                                         TMatrix *matrix,
//                                         TCube *cube )
// {
//   // parents[ MAIN ]->whichSetFirstCell( iCol, iPlane );
//   // whichParentSetFirstCell( iCol, iPlane );
//   whichParentSetFirstCell();

//   // bool isEndCell = parents[ MAIN ]->whichEndCell( iCol, iPlane );
//   bool isEndCell = whichParentEndCell();
//   if ( !isEndCell )
//   {
//     // iRow = parents[ MAIN ]->whichGetCurrentRow( iCol, iPlane );
//     iRow = whichParentGetCurrentRow();

//     if ( getThreeDimensions() )
//       cube->newRow( iPlane, iCol, iRow );
//     else
//       matrix->newRow( iCol, iRow );

//     whichThisSetFirstCell();
//   }

//   return isEndCell;
// };
