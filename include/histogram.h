#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include "kwindow.h"
#include "cube.h"

class HistogramStatistic;
class HistogramTotals;

class RowsTranslator
{
  public:
    /*
        PRECOND1: windows vector comes downwards level ordered.
        PRECOND2: KWindows in window vector have same hierarchy.
        PRECOND3: windows vector size is 2 or 3 KWindows.
    */

    RowsTranslator( vector<KWindow *>& windows );
    ~RowsTranslator();

    TObjectOrder globalTranslate( UINT16 winIndex,
                                  TObjectOrder rowIndex ) const;
    void getRowChilds( UINT16 winIndex,
                       TObjectOrder rowIndex,
                       TObjectOrder& iniRow,
                       TObjectOrder& endRow ) const;
    TObjectOrder totalRows() const;
  protected:

  private:
    struct RowChildInfo
    {
      bool oneToOne;
      vector< pair< TObjectOrder, TObjectOrder > > rowChilds;
    };

    vector< RowChildInfo > childInfo;
};


class ColumnTranslator
{
  public:
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
  MemoryTrace::iterator *comm;
};


class Histogram
{
  public:
    Histogram();
    ~Histogram();

    bool getThreeDimensions() const;

    TRecordTime getBeginTime() const;
    TRecordTime getEndTime() const;

    KWindow *getControlWindow() const;
    KWindow *getDataWindow() const;
    KWindow *getExtraControlWindow() const;
    void setControlWindow( KWindow *whichWindow );
    void setDataWindow( KWindow *whichWindow );
    void setExtraControlWindow( KWindow *whichWindow );
    void clearControlWindow();
    void clearDataWindow();
    void clearExtraControlWindow();

    void setControlMin( THistogramLimit whichMin );
    void setControlMax( THistogramLimit whichMax );
    void setControlDelta( THistogramLimit whichDelta );
    void setExtraControlMin( THistogramLimit whichMin );
    void setExtraControlMax( THistogramLimit whichMax );
    void setExtraControlDelta( THistogramLimit whichDelta );
    void setDataMin( THistogramLimit whichMin );
    void setDataMax( THistogramLimit whichMax );

    THistogramLimit getControlMin() const;
    THistogramLimit getControlMax() const;
    THistogramLimit getControlDelta() const;
    THistogramLimit getExtraControlMin() const;
    THistogramLimit getExtraControlMax() const;
    THistogramLimit getExtraControlDelta() const;
    THistogramLimit getDataMin() const;
    THistogramLimit getDataMax() const;

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

    void clearStatistics();
    void pushbackStatistic( HistogramStatistic *whichStatistic );

    void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime );

  protected:

  private:
    KWindow *controlWindow;
    KWindow *dataWindow;
    KWindow *xtraControlWindow;

    bool threeDimensions;

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
    THistogramLimit dataMin;
    THistogramLimit dataMax;

    vector<HistogramStatistic *> statisticFunctions;
    vector<HistogramStatistic *> commStatisticFunctions;

    vector<KWindow *> orderedWindows;
    RowsTranslator *rowsTranslator;
    ColumnTranslator *columnTranslator;
    ColumnTranslator *planeTranslator;

    Cube<TSemanticValue> *cube;
    Matrix<TSemanticValue> *matrix;
    Cube<TSemanticValue> *commCube;
    Matrix<TSemanticValue> *commMatrix;

    HistogramTotals *totals;
    HistogramTotals *rowTotals;
    HistogramTotals *commTotals;
    HistogramTotals *rowCommTotals;

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
                             UINT16 winIndex = 0, CalculateData *data = NULL );
    void calculate( TObjectOrder iRow,
                    TRecordTime fromTime, TRecordTime toTime,
                    TObjectOrder fromRow, TObjectOrder toRow,
                    UINT16 winIndex, CalculateData *data );
    void finishRow( CalculateData *data );
};


#endif // HISTOGRAM_H_INCLUDED
