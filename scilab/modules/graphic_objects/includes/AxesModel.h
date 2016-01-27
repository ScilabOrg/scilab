/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */
#ifndef __AXES_MODEL_H__
#define __AXES_MODEL_H__

#include "dynlib_graphic_objects.h"
#include "BOOL.h"

GRAPHIC_OBJECTS_IMPEXP int getAxesModel(void);

GRAPHIC_OBJECTS_IMPEXP void setAxesModel(int iUID);

GRAPHIC_OBJECTS_IMPEXP BOOL isAxesModel(int iUID);

#endif /* !__AXES_MODEL_H__ */
