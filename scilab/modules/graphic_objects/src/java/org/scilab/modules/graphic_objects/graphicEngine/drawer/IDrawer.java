/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.drawer;

import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;

/**
 * Author: Lando Pierre
 * Date: 23 mars 2010
 *
 * Interface of shape drawer
 */
public interface IDrawer <nodeType extends IDrawable, renderingInfoType extends IRenderingInfo> {

    /**
     * @return the type of object drawn by this drawer.
     */
    String getType();

    /**
     * Begin drawing of the given shape.
     * @param shape to render.
     * @param renderingInfo help to perform.
     */
    void beginDraw(nodeType shape, renderingInfoType renderingInfo);


    /**
     * End drawing of the given shape.
     * @param shape to render.
     * @param renderingInfo help to perform.
     */
    void endDraw(nodeType shape, renderingInfoType renderingInfo);
}
