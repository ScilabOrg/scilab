# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

SPARSE_C_SOURCES = \
src/c/spUtils.c \
src/c/lu.c \
src/c/spFactor.c \
src/c/spBuild.c \
src/c/spOutput.c \
src/c/spAllocate.c \
src/c/spSolve.c
# src/c/spFortran.c

SPARSE_FORTRAN_SOURCES = src/fortran/spcompack.f \
src/fortran/wsposp.f \
src/fortran/dspmax.f \
src/fortran/findl.f \
src/fortran/spcho1.f \
src/fortran/dspful.f \
src/fortran/blkslv.f \
src/fortran/wspcle.f \
src/fortran/wspt.f \
src/fortran/wspssp.f \
src/fortran/lspmat.f \
src/fortran/lsosp.f \
src/fortran/blkfc1.f \
src/fortran/wspxsp.f \
src/fortran/lspful.f \
src/fortran/blkfct.f \
src/fortran/symfct.f \
src/fortran/dspt.f \
src/fortran/wspos.f \
src/fortran/dful2sp.f \
src/fortran/sputil.f \
src/fortran/lspt.f \
src/fortran/dspxs.f \
src/fortran/sp2col.f \
src/fortran/dcompa.f \
src/fortran/sz2ptr.f \
src/fortran/spt.f \
src/fortran/lcompa.f \
src/fortran/wspmat.f \
src/fortran/iperm.f \
src/fortran/wspful.f \
src/fortran/spif1b.f \
src/fortran/spextr.f \
src/fortran/lful2sp.f \
src/fortran/dspos.f \
src/fortran/dspasp.f \
src/fortran/inpnv.f \
src/fortran/spreshape.f \
src/fortran/spsort.f \
src/fortran/wperm.f \
src/fortran/dij2sp.f \
src/fortran/dspcsp.f \
src/fortran/spcho2.f \
src/fortran/lspos.f \
src/fortran/wspe2.f \
src/fortran/spif.f \
src/fortran/lspasp.f \
src/fortran/dspisp.f \
src/fortran/wful2sp.f \
src/fortran/lij2sp.f \
src/fortran/lspcsp.f \
src/fortran/wspis.f \
src/fortran/dspmsp.f \
src/fortran/dsposp.f \
src/fortran/wsmsp.f \
src/fortran/wspms.f \
src/fortran/lspisp.f \
src/fortran/dspcle.f \
src/fortran/wsosp.f \
src/fortran/dspssp.f \
src/fortran/lsposp.f \
src/fortran/dspxsp.f \
src/fortran/ordmmd.f \
src/fortran/spextr1.f \
src/fortran/wcompa.f \
src/fortran/lspxsp.f \
src/fortran/spisp.f \
src/fortran/dperm.f \
src/fortran/spind.f \
src/fortran/dspe2.f \
src/fortran/wspasp.f \
src/fortran/wij2sp.f \
src/fortran/wspcsp.f \
src/fortran/spifp.f \
src/fortran/isort1.f \
src/fortran/dspis.f \
src/fortran/wspxs.f \
src/fortran/lspe2.f \
src/fortran/wspisp.f \
src/fortran/dsmsp.f \
src/fortran/dspms.f \
src/fortran/spord.f \
src/fortran/dsosp.f \
src/fortran/dspmat.f \
src/fortran/wspmsp.f \
src/fortran/lspis.f \
src/fortran/dspmin.f \
src/fortran/writebuf.f \
src/fortran/ta2lpd.f

GATEWAY_C_SOURCES =


libscisparse_la_CFLAGS=  	-I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/elementary_functions/includes \
				-I$(top_srcdir)/modules/operations/includes
#-Isrc/c/
if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscisparse.la
noinst_LTLIBRARIES = libscisparse-algo.la
else
noinst_LTLIBRARIES = libscisparse-algo.la libscisparse.la
endif


libscisparse_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

libscisparse_algo_la_SOURCES = $(SPARSE_C_SOURCES) $(SPARSE_FORTRAN_SOURCES)
libscisparse_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
libscisparse_algo_la_CFLAGS = $(libscisparse_la_CFLAGS)

# For the code check (splint)
CHECK_SRC = $(SPARSE_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscisparse_la_CFLAGS)


libscisparse_la_LIBADD = libscisparse-algo.la

#### Target ######
modulename=sparse


#### sparse : Conf files ####
libscisparse_la_rootdir = $(mydatadir)
libscisparse_la_root_DATA =  license.txt


#### sparse : init scripts ####
libscisparse_la_etcdir = $(mydatadir)/etc
libscisparse_la_etc_DATA = etc/sparse.quit etc/sparse.start

#### sparse : gateway declaration ####
libscisparse_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscisparse_la_sci_gateway_DATA = sci_gateway/sparse_gateway.xml

include $(top_srcdir)/Makefile.incl.am
