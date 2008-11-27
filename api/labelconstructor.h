#ifndef LABELCONSTRUCTOR_H_INCLUDED
#define LABELCONSTRUCTOR_H_INCLUDED

#include "paraverkerneltypes.h"
#include "trace.h"

class Histogram;

class LabelConstructor
{
  public:
    static string objectLabel( TObjectOrder globalOrder,
                               TWindowLevel level,
                               Trace *whichTrace );

    static string histoColumnLabel( THistogramColumn whichColumn,
                                    const Window *whichWindow,
                                    THistogramLimit min,
                                    THistogramLimit max,
                                    THistogramLimit delta );

    static string histoCellLabel( const Histogram *whichHisto,
                                  TSemanticValue value );

    static string histoTotalLabel( THistoTotals whichTotal );

    static string timeLabel( TTime value, TTimeUnit unit );

    static string semanticLabel( const Window * whichWindow, TSemanticValue value,
                                 bool text );
};

#endif // LABELCONSTRUCTOR_H_INCLUDED
