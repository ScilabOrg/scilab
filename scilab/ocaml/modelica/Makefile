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

MACHINE = $(shell uname)

# The standard is to create a opt (machinecode) version
BINTYPE = opt

ifeq ($(MACHINE),HP-UX)
THIS_IS_UNIX = yes
MACHDIR  = hp
export PATH :=/opt/hp-gcc/3.3.6/bin:$(PATH)
endif

ifeq ($(MACHINE),SunOS)
THIS_IS_UNIX = yes
MACHDIR  = sun
endif

ifeq ($(MACHINE),IRIX64)
THIS_IS_UNIX = yes
MACHDIR  = sgi
endif

ifeq ($(MACHINE),AIX)
THIS_IS_UNIX = yes
MACHDIR  = ibm
# For IBM we cannot run ocamlopt, we need to use the "-custom" option.
BINTYPE  = custom
endif

ifeq ($(MACHINE),Linux)
THIS_IS_UNIX = yes
MACHDIR  = lnx
endif

all: debug


release: 
ifeq ($(THIS_IS_UNIX),yes)
	$(MAKE) $(BINTYPE) PATH=${PATH}:/amedev/tools/ocaml/$(MACHDIR)/bin RELEASE=yes
else
	$(MAKE) $(BINTYPE) RELEASE=yes
endif

debug:
ifeq ($(THIS_IS_UNIX),yes)
	$(MAKE) bytecode PATH=${PATH}:/amedev/tools/ocaml/$(MACHDIR)/bin RELEASE=no
else
	$(MAKE) bytecode RELEASE=no
endif

depend:
ifeq ($(THIS_IS_UNIX),yes)
	$(MAKE) depends PATH=${PATH}:/amedev/tools/ocaml/$(MACHDIR)/bin RELEASE=yes
else
	$(MAKE) depends RELEASE=yes
endif

depends:
	cd parsing ; $(MAKE) depend 
	cd compilation ; $(MAKE) depend
	cd instantiation ; $(MAKE) depend
	cd exceptionHandling ; $(MAKE) depend
	cd translation ; $(MAKE) depend

opt: 
	$(MAKE) depend
	cd parsing ; $(MAKE)  opt  RELEASE=$(RELEASE)
	cd compilation ; $(MAKE)  opt RELEASE=$(RELEASE)
	cd instantiation ; $(MAKE)  opt RELEASE=$(RELEASE)
	cd exceptionHandling ; $(MAKE) opt RELEASE=$(RELEASE)
	cd translation ; $(MAKE) opt RELEASE=$(RELEASE)

custom: 
	$(MAKE) depend
	cd parsing ; $(MAKE)  bytecode RELEASE=$(RELEASE)
	cd compilation ; $(MAKE) bytecode RELEASE=$(RELEASE)
	cd instantiation ; $(MAKE) bytecode RELEASE=$(RELEASE)
	cd exceptionHandling ; $(MAKE) bytecode RELEASE=$(RELEASE)
	cd translation ; $(MAKE)  custom RELEASE=$(RELEASE)

bytecode: 
	$(MAKE) depend
	cd parsing ; $(MAKE)  bytecode RELEASE=$(RELEASE)
	cd compilation ; $(MAKE)  bytecode RELEASE=$(RELEASE)
	cd instantiation ; $(MAKE) bytecode RELEASE=$(RELEASE)
	cd exceptionHandling ; $(MAKE) bytecode RELEASE=$(RELEASE)
	cd translation ; $(MAKE)  bytecode RELEASE=$(RELEASE)

clean:
	cd parsing ; $(MAKE) clean
	cd compilation ; $(MAKE) clean
	cd instantiation ; $(MAKE) clean
	cd exceptionHandling ; $(MAKE) clean
	cd translation ; $(MAKE) clean
