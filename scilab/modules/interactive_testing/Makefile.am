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
modulename=interactive_testing


#### interactive_testing : Conf files ####
libsciinteractive_testing_la_rootdir = $(mydatadir)
libsciinteractive_testing_la_root_DATA =  license.txt version.xml


#### interactive_testing : init scripts ####
libsciinteractive_testing_la_etcdir = $(mydatadir)/etc
libsciinteractive_testing_la_etc_DATA = etc/interactive_testing.quit \
				etc/interactive_testing.start

include $(top_srcdir)/Makefile.incl.am

if GUI
USEANT=1
endif
