#!/bin/sh

echo "aclocal"
aclocal
echo "autoheader"
autoheader
echo "autoconf"
autoconf
echo "automake -a -c"
automake -a -c
echo "libtoolize --automake -c"
libtoolize --automake -c

