
# Process this file with autoconf to produce a configure script.

#########################
#    Initializations    #
#########################

# Initialize autoconf & define package name, version and bug-report address
AC_INIT(paraver-kernel, 1.0, tools@bsc.es)

# Safety check to ensure that the directory told with `--srcdir` contains the source code
AC_CONFIG_SRCDIR(src/)

# Initialize automake
AM_INIT_AUTOMAKE([subdir-objects])

# Specify the output configuration header file
AC_CONFIG_HEADERS(config.h)

AC_CONFIG_MACRO_DIR([m4])

: ${CFLAGS="-O2 -g"}
: ${CXXFLAGS="-O2 -g"}

#############################
#    Checks for programs    #
#############################

# Search for libtool support
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

AM_PATH_XML2

AX_PROG_WITH_PTOOLS_COMMON_FILES

# Search for available compilers and preprocessor
AC_PROG_CC(gcc xlc cc)
AC_PROG_CXX(g++ xlC CC)
AC_PROG_CPP
AC_PROG_CXXCPP
AC_PROG_SED

AX_BOOST_BASE(1.36)
AX_BOOST_SERIALIZATION

AC_CONFIG_HEADERS(config_traits.h)
AX_PROG_ENABLE_TRACING
AX_PROG_WITH_EXTRAE
AX_PROG_ENABLE_PARALLEL

AX_PROG_ENABLE_OTF2PRV
AM_CONDITIONAL(WANT_OTF2PRV, test "${enable_otf2prv}" = "yes" )
if test "${enable_otf2prv}" = "yes" ; then
  AX_PROG_WITH_OTF2
  AC_SUBST(OTF2_DIR)
fi

AX_PROG_ENABLE_EXTENDED_OBJECTS

AX_PROG_ENABLE_OLD_PCFPARSER
AM_CONDITIONAL(MAKE_OLD_PCFPARSER, test "${enable_old_pcfparser}" = "yes" )


######################################################################
#    Checks for typedefs, structures and compiler characteristics    #
######################################################################
AC_CONFIG_HEADERS(config_types.h)
AC_CHECK_TYPES([long, long long, char, int, short int])
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(short int)
AC_CHECK_TYPES([int64_t, uint64_t, int8_t, uint8_t, int32_t, uint32_t, int16_t, uint16_t])


#########################################
#    Checks for user defined options    #
#########################################

BOOST_RPATH="-R ${BOOST_LDFLAGS#-L}"

CFLAGS="-Iinclude/ -Iapi/ -I../include/ -I../api/ -I../common-files/ -I../../common-files/ $PCFPARSER_CFLAGS $CFLAGS $CFLAGS_PTOOLS $CFLAGS_EXTRAE $XML_CPPFLAGS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE"
CXXFLAGS="-Iinclude/ -Iapi/ -I../include/ -I../api/ -I../common-files/ -I../../common-files/ $PCFPARSER_CFLAGS $CXXFLAGS $BOOST_CPPFLAGS $CXXFLAGS_PTOOLS $CXXFLAGS_EXTRAE $XML_CPPFLAGS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE"
LDFLAGS="$LDFLAGS $BOOST_LDFLAGS $BOOST_SERIALIZATION_LIB $BOOST_RPATH $LDFLAGS_PTOOLS $LDFLAGS_EXTRAE $XML_LIBS"

AC_OUTPUT([Makefile src/Makefile api/Makefile include/Makefile])
