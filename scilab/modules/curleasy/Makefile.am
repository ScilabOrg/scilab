# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.


BOOLEAN_C_SOURCES = src/c/curleasy_wrap.c


BOOLEAN_FORTRAN_SOURCES = 

GATEWAY_C_SOURCES = sci_gateway/c/licburleasylib.c


GATEWAY_FORTRAN_SOURCES = 

libcurleasy_la_CPPFLAGS= -I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				$(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libcurleasy.la
else
noinst_LTLIBRARIES = libcurleasy.la
endif



libcurleasy_algo_la_SOURCES = $(BOOLEAN_C_SOURCES) 
libcurleasy_la_SOURCES = $(GATEWAY_C_SOURCES) 
libcurleasy_algo_la_CPPFLAGS = $(libcurleasy_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(BOOLEAN_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libcurleasy_la_CPPFLAGS)

libcurleasy_la_LIBADD = libcurleasy.la

modulename=curleasy

#### curleasy : Conf files ####
libcurleasy_la_rootdir = $(mydatadir)
libcurleasy_la_root_DATA =  license.txt


#### curleasy : init scripts ####
libcurleasy_la_etcdir = $(mydatadir)/etc
libcurleasy_la_etc_DATA = etc/curleasy.quit etc/curleasy.start

#### curleasy : gateway declaration ####
libcurleasy_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libcurleasy_la_sci_gateway_DATA = sci_gateway/curleasy_gateway.xml

include $(top_srcdir)/Makefile.incl.am
