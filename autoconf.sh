#!/usr/bin/env bash
aclocal
libtoolize
autoheader
automake --add-missing
autoconf
