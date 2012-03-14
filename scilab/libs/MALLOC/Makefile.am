# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.
 

MALLOC_C_SOURCES = src/getmaxMALLOC.c 

libscimalloc_la_includedir=$(pkgincludedir)

libscimalloc_la_include_HEADERS = includes/sci_mem_alloc.h \
includes/MALLOC.h \
includes/getmaxMALLOC.h \
includes/ExportImport.h

libscimalloc_la_CFLAGS= 

pkglib_LTLIBRARIES = libscimalloc.la

libscimalloc_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

libscimalloc_la_SOURCES = $(MALLOC_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(MALLOC_C_SOURCES)
INCLUDE_FLAGS = $(libscimalloc_la_CFLAGS)

libscimalloc_la_LIBADD = 

#### Target ######
modulename=malloc

include $(top_srcdir)/Makefile.incl.am
