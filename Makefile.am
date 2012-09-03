
SUBDIRS = \
          include \
          src \
          api

bin_PROGRAMS = paramedir.bin \
							 otf2prv.bin

paramedir_bin_SOURCES = \
                    api/paramedir.cpp

paramedir_bin_LDADD = \
                  -lz \
                  api/libparaver-api.la \
                  src/libparaver-kernel.la 

otf2prv_bin_SOURCES = \
											api/otf2prv.cpp

otf2prv_bin_LDADD = \
										api/libparaver-api.la \
										src/libparaver-kernel.la \
										-lotf2 \
										-lscorep_utilities

# Erases everything which should not be uploaded to CVS
mrproper: distclean
	rm -fr configure.scan  autom4te.cache config.h.in config.h.in~ configure Makefile.in config.guess autoscan.log config.log config.status aclocal.m4 config.sub install-sh ltmain.sh missing

        
