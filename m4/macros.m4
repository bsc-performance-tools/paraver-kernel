# AX_PROG_ENABLE_TRACING
# -----------
AC_DEFUN([AX_PROG_ENABLE_TRACING],
[
  AC_ARG_ENABLE(tracing,
    AC_HELP_STRING(
      [--enable-tracing],
      [Enable tracing generation. (Disabled by default)]
    ),
    [enable_tracing="${enableval}"],
    [enable_tracing="no"]
  )
  if test "${enable_tracing}" = "yes" ; then
    AC_DEFINE([TRACING_ENABLED], 1, [Tracing enabled by user.])
    CPPFLAGS="$CPPFLAGS -g"
    CXXFLAGS="$CXXFLAGS -g"
    CFLAGS="$CFLAGS -g"
  fi
])

# AX_PROG_WITH_EXTRAE
# -----------
AC_DEFUN([AX_PROG_WITH_EXTRAE],
[
  AC_ARG_WITH(extrae,
    AC_HELP_STRING(
      [--with-extrae@<:@=DIR@:>@],
      [Specify extrae library base install directory. Default: /usr/local]
    ),
    [EXTRAE_DIR=${withval}],
    [EXTRAE_DIR=/usr/local]
  )
  if test "${enable_tracing}" = "yes" ; then
    CPPFLAGS="$CPPFLAGS -I$EXTRAE_DIR/include"
    CXXFLAGS="$CXXFLAGS -I$EXTRAE_DIR/include"
    CFLAGS="$CFLAGS -I$EXTRAE_DIR/include"
    LIBS="$LIBS -L$EXTRAE_DIR/lib"
    LDFLAGS="$LDFLAGS -L$EXTRAE_DIR/lib -lseqtrace"
  fi
])

# AX_PROG_ENABLE_PARALLEL
# -----------
AC_DEFUN([AX_PROG_ENABLE_PARALLEL],
[
  AC_ARG_ENABLE(parallel,
    AC_HELP_STRING(
      [--enable-parallel],
      [Enable parallel version. (Disabled by default)]
    ),
    [enable_parallel="${enableval}"],
    [enable_parallel="no"]
  )
  if test "${enable_parallel}" = "yes" ; then
    AC_DEFINE([PARALLEL_ENABLED], 1, [Parallel version enabled by user.])
    PRVCC=sscc
    PRVCXX=sscxx
    #CC=sscc
    #CXX=sscxx
    CXXFLAGS="$CXXFLAGS --instrument"
    LDFLAGS="$LDFLAGS --instrument"
  else
    PRVCC="gcc xlc cc"
    PRVCXX="g++ xlC CC"
  fi
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
