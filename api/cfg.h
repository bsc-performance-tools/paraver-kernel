#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include "kwindow.h"
#include "trace.h"

bool loadCFG( string& filename, Trace *whichTrace, vector<KWindow *>& windows,
              TRecordTime& beginTime, TRecordTime& endTime );

#endif // CFG_H_INCLUDED
