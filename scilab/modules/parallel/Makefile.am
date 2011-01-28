# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2010 - DIGITEO - Allan CORNET
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

PARALLEL_CPP_SOURCES = \
src/cpp/SharedData.cpp \
src/cpp/ProcessesRunner.cpp \
src/cpp/ThreadsRunner.cpp \
src/cpp/ConcurrencyState.cpp \
src/cpp/ParallelRunner.cpp

GATEWAY_CPP_SOURCES = sci_gateway/cpp/parallel_gw.cpp \
					sci_gateway/cpp/sci_parallel_run.cpp \
					sci_gateway/cpp/sci_parallel_concurrency.cpp


PARALLEL_C_SOURCES =

GATEWAY_C_SOURCES =


libsciparallel_la_CFLAGS= $(OPENMP_CFLAGS) -O0


libsciparallel_la_CPPFLAGS= $(OPENMP_CXXFLAGS) \
					-Iincludes/ \
					-I$(top_srcdir)/modules/types/includes \
					-I$(top_srcdir)/modules/symbol/includes \
					-I$(top_srcdir)/modules/functions_manager/includes \
					-I$(top_srcdir)/modules/output_stream/includes \
					-I$(top_srcdir)/modules/operations/includes \
					-I$(top_srcdir)/modules/system_env/includes \
					-I$(top_srcdir)/modules/string/includes \
					-I$(top_srcdir)/modules/abstractSyntaxTree/includes


pkglib_LTLIBRARIES = libsciparallel.la

libsciparallel_la_LDFLAGS = $(OPENMP_LIBS) -version-info $(SCILAB_LIBRARY_VERSION)

if OPENMP
libsciparallel_la_SOURCES = $(PARALLEL_C_SOURCES) $(PARALLEL_CPP_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
else
libsciparallel_la_SOURCES = src/noparallel/noparallel.h \
src/noparallel/noparallel.c
endif

# For the code check (splint)
CHECK_SRC= $(PARALLEL_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciparallel_la_CFLAGS)

#### Target ######
modulename=parallel

#### parallel : Conf files ####
libsciparallel_la_rootdir = $(mydatadir)
libsciparallel_la_root_DATA = license.txt readme.txt version.xml


#### parallel : init scripts ####
libsciparallel_la_etcdir = $(mydatadir)/etc
libsciparallel_la_etc_DATA = etc/parallel.quit etc/parallel.start

#### parallel : gateway declaration ####
libsciparallel_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciparallel_la_sci_gateway_DATA = sci_gateway/parallel_gateway.xml

#### parallel : include files ####
libsciparallel_la_includedir=$(pkgincludedir)
libsciparallel_la_include_HEADERS =

include $(top_srcdir)/Makefile.incl.am


