#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) Scilab Enterprises - 2011 - Clement David
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

## Process this file with automake to produce Makefile.in (which will be changed by the ./configure)
NULL =

iconsdir=$(pkgdatadir)/desktop/
icons_DATA = $(top_srcdir)/desktop/scilab.png \
$(top_srcdir)/desktop/puffin.png \
$(top_srcdir)/desktop/puffin.icns \
$(top_srcdir)/desktop/scilab.icns

# Desktop file
# Note that here, it is datadir and NOT pkgdatadir
desktopdir = $(datadir)/applications
desktop_DATA = $(srcdir)/scilab-adv-cli.desktop \
$(srcdir)/scilab-cli.desktop \
$(srcdir)/scilab.desktop

# Mime types descriptor file
# Note that here, it is datadir and NOT pkgdatadir
mimedir = $(datadir)/mime/packages
mime_DATA = $(srcdir)/scilab.xml

# Mime types descriptor icons
# Note that here, it is datadir and NOT pkgdatadir
if test -x CONVERT -and -x IDENTIFY; then
install-data-local: create-default-icons create-mime-icons install-icons
uninstall-local: uninstall-icons
endif

install-icons:
	for icon in $$(ls $(top_builddir)/desktop/icons/*_*_*_*.png); do \
		THEME=`echo $$icon | cut -d_ -f1`; \
		CONTEXT=`echo $$icon | cut -d_ -f2`; \
		SIZE=`echo $$icon | cut -d_ -f3`; \
		ICONFILE=`echo $$icon | cut -d_ -f4`; \
		mkdir -p $(DESTDIR)$(datadir)/icons/$$THEME/$$SIZE/$$CONTEXT; \
		$(INSTALL_DATA) $(srcdir)/$$icon $(DESTDIR)$(datadir)/icons/$$THEME/$$SIZE/$$CONTEXT/$$ICONFILE; \
	done;

uninstall-icons:
	for icon in $$(ls $(top_builddir)/desktop/icons/*_*_*_*.png); do \
		THEME=`echo $$icon | cut -d_ -f1`; \
		CONTEXT=`echo $$icon | cut -d_ -f2`; \
		SIZE=`echo $$icon | cut -d_ -f3`; \
		ICONFILE=`echo $$icon | cut -d_ -f4`; \
		rm -f $(DESTDIR)$(datadir)/icons/$$THEME/$$SIZE/$$CONTEXT/$$ICONFILE; \
	done;

create-default-icons:
if test -x CONVERT -and -x IDENTIFY; then
	mkdir -p $(top_builddir)/desktop/hicolor \
	pushd . \
	-@( cd $(top_builddir)/desktop/hicolor ; \
	IDENTIFY -format "%wx%h\n" $(top_builddir)/desktop/*.ico \\
	    |uniq |xargs mkdir -p \
	popd \
	for icon in $$(ls $(top_builddir)/desktop/*.ico); do \
	    out_file=$(top_builddir)/desktop/icons/hicolor_apps_%[filename:size]_scilab.png; \
	    CONVERT $$icon \\
	        -set filename:size "%wx%h" $$out_file \
	done
else
	@echo "Cannot build default icons. Install ImageMagick if you want to build them."
endif

create-mime-icons:
if test -x CONVERT -and -x IDENTIFY; then
	mkdir -p $(top_builddir)/desktop/mimetypes \
	pushd . \
	-@( cd $(top_builddir)/desktop/mimetypes ; \
	IDENTIFY -format "%wx%h\n" $(top_builddir)/modules/windows_tools/src/c/resources/*.ico \\
	    |uniq |xargs mkdir -p \
	popd \
	for icon in $$(ls $(top_builddir)/modules/windows_tools/src/c/resources/*.ico); do \
	    app=$$(echo $$icon | sed "s|.ico$||; s|[a-z/_]*icon_||"); \
	    out_file=$(top_builddir)/desktop/icons/hicolor_mimetypes_%[filename:size]_application-$$app.png; \
	    CONVERT $$icon \\
	        -set filename:size "%wx%h" $$out_file \
	done
else
	@echo "Cannot build mime icons. Install ImageMagick if you want to build them."
endif

