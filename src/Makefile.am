
pkglib_LTLIBRARIES = libparaver-kernel.la
libparaver_kernel_la_SOURCES = \
    bplustreeblocks.cpp \
    bplustree.cpp \
    bplustreeexception.cpp \
    bplustreerecordleaf.cpp \
    filtermanagement.cpp \
    histogramexception.cpp \
    histogramstatistic.cpp \
    intervalcompose.cpp \
    intervalcontrolderived.cpp \
    intervalcpu.cpp \
    intervalderived.cpp \
    intervalnotthread.cpp \
    intervalthread.cpp \
    kfilter.cpp \
    khistogram.cpp \
    khistogramtotals.cpp \
    kprogresscontroller.cpp \
    krecordlist.cpp \
    ktrace.cpp \
    ktracecutter.cpp \
    ktracefilter.cpp \
    ktraceoptions.cpp \
    ktracesoftwarecounters.cpp \
    kwindow.cpp \
    kwindowexception.cpp \
    memorytrace.cpp \
    noloadblocks.cpp \
    noloadtrace.cpp \
    paraverkernelexception.cpp \
    plainblocks.cpp \
    plaintrace.cpp \
    processmodel.cpp \
    resourcemodel.cpp \
    semanticcomposefunctions.cpp \
    semanticcpufunctions.cpp \
    semanticderivedfunctions.cpp \
    semanticexception.cpp \
    semanticmanagement.cpp \
    semanticnotthreadfunctions.cpp \
    semanticthreadfunctions.cpp \
    statisticmanagement.cpp \
    tracebodyio.cpp \
    tracebodyio_v1.cpp \
    tracebodyio_v2.cpp \
    traceheaderexception.cpp\
    tracestream.cpp
          
noinst_HEADERS = \
    cell.cpp \
    column.cpp \
    cube.cpp \
    functionmanagement.cpp \
    index.cpp \
    matrix.cpp \
    statistic_2d.cpp

# Erases everything which should not be uploaded to CVS
mrproper: distclean
	rm -fr Makefile.in
