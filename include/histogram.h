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

    THistogramLimit getControlMin();
    THistogramLimit getControlMax();
    THistogramLimit getControlDelta();
    THistogramLimit getExtraControlMin();
    THistogramLimit getExtraControlMax();
    THistogramLimit getExtraControlDelta();
    THistogramLimit getDataMin();
    THistogramLimit getDataMax();

    void clearStatistics();
    void pushbackStatistic( HistogramStatistic *whichStatistic );

    void execute( TRecordTime beginTime, TRecordTime endTime );

  protected:

  private:
    KWindow *controlWindow;
    KWindow *dataWindow;
    KWindow *xtraControlWindow;

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

    vector<KWindow *> orderedWindows;
    RowsTranslator *rowsTranslator;
    ColumnTranslator *columnTranslator;
    ColumnTranslator *planeTranslator;

    Cube<TSemanticValue> *cube;
    Matrix<TSemanticValue> *matrix;

    void orderWindows();

};


#endif // HISTOGRAM_H_INCLUDED
