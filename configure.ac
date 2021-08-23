
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

AX_PROG_WITH_DEBUG_LEVEL

# Init for macro AX_PROG_WITH_DEBUG
AM_CFLAGS="$CPPFLAGS_DEBUG_LEVEL $AM_CFLAGS"
AM_CXXFLAGS="$CPPFLAGS_DEBUG_LEVEL $AM_CXXFLAGS"


#############################
#    Checks for programs    #
#############################

test "x$prefix" = xNONE && prefix=$ac_default_prefix

# Search for libtool support
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

: ${CFLAGS=""}
: ${CXXFLAGS=""}

# Search for available compilers and preprocessor
AC_PROG_CC(gcc xlc cc)
AC_PROG_CXX(g++ xlC CC)
AC_PROG_CPP
AC_PROG_CXXCPP
AC_PROG_SED

AM_PATH_XML2

AX_BOOST_BASE(1.36)
AX_BOOST_SERIALIZATION
AX_BOOST_DATE_TIME

AC_CONFIG_HEADERS(config_traits.h)
AX_PROG_ENABLE_DEPENDENCIES_AWARE_INSTALL
AX_PROG_ENABLE_EXTENDED_OBJECTS
AX_PROG_ENABLE_OLD_PCFPARSER
AX_PROG_ENABLE_OMPSS
AX_PROG_ENABLE_OPENMP
AX_PROG_ENABLE_MINGW
AX_PROG_WITH_EXTRAE
AX_PROG_WITH_OTF2

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

AM_CFLAGS="-I../include/ -I../api/ -I../ -I../../ $AM_CFLAGS $CFLAGS_EXTRAE $XML_CPPFLAGS $MINGW_CPPFLAGS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG $OPENMP_CXXFLAGS"
AM_CXXFLAGS="-I../include/ -I../api/ -I../ -I../../ $AM_CXXFLAGS $BOOST_CPPFLAGS $CXXFLAGS_EXTRAE $XML_CPPFLAGS $MINGW_CPPFLAGS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG $OPENMP_CXXFLAGS"
AM_LDFLAGS="$AM_LDFLAGS $BOOST_LDFLAGS $BOOST_SERIALIZATION_LIB $BOOST_DATE_TIME_LIB $LDFLAGS_EXTRAE $BOOST_RPATH $XML_LIBS $MINGW_LIBS $OPENMP_LDFLAGS"

AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])

AC_OUTPUT([Makefile src/Makefile api/Makefile include/Makefile utils/Makefile utils/include/Makefile utils/src/Makefile utils/pcfparser/Makefile utils/pcfparser/old/Makefile])
