
noinst_LTLIBRARIES = libcfg.la libapi.la
libcfg_la_SOURCES = \
                    cfg.cpp

libapi_la_SOURCES = \
                    histogram.cpp \
                    histogramtotals.cpp \
                    localkernel.cpp \
                    recordlist.cpp \
                    window.cpp \
                    ../../common-files/pcfparser/ParaverEventType.cpp \
                    ../../common-files/pcfparser/ParaverEventValue.cpp \
                    ../../common-files/pcfparser/ParaverGradientColor.cpp \
                    ../../common-files/pcfparser/ParaverGradientNames.cpp \
                    ../../common-files/pcfparser/ParaverState.cpp \
                    ../../common-files/pcfparser/ParaverStatesColor.cpp \
                    ../../common-files/pcfparser/ParaverTraceConfig.cpp \
                    ../../common-files/pcfparser/ParaverTraceConfigGrammar.cpp
                    

# Erases everything which should not be uploaded to CVS
mrproper: distclean
	rm -fr Makefile.in
