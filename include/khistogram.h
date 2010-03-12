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

class RowsTranslator
{
  public:
    /*
        PRECOND1: windows vector comes downwards level ordered.
        PRECOND2: KWindows in window vector have same hierarchy.
        PRECOND3: windows vector size is 2 or 3 KWindows.
    */
    RowsTranslator( const RowsTranslator& source );
    RowsTranslator( vector<KWindow *>& windows );
    ~RowsTranslator();

    TObjectOrder globalTranslate( UINT16 winIndex,
                                  TObjectOrder rowIndex ) const;
    void getRowChilds( UINT16 winIndex,
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
      vector< pair< TObjectOrder, TObjectOrder > > rowChilds;
      TObjectOrder numRows;
    };

    vector< RowChildInfo > childInfo;
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

    void setInclusive( bool newValue );

    THistogramColumn getNumPlanes() const;
    THistogramColumn getNumColumns() const;
    TObjectOrder getNumRows() const;

    TSemanticValue getCurrentValue( UINT32 col,
                                    UINT16 idStat,
                                    UINT32 plane = 0 ) const;
    UINT32 getCurrentRow( UINT32 col, UINT32 plane = 0 ) const;
    void setNextCell( UINT32 col, UINT32 plane = 0 );
    void setFirstCell( UINT32 col, UINT32 plane = 0 );
    bool endCell( UINT32 col, UINT32 plane = 0 );
    bool planeWithValues( UINT32 plane = 0 ) const;

    TSemanticValue getCommCurrentValue( UINT32 col,
                                        UINT16 idStat,
                                        UINT32 plane = 0 ) const;
    UINT32 getCommCurrentRow( UINT32 col, UINT32 plane = 0 ) const;
    void setCommNextCell( UINT32 col, UINT32 plane = 0 );
    void setCommFirstCell( UINT32 col, UINT32 plane = 0 );
    bool endCommCell( UINT32 col, UINT32 plane = 0 );
    bool planeCommWithValues( UINT32 plane = 0 ) const;

    HistogramTotals *getColumnTotals() const;
    HistogramTotals *getCommColumnTotals() const;
    HistogramTotals *getRowTotals() const;
    HistogramTotals *getCommRowTotals() const;

    void clearStatistics();
    void pushbackStatistic( const string& whichStatistic );

    bool itsCommunicationStat( const string& whichStat ) const;

    string getUnitsLabel( const string& whichStat ) const;

    void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime,
                  vector<TObjectOrder>& selectedRows );

    void getGroupsLabels( vector<string>& onVector ) const;
    void getStatisticsLabels( vector<string>& onVector, UINT32 whichGroup ) const;
    string getFirstStatistic() const;
    string getFirstCommStatistic() const;

    bool getControlOutOfLimits() const;
    bool getExtraOutOfLimits() const;

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
    bool xtraOutOfLimits;

    bool inclusive;

    /*vector<HistogramStatistic *> statisticFunctions;
    vector<HistogramStatistic *> commStatisticFunctions;*/
    Statistics statistics;

    vector<KWindow *> orderedWindows;
    RowsTranslator *rowsTranslator;
    ColumnTranslator *columnTranslator;
    ColumnTranslator *planeTranslator;

    Cube<TSemanticValue> *cube;
    Matrix<TSemanticValue> *matrix;
    Cube<TSemanticValue> *commCube;
    Matrix<TSemanticValue> *commMatrix;

    KHistogramTotals *totals;
    KHistogramTotals *rowTotals;
    KHistogramTotals *commTotals;
    KHistogramTotals *rowCommTotals;

    void orderWindows();
    bool createComms() const;
    void initTranslators();
    void initMatrix( THistogramColumn planes, THistogramColumn cols,
                     TObjectOrder rows );
    void initTotals();
    void initSemantic( TRecordTime beginTime );
    void initStatistics();
    void recursiveExecution( TRecordTime fromTime, TRecordTime toTime,
                             TObjectOrder fromRow, TObjectOrder toRow,
                             vector<TObjectOrder>& selectedRows, bool needInit,
                             UINT16 winIndex = 0, CalculateData *data = NULL );
    void calculate( TObjectOrder iRow,
                    TRecordTime fromTime, TRecordTime toTime,
                    UINT16 winIndex, CalculateData *data, bool needInit );
    void finishRow( CalculateData *data );
};


#endif // KHISTOGRAM_H_INCLUDED
