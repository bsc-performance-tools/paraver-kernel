
pkglib_LTLIBRARIES = libparaver-api.la


if MAKE_OLD_PCFPARSER
  PCFPARSER_FILES=  ../utils/pcfparser/old/LibException.cpp \
                    ../utils/pcfparser/old/ParaverEventType.cpp \
                    ../utils/pcfparser/old/ParaverEventValue.cpp \
                    ../utils/pcfparser/old/ParaverGradientColor.cpp \
                    ../utils/pcfparser/old/ParaverGradientNames.cpp \
                    ../utils/pcfparser/old/ParaverState.cpp \
                    ../utils/pcfparser/old/ParaverStatesColor.cpp \
                    ../utils/pcfparser/old/ParaverTraceConfig.cpp \
                    ../utils/pcfparser/old/ParaverTraceConfigGrammar.cpp 
else
  PCFPARSER_FILES=../utils/pcfparser/ParaverTraceConfig.cpp \
                  ../utils/pcfparser/UIParaverTraceConfig.cpp
endif


libparaver_api_la_SOURCES = \
                    cfg.cpp \
                    drawmode.cpp \
                    eventdrivencutter.cpp \
                    eventlabels.cpp \
                    eventtranslator.cpp \
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
		    symbolpicker.cpp \
                    syncwindows.cpp \
                    textoutput.cpp \
                    trace.cpp \
                    tracecutter.cpp \
                    traceeditactions.cpp \
                    traceeditsequence.cpp \
                    tracefilter.cpp \
                    traceoptions.cpp \
                    traceshifter.cpp \
                    tracesoftwarecounters.cpp \
                    window.cpp \
                    workspace.cpp \
                    workspacemanager.cpp \
                    $(PCFPARSER_FILES)

include_HEADERS = \
                  cfg.h\
                  cubecontainer.h \
                  ../config_traits.h\
                  ../config_types.h\
                  drawmode.h\
                  eventdrivencutter.h\
                  eventlabels.h\
                  eventtranslator.h\
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
                  prvtypes.h\
                  recordlist.h\
                  rowlabels.h\
                  selectionmanagement.h\
                  selectionrowsutils.h \
                  semanticcolor.h\
                  statelabels.h\
		  symbolpicker.h \
                  syncwindows.h\
                  trace.h\
                  textoutput.h\
                  tracecutter.h \
                  traceeditactions.h \
                  traceeditsequence.h \
                  traceeditstates.h \
                  tracefilter.h \
                  traceoptions.h \
                  traceshifter.h \
                  tracesoftwarecounters.h \
                  window.h\
                  workspace.h\
                  workspacemanager.h\
                  zoomhistory.h\
                  cubecontainer.cpp\
                  selectionmanagement.cpp\
                  traceeditstates.cpp\
                  zoomhistory.cpp

install-data-hook:
	$(mkdir_p) $(includedir)/utils/pcfparser
	cp -f ../utils/pcfparser/UIParaverTraceConfig.h $(includedir)/utils/pcfparser
	cp -f ../utils/pcfparser/UIParaverTraceConfig.ipp $(includedir)/utils/pcfparser

	$(mkdir_p) $(includedir)/utils/pcfparser/old
	cp -f ../utils/pcfparser/old/ParaverTraceConfig.h $(includedir)/utils/pcfparser/old

uninstall-hook:
	test -d $(includedir)/utils/pcfparser && rm -rf $(includedir)/utils/pcfparser


# Erases everything which should not be uploaded to CVS
mrproper: distclean
	rm -fr Makefile.in
