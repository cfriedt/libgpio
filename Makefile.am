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

# XXX: @CF: The only directory we need to operate on recusively is 'include'
# because nobase_include_HEADERS does not install headers to the correct
# locations otherwise.
SUBDIRS = \
	include

ACLOCAL_AMFLAGS = -Im4

AM_CPPFLAGS =
AM_CPPFLAGS += -Iinclude

AM_CXXFLAGS = -std=c++11

lib_LTLIBRARIES =
noinst_LTLIBRARIES =
noinst_PROGRAMS =

TESTS =

include example/Makefile.am
include src/Makefile.am
include test/Makefile.am
