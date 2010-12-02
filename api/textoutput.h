#ifndef TEXTOUTPUT_H_INCLUDED
#define TEXTOUTPUT_H_INCLUDED

#include <string>
#include "selectionmanagement.h"
#include "prvtypes.h"
#include "histogram.h"

using std::string;

class Window;
class Histogram;

class TextOutput
{
  public:
    TextOutput();
    ~TextOutput();

    void dumpWindow( Window *whichWindow, string& strOutputFile );
    void dumpHistogram( Histogram *whichHisto,
                        string& strOutputFile,
                        bool onlySelectedPlane = false,
                        bool hideEmptyColumns = false );

    bool getMultipleFiles() const;
    void setMultipleFiles( bool newValue );

  protected:

  private:
    typedef TSemanticValue (HistogramTotals::*THistogramTotalsMethod)( PRV_UINT16 idStat,
                                                       THistogramColumn whichColumn,
                                                       THistogramColumn whichPlane ) const;
    bool multipleFiles;

    void dumpMatrixHorizontal( Histogram *whichHisto,
                               TObjectOrder numRows,
                               THistogramColumn numColumns,
                               PRV_UINT16 currentStat,
                               vector<THistogramColumn> printedColumns,
                               THistogramColumn iPlane,
                               ofstream &outputfile );

    void dumpMatrixVertical( Histogram *whichHisto,
                             TObjectOrder numRows,
                             THistogramColumn numColumns,
                             PRV_UINT16 currentStat,
                             vector<THistogramColumn> printedColumns,
                             THistogramColumn iPlane,
                             ofstream &outputfile );

    void dumpTotalColumns( HistogramTotals *totals,
                           string totalName,
                           THistogramTotalsMethod totalFunction,
                           PRV_UINT16 currentStat,
                           vector<THistogramColumn> printedColumns,
                           THistogramColumn iPlane,
                           ofstream &outputFile );

    void dumpTotalRows( HistogramTotals *totals,
                        string totalName,
                        THistogramTotalsMethod totalFunction,
                        PRV_UINT16 currentStat,
                        TObjectOrder numRows,
                        THistogramColumn iPlane,
                        ofstream &outputFile );

    void dumpMatrixCommHorizontal( Histogram *whichHisto,
                                   TObjectOrder numRows,
                                   THistogramColumn numColumns,
                                   PRV_UINT16 currentStat,
                                   vector<THistogramColumn> printedColumns,
                                   THistogramColumn iPlane,
                                   ofstream &outputfile );

    void dumpMatrixCommVertical( Histogram *whichHisto,
                                 TObjectOrder numRows,
                                 THistogramColumn numColumns,
                                 PRV_UINT16 currentStat,
                                 vector<THistogramColumn> printedColumns,
                                 THistogramColumn iPlane,
                                 ofstream &outputfile );

};

#endif // TEXTOUTPUT_H_INCLUDED
