#
# Copyright (c) 2020      Intel, Inc.  All rights reserved.
# Copyright (c) 2021      IBM Corporation.  All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

AM_CPPFLAGS = -I$(top_builddir)/src/include

bin_PROGRAMS = mpirc

include_HEADERS = include/mpirshim.h

#
# libmpirshim[.so|.a]
#
lib_LTLIBRARIES = libmpirshim.la
libmpirshim_la_SOURCES = mpirshim.c include/mpirshim.h
libmpirshim_la_LDFLAGS = $(pmix_LDFLAGS) -version-info $(libmpirshim_so_version)

#
# C version
#
mpirc_SOURCES = mpirc.c include/mpirshim.h
mpirc_CFLAGS = $(pmix_CFLAGS)
mpirc_CPPFLAGS = $(pmix_CPPFLAGS)
mpirc_LDFLAGS = $(pmix_LDFLAGS) -static
mpirc_LDADD =  $(pmix_LIBS) libmpirshim.la

#
# Testing library
#
noinst_LTLIBRARIES = libmpirshimtest.la
libmpirshimtest_la_SOURCES = mpirshim.c include/mpirshim.h include/mpirshim_test.h
libmpirshimtest_la_CFLAGS = $(pmix_CFLAGS) -DMPIR_SHIM_TESTCASE
libmpirshimtest_la_CPPFLAGS = $(pmix_CPPFLAGS) -DMPIR_SHIM_TESTCASE
libmpirshimtest_la_LDFLAGS = $(pmix_LDFLAGS)
