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

#ifndef __CREATEGRAPHICOBJECT_H__
#define __CREATEGRAPHICOBJECT_H__

#include "dynlib_graphic_objects.h"

/*
** Ask Controler to create a graphic object
** of the given _sType.
*/
GRAPHIC_OBJECTS_IMPEXP char *createGraphicObject(char *_sType);

#endif /* !__CREATEGRAPHICOBJECT_H__ */
