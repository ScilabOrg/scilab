# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
#
# This file is distributed under the same license as the Scilab package.

modulename=curl

#### curl : Conf files ####
libscicurl_la_rootdir = $(mydatadir)
libscicurl_la_root_DATA = 


#### curl : init scripts ####
libscicurl_la_etcdir = $(mydatadir)/etc
libscicurl_la_etc_DATA = etc/curl.quit etc/curl.start

#### curl : gateway declaration ####
libscicurl_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscicurl_la_sci_gateway_DATA = sci_gateway/curl_gateway.xml

#### curl : librairies ####

CURL_C_SOURCES = 

GATEWAY_C_SOURCES = \
    sci_gateway/c/gw_curl.c \
    sci_gateway/c/curleasy_wrap.c

libscicurl_la_CPPFLAGS= -I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/mexlib/includes/ \
				$(CURL_CFLAGS) \
				$(AM_CPPFLAGS)

libscicurl_algo_la_CPPFLAGS = $(libscicurl_la_CPPFLAGS)

pkglib_LTLIBRARIES = libscicurl.la
noinst_LTLIBRARIES = libscicurl-algo.la

libscicurl_algo_la_SOURCES = $(CURL_C_SOURCES) 
libscicurl_la_SOURCES = $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(CURL_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscicurl_la_CPPFLAGS)

libscicurl_la_LIBADD = libscicurl-algo.la $(CURL_LIBS)

include $(top_srcdir)/Makefile.incl.am

