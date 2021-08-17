# AX_PROG_ENABLE_DEPENDENCIES_AWARE_INSTALL
# -----------------------------------------
AC_DEFUN([AX_PROG_ENABLE_DEPENDENCIES_AWARE_INSTALL],
[
   AC_ARG_ENABLE(dependencies_aware_install,
      AC_HELP_STRING(
         [--enable-dependencies-aware-install],
         [In the install phase, some execution scripts extend LD_LIBRARY_PATH to include path to dependent libs before calling binaries (default: disabled)]
      ),
      [enable_dependencies_aware_install="${enableval}"],
      [enable_dependencies_aware_install="no"]
   )

   AM_CONDITIONAL([ENABLE_DEPENDENCIES_AWARE_INSTALL], [test "${enable_dependencies_aware_install}" = "yes"] )
])



# AX_PROG_ENABLE_EXTENDED_OBJECTS
# -------------------------------
AC_DEFUN([AX_PROG_ENABLE_EXTENDED_OBJECTS],
[
   AC_ARG_ENABLE(extended_objects,
      AC_HELP_STRING(
         [--enable-extended-objects],
         [Enable extra number of objects (default: disabled)]
      ),
      [enable_extended_objects="${enableval}"],
      [enable_extended_objects="no"]
   )

   if test "${enable_extended_objects}" = "yes" ; then
     AC_DEFINE([EXTENDED_OBJECTS_ENABLED], 1, [Extended number of objects enabled by user.])
   fi
])



# AX_PROG_ENABLE_OLD_PCFPARSER
# ----------------------------
AC_DEFUN([AX_PROG_ENABLE_OLD_PCFPARSER],
[
   AC_ARG_ENABLE(old_pcfparser,
      AC_HELP_STRING(
         [--enable-old-pcfparser],
         [enable compilation with old pcfparser (default: disabled)]
      ),
      [enable_old_pcfparser="${enableval}"],
      [enable_old_pcfparser="no"]
   )

   if test "${enable_old_pcfparser}" = "yes" ; then
      AC_MSG_NOTICE([Using old pcfparser.])
      AC_DEFINE([OLD_PCFPARSER], 1, [Old pcfparser enabled by user.])
   fi

   AM_CONDITIONAL(MAKE_OLD_PCFPARSER, test "${enable_old_pcfparser}" = "yes" )
])



# AX_PROG_ENABLE_OMPSS
# --------------------
AC_DEFUN([AX_PROG_ENABLE_OMPSS],
[
   AC_ARG_ENABLE(ompss,
      AC_HELP_STRING(
         [--enable-ompss],
         [Enable OmpSs parallel version (default: disabled)]
      ),
      [enable_ompss="${enableval}"],
      [enable_ompss="no"]
   )
   if test "${enable_ompss}" = "yes" ; then
      AC_DEFINE([PARALLEL_ENABLED], 1, [OmpSs parallel version enabled by user.])
      #CXXFLAGS="$CXXFLAGS --instrument"
      #LDFLAGS="$LDFLAGS --instrument -fPIC"
      LDFLAGS="$LDFLAGS -fPIC"
   fi
])



# AX_PROG_ENABLE_OPENMP
# ---------------------
AC_DEFUN([AX_PROG_ENABLE_OPENMP],
[
   AC_ARG_ENABLE(openmp,
      AC_HELP_STRING(
         [--enable-openmp],
         [enable OpenMP parallel version (default: disabled)]
      ),
      [enable_openmp="${enableval}"],
      [enable_openmp="no"]
   )

   if test "${enable_openmp}" = "yes" ; then
      AC_DEFINE([PARALLEL_ENABLED], 1, [Parallel version enabled by user.])
      CXXFLAGS="$CXXFLAGS -fopenmp"
      LDFLAGS="$LDFLAGS -fopenmp"
   fi
])



# AX_PROG_WITH_EXTRAE
# -------------------
AC_DEFUN([AX_PROG_WITH_EXTRAE],
[
   AC_REQUIRE([AX_PROG_ENABLE_OPENMP])

   AC_ARG_WITH(extrae,
      AC_HELP_STRING(
         [--with-extrae@<:@=DIR@:>@],
         [specify extrae library base install directory. (default: none; if no parameter DIR given: /usr/local) ]
      ),
      [if test "${withval}" = "yes" ; then
          EXTRAE_DIR="/usr/local"m
       else
          EXTRAE_DIR=${withval}
       fi],
      [EXTRAE_DIR=""]
   )

   if test "x${EXTRAE_DIR}" != "x" ; then
      AC_MSG_NOTICE([Trying to detect Extrae installation in ${EXTRAE_DIR}])
      if test -d "${EXTRAE_DIR}/lib" && test -f "${EXTRAE_DIR}/lib/libseqtrace.so" ; then
         AC_MSG_NOTICE([Extrae installation found in ${EXTRAE_DIR}/lib])
         CPPFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         CXXFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         CFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         LIBS_EXTRAE="-L$EXTRAE_DIR/lib"
         if test "${enable_openmp_parallel}" = "yes" ; then
            LDFLAGS_EXTRAE="${LIBS_EXTRAE} -lomptrace"
         else
            LDFLAGS_EXTRAE="${LIBS_EXTRAE} -lseqtrace"
         fi
      elif test -d "${EXTRAE_DIR}/lib64" && test -f "${EXTRAE_DIR}/lib64/libseqtrace.so" ; then
         AC_MSG_NOTICE([Extrae installation found in ${EXTRAE_DIR}/lib64])
         CPPFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         CXXFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         CFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         LIBS_EXTRAE="-L$EXTRAE_DIR/lib64"
         if test "${enable_openmp_parallel}" = "yes" ; then
            LDFLAGS_EXTRAE="${LIBS_EXTRAE} -lomptrace"
         else
            LDFLAGS_EXTRAE="${LIBS_EXTRAE} -lseqtrace"
         fi
      else
         AC_MSG_WARN([No Extrae installation found in ${EXTRAE_DIR}])
      fi
   fi
])



# AX_PROG_WITH_OTF2
# -----------------
AC_DEFUN([AX_PROG_WITH_OTF2],
[
   AC_ARG_WITH(otf2,
      AC_HELP_STRING(
         [--with-otf2@<:@=DIR@:>@],
         [specify OTF2 base install directory (default: none; if no DIR parameter given: /usr/local)]
      ),
      [if test "${withval}" = "yes" ; then
          OTF2_DIR="/usr/local"
       else
          OTF2_DIR=${withval}
       fi],
      [OTF2_DIR=""]
   )

   if test "x${OTF2_DIR}" != "x" ; then
      CPPFLAGS_OTF2="-I$OTF2_DIR/include"
      CXXFLAGS_OTF2="-I$OTF2_DIR/include"
      CFLAGS_OTF2="-I$OTF2_DIR/include"
      LIBS_OTF2="-L$OTF2_DIR/lib"
      LDFLAGS_OTF2="-L$OTF2_DIR/lib -lotf2"
   fi

   AM_CONDITIONAL([WANT_OTF2PRV], [test "x${OTF2_DIR}" != "x"] )
   AC_SUBST(OTF2_DIR)
])


# AX_PROG_WITH_DEBUG_LEVEL
# ------------------------
AC_DEFUN([AX_PROG_WITH_DEBUG_LEVEL],
[
   AC_ARG_WITH(debug-level,
      AC_HELP_STRING(
         [--with-debug-level@<:@=MODE@:>@],
         [select debug level between "release", "debug" or "none" (default: "none")]
      ),
      [
       if test "${withval}" = "yes" ; then #with
            DEBUG_LEVEL="debug"
       elif test "${withval}" = "no" ; then #without
            DEBUG_LEVEL="release"
       else
            DEBUG_LEVEL=${withval}
       fi],
      [DEBUG_LEVEL="none"]
   )

   CPPFLAGS_DEBUG_LEVEL=""
   if test "${DEBUG_LEVEL}" = "none" ; then
      CPPFLAGS_DEBUG_LEVEL=""
      AC_MSG_NOTICE([Debug mode set to ${DEBUG_LEVEL} (${CPPFLAGS_DEBUG_LEVEL})])
   elif test "${DEBUG_LEVEL}" = "release" ; then
      CPPFLAGS_DEBUG_LEVEL="-O2 -g0"
      AC_MSG_NOTICE([Debug mode set to ${DEBUG_LEVEL} (${CPPFLAGS_DEBUG_LEVEL})])
   elif test "${DEBUG_LEVEL}" = "debug" ; then
      CPPFLAGS_DEBUG_LEVEL="-O0 -ggdb3"
      AC_MSG_NOTICE([Debug mode set to ${DEBUG_LEVEL} (${CPPFLAGS_DEBUG_LEVEL})])
   else
      AC_MSG_NOTICE([Incorrect debug mode found in ${DEBUG_LEVEL}])
   fi
])


# AX_PROG_ENABLE_MINGW
# ---------------------
AC_DEFUN([AX_PROG_ENABLE_MINGW],
[
   AC_ARG_ENABLE(mingw,
      AC_HELP_STRING(
         [--enable-mingw],
         [enable flags for MinGw environment (default: disabled)]
      ),
      [enable_mingw="${enableval}"],
      [enable_mingw="no"]
   )

   MINGW_CPPFLAGS=""
   MINGW_LIBS=""

   if test "${enable_mingw}" = "yes" ; then
      MINGW_CPPFLAGS="-Wa,-mbig-obj"
      MINGW_LIBS="-lshlwapi"
   fi
])



