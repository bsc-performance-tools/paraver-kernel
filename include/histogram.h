#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include "kwindow.h"
#include "cube.h"

class HistogramStatistic;

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
    vector<KWindow *> *window;

    vector<TObjectOrder> rowTranslator1;
    vector<TObjectOrder> rowTranslator2;
};


class ColumnTranslator
{
  public:
    ColumnTranslator( THistogramLimit whichMin, THistogramLimit whichMax,
                      THistogramLimit whichDelta );
    ~ColumnTranslator();

    THistogramColumn getColumn( THistogramLimit whichValue ) const;
    THistogramColumn totalColumns() const;

  protected:

  private:
    bool discrete;
};



class Histogram
{
  public:
    Histogram();
    ~Histogram();

    bool getThreeDimensions() const;

    TRecordTime getBeginTime() const;
    TRecordTime getEndTime() const;

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

    void clearStatistics();
    void pushbackStatistic( HistogramStatistic *whichStatistic );

    void execute( TRecordTime beginTime, TRecordTime endTime );

  protected:

  private:
    KWindow *controlWindow;
    KWindow *dataWindow;
    KWindow *xtraControlWindow;

    bool threeDimensions;

    TRecordTime beginTime;
    TRecordTime endTime;

    THistogramLimit controlMin;
    THistogramLimit controlMax;
    THistogramLimit controlDelta;
    THistogramLimit xtraControlMin;
    THistogramLimit xtraControlMax;
    THistogramLimit xtraControlDelta;
    THistogramLimit dataMin;
    THistogramLimit dataMax;

    vector<HistogramStatistic *> statisticFunctions;
    UINT16 numStatistics;
    UINT16 numCommStatistics;

    vector<KWindow *> orderedWindows;
    RowsTranslator *rowsTranslator;
    ColumnTranslator *columnTranslator;
    ColumnTranslator *planeTranslator;

    Cube<TSemanticValue> *cube;
    Matrix<TSemanticValue> *matrix;
    Cube<TSemanticValue> *commCube;
    Matrix<TSemanticValue> *commMatrix;

    void orderWindows();
    bool createComms() const;
    void initTranslators();
    void initMatrix( THistogramColumn numPlanes, THistogramColumn numCols,
                     TObjectOrder numRows );
    void initSemantic( TRecordTime beginTime );
    void initStatistics();
};


#endif // HISTOGRAM_H_INCLUDED
