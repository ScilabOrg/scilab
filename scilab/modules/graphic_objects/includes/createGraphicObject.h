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

#ifndef __CREATEGRAPHICOBJECT_H__
#define __CREATEGRAPHICOBJECT_H__

#include "dynlib_graphic_objects.h"
#include "BOOL.h"

/*
** Ask Controller to create a graphic object
** of the given _sType.
*/
GRAPHIC_OBJECTS_IMPEXP char *createGraphicObject(int _iType);

/*
** Ask Controller to clone a graphic object
** referenced the given _sId.
*/
GRAPHIC_OBJECTS_IMPEXP char *cloneGraphicObject(char const* _sId);

/*
** Ask Controller to create a data object
** referenced by the given _sId and of the given _sType
*/
GRAPHIC_OBJECTS_IMPEXP char const* createDataObject(char const* _sId, int _iType);

/*
** Ask Controller to create a figure menubar
** referenced by the given _sFigureId and of the given _sType
*/
GRAPHIC_OBJECTS_IMPEXP void buildFigureMenuBar(char const* _sFigureId);

GRAPHIC_OBJECTS_IMPEXP char * buildRect(char * pparentsubwinUID, double x, double y, double height, double width, int foreground, int background, int isfilled, int isline);
GRAPHIC_OBJECTS_IMPEXP int cloneGraphicContext(char const* sourceIdentifier, char const* destIdentifier);
GRAPHIC_OBJECTS_IMPEXP int cloneFontContext(char const* sourceIdentifier, char const* destIdentifier);

GRAPHIC_OBJECTS_IMPEXP char* createHiddenLabel(char const* parent);
GRAPHIC_OBJECTS_IMPEXP BOOL isAxesRedrawing(char const* subWin);
GRAPHIC_OBJECTS_IMPEXP int createLabel(char const* parent, int type);
GRAPHIC_OBJECTS_IMPEXP char* createNewFigureWithAxes();
GRAPHIC_OBJECTS_IMPEXP void cloneMenus(char const* model, char const* newParent);
GRAPHIC_OBJECTS_IMPEXP void cloneAxesModel(char const* parent);
#endif /* !__CREATEGRAPHICOBJECT_H__ */
