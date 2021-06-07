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


#ifndef TEXTOUTPUT_H_INCLUDED
#define TEXTOUTPUT_H_INCLUDED

#include <string>
#include "output.h"
#include "selectionmanagement.h"
#include "histogram.h"
#include "progresscontroller.h"

class Window;
class Histogram;

class TextOutput:public Output
{
  public:
    TextOutput();
    virtual ~TextOutput();

    void dumpWindow( Window *whichWindow,
                     std::string& strOutputFile,
                     ProgressController *progress = nullptr );
    void dumpHistogram( Histogram *whichHisto,
                        std::string& strOutputFile,
                        bool onlySelectedPlane = false,
                        bool hideEmptyColumns = false,
                        bool withLabels = true,
                        bool withPreferencesPrecision = true,
                        bool recalcHisto = true,
                        ProgressController *progress = nullptr );

    bool getMultipleFiles() const;
    void setMultipleFiles( bool newValue );

    bool getObjectHierarchy() const;
    void setObjectHierarchy( bool newValue );

    bool getWindowTimeUnits() const;
    void setWindowTimeUnits( bool newValue );

    bool getTextualSemantic() const;
    void setTextualSemantic( bool newValue );

    TTime getMinTime() const;
    TTime getMaxTime() const;

  protected:

  private:
    typedef TSemanticValue (HistogramTotals::*THistogramTotalsMethod)( PRV_UINT16 idStat,
                                                       THistogramColumn whichColumn,
                                                       THistogramColumn whichPlane ) const;
    bool multipleFiles;
    bool objectHierarchy;
    bool windowTimeUnits;
    bool textualSemantic;

    TTime minTime;
    TTime maxTime;

    void dumpMatrixHorizontal( Histogram *whichHisto,
                               TObjectOrder numRows,
                               THistogramColumn numColumns,
                               PRV_UINT16 currentStat,
                               std::vector<THistogramColumn> printedColumns,
                               THistogramColumn iPlane,
                               std::ofstream &outputfile,
                               bool withLabels,
                               ProgressController *progress = nullptr );

    void dumpMatrixVertical( Histogram *whichHisto,
                             TObjectOrder numRows,
                             THistogramColumn numColumns,
                             PRV_UINT16 currentStat,
                             std::vector<THistogramColumn> printedColumns,
                             THistogramColumn iPlane,
                             std::ofstream &outputfile,
                             bool withLabels,
                             ProgressController *progress = nullptr );

    void dumpTotalColumns( Histogram *whichHisto,
                           HistogramTotals *totals,
                           std::string totalName,
                           THistogramTotalsMethod totalFunction,
                           PRV_UINT16 currentStat,
                           std::vector<THistogramColumn> printedColumns,
                           THistogramColumn iPlane,
                           std::ofstream &outputFile,
                           ProgressController *progress = nullptr );

    void dumpTotalRows( HistogramTotals *totals,
                        std::string totalName,
                        THistogramTotalsMethod totalFunction,
                        PRV_UINT16 currentStat,
                        TObjectOrder numRows,
                        THistogramColumn iPlane,
                        std::ofstream &outputFile,
                        ProgressController *progress = nullptr );

    void dumpMatrixCommHorizontal( Histogram *whichHisto,
                                   TObjectOrder numRows,
                                   THistogramColumn numColumns,
                                   PRV_UINT16 currentStat,
                                   std::vector<THistogramColumn> printedColumns,
                                   THistogramColumn iPlane,
                                   std::ofstream &outputfile,
                                   bool withLabels,
                                   ProgressController *progress = nullptr );

    void dumpMatrixCommVertical( Histogram *whichHisto,
                                 TObjectOrder numRows,
                                 THistogramColumn numColumns,
                                 PRV_UINT16 currentStat,
                                 std::vector<THistogramColumn> printedColumns,
                                 THistogramColumn iPlane,
                                 std::ofstream &outputfile,
                                 bool withLabels,
                                 ProgressController *progress = nullptr );

};

#endif // TEXTOUTPUT_H_INCLUDED
