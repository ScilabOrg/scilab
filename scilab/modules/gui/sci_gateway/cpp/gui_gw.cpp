/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gui_gw.hxx"
#include "function.hxx"
#include "context.hxx"

#define MODULE_NAME L"gui"
extern "C"
{
	#include "gw_gui.h"
}

using namespace types;

int GuiModule::Load()
{
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"x_dialog", &sci_x_dialog, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"x_choose", &sci_x_choose, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"x_mdialog", &sci_x_mdialog, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"xchoicesi", &sci_x_choice, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"delmenu", &sci_delmenu, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"setmenu", &sci_setmenu, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"unsetmenu", &sci_unsetmenu, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"raise_window", &sci_raise_window, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"getlookandfeel", &sci_getlookandfeel, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"getinstalledlookandfeels", &sci_getinstalledlookandfeels, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"setlookandfeel", &sci_setlookandfeel, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"ClipBoard", &sci_ClipBoard, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"toolbar", &sci_toolbar, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uigetdir", &sci_uigetdir, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uicontrol", &sci_uicontrol, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uimenu", &sci_uimenu, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uipopup", &sci_mpopup, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"x_choose_modeless", &sci_x_choose_modeless, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uicontextmenu", &sci_uicontextmenu, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uiwait", &sci_uiwait, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"messagebox", &sci_messagebox, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"waitbar", &sci_waitbar, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"progressionbar", &sci_progressionbar, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"helpbrowser", &sci_helpbrowser, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uigetfont", &sci_uigetfont, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uigetcolor", &sci_uigetcolor, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"getcallbackobject", &sci_getcallbackobject, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"printfigure", &sci_printfigure, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"exportUI", &sci_exportUI, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"printsetupbox", &sci_printsetupbox, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"toprint", &sci_toprint, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uigetfile", &sci_uigetfile, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"usecanvas", &sci_usecanvas, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uiDisplayTree", &sci_displaytree, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uiputfile", &sci_uiputfile, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"about", &sci_about, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"fire_closing_finished", &sci_fire_closing_finished, MODULE_NAME));
    return 1;
}
