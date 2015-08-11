#!/bin/sh

echo "aclocal"
aclocal -I m4
echo "autoheader"
autoheader
echo "autoconf"
autoconf
echo "libtoolize --automake -c"
case `uname` in Darwin*) glibtoolize --automake -c ;;
  *) libtoolize --automake -c ;; esac
echo "automake -a -c"
automake -a -c

