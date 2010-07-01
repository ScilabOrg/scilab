#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2010 - Allan CORNET
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=history_browser


#### history_browser : Conf files ####
libscihistory_browser_la_rootdir = $(mydatadir)
libscihistory_browser_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### history_browser : init scripts ####
libscihistory_browser_la_etcdir = $(mydatadir)/etc
libscihistory_browser_la_etc_DATA = etc/history_browser.quit \
				etc/history_browser.start

include $(top_srcdir)/Makefile.incl.am


HISTORY_BROWSER_JNI_SOURCES = 

HISTORY_BROWSER_C_SOURCES = 

HISTORY_BROWSER_CPP_SOURCES = 

GIWS_WRAPPERS = 

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = 

GATEWAY_CPP_SOURCES = 

libscihistory_browser_la_CFLAGS= 


libscihistory_browser_la_CPPFLAGS = 

# Without the history_browser module 
libscihistory_browser_disable_la_CFLAGS = 

HISTORY_BROWSER_DISABLE_C_SOURCES = 
libscihistory_browser_disable_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscihistory_browser_disable_la_SOURCES = $(HISTORY_BROWSER_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscihistory_browser-disable.la

if GUI
pkglib_LTLIBRARIES += libscihistory_browser.la 
endif

libscihistory_browser_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscihistory_browser_la_SOURCES = $(HISTORY_BROWSER_C_SOURCES) $(HISTORY_BROWSER_JNI_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(HISTORY_BROWSER_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(HISTORY_BROWSER_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscihistory_browser_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = 

if SWIG
BUILT_SOURCES=swig
endif

libscihistory_browser_la_LIBADD = $(top_builddir)/modules/core/libscicore.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### history_browser : gateway declaration ####
libscihistory_browser_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscihistory_browser_la_sci_gateway_DATA = 

if GUI
USEANT=1
endif
