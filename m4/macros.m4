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
    PCFPARSER_CFLAGS="-DOLD_PCFPARSER -I../../common-files/pcfparser"
  else
    AC_DEFINE([SET_OLD_PCFPARSER], 0, [Old pcfparser enabled by user.])
    PCFPARSER_CFLAGS="-I../../common-files/pcfparser/libtools"
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
    CPPFLAGS_EXTRAE="-I$EXTRAE_DIR/include"
    CXXFLAGS_EXTRAE="-I$EXTRAE_DIR/include"
    CFLAGS_EXTRAE="-I$EXTRAE_DIR/include"
    LIBS_EXTRAE="-L$EXTRAE_DIR/lib"
    LDFLAGS_EXTRAE="-L$EXTRAE_DIR/lib -lseqtrace"
  fi
])


# AX_PROG_ENABLE_PARALLEL
# -----------------------
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
    LDFLAGS="$LDFLAGS --instrument -fPIC"
  else
    PRVCC="gcc xlc cc"
    PRVCXX="g++ xlC CC"
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


# AX_PROG_WITH_PTOOLS_COMMON_FILES
# -----------
AC_DEFUN([AX_PROG_WITH_PTOOLS_COMMON_FILES],
[
   AC_ARG_WITH(ptools_common_files,
      AC_HELP_STRING(
         [--with-ptools-common-files@<:@=DIR@:>@],
         [specify ptools_common_files library base install directory. Default: /usr/local]
      ),
      [PTOOLS_COMMON_FILES_DIR=${withval}],
      [PTOOLS_COMMON_FILES_DIR=/usr/local]
   )
   CPPFLAGS_PTOOLS="-I$PTOOLS_COMMON_FILES_DIR/include"
   CXXFLAGS_PTOOLS="-I$PTOOLS_COMMON_FILES_DIR/include"
   CFLAGS_PTOOLS="-I$PTOOLS_COMMON_FILES_DIR/include"

   if test -d "$PTOOLS_COMMON_FILES_DIR/lib"
   then
     LIBS_PTOOLS="-L$PTOOLS_COMMON_FILES_DIR/lib/ptools_common_files"
     LDFLAGS_PTOOLS="-L$PTOOLS_COMMON_FILES_DIR/lib/ptools_common_files -lptools_common_files"
   elif test -d "$PTOOLS_COMMON_FILES_DIR/lib64"
   then
     LIBS_PTOOLS="-L$PTOOLS_COMMON_FILES_DIR/lib64/ptools_common_files"
     LDFLAGS_PTOOLS="-L$PTOOLS_COMMON_FILES_DIR/lib64/ptools_common_files -lptools_common_files"
   fi
])

