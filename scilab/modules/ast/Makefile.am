##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
##  Copyright (C) 2007-2008 - DIGITEO - Antoine ELIAS
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

pkglib_LTLIBRARIES 	= libsciast.la

libsciast_la_SOURCES = \
	src/cpp/libAst.cpp \
	src/cpp/visitor_common.cpp \
	src/cpp/printvisitor.cpp \
	src/cpp/debugvisitor.cpp \
	src/cpp/mutevisitor.cpp \
	src/cpp/shortcutvisitor.cpp \
	src/cpp/scilabexception.cpp \
	src/cpp/macrovarvisitor.cpp \
	src/cpp/expHistory.cpp

libsciast_la_CPPFLAGS = \
	-Iincludes \
	-Isrc/cpp \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/symbol/includes \
	-I$(top_srcdir)/modules/types/includes \
	-I$(top_srcdir)/modules/functions_manager/includes \
	-I$(top_srcdir)/modules/console/includes \
	-I$(top_srcdir)/modules/elementary_functions/includes \
	-I$(top_srcdir)/modules/operations/includes \
	-I$(top_srcdir)/modules/output_stream/includes \
	-I$(top_srcdir)/modules/system_env/includes \
	-I$(top_srcdir)/modules/api_scilab/includes \
	-I$(top_srcdir)/modules/string/includes \
	-I$(top_srcdir)/modules/threads/includes \
	-I$(top_srcdir)/modules/localization/includes \
    $(EIGEN_CPPFLAGS) \
	$(AM_CPPFLAGS)


libsciast_la_includedir=$(pkgincludedir)
libsciast_la_include_HEADERS =


#### Target ######
modulename=ast

####io : init scripts ####
libsciast_la_etcdir = $(mydatadir)/etc
libsciast_la_etc_DATA = etc/ast.quit etc/ast.start


#### ast : include files ####
libsciast_scilab_la_includedir=$(pkgincludedir)
libsciast_scilab_la_include_HEADERS = \
includes/alldec.hxx \
includes/allexp.hxx \
includes/all.hxx \
includes/allvar.hxx \
includes/anydecs.hxx \
includes/arraylistexp.hxx \
includes/arraylistvar.hxx \
includes/assignexp.hxx \
includes/assignlistexp.hxx \
includes/ast.hxx \
includes/boolexp.hxx \
includes/breakexp.hxx \
includes/callexp.hxx \
includes/caseexp.hxx \
includes/cellcallexp.hxx \
includes/cellexp.hxx \
includes/colonvar.hxx \
includes/commentexp.hxx \
includes/constexp.hxx \
includes/continueexp.hxx \
includes/controlexp.hxx \
includes/debugvisitor.hxx \
includes/dec.hxx \
includes/decls.hxx \
includes/decs.hxx \
includes/dollarvar.hxx \
includes/doubleexp.hxx \
includes/dummyvisitor.hxx \
includes/dynlib_ast_tools.hxx \
includes/execvisitor.hxx \
includes/expHistory.hxx \
includes/exp.hxx \
includes/fieldexp.hxx \
includes/floatexp.hxx \
includes/forexp.hxx \
includes/functiondec.hxx \
includes/ifexp.hxx \
includes/intexp.hxx \
includes/listexp.hxx \
includes/location.hxx \
includes/logicalopexp.hxx \
includes/macrovarvisitor.hxx \
includes/mathexp.hxx \
includes/matrixexp.hxx \
includes/matrixlineexp.hxx \
includes/mutevisitor.hxx \
includes/nilexp.hxx \
includes/notexp.hxx \
includes/opexp.hxx \
includes/printvisitor.hxx \
includes/returnexp.hxx \
includes/run_AssignExp.hxx \
includes/run_CallExp.hxx \
includes/run_MatrixExp.hxx \
includes/run_OpExp.hxx \
includes/runvisitor.hxx \
includes/scilabexception.hxx \
includes/selectexp.hxx \
includes/seqexp.hxx \
includes/shortcutvisitor.hxx \
includes/simplevar.hxx \
includes/stepvisitor.hxx \
includes/stringexp.hxx \
includes/timedvisitor.hxx \
includes/token.hxx \
includes/transposeexp.hxx \
includes/trycatchexp.hxx \
includes/vardec.hxx \
includes/var.hxx \
includes/visitor_common.hxx \
includes/visitor.hxx \
includes/whileexp.hxx

include $(top_srcdir)/Makefile.incl.am
