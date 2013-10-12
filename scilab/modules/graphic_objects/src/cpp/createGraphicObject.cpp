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

int createGraphicObject(int _iType)
{
    return CallGraphicController::askGraphicObject(getScilabJavaVM(), _iType);
}

int cloneGraphicObject(int _sId)
{
    return CallGraphicController::cloneGraphicObject(getScilabJavaVM(), _sId);
}

int createDataObject(int _sId, int _iType)
{
    return DataController::createDataObject(_sId, _iType);
}

void buildFigureMenuBar(int _sFigureId)
{
    CallGraphicController::buildFigureMenuBar(getScilabJavaVM(), _sFigureId);
}

int buildRect(int iParentsubwinUID, double x, double y, double height, double width, int foreground, int background, int isfilled, int isline)
{
    return Builder::buildRect(getScilabJavaVM(), iParentsubwinUID, x, y, height, width, foreground, background, isfilled, isline);
}

int cloneGraphicContext(int sourceIdentifier, int destIdentifier)
{
    return Builder::cloneGraphicContext(getScilabJavaVM(), sourceIdentifier, destIdentifier);
}

int cloneFontContext(int sourceIdentifier, int destIdentifier)
{
    return Builder::cloneFontContext(getScilabJavaVM(), sourceIdentifier, destIdentifier);
}

int createHiddenLabel(int parent)
{
    return Builder::createHiddenLabel(getScilabJavaVM(), parent);
}

BOOL isAxesRedrawing(int subWin)
{
    return booltoBOOL(Builder::isAxesRedrawing(getScilabJavaVM(), subWin));
}

int createLabel(int parent, int type)
{
    return Builder::createLabel(getScilabJavaVM(), parent, type);
}

int createNewFigureWithAxes()
{
    return Builder::createNewFigureWithAxes(getScilabJavaVM());
}

void cloneMenus(int model, int newParent)
{
    Builder::cloneMenus(getScilabJavaVM(), model, newParent);
}

int cloneAxesModel(int parent)
{
    return Builder::cloneAxesModel(getScilabJavaVM(), parent);
}

int createSubWin(int parent)
{
    return Builder::createSubWin(getScilabJavaVM(), parent);
}

int allocateText(int iParentsubwinUID, char** text, int nbRow, int nbCol, double x, double y, BOOL autoSize, double* userSize, int  centerPos, int *foreground, int *background, BOOL isboxed, BOOL isline, BOOL isfilled, int align)
{
    return Builder::allocateText(getScilabJavaVM(), iParentsubwinUID, text, nbRow * nbCol, nbRow, nbCol, x, y, autoSize == TRUE,
                                 userSize, userSize == NULL ? 0 : 2,
                                 centerPos,
                                 foreground != NULL ? *foreground : 0, foreground != NULL,
                                 background != NULL ? *background : 0, background != NULL,
                                 isboxed == TRUE, isline == TRUE, isfilled == TRUE, align);
}