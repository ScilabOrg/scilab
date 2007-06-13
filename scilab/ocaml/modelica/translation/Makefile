#
#   Translator from Modelica 2.x to flat Modelica
# 
#   Copyright (C) 2005 – 2007 Imagine S.A.
#   For more information or commercial use please contact us at www.amesim.com
# 
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation; either version 2
#   of the License, or (at your option) any later version.
# 
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
# 
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
# 
#

OCAMLC=ocamlc
OCAMLOPT=ocamlopt
OCAMLDEP=ocamldep
CAMLP4=camlp4
INCLUDES=-I ../parsing -I ../compilation -I ../instantiation -I ../exceptionHandling

ifeq ($(RELEASE),yes)
OCAMLFLAGS=$(INCLUDES)
else
OCAMLFLAGS=-g $(INCLUDES)
endif

OCAMLOPTFLAGS=$(INCLUDES)
LIBS=../parsing/parsing.cma ../compilation/compilation.cma \
	../instantiation/instantiation.cma ../exceptionHandling/exceptionHandling.cma \
	unix.cma
LIBSOPT=$(LIBS:.cma=.cmxa)

SRCS=codeGeneration.ml

OBJS=$(SRCS:.ml=.cmo)
OBJSOPT=$(SRCS:.ml=.cmx)

include ../Make.machdeps

EXE=translator$(EXE_EXT)

.SUFFIXES: .ml .mli .cmo .cmi .cmx .mlp .mpi

%.cmo:	%mlp
	$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -impl $< > $(@:.cmo=.tmp)
	$(OCAMLC) $(OCAMLFLAGS) -c -impl $(@:.cmo=.tmp)
	rm -f $(@:.cmo=.tmp)

%.cmi:	%.mpi
	$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -intf $< > $(@:.cmi=.tmp)
	$(OCAMLC) $(OCAMLFLAGS) -c -intf $(@:.cmi=.tmp)
	rm -f $(@:.cmi=.tmp)

%.cmx:	%.mlp
	$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -impl $< > $(@:.cmx=.tmp)
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -c -impl $(@:.cmx=.tmp)
	rm -f $(@:.cmx=.tmp)

%.cmo:	%.ml %.cmi
	$(OCAMLC) $(OCAMLFLAGS) -c $<

%.cmi:	%.mli
	$(OCAMLC) $(OCAMLFLAGS) -c $<

%.cmx:	%.ml
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -c $<


all: bytecode

opt: $(OBJSOPT)
	$(OCAMLOPT) -verbose $(OCAMLOPTFLAGS) -o translator $(LFLAGS) $(LIBSOPT) $(OBJSOPT)  translator.ml
	mkdir -p $(INSTDIR)
	cp -p  $(EXE) $(INSTDIR)

bytecode: $(OBJS)
	$(OCAMLC) $(OCAMLFLAGS) -o translator $(LFLAGS) $(LIBS) $(OBJS)	translator.ml

custom: $(OBJS)
	$(OCAMLC) -verbose -custom $(OCAMLFLAGS) -o translator $(LFLAGS) $(LIBS) $(OBJS) translator.ml 
	mkdir -p $(INSTDIR)
	cp -p  $(EXE) $(INSTDIR)

parser.ml : parser.mly
	ocamlyacc parser.mly
	rm -f parser.mli

lexer.ml : lexer.mll
	ocamllex lexer.mll

linenum.ml : linenum.mll
	ocamllex linenum.mll

depend:
	$(OCAMLDEP) $(INCLUDES) $(SRCS:.ml=.mli) $(SRCS) > .depend

-include .depend

clean:
	rm -f *.cmxa
	rm -f *.cm[aiox]
	rm -f *.o
	rm -f *.obj
	rm -f translator
	rm -f translator.exe
	rm -f *~
