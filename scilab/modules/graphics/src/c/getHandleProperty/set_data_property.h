/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SET_DATA_PROPERTY_H__
#define __SET_DATA_PROPERTY_H__

#include "getPropertyAssignedValue.h"

/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the champ data is now set as a tlist (like for surface objects) */
/* setchampdata(pobj,cstk(l2), &l3, &numrow3, &numcol3, fname) */
int setchampdata(void* _pvCtx, int iObjUID, AssignedList * tlist);

/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the grayplot data is now set as a tlist (like for surface and champ objects) */
/* setgrayplot(pobj,cstk(l2), &l3, &numrow3, &numcol3, fname) */
int setgrayplotdata(void* _pvCtx, int iObjUID, AssignedList * tlist);

/*--------------------------------------------------------------------------*/
/* set3ddata(pobj,cstk(l2), &l3, &numrow3, &numcol3) */
int set3ddata(void* _pvCtx, int iObjUID, AssignedList * tlist);

int set_data_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol);

#endif // __SET_DATA_PROPERTY_H__
