/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw a
 * segs object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableSegsJoGL.hxx"
#include "DrawableSegs.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
    DrawableSegsJoGL::DrawableSegsJoGL(DrawableSegs * drawer):DrawableClippedObjectJoGL(drawer)
    {

    }
/*---------------------------------------------------------------------------------*/
    DrawableSegsJoGL::~DrawableSegsJoGL(void)
    {

    }
/*---------------------------------------------------------------------------------*/
    DrawableSegs *DrawableSegsJoGL::getSegsDrawer(void)
    {
        return dynamic_cast < DrawableSegs * >(getDrawer());
    }
/*---------------------------------------------------------------------------------*/
    DrawableSegsJavaMapper *DrawableSegsJoGL::getSegsJavaMapper(void)
    {
        return dynamic_cast < DrawableSegsJavaMapper * >(getJavaMapper());
    }
/*---------------------------------------------------------------------------------*/
}
