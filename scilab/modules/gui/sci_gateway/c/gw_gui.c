/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <stdio.h>
#include <string.h>
#include "gw_gui.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "scilabmode.h"
#include "callFunctionFromGateway.h"
#include "localization.h"
#include "Scierror.h"
#include "loadOnUseClassPath.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_x_dialog, "x_dialog"},
    {sci_x_choose, "x_choose"},
    {sci_x_mdialog, "x_mdialog"},
    {sci_x_choice, "xchoicesi"},
    {sci_delmenu, "delmenu"},
    {sci_setmenu, "setmenu"},
    {sci_unsetmenu, "unsetmenu"},
    {sci_raise_window, "raise_window"},
    {sci_getlookandfeel, "getlookandfeel"},
    {sci_getinstalledlookandfeels, "getinstalledlookandfeels"},
    {sci_setlookandfeel, "setlookandfeel"},
    {sci_ClipBoard, "ClipBoard"},
    {sci_toolbar, "toolbar"},
    {sci_uigetdir, "uigetdir"},
    {sci_uicontrol, "uicontrol"},
    {sci_uimenu, "uimenu"},
    {sci_mpopup, "uipopup"},
    {sci_x_choose_modeless, "x_choose_modeless"},
    {sci_uicontextmenu, "uicontextmenu"},
    {sci_uiwait, "uiwait"},
    {sci_messagebox, "messagebox"},
    {sci_waitbar, "waitbar"},
    {sci_progressionbar, "progressionbar"},
    {sci_helpbrowser, "helpbrowser"},
    {sci_uigetfont, "uigetfont"},
    {sci_uigetcolor, "uigetcolor"},
    {sci_getcallbackobject, "getcallbackobject"},
    {sci_printfigure, "printfigure"},
    {sci_exportUI, "exportUI"},
    {sci_printsetupbox, "printsetupbox"},
    {sci_toprint, "toprint"},
    {sci_uigetfile, "uigetfile"},
    {sci_usecanvas, "usecanvas"},
    {sci_displaytree, "uiDisplayTree"},
    {sci_uiputfile, "uiputfile"},
    {sci_about, "about"},
    {sci_useeditor, "useeditor"},
    {sci_fire_closing_finished, "fire_closing_finished"},
    {sci_openged, "openged"},
    {sci_datatipcreate, "datatipCreate"},
    {sci_datatipremove, "datatipRemove"},
    {sci_plotbrowser, "plotbrowser"},
    {sci_datatip_manager_mode, "datatipManagerMode"},
    {sci_datatip_toggle, "datatipToggle"},
    {sci_datatipmove, "datatipMove"},
    {sci_datatip_set_style, "datatipSetStyle"},
    {sci_datatip_set_display, "datatipSetDisplay"},
    {sci_datatip_set_interp, "datatipSetInterp"},
    {sci_datatip_set_orient, "datatipSetOrientation"},
    {sci_figure, "figure"},
    {sci_saveGui, "saveGui"},
    {sci_loadGui, "loadGui"}
};
/*--------------------------------------------------------------------------*/
int gw_gui(void)
{
    Rhs = Max(0, Rhs);

    if ( getScilabMode() == SCILAB_NWNI)
    {
        Scierror(999, _("Scilab '%s' module disabled in -nogui or -nwni mode.\n"), "GUI");
        return 0;
    }

    /**
     * We have to load the 'graphic' jars for
     * - uicontrol
     * - uimenu
     * because these functions need to create a figure to put the created object inside.
     *
     * **** TODO create Scilab figures without a canvas inside and remove this crappy test. ****
     *
     */
    if (!loadedDep && (strcmp(Tab[Fin - 1].name, "uicontrol") == 0
                       || strcmp(Tab[Fin - 1].name, "uimenu") == 0
                       || strcmp(Tab[Fin - 1].name, "usecanvas") == 0
                       || strcmp(Tab[Fin - 1].name, "loadGui") == 0
                       || strcmp(Tab[Fin - 1].name, "figure") == 0))
    {
        loadOnUseClassPath("graphics");
        loadedDep = TRUE;
    }


    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));

    return 0;
}
/*--------------------------------------------------------------------------*/
