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
                                    const Histogram *whichHisto,
                                    THistogramLimit min,
                                    THistogramLimit max,
                                    THistogramLimit delta );
};

#endif // LABELCONSTRUCTOR_H_INCLUDED
