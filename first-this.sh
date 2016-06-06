#!/bin/sh

set -x

aclocal -I m4
autoheader
autoconf
case `uname` in Darwin*) glibtoolize --automake -c ;;
  *) libtoolize --automake -c ;; esac
automake -a -c

