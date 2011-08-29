# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.


if TCLTK

TCLSCI_C_SOURCES = src/c/TCL_Global.c \
src/c/InitTclTk.c \
src/c/ScilabEval.c \
src/c/setenvtcl.c \
src/c/InitializeTclTk.c \
src/c/TerminateTclTk.c \
src/c/setvar.c \
src/c/TCL_ArrayDim.c \
src/c/TCL_ArrayExist.c \
src/c/TCL_ArrayGetVar.c \
src/c/TCL_ArraySize.c \
src/c/TCL_Command.c \
src/c/GlobalTclInterp.c \
src/c/TCL_getErrorLine.c

GATEWAY_C_SOURCES =  sci_gateway/c/sci_opentk.c \
sci_gateway/c/sci_TCL_SetVar.c \
sci_gateway/c/sci_TCL_ExistVar.c \
sci_gateway/c/sci_TCL_DoOneEvent.c \
sci_gateway/c/sci_TCL_UnsetVar.c \
sci_gateway/c/sci_TCL_ExistInterp.c \
sci_gateway/c/gw_tclsci.c \
sci_gateway/c/sci_TCL_EvalStr.c \
sci_gateway/c/sci_TCL_DeleteInterp.c \
sci_gateway/c/sci_TCL_GetVersion.c \
sci_gateway/c/sci_TCL_EvalFile.c \
sci_gateway/c/sci_TCL_GetVar.c \
sci_gateway/c/sci_TCL_ExistArray.c \
sci_gateway/c/sci_TCL_UpVar.c

else

TCLSCI_C_SOURCES = src/notclsci/notclsci.c

GATEWAY_C_SOURCES =

endif

pkglib_LTLIBRARIES = libscitclsci.la
noinst_LTLIBRARIES = libscitclsci-algo.la

libscitclsci_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(TCLTK_LIBS)

libscitclsci_la_CFLAGS= -I$(srcdir)/includes/ \
			-I$(srcdir)/src/c \
			-I$(srcdir)/sci_gateway/c/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			$(TCL_INC_PATH) \
			$(TK_INC_PATH) \
			-I$(top_srcdir)/modules/action_binding/includes \
			-I$(top_srcdir)/modules/fileio/includes \
			-I$(top_srcdir)/modules/call_scilab/includes \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/localization/includes

libscitclsci_algo_la_SOURCES = $(TCLSCI_C_SOURCES)
libscitclsci_la_SOURCES = $(GATEWAY_C_SOURCES)
libscitclsci_algo_la_CFLAGS = $(libscitclsci_la_CFLAGS)

# For the code check (splint)
CHECK_SRC= $(libscitclsci_la_SOURCES)
INCLUDE_FLAGS = $(libscitclsci_la_CFLAGS)

libscitclsci_la_LIBADD = libscitclsci-algo.la $(top_builddir)/modules/fileio/libscifileio.la \
							$(top_builddir)/libs/MALLOC/libscimalloc.la \
							$(top_builddir)/modules/output_stream/libscioutput_stream.la
#### Target ######
modulename=tclsci

#### tclsci : Conf files ####
libscitclsci_la_rootdir = $(mydatadir)
libscitclsci_la_root_DATA =  license.txt 

#### tclsci : init scripts ####
libscitclsci_la_etcdir = $(mydatadir)/etc
libscitclsci_la_etc_DATA = etc/tclsci.quit etc/tclsci.start

### TCL macros ###
libscitclsci_la_tcldir = $(mydatadir)
nobase_libscitclsci_la_tcl_DATA = tcl/utils/Notebook.tcl \
tcl/utils/Balloon.tcl \
tcl/utils/Combobox.tcl \
tcl/sciGUI/data/initial.help \
tcl/sciGUI/data/dontshow.txt \
tcl/sciGUI/data/tp00.gif \
tcl/sciGUI/data/tp01.gif \
tcl/sciGUI/data/tp10.gif \
tcl/sciGUI/data/tp02.gif \
tcl/sciGUI/data/initial \
tcl/sciGUI/data/tp04.gif \
tcl/sciGUI/data/giftable.txt \
tcl/sciGUI/data/ndemo_tx01.txt \
tcl/sciGUI/data/tp05.gif \
tcl/sciGUI/data/error.gif \
tcl/sciGUI/data/tp06.gif \
tcl/sciGUI/data/tp15.gif \
tcl/sciGUI/data/tp16.gif \
tcl/sciGUI/data/info.gif \
tcl/sciGUI/data/tp08.gif \
tcl/sciGUI/data/tp17.gif \
tcl/sciGUI/data/scilab01.gif \
tcl/sciGUI/data/passwd.gif \
tcl/sciGUI/data/warning.gif \
tcl/sciGUI/data/question.gif \
tcl/sciGUI/data/hourglass.gif \
tcl/sciGUI/data/icontable.txt \
tcl/sciGUI/local_extra/combobox-2.3/ANNOUNCE.txt \
tcl/sciGUI/local_extra/combobox-2.3/combobox.tcl \
tcl/sciGUI/local_extra/combobox-2.3/README.txt \
tcl/sciGUI/local_extra/combobox-2.3/combobox.html \
tcl/sciGUI/local_extra/combobox-2.3/example.tcl \
tcl/sciGUI/local_extra/balloonhelp.tcl \
tcl/sciGUI/local_extra/rnotebook.tcl \
tcl/sciGUI/local_extra/spinbutton.tcl \
tcl/sciGUI/license.txt \
tcl/sciGUI/sciGUI.tcl \
tcl/sciGUI/sciConfig.tcl \
tcl/sciGUI/sciBrowseVar.tcl \
tcl/sciGUI/sciEditVar.tcl \
tcl/BWidget/tests/entry.test \
tcl/BWidget/BWman/SelectColor.html \
tcl/BWidget/BWman/SpinBox.html \
tcl/BWidget/BWman/ButtonBox.html \
tcl/BWidget/BWman/ScrollView.html \
tcl/BWidget/BWman/LabelFrame.html \
tcl/BWidget/BWman/ProgressDlg.html \
tcl/BWidget/BWman/TitleFrame.html \
tcl/BWidget/BWman/Dialog.html \
tcl/BWidget/BWman/Separator.html \
tcl/BWidget/BWman/Entry.html \
tcl/BWidget/BWman/navtree.html \
tcl/BWidget/BWman/SelectFont.html \
tcl/BWidget/BWman/PanelFrame.html \
tcl/BWidget/BWman/Button.html \
tcl/BWidget/BWman/MessageDlg.html \
tcl/BWidget/BWman/PanedWindow.html \
tcl/BWidget/BWman/Label.html \
tcl/BWidget/BWman/options.htm \
tcl/BWidget/BWman/NoteBook.html \
tcl/BWidget/BWman/Widget.html \
tcl/BWidget/BWman/ListBox.html \
tcl/BWidget/BWman/BWidget.html \
tcl/BWidget/BWman/ArrowButton.html \
tcl/BWidget/BWman/PasswdDlg.html \
tcl/BWidget/BWman/ProgressBar.html \
tcl/BWidget/BWman/DropSite.html \
tcl/BWidget/BWman/PagesManager.html \
tcl/BWidget/BWman/ScrollableFrame.html \
tcl/BWidget/BWman/DynamicHelp.html \
tcl/BWidget/BWman/contents.html \
tcl/BWidget/BWman/ScrolledWindow.html \
tcl/BWidget/BWman/MainFrame.html \
tcl/BWidget/BWman/Tree.html \
tcl/BWidget/BWman/LabelEntry.html \
tcl/BWidget/BWman/DragSite.html \
tcl/BWidget/BWman/index.html \
tcl/BWidget/BWman/StatusBar.html \
tcl/BWidget/BWman/ComboBox.html \
tcl/BWidget/images/folder.gif \
tcl/BWidget/images/minus.xbm \
tcl/BWidget/images/paste.gif \
tcl/BWidget/images/dragfile.gif \
tcl/BWidget/images/oplink.xbm \
tcl/BWidget/images/file.gif \
tcl/BWidget/images/bold.gif \
tcl/BWidget/images/underline.gif \
tcl/BWidget/images/dragicon.gif \
tcl/BWidget/images/opmove.xbm \
tcl/BWidget/images/new.gif \
tcl/BWidget/images/error.gif \
tcl/BWidget/images/redo.gif \
tcl/BWidget/images/opcopy.xbm \
tcl/BWidget/images/info.gif \
tcl/BWidget/images/cut.gif \
tcl/BWidget/images/print.gif \
tcl/BWidget/images/overstrike.gif \
tcl/BWidget/images/palette.gif \
tcl/BWidget/images/save.gif \
tcl/BWidget/images/open.gif \
tcl/BWidget/images/passwd.gif \
tcl/BWidget/images/plus.xbm \
tcl/BWidget/images/italic.gif \
tcl/BWidget/images/undo.gif \
tcl/BWidget/images/warning.gif \
tcl/BWidget/images/openfold.gif \
tcl/BWidget/images/hourglass.gif \
tcl/BWidget/images/question.gif \
tcl/BWidget/images/target.xbm \
tcl/BWidget/images/copy.gif \
tcl/BWidget/lang/en.rc \
tcl/BWidget/lang/da.rc \
tcl/BWidget/lang/es.rc \
tcl/BWidget/lang/fr.rc \
tcl/BWidget/lang/de.rc \
tcl/BWidget/mainframe.tcl \
tcl/BWidget/dialog.tcl \
tcl/BWidget/utils.tcl \
tcl/BWidget/wizard.tcl \
tcl/BWidget/labelentry.tcl \
tcl/BWidget/entry.tcl \
tcl/BWidget/init.tcl \
tcl/BWidget/pagesmgr.tcl \
tcl/BWidget/font.tcl \
tcl/BWidget/pkgIndex.tcl \
tcl/BWidget/scrollframe.tcl \
tcl/BWidget/panelframe.tcl \
tcl/BWidget/button.tcl \
tcl/BWidget/panedw.tcl \
tcl/BWidget/label.tcl \
tcl/BWidget/notebook.tcl \
tcl/BWidget/widget.tcl \
tcl/BWidget/listbox.tcl \
tcl/BWidget/scrollw.tcl \
tcl/BWidget/passwddlg.tcl \
tcl/BWidget/scrollview.tcl \
tcl/BWidget/xpm2image.tcl \
tcl/BWidget/arrow.tcl \
tcl/BWidget/progressdlg.tcl \
tcl/BWidget/tree.tcl \
tcl/BWidget/separator.tcl \
tcl/BWidget/dragsite.tcl \
tcl/BWidget/dynhelp.tcl \
tcl/BWidget/CHANGES.txt \
tcl/BWidget/ChangeLog \
tcl/BWidget/combobox.tcl \
tcl/BWidget/statusbar.tcl \
tcl/BWidget/messagedlg.tcl \
tcl/BWidget/bitmap.tcl \
tcl/BWidget/color.tcl \
tcl/BWidget/LICENSE.txt \
tcl/BWidget/spinbox.tcl \
tcl/BWidget/buttonbox.tcl \
tcl/BWidget/dropsite.tcl \
tcl/BWidget/progressbar.tcl \
tcl/BWidget/labelframe.tcl \
tcl/BWidget/README.txt \
tcl/BWidget/titleframe.tcl \
tcl/TK_Scilab.tcl

#### tclsci : gateway declaration ####
libscitclsci_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscitclsci_la_sci_gateway_DATA = sci_gateway/tclsci_gateway.xml

include $(top_srcdir)/Makefile.incl.am

