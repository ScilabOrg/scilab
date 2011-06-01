/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __FIGURE_LIST_H__
#define __FIGURE_LIST_H__

#include "dynlib_graphic_objects.h"

GRAPHIC_OBJECTS_IMPEXP int  __sciGetNbFigure(void);
GRAPHIC_OBJECTS_IMPEXP void __sciGetFiguresId(int ids[]);

GRAPHIC_OBJECTS_IMPEXP void __registerToController();

#endif /* !__FIGURE_LIST_H__ */
