
SUBDIRS = \
	include \
	src \
	api \
	utils


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

otf2prv.o: $(otf2prv_bin_SOURCES)
	$(CXXCOMPILE) $(otf2prv_bin_CXXFLAGS) -c -o $@

otf2prv.bin$(EXEEXT): $(otf2prv_bin_OBJECTS) $(otf2prv_bin_DEPENDENCIES) $(EXTRA_otf2prv_bin_DEPENDENCIES)
	@rm -f otf2prv.bin$(EXEEXT)
	$(CXXLINK) $(otf2prv_bin_OBJECTS)	$(otf2prv_bin_LDFLAGS) $(otf2prv_bin_LDADD) $(LIBS)

endif


install-data-hook:
	if test -f $(DESTDIR)$(bindir)/paramedir ; then \
		cp -f $(DESTDIR)$(bindir)/paramedir $(DESTDIR)$(bindir)/old.paramedir ; \
	fi

if ENABLE_DEPENDENCIES_AWARE_INSTALL
	$(SED) "s|@inst_BOOST_LIBDIR@|${BOOST_LDFLAGS}|g ; s|-L||g ; \
					s|@inst_LOGIN_NODE_DETECTION@|if [ ! \"$$\(hostname \| grep login\)x\" = \"x\" ]; then\n  # login ==> limit to 4\n  export OMP_NUM_THREADS=4\nfi|g" < ./docs/paramedir > $(DESTDIR)$(bindir)/paramedir
else
	$(SED) "s|@inst_BOOST_LIBDIR@:||g ; \
					s|@inst_LOGIN_NODE_DETECTION@||g" < ./docs/paramedir > $(DESTDIR)$(bindir)/paramedir
endif

	chmod a+x $(DESTDIR)$(bindir)/paramedir

	$(mkdir_p) $(DESTDIR)$(prefix)/share
	$(mkdir_p) $(DESTDIR)$(prefix)/share/filters-config
	$(install_sh_PROGRAM) -m 775 ./docs/filters/*.xml $(DESTDIR)$(prefix)/share/filters-config


uninstall-hook:
	test -f $(DESTDIR)$(bindir)/paramedir            && rm  -fv $(DESTDIR)$(bindir)/paramedir
	test -f $(DESTDIR)$(bindir)/old.paramedir        && rm  -fv $(DESTDIR)$(bindir)/old.paramedir

	test -d $(DESTDIR)$(prefix)/share/filters-config && rm -rfv $(DESTDIR)$(prefix)/share/filters-config


dist-hook:
	$(mkdir_p) $(distdir)/docs
	cp -f ./docs/paramedir        $(distdir)/docs
	$(mkdir_p) $(distdir)/docs/filters
	$(install_sh_PROGRAM) -m 775 ./docs/filters/*.xml $(distdir)/docs/filters
#	$(install_sh_DATA) ./docs/filters/*.xml $(distdir)/docs/filters

installcheck-local:
	test -f $(DESTDIR)$(bindir)/paramedir && \
	echo "  [paraver-kernel][lib] Installed " || ( echo "  [paraver-kernel][lib] Check failed!!!" && exit 1 )


# Erases everything which should not be uploaded to the repository
mrproper: distclean
	rm -fr configure.scan  autom4te.cache config.h.in config.h.in~ configure Makefile.in config.guess autoscan.log config.log config.status aclocal.m4 config.sub install-sh ltmain.sh missing


        
