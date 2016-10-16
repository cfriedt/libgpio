dnl The MIT License (MIT)
dnl 
dnl Copyright (c) 2016, Christopher Friedt <chrisfriedt@gmail.com>
dnl 
dnl Permission is hereby granted, free of charge, to any person obtaining a copy
dnl of this software and associated documentation files (the "Software"), to deal
dnl in the Software without restriction, including without limitation the rights
dnl to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
dnl copies of the Software, and to permit persons to whom the Software is
dnl furnished to do so, subject to the following conditions:
dnl 
dnl The above copyright notice and this permission notice shall be included in all
dnl copies or substantial portions of the Software.
dnl 
dnl THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
dnl IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
dnl FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
dnl AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
dnl LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
dnl OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
dnl SOFTWARE.

AC_PREREQ([2.69])

AC_INIT([libgpio], [0.0.1], [chrisfriedt@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_CONFIG_SRCDIR([include/libgpio/Gpio.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Dependencies
PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.

dnl XXX: @CF: fix gtest-config and gtest.m4
dnl GTEST_LIB_CHECK([1.7.0])
AC_SUBST([HAVE_GTEST],[yes])
GTEST_CPPFLAGS="-I/usr/local/include"
AC_SUBST([GTEST_CPPFLAGS])
GTEST_LDFLAGS="-L/usr/local/lib"
AC_SUBST([GTEST_LDFLAGS])
GTEST_LIBS="-lgtest -lgtest_main"
AC_SUBST([GTEST_LIBS])
AM_CONDITIONAL([HAVE_GTEST],[test "x$HAVE_GTEST" = "xyes"])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UINT16_T

# Checks for library functions.
AC_CHECK_FUNCS([memset])

AM_PROG_AR

LT_INIT()

AC_CONFIG_FILES([
	Makefile
	include/Makefile
	libgpio.pc
	libgpio++.pc
])
AC_OUTPUT
