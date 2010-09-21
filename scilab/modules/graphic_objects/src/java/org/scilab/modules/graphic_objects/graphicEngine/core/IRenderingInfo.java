/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.core;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.IDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.IDrawersManager;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 *
 * Interface for rendering info, rendering info contains all necessary information to perform a drawing.
 */
public interface IRenderingInfo<drawerType extends IDrawer> {
    enum RenderingMode {
        NORMAL_MODE,
        SELECT_MODE
    }
    
    /**
     * @param renderingMode is the new rendering mode.
     */
    void setRenderingMode(RenderingMode renderingMode);

    /**
     * @return the rendering mode.
     */
    RenderingMode getRenderingMode();

    /**
     * @param frameWidth is the new frame width.
     */
    void setFrameWidth(int frameWidth);

    /**
     * @return the width of the frame.
     */
    int getFrameWidth();

    /**
     * @param frameHeight is the new frame width.
     */
    void setFrameHeight(int frameHeight);

    /**
     * @return the height of the frame.
     */
    int getFrameHeight();

    /**
     * @return the drawers manager.
     */
    IDrawersManager<drawerType> getDrawersManager();
}
