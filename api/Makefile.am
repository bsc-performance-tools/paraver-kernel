
pkglib_LTLIBRARIES = libparaver-api.la

libparaver_api_la_SOURCES = \
                    cfg.cpp \
                    drawmode.cpp \
                    eventlabels.cpp \
                    filter.cpp \
                    gnuplotoutput.cpp \
                    histogram.cpp \
                    histogramtotals.cpp \
                    labelconstructor.cpp \
                    loadedwindows.cpp \
                    localkernel.cpp \
                    output.cpp \
                    paraverconfig.cpp \
                    previousfiles.cpp \
                    progresscontroller.cpp \
										ParaverMetadataManager.cpp \
                    recordlist.cpp \
                    rowlabels.cpp \
                    selectionrowsutils.cpp \
                    semanticcolor.cpp \
                    statelabels.cpp \
                    syncwindows.cpp \
                    textoutput.cpp \
                    trace.cpp \
                    tracecutter.cpp \
                    traceeditactions.cpp \
                    traceeditsequence.cpp \
                    traceeditstates.cpp \
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
                  ../config_types.h\
                  drawmode.h\
                  eventlabels.h\
                  filter.h\
                  gnuplotoutput.h\
                  histogram.h\
                  histogramtotals.h\
                  kernelconnection.h\
                  labelconstructor.h\
                  loadedwindows.h\
                  localkernel.h\
                  otf2prv.h\
                  output.h\
                  paramedir.h\
                  paraverconfig.h\
                  paraverkernelexception.h\
                  paraverkerneltypes.h\
                  paraverlabels.h\
                  paravertypes.h\
                  previousfiles.h\
                  progresscontroller.h\
									ParaverMetadataManager.h\
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
                  traceeditactions.h \
                  traceeditsequence.h \
                  traceeditstates.h \
                  tracefilter.h \
                  traceoptions.h \
                  tracesoftwarecounters.h \
                  window.h\
                  zoomhistory.h\
                  selectionmanagement.cpp\
                  zoomhistory.cpp

install-data-hook:
	$(mkdir_p) $(includedir)/pcfparser
	cp -f ../../common-files/pcfparser/ParaverTraceConfig.h $(includedir)/pcfparser


# Erases everything which should not be uploaded to CVS
mrproper: distclean
	rm -fr Makefile.in
