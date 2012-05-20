# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

GATEWAY_C_SOURCES = 

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscidouble.la
else
noinst_LTLIBRARIES = libscidouble.la
endif


libscidouble_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

libscidouble_la_CFLAGS = 	-I$(top_srcdir)/modules/output_stream/includes \
				-I$(top_srcdir)/modules/elementary_functions/includes \
				-I$(top_srcdir)/modules/operations/includes \
				-I$(top_srcdir)/modules/double/includes

libscidouble_la_SOURCES = $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscidouble_la_CFLAGS)



#### Target ######
modulename=double


#### double : Conf files ####
libscidouble_la_rootdir = $(mydatadir)
libscidouble_la_root_DATA =  license.txt


#### double : init scripts ####
libscidouble_la_etcdir = $(mydatadir)/etc
libscidouble_la_etc_DATA = etc/double.quit etc/double.start

include $(top_srcdir)/Makefile.incl.am
