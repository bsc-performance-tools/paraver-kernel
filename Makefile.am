
SUBDIRS = \
          include \
          src \
          api


bin_PROGRAMS = paramedir.bin


if WANT_OTF2PRV
  bin_PROGRAMS += otf2prv.bin
endif


paramedir_bin_SOURCES = \
                    api/paramedir.cpp

paramedir_bin_LDADD = \
                  -lz \
                  api/libparaver-api.la \
                  src/libparaver-kernel.la 


if WANT_OTF2PRV
  otf2prv_bin_SOURCES = \
											api/otf2prv.cpp

  otf2prv_bin_LDADD = \
                  api/libparaver-api.la \
                  src/libparaver-kernel.la

  otf2prv_bin_CXXFLAGS = -I@OTF2_DIR@/include 
  otf2prv_bin_LDFLAGS = -L@OTF2_DIR@/lib -lotf2 -lscorep_utilities
endif


otf2prv.o: $(otf2prv_bin_SOURCES)
	$(CXXCOMPILE) $(otf2prv_bin_CXXFLAGS) -c -o $@

otf2prv.bin$(EXEEXT): $(otf2prv_bin_OBJECTS) $(otf2prv_bin_DEPENDENCIES) $(EXTRA_otf2prv_bin_DEPENDENCIES)
	@rm -f otf2prv.bin$(EXEEXT)
	$(CXXLINK) $(otf2prv_bin_OBJECTS)	$(otf2prv_bin_LDFLAGS) $(otf2prv_bin_LDADD) $(LIBS)


install-data-hook:
	cp -f ./docs/paramedir $(bindir)
	$(SED) "s|@inst_PARAVER_HOME@|$(prefix)|g" < ./docs/paramedir > $(bindir)/paramedir
	chmod a+x $(bindir)/paramedir


dist-hook:
	$(mkdir_p) $(distdir)/docs
	cp -f ./docs/paramedir          $(distdir)/docs


# Erases everything which should not be uploaded to CVS
mrproper: distclean
	rm -fr configure.scan  autom4te.cache config.h.in config.h.in~ configure Makefile.in config.guess autoscan.log config.log config.status aclocal.m4 config.sub install-sh ltmain.sh missing

        
