# AX_PROG_ENABLE_EXTENDED_OBJECTS
# -----------
AC_DEFUN([AX_PROG_ENABLE_EXTENDED_OBJECTS],
[
  AC_ARG_ENABLE(extended_objects,
    AC_HELP_STRING(
      [--enable-extended-objects],
      [Enable extension in number of objects. (Disabled by default)]
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
      [Enable compilation with old pcfparser. (Disabled by default)]
    ),
    [enable_old_pcfparser="${enableval}"],
    [enable_old_pcfparser="no"]
  )
  if test "${enable_old_pcfparser}" = "yes" ; then
    AC_DEFINE([SET_OLD_PCFPARSER], 1, [Old pcfparser enabled by user.])
    PCFPARSER_CFLAGS="-DOLD_PCFPARSER"
  else
    AC_DEFINE([SET_OLD_PCFPARSER], 0, [Old pcfparser enabled by user.])
    PCFPARSER_CFLAGS=""
  fi
])



# AX_PROG_ENABLE_OMPSS
# -----------------------
AC_DEFUN([AX_PROG_ENABLE_OMPSS_PARALLEL],
[
  AC_ARG_ENABLE(ompss,
    AC_HELP_STRING(
      [--enable-ompss],
      [Enable OmpSs parallel version. (Disabled by default)]
    ),
    [enable_ompss_parallel="${enableval}"],
    [enable_ompss_parallel="no"]
  )
  if test "${enable_ompss_parallel}" = "yes" ; then
    AC_DEFINE([PARALLEL_ENABLED], 1, [Parallel version enabled by user.])
    #CXXFLAGS="$CXXFLAGS --instrument"
    #LDFLAGS="$LDFLAGS --instrument -fPIC"
    LDFLAGS="$LDFLAGS -fPIC"
  fi
])



# AX_PROG_ENABLE_OPENMP
# -----------------------
AC_DEFUN([AX_PROG_ENABLE_OPENMP],
[
  AC_ARG_ENABLE(openmp,
    AC_HELP_STRING(
      [--enable-openmp],
      [Enable OpenMP parallel version. (Disabled by default)]
    ),
    [enable_openmp="${enableval}"],
    [enable_openmp="no"]
  )
  if test "${enable_openmp_parallel}" = "yes" ; then
    AC_DEFINE([PARALLEL_ENABLED], 1, [Parallel version enabled by user.])
    CXXFLAGS="$CXXFLAGS -fopenmp"
    LDFLAGS="$LDFLAGS -fopenmp"
  fi
])



# AX_PROG_WITH_EXTRAE
# -----------
AC_DEFUN([AX_PROG_WITH_EXTRAE],
[
  AC_REQUIRE([AX_PROG_ENABLE_OPENMP])

  AC_ARG_WITH(extrae,
    AC_HELP_STRING(
      [--with-extrae@<:@=DIR@:>@],
      [Specify extrae library base install directory. Default: /usr/local]
    ),
    [if test "x${withval}" = "xyes" ; then 
       EXTRAE_DIR="/usr/local"
     else
       EXTRAE_DIR=${withval}
     fi],
    [EXTRAE_DIR=""]
  )
  if test "x${EXTRAE_DIR}" != "x" ; then
    AC_MSG_NOTICE([Trying to detect Extrae installation in ${EXTRAE_DIR}])
    if test -d "${EXTRAE_DIR}/lib" && test -f "${EXTRAE_DIR}/lib/libseqtrace.so" ; then
      AC_MSG_NOTICE([Extrae installation found])
      CPPFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
      CXXFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
      CFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
      LIBS_EXTRAE="-L$EXTRAE_DIR/lib"
      if test "${enable_openmp_parallel}" = "yes" ; then
        LDFLAGS_EXTRAE="-L$EXTRAE_DIR/lib -lomptrace"
      else
        LDFLAGS_EXTRAE="-L$EXTRAE_DIR/lib -lseqtrace"
      fi
    else
      AC_MSG_WARN([No Extrae installation found in ${EXTRAE_DIR}])
    fi
  fi
])



# AX_PROG_ENABLE_OTF2PRV
# ----------------------
AC_DEFUN([AX_PROG_ENABLE_OTF2PRV],
[
  AC_ARG_ENABLE(otf2prv,
    AC_HELP_STRING([--enable-otf2prv], [Enable otf2prv compilation. (Disabled by default)]),
    [enable_otf2prv="${enableval}"],
    [enable_otf2prv="no"]
  )
])



# AX_PROG_WITH_OTF2
# -----------------
AC_DEFUN([AX_PROG_WITH_OTF2],
[
  AC_ARG_WITH(otf2,
    AC_HELP_STRING(
      [--with-otf2@<:@=DIR@:>@],
      [Specify otf2 library base install directory. Default: /usr/local]
    ),
    [OTF2_DIR=${withval}],
    [OTF2_DIR=/usr/local]
  )
  CPPFLAGS_OTF2="-I$OTF2_DIR/include"
  CXXFLAGS_OTF2="-I$OTF2_DIR/include"
  CFLAGS_OTF2="-I$OTF2_DIR/include"
  LIBS_OTF2="-L$OTF2_DIR/lib"
  LDFLAGS_OTF2="-L$OTF2_DIR/lib -lotf2"
])

