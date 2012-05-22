#!/bin/bash
for f in `ls -a1 *.xml`; do clear; cat $f; echo; echo; echo "Press return"; read $tocatecla; done
