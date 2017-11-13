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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef KHISTOGRAM_H_INCLUDED
#define KHISTOGRAM_H_INCLUDED

#include "cube.h"
#include "histogram.h"
#include "memorytrace.h"
#include "recordlist.h"
#include "histogramstatistic.h"

class KHistogramTotals;
class KWindow;
#ifdef PARALLEL_ENABLED
class CubeBuffer;
#endif

class RowsTranslator
{
  public:
    /*
        PRECOND1: windows std::vector comes downwards level ordered.
        PRECOND2: KWindows in window std::vector have same hierarchy.
        PRECOND3: windows std::vector size is 2 or 3 KWindows.
    */
    RowsTranslator( const RowsTranslator& source );
    RowsTranslator( std::vector<KWindow *>& windows );
    ~RowsTranslator();

    TObjectOrder globalTranslate( PRV_UINT16 winIndex,
                                  TObjectOrder rowIndex ) const;
    void getRowChilds( PRV_UINT16 winIndex,
                       TObjectOrder rowIndex,
                       TObjectOrder& iniRow,
                       TObjectOrder& endRow ) const;
    TObjectOrder totalRows() const;

    RowsTranslator *clone();

  protected:

  private:
    struct RowChildInfo
    {
      bool oneToOne;
      std::vector< std::pair< TObjectOrder, TObjectOrder > > rowChilds;
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
    ~ColumnTranslator();

    bool getColumn( THistogramLimit whichValue,
                    THistogramColumn& column ) const;
    THistogramColumn totalColumns() const;

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

    bool getThreeDimensions() const;

    TRecordTime getBeginTime() const;
    TRecordTime getEndTime() const;

    Window *getControlWindow() const;
    Window *getDataWindow() const;
    Window *getExtraControlWindow() const;
    void setControlWindow( Window *whichWindow );
    void setDataWindow( Window *whichWindow );
    void setExtraControlWindow( Window *whichWindow );
    void clearControlWindow();
    void clearDataWindow();
    void clearExtraControlWindow();

    void setControlMin( THistogramLimit whichMin );
    void setControlMax( THistogramLimit whichMax );
    void setControlDelta( THistogramLimit whichDelta );
    void setExtraControlMin( THistogramLimit whichMin );
    void setExtraControlMax( THistogramLimit whichMax );
    void setExtraControlDelta( THistogramLimit whichDelta );
    void setDataMin( TSemanticValue whichMin );
    void setDataMax( TSemanticValue whichMax );
    void setBurstMin( TRecordTime whichTime );
    void setBurstMax( TRecordTime whichTime );
    void setCommSizeMin( TCommSize whichSize );
    void setCommSizeMax( TCommSize whichSize );
    void setCommTagMin( TCommTag whichTag );
    void setCommTagMax( TCommTag whichTag );


    THistogramLimit getControlMin() const;
    THistogramLimit getControlMax() const;
    THistogramLimit getControlDelta() const;
    THistogramLimit getExtraControlMin() const;
    THistogramLimit getExtraControlMax() const;
    THistogramLimit getExtraControlDelta() const;
    TSemanticValue getDataMin() const;
    TSemanticValue getDataMax() const;
    TRecordTime getBurstMin() const;
    TRecordTime getBurstMax() const;
    TCommSize getCommSizeMin() const;
    TCommSize getCommSizeMax() const;
    TCommTag getCommTagMin() const;
    TCommTag getCommTagMax() const;

    bool getInclusiveEnabled() const;
    void setInclusive( bool newValue );
    bool getInclusive() const;

    THistogramColumn getNumPlanes() const;
    THistogramColumn getNumColumns() const;
    TObjectOrder getNumRows() const;

    TSemanticValue getCurrentValue( PRV_UINT32 col,
                                    PRV_UINT16 idStat,
                                    PRV_UINT32 plane = 0 ) const;
    PRV_UINT32 getCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const;
    void setNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    void setFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    bool endCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    bool planeWithValues( PRV_UINT32 plane = 0 ) const;
    bool getCellValue( TSemanticValue& semVal,
                       PRV_UINT32 whichRow,
                       PRV_UINT32 whichCol,
                       PRV_UINT16 idStat,
                       PRV_UINT32 whichPlane = 0 ) const;

    TSemanticValue getCommCurrentValue( PRV_UINT32 col,
                                        PRV_UINT16 idStat,
                                        PRV_UINT32 plane = 0 ) const;
    PRV_UINT32 getCommCurrentRow( PRV_UINT32 col, PRV_UINT32 plane = 0 ) const;
    void setCommNextCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    void setCommFirstCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    bool endCommCell( PRV_UINT32 col, PRV_UINT32 plane = 0 );
    bool planeCommWithValues( PRV_UINT32 plane = 0 ) const;
    bool getCommCellValue( TSemanticValue& semVal,
                           PRV_UINT32 whichRow,
                           PRV_UINT32 whichCol,
                           PRV_UINT16 idStat,
                           PRV_UINT32 whichPlane = 0 ) const;

    HistogramTotals *getColumnTotals() const;
    HistogramTotals *getCommColumnTotals() const;
    HistogramTotals *getRowTotals() const;
    HistogramTotals *getCommRowTotals() const;

    void clearStatistics();
    void pushbackStatistic( const std::string& whichStatistic );

    bool itsCommunicationStat( const std::string& whichStat ) const;

    std::string getUnitsLabel( const std::string& whichStat ) const;

    void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime,
                  std::vector<TObjectOrder>& selectedRows, ProgressController *progress );

    void getGroupsLabels( std::vector<std::string>& onVector ) const;
    void getStatisticsLabels( std::vector<std::string>& onVector,
                              PRV_UINT32 whichGroup,
                              bool dummy ) const;
    std::string getFirstStatistic() const;
    std::string getFirstCommStatistic() const;

    bool getControlOutOfLimits() const;
    bool getExtraOutOfLimits() const;

    TTimeUnit getTimeUnit() const;

    virtual KHistogram *clone();

  protected:

  private:
    KWindow *controlWindow;
    KWindow *dataWindow;
    KWindow *xtraControlWindow;

    TRecordTime beginTime;
    TRecordTime endTime;

    TObjectOrder numRows;
    THistogramColumn numCols;
    THistogramColumn numPlanes;

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

    std::vector<KWindow *> orderedWindows;
    RowsTranslator *rowsTranslator;
    ColumnTranslator *columnTranslator;
    ColumnTranslator *planeTranslator;

    Cube<TSemanticValue> *cube;
    Matrix<TSemanticValue> *matrix;
    Cube<TSemanticValue> *commCube;
    Matrix<TSemanticValue> *commMatrix;

#ifdef PARALLEL_ENABLED
    CubeBuffer *semanticBuffer;
    CubeBuffer *commBuffer;
#endif

    KHistogramTotals *totals;
    KHistogramTotals *rowTotals;
    KHistogramTotals *commTotals;
    KHistogramTotals *rowCommTotals;

    TTimeUnit myTimeUnit;

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
                             PRV_UINT16 winIndex = 0, CalculateData *data = NULL );
    void calculate( TObjectOrder iRow,
                    TRecordTime fromTime, TRecordTime toTime,
                    PRV_UINT16 winIndex, CalculateData *data,
                    std::vector<bool>& needInit,
                    bool calcSemanticStats );
    void finishRow( CalculateData *data );
};


#endif // KHISTOGRAM_H_INCLUDED
