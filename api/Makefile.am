
pkglib_LTLIBRARIES = libparaver-api.la

libparaver_api_la_SOURCES = \
                    cfg.cpp \
                    drawmode.cpp \
                    eventlabels.cpp \
                    filter.cpp \
                    histogram.cpp \
                    histogramtotals.cpp \
                    labelconstructor.cpp \
                    loadedwindows.cpp \
                    localkernel.cpp \
                    paraverconfig.cpp \
                    previousfiles.cpp \
                    progresscontroller.cpp \
                    recordlist.cpp \
                    rowlabels.cpp \
                    selectionrowsutils.cpp \
                    semanticcolor.cpp \
                    statelabels.cpp \
                    syncwindows.cpp \
                    textoutput.cpp \
                    trace.cpp \
                    tracecutter.cpp \
                    tracefilter.cpp \
                    traceoptions.cpp \
                    tracesoftwarecounters.cpp \
                    window.cpp \
                    ../../common-files/pcfparser/LibException.cpp \
                    ../../common-files/pcfparser/ParaverEventType.cpp \
                    ../../common-files/pcfparser/ParaverEventValue.cpp \
                    ../../common-files/pcfparser/ParaverGradientColor.cpp \
                    ../../common-files/pcfparser/ParaverGradientNames.cpp \
                    ../../common-files/pcfparser/ParaverState.cpp \
                    ../../common-files/pcfparser/ParaverStatesColor.cpp \
                    ../../common-files/pcfparser/ParaverTraceConfig.cpp \
                    ../../common-files/pcfparser/ParaverTraceConfigGrammar.cpp
                    
include_HEADERS = \
                  cfg.h\
                  ../config.h\
                  drawmode.h\
                  eventlabels.h\
                  filter.h\
                  histogram.h\
                  histogramtotals.h\
                  kernelconnection.h\
                  labelconstructor.h\
                  loadedwindows.h\
                  localkernel.h\
                  paramedir.h\
                  paraverconfig.h\
                  paraverkernelexception.h\
                  paraverkerneltypes.h\
                  paraverlabels.h\
                  paravertypes.h\
                  previousfiles.h\
                  progresscontroller.h\
                  ../../common-files/prvtypes.h\
                  recordlist.h\
                  rowlabels.h\
                  selectionmanagement.h\
                  selectionrowsutils.h \
                  semanticcolor.h\
                  statelabels.h\
                  syncwindows.h\
                  trace.h\
                  textoutput.h\
                  tracecutter.h \
                  tracefilter.h \
                  traceoptions.h \
                  tracesoftwarecounters.h \
                  window.h\
                  zoomhistory.h\
                  selectionmanagement.cpp\
                  zoomhistory.cpp


# Erases everything which should not be uploaded to CVS
mrproper: distclean
	rm -fr Makefile.in
