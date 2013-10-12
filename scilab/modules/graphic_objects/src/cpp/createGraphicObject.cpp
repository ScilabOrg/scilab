/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include "createGraphicObject.h"
#include "getScilabJavaVM.h"
}

#include "CallGraphicController.hxx"
#include "Builder.hxx"
#include "DataController.hxx"

using namespace org_scilab_modules_graphic_objects_builder;
using namespace org_scilab_modules_graphic_objects;

char *createGraphicObject(int _iType)
{
    return CallGraphicController::askGraphicObject(getScilabJavaVM(), _iType);
}

char *cloneGraphicObject(char const* _sId)
{
    return CallGraphicController::cloneGraphicObject(getScilabJavaVM(), _sId);
}

char const* createDataObject(char const* _sId, int _iType)
{
    return DataController::createDataObject(_sId, _iType);
}

void buildFigureMenuBar(char const* _sFigureId)
{
    CallGraphicController::buildFigureMenuBar(getScilabJavaVM(), _sFigureId);
}

char* buildRect(char * pparentsubwinUID, double x, double y, double height, double width, int foreground, int background, int isfilled, int isline)
{
    return Builder::buildRect(getScilabJavaVM(), pparentsubwinUID, x, y, height, width, foreground, background, isfilled, isline);
}

int cloneGraphicContext(char const* sourceIdentifier, char const* destIdentifier)
{
    return Builder::cloneGraphicContext(getScilabJavaVM(), sourceIdentifier, destIdentifier);
}

int cloneFontContext(char const* sourceIdentifier, char const* destIdentifier)
{
    return Builder::cloneFontContext(getScilabJavaVM(), sourceIdentifier, destIdentifier);
}

char* createHiddenLabel(char const* parent)
{
    return Builder::createHiddenLabel(getScilabJavaVM(), parent);
}

BOOL isAxesRedrawing(char const* subWin)
{
    return booltoBOOL(Builder::isAxesRedrawing(getScilabJavaVM(), subWin));
}

int createLabel(char const* parent, int type)
{
    return Builder::createLabel(getScilabJavaVM(), parent, type);
}

char* createNewFigureWithAxes()
{
    return Builder::createNewFigureWithAxes(getScilabJavaVM());
}

void cloneMenus(char const* model, char const* newParent)
{
    Builder::cloneMenus(getScilabJavaVM(), model, newParent);
}

void cloneAxesModel(char const* parent)
{
    Builder::cloneAxesModel(getScilabJavaVM(), parent);

}
