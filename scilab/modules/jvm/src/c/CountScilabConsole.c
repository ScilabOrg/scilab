/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include "localization.h"
#include "version.h"

static Display *display = NULL;

static int countScilabConsole(Window win);
static int checkIsConsole(const char *name);

int getNumberOfScilabConsole(void)
{
    Window root;

    if (!display)
    {
        display = XOpenDisplay(NULL);
        if (!display)
        {
            return 0;
        }
    }

    int screen = XDefaultScreen(display);
    root = RootWindow(display, screen);

    return countScilabConsole(root);
}

int countScilabConsole(Window win)
{
    Window root_win, parent_win, *child_list;
    XTextProperty tp;
    unsigned int num_children;
    int num_scilab = 0;

    if (XGetWMName(display, win, &tp) && tp.nitems > 0)
    {
        int count = 0, ret;
        char **list = NULL;
        ret = XmbTextPropertyToTextList(display, &tp, &list, &count);
        if ((ret == Success) && list)
        {
            int j = 0;
            for (; j < count; j++)
            {
                num_scilab += checkIsConsole(list[j]);
            }
            XFreeStringList(list);
        }
        else
        {
            num_scilab += checkIsConsole(tp.value);
        }
    }

    if (XQueryTree(display, win, &root_win, &parent_win, &child_list, &num_children) && child_list && num_children > 0)
    {
        unsigned int i = 0;
        for (; i < num_children; i++)
        {
            num_scilab += countScilabConsole(child_list[i]);
        }
        XFree(child_list);
    }

    return num_scilab;
}

int checkIsConsole(const char *name)
{
    static char *SCILAB_CONSOLE = NULL;
    if (!SCILAB_CONSOLE)
    {
        SCILAB_CONSOLE = _("Scilab Console");
    }

    if (strstr(name, SCILAB_CONSOLE) || strstr(name, SCI_VERSION_STRING))
    {
        return 1;
    }

    return 0;
}
