
SUBDIRS = \
	include \
	src \
	api \
	utils


bin_PROGRAMS = paramedir.bin




paramedir_bin_SOURCES = \
	api/paramedir.cpp

paramedir_bin_LDADD = \
	-lz \
	api/libparaver-api.la \
	src/libparaver-kernel.la 


install-data-hook:
	if test -f $(DESTDIR)$(bindir)/paramedir ; then \
		cp -f $(DESTDIR)$(bindir)/paramedir $(DESTDIR)$(bindir)/old.paramedir ; \
	fi

if ENABLE_DEPENDENCIES_AWARE_INSTALL
	$(SED) "s|@inst_BOOST_LIBDIR@|${BOOST_LDFLAGS}|g ; s|-L||g ; \
					s|@inst_LOGIN_NODE_DETECTION@|if [ ! \"$$\(hostname \| grep login\)x\" = \"x\" ]; then export OMP_NUM_THREADS=4 ; fi|g" < ./docs/paramedir > $(DESTDIR)$(bindir)/paramedir
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
