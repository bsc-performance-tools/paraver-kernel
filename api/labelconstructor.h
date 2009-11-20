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
                                  TSemanticValue value,
                                  bool showUnits );

    static string histoTotalLabel( THistoTotals whichTotal );

    static string timeLabel( TTime value, TTimeUnit unit, UINT32 precision );

    static string semanticLabel( const Window * whichWindow, TSemanticValue value,
                                 bool text, UINT32 precision );

    static string eventLabel( Window *whichWindow,
                              TEventType whichType,
                              TEventValue whichValue,
                              bool text );

    static string eventValueLabel( Window *whichWindow,
                                   TEventType whichType,
                                   TEventValue whichValue );

    enum TGroupID
    {
      COLOR = 0,
      GRADIENT_FUNCTION,
      DRAWMODE,
      PIXEL_SIZE,
      IMAGE_FORMAT,
      TEXT_FORMAT
    };

    static void getGUIGroupLabels( const TGroupID group, vector< string > &labels );
};

#endif // LABELCONSTRUCTOR_H_INCLUDED
