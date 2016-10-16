# The MIT License (MIT)
# 
# Copyright (c) 2016, Christopher Friedt <chrisfriedt@gmail.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

#/bin/sh

if [ "$1" = "clean" ]; then
  make clean &>/dev/null
  make distclean &>/dev/null
  make maintainer-clean &>/dev/null
  rm -Rf \
    $(find * \
      -name '*~' \
      -o -name 'config.h' \
      -o -name 'config.h.in' \
      -o -name 'Makefile.in' \
      -o -name 'aclocal.m4' \
      -o -name 'ar-lib' \
      -o -name 'autom4te.cache' \
      -o -name 'compile' \
      -o -name 'config.guess' \
      -o -name 'config.sub' \
      -o -name 'configure' \
      -o -name 'depcomp' \
      -o -name 'install-sh' \
      -o -name 'ltmain.sh' \
      -o -name 'libtool.m4' \
      -o -name 'ltoptions.m4' \
      -o -name 'ltsugar.m4' \
      -o -name 'ltversion.m4' \
      -o -name 'lt~obsolete.m4' \
      -o -name 'missing' \
      -o -name 'stamp-h1' \
      -o -name 'test-driver' \
      -o -name '.deps' \
      -o -name '.libs' \
      -o -name '.dirstamp' \
      -o -name '*.o' \
      -o -name '*.lo' \
      -o -name '*.la' \
      -o -name '*.log' \
      -o -name '*.scan' \
  )
  exit 0
fi

autoreconf -vif
