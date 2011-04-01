#!/bin/sh

echo "aclocal"
aclocal -I m4
echo "autoheader"
autoheader
echo "autoconf"
autoconf
echo "libtoolize --automake -c"
libtoolize --automake -c
echo "automake -a -c"
automake -a -c

