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
 * Basic implementation of IRenderingInfo.
 */
public class RenderingInfo<drawerType extends IDrawer> implements IRenderingInfo<drawerType> {
    private RenderingMode renderingMode = null;
    private int index = 0;
    private int frameWidth;
    private int frameHeight;
    private IDrawersManager<drawerType> drawersManager;

    public RenderingInfo(IDrawersManager<drawerType> drawersManager) {
        this.drawersManager = drawersManager;
    }

    
    public void setRenderingMode(RenderingMode renderingMode) {
        this.renderingMode = renderingMode;
    }

    
    public RenderingMode getRenderingMode() {
        return renderingMode;
    }

    
    public void setFrameWidth(int frameWidth) {
        this.frameWidth = frameWidth;
    }

    
    public int getFrameWidth() {
        return frameWidth;
    }

    
    public void setFrameHeight(int frameHeight) {
        this.frameHeight = frameHeight;
    }

    
    public int getFrameHeight() {
        return frameHeight;
    }

    
    public IDrawersManager<drawerType> getDrawersManager() {
        return drawersManager;
    }
}
