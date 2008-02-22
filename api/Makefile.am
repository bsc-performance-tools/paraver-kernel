
noinst_LTLIBRARIES = libcfg.la libapi.la
libcfg_la_SOURCES = \
                    cfg.cpp

libapi_la_SOURCES = \
                    histogram.cpp \
                    histogramtotals.cpp \
                    localkernel.cpp \
                    recordlist.cpp \
                    window.cpp

# Erases everything which should not be uploaded to CVS
mrproper: distclean
	rm -fr Makefile.in
